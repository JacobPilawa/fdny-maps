# PlayerHose shared host slice (2026-10-20)

Status: **the shared game host loads the four dedicated PlayerHose maps,
resolves authored WindowFire target names to BSP model IDs, applies the
source-derived numeric mode timer scale, runs a source-shaped target/cycle/
hotspot state path, and follows the authored HosePath camera rail with a
floor-height query. Retail menu labels/initial mode, rail timing calibration,
mouse-to-nozzle mapping, live dousing response, WindowFire visuals, result
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
`FirstPathPoint` by following A links to the first hotspot, and follows D links
after each authored target cycle. The default macOS bundle includes the four
hose BSPs. Launch any one directly with its map key, for example
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

The host maps left-button hold to `MOUSE_LB`, traces a 2000-unit segment with
the same `0x43/2` world-collision arguments seen in the retail stream collision,
and only advances the currently selected target when the hit BSP model matches
that target's authored model. It decrements the hotspot countdown before the
dousing update, applies the authored SprayTime divisor, and presents the
current hotspot, target, cycle, and timer in the window title. WASD movement is
disabled in dedicated rail mode.

The host's WindowFire meter is intentionally only a first gameplay proxy. It
starts at 1.0 and decays by `dt / SprayTime`; `SprayTime <= 0` completes on the
first positive-delta contact. Retail initializes a related visual blend at
zero, then updates it through both burning and dousing paths, so the exact
douse-ready baseline has not been reproduced. The source-derived session mode
scale modifies M_SpotTime before it becomes the countdown; the countdown's
wall-clock units remain unresolved. Host mode index 2 is the default because
the retail selector and initial index are not yet mapped. Pitch is clamped to
the current N_PitchSpread in degrees; mapping mouse deltas to retail
STEER_PITCH/YAW remains provisional. `M_MaxPressure` is loaded but has no
effect until its downstream use is found. The simple blue beam and host
win/loss title are not retail WindowFire or result presentation.

The rescue catalog still validates and displays exactly eight `rescue.rrf`
rows. The four hose packages are available as direct launches from the bundle,
but a retail-authentic hose selector and result/return flow remain open.

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

## Authored rail build

After the camera-rail port, the macOS target compiled successfully. The
standalone and rescue-all bundle executables have identical SHA-256
`597ca269667e3002d2411c40877f0d5c0eef1ec210f48a39a0f4a68c6a1ae3be`. The
package build validated `Contents/Info.plist`, retained all eight rescue
catalog rows, and included `city_h`, `const_h`, `dock_h`, and `oil_rig_h`.
Linking repeated the existing Homebrew json-c warning: the library was built
for macOS 27.0 while the target links for 13.3. No tests or interactive run
were performed; compilation does not establish camera timing or retail parity.
