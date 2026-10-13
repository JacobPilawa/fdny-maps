# Shipped Genesis bitmap and GBSP resource path

Updated: 2026-10-13

## Pinned input and method

This trace analyzes the supplied `Genesis.dll`, PE32 x86, SHA-256
`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643` in
Ghidra. Function excerpts are saved in
[`genesis_render_asset_functions.c`](../genesis-boundary/genesis_render_asset_functions.c)
and
[`genesis_bsp_puppet_functions.c`](../genesis-boundary/genesis_bsp_puppet_functions.c).
The Ghidra database has no configured save location, so these hash-pinned
pseudocode exports and this report are the durable analysis record.

## Bitmap file loading

`geBitmap_CreateFromFileName` (`0x1001C410`) opens a path through a Genesis
VFile system (or a supplied parent VFile), calls `geBitmap_CreateFromFile`, and
closes the VFile. The executable imports `geBitmap_CreateFromFile` directly;
its existing EXE wrapper opens a VFile before decoding. The decoder
(`0x1001B930`) accepts the native `GeBm` image format and a BMP fallback.
`FUN_100195A0` parses the BMP header and accepts uncompressed `BI_RGB` pixel
formats at 8, 16, 24, or 32 bits, handles row padding and row direction, and
rejects unsupported compression. The exporter emits the embedded palette
texels as 24-bit BMP, a format accepted by this engine path.

`geBitmap_SetAlpha` (`0x1001B890`) attaches a referenced alpha bitmap to the
base bitmap, releasing the old reference when replaced. This confirms the
EXE's optional alpha-bitmap setter is a bitmap relationship; it does not by
itself establish how a BSP surface finds an external alpha resource.

## GBSP load and world bitmap registration

`geWorld_Create` (`0x1003E810`) allocates and initializes a world, then routes a
provided VFile through `FUN_1003B8C0` to create world data. That loader calls
`FUN_10032400`, whose loop consumes chunk records until the `0xFFFF` terminator.
The record decoder `FUN_100319D0` verifies the `GBSP` tag and v15 header, checks
record sizes, allocates typed chunk arrays, and reads payloads. The accepted
sizes match the local exporter and Genesis3D format definitions: for example,
MODELS `0x50`, FACES `0x24`, TEXINFO `0x40`, TEXTURES `0x34`, SKYDATA `0x2C`,
PALETTES `0x300`, and byte-sized TEXDATA/LIGHTDATA/VISDATA chunks. The header
requires version 15. `FUN_10032350` is a follow-up recursive BSP pass that
checks face flags referenced from leaves; the meaning of its bit `0x04` is not
assigned here.

During world creation, `FUN_1003E6E0` initializes the attached-bitmap list,
iterates the embedded bitmaps in loaded world data, queries their bitmap info,
and passes each through `geWorld_AddBitmap`. That API (`0x1003CF10`) adds a
referenced bitmap to the world list and marks world bitmap state dirty;
`geWorld_RemoveBitmap` (`0x1003CF80`) removes it and also marks the state dirty.
The EXE imports `geWorld_AddBitmap` and uses the same world registration API
after its `.rrf` bitmap and alpha-resource loaders create bitmap objects.

Genesis actor materials use the same world list: `FUN_10010D50` walks body
materials, keeps a reference to each non-null bitmap, and registers it with the
current world. `FUN_10010EC0` updates a puppet material, unregisters the prior
bitmap when changed, then registers the replacement. This is the engine-side
path needed for later actor texture integration.

## Name lookup and unresolved material join

`geWorld_GetBitmapByName` (`0x1003CFC0`) delegates to `FUN_1003AC10`, which
compares bitmap-list names case-insensitively and returns the associated bitmap
pointer. Ghidra found no direct caller of that wrapper in the DLL, and the EXE
does not import it. Therefore this API is not proof that the EXE's `.rrf` keys
join by name to BSP `GFX_Texture` records. The EXE's direct import table
confirms `geBitmap_CreateFromFile`, `geBitmap_SetAlpha`, and
`geWorld_AddBitmap`, but the RRF numeric-key-to-BSP-face surface join remains
unresolved.

The static port exporter already follows BSP face → texinfo → texture index,
decodes embedded palette-indexed TEXDATA, and produces the base BMPs consumed
by the viewer. The macOS CGL framebuffer checks confirm that those BMPs upload
and produce textured pixels on all seven rescue maps. The shipped D3DDrv.dll
texture create/lock/unlock and textured draw callbacks are now traced in
[d3ddrv_render_path.md](d3ddrv_render_path.md); the RRF sidecar audit finds its
alpha bitmaps are sprite/effect/HUD paths with no BSP texture-name matches.
This does not reproduce Genesis lighting/lightmaps, per-face alpha, surface
caching, or original window presentation. The concrete preview failure was the
viewer disabling world model 0 for BoundsTriggers with no `model` property, plus
its visible `col_*` helper meshes; see
[`scene_preview_collision_texture_diagnostics.md`](scene_preview_collision_texture_diagnostics.md).

## Evidence inventory

The engine ledger in `tracker/genesis_function_reviews.csv` now records 25
hash-pinned Genesis.dll functions across collision, GBSP loading, bitmap
registration, and actor material paths. It is separate from the EXE-only
function-count dashboard. The collision-specific map and the static Training
oracle comparison are in
[`genesis_collision_path.md`](genesis_collision_path.md).
