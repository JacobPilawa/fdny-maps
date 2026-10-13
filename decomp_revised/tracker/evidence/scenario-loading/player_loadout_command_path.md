# Player tool loadout command path

## Result

The current-hash path connects authored rescue-map PlayerRescue scripts and shared EVS commands to Player inventory creation and selection. It identifies the exact axe, extinguisher, and goggles slots and shows that the older native prototype already reuses the same slot mapping and part of the item-switch gate. Its startup inventory is still hardcoded to axe, so the BSP-driven loadout contract has not yet been integrated into a map-neutral host.

Pinned EXE: FDNYFirefighter.exe, SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.
Shared script: Data/Scripts/rescue.evs, SHA-256 4b6d986fb9eb04f593956d7dcf3b8290ada0858a777dfaa29e204f1f4c0195c7.

## Authored map loadouts

The hash-checked rescue_actor_inventory.csv preserves each PlayerRescue entity's ordered S_Script epairs. These are the nonempty equipment commands across all 13 rescue, stage, and Training BSPs that contain PlayerRescue:

| BSP key | Ordered equipment commands |
|---|---|
| bank | equip axe |
| bar_r | equip axe |
| boat_a | equip axe |
| boat_b | equip axe |
| gas_r | equip axe; equip extinguisher |
| mbase | equip axe |
| oil_rig_a | equip goggles; equip axe |
| oil_rig_b | equip goggles; equip axe; equip extinguisher |
| oil_rig_c | equip goggles; equip axe; equip extinguisher |
| oil_rig_d | equip goggles; equip axe; equip extinguisher |
| subway_a | equip axe |
| subway_b | equip axe |
| training | equip axe |

All seven catalog rescue maps, their linked rescue stages, and Training are represented. The complete 17-BSP inventory remains in scope; the four hose maps use PlayerHose and are tracked separately.

## Script dispatch path

1. The PlayerRescue factory path in player_rescue_factory_and_spawn_path.md constructs the PlayerRescue BSP entity. InitializePlayerRescueEntity at 0044D550 registers its embedded ScriptActor with the exact name Player.
2. The initializer reads ten authored script slots and routes non-null entries in slot order through RouteParsedScriptActorCommand at 00470150. The Player ScriptActor vtable dispatches derived commands to HandlePlayerScriptActorCommand at 00451870.
3. Per-map EVS files that execute rescue.evs add the shared commands:
   - ifnot LEVEL training (if DIFFICULTY [0, 1] (tell player (equip goggles)))
   - tell player (select axe)
   Gas R is an exception: gas_r.evs executes doors.evs, bonusitems.evs, and refillext.evs, while the Gas R PlayerRescue BSP fields equip the axe and extinguisher directly.
4. The shared tell route resolves Player by name, parses the nested command, and reaches the same Player handler. The map-authored S_Script path and EVS tell path therefore converge on one equipment implementation.

## Queue order at mission start

The initial selected-map EVS pass runs before the `World` request is processed. For maps that execute shared `rescue.evs`, its early `tell player` equipment and selection commands run before any BSP PlayerRescue has registered the `Player` ScriptActor. The named-actor dispatcher does not queue a missing target, so these first-pass tells are dropped. The World factory is then processed, and WorldEntity initialization appends BSP spawn requests after that queue pump's drain phase. PlayerRescue initialization therefore occurs on a later queue-pump invocation. The delayed full mode-0 EVS pass occurs after four WorldEntity slot `+0x14` callbacks, but its ordering against the later pump is still open. See [entity_factory_queue_update_order.md](entity_factory_queue_update_order.md).

## Equipment slots and state changes

GetPlayerEquipmentSlotFromName at 0044CA50 maps axe to slot 1, extinguisher to slot 2, goggles to slot 3, and unsupported names to slot 0. The string table bytes at 004E188F contain the @goggles marker; the parser compares the goggles token at 004E1890.

HandlePlayerEquipCommand at 0044D080 allocates the matching object only when its slot is empty, binds the object to the Player through its vtable slot +0x08, stores it in the Player inventory beginning at +0x3564, then attempts selection with force=false. The mappings are:

| Item | Slot | Allocation | Initializer |
|---|---:|---:|---|
| axe | 1 | 0xA94 bytes | InitializePlayerAxeTool at 00460A50 |
| extinguisher | 2 | 0x8D0 bytes | InitializePlayerExtinguisherTool at 00461690 |
| goggles | 3 | 0x600 bytes | InitializePlayerGogglesTool at 00462960 |

A duplicate axe takes a separate existing-object callback path; duplicate extinguisher or goggles commands return without allocating a second item. A newly added item sends an equipment UI event and requests a sound after its selection attempt.

SelectPlayerEquipment at 0044D270 requires the named slot to be occupied, then calls SetPlayerSelectedEquipmentSlot at 0044C2A0. That helper records the selected slot at Player +0x357C. When switching from a different item, it asks the active item's vtable slot +0x38 whether it can deactivate. If allowed, it deactivates the old and activates the new item through slot +0x3C; if denied, the selection change does not occur. The actual returned state and subsequent frame behavior still depend on the item callbacks.

The order of authored S_Script slots is known, and the first-pass shared-EVS Player tells are now shown to run before Player registration and be dropped. The later BSP initialization order is mapped, but the WorldEntity callback timing relative to that later queue pump remains open. The final selected tool for each mission is not yet established; the shared script condition and active-item switch gate can affect that outcome.

## Reuse in the native prototype

The native prototype already uses the same one-based item slots in main.cpp: axe 1, extinguisher 2, goggles 3, and hose 4. Its Player command handler implements equip and select using HoseItemSelection::selectSlot. That reusable component models the active-item can-deactivate gate, including the axe-specific callback state. The host still initializes selected="axe" and equipment={"axe"} directly and reads Training assets. A later shared rescue host can retain the slot/state component while deriving starting inventory and ordered equip/select commands from each BSP and the shared scripts.

## Evidence and limits

Fresh Ghidra pseudocode and the raw tool-name bytes are in this directory:

- 0044ca50_player_equipment_name_to_slot_fresh.c
- 0044c2a0_set_player_selected_equipment_slot_fresh.c
- 0044d080_player_equip_command_fresh.c
- 0044d270_player_select_equipment_fresh.c
- 00460a50_initialize_player_axe_tool_fresh.c
- 00461690_initialize_player_extinguisher_tool_fresh.c
- 00462960_initialize_player_goggles_tool_fresh.c
- 0045f9f0_existing_axe_equip_branch_fresh.c
- 0044d550_player_rescue_entity_init_fresh.c
- 00451870_player_script_command_handler.c
- memory_004e188f_player_tool_names.json

Shared EVS dispatch and Player identity are documented in [named_script_actor_dispatch.md](named_script_actor_dispatch.md), [training_tell_recipient_boundary.md](training_tell_recipient_boundary.md), and [player_rescue_factory_and_spawn_path.md](player_rescue_factory_and_spawn_path.md).

This is a static command and state-path reconstruction. Live command timing, item animation/callback parity, resource readiness, and the final starting tool per scenario remain open. Hose-specific maps and mechanics remain part of the full port scope.
