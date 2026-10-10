# Player collision correction — 2026-09-06

The previous audit incorrectly treated the player's collision callback as an
empty base-class method, and consequently described all walls as hard stops
with no stepping. That conclusion was wrong. It caused the open-door bug:
four-unit world thresholds remained impassable after the door brush was disabled.
The earlier door test started above the threshold and failed to catch this.

## Verified dispatch and recovered response

Fresh read-only Ghidra exports are in `analysis/collision-audit` and the log is
`analysis/logs/ghidra-collision-audit.log`. The player constructor at `0x00453D50`
sets the primary vtable to `0x004E1C48`; `0x004E1C40` is the secondary Think
interface at entity offset `0x140`. Confusing those bases changed the resolved
collision callback.

- Primary slot `+0x38` is `0x0043F810`: step attempt, then sliding response.
- Primary slot `+0x64` is `0x0043F680`: integrate, reset the per-frame step flag.
- Constructor `0x0043F610` sets step height to **16 original units**.
- Stepping is eligible for normals with Y ≤ **0.9** (double at `0x004E0EB8`).
- Test full-hull clearance 16 units up, then the horizontal displacement, then
  17 units down for landing. A successful step updates both body/collision
  origins and sets the per-frame step flag. The caller retries collision using
  the displacement, as the executable does.
- Sliding `0x0049E0A0` marks ground at normal Y ≥ 0.5, projects inward velocity
  and acceleration away from the plane, then recomputes displacement.
- `0x0043F6D0` retries up to five contacts. The hard-stop function `0x0049E040`
  is its final fallback, not the ordinary response.

`app/src/movement.ts` now implements this sequence. The runtime no longer uses
the old ad hoc floor-tangent correction and separate wall sweep.
`applyCollisionResponse` now only implements the recovered fallback. The spawn
settling and sustained walking fixtures call the live shared movement function.
The floor classification was also corrected to use zero for the inside/outside
decision, as in Genesis3D `IntersectLeafSides_r`; the 0.1-unit epsilon belongs
only in the crossing fraction. Previously a short downward move could cross
the mathematical floor without reporting a contact.

## Regression coverage

- All **20 Training doorways**, both directions, from floor level, walking
  without jumping. They must clear by more than a player hull radius.
- Four-unit thresholds climb; twenty-unit barriers remain blocking.
- Standing head clearance is required for a step; the crouched hull fits.
- Wall contact preserves tangential movement.
- Jumping into a ceiling cancels upward movement, retains horizontal movement,
  and returns to the floor.
- PE tests verify the actual player vtable slots, constructor's 16-unit store
  and 0.9 normal threshold, guarding against the earlier dispatch mistake.

These are executable-informed reconstruction tests, not retail runtime traces.
Moving-door quaternion interpolation, scheduling, actor AI and the complete
interaction state machine still
need further recovery and comparison. No claim of full 1:1 parity is made.

## Victim contact correction

The previously undefined victim callback `0x0045A070` was disassembled and
decompiled in `analysis/interaction-audit`. It excludes victim states 5/6,
checks entity type 1 (player), overlaps the player's body hull against the
victim's ±authored-radius AABB, and sends message `0x7532`.
`0x00450440` accepts it only if no victim is already carried; `0x0045BEF0`
switches the victim to state 5, hides its actor and dispatches `pickup`.

Training's `J_Radius` is 60 original units, also the callback's fallback for a
nonpositive radius. The recreation now collects eligible survivors on body
contact, without pressing interact. It no longer uses the guessed 115-unit
camera sphere or nearest-camera sorting. Tests cover inclusive edges, corners,
standing/crouched hull height, and authored/fallback radii. Full victim AI,
event-loop ordering, carrying presentation and rescue completion remain partial.

Verification now includes 66 TypeScript tests, nine Python extraction checks and
the production build. Logs: `analysis/logs/native-code-verification.log`. Browser/UI checks
could not run because the computer-use service failed to start; no retail
runtime comparison was performed.
