# FireRescue visual startup in the shared scene viewer

## Source-backed integration

The shared viewer no longer consumes FireRescue startup random draws without
creating actors. `loadPreviewFires()` walks the serialized `RCS_FireRescue`
entities before victim startup, initializes each `FireEntity` with the shared
RNG, samples `ZZ_location`, `H_Radius`, `H_Height`, `K_SprayTime`, `A_Name`,
`M_Smoking`, and `L_FloorMe`, and runs the source-shaped floor placement from
`FireFloor::alignInWorld` (`00448630`). Initial visual size and named sound
choice come from the existing `FireEntity`/`FireVisual` components.

`FireRenderer` now accepts an explicit sprite directory. The scene viewer uses
the local actor asset bundle, while Training can keep its own caller-supplied
asset root. `tools/export_fire_sprites.py` copies the exact 19 `lflame` and 19
`fflame` BMPs from `inputs/original/Data/sprites/` into
`.local/actor_exports/fire-sprites/` and writes per-file SHA-256 values to
`manifest.json`. The Mac app builder includes this asset directory with the
other actor exports. The renderer loads those bitmaps through SDL and applies
the existing driver-derived blend state and alpha channel.

The interactive host calls recovered `FireEntity::think` once per rendered
frame and draws each actor after world geometry. That path still supplies
no-op callbacks for light updates, bookkeeping, and the separate state-4
path-motion route. Random proximity spread is connected as a distinct contact
pass after player movement, following the mapped `004483A0` callback into
`00448320` and `00447A90`. The host uses the strict `<750` attempt gate,
max-axis proximity distance truncated toward zero, candidate sweep, accepted-hit tolerances, random
child shape and lifetime, and defers child construction until the active fire
iteration ends, matching the mapped owner queue order. In `00447A90`, the
child peak uses `b=(10+90*(1-v))*U(.9,1.1)`, `x=b*(.25+.75*v)*U(.9,1.1)`,
`y=(10+30*v)*U(.8,1.2)`, and `z=b`; growth is 750–3000 ms followed by a
1000–6000 ms tail.

Several host inputs remain provisional. This single-player scene calls the
contact path once per rendered frame. Current-hash vtable data maps the
PlayerRescue root slot `+0x0C` to thunk `0049DB90`; the older body export shows
that thunk querying the transform at `+0x24` and returning transformed XYZ at
`+0x68`. The viewer adapts its movement origin `Player::p` to that world point,
but the getter body and the host-to-retail transform alignment have not been
re-exported/validated against the active executable. The callback's fourth
float and its current-hash initialization also remain open.

The spread-gate pointer at FireRescue root `+0x48` is a reference-counted
string object. Retail constructor/initializer evidence plus a disassembly of
the pinned executable shows that `00448D60` copies UserData `A_Name` into this
string and the spread routines check its length. The preview gates authored
fires on nonempty `A_Name`; generated children have a zero `A_Name` field and
do not become spread parents. See
[`fire_name_gate.md`](../fire-spread/fire_name_gate.md). The global proximity
count is rebuilt from named active fires in the host list, which models one
player and does not reproduce pooled/stale entries or other candidate
categories. Dynamic child keys use host absolute ticks, child visual
initialization consumes the shared host RNG, and collision uses the preview's
world trace; lifecycle and whole-engine RNG ordering remain open. The shared host also adds the
extinguisher's source-derived sampled contact and nonterminal suppression
message path; its camera parent and frame cadence remain provisional. Player
exposure, particle rendering, named loop playback, and exact dousing
presentation remain open. The `H_Radius` UserData binding used for victim
obstacles also remains unproven. See
[extinguisher_scene_viewer_runtime.md](extinguisher_scene_viewer_runtime.md).

The shared `FireEntity::spray` wrapper no longer disables flame animations
when the suppression helper enters terminal state 5. Retail code sets state 5
and clears byte `+0x1C8`, whose meaning is unknown; its visual callback returns
before the regular alpha/effect updates. The sprite lifecycle after that return
is unresolved, so the wrapper preserves the current visual state pending more
evidence.

## Verification

- After updating the candidate-origin adapter and spread name gate,
  `cmake --build build-fdny-current --target fdny-scene-viewer -j4`
  completed successfully. Linking warned that Homebrew `json-c` was built for
  macOS 27.0 while the target is 13.3. No gameplay run was made.
- After connecting random proximity spread to the shared viewer, the current
  macOS `fdny-scene-viewer` CMake target rebuilt successfully. No gameplay run
  was made, so live spread frequency, surface acceptance in a playable map,
  and child visual lifecycle remain unverified. Linking also warned that the
  Homebrew `json-c` dylib was built for macOS 27.0 while the target is 13.3;
  runtime deployment compatibility has not been checked.
- The CMake build succeeded for both `fdny-scene-viewer` and `fdny-training`.
- `gas_r --render-check` loaded 39 authored FireRescue entities and drew 156
  sprite quads; 120/120 world textures uploaded with no OpenGL error.
- `bank --render-check` loaded 72 authored FireRescue entities and drew 288
  sprite quads; 101/101 world textures uploaded with no OpenGL error.
- `bank --interaction-check` still passes its synthetic `*Door` and vault
  command/PATH flow. The user reports the live door still sounds but remains
  impassable until `M`; this synthetic check does not resolve that report.
- The rebuilt Bank app's packaged binary also passes `--render-check` and
  `--interaction-check`, confirming that the Resources bundle contains the
  scene, actor exports, fire sprites, scripts, and sounds.
- The historical Bank offscreen capture was taken while BoundsTrigger model 301
  (`bankzone`) was filtered from rendering. That class/model render filter has
  since been removed; rebuild before comparing a new capture. The user reports
  an opaque enclosure in the interactive start view, so identify the rendered
  mesh and apply authored material behavior before treating the issue as fixed.
- The BoundsTrigger loader now reports missing `C_Model`, missing `%Model%`,
  missing collision model, and invalid bounds joins to stderr instead of
  silently skipping them.

These checks confirm fire asset loading and the OpenGL draw path. They do not
verify animation timing, visibility culling, original material blending,
dynamic fire mechanics, or interactive desktop presentation.

## Player exposure integration update, 2026-10-13

`scene_viewer.cpp` now samples active FireRescue volumes after player movement,
uses the source-derived ellipsoid falloff and visibility attenuation, and sends
accepted influences through Player message `0x7531`. The shared handler updates
pain/burn deadlines and enters failed state 6 at the retail exposure threshold;
the retail pain and passout WAVs are bundled. The host uses its 30-unit player
hull radius as provisional padding because the fourth float returned by the
retail candidate getter remains unresolved. Fire volume extents, candidate
origin alignment, contact scheduling, and the minimum-influence constant also
remain provisional. No interactive damage capture has been made.
