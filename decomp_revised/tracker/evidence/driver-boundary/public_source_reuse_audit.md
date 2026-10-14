# Public Genesis3D source as a reverse-engineering reference

Checked 2026-10-13. This is an evidence audit for the FDNY decompilation and
port. Public source is being used only to form/test hypotheses. No public code
was copied into the playable port, and no claim is made that public source is
the source of the shipped FDNY binaries.

## Finding

Public Genesis3D material is useful, particularly for naming standard driver
interface fields and understanding likely engine/driver responsibilities.
It materially narrows some questions, but it does not replace decompilation of
the supplied `Genesis.dll`, `D3DDrv.dll`, game EXE, or game data. The best
available public code inspected here is a later, community-modified Genesis3D
1.1 tree; its own driver ABI says 100.3, while the FDNY driver returns 101.1.
That mismatch precludes treating it as an interchangeable implementation.

## Public source identity and limits

- The [official Genesis3D SDK manifest](https://genesis3d.com/old/docs/Manifest.htm)
  says the SDK included complete engine/tool source under `OpenSource`, and
  separately lists `genesis.dll` and `d3ddrv.dll` binaries. The page describes
  the package layout; it does not authenticate a particular downloaded SDK
  archive or show that the FDNY DLLs came from it.
- The [RealityFactory/Genesis3D repository](https://github.com/RealityFactory/Genesis3D)
  identifies itself as based on Genesis3D 1.1 (1999-11-15) with contributions
  from various authors. Its [driver directory](https://github.com/RealityFactory/Genesis3D/tree/master/Engine/Drivers)
  contains `D3DDrv`, `D3D7xDrv`, OpenGL, and WireFrame drivers plus shared
  `Dcommon.h`. The inspected URLs use mutable `master`; this audit did not
  retrieve or pin a commit SHA or inspect the complete D3DDrv implementation.
- The [shared Dcommon.h](https://raw.githubusercontent.com/RealityFactory/Genesis3D/master/Engine/Drivers/Dcommon.h)
  has Genesis3D Public License 1.01 notices, identifies the 1.1 release, and
  contains edits dated 2003–2004, including bump mapping and shadow additions.
  This is therefore a later community header, not an untouched 1999 header.
  The current [official site](https://www.genesis3d.com/) states Genesis3D 1.1
  can be used without licensing obligations; that statement does not establish
  provenance or terms for each community contribution. This is not a license
  clearance for reuse.
- The [official driver documentation](https://genesis3d.com/old/docs/Drivers.htm)
  describes D3D6-era rendering and lightmap support. This supports the broad
  technology context, not exact behavior or binary identity.

## Concrete cross-check against the pinned FDNY binaries

Local binary evidence is recorded in
[`public_driver_comparison.md`](public_driver_comparison.md),
[`d3ddrv_driverhook_boundary.md`](d3ddrv_driverhook_boundary.md),
[`selected_driver_initialization_trace.md`](selected_driver_initialization_trace.md),
and [`d3ddrv_render_path.md`](../../runtime/d3ddrv_render_path.md).

| Question | Public-source evidence | Pinned FDNY evidence | What it changes |
|---|---|---|---|
| Hook and initial table contract | `Dcommon.h` defines `DRV_Hook(DRV_Driver **)`, `_fastcall` callbacks, and table version 100.3. Its initial table fields name `+0x14` subdriver enumeration, `+0x18` mode enumeration, `+0x1C` pixel-format enumeration, and `+0x20` initialization. | FDNY `D3DDrv.dll` exports ordinal 1 `DriverHook`; it returns version 101.1. Genesis calls `+0x14`, `+0x18`, and `+0x20`; local disassembly independently shows enumeration, mode callbacks, and initialization. | The common hook shape and those first four field roles now have good comparative support. Version mismatch and incomplete field-by-field layout still matter; later offsets/callback signatures cannot be filled from the public header by assumption. |
| Texture and lightmap contract | Header gives names/types for texture create/destroy/lock/unlock, texture info, separate alpha/palette operations, `DRV_TexInfo`, `DRV_LInfo`, and `RENDER_W_POLY`; it explicitly includes a combine-lightmap capability flag. | FDNY table has texture create/destroy/lock/unlock, textured submissions, and fog slots; raw callbacks and DirectDraw paths are traced. Genesis GBSP and bitmap paths are traced, but the exact BSP face → driver call and lightmap/alpha joins are unresolved. | Useful as a checklist and vocabulary for targeted xrefs: follow real Genesis callsites and compare exact structures/flags with local callbacks. Do not treat public field names or flag bits as FDNY-confirmed until offsets and runtime values agree. |
| Renderer generation | Repository includes separate `D3DDrv` and `D3D7xDrv`; public docs refer to D3D6-era driver. | FDNY imports `DirectDrawCreate` and `DirectDrawEnumerateA`; `D3DDrv.dll` build string/date is Mar 4, 2002, with 101.1 table version. | `D3DDrv` is the relevant comparison candidate; D3D7x is a different generation. No matching build identity has been found. |
| Engine-side resource semantics | Public SDK docs/source can guide likely Genesis resource flow and expected data structures. | Pinned `Genesis.dll` itself shows GBSP v15 loading, bitmap registration, name lookup, texture surface dispatch, collision, and actor-material registration. | The supplied engine binary remains the primary authority. Public source is most valuable for choosing the next xrefs and interpreting corroborated dataflow, not replacing it. |

## Useful next checks

1. Pin the RealityFactory repository to a full commit SHA and inspect its
   `D3DDrv` source, shared header, and project configuration together. Record
   the exact commit and file hashes before using any source detail as evidence.
2. Reconstruct the FDNY 101.1 table from the bytes at `0x1001F040`, then
   compare each public 100.3 field by **offset, calling convention, argument
   dataflow, and caller**. Public `+0x14/+0x18/+0x1C/+0x20` are plausible
   anchors; do not extrapolate beyond them.
3. On the pinned Genesis binary, trace world-face submission through lightmap
   setup into D3DDrv `+0x7C/+0x80/+0x84`, including raw flags, dimensions,
   vertex alpha, and the second texture/lightmap handle. Compare those facts
   with public structure/flag descriptions and the FDNY callsites.
4. Resolve the existing open points in
   [`surface_material_alpha_join.md`](../../runtime/surface_material_alpha_join.md)
   and [`genesis_render_asset_path.md`](../../runtime/genesis_render_asset_path.md)
   from FDNY EXE/Genesis/D3DDrv evidence before adjusting the port renderer.
5. If a locally available original Genesis3D SDK archive is found, hash it and
   compare its `OpenSource` driver/header to both the community tree and FDNY.
   Do not infer that SDK's availability, provenance, or terms from the manifest
   alone.

## Practical reuse decision

Use the public source as a **reference for architecture, terminology, and
candidate structure layouts**. Specifically, it has already made the initial
driver enumeration/init slots much less ambiguous and offers a targeted
lightmap/alpha/render checklist for the next binary pass. Keep all port code
independently derived from pinned FDNY binary and game-data evidence; do not
copy public renderer code or treat public source behavior as the desired FDNY
behavior. No gameplay change follows from this research alone.
