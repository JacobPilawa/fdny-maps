# Training named-zone behavior: Ghidra trace

Read-only Ghidra 12.0.4 investigation of `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`) on 2026-10-11. No live gameplay capture or native gameplay changes were used.

## `makezone` command path

Training entities 114, 135, 158, 190, and 234 are `RCS_BoundsTrigger` records whose `S_Script01` assigns the five exercise labels listed below. The constructor at `00441040` registers the trigger's script fields through `00475310`, which enters the generic script runner at `00470150`. When the generic command handler at `0046F9D0` declines a command, `00470150` dispatches the trigger's script-subobject vtable slot `+4`.

The preserved database did not contain a function boundary for that handler. In a scratch copy only, `00441A60` was defined after verifying the trigger subobject vtable at `004E1088` points from slot `+4` (`004E108C`) to that entry. Ghidra decompilation shows it first calls common handler `0047A1F0`, then compares an unhandled command against `makezone`. For each parsed argument it calls `004418E0` with the parent BoundsTrigger (`script subobject - 0x24`) and the argument. `004418E0` logs the zone label and calls `00484740` on the map-zone manager's zone list at `*(DAT_00503A78 + 0x30) + 0x5D8`.

## Zone construction and lookup

- `00484740` searches for a zone with the supplied label through `00483B60`. If none exists, it creates the named record through `004844E0`, initializes accumulated volume to zero, then appends the BoundsTrigger pointer. It adds that trigger's `004818A0` bounds volume to the zone's accumulated volume.
- `00483C80` considers a point inside a zone if it is inside any member trigger's bounds; it calls `00481840`, whose six comparisons include the bounds edges.
- `00483E10` searches all zones and returns a containing zone with the smallest accumulated member volume. Its `<=` comparison allows a later enumerated equal-volume zone to replace an earlier match. The volume criterion is the sum of member-box volumes, not a computed union volume.
- The zone iterator starts at the leftmost tree node (`0046E8C0`) and advances by the ordered-tree successor (`00432A60`), so lookup tie order is case-sensitive label order, not entity startup order.
- `00484260` clears the actors stored on each zone, then enumerates actors with `+0x18 == 2` and state `+0x120` not equal to 5 or 6. It gets each actor position through its vtable slot `+0x18`, looks up the containing zone with `00483E10`, and appends the actor to that zone's actor list. Actors without a containing zone trigger a diagnostic whose text names a victim.
- Ghidra xrefs show `00484260` called from `0044FBA0`, `0044CD10`, `00450440`, and `004504C0`. The latter is in the rescue/exit path and also looks up a final-exit trigger's zone. These callsites establish specific recomputation points, not a per-frame zone update schedule.

## Named-zone comparison mode

The Training named-zone tree is case-sensitive. The call at `00441A17` passes `player + 0x5D8` as `this` to `00484740`; its `00483B60` lookup reads the comparison-mode byte at tree offset `+0x18`. Player constructor `00453D50` initializes the tree header at `player + 0x5D8` (zeroing `+0x5D8..+0x5EC`) and explicitly stores byte value `1` at `player + 0x5F0`, which is tree `+0x18`. In both lookup `00483B60` and insertion comparator `0047C930`, zero selects `__stricmp`, while nonzero selects bytewise case-sensitive comparison. Thus the authored Training labels are looked up and inserted with case-sensitive matching. This is directly source-verified; no live capture is needed for the comparator mode.

| Training entity | Model | `makezone` label |
|---:|---:|---|
| 114 | 47 | Chopping and Throwing Exercise |
| 135 | 51 | Jumping and Navigation Exercise |
| 158 | 58 | Fire Extinguishing Exercise |
| 190 | 73 | Survivor Extraction Exercise |
| 234 | 82 | Crouching and Jumping Exercise |

The trigger-bound construction and exact five-box data cross-check are separately recorded in [`training-zone-bounds-ghidra-audit.md`](training-zone-bounds-ghidra-audit.md). Ghidra now verifies the generic `C_Model`-to-runtime-model-pointer handoff consumed by `00441040`; the remaining instance-level caveat is tying each extracted Training entity ID to its particular runtime `geEntity`/actor.

## Native gap and verification limits

`src/main.cpp` now handles Training `makezone` during entity startup. It resolves the native Training asset record by entity index, requires an `RCS_BoundsTrigger`, and adds its authored label and Ghidra-verified contracted bounds to `src/zones.h`. That module groups labels case-sensitively, computes each box volume with the existing x87 64-bit-significand helper, rounds each accumulated volume back to float, performs inclusive point-in-zone checks, and selects the containing zone with the smallest total volume in source tree order. `--training-zone-check` checks all five labels, entity IDs, bounds, volume values, inclusive edges, centers, and outside points.

This slice does not yet populate actor membership, call zone lookup from a reconstructed victim/player state update, or perform the original exit-zone check. The generic Genesis parser conversion from `C_Model` to the BoundsTrigger's UserData pointer is source-verified, but mapping each original runtime entity ID to its specific parsed `geEntity` instance still lacks a live/per-instance capture. The native uses the extracted Training asset index for its own entity dispatch. The `00484260` xrefs establish specific membership recomputation callsites, not a per-frame schedule; do not infer cadence from them.

