# Victim roam target gate reconciliation

Status: **the apparent current-position/radius mismatch was not real; the
shared host and pinned retail target gate match statically.** Retail callback
cadence, collision-query parity, and runtime parity remain open.

The retail image is `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Reading
the PE image at its mapped virtual addresses confirms `.rdata` value
`0x004E2328 = 5184.0` (`0x45A20000`) and
`0x004E2338 = 36.0` (`0x42100000`). The target updater `0x0045AE50` compares the
squared distance between its current target at victim `+0x1C8` and its current
actor position at `+0x4C` with `0x004E2328`; it returns only when that value is
strictly less than the computed squared distance. Thus a new target may be
selected at or inside 72 units. The 36.0 value belongs to the separate
movement-turn gate in `0x0045B220` (squared distance greater than 36).

The apparent center mismatch came from treating host
`victim.spawn.selectedPosition` as immutable spawn data. It is the live actor
position: the movement loop passes it to `advanceVictimRoamPosition` and
assigns the returned position back on each update. The original authored roam
center is held separately in `victim.roamCenter` for candidate-bound checks.
The host gate in `updateVictimRoamTarget` uses the live position and the same
strict `> 5184` return condition. The host's `previousRoamTarget` is also
updated alongside each accepted target, matching retail's `+0x75` previous
target and the fallback direction vector.

The Player-relative actor-position angle operand also resolves statically:
VictimRescue slot `+0x18` returns its embedded ScriptActor at root `+0x24`, so
`0x00459C70` reads the transform origin at embedded `+0x28` (root `+0x4C`).
VictimRescue initialization keeps that origin aligned with its current actor
position, which the host also uses for its angle check. The Player-facing gate
uses the current target relative to Player position in both paths.

No gameplay code changed. The collision request parameters also match
statically: `0x00490130` forwards endpoints and mode zero to `0x0048FEA0`; the
pinned `0x0048FEA0` export passes the current Genesis world, start/end vectors,
contents mask `0x43`, and flags `2` to `geWorld_Collision`. The host calls
`tracePoint(target, playerPosition, 0x43, 2)` and treats a hit as blocked, the
same boolean sense as the retail wrapper. This verifies the request inputs,
not that the host's BSP collision traversal produces retail-identical results
for every scene.

## Evidence

- [`0045ae50_update_victim_roam_target_fresh.c`](0045ae50_update_victim_roam_target_fresh.c)
- [`0045b220_update_victim_roam_movement_fresh.c`](0045b220_update_victim_roam_movement_fresh.c)
- [`00459c70_roam_offset_vector_angle_fresh.c`](00459c70_roam_offset_vector_angle_fresh.c)
- [`00490130_world_segment_clear_wrapper_fresh.c`](00490130_world_segment_clear_wrapper_fresh.c)
- [`victim_roam_behavior.md`](victim_roam_behavior.md)

This is static-source and pinned-image evidence only. No game runtime was
launched and no gameplay parity check was made.
