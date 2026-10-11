# Training ladder commands and player movement state

Read-only Ghidra 12.0.4 review of the preserved `FDNYFirefighter.exe` project, plus the extracted Training entity records. No live gameplay capture was used. Ghidra outputs are provisional decompilations; the field offsets, branch tests, call targets, and original entity records are the evidence retained here.

## Authored Training ladders

Training has two ladder BModels:

| Entity | Name | Model | Authored location | Bounds | Script events |
|---|---|---:|---|---|---|
| 108 | `ladder2` | 5 | `(-248, -372, -60)` | `(-252, -504, -76)` to `(-244, -264, -12)` | `touch: tell player climb`; `untouch: tell player unclimb` |
| 110 | `ladder3` | 6 | `(216, -376, -1044)` | `(204, -504, -1052)` to `(212, -264, -988)` | `touch: tell player climb`; `untouch: tell player unclimb` |

Both records set `E_Reach=5` and `F_HitPoints=0`. The native loop dispatches their scripts from the extracted AABB bounds, expanded by five units on each BModel axis. Ghidra confirms the original callback also expands BModel bounds by the UserData float at `+0x30` and uses inclusive comparisons. The original queue-to-callback schedule is still open, so only the volume predicate is source-mapped; the native event cadence is not claimed to match.

## Player command path

Ghidra string references place `climb` and `unclimb` in the Player script handler `00451870`. Its `climb` branch calls `0044C0B0` at `00451B92`; its `unclimb` branch changes state only when the Player is currently in state 5.

- `0044C0B0` enters state 5 only if Player state `+0x18C` is not already 5 and pointer-sized field `+0x3440` is zero. It then calls `0042D280` on Player `+0xFC` with `(0,0,0)`. That helper stores three zero floats. The movement updater stores its velocity vector at `+0xFC..+0x104`, so this clears velocity on climb entry.
- `unclimb` changes state 5 back to state 3; otherwise it leaves the state unchanged.
- The Player constructor `0044D550` initializes state `+0x18C` to 3 and `+0x3440` to zero. Ghidra's bounded direct-store scan found only three writers to `+0x3440`: constructor zero at `0044D550`, pointer assignment at `0044BC40`, and clear at `0044CD10`. The BModel slot-1 handler `0043D2B0` calls `0044BC40` with the BModel root when BModel `+0x258 == 1`; the Player update at `00452570` uses the pointer as the held object in state 7 and clears it on release. This maps the pointer to a carried BModel and establishes that the gate can be reached by Training's liftable-object route. No live gameplay was captured.
- Player movement updater `00452570` reads state 5 directly. Its state-5 path applies player-view orientation to the input vector, uses the normal movement speed path (400), selects the grounded/climb smoothing factor (0.2 raised to `27 × frame delta`), and suppresses the normal `-1000` vertical acceleration. These are source-level branches; their full native frame and collision integration has not been compared as a complete Training run.
- The `JUMP` branch reaches `0044E990`. Once Player timer `+0x388` plus 500 ms is no later than global time, state 5 becomes state 3 and the updater writes `400` to Player `+0x100`. Outside state 5, the helper requires the grounded byte before applying its ordinary jump update. The native climbing jump path mirrors the state exit, velocity assignment, and 500 ms cooldown, but full input/frame timing parity remains open.

The current native `climbing` flag models Training's source-backed state-3 ↔ state-5 path: `climb` clears velocity and enables the pitch-directed movement branch; `unclimb` disables it; a jump exits and sets vertical velocity. The native Player does not yet model held-object state 7, so ladder parity is unverified when a Training liftable is carried. Training's authored `setrescuebot 0` writes a separate boolean at `+0x32E4`; it does not affect the `+0x3440` held-object pointer.

## BModel contact and Training liftables

