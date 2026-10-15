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

Current-hash disassembly shows `0049DB90` jumping to `0049D790`. The body
transforms local origin through the object transform at `+0x24`, stores the
resulting XYZ at `+0x68..+0x70`, and returns that buffer. For the PlayerRescue
root, local origin is its world translation at `+0x4C..+0x54`. The Player
floor-hit writer at `0043F810` updates that canonical translation and mirrors
the derived origin cache. The viewer's `Player::p` is its canonical movement
origin, so `playerRescueCandidateWorldOrigin()` returns it directly. Retail
and host collision bounds also agree (`(-30,-30,-30)` to `(30,85,30)`, with
the source crouch height at 25); no position or hull adjustment is indicated.

Classification:

- **Verified current hash:** `004483A0` requests candidate slot `+0x0C`; the
  PlayerRescue root vtable address point is `004E1C48`; its slot `+0x0C` is
  `0049DB90`; `00453D50` installs that vtable.
- **Verified by current-hash disassembly:** `0049DB90 -> 0049D790`; the body
  transforms local origin through root transform `+0x24` and returns XYZ from
  the cache at `+0x68..+0x70`.
- **Corroborated by field writers:** Player translation `+0x4C..+0x54` is
  canonical; `0043F810` refreshes the derived origin cache after writing it.
- **Still open:** the fourth value read by the exposure path and its
  initialization, callback cadence, and relative order with other updates.
  These do not change the mapped candidate position.

The shared viewer now calls its source-shaped `0x7531` fire-exposure handler
from the ordinary frame loop. This adapter and call site do not establish
retail exposure callback cadence or the required fourth-float value; see
[`player_hud_goggles_equipment_fire_viewer_runtime.md`](../scenario-loading/player_hud_goggles_equipment_fire_viewer_runtime.md).

## Plain-language explanation

Retail asks the PlayerRescue actor for the world position of the middle of its
root frame. That point is the same translation the viewer already stores in
`Player::p`, so the port can pass `Player::p` directly. The position alignment
is now source-backed; the exact callback timing and one separate exposure
value are still unknown.
