# Shipped D3DDrv texture and draw path

Updated: 2026-10-13

## Input and method

This trace analyzes the supplied PE32 x86 D3DDrv.dll at image base
0x10000000, SHA-256
7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7.
Ghidra's active project has no configured save path, so address-level
pseudocode exports and this report are the durable record. Selected function
bodies are in [d3ddrv_render_functions.c](../driver-boundary/d3ddrv_render_functions.c).
The earlier hook and Genesis-to-driver table traces are
[d3ddrv_driverhook_boundary.md](../driver-boundary/d3ddrv_driverhook_boundary.md)
and
[selected_driver_initialization_trace.md](../driver-boundary/selected_driver_initialization_trace.md).

## Driver interface table

DriverHook at 0x10006300 returns the table at 0x1001F040. The version fields
are 101.1, as checked by Genesis during driver selection. Reading the table
bytes and decompiling its targets gives these verified slots:

| Offset | Target | Evidence-level behavior |
|---:|---:|---|
| +0x14 | 0x10004C20 | Enumerates DirectDraw subdrivers through a callback. |
| +0x18 | 0x10004DA0 | Enumerates display modes; callback receives 16-bit modes and WindowMode. |
| +0x1C | 0x10005F10 | Enumerates pixel-format records, including 16-, 24-, and 32-bit cases selected from queried display fields. |
| +0x20 | 0x10005DD0 | Initializes the renderer from a context and fills client width/height when the context requests defaults. |
| +0x24 | 0x10005E80 | Calls the driver shutdown/reset helper. |
| +0x28 | 0x10005F00 | Initializes renderer caches and resets the draw counter. |
| +0x38 | 0x10008990 | Creates a texture handle. |
| +0x3C | 0x10008600 | Destroys a texture handle and releases surfaces/cache references. |
| +0x40 | 0x100082B0 | Locks one mip surface and returns the locked pixel pointer. |
| +0x44 | 0x10008330 | Unlocks one mip surface and updates its state flags. |
| +0x58 | 0x10008380 | Reports per-mip dimensions and texture flags. |
| +0x5C | 0x10007F30 | Begins a frame, resets per-frame counters, clears buffers, and sets initial render state. |
| +0x60 | 0x10008080 | Ends the Direct3D scene and flushes pending presentation work. |
| +0x7C | 0x10007750 | Converts an input vertex stream to a driver vertex buffer and submits it. |
| +0x80 | 0x10007BC0 | Builds a textured vertex stream and submits it with texture and flag parameters. |
| +0x84 | 0x10007910 | Builds a textured polygon stream, applies texture-sized UV scaling and optional vertex alpha, then submits it. |
| +0x88 | 0x10007DE0 | Copies a source surface rectangle to a destination surface through DirectDraw. |
| +0xA8 | 0x10003690 | Applies the fog-setting tuple; Genesis callsites are documented in the selected-driver trace. |

The table has additional entries that remain unnamed or only partially
understood. In particular, this table is an observed binary interface, not a
recovered C header. Callback signatures and some flag meanings remain
provisional.

## Texture allocation and upload

The table's texture-create callback allocates a handle and selects one of
three creation paths from the request flags. It builds the DirectDraw pixel
format description from the supplied format index and creates cached surfaces
through the driver's main texture cache. The mip-chain path allocates a
separate 20-byte record per level, creates successively halved surfaces, and
uses the request dimensions to select a power-of-two base size. Another path
creates a single multi-level surface; a third creates a surface at the
requested dimensions. The decompilation does not identify the public Genesis
flag names, so the flag values are preserved as raw bits.

Lock callback +0x40 calls the DirectDraw surface Lock method for a selected
level, marks the level locked, and returns the surface pointer. Unlock callback
+0x44 calls the matching Unlock method and updates the level state. The
dimension callback shifts the base width and height by the requested mip
number. Destruction releases each cache reference and surface object before
freeing the mip records. This is direct evidence for the original engine
texture-upload boundary, rather than a claim that Genesis bitmap decoding and
GPU upload are the same operation.

