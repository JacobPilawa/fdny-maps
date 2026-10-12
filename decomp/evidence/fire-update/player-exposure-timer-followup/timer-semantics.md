# Player pain deadline from raw instructions

Read-only source audit of `00451640`, using `../player-exposure-assembly.txt`, preserved helper instructions in `random-conversion-assembly.txt`, and the original clock path in `../../timing-audit/clock.asm`.

At `00451748`, the player callback compares global clock `[0x00502AFC]` against player field `+0x390`. `JBE 00451820` skips the pain branch while the unsigned current value is less than or equal to the stored value. In the branch, it resolves `RC_Pain` through `004897C0` before calling `00498B30(1000,2500)`. The key lookup can play a matching chatter set: flag 0 reaches `00488D40`, which consumes one shared draw and a second only when avoiding a repeated sound. After that host callback, the deadline range helper consumes exactly one further `00404AC0` draw and returns its scaled result in x87 `ST(0)`.

The raw instructions after the helper reload `[0x00502AFC]`, convert the current dword to x87 (correcting negative signed interpretations to the unsigned value), add it to the random delay with `FADDP`, and pass the sum to `004B9400`. The conversion helper produces the integer form and the caller stores `EAX` at player `+0x390`, then invokes `00486980` to add one to the `Serious Burns` statistic. Thus this field is a future time deadline, and its pain draw occurs only after the previous deadline. `004B9400` in this callback is a float-to-integer conversion, not a draw.

Ghidra's `00451640` pseudocode shows the range call and conversion but omits the current-clock addition visible in raw assembly. This note follows the instructions, not that simplified expression. It does not establish how often Training dispatches `0x7531`; capture callback scheduling before wiring the exposure path into the native tick.

Raw instructions: `player-exposure-assembly.txt`, `random-conversion-assembly.txt`. Related decompilation: `../../startup-factories/00451640_FUN_00451640.c`, `../../rng/00498b30_FUN_00498b30.c`, and `../../rng/004b9400_FUN_004b9400.c`.
