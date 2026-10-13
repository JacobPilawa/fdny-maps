# PlayerRescue factory and BSP spawn path

The successful static creation path is mapped from startup registration through
the Player command handler. It uses the same WorldEntity loader and spawn queue
already traced for shared rescue entities. This is a hash-pinned code/data path,
not a live session trace.

## Pinned inputs

- `FDNYFirefighter.exe`, SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- All 17 BSP hashes are checked against `scene_load_coverage.csv` by the
  generated [map inventory](../../rescue_exit_trigger_inventory.md).

## Successful class-to-handler path

1. Startup `0042B4E0` calls `RegisterEntityClassFactories` (`004297E0`). That
   routine constructs `InitializePlayerRescueFactoryDescriptor` (`0044BFF0`)
   and appends it to the shared entity-class registry. The descriptor stores
   the exact class key `RCS_PlayerRescue`.
2. The PlayerRescue descriptor vtable at `004E1860` has `00454080` in slot
   `+0x08`. `CreatePlayerRescueEntity` allocates `0x3580` bytes and calls
   `ConstructPlayerRescueEntity` (`00453D50`). The class constructor installs
   the PlayerRescue root vtable at object `+0x00` and the embedded ScriptActor
   vtable at object `+0x140`.
3. `WorldEntity` (`0049E4D0`) enumerates BSP entity records against registered
   class descriptors and queues matches. The shared
   `ProcessEntitySpawnQueue` (`0049C670`) calls each queued descriptor's slot
   `+0x08`, registers the returned entity, then performs two ordered passes:
   root vtable slot `+0x20` for every spawned entity, followed by slot `+0x24`
   for every entity. The generic World request and this queue order are mapped
   in [World factory resolution](world_factory_resolution.md); the queue's
   fresh pseudocode is [here](0049c670_entity_spawn_queue_fresh.c).
4. In PlayerRescue's root vtable address point at `004E1C48`, slot `+0x20`
   points to `InitializePlayerRescueEntity` (`0044D550`). That first pass
   registers the embedded ScriptActor at entity `+0x140` under the exact name
   `Player` and applies the parsed PlayerRescue fields. The embedded ScriptActor
   vtable address point at `004E1C40` has `HandlePlayerScriptActorCommand`
   (`00451870`) at slot `+0x04`. A script `tell player escape` therefore has a
   static class path to the Player handler and then to shared rescue completion.

Raw vtable bytes captured from Ghidra are preserved in
[`memory_004e1860_player_rescue_factory_vtable.json`](memory_004e1860_player_rescue_factory_vtable.json)
and [`memory_004e1c40_player_rescue_vtables.json`](memory_004e1c40_player_rescue_vtables.json).
The function pseudocode is saved in the adjacent `0044bff0`, `00454080`,
`00453d50`, and `0044d550` exports.

## BSP coverage and limits

The hash-checked inventory finds 13 `RCS_PlayerRescue` entities: one in each
rescue map and transition-stage BSP (`bank`, `bar_r`, `boat_a`, `boat_b`,
`gas_r`, `mbase`, `oil_rig_a` through `oil_rig_d`, `subway_a`, `subway_b`, and
`training`). The four hose BSPs (`city_h`, `const_h`, `dock_h`, and `oil_rig_h`)
each contain one `RCS_PlayerHose` and no `RCS_PlayerRescue`. It finds zero exact
`Player` epair values, so this identity is supplied by the class initializer.

This closes the static successful-path factory and class-to-handler map for all
required rescue BSPs, including Training and all stage maps. It does not observe
a live game session, prove failure-path behavior, or determine scheduler frame
cadence and visible parity. The port scope remains all seven campaign rescues,
Training, every stage BSP, and every rescue mechanic.

## Reused evidence and implementation

The generic loader, queue, and entity registration are reused from
[World factory resolution](world_factory_resolution.md) and the
[shared rescue contact-manager route](rescue_contact_manager_route.md). The
existing C++ victim release and exit components remain reusable in the shared
rescue host; this factory trace does not require a Training-only port.
