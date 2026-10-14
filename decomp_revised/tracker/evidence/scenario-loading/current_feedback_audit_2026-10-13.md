# Current gameplay feedback audit

Recorded 2026-10-13 from the current shared viewer, saved hash-pinned retail
Ghidra evidence, and read-only subagent reviews. No source files were changed
by the audit agents. No live Ghidra instance or interactive desktop capture
was available in this session.

## Packaged state at the original 2026-10-13 audit

The latest CMake viewer and packaged Training app binary both have SHA-256
`4c9564098bda8b8395752a2983904eb6c0a93e815e3c7df45c5995242369208d`.
The rebuilt app's `Info.plist` passes lint. `--inspect` loaded 121/121 Training
groups and decoded all 24 textures. `--render-check` uploaded all 24 textures
without GL errors. The new `--tool-orientation-check` exercised both actual
equipment ACT render calls and compared direct-camera and half-turn roots; the
new `--hint-render-check` confirmed 22,418 pixels change when the hint panel is
drawn. These checks establish that the code paths produce pixels offscreen,
not that the result matches the retail game or a live desktop run.

## HUD hints

**Verified retail route:** EXE `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`:
Player hint handler `0044EE50` passes the authored string and lifetime to
`004564C0`. The latter creates a GUI text widget at Player `+0x6D8` in source
rectangle `(120,40)..(520,440)` in 640x480 coordinates.

**Current implementation:** authored hints set `activeHint` in
`port/spikes/training-native-prototype/src/scene_viewer.cpp`; the normal render
loop calls `HintOverlay::draw` before `SDL_GL_SwapWindow`. The overlay draws a
bitmap-font panel with an orthographic projection and no depth test. It is
also still appended to the application title.

**Offscreen result:** `--hint-render-check` rendered the sample
`Press E or Space to use this console.` panel over Training and recorded
22,418 changed pixels; the saved frame visibly contains a centered,
high-contrast panel. The ordinary render loop calls the same draw method
before swap. This demonstrates the panel is rasterized, but does not prove the
user's interactive build displayed it at the time of the earlier report.

**Still open:** interactive visibility/readability, font, panel style,
transition, and exact retail lifetime remain unverified or approximate.

## Goggles

**Verified retail route:** constructor `00462A60` binds alpha resources 300/301
to 640x32 screen rectangles at Y 68..100 and 380..412. `00462E80` accepts
actor kinds 2 and 6 within 2,000 units and excludes kind-2 states 5 and 6.
Resource 703 is `heatpulse.bmp` plus its alpha map, and the target effect is
active for 8,500 game-time ticks. Existing hash-pinned evidence is in
`goggles_scan_viewer_runtime.md` and the `00462A60`/`00462730` exports.

**Current implementation:** the viewer draws both visor masks while goggles
are selected. Left-click or the mapped selection transition starts the
source-filtered target scan; resource 703 is drawn as a depth-tested billboard
near each accepted actor for 8.5 seconds.

**Still open:** no interactive pixel capture confirmed mask visibility or
survivor markers. The heatpulse actor-local transform/child attachment,
animation, tint, size, and placement remain provisional. Current retail
evidence does not support general night vision or a broad visibility change.

## Player fire damage

**Verified retail route:** FireRescue callback `004483A0` sends Player message
`0x7531`; Player handler `00451640` accumulates exposure and reaches passout
above the strict threshold. Current-hash PlayerRescue slot `+0x0C` maps to
thunk `0049DB90`; its archived body export returns transformed world XYZ.

**Current implementation:** the normal viewer frame loop updates Player
movement, then tests fire candidates and calls
`handlePlayerFireMessage7531`. This is not a disconnected helper. The host
uses fire visual X/Z size, a 30-unit movement-hull padding, unit emitter scale,
suppression strength as intensity, a host BSP visibility trace, and a `.0001`
influence cutoff. Candidate evaluation is per FireRescue instance; retail's
four-record gate is stored at `FireRescue+0xB8`, so the viewer's frame-wide
four-record cap has been removed. Exact per-instance record reset timing
remains unresolved.

**Still open:** these host inputs are provisional; the retail candidate
getter's fourth float, emitter scale binding, callback/candidate scheduling,
exact coordinate alignment, minimum influence, and visibility equivalence
remain unresolved. The audit cannot identify which condition explains the
reported lack of damage without observing the live route. Do not increase
damage or enlarge fire volumes by guesswork.

The new fire-contact audit confirms that retail `0044BB00` writes one of four
transient directional burn-indicator bytes; it does not write view or movement
yaw. The current port sector math matches the retail ranges for a viewer where
yaw zero faces `-Z`, and the fire handler no longer changes `yaw` or
`yawInput`. The cooling call order is consistent with retail's consume-and-
clear behavior, but interactive camera response remains unobserved.

A second audit searched all current fire/contact/player yaw writers. It found
no fire-path assignment to camera yaw, input yaw, player position, or relative
mouse mode. `PreviewPlayerFireOps` now receives yaw by value, so the exposure
handler cannot change it through that adapter. Retail's saved `00451640` and
`0044BB00` outputs agree. The Training app now contains that source path at
SHA-256 `4c9564098bda8b8395752a2983904eb6c0a93e815e3c7df45c5995242369208d`.
If rotation persists, the main live input to inspect is SDL relative-mouse
motion around contact; player-camera translation from movement/collision can
also feel like rotation. No interactive capture was available to decide which
applies.

