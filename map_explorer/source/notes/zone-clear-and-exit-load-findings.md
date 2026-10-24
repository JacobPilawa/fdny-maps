# Zone clear and exit/load trigger findings

Date: 2026-10-17  
Scope: static review of the extracted BSP entities and EVS files, plus the existing read-only Ghidra audit of `FDNYFirefighter.exe`. No viewer code or viewer data was changed.

Paths below are relative to the project root unless otherwise stated.

## Main findings

1. `makezone (Name)` groups one or more `RCS_BoundsTrigger` boxes under a named zone. It defines the zone's spatial bounds; it is not itself the clear test.
2. In the original executable, `ifclear Name (commands)` runs its branch when that zone has zero current victim actors in its zone-membership list. `ifnotclear` runs the branch when the list is nonempty. The matching logic is in the script handler and the zone actor-list code.
3. A victim being carried or handled for rescue is excluded from that list (runtime states 5 and 6). As a result, a zone door can become `ifclear` while its victim is being carried, before the final level exit is available.
4. `finalexitloadmap next_map` has a separate, global survivor-count gate. It is not just another `ifclear` test. Bypassing a local door check alone does not bypass the stage-transition requirement.

## What the zone clear test counts

The original script handler resolves the supplied zone name. For `ifclear`, it runs the nested script only when the zone's actor count is zero; for `ifnotclear`, it runs the nested script when the count is greater than zero. A missing zone logs `Zone "..." not found!` and does not run the branch.

The zone actor list is rebuilt from live actor entities of victim type. During that rebuild, victims in runtime states 5 and 6 are skipped. The research notes identify state 5 as picked up and state 6 as being handled/rescued. The list is therefore a count of eligible victims currently assigned to the zone, not a count of fires, doors, or all victims originally authored in the room.

Zone assignment uses actor position and the bounds of `makezone` trigger boxes. Repeated boxes with the same label form one zone. If boxes with different labels overlap, the lookup chooses the containing zone with the smallest total member-box volume. Membership is refreshed at identified setup, pickup, rescue, and exit-related callsites; the audit does not establish a per-frame refresh schedule. A roaming victim crossing a boundary may therefore not be reassigned until a rebuild occurs.

## Authored clear gates found

| Map | BSP entity | Condition and effect |
|---|---|---|
| `oil_rig_A` | D5, `door13` | Touch opens via `*DoorOpen` if `Dormatory` is clear; otherwise shows “There are still survivors in the Dormatory.” Untouch closes it if the zone is clear. |
| `oil_rig_B` | D7, `door101` | Touch opens via `*DoorOpen` if `Warehouse` is clear; otherwise shows “There are still survivors in the Warehouse.” Untouch closes it if the zone is clear. |
| `oil_rig_C` | D3, `trapdoor 2` | Touch opens if `Tower` is clear; otherwise shows “There are still survivors on the Tower.” |
| `oil_rig_C` | D4, `trapdoor 3` | Touch opens if `Complex` is clear; otherwise shows “There are still survivors in the Complex.” |
| `oil_rig_C` | T9, `quake trigger4` | Calls the external `*TowerStairs` EVS macro. That macro requires both `Complex` and `Tower` to be clear before starting the quake/stair-drop sequence. |

The `DoorOpen` and `DoorClose` macros are defined in `map_explorer/source/scripts/scripts/doors.evs`. The `TowerStairs` macro is defined in each `map_explorer/source/scripts/maps/oil_rig_*.evs`; the BSP trigger that calls it is in `oil_rig_C`. This is one reason a BSP-only view can miss the full behavior: a BSP script can invoke a macro whose condition lives in the separate map EVS file.

The extracted campaign BSPs contain these `ifclear`/`ifnotclear` gates. Other `makezone` triggers also drive effects such as fog, oxygen, and ambient sound, but those are not clear gates.

## What the final exit/load trigger does

The BSP touch scripts name the destination map directly:

| Map | Exit entity | Authored command |
|---|---|---|
| `boat_a` | T14 | `finalexitloadmap boat_b` |
| `oil_rig_A` | T5 | `finalexitloadmap oil_rig_B` |
| `oil_rig_B` | T10 | `finalexitloadmap oil_rig_C` |
| `oil_rig_C` | T11 | `finalexitloadmap oil_rig_D` |
| `subway_a` | T37 and T39 | `finalexitloadmap subway_b` |

The original player-command handler checks the difference `total victims - rescue count - player field at +0x352C`. A zero result proceeds with the `finalexitloadmap` command; a nonzero result does not load and reports the zones that still have victims. The player constructor initializes that field to zero in the audited Training path. Its campaign meaning or any authored override has not been resolved, so the safest description is “global remaining-victim gate,” with an all-victims-rescued outcome under the observed zero threshold.

There is also an exit-activation pass that scans `RCS_BoundsTrigger` entities with `finalexit` or `finalexitloadmap` touch scripts. It waits for the same global count gate before activating those exit triggers. Once activated, it maps each exit trigger's center point to a zone to tell the player where to leave. This exit-zone lookup is separate from the per-zone victim list used to explain a blocked `finalexitloadmap` command.

## Possible bypass routes, if you choose to pursue one

- **Open a particular gated door:** its local BSP script contains the `ifclear` branch. The condition can be removed or made unconditional at the script layer. A normal gameplay shortcut may already exist: carrying the victim out of the zone removes that victim from the zone list on the pickup refresh, which can make the door's zone clear before the victim is delivered.
- **Advance to the next map early:** changing only the BSP exit's `finalexitloadmap` text is unlikely to be enough. The original game separately gates exit-trigger activation and checks the global remaining-victim count in the player command. A true stage-skip would need to address those runtime gates too.
- **Move or rename zone volumes:** this can change victim assignment, door checks, and the zone label associated with an exit. It has wider effects than changing one local condition.

No bypass has been implemented or tested in-game.

## Evidence and limits

- BSP scripts and `makezone` entries: `map_explorer/source/{oil_rig_a,oil_rig_b,oil_rig_c}/entities.txt` and corresponding `annotated_entities.json` files.
- External EVS definitions: `map_explorer/source/scripts/maps/oil_rig_A.evs` through `oil_rig_D.evs`; shared door macros: `map_explorer/source/scripts/scripts/doors.evs`.
- Original executable zone and exit analysis: `decomp/evidence/training-zones-runtime-audit.md`, including `00484260` (zone actor-list rebuild), `004772C0`/`00477500` (clear/not-clear branch checks), `004504C0` (exit activation), and `00450A70`/`00451870` (`finalexitloadmap`). The decompilation `decomp/evidence/interactions/0047a1f0_FUN_0047a1f0.c` shows the EVS handler dispatching both conditions.
- The existing browser reconstruction helper at `map_explorer/source/research/reconstruction_main.ts` has its own simplified `zoneIsClear()` that checks whether fires in the box are out. That helper is an approximation and does not describe the original executable's `ifclear` victim-list behavior.
- These findings are static and source-backed. No original gameplay capture was made. In particular, the `+0x352C` threshold meaning and the exact timing of all zone-list refreshes remain open questions.
