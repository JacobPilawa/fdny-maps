# Shared rescue completion and map exit path

Status: **static Player class-to-handler identity, command/completion routes, and authored exit triggers across all 17 BSPs are mapped; runtime factory admission, scheduler order, and parity remain open.**

All executable observations use `FDNYFirefighter.exe` SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The [exit-trigger inventory](../rescue_exit_trigger_inventory.md) is generated from copied BSP files, checks each source hash against `scene_load_coverage.csv`, and retains complete ordered entity epairs.

## Map-authored exits

Across the 17 BSP assets, **40** `RCS_BoundsTrigger` entities have an `escape`, `finalexit`, or `finalexitloadmap` event binding. All 40 textual `C_Model` names join exactly to same-BSP `%Model%` entities; the CSV retains model entity index, BSP model index/origin, `ZZ_location`, `E_Reach`, script fields, and full epairs.

- 33 trigger entities author `tell player escape`.
- Two author `tell player finalexit` (Bank and Boat B). Bank's trigger also has an `escape` event.
- Six author `finalexitloadmap`: Boat A → Boat B, Subway A → Subway B, and Oil Rig A → B → C → D.
- Training, MBase, Bar R, Gas R, Boat A/B, Subway A/B, Bank, and all four Oil Rig stages retain map-specific exit triggers in the ledger. `City_h`, `const_h`, `dock_h`, and `oil_rig_h` have no matching exit scripts in their BSP entities.

These are asset/script facts, not proof that a factory admitted the trigger or that its touch callback fires on a specific frame. The asset-level stage list remains part of the port scope, including Boat B, Subway B, and Oil Rig B/C/D.

## Shared `escape` path

The shared `tell` dispatcher looks up the first ScriptActor whose registered name matches the command's first token, then sends the second token through that actor's ScriptActor dispatcher. That dispatcher falls through to the target class's vtable slot `+0x04`. The Player handler at `0x00451870` has a case-insensitive `escape` branch which calls `DispatchPlayerEscapeCommand` (`0x004513A0`). See [named actor dispatch](named_script_actor_dispatch.md).

`DispatchPlayerEscapeCommand` applies a Player frame/state prepass and calls `ProcessCarriedVictimRescue` (`0x00451160`). That routine returns when the carry field at Player `+0x3534` is empty. Otherwise it updates `Rescues`, `Longest Rescue Time`, and `Shortest Rescue Time`; calls victim release at `0x0045C000`; updates Player status and the rescue deadline; clears the carry pointer; resets carry effects; then calls `UpdateFinalExitAvailability` (`0x004504C0`). The older address-specific exports in this directory were rechecked against the same hash and this current Ghidra analysis; their behavior is reused without assuming the old Training host is the full port.

The victim release helper's ordered actor/frame/timer/event effects are already modeled by the C++ [`victim_release.h`](../../../port/spikes/training-native-prototype/src/victim_release.h), and its frame math by [`victim_release_frame.h`](../../../port/spikes/training-native-prototype/src/victim_release_frame.h). Those components use callback adapters and can move behind the shared rescue runtime; their current Training-prefixed type names and host integration are not evidence that the mechanic is Training-only.

## Shared final-exit gates

`UpdateFinalExitAvailability` refreshes zone membership and lazily resolves the total-victim count. Once its rescue threshold passes, it scans active type-8 entities for `touch` bindings matching `tell player finalexit` or `tell player (finalexitloadmap`, maps their bounds centers to zones, deduplicates exit zones, and updates the prompt/trigger state. If no zone exists, it sets Player `+0x3438` and calls `0x0044C360`; that call is not proof of a complete outro. The source-derived C++ [`victim_exit.h`](../../../port/spikes/training-native-prototype/src/victim_exit.h) models this shared gate with zone and engine operations supplied as adapters.

The `finalexit` command reaches `HandleFinalExitCommand` (`0x00450A00`). It computes `total victims - field +0x3530 - rescued count +0x352c`. A zero result sets `+0x3438` and calls `0x0044C360`; otherwise the remaining-count value is forwarded to `0x004564C0` with `7.0`. This path is authored on Bank and Boat B.

The `finalexitloadmap` command reaches `HandlePlayerFinalExitLoadMap` (`0x00450A70`). It checks `data\\maps\\<key>.bsp`, applies the same remaining-victim gate, and, when accepted, queues a pending level request with `data\\resdefs\\rescue.rrf`. The pending-level update and World loader route are statically mapped in [the pending-level evidence](evergame_pending_load_update.md). Equality between the checked key and the forwarded pending map-name context, runtime stage handoff, and transition timing are still open.

## Current uncertainty and next trace

- `tell player ...` uses first-match, case-insensitive actor-name lookup. Static factory, class, and vtable evidence resolves rescue-map `Player` to `RCS_PlayerRescue`; the separate `RCS_PlayerHose` class appears in hose-map BSPs. Live load execution, insertion order, and command delivery timing still need a runtime capture. See [named actor dispatch](named_script_actor_dispatch.md), the [PlayerRescue factory path](player_rescue_factory_and_spawn_path.md), and the [hash-checked 17-BSP class inventory](../rescue_exit_trigger_inventory.md).
- The shared overlap/contact manager and authored trigger records are mapped, but trigger factory admission, dispatch cadence, `touch` event order, message delivery frame, and completion timing still need runtime evidence.
- The original game has not been captured live for these routes. A Wine/Sikarugir capture or a Windows capture is still needed for visual/audio/mission parity.
- Promote the already ported release/exit components into a mission-neutral host contract, then exercise the authored exits in every required rescue map and stage. Keep the seven campaign rescues, Training, all other catalog scenarios, and all 17 BSP assets in scope.

## Fresh evidence files

- [Final-exit availability/update](004504c0_update_final_exit_availability_fresh.c)
- [Final-exit command gate](00450a00_handle_final_exit_command_fresh.c)
- [PlayerRescue entity initializer and `Player` registration](0044d550_player_rescue_entity_init_fresh.c)
- [Player rescue completion](00451160_player_rescue_complete.c)
- [Player escape prepass](004513a0_player_escape_prepass.c)
- [Player ScriptActor command handler](00451870_player_script_command_handler.c)
- [Map-stage request helper](00450a70_player_finalexitloadmap_fresh.c)
- [Per-entity exit trigger inventory](../rescue_exit_trigger_inventory.csv)
