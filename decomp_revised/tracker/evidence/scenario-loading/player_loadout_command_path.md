# Player tool loadout command path

## Result

The current-hash path connects authored rescue-map PlayerRescue scripts and shared EVS commands to Player inventory creation and selection. It identifies the exact axe, extinguisher, and goggles slots and shows that the native workbench already reuses the same slot mapping and part of the item-switch gate. The shared scene viewer now processes PlayerRescue script fields in order, routes `equip` and `select` through that slot state, and supports viewer keys 1/2/3 for choosing an equipped tool. Goggles selection and left-click populate a source-filtered nearby actor scan list; the shared viewer renders resource 703's heatpulse texture for the verified target-effect window, while retail actor-local transform, child attachment, animation, and exact placement remain open.

### Plain-language explanation

The first level-script pass can run before the Player exists, so its equipment messages have no recipient. Retail runs the full level-script pass again after four World updates. The shared viewer now follows that delayed order; controlled checks confirm each rescue package gets the equipment written in its BSP or shared script. Retail frame timing and the final selected tool remain unverified.

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

The initial selected-map EVS pass runs before the `World` request is processed. For maps that execute shared `rescue.evs`, its early `tell player` equipment and selection commands run before any BSP PlayerRescue has registered the `Player` ScriptActor. The named-actor dispatcher does not queue a missing target, so these first-pass tells are dropped. The World factory is then processed, and WorldEntity initialization appends BSP spawn requests after that queue pump's drain phase. PlayerRescue initialization therefore occurs on a later queue-pump invocation. Retail's delayed full mode-0 EVS pass follows four WorldEntity slot `+0x14` callbacks. The shared viewer now runs this second pass after four host World updates, after PlayerRescue registration. Its map checks confirm equipment inventory, while exact retail callback cadence and final selection parity remain open. See [entity_factory_queue_update_order.md](entity_factory_queue_update_order.md) and [delayed_world_evs_pass_port_2026-10-15.md](delayed_world_evs_pass_port_2026-10-15.md).

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

The rescue-player HoseSpray is a separate command path from `equip`: handler
`00451870` recognizes `touchhose`, then `0044CAB0` creates the `0xD54`-byte
HoseSpray item in inventory slot 4 and dispatches its hose mode. The regular
name-to-slot helper still rejects `equip hose`. Raw-string scanning found 16
authored `touch: tell player (touchhose N)` commands on `RCS_PropModel` entities
in 12 preserved BSPs; PropModel contact dispatch and named `tell player`
resolution are covered by existing static traces. The shared viewer already
dispatches PropModel touch events and interprets nested `tell` commands, but
its Player host now handles `touchhose N`, creates a slot-4 HoseSpray loadout
entry, and models its source-derived attachment-list count. The item frame
attachment, continuous update, and FireRescue contact remain unported. `touchhose N` supplies N to `00482780` for both new and
existing items; the helper converts `N * 24 / (item+0xCB0)` toward zero and
passes the count to the `0x30`-byte attachment-list builder `00485D20`. The
list records' purpose and argument units remain unresolved. The map evidence,
full BSP hashes, and verified event route are recorded in
[the authored trigger audit](../fire-spread/touchhose_authored_trigger_audit_2026-10-20.md).
The count formula is detailed in
[the attachment-list trace](../fire-spread/touchhose_attachment_list_trace_2026-10-20.md).
Pinned contact/dousing behavior and update ordering are tracked in
[the hose contact-frame audit](../fire-spread/hose_contact_frame_gap_2026-10-20.md).

## Shared viewer integration

`scene_viewer.cpp` now registers the authored `RCS_PlayerRescue` fields with the EVS host after constructing the shared Player actor. The ten ordered `S_Script` values therefore drive equipment state through the same `equip`/`select` command handlers used by named Player tells. The host uses `HoseItemSelection::State` for occupied slots, current selection, and its existing active-item switch gate. It also routes the source-mapped `setashdensity` and `setrescuebot` fields used by Training and Gas R. The same event and named-actor path reaches authored PropModel `touchhose` commands; the Player host now creates the slot-4 loadout state and source-shaped attachment-list count. Retail item frame attachment, continuous update, action effects, and FireRescue contact remain unported.

Keys 1/2/3 select the authored axe/extinguisher/goggles slots when present. This is a viewer input choice, not a recovered retail key mapping. The window title reports the selected slot. The extinguisher's held left-click connects the recovered active pose sampler and four fire contact points to the message-30000 suppression route; its parent frame and rendered-frame cadence remain provisional. The equipment ACT assets now render: the extinguisher uses a separate contact/action frame with a provisional 0.25 display scale, and axe left-click advances the active child actor clock and samples the exported `swing` track candidate; the retail callback does not explicitly name a motion. Retail Axe's root `+0x1FC` gate starts at zero, so ordinary first equip activates secondary model 1 `axe.act`; duplicate equip enables primary model 6 `axel.act` as well. The shared viewer now matches that gate and asset selection. The extinguisher also uses its direct camera-root candidate. Their camera-space parent matrices, hand placement, scale, exact action timing, and Axe hit/damage behavior remain open; see [held-item orientation follow-up](held_item_and_fire_camera_followup_2026-10-14.md). Goggles run the mapped kind-2/kind-6 range filter, track current host target positions, and render the copied retail heatpulse texture for the verified 8.5-second window. Fresh Ghidra output for `00462A60` also confirms 640x32 screen rectangles for resource IDs 300/301; the copied alpha masks now draw while goggles are selected. Their tint and viewport adaptation are provisional. The Player hint widget now uses the retail resource-1020 atlas and alpha map, parses RGB markup, and follows the strict authored lifetime; the synthetic panel is removed. Mode-2 placement, wrapping and tick rollover remain open. The helper's generic item deactivation defaults do not substitute for each retail item's full vtable behavior. See [extinguisher_scene_viewer_runtime.md](extinguisher_scene_viewer_runtime.md), [goggles_scan_viewer_runtime.md](goggles_scan_viewer_runtime.md), and [player_hud_goggles_equipment_fire_viewer_runtime.md](player_hud_goggles_equipment_fire_viewer_runtime.md).

The mode-1 level pass loads EVS definitions before entity startup, and the shared host now schedules the full mode-0 pass after four World updates. This lets shared `rescue.evs` equipment commands reach the registered Player. The native view confirms authored inventory on all 13 rescue/stage/Training packages; final retail selection parity and callback cadence remain open.

## Reuse in the native prototype

The Training workbench uses the same one-based item slots in `main.cpp`: axe 1, extinguisher 2, goggles 3, and hose 4. Its Player command handler and the shared viewer use `HoseItemSelection::selectSlot`. Training remains a separate host; the shared viewer now derives its rescue loadout from the BSP PlayerRescue entity.

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

Static command paths are mapped, and BSP-authored starting equipment is integrated into the shared viewer. Live command timing, extinguisher draw-root/cadence parity, particle/audio presentation, axe hit behavior, goggles child animation/target placement, resource readiness, and the final starting tool per scenario remain open. Hose-specific maps and mechanics remain part of the full port scope.
