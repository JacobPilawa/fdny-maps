# PlayerRescue candidate position used by FireRescue

## Current-hash callback route

The current-hash FireRescue callback export `004483A0_fresh.c` calls the
candidate entity's root vtable slot `+0x0C` and consumes the returned pointer
as a position array. Current-hash PlayerRescue vtable bytes are preserved in
[`memory_004e1c40_player_rescue_vtables.json`](../scenario-loading/memory_004e1c40_player_rescue_vtables.json)
for executable SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
`ConstructPlayerRescueEntity` (`00453D50`) installs root vtable address point
`004E1C48`. Counting from that point, slot `+0x0C` contains `0049DB90`.

The raw neighboring slot matters: `+0x10` contains `0049D7F0`, whose archived
decompilation returns a different buffer at PlayerRescue `+0x9C`. It is not
the target of the FireRescue candidate call. Do not conflate `0049D7F0` with
the body at `0049D790`.

## Getter body and port adapter

The preserved `0049DB90` thunk export routes to `0049D790`. The historical
body export transforms local origin through the object transform at `+0x24`,
stores the resulting XYZ at `+0x68..+0x70`, and returns that buffer. This
matches the kind of world-space point used by the viewer's Player movement
origin, so the shared spread adapter now passes `Player::p` through an
explicit `playerRescueCandidateWorldOrigin()` seam.

Classification:

- **Verified current hash:** `004483A0` requests candidate slot `+0x0C`; the
  PlayerRescue root vtable address point is `004E1C48`; its slot `+0x0C` is
  `0049DB90`; `00453D50` installs that vtable.
- **Corroborated by archived decompilation:** `0049DB90 -> 0049D790`; the
  body transforms the object origin and returns XYZ at `+0x68`.
- **Still open:** a current-hash body export for `0049DB90/0049D790`, the
  fourth float read by the exposure path, its initialization, and exact
  alignment between retail transform origin and viewer movement origin.

No Ghidra instance was available during this audit, so the getter body was not
refreshed from the active executable. The shared viewer now calls its
source-shaped `0x7531` fire-exposure handler from the ordinary frame loop.
This adapter and call site do not establish retail exposure callback cadence
or the required fourth-float value; see
[`player_hud_goggles_equipment_fire_viewer_runtime.md`](../scenario-loading/player_hud_goggles_equipment_fire_viewer_runtime.md).
