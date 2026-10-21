# VictimRescue roam and spawn-spot behavior

Status: **static command-to-movement path mapped for the pinned EXE; runtime timing and visual parity remain open.**

Evidence comes from fresh Ghidra analysis of `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, plus the hash-pinned BSP actor inventory. The provisional old-work [`0x00459840` export](../../../reference/previous-work/decomp/evidence/victim-startup/00459840_FUN_00459840.c) led to a recreated and verified function boundary for the alternate-spot handler. The pseudocode exports listed below are decompiler output, not recovered original source. No game runtime was launched for this pass.

## Command path

The VictimRescue-specific ScriptActor handler is `DispatchVictimRescueScriptCommand` (`0x0045CDB0`). It gives the shared ScriptActor handler `0x0047A1F0` first refusal. On fallback it handles these command names:

- `roam <float>` parses the argument as a float and calls `StartVictimRoam` (`0x0045C7F0`).
- `forcespawnspot <index>` parses an integer and calls `ForceVictimSpawnSpot` (`0x0045A4C0`).
- `playanim` calls `0x0045B3B0`, which logs `"PlayAnim" IS UNIMPLEMENTED`.
- `norescuebot` sets the byte at victim offset `+0x129`.

For `norescuebot`, the dispatcher writes the byte at victim `+0x129`, which is the embedded ScriptActor's `+0x129` (`VictimRescue +0xD4` is the subobject base). The RescueBot selector (`0x00455230`) reads that same byte and skips nonzero candidates. It scans VictimRescue actors in state 4 or 7, requires the actor to be at least 600 units from the player, and requires the world segment to be blocked; the first matching victim is sent to `StartVictimBotRescue` (`0x00454750`). The threshold is the binary64 value at `0x004E1F50` (`600.0`), not a 32-bit float. This proves `norescuebot` excludes an actor from that selector. No authored `runrescuebot` command was found in the BSP/EVS inventory. Training sets `setrescuebot 0`; the recurring Player-update callback is connected to that field, but other maps' default enable state and the host's callback/lifecycle behavior remain unresolved. See [the RescueBot Player update trace](rescuebot_update_path_2026-10-15.md).

The actor inventory preserves every non-empty `S_Script*` epair on all 17 BSPs. Its generated [map-wide table](rescue_actor_inventory.md) shows exact authored literals. `roam` occurs across the rescue catalog, including both Boat maps, both Subway maps, all four Oil Rig stages, and Training's victim records use `pickup:` commands. Boat B also has a numeric-only `500` value, and Gas R a numeric-only `100`; neither is classified as a command here. Bar R's `playanim 1` and `setskin 2` are retained as authored data: `playanim` logs unimplemented, and the inspected dispatch layers have no `setskin` branch (see below).

## Bar R `setskin` literal

The Bar R BSP contains three VictimRescue `S_Script*` values equal to `setskin 2`. The shared handler `DispatchSharedScriptActorCommand` (`0x0047A1F0`) has no `setskin` command branch in its fresh decompilation. Its token helper `CompareStringObjectCaseInsensitive` (`0x0040C360`) compares the complete token with `__stricmp`, so `setskin` does not match the `set` command. The VictimRescue-specific `DispatchVictimRescueScriptCommand` (`0x0045CDB0`) also has no `setskin` branch. If the value reaches `RouteParsedScriptActorCommand` (`0x00470150`), the existing unrecognized-command path logs `Unknown script command`; no runtime capture confirms whether or when these authored fields are dispatched. This is evidence of no handler in the inspected dispatch path, not evidence of a skin change.

## Roam startup and per-update movement

`StartVictimRoam` stores the parsed roam float at `+0x224`, writes numeric state `7` at `+0x120`, copies the current location at `+0x4C` to the target at `+0x1C8`, initializes the movement input at `+0x228` to `36.0`, and calls `BuildVictimRoamObstacleList` (`0x0045C4D0`). The validator's `+0x218` center is initialized from the original authored victim location in `InitializeVictimRescueEntity`; random alternate-spot selection later changes the current actor location, not that center. Startup tries four offsets around the selected spawn: `(5,0,5)`, `(5,0,-5)`, `(-5,0,5)`, and `(-5,0,-5)`. If none pass `ValidateVictimRoamCandidate` (`0x0045AB40`), it logs `is stuck when spawned!` and writes state `4`; otherwise it calls `SelectVictimRoamAnimation` (`0x00459A50`). These are observed numeric state values; semantic names for states 4 and 7 are not assumed.

The obstacle builder iterates world entities whose kind field is `6`. The FireRescue constructor (`0x0044A330`) installs primary vtable `0x004E1780` and writes `6` to root `+0x18`, identifying these records as `RCS_FireRescue`. Its virtual radius slot (`vtable+0x0C`, `0x0044A310`) returns `root+0x7C`; the builder reads returned `+0x0C`, so the runtime radius is at FireRescue `+0x88`. Values below 24 or above 1200 are logged and clamped to that range. It includes fires whose center is within `fire radius + victim roam radius`, then stores the center and `(fire radius + 48)^2`. A candidate inside any stored expanded sphere is rejected. The builder logs `Max obstacles exceeded ... decrease radius!` when the post-insertion count is greater than 9. The exact retail UserData binding from authored `H_Radius` to runtime `+0x88` remains unproven; the preview uses `H_Radius` as its static map-data proxy.

For fire entities with `L_FloorMe`, the native FireRescue initializer's floor helper (`0x00448630`) traces from 50 units above the authored position down 400 units and adjusts the visual transform to the floor. The preview applies this same probe and +3-unit offset when building its static fire-obstacle centers. Dynamic fire movement, scripted size changes, and full fire-runtime ordering are not part of this viewer slice yet.

The candidate validator rejects a near-zero offset, enforces the authored roam-radius circle around `+0x218`, sweeps the victim's 48-unit box along the candidate, checks the collected FireRescue obstacle spheres, then requires both destination probes rotated by +15 and -15 degrees to hit floor within six units of the current foot height. The floor trace starts 50 above the probe and ends 264 below it. This is a combination of an origin radius, a box sweep, nearby fire clearance, and two floor-height checks, not a simple point-in-circle test.

The state-7 handler `UpdateVictimRoamMovement` (`0x0045B220`) calls `UpdateVictimRoamTarget` (`0x0045AE50`), computes the vector from the current location to the stored target, turns toward it when squared distance exceeds 36, and advances the actor through `StepVectorTowardTarget` (`0x00483580`) using `+0x228`. The pinned PE stores `5184.0` at `0x004E2328`, the target-selection gate, so the actor selects a new target at or inside 72 units of its current target. This is separate from `0x004E2338=36.0`, the movement turn-distance-squared threshold (6 units). The host's `updateVictimRoamTarget` compares the current target to live `spawn.selectedPosition` and returns only when squared distance is greater than 5184; `advanceVictimRoamPosition` updates that position each host tick. The apparent target-center mismatch was a false alarm: `roamCenter` is the separate original authored center used by candidate validation. The host and retail target-update center and strict outer gate therefore match statically. This does not establish retail callback cadence or runtime parity.

The owner phase for this state-7 update is now mapped. VictimRescue's primary
virtual slot `+0x14` points to adapter `00459990`, which forwards to its
embedded actor updater at root `+0xD4`. The embedded `0045BEA0` state dispatcher
calls `0045B220` in state 7. Common root initialization sets eligibility mask
`0xFFEB`, so generic registration admits the victim to owner Think manager
`+0x50`. Since normal owner pass `00429520` runs that manager before contact
manager `+0xE0`, the shared viewer now advances roaming victims before sampling
contacts. Host frame delta, exact Think-list order, and live collision/pickup
parity remain provisional. See
[`victim_roam_think_phase_2026-10-21.md`](victim_roam_think_phase_2026-10-21.md).

This helper's sole retail caller is the state-7 handler. It computes `step = globalFrameDelta * movementScalar`; it snaps to the target only when `step > 0` and squared remaining distance is strictly less than `step²`. Otherwise it advances by a normalized target delta scaled by `step`. The viewer's former arbitrary `step > 0.2` snap gate was removed. The turn helper `00482AA0` applies `old * pow(0.5, frameDelta / 0.3) + adjustedDesired * (1 - pow(...))`, where the caller first adjusts the desired angle to its nearest equivalent around the old heading. The viewer now uses this exponential blend instead of a linear `dt * 8` interpolation. The 0.3-second half-life and strict arrival comparison are verified from the pinned executable; the viewer's rendered-frame delta is still a host input. The updater uses a random 6–18-unit direction for its initial/random mode; otherwise it follows its last step direction when the player is outside the 432-unit gate. Nearer the player, the 432 gate compares the player with the victim's current waypoint, then requires a 45-degree victim-facing check and a clear segment from that waypoint to the player. If those pass, the initial 12-unit horizontal candidate points toward the player. It is reversed away if the Player is carrying a victim, is closer than 48 units to the waypoint, or is facing more than 45 degrees away from the waypoint. The 48 and 192 thresholds are squared-distance comparisons against 2,304 and 36,864 respectively; the 192-unit branch consumes a random draw whose return is discarded. If the Player-relative gates fail, the updater continues the previous step direction. Failed normal candidates are retried at half length and +30 degrees, then -60 degrees from that alternative; if both fail, random mode is re-enabled. The retail callback period remains unresolved.

### Player-proximity, angle, and visibility gates

In `UpdateVictimRoamTarget`, the local pointer passed as the first argument to `FUN_00459BE0` addresses the stored roam target at victim offset `+0x1C8`. `FUN_00459BE0(target, player_position, 432.0)` compares squared vector distance with `432.0 * 432.0`; the player must be strictly within 432 world units of that target to enter the following player-relative selection path. The same block builds a vector between that target and the Player, passes it to `FUN_00441E70` (`0x00441E70`, sum of the three squared components), then compares its squared length against `0x004E2324` (`48² = 2,304`) and `0x004E2320` (`192² = 36,864`). A nonzero pointer in the Player's carried-victim field at `+0x3534` also reverses the candidate. The toward vector is negated through `FUN_0042D2B0` when any away condition applies. The second angle check uses the Player transform and the vector from Player position to the current waypoint; an angle above 45 degrees also reverses it. The 192-unit branch calls the random routine but does not use its return value in this selection path.

The angular helper is `ComputeVectorAngleDegrees` (`0x00497AA0`). Its x86 instructions form the dot product of two 3-component vectors, divide by the product of their lengths, call the x87 arccos routine, and multiply by the double constant `57.2957795` (radians-to-degrees). Its corrected Ghidra prototype models ECX as the first vector and the stack parameter as the second.

The reference vector's object layout is now tied to VictimRescue. Its root vtable at `0x004E25B8` uses `GetScriptActorSubobject` (`0x0049D760`) in slot `+0x18`; that accessor returns `victim + 0x24`. The previous work already exported the getter's `+0x24` pseudocode; fresh Ghidra work recreated the missing function boundary and tied it to the vtable and embedded ScriptActor. `InitializeVictimRescueEntity` installs the embedded ScriptActor vtable at `victim + 0x24`. The shared transform constructor initializes basis vectors at embedded offsets `+0x04` through `+0x24` and position at `+0x28` through `+0x30`; VictimRescue initialization separately writes its position at `victim + 0x4C` through `+0x54`, confirming the same position field. Thus `MeasurePointOffsetAlignmentAngle` (`0x00459C70`) compares `player_position - victim_position` against the transform's third basis vector at embedded `+0x1C` (victim `+0x40`). `RotateTransformBasisAboutYDegrees` (`0x00496C30`) rotates the first and third basis vectors about Y using a degrees-to-radians constant and re-orthonormalizes the transform. This makes the 45-degree check an orientation-cone test; whether the third basis vector corresponds to the victim model's visible forward direction still needs runtime confirmation.

In the player-relative target branch, the player must be strictly within 432 units of the victim's current roam target, fall inside the 45-degree cone around the victim transform's `+0x1C` basis, and have an unobstructed segment to that target. Otherwise the code selects a fallback candidate along the previous target-step direction. When the gates pass, the initial candidate is a nominal 12-unit vector toward the player. Carrying a victim, being under 48 units from the current waypoint, or the Player facing more than 45 degrees away from that waypoint negates the candidate. If the Player is not carrying a victim, is at least 48 but less than 192 units from the waypoint, and is facing toward it, a separate 192-unit test consumes a random draw whose return value is discarded; it does not alter the chosen direction in the recovered path. The validator later zeros the candidate's Y component and can reject it. Because the vector is normalized before scaling and then flattened, vertical separation can make the final horizontal step shorter than 12. These are candidate-selection rules rather than guarantees of actual movement. The visible forward-axis interpretation still needs a runtime check.

If the victim-facing angle gate passes, `CheckActiveWorldSegmentClearance` (`0x00490130`) forwards the segment endpoints and mode zero to `IsWorldSegmentClear` (`0x0048FEA0`) using the current world at `DAT_00503A78 + 4`. The lower helper calls `geWorld_Collision` with contents mask `0x43` and flags `2` for mode zero, then returns true when that query reports no collision. The shared host uses the same segment endpoints, mask, flags, and hit/clear polarity. This confirms query-parameter parity; identical BSP/geWorld hit results across all authored scenes remain unverified. The roam caller falls back if the segment is blocked. The collision wrapper's corrected prototype returns the boolean consumed by the caller. A separate Player-facing angle check can reverse the Player-relative candidate as described above.

## Spawn spots

`InitializeVictimRescueEntity` (`0x0045DDD0`) reads the authored entity data, initializes the victim, copies its initial location, then registers that position and authored heading via `RegisterVictimSpawnSpot` (`0x0045A900`). Registration appends an XYZ-plus-heading record to the list at `+0x200`, with count at `+0x204`. `ForceVictimSpawnSpot` accepts an in-range integer list index, copies the selected record's position into the actor, and applies its heading through `0x00496C30`. The observed conversion is `heading * 30.0 + 180.0` degrees before the transform helper applies the rotation.

The second registration path is now verified as `RegisterVictimAlternateSpawnSpot` (`0x00459840`). It reads the authored `A_Victim` name, searches the world list for a type-2 entity whose registered name matches, then registers the alternate spot's `ZZ_location` and `B_Heading` on that victim. The handler logs when the target name is empty and marks the alternate-spot object handled after the lookup. The map inventory joins all 45 authored `RCS_VictimAltSpot` references to a local `RCS_VictimRescue` name, with no unresolved links. Runtime factory admission remains unobserved.

The two queue passes matter to random state as well as spot availability. FireRescue factory initialization precedes VictimRescue initialization; the viewer advances the recovered fire smoke/sprite draws and named-loop choice before creating victims. Each victim then consumes its initial sound-controller deadline draw before model/motion selection, selects a random registered spot in the second pass, and may draw again when a `roam` script selects a walk motion. Earlier factory classes such as `RCS_MiscEmitter` are not yet included in the preview's shared stream, so its current seed is not retail seed-for-seed parity. See [victim spawn RNG coverage](victim_spawn_rng_port_coverage.md).

## Fresh pseudocode exports

- [Victim script dispatcher](0045cdb0_dispatch_victim_rescue_script_fresh.c)
- [Shared ScriptActor command dispatcher](0047a1f0_script_command_dispatch.c)
- [Case-insensitive token comparator](0040c360_case_insensitive_string_compare_fresh.c)
- [Roam startup](0045c7f0_start_victim_roam_fresh.c)
- [Obstacle builder](0045c4d0_build_victim_roam_obstacles_fresh.c)
- [FireRescue radius getter bytes](0044a310_firerescue_obstacle_radius_getter.md)
- [Candidate validator](0045ab40_validate_victim_roam_candidate_fresh.c)
- [Target updater](0045ae50_update_victim_roam_target_fresh.c)
- [State-7 movement updater](0045b220_update_victim_roam_movement_fresh.c)
- [State-7 normalized movement helper](00483580_victim_roam_step_fresh.c)
- [Exponential heading blend helper](00482aa0_victim_roam_heading_blend_fresh.c)
- [Spawn-spot selector](0045a4c0_force_victim_spawn_spot_fresh.c)
- [Spawn-spot registrar](0045a900_register_victim_spawn_spot_fresh.c)
- [Alternate-spot resolver](00459840_register_victim_alternate_spawn_spot_fresh.c)
- [Victim initializer](0045ddd0_initialize_victim_rescue_entity_fresh.c)
- [Roam animation selector](00459a50_select_victim_roam_animation_fresh.c)
- [Vector angle helper](00497aa0_compute_vector_angle_degrees_fresh.c)
- [Roam angle wrapper](00459c70_roam_offset_vector_angle_fresh.c)
- [World-segment query wrapper](00490130_world_segment_clear_wrapper_fresh.c)
- [Victim ScriptActor accessor](0049d760_get_script_actor_subobject_fresh.c)
- [Transform yaw update](00496c30_rotate_transform_basis_about_y_fresh.c)

## Remaining evidence gaps

- Prove the retail UserData key-to-field mapping from authored FireRescue `H_Radius` to runtime `+0x88`; account for FireRescue movement and scripted size changes in obstacle centers/radii.
- Identify remaining victim numeric state meanings and `+0x180`; capture the visible forward-axis interpretation for the 45-degree gate.
- `setskin` and `playanim` have no supported gameplay effect in the inspected retail dispatch path (`playanim` explicitly logs unimplemented; `setskin` has no branch). Runtime dispatch/logging of those authored values remains unobserved; do not invent a skin or animation port. Numeric-only script values and other victim command fields remain unresolved.
- Establish the victim callback caller/cadence and capture original-game spawn draws, movement, obstacle behavior, and rescue completion for parity.
- Keep integrating the shared behavior for every rescue mission and Training; the authored map values are inputs, not evidence of runtime completion.
