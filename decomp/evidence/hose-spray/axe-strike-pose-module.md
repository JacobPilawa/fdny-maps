# Axe strike pose and swept-query inputs

Status: the native pose/query builder matches three original-P-code collision-call snapshots bit-for-bit (`collision_miss`, `collision_success_empty`, and `candidate_hit_flags_1`); focused CTest `axe_strike_pose_pcode` passes. Source entry is Axe strike `00460B90` in [`../ghidra-original-c/00460B90.c`](../ghidra-original-c/00460B90.c); the pinned executable SHA-256 is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Recovered input construction

`src/axe_strike_pose.h` transcribes the query-input portion before `00492CE0`:

1. Copy Player frame `+0x10` with `00447830`; scale by 20 with `00446390`; add Player `+0x34CC` with `0042D050` to form the query start.
2. Copy frame `+0x04` with `00446EE0`. Scale first by `-1` or `+1` from Axe byte `+0x1FC` using `00447710`, then by 10 using a separate `00446390` call and float store. Add that offset to the start.
3. Copy frame `+0x10` again and scale by 60. Copy frame `+0x1C` with `0049E770` and scale by 100. `0043B2D0` stores the difference `(frame+0x1C)*100 - (frame+0x10)*60` as the sweep displacement.
4. Initialize the box bounds to min `(-6,-6,-6)` and max `(6,6,6)`. Before `00492CE0`, the 12-float local array has zero output words `[0..2]`, sweep displacement at `[3..5]` and `[9..11]`, and positive extents at `[6..8]`; the hit-position output vector is also zeroed.

The copy helpers were checked in the original PE32 instructions: `00447830` reads the three words at `this+0x10`; `00446EE0` reads `this+4`; `0049E770` reads `this+0x1C`. `00446390` and `00447710` multiply each vector component and store a float before proceeding; `0042D050` and `0043B2D0` likewise store each sum/difference component through a float-sized temporary. The two sign/scale calls remain separate because the original has an intervening binary32 store.

## Cross-file map

- [`../ghidra-original-c/00460B90.c`](../ghidra-original-c/00460B90.c): caller order, field offsets, local query layout, and the boundary arguments.
- [`axe-strike-and-bmodel-hit-audit.md`](axe-strike-and-bmodel-hit-audit.md): strike gate, query role, post-query effects, and completed `0043E2A0` leaf.
- [`axe-child-think-audit.md`](axe-child-think-audit.md): child state 1 dispatches this strike body.
- [`../../assets/training/scripts.json`](../../assets/training/scripts.json) and [`../../assets/training/entities.json`](../../assets/training/entities.json): Training equips Axe and authors the Chopping and Throwing Exercise.
- [`../../src/axe_world_collision.h`](../../src/axe_world_collision.h): `00492CE0` argument mapping; the imported `geWorld_Collision` behavior stays its own engine boundary.
- [`../../src/bmodel_candidate_iterator.h`](../../src/bmodel_candidate_iterator.h) and [`../../src/bmodel_axe_hit.h`](../../src/bmodel_axe_hit.h): later list and per-candidate processing modules.
- [`../../tools/axe_strike_pose_pcode_check.cpp`](../../tools/axe_strike_pose_pcode_check.cpp) and [`axe-strike-pcode-oracle.json`](axe-strike-pcode-oracle.json): raw-binary32 comparison against the original call arguments. Two non-axis-aligned fractional input sets exercise both `Axe+0x1FC` sign branches.

This component builds inputs only. It does not claim world collision geometry/results, the runtime candidate snapshot/order, post-collision presentation/stat behavior, recurring callback cadence, or live Training parity. The full-handler oracle stubs collision and later engine services; the pose comparison validates only the four vectors delivered to `00492CE0`.
