# GBSP v15 format map

Checked 2026-10-12 using the copied MBase and Training BSPs, the FDNY world-loader evidence, and the public Genesis3D 1.1 structure definitions. The public header is a strong format reference because the MBase chunk element sizes match its structs; this does not establish an exact source/binary match for FDNY's `Genesis.dll`.

## Evidence and provenance

- [`World/Gbspfile.h`](https://github.com/RealityFactory/Genesis3D/blob/f85b288ff54873e93879791ee9eb1367a311909f/World/Gbspfile.h#L36-L65) defines GBSP version 15 and chunk IDs. Its structures are at [lines 129–266](https://github.com/RealityFactory/Genesis3D/blob/f85b288ff54873e93879791ee9eb1367a311909f/World/Gbspfile.h#L129-L266).
- [`World/Surface.c`](https://github.com/RealityFactory/Genesis3D/blob/f85b288ff54873e93879791ee9eb1367a311909f/World/Surface.c#L210-L225) computes vertex dot products; its later [surface projection code](https://github.com/RealityFactory/Genesis3D/blob/f85b288ff54873e93879791ee9eb1367a311909f/World/Surface.c#L370-L386) applies inverse draw scale, shifts, texture dimensions, and vertical orientation. The new projector was compared against all 121 existing Training mesh groups: positions matched exactly and the maximum UV difference was `1.78e-15`.
- The pinned EXE's [`00495210` world loader](../tracker/evidence/genesis-boundary/00495210_bsp_world_loader.c) opens the supplied BSP through VFile, passes it to `geWorld_Create`, and stores the created world. [`0049E4D0` startup](../tracker/evidence/scenario-loading/0049e4d0_world_entity_load.c) then enumerates world models and entities for startup.
- The older `map_explorer/source/tools/extract_gbsp.py` in the previous `FDNY_trackdown` workspace was a useful partial inspector for models, faces, texture records, planes, and OBJ output. The active parser now centralizes chunk reading in [`gbsp_v15.py`](../tools/gbsp_v15.py) and exports a typed scene package with [`export_gbsp_scene.py`](../tools/export_gbsp_scene.py). Its TEXINFO decoder reads the `Flags` word as an integer, correcting the old inspector's all-floats interpretation.

## MBase chunk inventory

Offsets are chunk-header offsets; payload begins 12 bytes later. `size × count` is the on-disk element size and count. Runtime uses below follow the public Genesis3D structure names. The exporter preserves every payload under `.local/exports/mbase/chunks/` so opaque or not-yet-understood chunks remain available.

| ID | Name | Header offset | Size × count | Port use |
|---:|---|---:|---:|---|
| 0 | HEADER | `0x000000` | 28 × 1 | Tag, version, build time |
| 1 | MODELS | `0x000028` | 80 × 26 | World/submodel ranges, bounds, origins, motions |
| 2 | NODES | `0x000854` | 44 × 5,465 | Render BSP children, face ranges, planes, bounds |
| 3 | BNODES | `0x03B3B8` | 12 × 0 | Bevel collision nodes; empty in MBase |
| 4 | LEAFS | `0x03B3C4` | 60 × 5,491 | Contents, bounds, face/portal ranges, collision-side ranges |
| 5 | CLUSTERS | `0x08BB4C` | 4 × 1,263 | Visibility cluster offsets |
| 6 | AREAS | `0x08BAC4` | 8 × 6 | Area-to-portal ranges |
| 7 | AREA_PORTALS | `0x08BB00` | 8 × 8 | Dynamic area connectivity |
| 8 | LEAF_SIDES | `0x0B70B8` | 8 × 18,018 | Beveled collision plane references and side flags |
| 9 | PORTALS | `0x03B3AC` | 16 × 0 | Render/visibility portals; empty in MBase |
| 10 | PLANES | `0x08CF14` | 20 × 5,735 | Plane normals, distances, and type |
| 11 | FACES | `0x057C1C4` | 36 × 10,023 | Polygon spans, material, plane, and lightmap references |
| 12 | LEAF_FACES | `0x0A8F2C` | 4 × 14,432 | Leaf-to-face index list |
| 13 | VERT_INDEX | `0x0FD318` | 4 × 48,585 | Face corners into the vertex array |
| 14 | VERTS | `0x0DA3D4` | 12 × 11,930 | World-space vertex positions |
| 15 | RGB_VERTS | `0x04EDC4C` | 12 × 48,585 | Per-corner color values |
| 16 | ENTDATA | `0x12CA48` | 1 × 61,691 | Ordered entity key/value records and typedefs |
| 17 | TEXINFO | `0x13C9C7` | 64 × 1,495 | Texture vectors, shifts/scales, integer flags, alpha, texture index |
| 18 | TEXTURES | `0x13BB4F` | 52 × 71 | Texture names, dimensions, TEXDATA offsets, palettes |
| 19 | TEXDATA | `0x153F93` | 1 × 2,704,700 | Embedded indexed texture and mip bytes |
| 20 | LIGHTDATA | `0x4280FD` | 1 × 809,795 | Face lightmap payload |
| 21 | VISDATA | `0x3E84DB` | 1 × 202,080 | Cluster visibility payload |
| 22 | SKYDATA | `0x419A47` | 44 × 1 | Sky axis, rotation, texture indices, draw scale |
| 23 | PALETTES | `0x419A7F` | 768 × 71 | 256 RGB entries per palette |
| 24 | MOTIONS | `0x426F8B` | 1 × 4,454 | Serialized model motion records; encoding still open |

## Current export and remaining decode work

Run from the project root:

```sh
python3 tools/export_gbsp_scene.py inputs/original/Data/Maps/mbase.bsp --out .local/exports/mbase
```

The local-only export is pinned to BSP SHA-256 `4ff3b18dd809c619a6734673a7f90bd2e98d9fc9518ee7bf94ece4b6ff90b266`. It emits `world_mesh.json`, `geometry.json`, `collision.json`, ordered `entities.json`, typedefs, a texture manifest, and BMPs decoded from the embedded indexed texture data. Mesh groups carry both the original texture identity and a deterministic, case-insensitively unique file path so names that collide on macOS remain distinct. Training provides the parity fixture: all 121 mesh groups and all extracted BMP pixels match the existing prototype assets.

This export is a data milestone, not a playable level. The C++ host still builds Training's fixed scene and actor/script wiring. Next decode the `MOTIONS` records, resolve MBase ACT/SRF resources and entity-to-model aliases, then introduce the shared `SceneData` loader. Keep lightmaps, visibility bytes, and all source fields intact while their rendering/behavior is mapped; transparent surface behavior and Genesis color/light handling still need parity work.
