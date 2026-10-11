# Training ladder commands and player movement state

Read-only Ghidra 12.0.4 review of the preserved `FDNYFirefighter.exe` project, plus the extracted Training entity records. No live gameplay capture was used. Ghidra outputs are provisional decompilations; the field offsets, branch tests, call targets, and original entity records are the evidence retained here.

## Authored Training ladders

Training has two ladder BModels:

| Entity | Name | Model | Authored location | Bounds | Script events |
|---|---|---:|---|---|---|
| 108 | `ladder2` | 5 | `(-248, -372, -60)` | `(-252, -504, -76)` to `(-244, -264, -12)` | `touch: tell player climb`; `untouch: tell player unclimb` |
| 110 | `ladder3` | 6 | `(216, -376, -1044)` | `(204, -504, -1052)` to `(212, -264, -988)` | `touch: tell player climb`; `untouch: tell player unclimb` |

Both records set `E_Reach=5` and `F_HitPoints=0`. The native loop dispatches their scripts from the extracted AABB bounds. The original ladder touch-volume and callback scheduling have not been compared, so the host contact query is not claimed to match them exactly.

## Player command path

Ghidra string references place `climb` and `unclimb` in the Player script handler `00451870`. Its `climb` branch calls `0044C0B0` at `00451B92`; its `unclimb` branch changes state only when the Player is currently in state 5.

- `0044C0B0` enters state 5 only if Player state `+0x18C` is not already 5 and pointer-sized field `+0x3440` is zero. It then calls `0042D280` on Player `+0xFC` with `(0,0,0)`. That helper stores three zero floats. The movement updater stores its velocity vector at `+0xFC..+0x104`, so this clears velocity on climb entry.
- `unclimb` changes state 5 back to state 3; otherwise it leaves the state unchanged.
- The Player constructor `0044D550` initializes state `+0x18C` to 3 and `+0x3440` to zero. Ghidra's bounded direct-store scan found only three writers to `+0x3440`: constructor zero at `0044D550`, pointer assignment at `0044BC40`, and clear at `0044CD10`. `0044BC40` also writes Player state 7; its direct caller is `0043D2B0`. The pointer's exact semantic name and whether that separate path can run in Training remain unresolved. Do not treat the gate as globally irrelevant.
- Player movement updater `00452570` reads state 5 directly. Its state-5 path applies player-view orientation to the input vector, uses the normal movement speed path (400), selects the grounded/climb smoothing factor (0.2 raised to `27 × frame delta`), and suppresses the normal `-1000` vertical acceleration. These are source-level branches; their full native frame and collision integration has not been compared as a complete Training run.
- The `JUMP` branch reaches `0044E990`. Once Player timer `+0x388` plus 500 ms is no later than global time, state 5 becomes state 3 and the updater writes `400` to Player `+0x100`. Outside state 5, the helper requires the grounded byte before applying its ordinary jump update. The native climbing jump path mirrors the state exit, velocity assignment, and 500 ms cooldown, but full input/frame timing parity remains open.

The current native `climbing` flag models Training's observed state-3 ↔ state-5 path: `climb` clears velocity and enables the pitch-directed movement branch; `unclimb` disables it; a jump exits and sets vertical velocity. It does not model the `+0x3440` gate or other Player states. Training's authored `setrescuebot 0` writes a separate boolean at `+0x32E4`; that alone does not prove the `+0x3440` pointer stays zero throughout a run.

## Remaining verification

- Compare original BModel ladder touch bounds and touch/untouch event cadence against the native AABB dispatch.
- Establish the runtime role and Training value of Player `+0x3440`, including whether the `0043D2B0 → 0044BC40` path is reachable in Training.
- Compare the complete state-5 movement, jump cooldown, collision, and callback order against the original runtime before claiming ladder parity.

Ghidra evidence: `FindStringReferences.java climb unclimb ladder`, `DecompileFunctions.java 00451870 0044C0B0 0042D280 00452570 0044E990`, `AddressXrefs.java 0044BC40 0044C0B0`, and `FieldStoreAudit.java 0x3440`, all against the preserved project with `-noanalysis -readOnly`. Raw local exports are under `evidence/gameplay/`, `evidence/mechanics-decompiled/`, and `evidence/player-tool-pose/state-offsets/`.