## Draw and frame behavior

The textured-polygon handler at 0x10007910 transforms the incoming stream into
driver vertices. It scales texture coordinates using the base texture
dimensions and mip level; conditionally uses vertex alpha depending on raw
flags; then obtains the cached texture surface and submits the converted
vertices. The +0x80 callback follows a related textured-vertex route. A
follow-up hash-pinned Genesis-to-driver trace maps the BSP face descriptor and
dynamic RGB lightdata callback through this slot; see
[`genesis_face_lightmap_driver_handoff_2026-10-15.md`](genesis_face_lightmap_driver_handoff_2026-10-15.md).

BeginScene checks that a D3D device exists, resets frame counters, checks the
texture cache, clears the requested buffers, begins the D3D scene, and sets
the initial render states. EndScene closes the D3D scene and invokes the
driver's buffer/presentation helper. The initialization function
0x10005070 logs driver version 101.1 and build date Mar 4, 2002, initializes
DirectDraw and device/render state through a chain of helpers, creates the
texture cache, and sets the initial viewport.

These functions establish what the shipped Windows renderer expects from
Genesis and its bitmap surfaces. They do not demonstrate that every BSP
surface reaches a particular callback at runtime, nor do they recover the
original pixel shader/state semantics from public source.

## RRF comparison against all seven rescue BSPs

The copied raw Data/ResDefs/rescue.rrf (SHA-256
8ab12b7c148c4d984a912525e432967556c9267af5a1b0ea86451ed1ee123f33) has 114
ROC_ALPHA_TEXTURE records,
113 distinct numeric keys, 110 distinct base paths, and 44 records with an
optional alpha path. All listed base paths are under Data/Sprites. A
case-insensitive basename comparison against the BSP TEXTURES records in the
seven exported rescue maps found zero name overlaps. The numeric key-to-BSP
surface relationship remains unproven; the source trace does not show a
lookup by those keys from GBSP surfaces. The result table is saved in
[rescue_rrf_bsp_texture_name_audit.csv](rescue_rrf_bsp_texture_name_audit.csv);
regenerate it from the local copied raw assets and exports with
`python3 tools/audit_rrf_bsp_texture_names.py`.

This separates two assets that had been conflated during the original
texture investigation:

1. BSP faces use their TexInfo index to select a BSP TEXTURES record; the
   current exporter decodes that embedded paletted TEXDATA to base RGB BMPs.
2. RRF alpha records identify authored sprite/effect/HUD bitmap pairs and are
   registered with the Genesis world or retained as deferred resources.

Therefore the RRF alpha-pair loader is not the explanation for missing BSP
wall/floor textures in the preview. The preview bug was the disabled model 0
and visible col_* helper surfaces, both corrected and covered by the
all-rescue render/collision evidence in
[scene_preview_collision_texture_diagnostics.md](scene_preview_collision_texture_diagnostics.md).

## Remaining work

- Connect EXE world/bitmap resources to Genesis texture requests at runtime;
  this static trace does not prove the exact per-face dynamic call sequence.
- Preserve and port BSP TexInfo flags/alpha and apply lightmaps instead of
  treating the exported BMP as the entire material.
- Recreate sprite/effect/HUD RRF key lookup and alpha-pair behavior separately
  from BSP geometry.
- Confirm the rebuilt viewer in a desktop window; the CGL test verifies the
  production OpenGL draw path offscreen and does not test window presentation.
- Keep DirectDraw/D3D behavior behind the shared renderer contract for Windows,
  macOS, and Linux parity work.



## Evidence inventory

The driver review ledger is [d3ddrv_function_reviews.csv](../../d3ddrv_function_reviews.csv).
The RRF name-audit script reads the local copied raw RRF and hash-pinned BSP
export reports; the generated CSV contains source hashes, record counts, and
per-map overlap results without copying game bitmap or BSP payloads.
