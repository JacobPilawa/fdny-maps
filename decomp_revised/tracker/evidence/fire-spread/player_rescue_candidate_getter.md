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

## Fourth exposure value resolved

The same PlayerRescue getter returns a pointer at root `+0x68`, and
`004483A0` passes element `[3]` from that pointer to `ComputeFireVolumeFalloff`.
That is root `+0x74`. Fresh hash-pinned Ghidra output traces the normal
constructor chain `00453D50 -> 0043F610 -> 0049DF30 -> 0049D8C0`; `0049D8C0`
explicitly initializes root `+0x74` to zero. `0049D790` writes only XYZ at
`+0x68/+0x6C/+0x70`, and the mapped floor cache copy `00441E90` writes three
components. The shared port now passes `0.0f` padding for Player exposure
instead of the provisional 30-unit movement-hull radius.

This establishes the value for the normal construction path and reviewed
position-cache updates. It does not rule out every possible later aliased
write, and no retail runtime watchpoint was available. The independent callback
cadence question remains open. The fresh export is
[`player_fire_exposure_padding_fresh_2026-10-20.c`](player_fire_exposure_padding_fresh_2026-10-20.c).

Classification:

- **Verified current hash:** `004483A0` requests candidate slot `+0x0C`; the
  PlayerRescue root vtable address point is `004E1C48`; its slot `+0x0C` is
  `0049DB90`; `00453D50` installs that vtable.
- **Verified by current-hash disassembly:** `0049DB90 -> 0049D790`; the body
  transforms local origin through root transform `+0x24` and returns XYZ from
  the cache at `+0x68..+0x70`.
- **Corroborated by field writers:** Player translation `+0x4C..+0x54` is
  canonical; `0043F810` refreshes the derived origin cache after writing it.
- **Verified by fresh hash-pinned constructor trace:** the fourth value at
  PlayerRescue `+0x74` is initialized to `0.0f`; getter and floor cache helper
  write only XYZ. Later aliased writes are not exhaustively ruled out.
- **Still open:** callback cadence and its relative order with other updates.

The shared viewer now calls its source-shaped `0x7531` fire-exposure handler
from the ordinary frame loop and supplies the constructor-backed zero padding.
This adapter does not establish retail exposure callback cadence; see
[`player_hud_goggles_equipment_fire_viewer_runtime.md`](../scenario-loading/player_hud_goggles_equipment_fire_viewer_runtime.md).

## Shared getter record used by victim pickup

VictimRescue also calls candidate slot `+0x0C` from `0045A070`. For the
PlayerRescue root, the returned pointer starts at `+0x68`; pickup interprets
the surrounding record as origin XYZ at indices `[0..2]`, the fourth-float
padding at `[3]`, mins at `[4..6]`, and maxs at `[7..9]`. Those last six values
are Player root `+0x78..+0x8c`, initialized by `0044D550` to
`(-30,-30,-30)` / `(30,85,30)`. The Player `CROUCH` command changes max Y at
`+0x88` to 25, and a successful uncrouch restores it to 85. This extends the
getter's point interpretation for FireRescue without changing the meaning of
the FireRescue falloff's first three coordinates. The shared port's
`VictimPickupBox` uses the same position and crouch-aware extents; only natural
callback cadence and runtime parity remain open.

## Plain-language explanation

Retail asks PlayerRescue for a world point and a fourth falloff value. The
point is the same translation the viewer stores in `Player::p`; its adjacent
fourth value is initialized to zero by the constructor. The port now uses both
source-backed values. Callback timing remains open.
