# Genesis GBSP face lightdata decode

## Provenance

- Pinned `inputs/original/binaries/Genesis.dll`, SHA-256 `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`; checked PE32 x86 instructions with `objdump -d -Mintel`.
- Style-sequence call sites were checked in a separate read-only Ghidra 12.0.4 headless project at `/private/tmp/fdny-genesis-render-audit/style`; the same pinned Genesis.dll hash was imported.
- Parsed the copied retail maps under `inputs/original/Data/Maps/*.bsp` read-only. Face fields use the existing `<8i4B` decoder in [`tools/export_gbsp_scene.py`](../../../tools/export_gbsp_scene.py) and 36-byte FACES record size in [`tools/gbsp_v15.py`](../../../tools/gbsp_v15.py). This note records source and data analysis; it does not validate host renderer parity.

## Plain-language explanation

Genesis stores a short marker before each face's RGB light samples. It scales each sample by that face's active light-style value, adds extra style layers, clamps the result to a byte, then creates and caches a bitmap for the face. The face descriptor and RGB update callback trace through Genesis's direct/deferred draw path into D3DDrv. The ordinary D3D paths are now mapped: one draws the base then multiplies it by the lightmap, while the other multiplies both textures in two stages when the device supports more than one simultaneous texture. Genesis advances each style string once per `geEngine_RenderWorld` call; the host's render-call rate and full pixel parity remain open.

## Verified sample layout

`FUN_10033d60` takes a small face-light record: signed 16-bit width/height at `+0/+2`, and face index at `+20` (`0x10033d71–0x10033d99`). It uses that index to locate the 36-byte GBSP FACES record (`index * 36`, `0x10033d9f–0x10033daf`), reads its `LightOfs` at `+20` (`0x10033dbd`), then addresses the loaded LIGHTDATA buffer at `LightOfs + 1` (`0x10033e0c–0x10033e18`). The four style bytes come from FACES `+32..+35` (`0x10033e60–0x10033e69`). The pixel count is `width * height` (`0x10033d99–0x10033d9d`).

For each active style slot, the sample helper consumes three input bytes per pixel and advances source by `3 * width * height` before the next slot (`0x10033ef0–0x10033efa`). The corresponding byte helpers are exact about the packed layout: `FUN_100328e0` writes each consecutive input triplet to three 32-bit channel accumulators (`0x100328e0–0x1003291b`); `FUN_10032850` consumes the same three-byte stride and accumulates into the same three channels (`0x10032850–0x10032899`). There is no row padding or channel permutation in these routines.

The `+1` is not guessed: each valid face offset in all 17 copied map BSPs points at a byte `0x01`. Sorting each map’s valid light offsets, every next offset equals `current LightOfs + 1 + 3*width*height*activeStyleCount`; every record stays in LIGHTDATA bounds and styles are contiguous before `255`. This matches all records in the supplied maps. `bank.bsp` has 316 two-style faces (styles `(0,1)`); `bar_r.bsp` has 120 `(0,1)`, 77 `(0,2)`, and 87 `(0,1,2)` faces. The other 15 supplied maps use only style 0. No supplied BSP face uses style ID 15.

## Verified per-style arithmetic

For ordinary nonzero styles used by these maps, `FUN_10033D60` gets the
integer scale from `[0x10361f1c + 96 + 4*styleId]`
(`0x10033e74–0x10033e8f`). A face plane whose style ID is 0 takes the special
branch to `FUN_100328a0` and copies each RGB byte into the accumulator shifted
left by 8; it does not use a style coefficient. For nonzero IDs, plane slot 0
initializes the accumulator with `sampleByte * scale`, while slots 1–3 add
that product through `FUN_10032850`. The tail at `0x10033fb0–0x1003400a`
arithmetic-shifts each signed accumulator right by 8, clamps it to `[0,255]`,
and writes the three resulting bytes consecutively. This is Q8 scaling with
truncation via `sar`, followed by saturation; it is not a floating-point
blend.

`FUN_10032e30` (`0x10032e30–0x10032ee8`) supplies animated style coefficients: signed current style character minus 96, converted to float, multiplied by the binary’s `1/27` and `256` constants, then passed to its float-to-integer helper and stored per style. This establishes the coefficient conversion, but not update cadence or real-time units. Style ID 15 follows separate table-lookup helpers (`FUN_100331b0` for slot 0; `FUN_10033270` for later slots), rather than the ordinary multiply path; it is absent from the supplied face records.

## Verified face projection and lightmap origin

