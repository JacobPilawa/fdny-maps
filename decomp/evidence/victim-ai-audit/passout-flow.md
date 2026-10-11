# Player passout and carried-victim drop

This is a static Ghidra trace of the failure/drop path. It is retained for Training failure behavior and later levels. No runtime capture establishes how often or in what scheduler pass the path executes.

## `0045C1A0`: victim drop transform and state

Input is the carried victim (`this`) and Player (`param_1`). The function:

1. Sets victim state `+0x120` to 4. Its actor subobject begins at `+0x170`.
2. If victim word `+0x180` equals 6, draws `00404B30(3)` and calls `00480EA0(victim+0x170, draw, 1.0f)`. Otherwise it draws `00404B30(victim+0x180)`, calls the same animation routine at speed `0.2f`, and writes state 4 again. Thus this helper makes one bounded shared-RNG draw on either branch; the bound differs.
3. Reads Player frame through Player vtable slot `+0x18`. It passes that frame through `00446EE0` and `0042D2B0`, then stores the resulting three words at victim `+0x28..+0x30`.
4. Reads the Player frame again, passes it through `0049E770` and `0042D2B0`, then stores three words at victim `+0x40..+0x48`.
5. Calls virtual slot `+8` on the victim frame at victim `+0x24`.
6. Reads Player frame a third time and copies frame `+0x28..+0x30` into victim position `+0x4c..+0x54`.
7. Calls `0049DA00(victim)` for model-floor placement, calls `00481140(victim+0x170, 1)`, and dispatches actor event `passout` through the actor script subobject at victim `+0xd4` (`004705B0`).

This trace records the helper identities and exact field writes without assigning a guessed semantic meaning to the two frame conversion helpers or flag value 1.

## `0044FF10`: caller behavior

Player helper `0044FF10` first increments Player `+0x3428` and copies the three words at `+0x3400..+0x3408` to Player position `+0x4c..+0x54`. If carried-victim pointer `+0x3534` is non-null, it calls `0045C1A0`, calls `0044FCE0(0)`, raises Player position Y by the value at `004E1698`, subtracts that value from the dropped victim frame's Y translation (`victim frame slot +0x18`, frame field `+0x28`), then clears Player `+0x3534`.

After the drop, another branch runs only when global mode at `DAT_00502AAC+0x6e4`, field `+0x2c`, is 2 and the helper argument is zero. It walks entities of type 2, compares their script name with `herodown`, counts states 4 or 7, and when the count is below 3 constructs a `POFighter` `RCS_VictimRescue` at Player position with script name `herodown`, increments Player `+0x3528`, and updates the `Rescues` statistic. The constructor/spawn side effects are separate from the carried-victim bounded draw.

## Callers mapped so far

- Player state-6 helper `00451080` calls `0044FF10(player, 0)` when its timer-derived value drops below the mapped threshold; otherwise it updates the Player view and calls a vtable entry at `+100`.
- Damage helper `00450FB0` calls `0044FF10(player, 1)` only for its `param_1 == 2` branch. Other branches set Player state 6 and dispatch the `RC_PassOut` variable without that immediate call.
- Script handler `00451870` has a `RC_PassOut` path through `00450FB0`; the exact runtime script reachability is not captured.

Raw Ghidra function exports are retained locally under `evidence/collision-audit/0045c1a0_FUN_0045c1a0.c`, `evidence/entity-scheduler/0044ff10_FUN_0044ff10.c`, `evidence/startup-factories/00451080_FUN_00451080.c`, and `evidence/startup-factories/00450fb0_FUN_00450fb0.c`. These raw exports are excluded from the Git mirror by `decomp/.gitignore`; this authored trace preserves the findings in the repository.
