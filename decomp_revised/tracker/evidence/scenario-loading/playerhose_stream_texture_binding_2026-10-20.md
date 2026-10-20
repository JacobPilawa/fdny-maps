# PlayerHose stream resource-3 texture binding

This note records the static retail resource path and the current shared-host
rendering step for dedicated PlayerHose. The evidence establishes the authored
bitmap pair and strip UV inputs; it does not establish full pixel parity.

## Hash-pinned source and authored data

- Retail executable: `inputs/original/binaries/FDNYFirefighter.exe`
- Retail executable SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
- Resource definitions: `inputs/original/Data/ResDefs/hose.rrf`
- `hose.rrf` SHA-256: `93a1b2dae7d755fdd66fabaee89514de8e420cfb62f2f96132b341c0cce6fe45`
- Resource 3 entry: `ROC_ALPHA_TEXTURE 3 "data\sprites\hdstream.bmp" "data\sprites\hdstream_a.bmp";`
- Resource 5 is a separate unpaired entry for `hosewater.bmp`.

The copied color bitmap `hdstream.bmp` is a 64x64 indexed BMP with SHA-256
`43dfba5607fdd997d5d517a6054bfff3a310735ad67825380d5320e25e70d2a6`. The
paired `hdstream_a.bmp` is a 128x64 24-bit BMP with SHA-256
`d6309efdb5f2f37cf148f805152073fe223fc4e1f84aa99f1ec1a39b777c0872`. The
exporter copies both files verbatim into the ignored local actor-assets bundle
and writes a manifest containing the input and output hashes.

## Retail lookup and strip texture coordinates

The relevant Ghidra excerpts were produced against the executable hash above:

- [`playerhose_stream_component_consumer_fresh_2026-10-20.c`](../fire-spread/playerhose_stream_component_consumer_fresh_2026-10-20.c)
  includes constructor `0046D6B0` and strip consumer `0046C5A0`.
- [`playerhose_strip_construction_fresh_2026-10-20.c`](../fire-spread/playerhose_strip_construction_fresh_2026-10-20.c)
  includes constructor calls and the resource lookup wrapper `00493D00`.
- [`playerhose_strip_texture_lookup_fresh_2026-10-20.c`](../fire-spread/playerhose_strip_texture_lookup_fresh_2026-10-20.c)
  contains the numeric resource-table search at `00493650`.

Constructor `0046D6B0` asks the selected renderer manager for resource ID 3 via
`00493D00`; that wrapper chooses one of two manager resource arrays and calls
`00493650` to find the numeric ID. It creates each strip polygon with four
vertices, the returned texture pointer, flag `0x20`, and alpha argument `1.0`.
The constructor initializes all four vertex color channels to 255, sets the U
span at component `+0x2E8` to float32 `0.15`, and starts the U offset at zero.
The sample interval and scroll arithmetic are in `0046C5A0`: the offset is
decremented by `M_DeltaTime * 6` each consumer update.

The current UV values in source strip-vertex order are:

| Vertex | Strip point | U | V |
| --- | --- | --- | --- |
| 0 | Previous main curve | `u` | `0` |
| 1 | Previous secondary curve | `u + 0.15` | `0` |
| 2 | Current secondary curve | `u` | `0.9` |
| 3 | Current main curve | `u + 0.15` | `0.9` |

Here `u` starts at zero and decreases by six times the source frame delta. The
vertex positions remain in source order: previous main, previous secondary,
current secondary, current main. This mapping comes from writes to the four
LVertex UV fields in `0046C5A0`; it does not infer corners from a conventional
rectangle ordering.

## Shared-host implementation

`tools/export_playerhose_stream_sprites.py` verifies the resource-3 RRF entry,
copies the two source bitmaps verbatim under
`.local/actor_exports/playerhose-stream/`, and records the hashes. The macOS
app build runs this exporter and packages the actor-assets directory. The
shared viewer loads `PlayerHoseStreamRenderer` only for dedicated
`RCS_PlayerHose` stages. It combines the base color and alpha bitmap by
normalized coordinates, draws with white vertex color, applies the four UV
values above, and scrolls U from `PlayerHoseStage::streamClockSeconds`.
PlayerRescue HoseSpray continues through the separate preview renderer.

The alpha bitmap is twice the color bitmap's width. The host resamples its red
channel at normalized color-texel centers, which averages adjacent alpha
columns for this pair. Retail calls `geBitmap_SetAlpha` through its resource
loader, but the renderer's handling of this dimension mismatch is not
established. The host's linear filtering, U repeat/V clamp, standard source
alpha blend, and interpretation of AddPoly flag `0x20` are provisional. Exact
Genesis/D3D sampling, component render cadence, and same-view retail comparison
remain open.

## Compile/package checkpoint

`./tools/build_scene_viewer_app.sh rescue-all` rebuilt the macOS scene viewer
and app bundle. The standalone and packaged executables match at SHA-256
`2db429ecb40fd0d16a1a11e9420d0d440691b32aee7570f42ae37dd7251c76df`. The
bundle's `Info.plist` validated, its mission catalog contains eight rescue
choices, and all four PlayerHose maps (`city_h`, `const_h`, `dock_h`,
`oil_rig_h`) are present along with the resource-3 sprite manifest and files.
Linking reports the existing Homebrew json-c deployment warning (library
built for macOS 27.0, target 13.3). No tests, interactive run, or retail
parity comparison were performed.