`FUN_10035110` allocates the runtime face table at world-data `+0x1c4`, with 136-byte records, then calls `FUN_10034650` and `FUN_10034750` (`0x10035160–0x100351b5`). `FUN_10034650` follows each FACES texinfo index to a 64-byte TEXINFO record and caches each face vertex's two projected coordinates using only the first six vector floats: `U = dot(vertex, TEXINFO[0..2])` and `V = dot(vertex, TEXINFO[3..5])` (`0x10034680–0x1003470c`). These cached projections do not include TEXINFO Shift.

`FUN_10034750` initializes the runtime descriptor and finds projected minima and maxima. It floors each minimum after multiplying by `1/16` (`0x10054b1c`, `0x10034bf0–0x10034c2a`), then multiplies that snapped value by 16 (`0x10054b50`, `0x10034c4b–0x10034c69`) and stores it at descriptor `+4/+8`. The resulting origin is exactly `floor(minProjected / 16) * 16`, in the same unshifted projection units as the vertex stream. GBSP `light_width/light_height` at `+24/+28` are copied to descriptor `+0/+2` (`0x10034c5b–0x10034c65`). `FUN_100440a8` is a float-to-integer conversion that adjusts the x87 nearest-integer result toward zero for ordinary finite inputs; here it receives the already-grid-aligned integer-valued minimum.

The point-query path in `FUN_10033510` adds TEXINFO Shift at `+24/+28` before subtracting descriptor `+4/+8`, then converts to the 16-unit sample grid. This joins the descriptor writer to the query consumer and confirms that Shift is added at query time. The renderer's separate per-vertex coordinate path is traced in the [face-to-driver handoff](genesis_face_lightmap_driver_handoff_2026-10-15.md).

## Point-to-light-sample mapping

`FUN_10033510` is a light-query sampler, not proven to be the renderer's vertex-UV builder. It selects a 64-byte record from the world array at `+140` using the face descriptor's `+92` index, dots the supplied 3D point against its first two 3D vectors, adds the two values at texinfo offsets `+24/+28`, then subtracts the face descriptor minima at `+4/+8` (`0x10033536–0x100335bb`). It rejects projected positions outside the face light dimensions in 16-unit steps (`width<<4`, `height<<4`, `0x100335e9–0x10033622`), multiplies S/T by the pinned float at `0x10054b1c` (`1/16`), and converts the resulting grid coordinates with `0x100440a8` (`0x10033628–0x100336b6`). It indexes samples as `y*width+x`, then reads the same three consecutive bytes (`0x100336b6–0x100336c4`, `0x100336d8–0x100336f8`). This independently supports row-major, tightly packed triplets and 16 world units per light sample for the point-query path. Exact vertex interpolation/filtering remains open.

## Rendering connection and limits

`FUN_10025770` skips runtime face entries whose stored GBSP face index at `+20` is `-1`, whose flag byte at `+132` lacks bit `0x04`, or whose cached bitmap at `+24` already exists (`0x100257b2–0x100257d3`). `FUN_10033d60` then resolves the GBSP face record and its actual `LightOfs`. The bitmap-create path calls the selected driver table at `+0x38` (the table's texture-create slot) through `0x10024e90`, and stores the returned handle at runtime face `+24` (`0x100257d5–0x100257fb`; the `+0x38` target is recorded in [`d3ddrv_render_path.md`](d3ddrv_render_path.md)). This proves Genesis builds and caches per-face lightmap bitmaps and hands the generated triplet buffer to the driver texture-create path.

The binary transform preserves the input channel order and emits 3 bytes per sample; the channel convention is not named. The follow-up [face-to-driver handoff trace](genesis_face_lightmap_driver_handoff_2026-10-15.md) verifies that the 136-byte face descriptor reaches D3DDrv `+0x80`; D3DDrv calls Genesis `FUN_10033D60` through its installed `+0xB0` callback, which writes the converted RGB buffer pointer into descriptor `+12` and returns an update flag. The same raw projected pair is used for base UV conversion and lightmap UV calculation; the formula is in the handoff trace. The Genesis-to-D3DDrv trace now establishes row-preserving sample upload and positive V progression through those source rows, though row zero has no independent world-space top/bottom label. The handoff also maps ordinary two-pass and two-stage lightmap composition, linear minification/magnification filters, clamped lightmap addressing, Z writes, and the Z comparison. Genesis advances each animated style once per `geEngine_RenderWorld` call; the host render-call rate remains unknown. Effective Z-enable, mip filtering, runtime path selection, channel convention, and full rendered parity remain open.
