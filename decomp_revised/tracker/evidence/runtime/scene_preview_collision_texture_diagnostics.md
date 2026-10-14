# Scene preview collision and texture diagnostics

Updated: 2026-10-13

> **Historical render-filter results:** The group counts and sample frame below
> predate the source cleanup. The old hard-coded class/model BoundsTrigger
> suppression and `col_*` texture-prefix filter were removed. The shared viewer
> now applies the authored per-model `ModelSetFlags` state for resolved
> BoundsTriggers; see [`trigger_brush_render_visibility.md`](trigger_brush_render_visibility.md).
> `Collision::disabled` still excludes authored trigger brush models from
> player-solid collision; it does not suppress rendering. Rebuild and capture
> again before treating the old render counts or appearance as current.

## Question

The user reported that the interactive scene preview loads a level but the player
falls through it and the geometry has no textures. This trace checks whether the
current exported rescue packages contain floor collision and decodable texture
assets, and verifies the shared viewer's actual OpenGL drawing path.

## Reproduction method

Rebuilt `fdny-scene-viewer` from the C++ source and ran each package with both
`--inspect` and `--render-check`. Inspect mode decodes the unique BMP files,
probes the authored player spawn using the same swept hull as the player, and
simulates five seconds of idle movement through the production `Player::step`
and `Collision::trace` code. Render-check creates a macOS CGL OpenGL 2.1
offscreen context, then invokes the same `renderScene` path as the interactive
viewer, reads back the framebuffer, and counts distinct colors and drawn pixels.
It verifies texture sampling/drawing without requiring a desktop display.

## Current results

| Map | BSP SHA-256 | Texture files decoded | Spawn | Walkable world-model contact | After 5 s idle |
| --- | --- | ---: | --- | --- | --- |
| MBase | `4ff3b18dd809c619a6734673a7f90bd2e98d9fc9518ee7bf94ece4b6ff90b266` | 63/63 | `(-132, -36, 140)` | `(-132, -205.9, 140)`, up normal | grounded at `y=-206` |
| Bank | `02a4b351279a9a4a1242050d3a7622f8d10149201646f890f310729a189d27b7` | 105/105 | `(-697, 160, 1196)` | `(-697, -47.9, 1196)`, up normal | grounded at `y=-47.9` |
| Bar R | `1a796f4dfbf0d04bedf7e35ebdcf82e2e726940bf69b1df7ce2ea7428e018a98` | 76/76 | `(-304, 128, -332)` | `(-304, 46.1, -332)`, up normal | grounded at `y=46.1` |
| Gas R | `f6822f8d598d2888896165bbee8b554351230cd76ad6331abf0d735a03b54cbc` | 123/123 | `(-432, -112, -556)` | `(-432, -201.9, -556)`, up normal | grounded at `y=-201.9` |
| Boat A | `e1685b45ef29aa075c688985617b593079a6aad8a6b493bbae327356bd45ad44` | 76/76 | `(-1253, 16, 121)` | `(-1253, -73.9, 121)`, up normal | grounded at `y=-73.9` |
| Subway A | `b33b67a01e0a2c144033f1da5c9c627b197d0a413b3b13190dd8531b63881f7e` | 81/81 | `(-85, 35, 1948)` | `(-85, -37.9, 1948)`, up normal | grounded at `y=-37.8` |
| Oil Rig A | `c63ff7e6a7e41a6df15f572bc1c327cbc6ecbe934a17cf1c5a136c2a7d7e36c1` | 71/71 | `(-632, 952, -620)` | `(-632, 838.1, -620)`, up normal | grounded at `y=838.4` |

All seven rescue maps resolve their referenced BMPs, and the current C++
collision path finds a walkable floor and settles on each map in this simulation.
The first test used a fresh `Collision` object and therefore missed the
interactive viewer's disabled-model list; that diagnostic bug is fixed below.

## Root causes and corrections

Two preview defects caused the reported behavior:

