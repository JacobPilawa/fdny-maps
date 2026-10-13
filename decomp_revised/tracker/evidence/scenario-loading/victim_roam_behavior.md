# VictimRescue roam and spawn-spot behavior

Status: **static command-to-movement path mapped for the pinned EXE; runtime timing and visual parity remain open.**

Evidence comes from fresh Ghidra analysis of `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, plus the hash-pinned BSP actor inventory. The provisional old-work [`0x00459840` export](../../../reference/previous-work/decomp/evidence/victim-startup/00459840_FUN_00459840.c) led to a recreated and verified function boundary for the alternate-spot handler. The pseudocode exports listed below are decompiler output, not recovered original source. No game runtime was launched for this pass.

## Command path

The VictimRescue-specific ScriptActor handler is `DispatchVictimRescueScriptCommand` (`0x0045CDB0`). It gives the shared ScriptActor handler `0x0047A1F0` first refusal. On fallback it handles these command names:

- `roam <float>` parses the argument as a float and calls `StartVictimRoam` (`0x0045C7F0`).
- `forcespawnspot <index>` parses an integer and calls `ForceVictimSpawnSpot` (`0x0045A4C0`).
- `playanim` calls `0x0045B3B0`, which logs `"PlayAnim" IS UNIMPLEMENTED`.
- `norescuebot` sets the byte at victim offset `+0x129`.

For `norescuebot`, the inspected dispatcher proves only the write. Direct x86 read-pattern searches for byte loads, compares, tests, and zero-extension at displacement `0x129` returned no candidate reader; scanning the raw `29 01 00 00` byte sequence produced the writer plus unrelated branch/immediate bytes. This narrows the static search but does not prove the flag is never read through another access pattern or alias. Its effect remains unresolved.

The actor inventory preserves every non-empty `S_Script*` epair on all 17 BSPs. Its generated [map-wide table](rescue_actor_inventory.md) shows exact authored literals. `roam` occurs across the rescue catalog, including both Boat maps, both Subway maps, all four Oil Rig stages, and Training's victim records use `pickup:` commands. Boat B also has a numeric-only `500` value, and Gas R a numeric-only `100`; neither is classified as a command here. Bar R's `playanim 1` and `setskin 2` are retained as authored data: `playanim` logs unimplemented, and the inspected dispatch layers have no `setskin` branch (see below).

## Bar R `setskin` literal

The Bar R BSP contains three VictimRescue `S_Script*` values equal to `setskin 2`. The shared handler `DispatchSharedScriptActorCommand` (`0x0047A1F0`) has no `setskin` command branch in its fresh decompilation. Its token helper `CompareStringObjectCaseInsensitive` (`0x0040C360`) compares the complete token with `__stricmp`, so `setskin` does not match the `set` command. The VictimRescue-specific `DispatchVictimRescueScriptCommand` (`0x0045CDB0`) also has no `setskin` branch. If the value reaches `RouteParsedScriptActorCommand` (`0x00470150`), the existing unrecognized-command path logs `Unknown script command`; no runtime capture confirms whether or when these authored fields are dispatched. This is evidence of no handler in the inspected dispatch path, not evidence of a skin change.

## Roam startup and per-update movement

`StartVictimRoam` stores the parsed roam float at `+0x224`, writes numeric state `7` at `+0x120`, copies the current location at `+0x4C` to the target at `+0x1C8`, initializes the movement input at `+0x228` to `36.0`, and calls `BuildVictimRoamObstacleList` (`0x0045C4D0`). It then tries four initial offsets around the starting location: `(5,0,5)`, `(5,0,-5)`, `(-5,0,5)`, and `(-5,0,-5)`. If none pass `ValidateVictimRoamCandidate` (`0x0045AB40`), it logs `is stuck when spawned!` and writes state `4`; otherwise it calls `SelectVictimRoamAnimation` (`0x00459A50`). These are observed numeric state values; semantic names for states 4 and 7 are not assumed.

The obstacle builder iterates world entities whose observed kind field is `6`. It compares each entity's position with the victim's current location and includes candidates within `obstacle radius + victim[+0x224]`. Stored records contain the obstacle center and a squared, expanded radius using a `48.0` offset. It logs the source message `Max obstacles exceeded ... decrease radius!` when the post-insertion count is greater than 9. The runtime meaning of entity kind 6 and the radius validation globals are still open.

The candidate validator checks a proposed position against a distance bound, the obstacle array, and world/collision helpers. It also tests rotated nearby vectors and vertical/path conditions. `+0x218` participates as the center of the roam-distance comparison, but its field meaning is not yet resolved. The report deliberately does not reduce this routine to a simple point-in-circle test.

The state-7 handler `UpdateVictimRoamMovement` (`0x0045B220`) calls `UpdateVictimRoamTarget` (`0x0045AE50`), computes the vector from the current location to the stored target, turns toward it when the squared vector length exceeds the observed constant `36.0`, and advances the actor through a movement helper using `+0x228`. The target updater only selects another target after the current one is within the observed squared-distance threshold `5184.0` (`72.0` units). It consults the player's position, applies a `432.0`-unit proximity gate, then uses an angular gate and world-segment collision query before generating player-relative candidates. Candidate offsets still pass through `ValidateVictimRoamCandidate`.

### Player-proximity, angle, and visibility gates

In `UpdateVictimRoamTarget`, the local pointer passed as the first argument to `FUN_00459BE0` addresses the stored roam target at victim offset `+0x1C8`. `FUN_00459BE0(target, player_position, 432.0)` compares squared vector distance with `432.0 * 432.0`; the player must be within 432 world units of that target to enter the following player-relative selection path.

The angular helper is `ComputeVectorAngleDegrees` (`0x00497AA0`). Its x86 instructions form the dot product of two 3-component vectors, divide by the product of their lengths, call the x87 arccos routine, and multiply by the double constant `57.2957795` (radians-to-degrees). Its corrected Ghidra prototype models ECX as the first vector and the stack parameter as the second.

The reference vector's object layout is now tied to VictimRescue. Its root vtable at `0x004E25B8` uses `GetScriptActorSubobject` (`0x0049D760`) in slot `+0x18`; that accessor returns `victim + 0x24`. The previous work already exported the getter's `+0x24` pseudocode; fresh Ghidra work recreated the missing function boundary and tied it to the vtable and embedded ScriptActor. `InitializeVictimRescueEntity` installs the embedded ScriptActor vtable at `victim + 0x24`. The shared transform constructor initializes basis vectors at embedded offsets `+0x04` through `+0x24` and position at `+0x28` through `+0x30`; VictimRescue initialization separately writes its position at `victim + 0x4C` through `+0x54`, confirming the same position field. Thus `MeasurePointOffsetAlignmentAngle` (`0x00459C70`) compares `player_position - victim_position` against the transform's third basis vector at embedded `+0x1C` (victim `+0x40`). `RotateTransformBasisAboutYDegrees` (`0x00496C30`) rotates the first and third basis vectors about Y using a degrees-to-radians constant and re-orthonormalizes the transform. This makes the 45-degree check an orientation-cone test; whether the third basis vector corresponds to the victim model's visible forward direction still needs runtime confirmation.

In the player-relative target branch, the player must therefore be within 432 units of the victim's current roam target, fall inside the 45-degree cone around the victim transform's `+0x1C` basis, and have an unobstructed segment to that target. Otherwise the code selects a fallback candidate. A later angle check in the same block can also choose the fallback.

If the angle gate passes, `CheckActiveWorldSegmentClearance` (`0x00490130`) forwards the segment endpoints and mode to `IsWorldSegmentClear` (`0x0048FEA0`) using the current world at `DAT_00503A78 + 4`. The lower helper calls `geWorld_Collision` and returns true when that query reports no collision. The roam caller also falls back if this segment is blocked. The collision wrapper's corrected prototype returns the boolean consumed by the caller. A later angle check in the same target-selection block can also select the fallback candidate.

## Spawn spots

`InitializeVictimRescueEntity` (`0x0045DDD0`) reads the authored entity data, initializes the victim, copies its initial location, then registers that position and authored heading via `RegisterVictimSpawnSpot` (`0x0045A900`). Registration appends an XYZ-plus-heading record to the list at `+0x200`, with count at `+0x204`. `ForceVictimSpawnSpot` accepts an in-range integer list index, copies the selected record's position into the actor, and applies its heading through `0x00496C30`. The observed conversion is `heading * 30.0 + 180.0` degrees before the transform helper applies the rotation.

The second registration path is now verified as `RegisterVictimAlternateSpawnSpot` (`0x00459840`). It reads the authored `A_Victim` name, searches the world list for a type-2 entity whose registered name matches, then registers the alternate spot's `ZZ_location` and `B_Heading` on that victim. The handler logs when the target name is empty and marks the alternate-spot object handled after the lookup. The map inventory joins all 45 authored `RCS_VictimAltSpot` references to a local `RCS_VictimRescue` name, with no unresolved links. Runtime factory admission remains unobserved.

## Fresh pseudocode exports

- [Victim script dispatcher](0045cdb0_dispatch_victim_rescue_script_fresh.c)
- [Shared ScriptActor command dispatcher](0047a1f0_script_command_dispatch.c)
- [Case-insensitive token comparator](0040c360_case_insensitive_string_compare_fresh.c)
- [Roam startup](0045c7f0_start_victim_roam_fresh.c)
- [Obstacle builder](0045c4d0_build_victim_roam_obstacles_fresh.c)
- [Candidate validator](0045ab40_validate_victim_roam_candidate_fresh.c)
- [Target updater](0045ae50_update_victim_roam_target_fresh.c)
- [State-7 movement updater](0045b220_update_victim_roam_movement_fresh.c)
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

- Confirm runtime factory admission and ordering for alternate spots; the static name lookup and 45 authored same-map links are resolved.
- Identify the meanings of victim numeric states, `+0x180`, and `+0x218` from their writers/readers; establish the exact gameplay interpretation of the two vectors used by the 45-degree gate.
- Capture whether Bar R's unhandled `setskin 2` fields are dispatched and logged; interpret numeric-only script values and other victim command fields.
- Establish the updater caller and cadence, then capture original-game victim movement, obstacle behavior, spawn selection, and rescue completion for parity.
- Implement this shared behavior for every rescue mission and Training; the authored map values are inputs, not evidence of runtime completion.
