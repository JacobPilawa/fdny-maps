# Training named-zone behavior: Ghidra trace

Read-only Ghidra 12.0.4 investigation of `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`) on 2026-10-11. No live gameplay capture or native gameplay changes were used.

## `makezone` command path

Training entities 114, 135, 158, 190, and 234 are `RCS_BoundsTrigger` records whose `S_Script01` assigns the five exercise labels listed below. The constructor at `00441040` registers the trigger's script fields through `00475310`, which enters the generic script runner at `00470150`. When the generic command handler at `0046F9D0` declines a command, `00470150` dispatches the trigger's script-subobject vtable slot `+4`.

The preserved database did not contain a function boundary for that handler. In a scratch copy only, `00441A60` was defined after verifying the trigger subobject vtable at `004E1088` points from slot `+4` (`004E108C`) to that entry. Ghidra decompilation shows it first calls common handler `0047A1F0`, then compares an unhandled command against `makezone`. For each parsed argument it calls `004418E0` with the parent BoundsTrigger (`script subobject - 0x24`) and the argument. `004418E0` logs the zone label and calls `00484740` on the map-zone manager's zone list at `*(DAT_00503A78 + 0x30) + 0x5D8`.

## Zone construction and lookup

- `00484740` searches for a zone with the supplied label through `00483B60`. If none exists, it creates the named record through `004844E0`, initializes accumulated volume to zero, then appends the BoundsTrigger pointer. It adds that trigger's `004818A0` bounds volume to the zone's accumulated volume.
- `00483C80` considers a point inside a zone if it is inside any member trigger's bounds; it calls `00481840`, whose six comparisons include the bounds edges.
- `00483E10` searches all zones and returns a containing zone with the smallest accumulated member volume. Its `<=` comparison allows a later enumerated equal-volume zone to replace an earlier match. The volume criterion is the sum of member-box volumes, not a computed union volume.
- `00484260` clears the actors stored on each zone, then enumerates actors with `+0x18 == 2` and state `+0x120` not equal to 5 or 6. It gets each actor position through its vtable slot `+0x18`, looks up the containing zone with `00483E10`, and appends the actor to that zone's actor list. Actors without a containing zone trigger a diagnostic whose text names a victim.
- Ghidra xrefs show `00484260` called from `0044FBA0`, `0044CD10`, `00450440`, and `004504C0`. The latter is in the rescue/exit path and also looks up a final-exit trigger's zone. These callsites establish specific recomputation points, not a per-frame zone update schedule.

| Training entity | Model | `makezone` label |
|---:|---:|---|
| 114 | 47 | Chopping and Throwing Exercise |
| 135 | 51 | Jumping and Navigation Exercise |
| 158 | 58 | Fire Extinguishing Exercise |
| 190 | 73 | Survivor Extraction Exercise |
| 234 | 82 | Crouching and Jumping Exercise |

The trigger-bound construction and exact five-box data cross-check are separately recorded in [`training-zone-bounds-ghidra-audit.md`](training-zone-bounds-ghidra-audit.md). Ghidra now verifies the generic `C_Model`-to-runtime-model-pointer handoff consumed by `00441040`; the remaining instance-level caveat is tying each extracted Training entity ID to its particular runtime `geEntity`/actor.

## Native gap and verification limits

`src/main.cpp` currently accepts `makezone` as a host no-op. The native runtime has no named-zone records, actor membership lists, smallest-volume lookup, or original exit-zone check. These are needed to reproduce mission grouping and survivor extraction; the native no-op does not implement those behaviors. Do not infer zone scheduling from the listed callsites.

Ghidra provenance: preserved-project `DecompileFunctions.java` queries for `00441040`, `00475310`, `00470150`, `00484740`, `00483B60`, `00483C80`, `00483E10`, `00481840`, `004818A0`, `00484260`, and `004504C0`; `FindStringReferences.java makezone`; and `AddressXrefs.java 00484260 004504C0`. The missing handler boundary was recovered in `/private/tmp/scriptactor-462660-audit` with `DefineAndDecompile.java 00441A60` after the vtable pointer was checked; `DataValue.java`/`AddressXrefs.java` were used for `004E1088` and `004E108C`. The preserved Ghidra project remained read-only. No runtime oracle was used.