1. `RCS_BoundsTrigger` entities in these BSP exports have no `model` field.
   Both viewers converted that missing JSON value to integer zero and inserted
   world BSP model 0 into `collision.disabled`. That removed the main world
   from collision, so the player fell through, and from drawing, so the scene
   appeared empty. The map-neutral viewer and Training workbench now disable a
   trigger model only when an explicit positive integer model index exists.
   `--inspect` now reports through the same collision instance used to load the
   scene, including its disabled-model state.
2. The map-neutral renderer was drawing `col_*` helper/collision surfaces in
   debug colors. A texture-prefix draw filter was added as a temporary visual
   workaround, then removed from both render hosts because texture names are
   not evidence of retail visibility semantics. Those surfaces now use the
   ordinary draw path; authored face material behavior must determine their
   appearance.

Neither issue came from missing BSP bitmap joins or the alpha-resource route.
The pixel readback below verifies that the existing base texture pipeline is
now reaching the OpenGL sampler and draw calls.

## OpenGL framebuffer results

| Map | Visible BSP groups | Textures uploaded | Distinct framebuffer colors | Non-background pixels |
| --- | ---: | ---: | ---: | ---: |
| MBase | 77/95 | 60/60 | 93,703 | 958,411 |
| Bank | 373/497 | 101/101 | 70,542 | 1,024,000 |
| Bar R | 140/192 | 67/67 | 70,696 | 1,024,000 |
| Gas R | 355/361 | 120/120 | 142,644 | 1,024,000 |
| Boat A | 163/182 | 73/73 | 124,880 | 1,000,068 |
| Subway A | 317/384 | 76/76 | 91,951 | 1,024,000 |
| Oil Rig A | 76/81 | 69/69 | 129,290 | 878,701 |

Each historical render-check completed with a nonempty, highly varied
framebuffer. At that time `col_*` helper groups were counted but omitted from
drawing. These results do not describe current source after filter removal. A
sample Bank frame is saved locally as
`.local/exports/bank.render-check.png`; generated frames are not committed.

## Changes made to the preview

- Texture files are cached by resolved path instead of loaded once per mesh
  group.
- Asset decoding and OpenGL upload failures are counted and the first eight
  failing paths/errors are printed. `--inspect` now decodes BMPs rather than
  only checking path existence.
- The title reports texture uploads, grounded/falling state, and player XYZ.
- Pressing `F` snaps the player to the nearest walkable BSP collision surface
  below them, providing a recovery control during map inspection.
- `--inspect` now reports the player spawn's collision state, floor hit, and
  idle movement result.
- BoundsTrigger collision suppression now requires an explicit positive model
  index, so a missing field cannot disable world model 0.
- Historical: a `col_*` texture-prefix render filter was used during the first
  textured-preview fix. It is removed from current source; do not reintroduce
  it as a visibility rule.
- `--render-check` now exercises the real OpenGL scene draw path offscreen and
  counts texture uploads, distinct framebuffer colors, and drawn pixels.

## Limits and current Bank check

The fresh Bank build's offscreen check exercised the production draw function,
submitted 497/497 world mesh groups, decoded/uploaded 105/105 textures, and
produced 70,542 framebuffer colors with 1,024,000 non-background pixels. It
does not verify window presentation or match the user's exact interactive
camera view. The visible dark obstruction remains unmatched. Original
Genesis material/alpha behavior, lighting/lightmaps, texture filtering, actors,
scripts, and gameplay parity remain open; this is not a complete rescue port.
See the genesis collision path evidence for the original engine collision
boundary and Training oracle parity results.

## Current source audit (2026-10-13)

The shared viewer submits exported mesh groups through the ordinary draw loop,
subject to the per-model flag gate. BoundsTrigger contact and collision
handling are separate from that render path. A fresh out-of-tree macOS build
and focused `--trigger-render-check 106` on the current source reproduced the
forced-visible versus flags-zero comparison (1,024,000 changed pixels), so the
current source gate suppresses Training's red hint brush when it receives the
retail initialization value. This check validates the port's flag path, not
the shipped renderer's interpretation of `ModelSetFlags(0)`. The Bank
start-view investigation remains open: identify the exact surface, preserve
its face material data, and implement the behavior established by the retail
renderer.
