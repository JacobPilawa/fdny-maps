# Scene preview collision and texture diagnostics

Updated: 2026-10-13

## Question

The user reported that the interactive scene preview loads a level but the player
falls through it and the geometry has no textures. This trace checks whether the
current exported rescue packages contain floor collision and decodable texture
assets, and adds runtime reporting for the OpenGL upload path.

## Reproduction method

Rebuilt `fdny-scene-viewer` from the checked-in C++ source and ran each package
with `--inspect`. Inspect mode decodes the unique BMP files, probes the authored
player spawn using the same swept hull as the player, and simulates five seconds
of idle movement through the production `Player::step` and `Collision::trace`
code. It does not create an OpenGL context or present a window.

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

The six campaign rescue-map packages other than MBase and MBase itself (all
seven required campaign rescue maps) resolve their referenced BMPs, and the
current C++ collision path finds a walkable floor and settles on each map in
this headless simulation. For world model 0, the origin is zero, so the
previously noted local-bounds broad-phase risk for translated submodels does
not explain the reported world-floor fall-through.

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

## Limits and next check

The shell used for this work has no interactive display. The inspect results
prove the exported BSP collision route and BMP decoding, but do not prove that
OpenGL textures appear in a desktop window. Interactive startup now prints the
actual OpenGL upload count/errors and exposes that count in the title. The next
visible checkpoint is the rebuilt Mac preview bundle; if the title shows all
textures uploaded and `grounded` while the scene still appears gray or the player
continues below the floor, capture that title state and a scene screenshot so
we can target the renderer or platform GL behavior separately.