Read-only Ghidra inspection of `0043B620` (the BModel contact callback at vtable slot 10) shows it ignores BModels with no model and candidates whose byte at `+0x18` is not 1 or whose vtable callback at `+0x0C` returns null. It then passes the BModel bound vectors and UserData `+0x30` to `004817A0`. Before that call, it applies a conditional 48-unit override for hitpoint-enabled, non-liftable BModels whose maximum child-motion end time is at least `+1e-28`, provided authored reach is below 48. Training entities 113, 212, and 213 meet the data gate; entity 112 has authored reach 50 and keeps it. Full gate evidence and derivation are in `training-bmodel-contact-audit.md`.

`004817A0` accepts a contact exactly when, on each axis, `actorMax >= bmodelMin - reach` and `actorMin <= reach + bmodelMax`. Equality is included. The native helper in `src/bmodel_contact.h` follows those positive comparisons, including their rejection of NaN operands. `--bmodel-contact-check` exercises the AABB boundaries, NaN cases, strict 48-unit threshold and property/motion gates; `--bmodel-contact-asset-check` checks the Training-derived effective reaches.

The extracted raw Training BSP typedef in `evidence/bsp-typedefs/training-bmodel.json` maps `E_Reach` to UserData `+0x30`, `F_HitPoints` to `+0x34`, and `G_Liftable` to `+0x38`. The same BModel initialization path sets BModel `+0x258` to 2, checks UserData `+0x38`, then calls `0043BE00` when it is nonzero. `0043BE00` requires model path 0 with exactly two keyframes at times 0 and 1; on success it writes 1 to BModel `+0x258`. The meaning of initial value 2 remains unknown.

Training entities 109 and 111 set `G_Liftable=1`, with models 7 and 8. Their extracted motions each have two translation/rotation keys at times 0 and 1, matching the validator's shape. Entity 109's touch script displays “Press SPACE to clear this object from your path.” This statically establishes the authored data and code path for liftable use; input routing and in-game activation have not been observed. `0043D2B0` calls Player setter `0044BC40` only when `+0x258==1`. The setter stores the BModel root at Player `+0x3440`, sets object `+0x25C` to 1, and enters Player state 7.

Player state 7 is only partly mapped. `00452570` retains ordinary movement input and gravity; the held-object branch follows the Player transform using offsets 90 and -60 and calls `0043BD90`. Its branch through `00497AA0() < 10.0` releases the object through `0043B580`, clears Player `+0x3440`, restores state 3, and increments “Objects Thrown.” The threshold's units and exact trigger semantics remain unknown. `0043D330` updates objects in state `+0x25C==2`; trajectory completion resets that state and dispatches `drop`. Native carry/throw state, object motion, and corresponding input timing are not implemented.

## Remaining verification

- Resolve BModel contact-candidate ownership, overlap queue conversion, and original touch/touching/untouch cadence; compare the full event sequence to the native same-pass latch. The contact predicate and Training reach values are source/data mapped, but frame cadence remains open.
- Port and verify Training lift/carry/throw, including the `0043D2B0 → 0044BC40` path, state 7 movement, object trajectory, and exact input threshold/timing.
- Compare the complete state-5 movement, jump cooldown, collision, and callback order against the original runtime before claiming ladder parity.

Ghidra evidence: `FindStringReferences.java climb unclimb ladder`, `DecompileFunctions.java 0043B620 004817A0 00470840 004901E0 004901A0 0043DC60 0043BE00 0043D2B0 0044BC40 00452570 0044E990`, `AddressXrefs.java 0044BC40 0044C0B0`, and `FieldStoreAudit.java 0x3440`, all read-only. The saved project had no function boundary at `0043B620`; its vtable reference and callback body were inspected from the local instruction/decompiler evidence and the read-only scratch project where needed. The user-open Ghidra project was not modified. Raw local exports are under `evidence/gameplay/`, `evidence/mechanics-decompiled/`, and `evidence/player-tool-pose/state-offsets/`.

The BModel use-candidate query, status/action code, and Training liftable records are documented separately in `evidence/training-liftable-audit.md`; the conditional contact reach and per-entity results are in `evidence/training-bmodel-contact-audit.md`.
