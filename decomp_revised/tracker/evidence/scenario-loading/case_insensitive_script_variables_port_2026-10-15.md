# Case-insensitive EVS variables in the shared interpreter

## Plain-language explanation

Retail treats global EVS variable names without regard to letter case. For
example, `bonusitems.evs` defines `BONUS60`, while BSP script fields reference
`*bonus60`. The shared interpreter used to treat those as different names,
so it could not expand the authored bonus pickup commands. The variable table
and `if` value comparisons now follow the case-insensitive behavior found in
the retail executable.

## Retail evidence

The static trace uses `inputs/original/binaries/FDNYFirefighter.exe`,
SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

- `StoreScriptVariableValue` (`00470FC0`) writes `set` values to the shared
  table at `DAT_00503044` through `FindScriptVariableEntry`.
- `ExpandScriptVariableReferences` (`004730D0`) extracts `*name` references
  and looks them up through `FindScriptVariableEntry` (`0047CF70`). That
  lookup uses the table comparator selected by `FUN_0047C990`; inspection of
  the hash-pinned x86 path shows the shared table's case-insensitive mode.
- `ExecuteScriptIfValueMatches` (`004758F0`) compares the stored value and
  expected value with `__stricmp`.
- The saved fresh decompilations are
  [`00470fc0_script_variable_store_fresh.c`](00470fc0_script_variable_store_fresh.c),
  [`004730d0_expand_script_variables_fresh.c`](004730d0_expand_script_variables_fresh.c),
  [`0047cf70_script_variable_lookup_fresh.c`](0047cf70_script_variable_lookup_fresh.c),
  and [`004758f0_script_if_equals_fresh.c`](004758f0_script_if_equals_fresh.c).
  The command and authored-property path is also summarized in
  [`gas_r_script_variable_queue_path.md`](gas_r_script_variable_queue_path.md).

## Port and verification

`CaseInsensitiveVariableLess` in
[`scripts.h`](../../../port/spikes/training-native-prototype/src/scripts.h)
makes global variable set, lookup, count, and indexing case-insensitive. The
`if` handler uses the same comparison rule for values. This keeps the EVS
variable spelling available for diagnostics while resolving names such as
`BONUS60` and `bonus60` to the same entry.

The rebuilt rescue-all app passed `--rescue-check` on all 13 bundled
Training/rescue/stage packages. No package reported an unresolved `*bonusXX`
reference. The app's `--mission-catalog-check` loaded all eight retail rescue
entries. The CMake and bundled app executables match at SHA-256
`23dba30c0f82de337966de8b6c3394bd7534fddfa1267b8ec6044796863e9c60`.

These are controlled interpreter and package checks. They do not verify live
bonus-item contact, pickup rewards, or full retail script timing. Other
unsupported EVS commands remain, including fog/light-table directives and
some map-specific movement or sound commands.
