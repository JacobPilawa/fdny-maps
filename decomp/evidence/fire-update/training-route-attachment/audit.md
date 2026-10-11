# Training fire route attachment audit

This note separates the route-spread code present in the executable from what the preserved Training inputs establish.

## Verified in Ghidra

- The Ghidra project analyzes `FDNYFirefighter.exe` with SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Queries here used Ghidra 12.0.4 headless with `-noanalysis -readOnly`.
- Function `00449A80` compares command strings `moveto`, `stopmove`, and `setsize`. Its `moveto` branch iterates the supplied point data and calls `00448A90` (`assembly.txt`, `00449a80_FUN_00449a80.c`).
- The preserved Ghidra reference table has one direct code reference to `00448A90`: call at `00449CE1` inside `00449A80`. In the `moveto` path, `00448A90` resolves the referenced object, stores it at fire offset `+0x100`, prepares route position/direction/progress fields, and writes state `4` at fire offset `+0x70` (`assembly.txt`, `00448a90_FUN_00448a90.c`, `references.tsv`).
- Fire Think `00449FE0` calls `00449340` only when state `+0x4c` is `4`. `00449340` advances that route and can create another fire. The existing spread export is preserved at `../00449fe0_FUN_00449fe0.c` and `../subobject-init-followup/00449340_FUN_00449340.c`.
- Ghidra currently has no reference to the entry of `00449A80` in `references.tsv`. This does not rule out indirect or runtime registration, so how this handler is entered remains unresolved.

## Training input boundary

- The extracted Training map contains seven `RCS_FireRescue` entities (source indices 162, 163, 165–169). All have `P_NumKeys=0`; their four authored size keys are zero vectors. The extracted entity records contain no movement, route, or path properties.
- A case-insensitive text search of `decomp/assets/training/` and `decomp/evidence/original/training*` found no literal `moveto`, `stopmove`, or `setsize` command.
- These input checks establish no authored Training route in the preserved extracted records. They do not establish that runtime code cannot create or attach a route by another mechanism.

## Native implementation consequence

Keep the state-4 route/spread behavior documented for later levels and dynamic cases. Do not treat it as verified Training behavior until the handler's registration/reachability or a runtime trace is established. The current live capture path is deferred; no player action is needed now. Fire Think's scheduler and Training callback order remain separate open questions.
