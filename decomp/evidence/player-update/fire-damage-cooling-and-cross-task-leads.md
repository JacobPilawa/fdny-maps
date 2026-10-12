# Player burn cooling and carry-over fire-system findings

## Training-critical burn cooling path

The previous task **“Find FDNY project files and chats (2)”** contains a Ghidra static fire-system audit in the sibling `fire_spread/` directory. Its preserved executable has SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, matching this `decomp/` project. `00452570_FUN_00452570.c` was copied into this workspace under `decomp/evidence/player-update/`; its SHA-256 is `17dba566c95d14ff9efa2e921a73cced917d9ad3ca7a35217de302431973a9c2`. The repository's `.gitignore` deliberately keeps raw Ghidra C and assembly exports local. The assembly listing is available locally in `evidence/player-tool-pose/movement-callers/assembly.txt` at `00452974–00452A5F`; this note and the hash-pinned P-code fixture are the tracked record. The C export remains provisional.

The Player movement updater is reached through the Player `+0x140` subobject route documented in `../player-update-scheduling-audit.md`. In the normal non-state-6 path of `00452570`, after status service `004559D0`, instructions `004529A2–00452A58` test four bytes at subobject `+0x33FC..+0x33FF` (Player `+0x353C..+0x353F`) and clear subobject byte `+0x494` (Player `+0x5D4`). When all four directional bytes are zero, x87 subtracts `0x004E1814 * [0x00502B04]` from subobject `+0x33F8` (Player burn accumulator `+0x3538`), stores binary32 without popping the extended result, then compares that still-extended value strictly below binary64 `0.1`. `pe_constants.py` confirms `0x004E1814` is float `0.10000000149` and the comparison constant at `0x004DF7B8` is binary64 `0.1`; these differ at the boundary. If the comparison passes, Player byte `+0x12C` must be nonzero and float `+0x3450` must be strictly greater than `0.5` before the call to `00441E90`. That helper copies three DWORDs from Player `+0x4C` to `+0x3400`; no higher-level meaning is assigned to these words here. The helper call occurs before the negative stored accumulator is clamped to zero. All four directional bytes are then cleared even when one had blocked cooling. A negative accumulator is also clamped when any directional byte is set. When Player state is 6, this block is bypassed for a separate `00451080` call; that path is outside this component.

The exact local assembly excerpt is retained here because raw `.txt`/assembly exports are ignored by repository policy. `ESI` is the Player `+0x140` movement subobject:

```text
00452974  CMP dword ptr [ESI + 0x4c],0x6
00452978  JNZ 0x0045298a
0045297a  LEA ECX,[ESI + 0xfffffec0]
00452980  CALL 0x00451080
00452985  JMP 0x00453a50
0045298a  XOR EDX,EDX
0045298c  MOV DL,byte ptr [ESI + 0x494]
00452992  LEA ECX,[ESI + 0x4b4]
00452998  MOV dword ptr [ESP + 0x38],ECX
0045299c  PUSH EDX
0045299d  CALL 0x004559d0
004529a2  MOV AL,byte ptr [ESI + 0x33fc]
004529a8  TEST AL,AL
004529aa  MOV byte ptr [ESI + 0x494],0x0
004529b1  JNZ 0x00452a22
004529b3  MOV AL,byte ptr [ESI + 0x33fd]
004529b9  TEST AL,AL
004529bb  JNZ 0x00452a22
004529bd  MOV AL,byte ptr [ESI + 0x33fe]
004529c3  TEST AL,AL
004529c5  JNZ 0x00452a22
004529c7  MOV AL,byte ptr [ESI + 0x33ff]
004529cd  TEST AL,AL
004529cf  JNZ 0x00452a22
004529d1  FLD float ptr [0x004e1814]
004529d7  FMUL float ptr [0x00502b04]
004529dd  FSUBR float ptr [ESI + 0x33f8]
004529e3  FST float ptr [ESI + 0x33f8]
004529e9  FCOMP double ptr [0x004df7b8]
004529ef  FNSTSW AX
004529f1  TEST AH,0x5
004529f4  JP 0x00452a22
004529f6  MOV AL,byte ptr [ESI + -0x14]
004529f9  TEST AL,AL
004529fb  JZ 0x00452a22
004529fd  FLD float ptr [ESI + 0x3310]
00452a03  FCOMP double ptr [0x004de6f8]
00452a09  FNSTSW AX
00452a0b  TEST AH,0x41
00452a0e  JNZ 0x00452a22
00452a10  LEA EAX,[ESI + 0xffffff0c]
00452a16  PUSH EAX
00452a17  LEA ECX,[ESI + 0x32c0]
00452a1d  CALL 0x00441e90
00452a22  FLD float ptr [ESI + 0x33f8]
00452a28  FCOMP float ptr [0x004dc50c]
00452a2e  FNSTSW AX
00452a30  TEST AH,0x5
00452a33  JP 0x00452a3f
00452a35  MOV dword ptr [ESI + 0x33f8],0x0
00452a3f  CMP dword ptr [ESI + 0x4c],0x3
00452a43  MOV byte ptr [ESI + 0x33ff],0x0
00452a4a  MOV byte ptr [ESI + 0x33fe],0x0
00452a51  MOV byte ptr [ESI + 0x33fd],0x0
00452a58  MOV byte ptr [ESI + 0x33fc],0x0
00452a5f  JNZ 0x00452ac3
```