Ghidra provenance: preserved-project `DecompileFunctions.java` queries for `00441040`, `00475310`, `00470150`, `00484740`, `00483B60`, `00483C80`, `00483E10`, `00481840`, `004818A0`, `00484260`, `004504C0`, `00453D50`, `004844E0`, `0047C930`, `0046E8C0`, `00432A60`, and `00454330`; `FindStringReferences.java makezone`; `AddressXrefs.java 00484260 004504C0 00484740 00483B60 004844E0 00503A78`; and `InstructionContext.java 00441A1D`. The missing handler boundary was recovered in `/private/tmp/scriptactor-462660-audit` with `DefineAndDecompile.java 00441A60` after the vtable pointer was checked; `DataValue.java`/`AddressXrefs.java` were used for `004E1088` and `004E108C`. The preserved Ghidra project remained read-only. No original runtime oracle was used.

## Rescue-path zone refresh and exit gating follow-up

Read-only Ghidra follow-up (`DecompileFunctions.java 0044FBA0 0044CD10 00450440 004504C0 00451160 0045BEF0 0045C000 00484260`; `AddressXrefs.java 00450440 004504C0 00451160 004513A0 00484260 0044FBA0`; `FieldStoreAudit.java 0x3528 0x352c 0x3530`) confirms these details:

- Successful pickup enters `00450440` only while Player `+0x3534` is empty. It stores the victim pointer, then `0045BEF0` sets victim `+0x120` to state 5 before `00484260` recomputes memberships. Since the membership pass excludes states 5 and 6, the carried victim is absent from every zone actor list at that recompute point.
- Rescue through `00451160` sets victim state 6 via `0045C000`, clears Player `+0x3534`, then calls `004504C0`; that routine recomputes memberships again. The released/rescued victim is still excluded by the same state gate.
- `004504C0` first refreshes memberships. If the cached type-2 count at Player `+0x3528` is still `-1`, it then calls `0044FBA0`, which counts global-list objects with type `+0x18 == 2`, writes that total to the UI statistic named `Rescues`, and performs another membership refresh. The player constructor initializes `+0x3528` to `-1`, `+0x3530` to 0, and `+0x352c` to 0.
- Before processing exit prompts, `004504C0` returns while `(Player+0x3528 - Player+0x3530) > Player+0x352c`. `00454750` increments `+0x3530` while RescueBot handles a victim. The source therefore verifies this gate expression; it does not establish a semantic name or authored Training override for `+0x352c`.
- Once past that gate, `004504C0` scans global-list type-8 entities whose `touch` script is `finalexit` or `finalexitloadmap`, gets each trigger's bounds center through `0044BE90`, and maps that point with `00483E10`. It deduplicates the resulting zone pointers. If none map, it sets Player `+0x3438` and calls `0044C360`; otherwise it prints the available zone labels and calls `00440F50(trigger, true)` for each scanned trigger. Thus the final-exit query uses each exit trigger's center point and zone lookup; it does not consult the per-zone victim arrays built by `00484260`.

The same membership rebuild also occurs at the end of `0044CD10`: that Player routine resets position/state, calls `0044C360`, then calls `00484260`. Ghidra xrefs place direct calls to `0044CD10` at `0044DF1E` inside `0044D550` and `00450415` inside `0044FF10`. The latter routine scans type-2 actors for downed firefighters and may create a `RCS_VictimRescue` actor before reaching the reset. These give concrete setup/state-transition refresh points; they do not establish a per-frame membership schedule.

The native code currently has the verified zone builder/point lookup and static `VictimSpawn` positions, but not original live actor objects with the source type/state fields, their virtual position method, or the pickup/release transitions. Keeping membership reconstruction disconnected is the safe seam until those actor states and source event hooks are modeled; this audit does not infer a per-frame refresh cadence.

## Player `finalexit` command and occupied-zone warning

`00451870` dispatches Player commands `finalexit` to `00450A00` and `finalexitloadmap` to `00450A70`. Each lazily initializes the type-2 entity count through `0044FBA0`, then compares `total - rescued - Player+0x352C` with zero. The zero branch completes `finalexit` by setting Player `+0x3438` and calling `0044C360`; `finalexitloadmap` checks/loads `data\\resdefs\\rescue.rrf`. A nonzero remainder calls `00483F20`, which enumerates zones in tree order and collects those with a nonzero actor count at zone `+0x30`; the UI warning lists their labels as “There are still survivors in” those zones. The meaning of the `+0x352C` threshold is still unresolved.

This command path is distinct from `004504C0`: that routine maps final-exit trigger centers to available zone labels and does not read the per-zone actor lists. The extracted Training entities author exit triggers as `touch: tell player escape`; none of the extracted Training scripts contain `finalexit` or `finalexitloadmap`. Thus the occupied-zone warning is verified reusable Player behavior, but it does not establish that Training invokes those two commands.

Ghidra provenance for this follow-up: `DecompileFunctions.java 00450A00 00450A70 00451870 00483D10 00483F20 0044FBA0 0044CD10 0044D550 0044FF10`; `AddressXrefs.java 00483F20 00450A00 00450A70 00484260 0044FBA0 0044CD10`; and `FindStringReferences.java MapZone zone rescues finalexit survivor victim`. The preserved project was opened with `-noanalysis -readOnly`; no live capture was used.
