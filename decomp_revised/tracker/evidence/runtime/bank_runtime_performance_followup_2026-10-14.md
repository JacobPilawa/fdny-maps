# Bank runtime performance follow-up — 2026-10-14

## Evidence snapshot

- Retail executable: `inputs/original/binaries/FDNYFirefighter.exe`,
  SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Bank export: `.local/exports/bank`, BSP SHA-256
  `02a4b351279a9a4a1242050d3a7622f8d10149201646f890f310729a189d27b7`.
- Bank contains 578 entities, 334 collision models, 9,328 BSP nodes, 497
  render groups, 46,189 triangles, 105 textures, and 72 FireRescue actors.
  The authored spawn is `(-697, 160, 1196)`.

## Port changes

The Bank exposure loop previously issued a full point-visibility trace for each
active fire before evaluating whether that fire could affect the Player.
Retail `004483A0` rejects candidates outside its 500-unit max-axis gate and
computes volume falloff before requesting visibility. The port now evaluates
the unobstructed potential contact first and skips LOS when influence is below
the existing `.0001` cutoff. At the authored Bank spawn, only one of 72 fires
is within the retail 500-unit gate. Player exposure padding is resolved to
zero from the PlayerRescue constructor; the cutoff constant remains provisional
for damage behavior, while this ordering mirrors retail.

`Collision` now caches each model's index, root, origin, and authored bounds
once at load. `trace`, `tracePoint`, and `solid` use those compact records
instead of looking up JSON fields for all 334 models on every query. Moving
BModels still use their live pose transforms. Dynamic mesh rendering also uses
the cached model-origin map, replacing repeated scans of the JSON model table.
The renderer submits mesh vertices and UVs through OpenGL client arrays and
`glDrawArrays` instead of issuing one immediate-mode vertex call per vertex.

The render loop also performs conservative camera-frustum rejection against a
precomputed bounding sphere for each static mesh group. Meshes with a live
runtime angle/pose or an inspection pose bypass this rejection, so moving
BModels retain their existing render path. A Bank offscreen render-check image
captured immediately before and after this culling change has the same
SHA-256 (`4efee9c7bee109c21bb01b82eab28b3bc6fa82853593670d0ecaecb1d719171d`),
which confirms the cull did not change that authored camera's pixels. It does
not measure how many groups were rejected in an interactive view or the frame
time saved.

The Bank render check activates all 72 FireRescue visuals and draws 288 sprite
quads. Before this follow-up, `FireVisual::renderStep` constructed a 19-element
duration `std::vector` every time each fire was drawn: 72 heap allocations per
frame at Bank. Fire initialization now creates that per-visual duration table
once, and `renderStep` reuses it. `SpriteAnimation::update` receives the same
durations and timestamp as before; only the repeated allocation/fill was
removed.

## Retail timing trace

Fresh Ghidra 12.0.4 headless decompilation of `0049DBC0` is saved alongside
this note. It integrates each velocity component using the global frame delta
at `DAT_00502B04`, includes the acceleration contribution proportional to
`dt * dt`, and writes the updated position. This is elapsed-time-scaled
integration, not a constant movement increment per rendered frame. Other
reviewed routines (`00482AA0`, `00483580`, and `0049E0A0`) also consume the same
delta. Retail's idle loop calls update/render callbacks while its message queue
is empty (`00499410`); the game frame path enters `00494670` and calls
`geEngine_BeginFrame`/`geEngine_EndFrame`.

The port derives `dt` from SDL elapsed milliseconds and currently caps it at
100 ms. The exact retail delta source and any long-stall cap have not yet been
traced, so that clamp stays explicitly provisional. VSync is enabled in the
interactive viewer. A same-machine retail FPS capture is still required before
claiming frame-rate or performance parity.

## Verification

- The scene-viewer target builds successfully after the cache and renderer
  changes. The linker reports an existing Homebrew `json-c` minimum-OS warning
  (library built for macOS 27.0 while the target is 13.3); it is non-fatal.
- Bank `--render-check` loads all 497 render groups, uploads all 105 textures,
  and draws 72 FireRescue actors / 288 sprite quads. Its framebuffer matches
  the pre-culling packaged build exactly.
- Bank `--interaction-check` opens the authored door/PATH and vault sequence,
  including a later aligned passage through the door. `--rescue-check` releases
  one victim through its authored escape trigger.
- The rebuilt Bank app's `--render-check` framebuffer matches the prior
  pre-optimization Bank app exactly. Its door/vault interaction and
  rescue-release checks also pass against the packaged Bank scene.
- An offscreen render-check took about 1.85 seconds end-to-end for both the
  previously staged and newly built executables. That measurement includes
  scene loading and texture upload and is not an interactive FPS comparison.

## Remaining work

The user reported that Bank framerate now seems sorted. A same-machine retail
and port FPS recording is still needed before claiming measured parity. Keep
time-scaled movement while retail's delta producer and long-stall behavior are
unresolved. Continue the shared PropModel pickup and retail HUD work tracked in
`docs/WORK_QUEUE.md`.

## Follow-up checkpoint — 2026-10-14

Player sweeps now broadphase static collision models by their authored minimum
X bounds. Each candidate still goes through the same rotated-model bounds,
Genesis BSP leaf, and convex-plane trace. Models with sampled PATH poses are
removed from the static index and checked in a separate list each query;
candidate order is restored to the export's source order before traces run.
For a stationary query at Bank's authored spawn, 2 static model AABBs overlap
the Player sweep bounds from the 334-model set, before the moving-model checks.

An opt-in `--performance-audit` prints one-second FPS, average/p95/maximum
frame-time, update, world submission, overlays, swap, drawn/cull group, and
triangle summaries. These CPU/wall measurements can separate update cost from
render submission and presentation wait; they do not provide GPU timing.
Retail frame-pacing findings are recorded in
`retail_frame_pacing_audit_2026-10-14.md`: retail consumers read elapsed delta
at `DAT_00502B04`, but its writer and any presentation limit remain unknown.
The idle queue-empty path contains no explicit sleep.

The rescue-all macOS app was rebuilt with the current shared viewer. Its
executable SHA-256 is
`44488b009bcb83bc4fe8d4725edbee1dcffcfcc9b2c2bc9c2ac29e90c98213c1`. Packaged
Bank interaction and rescue-release checks pass, its render-check image retains
SHA-256
`4efee9c7bee109c21bb01b82eab28b3bc6fa82853593670d0ecaecb1d719171d`, and all
13 rescue/stage packages pass clear-spawn and five-second grounded-idle checks.
