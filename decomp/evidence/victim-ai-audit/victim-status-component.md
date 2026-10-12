# Training victim status actions 1 and 2

`victim_status.h` ports only actions 1 and 2 of `004571A0`. It takes the existing `RescueExitState&`, so total/rescue counters stay shared with `victim_exit.h`; the component stores only status timestamps `+0xA88` and `+0x2B38`.

For action 1 the order is: enable status subobject `+0x964` (`0047DC90`), read the clock into `+0xA88`, sample list `+0xA8C` with `0.0` (`00484940`), update `+0x964` (`0047EDF0`), then look up/enable `RC_VictPick` (`004897C0`). The callback check mutates its fake clock at each preceding callback and verifies the timestamp is sampled after layer enable and before the list/update/key callbacks.

For action 2 the order is: `00455F10(+0x24C)`, disable `+0x964`, read the clock into `+0x2B38`, opaque `004054A0` message preparation, then test the existing total. The `0xffffffff` sentinel triggers the source type-2 count first, followed by statistic total (`00486920`) and membership refresh (`00484260`), matching `0044FBA0`; only then are the live `+0x3530` and `+0x352C` counters subtracted. The source signed-positive branch is preserved with 32-bit wrapping. Positive remainder 1 selects the literal singular message; larger positive values select the `%i` message and pass the remainder. Both use duration `5.0`, followed by `RC_VictDrop` lookup/enable. Zero and signed-negative remainder do not show a message or enable that key.

Only the source key strings and message text are carried over. UI helper internals, the meaning of the status subobjects, and the clock producer are left opaque. Actions other than 1 and 2 return `ignoredAction` without callbacks.

The source is in `exit-status-native/helpers.log`, fresh read-only Ghidra 12.0.4 decompilation of `004571A0`, `0044FBA0`, and `0044C360` (binary identity and query provenance are recorded in the adjacent rescue/exit component note). Standalone ordered-boundary checks:

```sh
c++ -std=c++17 -Wall -Wextra -Werror decomp/tools/victim_status_check.cpp \
  -o /private/tmp/victim_status_check
/private/tmp/victim_status_check
```

The fixture covers ordered callback traces, clock mutation boundaries, lazy type-2 counting and statistic/refresh order, singular/multiple/zero/signed-negative remainder, and the five-second display duration. Integration: include this after `victim_exit.h`, construct `TrainingVictimStatusState{sharedRescueExitState}`, and supply callbacks at the existing `004571A0` helper boundaries; call with source action 1 or 2. No scheduler or application UI is included.
