# Shared rescue completion and map exit path

Status: **static Player class-to-handler identity, command/completion routes, authored exit triggers across all 17 BSPs, BoundsTrigger admission, owner-pass phase order, retail pending-key identity, and the pre-load runtime-flush sequence are mapped; the shared escape transaction and a sibling-package adapter for authored `finalexitloadmap` chains are integrated in the preview; live allocation/overlap, wall-clock cadence, transition completion, cross-stage state retention, and parity remain open.**

All executable observations use `FDNYFirefighter.exe` SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The [exit-trigger inventory](../rescue_exit_trigger_inventory.md) is generated from copied BSP files, checks each source hash against `scene_load_coverage.csv`, and retains complete ordered entity epairs.

## Map-authored exits

Across the 17 BSP assets, **40** `RCS_BoundsTrigger` entities have an `escape`, `finalexit`, or `finalexitloadmap` event binding. All 40 textual `C_Model` names join exactly to same-BSP `%Model%` entities; the CSV retains model entity index, BSP model index/origin, `ZZ_location`, `E_Reach`, script fields, and full epairs.

- 33 trigger entities author `tell player escape`.
- Two author `tell player finalexit` (Bank and Boat B). Bank's trigger also has an `escape` event.
- Six author `finalexitloadmap`: Boat A → Boat B, Subway A → Subway B, and Oil Rig A → B → C → D.
- Training, MBase, Bar R, Gas R, Boat A/B, Subway A/B, Bank, and all four Oil Rig stages retain map-specific exit triggers in the ledger. `City_h`, `const_h`, `dock_h`, and `oil_rig_h` have no matching exit scripts in their BSP entities.

The generic spawn queue registers each successfully created root through
`00429490`; BoundsTrigger's constructor mask includes owner Think filter value
2, and its type/category also reaches contact list 8 through manager filter
`0x20`. This statically supports admission on the successful factory path, not
runtime allocation or a live overlap on any particular map. The asset-level
stage list remains part of the port scope, including Boat B, Subway B, and Oil
Rig B/C/D.

## Shared `escape` path

The shared `tell` dispatcher looks up the first ScriptActor whose registered name matches the command's first token, then sends the second token through that actor's ScriptActor dispatcher. That dispatcher falls through to the target class's vtable slot `+0x04`. The Player handler at `0x00451870` has a case-insensitive `escape` branch which calls `DispatchPlayerEscapeCommand` (`0x004513A0`). See [named actor dispatch](named_script_actor_dispatch.md).

`DispatchPlayerEscapeCommand` applies a Player frame/state prepass and calls `ProcessCarriedVictimRescue` (`0x00451160`). That routine returns when the carry field at Player `+0x3534` is empty. Otherwise it updates `Rescues`, `Longest Rescue Time`, and `Shortest Rescue Time`; calls victim release at `0x0045C000`; updates Player status and the rescue deadline; clears the carry pointer; resets carry effects; then calls `UpdateFinalExitAvailability` (`0x004504C0`). The older address-specific exports in this directory were rechecked against the same hash and this current Ghidra analysis; their behavior is reused without assuming the old Training host is the full port.

BoundsTrigger pair production runs in the owner contact manager at `+0xE0`;
its embedded ScriptActor pair drain runs through root slot `+0x14` in the
owner `+0x50` Think manager, which precedes contact production. A `touch` pair
produced in one normal owner invocation is therefore available to the script
event dispatcher on a later normal owner invocation. The shared host now
buffers these pairs across one paced update iteration; that interval remains
provisional. See the [owner scheduler trace](root_plus14_owner_scheduler_trace_2026-10-20.md).

The victim release helper's ordered actor/frame/timer/event effects are modeled by the C++ [`victim_release.h`](../../../port/spikes/training-native-prototype/src/victim_release.h), with frame math in [`victim_frame_ops.h`](../../../port/spikes/training-native-prototype/src/victim_frame_ops.h). `VictimTransactions` composes them behind the shared viewer host; Training-prefixed source-derived type names do not make the mechanic Training-only.

## Preview integration checkpoint (2026-10-13)

The shared preview's EVS host now routes `tell player escape` to
`VictimTransactions::escape()`. It synchronizes preview victim state into the
source-derived transaction, supplies the player frame and BSP floor trace,
dispatches the victim's `rescue` event, updates rescue statistics, clears carry
state, and evaluates the mapped final-exit gate. Trigger contact in the
interactive loop dispatches the authored touch binding through this EVS path.

