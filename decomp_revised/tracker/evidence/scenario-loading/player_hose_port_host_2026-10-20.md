# PlayerHose shared host slice (2026-10-20)

Status: **the shared game host loads the four dedicated PlayerHose maps,
resolves authored WindowFire target names to BSP model IDs, applies the
source-derived numeric mode timer scale, runs a source-shaped target/cycle/
hotspot state path, and follows the authored HosePath camera rail with a
floor-height query. Retail menu labels/initial mode, rail timing calibration,
mouse-to-nozzle mapping, live dousing cadence, WindowFire visuals, result
widget, and stage transition remain open.**
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

The host maps left-button hold to `MOUSE_LB`, traces a 2000-unit segment from
the retail-shaped `00465EF0` frame offset with the same `0x43/2`
world-collision arguments seen in the retail stream collision, and only
advances the currently selected target when the hit BSP model matches that
target's authored model. The temporary aim basis follows retail's pitch-around-
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
frame-delta calibration, and live contact cadence remain provisional. The
source-derived session mode scale modifies M_SpotTime before it becomes the
countdown; the countdown's wall-clock units remain unresolved. Host mode index 2 is the default because
the retail selector and initial index are not yet mapped. Pitch is clamped to
the current N_PitchSpread in degrees; mapping mouse deltas to retail
STEER_PITCH/YAW remains provisional. `M_MaxPressure` is loaded but its
downstream use remains unknown. A fresh exact `+0x1834` instruction-text
scan finds only the assignment at `00467B8A`; direct register aliases are not
excluded. The simple blue beam and host
win/loss title are not retail WindowFire or result presentation.

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

A headless instruction-text scan against retail EXE SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` found
only the assignment to PlayerHose `+0x1834` at `00467B8A`; this does not rule
out computed or aliased consumers. The full scan scope is in
[`player_hose_pressure_scan_2026-10-20.md`](player_hose_pressure_scan_2026-10-20.md).

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
