# VictimRescue and alternate spawn-spot factory path

## Result

The hash-pinned executable statically confirms that all authored VictimRescue and VictimAltSpot records enter one shared world-spawn pipeline. VictimRescue factories are registered before VictimAltSpot factories. The queue constructs both classes, runs the VictimRescue runtime-actor pass, then resolves alternate spots, and only after the full first pass runs does it select victim spawn points. This ordering ensures the authored alternate spots are available before the victims choose their spawn positions.

This is a static code and data trace for FDNYFirefighter.exe, SHA-256:
4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

## Map coverage

The hash-checked actor inventory joins all 45 alternate-spot references to named victims and records 71 VictimRescue entities across the 13 BSPs that author these classes.

| BSP key | Victims | Alternate spots |
|---|---:|---:|
| bank | 6 | 4 |
| bar_r | 6 | 4 |
| boat_a | 9 | 7 |
| boat_b | 5 | 5 |
| gas_r | 7 | 0 |
| mbase | 5 | 0 |
| oil_rig_a | 3 | 4 |
| oil_rig_b | 3 | 1 |
| oil_rig_c | 6 | 6 |
| oil_rig_d | 3 | 4 |
| subway_a | 6 | 2 |
| subway_b | 10 | 4 |
| training | 2 | 4 |
| Total | 71 | 45 |

The inventory includes all seven catalog rescues, their linked stages, and Training. The broader map inventory still covers all 17 BSPs, including the four hose-only maps.

## Factory and queue path

1. Startup 0042B4E0 calls RegisterEntityClassFactories at 004297E0.
2. That function appends the RCS_VictimRescue descriptor (constructor 0045A010) immediately before the RCS_VictimAltSpot descriptor (constructor 004597C0). Their descriptor vtables store class keys and create slots:
   - RCS_VictimRescue descriptor vtable address point 004E22AC; slot +0x08 is CreateVictimRescueEntity at 0045DF80.
   - RCS_VictimAltSpot descriptor vtable address point 004E2240; slot +0x08 is CreateVictimAltSpotEntity at 00459760.
3. WorldEntity at 0049E4D0 enumerates the registered class descriptors and queues matching BSP entity records in that iteration order. ProcessEntitySpawnQueue at 0049C670 creates each queued object by calling descriptor slot +0x08 and registers the results. It then makes two complete passes over the collected entities: root vtable slot +0x20 for every object, followed by root slot +0x24 for every object.
4. CreateVictimRescueEntity allocates 0x22c bytes and calls InitializeVictimRescueEntity at 0045DDD0. That initializer reads the parsed name and transform, registers the authored XYZ plus heading as the victim's initial spawn spot, and registers the victim entity with the world.
5. CreateVictimAltSpotEntity allocates 0x28 bytes and calls ConstructVictimAltSpotEntity at 004596F0. Its root vtable address point is 004E21E8; slot +0x20 points to RegisterVictimAlternateSpawnSpot at 00459840.
6. A VictimRescue root vtable at 004E25B8 uses slot +0x20 for InitializeVictimRescueRuntimeActor at 0045B690 and slot +0x24 for SpawnVictimRescueAtRegisteredSpot at 0045D5B0. The first pass constructs each runtime victim actor. Because the VictimRescue descriptor precedes VictimAltSpot in the registry, those first-pass victim calls are followed by all alternate-spot calls.
7. RegisterVictimAlternateSpawnSpot reads the authored A_Victim name, finds a type-2 VictimRescue with a matching registered name, appends the alternate XYZ and heading to that victim's spot list, and marks the alternate record handled.
8. Only after slot +0x20 has run on every queued object does the queue begin the +0x24 pass. SpawnVictimRescueAtRegisteredSpot calls ScalePseudoRandomFloatToRange at 00498B30 with lower bound 0 and upper bound spot_count minus 0.01. That helper scales GetPseudoRandomUnitFloat at 00404AC0, whose output is in [0,1); ConvertX87FloatToIntegerTowardZero at 004B9400 converts the result to an index, and the caller checks it against the spot count. Every index from 0 through spot_count minus 1 can be selected. The method applies that spot's XYZ and heading, then dispatches the victim's authored S_Script01-S_Script05 fields.

## Evidence

Fresh Ghidra pseudocode and vtable bytes in this directory:

- 004297e0_entity_class_factory_registration_fresh.c
- 0045a010_victim_rescue_factory_descriptor_fresh.c
- 004597c0_victim_alt_spot_factory_descriptor_fresh.c
- 0045df80_create_victim_rescue_entity_fresh.c
- 00459760_create_victim_alt_spot_entity_fresh.c
- 004596f0_construct_victim_alt_spot_entity_fresh.c
- 0045ddd0_initialize_victim_rescue_entity_fresh.c
- 0045b690_initialize_victim_rescue_runtime_actor_fresh.c
- 00459840_register_victim_alternate_spawn_spot_fresh.c
- 0045a900_register_victim_spawn_spot_fresh.c
- 0045d5b0_spawn_victim_rescue_at_registered_spot_fresh.c
- 00404ac0_get_pseudo_random_unit_float_fresh.c
- 00498b30_scale_pseudo_random_float_to_range_fresh.c
- 004b9400_convert_x87_float_to_integer_toward_zero_fresh.c and its assembly listing
- memory_004dfc78_spawn_random_upper_margin.json (the 0.01 margin)
- 00404ac0_get_pseudo_random_unit_float_fresh.c
- 00498b30_scale_pseudo_random_float_to_range_fresh.c
- 004b9400_convert_x87_float_to_integer_toward_zero_fresh.c and its assembly listing
- memory_004dfc78_spawn_random_upper_margin.json (the 0.01 margin)
- 0049e4d0_world_entity_load.c
- 0049c670_entity_spawn_queue_fresh.c
- memory_004e22ac_victim_rescue_factory_vtable.json
- memory_004e2240_victim_alt_spot_factory_vtable.json
- memory_004e25b8_victim_rescue_root_vtable.json
- memory_004e21e8_victim_alt_spot_root_vtable.json

The map/entity joins are in [rescue_actor_inventory.csv](../../rescue_actor_inventory.csv). The shared queue behavior is also summarized in [world_factory_resolution.md](world_factory_resolution.md). Names were verified against the current EXE in Ghidra 12.0.4.

## Limits and next port work

This closes the static successful path from BSP class keys to named victim actors and spawn-point selection. Runtime allocation failures, PRNG seeding/refill behavior, callback cadence, and visual parity remain unobserved. The native victim-release and pickup components remain useful, but the port still needs to materialize this map-neutral victim/alternate-spot contract from parsed BSP data for every rescue stage and Training.
