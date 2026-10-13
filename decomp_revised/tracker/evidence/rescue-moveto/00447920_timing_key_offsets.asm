# FireRescue `setsize` duration-to-key-time trace

Binary: `FDNYFirefighter.exe`

SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`

Source: fresh Ghidra `disassemble_function` output for `00447920` and the call
site in `00449A80`; the Ghidra program was saved after correcting the size
helper prototype to include its fourth stack argument.

## Direct instruction evidence

```text
0044793c: MOV EAX,[0x00502afc]
00447941: ADD EAX,0xa
00447944: PUSH EAX
00447945: LEA ECX,[ESP + 0x8]
00447949: PUSH ECX
0044794a: MOV ECX,ESI
0044794c: CALL 0x00447850
00447951: PUSH EAX
00447952: MOV ECX,ESI
00447954: CALL 0x0044b620
00447959: FILD dword ptr [ESP + 0x14]
0044795d: MOV EDX,dword ptr [0x00502afc]
00447963: TEST EDX,EDX
00447965: FSTP float ptr [ESP + 0x4]
00447969: FILD dword ptr [ESP + 0x18]
0044796d: FSTP float ptr [ESP + 0x8]
00447971: FILD dword ptr [ESP + 0x1c]
00447975: FSTP float ptr [ESP + 0xc]
00447979: FLD float ptr [ESP + 0x20]
0044797d: FMUL float ptr [0x004e0ed4]
00447983: FILD dword ptr [0x00502afc]
00447989: JGE 0x00447991
0044798b: FADD float ptr [0x004dfa30]
00447991: FADDP
00447993: CALL 0x004b9400
00447998: PUSH EAX
00447999: LEA EAX,[ESP + 0x8]
0044799d: PUSH EAX
0044799e: MOV ECX,ESI
004479a0: CALL 0x0044b620
004479a5: MOV AL,0x1
004479ab: RET 0x10
```

At function entry, `this` is in `ECX`; after `SUB ESP,0xc` and `PUSH ESI`,
`[ESP+0x14]`, `[ESP+0x18]`, and `[ESP+0x1c]` are the three integer size
components, and `[ESP+0x20]` is the fourth stack argument. The latter is loaded
as a float. `RET 0x10` confirms the callee consumes four stack arguments.

The first key is the current size vector at `DAT_00502AFC + 10`. For the second
key, the fourth argument is multiplied by the float at `004E0ED4` (1000.0f),
added to the game clock at `DAT_00502AFC`, passed through `FUN_004B9400`, and
used as the key timestamp by `AddFireRescueMotionKey` (`0044B620`). The branch
at `00447989` adds `004DFA30` when the loaded clock is negative; the common
nonnegative-clock path skips that correction.

The `00449A80` `setsize` branch reads the fourth token through
`FUN_0040CC50(&local_30, local_1c)` and calls `00447920` at `00449FAA`. The
call site emits four stack operands. This agrees with the corrected four-arg
prototype, while fresh C-like output still does not render the x87 duration
calculation even with that prototype.

## Authored examples and interpretation

The hash-pinned rescue script inventory finds 12 FireRescue-targeted `setsize`
commands in `gas_r.evs`, including `300 375 300 .6`, `180 350 250 .6`, and
`10 10 10 2`. The timed-vector consumer at `0044AE80` interpolates between
keys. Thus, `.6` and `2` control the target-key time relative to the current
clock; interpreting them as seconds gives approximately 600 and 2,000 clock
units and remains an inference from the 1000.0f multiplier. The authored
`boat_a.evs` command `setsize 3` targets exploder actors and has a different
operand shape; this FireRescue helper is not evidence for that command.

This evidence was entered as comments at `00447979` and `00449FAA` in the
project-local Ghidra database. It does not establish the recurring callback
schedule or live visual effect; those still need source-backed port behavior or
an original-game capture.
