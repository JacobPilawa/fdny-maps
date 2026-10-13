# Per-level entity factory queue and EVS ordering

## Result

The initial selected-level EVS pass runs before the `World` factory request is processed. The request is then drained during the same `ProcessEverGameUpdate` invocation. `WorldEntity` loads the BSP in the queue's initialization pass and appends map-entity factory requests only after that pump's queue-drain phase has ended. Those BSP entities, including `PlayerRescue`, therefore require a later entity-factory queue pump.

For maps whose mode-1 EVS executes `rescue.evs`, its early `tell player (equip goggles)` and `tell player (select axe)` commands run before the Player ScriptActor is registered. The named-actor dispatcher returns when it finds no target and does not retain the command. The later full mode-0 EVS pass is attached to the WorldEntity and is triggered after four calls to its root slot `+0x14`. The relative timing of those four callbacks versus the later BSP entity pump remains unresolved, so the final mission-start tool state is not yet established.

This queue path is shared across the campaign rescue maps, linked stages, Training, and the other BSP loads. Gas R remains a script exception: its map EVS does not execute shared `rescue.evs`, and its axe/extinguisher loadout is authored on its PlayerRescue BSP record.

## Queue service and pump

Pinned executable: `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

1. `InitializeEntityFactoryQueueService` (`0042B350`) installs primary vtable `004E4158` on the service and secondary vtable `004DF8F8` on the nested queue interface at service `+0x40`. Raw table bytes are saved in `memory_004e4158_entity_factory_service_vtable.json` and `memory_004df8f8_entity_spawn_queue_vtable.json`.
2. Primary slot `+0x04` maps to `SetEntityFactoryRequestPending` (`0042B1A0`). For a non-null request, it increments the queue count at service `+0x18`, allocates a 0x0c-byte linked-list node, stores the request pointer and flag, and appends it at the tail. It does not create the entity.
3. `ProcessEverGameUpdate` (`0042AEA0`) handles a pending level-load request through `ProcessLevelLoadRequest` (`0042AB00`), then calls `ProcessEntityFactoryQueues` (`00429520`). The latter dispatches the nested queue interface's slot `+0x04`; the secondary table maps that slot to `ProcessEntitySpawnQueue` (`0049C670`). This proves the queue pump runs later in the same update-method invocation as the selected-level request.
4. `ProcessEntitySpawnQueue` first drains the queued records into a local list, constructing and registering each entity. Only after the drain loop finishes does it run a complete root slot `+0x20` pass and then a complete root slot `+0x24` pass.
5. The World factory request is first in that queue. WorldEntity slot `+0x20` calls `WorldEntityLoad` (`0049E4D0`), which parses the BSP and calls the service's primary slot `+0x04` for each authored map entity. Those new requests are appended during the later initialization pass, after the pump's drain loop. The current pump does not return to its drain phase, so it leaves the BSP requests for a subsequent `ProcessEntityFactoryQueues` invocation.
6. When a `PlayerRescue` factory request is eventually drained, its `+0x20` handler registers the embedded ScriptActor as `Player` before later `tell` dispatches can resolve it. This connects the queue ordering to the authored per-map loadouts documented in [player_loadout_command_path.md](player_loadout_command_path.md).

## EVS consequences and open timing

`ProcessLevelLoadRequest` calls `00471310` with mode 1 on the selected `.evs` path before it creates and queues the `World` request. The mode-1 parser admits `set` and `exec`; an `exec rescue.evs` therefore performs the nested mode-0 load immediately, including its Player-targeted commands. At that point no World or map Player entity has gone through the later queue pump.

`DispatchScriptCommandToNamedActor` (`00475560`) performs a name lookup and returns zero if no actor matches. It has no pending-target queue. Therefore these first-pass Player tells are dropped. The WorldEntity's delayed mode-0 parser call is triggered when its `+0x2C` counter reaches zero after four slot `+0x14` invocations. Static evidence now establishes that BSP entities require another queue-pump invocation, but not the exact invocation at which the WorldEntity receives its fourth callback. Do not infer rendered frames, elapsed time, or final selected equipment from the four-call threshold alone.

## Evidence files

- `0042b1a0_set_entity_factory_request_pending_fresh.c` and `.asm`
- `0042b350_initialize_entity_factory_queue_service_fresh.c` and `.asm`
- `00429520_process_entity_factory_queues_fresh.c` and `.asm`
- `0049c670_entity_spawn_queue_fresh.c`
- `0049e4d0_world_entity_load.c`
- `memory_004e4158_entity_factory_service_vtable.json`
- `memory_004df8f8_entity_spawn_queue_vtable.json`
- `player_loadout_command_path.md`
- `worldentity_callback_dispatch_boundary.md`