The packaged headless `--rescue-check` passes on Training, MBase, Bar R, Gas R,
Bank, Boat A, Subway A, and Oil Rig A. It iterates every authored victim on
each primary map, queues the mapped Player pickup message `0x7532`, then
dispatches an authored `touch` binding for `tell player escape`. Each check
requires the rescue count to reach the authored victim total, every victim to
finish in state 6 with actor flags `0x400`, and the carried-victim pointer to
clear. The verified counts are Training 2/2, MBase 5/5, Bar R 6/6, Gas R 7/7,
Bank 6/6, Boat A 9/9, Subway A 6/6, and Oil Rig A 3/3. These checks exercise
the primary packages' EVS startup and shared pickup/release transaction; they
do not simulate natural travel/contact, retail callback scheduling, the
five-minute mission objective, linked-stage progression, visual/audio parity,
or full mission completion. To keep the check headless, it loads ACT data but
skips renderer/OpenGL texture setup.

The packaged MBase check uses authored BoundsTrigger 159 (`escape1`, bounds
center `(1906, -124, -2788)`) as its escape binding. After the fifth release,
the MBase exit gate reaches `messageGate=1`; the map has no authored
`finalexit` or `finalexitloadmap` binding and no enabled final-exit trigger.
The no-zone branch calls `0044C360`, which is not evidence of a mission outro.
Individual release placement is exercised by the transaction, but the all-five
check's acceptance criterion is the count/state/flag/carry result rather than
a particular floor-placement coordinate.

## Plain-language explanation

The harness asks the same mapped Player message used for picking up a victim,
then directly dispatches the map's authored escape trigger. That proves each
victim can pass through the shared pickup and release code when the relevant
events are supplied. It does not prove the player can walk to victims, reach
the trigger naturally, or finish the mission on time. MBase has no authored
final exit; rescuing all five opens a message/input gate and refreshes the
inventory, but the retail meaning of that branch as a mission ending is not
verified.

## Shared final-exit gates

`UpdateFinalExitAvailability` refreshes zone membership and lazily resolves the total-victim count. Once its rescue threshold passes, it scans active type-8 entities for `touch` bindings matching `tell player finalexit` or `tell player (finalexitloadmap`, maps their bounds centers to zones, deduplicates exit zones, and updates the prompt/trigger state. If no zone exists, it sets Player `+0x3438` and calls `0x0044C360`; that call is not proof of a complete outro. The source-derived C++ [`victim_exit.h`](../../../port/spikes/training-native-prototype/src/victim_exit.h) models this shared gate with zone and engine operations supplied as adapters.

The viewer's `refreshSelectedItem` service now calls
`HoseItemSelection::reselectFirst(..., force=true)` after this no-zone branch,
then applies the existing goggles-selection side effect. This wires the
source-mapped inventory refresh; it does not assign a mission-complete meaning
to Player `+0x3438` or show a claimed retail outro.

The `finalexit` command reaches `HandleFinalExitCommand` (`0x00450A00`). It computes `total victims - field +0x3530 - rescued count +0x352c`. A zero result sets `+0x3438` and calls `0x0044C360`; otherwise the remaining-count value is forwarded to `0x004564C0` with `7.0`. This path is authored on Bank and Boat B.

The viewer now follows both branches: an incomplete exit shows the remaining count for seven seconds, while a complete exit sets the shared gate and refreshes the first owned inventory slot through the existing `refreshSelectedItem` adapter. The complete branch does not add a success hint; the source trace does not show one. The controlled rescue check dispatches authored final-exit triggers before and after the last release to verify the timer, gate, and item reselection.

For an accepted standalone `finalexit`, the shared host also requests the
source-mapped one-entry `RC_ResLvlWin0` chatter and Player-owned `RescueWin`
sound once. The matching resources are included in every scene bundle. This
event remains distinct from the no-exit-zone state-1 write and from
`finalexitloadmap`, which continues to another stage. The host dispatches the
audio during the EVS command invocation; exact retail callback cadence is not
verified. See [`rescue_win_feedback_port_2026-10-20.md`](rescue_win_feedback_port_2026-10-20.md).