## Original P-code and native component

`tools/ghidra/PlayerBurnCoolingPcodeOracle.java` executes original P-code from `004529A2` (after `004559D0` and the state-6 dispatch) to `00452A5F`. The 12 hash-checked samples in `burn-cooling-pcode-oracle.json` cover normal cooling, each directional byte independently blocking one update, negative clamping with and without a directional byte, adjacent binary32 values below and above binary64 `0.1`, the strict `+0x3450 > 0.5` gate, the `+0x12C` gate, and the `00441E90` call. The emulator executes `00441E90` itself; the fixture confirms its receiver/source offsets and three copied words. In the negative sample the helper sees the negative stored accumulator before the following clamp stores zero. The fixture also compares the `+0x5D4` status-marker clear.

`src/player_burn_cooling.h` matches all 12 fixture cases through `player_burn_cooling_pcode`; it preserves the extended value for the `0.1` comparison while retaining the binary32 store, then clamps and clears flags in source order. The oracle starts after the status helper and does not model state-6 dispatch, the rest of `00452570`, or its recurring call schedule. Its synthetic field inputs prove bounded helper behavior, not field meanings or complete Training damage parity.

This closes the missing native update-side accumulator slice paired with `src/player_fire_message.h`, which increases `+0x3538`; `0044BB00` is the directional-indicator writer called by fire damage. The bounded arithmetic and side-copy branches now match original execution, while live callback order and the movement updater cadence remain unknown. Do not claim burn recovery timing or whole-Training damage parity from the `0.1 * dt` arithmetic alone.

The full updater contains adjacent branches whose exact side effects need separate names and boundaries. Continue from `00452A5F` to recover the remaining Player tool/interaction update in address-sized slices, preserving its fields even when Training does not immediately exercise them. Connect burn cooling to a recurring Player update only after the owner schedule and its order relative to message drains and Fire Think are known.

## Other high-value full-port findings from the prior audit

These are carry-over static findings from the same Ghidra audit. They preserve investigation leads; they do not make the Training frame schedule known.

- **Fire contact has two strength-dependent factors.** `00449FE0` updates fire strength and its alpha input; `FireRescue` contact at `004483A0` obtains radial falloff through `0044A7A0` and also multiplies by `0.25 + 0.75 * fireStrength`. The `+0xD0` alpha scale is written through `0044A610 → 0047F1F0`. The native sources already represent these arithmetic inputs through `FireUpdate`, `FireEntity::think`, `FireContact`, and `FireRescueContact`; the unresolved part is their callback timing and exact original comparison coverage.
- **Extinguisher suppression is nonmonotonic in its input.** `00461F80` doubles the sampled contact before queued message `30000`; `00448970` applies `1 - (1-influence)^2`. Thus maximum falloff influence does not produce maximum suppression, and the C++ code correctly keeps the original unclamped arithmetic. The carried rescue hose calls the same suppression routine directly with the terminal flag set; the extinguisher message uses terminal false.
- **There are two fire-spread routes.** Named FireRescue entities can attempt random nearby child creation through `004483A0 → 00448320 → 00447A90`; fixed authored route movement advances through `00449340`. The audit found Gas Station Fire42 and Military Base mortar-fire route sequences. The ordinary-fire factory is used for child flames, so they can use ordinary FireRescue damage handling. New children are unnamed and the random-spread parent gate rejects unnamed parents, which prevents recursive child-of-child spread through that path.
- **Flame jets and explosions use a separate damage path.** Their callback is `00445490`, with an outer camera-distance/visibility gate through `00441F10/0048FE20`; it computes distance to a configured segment and calls the player burn routine directly. Do not infer their behavior from ordinary-fire message `0x7531` checks.
- **Victim fire damage is a bounded negative finding.** In the audited routes, the Victim `+0x8` message handler is a no-op and the direct emitter route filters for the Player candidate kind. This does not rule out scripted mission failure or other victim-specific mechanics.
- **The local content audit covered 17 maps and 21 external scripts.** It counted 430 placed ordinary fires and preserved 83 authored `RCS_HosePath` nodes with 28 hotspots; these are installation-specific content counts, not whole-game function coverage. Training has seven named FireRescue entities, no authored size-key records/routes, three small fires with automatic spray time and four larger fires authored at 4 seconds. Existing Training evidence in `../fire-update/` and `../bsp-typedefs/training.json` remains the primary record for those details.

The complete human-readable prior report remains locally available at `fire_spread/MECHANICS.md`, with its file-hashed evidence archive at `fire_spread/evidence.zip`; that sibling directory is not part of the tracked `decomp/` repository. This transfer note and the retained `00452570` export keep the most relevant leads discoverable in the decomp project without implying that every supporting export has already been imported.
