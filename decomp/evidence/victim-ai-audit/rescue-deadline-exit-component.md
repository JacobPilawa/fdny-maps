# Rescue deadline, membership and exit bookkeeping

Read-only Ghidra 12.0.4 queries on the preserved EXE verified `004504C0`, `0044FBA0`, `004541A0`, `004571A0`, `0044C360`, `00480D40`, original assembly of the escape caller and deadline helper, and scalar data. Raw logs stay local in `exit-status-native/`. Exact EXE identity is recorded in HANDOFF.md and the numeric oracle fixture.

## Previously unported post-rescue RNG

Escape caller `00451160` calls victim release, status action 2, then thunk `00454300→004541A0` with `Player+0x340C`. Its target deadline word is therefore Player `+0x3420`. This call runs before the carry pointer is cleared and before entity-mode reset/exit checks.

The deadline helper reads Player field `+0x343C` through the global Player singleton. It clamps that binary32 value to `[180,7200]`, multiplies by binary32 1000 and a mode-selected binary64 factor `(0.6,0.75,1)` for modes 0, 1, and other, then applies original truncating integer conversion `004B9400`. If unsigned current clock is below that threshold, it stores `threshold + bounded(30000)`. Otherwise its baseline is current clock plus binary32 60000 times a mode-selected binary64 factor `(1,2,1000)`; it stores that baseline before drawing `bounded(60000)`, then adds the draw with 32-bit wrap. The literal 1000 factor on the latter generic branch is preserved from assembly; no gameplay interpretation is invented for it.

`player_rescue_deadline.h` preserves x87 intermediate precision through `Extended`, low-word conversion/wrap, strict unsigned threshold comparison, store order and one shared draw. The misleading decompiler export omitted the clamping, mode table and arithmetic entirely. The original P-code oracle executes the complete helper and original seeded RNG without replacing calls; supplied singleton/app/Player memory selects the inputs. **48 original calls match native deadline and draw count**, covering clamp endpoints, mode branches, threshold boundaries and high-bit clocks. Finite authored inputs are covered; general NaN/infinity behavior and the source producer of field `+0x343C` remain open. No new consumer is compensated with discarded draws.

## Membership and exit component

Luna supplied `zone_membership.h`, the source `00484260` rebuild: clear all zones, iterate global-list order, include type 2 except states 5/6, use the source position adapter, append to the smallest containing named zone, report unzoned actors and continue. Lead review switched its standalone harness from a fake arithmetic helper to the actual `Extended` implementation.

`victim_exit.h` ports local `004504C0` bookkeeping and lazy `0044FBA0` count. It refreshes memberships first; an unknown total triggers a type-2 count, statistic-total callback and another refresh. Original instructions `0045051C/1E/20/22` use two 32-bit SUBs, TEST and JG: a positive signed wrapped remainder returns. Otherwise final-exit candidates are collected in source entity order, mapped by bounds center, and zones deduplicated by identity. Unzoned candidates report but remain in the trigger-enable list. With no zone, Player `+0x3438` is set and `0044C360` is called. With zones, source prompt punctuation/order and `+0x5CC=0` are preserved before enabling each collected trigger.

Binding classification (`0046ED70`) and bounds centers remain supplied source inputs, as do item-selection/status callbacks. Training entities contain `touch: tell player escape`, with no authored finalexit/finalexitloadmap binding, so the empty candidate path is relevant. Setting `+0x3438` and calling `0044C360` does not by itself prove an implemented complete mission/outro.

Twelve CTest checks pass. New exit checks cover lazy count, wrapped gate, candidate order/dedup, unzoned diagnostic, source prompt and Training's empty-candidate path. Component integration still needs a shared runtime state, status helpers and original recurring actor/tool scheduling. The parallel five-candidate scheduling follow-up ruled out parser/container receivers but found no dispatcher; its limits are in `scriptactor-common-scheduler-audit/remaining-candidates-followup.md`.
