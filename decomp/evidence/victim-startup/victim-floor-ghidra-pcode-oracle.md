# Training victim floor contacts: Ghidra P-code oracle

Updated 2026-10-11. This recovers the six static Training victim hull contacts that the Unicorn-based attempt in [`victim-floor-capture-attempt.txt`](victim-floor-capture-attempt.txt) could not produce. That old attempt still fails at its first `uc.mem_map` with SIGILL; this route uses Ghidra's software P-code emulator and does not start or attach to the game.

## What was executed

The read-only Ghidra 12.0.4 project identifies the input as PE32 x86 and recognizes `geWorld_Collision` at `0x10001E30`. Its non-null hull path dispatches to `0x100372A0`; traversal reaches `0x100371F0 → 0x10036EA0`. The hull leaf routine commits a new contact at `0x100371B3`, where the original instruction writes `1` to `DAT_10362F44`. This is distinct from `0x10036633`, the point-trace commit in `FUN_100363F0`. The oracle checks the function name, image language, and hull marker instruction before running.

For each authored candidate, the oracle loads a fresh synthetic runtime-memory image built from the preserved Training `collision.json`, initializes the `(-24,-24,-24)/(24,24,24)` hull and original floor-helper inputs, then executes the original DLL instructions as Ghidra EmulatorHelper P-code. It initializes `EFLAGS=0x202`, `DF=0`, x87 control/status/tag words and x87 stack; explicitly setting DF removes EmulatorHelper's uninitialized-register warning. The trace uses contents `0x43`, flags `2`, a Y start offset of `+48`, and a downward sweep of `1000`; the final floor position is the contact with Y increased by `1`.

The generated fixture is [`victim-floor-collision-oracle.json`](victim-floor-collision-oracle.json). Its provenance records SHA-256 hashes for the preserved DLL, BSP JSON, generated 16 MiB state, and candidate list. The raw generated state and candidates are kept under `/private/tmp/fdny-victim-floor-pcode-inputs`, not in the repository.

| Victim | Candidate entity | Authored XYZ | DLL contact XYZ | Distance | Final floor Y |
|---|---:|---|---|---:|---:|
| Rescue1 (191) | 191 | `(1104,-60,-4976)` | `(1104,-231.9,-4976)` | `219.9` | `-230.9` |
| Rescue1 (191) | 192 | `(1300,-120,-4952)` | `(1300,-231.9,-4952)` | `159.9` | `-230.9` |
| Rescue1 (191) | 193 | `(1472,-120,-4972)` | `(1472,-231.9,-4972)` | `159.9` | `-230.9` |
| Rescue2 (194) | 194 | `(1656,-120,-4968)` | `(1656,-231.9,-4968)` | `159.9` | `-230.9` |
| Rescue2 (194) | 195 | `(1820,-120,-4960)` | `(1820,-231.9,-4960)` | `159.9` | `-230.9` |
| Rescue2 (194) | 196 | `(1972,-120,-4960)` | `(1972,-231.9,-4960)` | `159.9` | `-230.9` |

All six return a hit, commit a new contact, report model `0` and leaf `2303`, and have normal `(0,1,0)`. A separately compiled temporary C++ harness called the current native `Collision::trace` with these six exact inputs. A bitwise comparison of contact XYZ, normal, distance, model, and final floor XYZ against the Ghidra fixture passed for all six. The native harness and generated inputs remain temporary; the Ghidra runner and input builder are retained in `tools/ghidra/VictimFloorCollisionPcodeOracle.java` and `tools/victim_floor_pcode_inputs.py`.

## Reproduction

From `decomp/`, first build the local raw inputs:

```sh
python3 tools/victim_floor_pcode_inputs.py /private/tmp/fdny-victim-floor-pcode-inputs
```

Then run the query-only oracle against the preserved Ghidra project and source files:

```sh
JAVA_HOME=/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home \
PATH=/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home/bin:/usr/bin:/bin \
JAVA_TOOL_OPTIONS=-Duser.home=/private/tmp/fdny-ghidra-user-home \
/Applications/Ghidra/support/analyzeHeadless evidence/ghidra/genesis Genesis \
  -process Genesis.dll -noanalysis -readOnly \
  -scriptPath tools/ghidra -postScript VictimFloorCollisionPcodeOracle.java \
  /private/tmp/fdny-victim-floor-pcode-inputs/state.bin \
  /private/tmp/fdny-victim-floor-pcode-inputs/candidates.tsv \
  /private/tmp/fdny-victim-floor-collision-oracle.json \
  evidence/original/Genesis.dll assets/training/collision.json
```

This verifies static world-BSP contact for these six authored candidate positions. It does not capture a full game launch or establish moving-model collision, actor construction/rendering, victim updates/AI, rescue behavior, or whole-Training RNG parity.