The `finalexitloadmap` command reaches `HandlePlayerFinalExitLoadMap` (`0x00450A70`). It checks `data\\maps\\<key>.bsp`, applies the same remaining-victim gate, and, when accepted, queues a pending level request with the original command key and `data\\resdefs\\rescue.rrf`. Raw stack offsets confirm the same key is checked and stored at EverGame `+0x660`; see [the key dataflow trace](finalexitloadmap_pending_key_dataflow_2026-10-20.md). Fresh headless Ghidra tracing maps the in-process flush sequence called before the new World request is queued; see [level transition flush](level_transition_flush_2026-10-21.md). Live stage handoff, replacement presentation, and cross-stage state retention remain open.

The terminal `finalexit` and `finalexitloadmap` handlers both compute the
remaining count with two 32-bit subtractions and accept only an exact zero
remainder. The shared host now uses this wraparound rule for both handlers;
the previous widened-and-clamped calculation could accept an over-counted
state. The shared exit-availability path uses the same wrapped arithmetic
before its signed-positive check. This is a static-source correction and has
not been checked against corrupted counters or natural retail gameplay. See
[`rescue_final_exit_wrap_arithmetic_2026-10-21.md`](rescue_final_exit_wrap_arithmetic_2026-10-21.md).

The shared viewer has a bounded adapter for this route: after the remaining-victim gate, it resolves the authored key to a sibling exported scene package and continues the same rescue run. This covers the authored Boat A→B, Subway A→B, and Oil Rig A→B→C→D chains. A controlled `--rescue-check` dispatches all six authored `finalexitloadmap` touch bindings after all victims are rescued, verifies that each key loads a matching sibling package, and confirms the command remains gated after the first rescue. All 13 bundled rescue/stage packages pass their rescue checks. The adapter uses the authored command key, matching the static retail request path. Live exit contact, trigger enable timing, transition completion, and full mission completion remain open. See [the stage transition port checkpoint](rescue_stage_transition_port_2026-10-14.md).

Fresh tracing of the Player status callback confirms that Player `+0x3438` is
also read as a result state: value `1` dispatches win feedback and value `2`
dispatches loss feedback while the status is visible. Direct final-exit writers
set `1`; the countdown path writes `2` on timeout. The shared host requests
source-confirmed win audio on accepted standalone `finalexit` and the two
timeout loss sounds once, but does not yet render the result widget or enter
the frontend return flow. Widget lifetime/presentation, replay cadence, and
full result-to-frontend behavior remain unresolved. See
[`mission_outcome_status_dispatch_2026-10-19.md`](mission_outcome_status_dispatch_2026-10-19.md).

## Current uncertainty and next trace

- `tell player ...` uses first-match, case-insensitive actor-name lookup. Static factory, class, and vtable evidence resolves rescue-map `Player` to `RCS_PlayerRescue`; the separate `RCS_PlayerHose` class appears in hose-map BSPs. Live load execution, insertion order, and command delivery timing still need a runtime capture. See [named actor dispatch](named_script_actor_dispatch.md), the [PlayerRescue factory path](player_rescue_factory_and_spawn_path.md), and the [hash-checked 17-BSP class inventory](../rescue_exit_trigger_inventory.md).
- The static BoundsTrigger factory/registration route and `+0x50`-before-`+0xE0` phase boundary are mapped. Live allocation, overlap, within-list order, the interval between owner passes, and natural completion timing still need runtime evidence.
- The original game has not been captured live for these routes. A Wine/Sikarugir capture or a Windows capture is still needed for visual/audio/mission parity.
- Complete carried-body presentation and live exit contact, then exercise the authored exits in every required rescue map and stage. The initial sibling-package stage adapter is implemented but needs user/live contact confirmation. Keep the seven campaign rescues, Training, all other catalog scenarios, and all 17 BSP assets in scope.

## Fresh evidence files

- [Final-exit availability/update](004504c0_update_final_exit_availability_fresh.c)
- [Final-exit command gate](00450a00_handle_final_exit_command_fresh.c)
- [PlayerRescue entity initializer and `Player` registration](0044d550_player_rescue_entity_init_fresh.c)
- [Player rescue completion](00451160_player_rescue_complete.c)
- [Player escape prepass](004513a0_player_escape_prepass.c)
- [Player ScriptActor command handler](00451870_player_script_command_handler.c)
- [Map-stage request helper](00450a70_player_finalexitloadmap_fresh.c)
- [Per-entity exit trigger inventory](../rescue_exit_trigger_inventory.csv)
