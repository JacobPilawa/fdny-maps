# PlayerHose shared host slice (2026-10-20)

Status: **the shared game host loads the four dedicated PlayerHose maps,
resolves authored WindowFire target names to BSP model IDs, applies the
source-derived numeric mode timer scale, runs a source-shaped target/cycle/
hotspot state path, follows the authored HosePath camera rail with a
floor-height query, and tracks the source-shaped `M_MaxPressure` scalar with a
timestamped ballistic strip path.
The shared loop now uses the traced retail delta scale,
lower pacing threshold, and upper cap. Retail menu labels/initial mode, rail
speed calibration, SDL tick quantization/focus-pause mapping,
mouse-to-nozzle mapping, strip resource/UV/material binding, live dousing
cadence, WindowFire visuals, result widget, and stage transition remain
open.**
This is a port checkpoint, not a live retail-parity result.

## Evidence and authored data

Retail `FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The `city_h` source BSP SHA-256 is
`9bc788dcaceb477fea52efd49cfbb2fd8f86824907d73c819280710b04fcb27a`.
The four dedicated PlayerHose exports contain 83 path nodes, 28 authored
hotspots, and 103 non-null `ZF0..ZF8_FireModel` references across the maps.
`city_h` begins at `RCS_HosePath19`, follows `A_NextPoint` to hotspot 1, and
contains nine hotspots with 29 references. The references resolve to `%Model%`
entities and their BSP collision model indices; they are not `RCS_FireRescue`
actors.

The current source evidence establishes that `00465B90` creates WindowFire
targets from each non-null ZF slot and retains paired SprayTime values;
`004677D0` reads hotspot D links, first-hotspot marker, spot time, max pressure,
and pitch spread; `00465A20` advances selected targets and complete target
cycles; `00468680` subtracts frame delta divided by SprayTime and clears a target
when the value falls below zero; and `004674A0` decrements the hotspot timer,
checks the target-cycle result, and calls the win/loss handler on its terminal
branch. The input and stream collision path is `00466780` / `0046D1B0` /
`0046C5A0`; DouseHose sends the hit point and stream-owner intensity to
WindowFire through message `30000`.

Fresh decompilation is preserved in
[`playerhose_hotspot_state_machine_fresh_2026-10-20.c`](playerhose_hotspot_state_machine_fresh_2026-10-20.c),
[`playerhose_douse_windowfire_fresh_2026-10-20.c`](../fire-spread/playerhose_douse_windowfire_fresh_2026-10-20.c),
[`hose_contact_and_playerhose_fresh_2026-10-20.c`](../fire-spread/hose_contact_and_playerhose_fresh_2026-10-20.c),
[`playerhose_stream_shape_interpolation_fresh_2026-10-20.c`](../fire-spread/playerhose_stream_shape_interpolation_fresh_2026-10-20.c),
[`playerhose_strip_construction_fresh_2026-10-20.c`](../fire-spread/playerhose_strip_construction_fresh_2026-10-20.c),
[`playerhose_strip_texture_lookup_fresh_2026-10-20.c`](../fire-spread/playerhose_strip_texture_lookup_fresh_2026-10-20.c),
and the fuller authored-data and open-question inventory in
[`player_hose_stage_lifecycle_2026-10-20.md`](player_hose_stage_lifecycle_2026-10-20.md).

## Shared-host implementation

[`player_hose_stage.h`](../../../port/spikes/training-native-prototype/src/player_hose_stage.h)
keeps the dedicated stage separate from rescue-player HoseSpray. The shared
`scene_viewer.cpp` loader reads `RCS_PlayerHose`, `RCS_HosePath`, and `%Model%`
records from the raw `SceneData` entities. It preserves the nine ZF slot
positions and paired SprayTime values, resolves target model IDs, starts from
`FirstPathPoint`, and builds control-node sample distances from the A rail.
Hotspot activation is anchor-driven: the first marker starts the first hotspot,
and each D-linked hotspot activates only when its A-rail anchor is selected.
If target cycles finish first, the host waits at that boundary. This mirrors
the selected-control-record change event from `00490BB0` and the
expected-pointer/first-marker check in `004677D0`. The exact
state scheduler frame between hotspot setup and countdown entry remains
unresolved. The default macOS bundle includes the four hose BSPs. Launch any
one directly with its map key, for example
`./Open FDNY Firefighters.command city_h`. A direct launch may pass
`--hose-mode-index 0|1|2` after the map key to select the source-derived timer
scale; the launcher forwards this host option to the executable.

The shared host builds its camera rail from `FirstPathPoint` and the linked
`A_NextPoint`/`B_LookAtPoint` data. It samples ten cubic subdivisions per
segment, traces from rail X/Z at Y `+50` to `-2000` using collision arguments
`1/2`, and places the camera origin at the hit Y `+32`. It advances the rail
with `dt*500/length`; interpreting the retail `+500` factor as units per host
second remains provisional. The view follows the interpolated LookAt point.
Mouse steering offsets the nozzle aim around that direction, preserving the
existing `N_PitchSpread` degree clamp; its exact relation to retail
`STEER_PITCH/YAW` remains provisional.

The host maps left-button hold to `MOUSE_LB`, traces a pressure-scaled segment
from the retail-shaped `00465EF0` frame offset with the same `0x43/2`
world-collision arguments seen in the retail stream collision, and checks the
selected target's model bounds against the impact point expanded by the
current-pressure contact radius. A world collision is still required to set
the contact gate. The temporary aim basis follows retail's pitch-around-
axis-0 then negative-yaw-around-updated-axis-1 order, including the helper's
basis-by-axis cross-product order, with source point and direction kept
separate. SDL deltas stand in for retail's separate pitch/yaw accumulators;
their accessor cadence, scale/sign mapping, effective view-angle limit, and
exact matrix rounding remain provisional. It decrements the hotspot countdown
before the dousing update, applies the authored SprayTime divisor, and presents the
current hotspot, target, cycle, and timer in the window title. WASD movement
is disabled in dedicated rail mode.

The host now models the source-derived WindowFire timer path. Selecting a
target starts its `+0x258` timer at zero and enables a ramp; host updates add
`2*dt` until it reaches one. Matching stream contact subtracts
`dt/SprayTime`, and the target completes only when the result is strictly
below zero. The source threshold at `004dc50c` is `0.0f`; zero and negative
divisor arithmetic are preserved. The callback versus entity-update order,
SDL tick quantization/focus behavior, and live contact cadence remain
provisional. The source-derived session mode scale modifies M_SpotTime before
it becomes the countdown; the frame-delta trace establishes both values are
seconds. Host mode index 2 is the default because
the retail selector and initial index are not yet mapped. Pitch is clamped to
the current N_PitchSpread in degrees; mapping mouse deltas to retail
STEER_PITCH/YAW remains provisional. `M_MaxPressure` at owner `+0x1834`
aliases stream-component target `+0x3B4`, while current stream pressure is at
owner `+0x1830` / component `+0x3B0`. The shared host tracks the source's
per-update 0.2 interpolation toward that target while held and toward zero
after release. Retail scales a stored aim-velocity vector by `4000 * pressure`
and records the separate shape scalar `0.8 + 2.2 * pressure`. Its eight-sample
ring covers 0.75 seconds, with samples written at 0.1 second intervals. The
25-section consumer interpolates that source/velocity history and evaluates
two ballistic curves, the second using a 0.6 age scale and a -500 Y velocity
offset. It submits their consecutive samples as four-vertex quads and checks
the main path for world collision; at full pressure its nominal horizontal
travel is 3000 units over the 0.75 second sample lifetime. The host now draws
these dual-curve quads and collision-tests successive main-path segments. The
strip consumer culls samples whose interpolated shape scalar is
at most 1.0 and passes twice that scalar to the curve helper. It remains unclear
how that value transfers to rendered vertex width or color. Retail also sends
current pressure as a WindowFire message payload; the traced timer ignores it
as a rate input. On collision it sets the douse contact radius from 10 to 40
using current pressure; the host checks that radius against the selected
target's world AABB. The host does not yet bind renderer resource ID 3, which
the retail strip constructor requests, and its opacity remains provisional.
Exact texture, UV state and rendered-water appearance remain open. The earlier exact
`+0x1834` instruction-text scan finds only the assignment at `00467B8A`; it
missed the component-relative alias. The host win/loss title is not retail
result presentation. See
[`player_hose_pressure_stream_2026-10-20.md`](player_hose_pressure_stream_2026-10-20.md).

The rescue catalog still validates and displays exactly eight `rescue.rrf`
rows. The four hose packages are available as direct launches from the bundle,
but a retail-authentic hose selector and result/return flow remain open.

## WindowFire selected-target timer port

Current-hash Ghidra evidence establishes that target selection through
`00468850` starts the WindowFire timer at zero and enables the ramp. The
`00468870` code adds `2*dt` while that ramp flag is set, caps the timer at one,
and clears the ramp flag on the upper branch. Contact delivery through
`00468680` subtracts `dt/SprayTime` only while the linked target is active and
selected. It clears the dousing flag only when the result is strictly below
`004dc50c`, verified as `0.0f`, then resets the timer. The shared host now
uses that initialization, ramp, arithmetic, and comparison. Its target update
executes ramping before the host contact, but message/entity scheduling and
frame-delta units remain provisional.

A headless instruction-text scan against the pinned retail EXE found only the
assignment to PlayerHose `+0x1834` at `00467B8A`; a follow-up resolves the
consumer through the embedded stream component at `+0x1480`, whose `+0x3B4`
field aliases that pressure target. The original scan scope is in
[`player_hose_pressure_scan_2026-10-20.md`](player_hose_pressure_scan_2026-10-20.md),
and the alias plus port are in
[`player_hose_pressure_stream_2026-10-20.md`](player_hose_pressure_stream_2026-10-20.md).

The updated shared macOS target and rescue-all bundle compiled successfully.
Standalone and packaged executable SHA-256 values match at
`598ecd280b436093a5e11d0e0d2775924bbc10a1defcbb0833876399ed3bc52c`. The
bundle plist passed; it retains eight rescue choices and all four hose maps.
The existing json-c macOS 27.0 versus target 13.3 warning remains. No test
suite, interactive run, or retail comparison was performed.

## Anchor event and stream source follow-up

The stage now selects a route control record from the current sampled rail
distance and processes a hotspot only when that selected record changes. The
first marked hotspot starts from its anchor; after entry, `D_NextHotSpot` is
the expected path node. A completed target-cycle context waits if its next
anchor has not arrived. This follows `00490BB0` and `004677D0` statically; the
host does not claim retail frame timing or a controlled gameplay match.

The stream collision and line now start from the entity frame offset returned
by `00465EF0`: `origin + 10*basis3 + 15*basis2 - 10*basis1`. The steered
direction comes from the separate provisional host aim frame. Source-derived
rotation order and `STEER_PITCH/YAW` accessor cadence remain open.

## Build result

The initial gameplay-slice macOS `fdny-scene-viewer` target compiled successfully with
`cmake --build port/spikes/training-native-prototype/.local/build-scene-viewer --target fdny-scene-viewer -j 4`.
Its output SHA-256 was
`514dd0ae15bb0c733f7b1b01dca939f26eb5e5d23c1696b187ec6f3af1eb059c`.
The `./tools/build_scene_viewer_app.sh rescue-all` packaging build completed,
validated its `Info.plist`, included the four dedicated hose scenes, and
generated an eight-entry rescue catalog. The packaged executable has the same
SHA-256 as the standalone target.
Linking emitted the existing Homebrew `json-c` warning that the dependency was
built for macOS 27.0 while this target links for 13.3. No test suite or
interactive run was performed; compilation confirms the source builds but does
not establish loader/runtime correctness or retail parity.

The later mode-timer update rebuilt the rescue-all bundle. That checkpoint's
packaged executable SHA-256 is recorded in
[`player_hose_mode_timer_2026-10-20.md`](player_hose_mode_timer_2026-10-20.md).

The anchor event and stream-source follow-up rebuilt the shared macOS target
and rescue-all app. Standalone and bundled executables match at SHA-256
`7fc114f3bee70cef91352ae49d85c694c892db8128156ad6759c760b84918018`; the
plist validated, all four hose maps are packaged, and the rescue catalog has
eight entries. The build emitted the existing Homebrew `json-c` deployment
warning (library built for macOS 27.0 while the target links for 13.3). No
interactive run, tests, or retail comparison were performed.

The steering-frame follow-up rotates a temporary copy of the rail-derived aim
basis around pitch axis 0 and then negative yaw around the updated axis 1.
The host matches `00496DD0`'s half-angle rotation through its equivalent
vector formula and uses the same `basis × axis` term as `004476B0`. SDL angle
mapping remains provisional. The rebuilt standalone and bundled executables
match at SHA-256
`6e639a00367ce7bff8a95da4b98b7fc6e4805c050c1399715527c32a43692bbc`. The
rescue-all plist passed validation; all four hose maps and eight rescue choices
remain packaged. This is a compile/package check only, not interactive or
retail parity evidence.

## Authored rail build

After the camera-rail port, the macOS target compiled successfully. The
standalone and rescue-all bundle executables have identical SHA-256
`597ca269667e3002d2411c40877f0d5c0eef1ec210f48a39a0f4a68c6a1ae3be`. The
package build validated `Contents/Info.plist`, retained all eight rescue
catalog rows, and included `city_h`, `const_h`, `dock_h`, and `oil_rig_h`.
Linking repeated the existing Homebrew json-c warning: the library was built
for macOS 27.0 while the target links for 13.3. No tests or interactive run
were performed; compilation does not establish camera timing or retail parity.

## Frame-clock source and pacing port (2026-10-20)

The timer alias trace resolves `00502b04` to the active WinMM clock record.
The shared loop now converts SDL millisecond differences with `0.001`, waits
until the stored delta exceeds `1/60`, and caps the exposed delta at `0.1`.
The source delta establishes `M_SpotTime`, its countdown, and each
`SprayTime` divisor are in seconds. The timer source and pause path are
documented in
[`player_hose_frame_delta_refs_2026-10-20.md`](player_hose_frame_delta_refs_2026-10-20.md).

The rescue-all macOS app rebuilt successfully. Its standalone and packaged
executable SHA-256 values match at
`7d04b10a561f5a1bafc53551ef601093a8a894d6c8a68498c076cc87e2a2f17b`. The
plist passed validation; all eight rescue choices and four hose scenes remain
packaged. The existing Homebrew json-c warning remains (built for macOS 27.0,
linked for 13.3). No tests, interactive run, or retail comparison were made.

## Pressure scalar build (2026-10-20)

The shared macOS target and rescue-all app rebuilt after the PlayerHose
pressure-state update. Standalone and bundled executable SHA-256 values match
at `3bb0928d0a5fa0974980aebeb314b4bf1d4fbf13c55bca29436eced9d674d36a`. The
plist passed validation; the rescue catalog still has eight rows, and all
four hose maps are packaged. The existing Homebrew json-c warning remains
(built for macOS 27.0, linked for 13.3). No tests, interactive run, or retail
comparison were made.

## Ballistic stream path follow-up (supersedes the earlier reach interpretation)

Fresh traces of `0046D1B0`, `0046C000`, `0046BAD0`, `0046BC10`, `0046C2F0`,
`0046C5A0`, `0046C480`, and `0046D6B0` separate stored velocity magnitude
(`4000 * currentPressure`) from final travel. The sample ring holds eight
records over 0.75 seconds and refreshes at 0.1 second intervals. The strip has
25 nonlinear time sections. Its main point is
`source + velocity*age + (0,-1000,0)*age^2`; the second curve uses
`(velocity + (0,-500,0))*(0.6*age)` plus the matching gravity term. Retail
submits each quad as previous-main, previous-secondary, current-secondary,
current-main, with white vertex channels and a 0.15 U span scrolling at six
units per second. At full pressure and horizontal aim, the main curve yields
3000 units of nominal travel and 562.5 units of downward displacement. The
host now samples both curves and draws the source-ordered quads while checking
the main path for collision. Renderer resource ID 3 is traced to a lookup
helper, but its bitmap, complete UV state, blend/material flags, and live
appearance remain open; the host's color and opacity are provisional.

## Ballistic stream build (2026-10-20)

The shared macOS executable and rescue-all app rebuilt successfully after the
ballistic path update. Standalone and packaged executable SHA-256 values match
at `214827038a147653b9ea541846b5e96bc7b7486b30f69e0c140bc0dc5bf8d804`.
`Info.plist` passed validation; all eight rescue choices and four PlayerHose
maps remain packaged. Linking retained the existing Homebrew json-c warning
(built for macOS 27.0 while the target links for 13.3). No tests, interactive
run, or retail comparison were performed.

## Dual-curve strip geometry build (2026-10-20)

After porting the traced secondary curve and quad vertex order, the shared
macOS target and rescue-all app rebuilt with matching standalone and packaged
executable SHA-256
`b2d7a1f802065a489636729ddb266a7e39df61bef528230ebe8123375bf77716`.
`Info.plist` passed validation, the packaged catalog reports eight retail
rescue choices, and all four PlayerHose maps are present. The existing
Homebrew json-c macOS 27.0 versus target 13.3 warning remains. No tests,
interactive run, or retail comparison were performed.

The shared macOS target and rescue-all bundle rebuilt with matching standalone
and packaged executable SHA-256
`39c681b93c13f6bfb79b1b82fbfc8a3f19f55d450992ba509f0521c1786789ac`. The
bundle plist passed, all eight rescue choices and four hose scenes remain
packaged, and the existing Homebrew json-c macOS 27.0 versus target 13.3
warning remains. No tests, interactive run, or retail comparison were made.
