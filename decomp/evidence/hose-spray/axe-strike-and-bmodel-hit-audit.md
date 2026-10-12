# Training Axe strike and BModel hit leaf

Status: the leaf `0043E2A0` is a source-comparable native/P-code chunk; the caller strike `00460B90` remains unmapped as a whole. The executable is pinned at SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Ghidra exports are [`00460B90.c`](../ghidra-original-c/00460B90.c) and [`0043E2A0.c`](../ghidra-original-c/0043E2A0.c); the callsite listing for the strike is in [`item-pose/assembly.txt`](../item-pose/assembly.txt).

## Completed leaf: `0043E2A0`

`00460B90` calls this with a type-4 candidate from the global BModel candidate iterator. The receiver is a BModel root. `receiver+0xEC` points to its data record; `data+0x34` is hitpoints, and `data+0x38` is a script-actor pointer. The first gate returns zero when hitpoints are zero or the script-actor pointer is nonzero.

The remaining strike-time gate loads a **float** from `BModel+0x1EC` and uses x87 `FCOM/FCOMP` against **double** constants at `004E0B68` (`+1e-28`) and `004E0B60` (`-1e-28`). It proceeds when `time >= +1e-28` or `time <= -1e-28`; the tiny open interval returns zero. Since the field stores float32 and both operands are binary64, no field value can equal either threshold exactly. The fixture checks nearest float values on both sides of each threshold. This raw instruction width corrects the misleading apparent float cast in Ghidra's C-like output.

On an accepted chop, the exact local transaction is:

1. Set result flags to `2`.
2. Construct command string `"chop"` (`00402790`) and dispatch it through `004705B0` with `BModel+0x24` as receiver and the temporary command-string reference. If dispatch returns nonzero, OR flag `4`.
3. Decrement `data+0x34` after the chop event, even if the event returned false.
4. If hitpoints became zero, construct/dispatch `"break"`; clear flag `4` if this event returns false; OR flag `1`; then call `0043D660(BModel)` unconditionally. `0043D660` is the destroy/open boundary; its internals are not included in this leaf.

[`src/bmodel_axe_hit.h`](../../src/bmodel_axe_hit.h) implements the gates, post-dispatch decrement, result bits, order, and injected script/destroy boundaries. [`BModelAxeHitPcodeOracle.java`](../../tools/ghidra/BModelAxeHitPcodeOracle.java) executes original `0043E2A0`; only string construction `00402790`, script dispatch `004705B0`, and destruction `0043D660` are stubbed. The fixture [`bmodel-axe-hit-pcode-oracle.json`](bmodel-axe-hit-pcode-oracle.json) has ten hash-pinned cases for hp zero, script actor presence, the interval, neighboring float32 values around both binary64 thresholds, both chop outcomes, and break success/failure. For service calls, it records the constructor's shared string reference, the script receiver (`BModel+0x24`) and string-reference argument, and the destroy receiver (BModel root); the native checker asserts those addresses and their order alongside hp and result bits. Focused CTest `bmodel_axe_hit_pcode` passes 10/10 with zero mismatches.

Training's shared `Door` macro sets hitpoints to zero. Therefore this leaf rejects every authored Training door before dispatching `chop`; there is no Axe-driven door decrement or break through this path. This does not establish normal door motion behavior or prove that a strike's spatial query reaches the door. See the `sethitpoints` row in [`training-script-command-audit.md`](../training-script-command-audit.md#command-host-status).

## Open caller: `00460B90`

The complete callback is reached from the Axe child Think switch in `004610E0` (child vtable `+0x20`, state `1`); see [`axe-child-think-audit.md`](axe-child-think-audit.md). It begins with `0045FEC0(Axe,1.0)` and returns unless both Axe timer values at `+0x340` and `+0x33C` pass strict comparisons against binary64 `0.275` (`004E28B0`). It then forms a view-frame-relative start/end and swept box from Player view data at `Player+0x34A4/+0x34CC`, Axe state/orientation, and helpers `00447830`, `00446390`, `0042D050`, `00446EE0`, `00447710`, `0049E770`, and `0043B2D0`. The final world-query arguments go to `00492CE0` with axis extents `(6,6,6)` and `(-6,-6,-6)`, Player/world pointer `*(DAT_00503A78+4)`, and additional vectors. `00492CE0` calls `geWorld_Collision` with mask `0x43`, collision type `2`, and output vectors; a zero result returns before Axe Hits stats or state transitions.

After a positive query, the handler increments the shared Axe Hits stat (`00486980`), updates Axe+`0x36C`, obtains Player's virtual `+0x18` result, changes follower parenting with `00483A20`, calls `0045FB90(Axe,1.99)`, stops sound handles at Axe+`0x3B8` and later `+0x400`, writes attack/model state, then enumerates BModel candidates through `00481C00` (which initializes through `00481BA0`). It considers only candidate type `4` at `candidate+0x18`, applies inclusive axis-aligned overlap with radius `8` via `004816C0(candidate+0x240,position,8.0)`, and calls this completed leaf `0043E2A0`. Result bit `4` drives a hide at Axe+`0x3DC` or `+0x424`; bit `1` sets child state `3`, timer `0.5`, and reparents Axe+`0x15C` to Player+`0x34A4`. After a candidate pass it sets shared `DAT_00502E04=1`.

`00460B90` should be implemented as a composed source module only after its caller chunks are explicit. Remaining separate packages are:

- pose math / sweep construction and its original float/x87 behavior (the listed transform helpers);
- `00492CE0` / `geWorld_Collision`, including authored world collision inputs and result vectors;
- the `00481C00`/`00481BA0` candidate-list representation and ordering plus exact target candidate data;
- the effects behind `00486980`, `00483A20`, `0045FB90`, `004A11A0`, `0043D660`, and ScriptActor `004705B0`.

These direct dependencies are the reason the native leaf is deliberately `0043E2A0` only: the caller performs spatial queries and multiple stateful engine effects before and after it, so an isolated approximation of those services would not be a completed strike. Existing Axe root/action and child modules remain unchanged; no strike or Training runtime parity is claimed.