## Axe

**Verified retail route:** Axe root ACTION press `0045F8F0` writes a deadline
of global clock +100; held ACTION `0045FAD0` dispatches child update
`00460920`; child Think `004610E0` dispatches the strike `00460B90` in state 1.
The strike builds a swept query and on accepted BModel candidates invokes
`0043E2A0`, whose local `chop`/hitpoint/`break` order is source-comparable.
P-code-checked components live only in the older Training workbench at
`port/spikes/training-native-prototype/src/axe_*` and
`bmodel_axe_hit.h`; the shared scene viewer currently plays the selected
model's exported `swing` ACT motion after left-click but does not connect the
hit transaction. Its Axe click edge now also invokes `AxeActionCallbacks::press`
to record the source-derived global-clock +100 ms deadline; mapping that
mouse edge to retail ACTION is an inference, and held ACTION cadence remains
open. The selected Axe asset was corrected in the 2026-10-14
follow-up: retail's active primary child is model 6 `axel.act`, and the viewer
now draws that model.

**Still open:** query pose integration, Genesis world collision results, live
BModel candidate membership/order, strike service calls, model/effect
updates, and caller cadence. Training has authored positive-hitpoint
breakable props, but the `0043E2A0` script-actor and strike-time gates must be
resolved against actual runtime objects before claiming those props are
choppable in the port.

The follow-up integration audit narrows the strike gates: `00492CE0` returns
true only after a Genesis world collision, and `00460B90` scans the
`00481C00` iterator for the first type-4 BModel passing an 8-unit overlap and
the `0043E2A0` handler. That handler requires nonzero hitpoints, a null backing
ScriptActor, and a strike-time field outside ±1e-28. `0043C670` initializes
that field to zero; no current trace identifies the runtime writer that makes
the authored Training props eligible. Keep the viewer's Axe swing visual
until Player-frame mapping, iterator membership/order, per-prop state, and
callback cadence are resolved.

## Held-tool orientation

The user reported that the held models face backward. The first-person ACT
render candidate has changed following source review: the selected Axe's
active primary child is model 6 `axel.act`, not the model-1 `axe.act` that the
viewer previously drew. Its direct camera root exposes the blade in the
decoded port asset. The extinguisher direct basis exposes its label. These
are render candidates; neither is a retail pixel comparison.

Ghidra maps `0045F660` as a follower helper used by several equipment setup
paths, including extinguisher setup `00461830`. The mode-2 updater chain is
`00497E90` → `00497F70` → `00492110` → `00492450` → `geActor_SetPose`; it writes
ActorInfo matrix `+0x10..+0x48` before render callback `004926E0`. Retail
`0044F340` feeds selected-item view offsets into Player frame `+0x3460`, then
updates shared follower parent `+0x34A4`. Static evidence establishes the
writer and parent chain, but not the live composed parent matrix, per-item
mesh-facing result, hand origin, visible actor scale, or depth flags. The
camera-relative roots remain provisional until the user compares this build
with retail. See
[`held_item_orientation_audit_2026-10-13.md`](held_item_orientation_audit_2026-10-13.md).

## Fire-contact camera report

The retail `0044BB00` path does not write camera yaw; it sets one of four
directional burn bytes. In the viewer, `yaw`/`pitch` change only from captured
SDL relative-mouse events. The opt-in `--fire-camera-audit` logs exposure
entry/exit, player position, yaw/pitch, and mouse deltas; its 2026-10-14
update also logs SDL input-focus/relative-mode state, per-sample angle deltas,
and passout state. It is diagnostic only. Static code does not explain the
reported apparent autorotation; a live run must determine whether relative
input is arriving or whether the symptom is a different fire/pain response.

## Next checks

1. Compare the rebuilt model-6 Axe and extinguisher roots against retail.
   Check facing and hand placement; only offscreen port evidence exists so
   far.
2. Run with `--fire-camera-audit`, enter an authored fire with mouse movement
   stopped, and share the exposure/yaw/mouse log. This distinguishes actual
   relative input from a non-yaw fire/pain visual response.
3. Capture one authored hint and one goggles scan near a survivor; compare
   rendered overlays and target effect placement with the retail game.
4. Continue the shared Axe caller trace from the saved `00460B90.c` output,
   then integrate only when candidate and state gates can be mapped without
   guessed BModel behavior.

## Latest focused re-audit

Two read-only Luna reviews found no source-backed camera-heading correction:
captured SDL relative-mouse events are the only live yaw/pitch writer, and the
retail fire direction handler sets burn-indicator state rather than changing
camera yaw. The existing `--fire-camera-audit` is the discriminating runtime
check; no live trace was captured here. The held-item review likewise found no
evidence supporting an axis flip or 180-degree turn: the port root, offsets,
scale, and visible hand placement remain provisional because retail's final
camera-space ActorInfo matrix is still unknown.

