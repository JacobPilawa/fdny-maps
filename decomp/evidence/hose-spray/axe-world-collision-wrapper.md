# Axe world-collision wrapper `00492CE0`

Executable: `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

`FUN_00492CE0` is the local wrapper called once by Axe strike `00460B90`. Its `__thiscall` receiver is the first register argument (`ECX`); six pointer arguments are on the stack in source order:

| Wrapper parameter | Caller identity at `00460B90` | Use |
|---|---|---|
| `param_1` | `&local_fc` | sweep start, three float32 values |
| `param_2` | `&local_d8` | mins, three float32 values |
| `param_3` | `local_cc + 6` | maxs, three float32 values |
| `param_4` | `local_cc + 3` | displacement, three float32 values |
| `param_5` | `&local_f0` | hit-position output, three float32 values |
| `param_6` | `local_cc` | second output vector, three float32 values |

The receiver chooses its world pointer using byte field `receiver+0x160`: zero selects `*(receiver+0x08+0x78)`; nonzero selects `*(receiver+0xB4+0x78)`. A null selected pointer returns zero without calling the engine. Otherwise the wrapper snapshots the four vector inputs into stack locals, computes `end = param_1 + param_4` with x87 float operations and stores each result as binary32, then calls the indirect Genesis service through IAT slot `004DC0BC` at instruction `00492DA0`. The original call has world, mins, maxs, start, end, contents mask `0x43`, collision type `2`, three zero flags, and a trace-structure pointer. The oracle records the real original stack identities for those locals and trace record: in its deterministic frame they are `20ffefc8`, `20ffefbc`, `20ffefb0`, `20ffefa4`, and `20ffefd4` respectively. The first call argument is the selected world pointer.

The local trace record starts at the final engine-call pointer. On a nonzero engine return, the wrapper copies trace offsets `+0x0c/+0x10/+0x14` to `param_5`, then offsets `+0x1c/+0x20/+0x24` to `param_6`, and returns one in `AL`. On zero, it returns zero and leaves both output vectors untouched. No collision geometry or trace production is inferred here.

[`../../src/axe_world_collision.h`](../../src/axe_world_collision.h) models the wrapper and exposes the engine as an injected `Collision` callback. [`axe-world-collision-pcode-oracle.json`](axe-world-collision-pcode-oracle.json) records three original-P-code cases (inactive selected world hit, active selected world miss, and null selected world), including caller pointer identities, original service argument addresses/values, untouched miss outputs, and hash provenance. The script intercepts only the indirect engine service at `00492DA0`; it executes the wrapper's original instructions. The native comparison is `../../tools/axe_world_collision_pcode_check.cpp`.

Limits: this establishes the wrapper and its inputs/outputs. It does not reproduce Genesis `geWorld_Collision`, decide whether the authored sweep intersects the BSP, or cover the rest of the Axe strike. Pose construction is implemented and compared separately in `axe-strike-pose-module.md`. Caller-side values and pointer names come from `00460B90.c`; they are not claimed as a full original caller execution. The focused `axe_world_collision_pcode` CTest runs all three wrapper cases.

## Cross-file sources consulted

- `decomp/evidence/ghidra-original-c/00492CE0.c`, `00460B90.c`, and `0043E2A0.c` for wrapper, callsite, and following candidate-hit context.
- `decomp/evidence/hose-spray/axe-strike-and-bmodel-hit-audit.md` and `README.md` for the adjacent Axe leaf's known engine boundaries and package conventions.
- `decomp/src/bmodel_axe_hit.h`, `decomp/tools/ghidra/BModelAxeHitPcodeOracle.java`, and `decomp/tools/bmodel_axe_hit_pcode_check.cpp` for native/P-code fixture conventions.
- `decomp/tools/ghidra/VictimFloorCollisionPcodeOracle.java` and `decomp/tools/ghidra/README.md` for original EmulatorHelper setup, read-only Ghidra use, and engine-collision boundary patterns.
- `decomp/CMakeLists.txt` to inspect focused-test conventions and register the native comparison.
- `decomp/evidence/original/FDNYFirefighter.exe` to verify the hash-pinned binary; it was not changed.
