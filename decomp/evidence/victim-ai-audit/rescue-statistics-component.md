# Isolated Training rescue statistics component

`src/victim_rescue_statistics.h` models only the statistics prefix of the carried-victim branch in Player escape handler `00451160`. It increments Player `+0x352C`, adds one to `Rescues` through `00486980`, then separately loads Player `+0x5C4` followed by the global clock, subtracts with 32-bit wrap, and calls `004868C0` for `Longest Rescue Time`; it repeats the timestamp and clock loads before calling `004868C0` for `Shortest Rescue Time`. The live timestamp reads, separate clock reads, and exact source order are retained.

The two statistic updates are callback operations. `004868C0` looks up a record by name and uses its record mode at `+0x14`: mode 0 initializes the value, mode 1 replaces it when the new value is lower, and mode 2 replaces it when the new value is higher. The available evidence does not establish the configured mode for either named time record, so the component does not hard-code max/min behavior based on the labels. If the record is absent, the helper has no update effect.

## Ghidra evidence

Read-only Ghidra 12.0.4 decompilation against preserved `FDNYFirefighter.exe` in `evidence/ghidra/fdny.gpr` queried `00451160`, `00486980`, and `004868C0` using `-noanalysis -readOnly`. The caller increments `[Player+0x352C]` before the statistic calls. It passes `1` to `00486980` for `Rescues`; both `004868c0` callsites load `[Player+0x5C4]` first, then `DAT_00502afc`, then subtract timestamp from clock. The statistics helpers confirm add behavior and configured-mode comparison behavior described above. The binary identity is recorded in [`../HANDOFF.md`](../../HANDOFF.md): `FDNYFirefighter.exe` SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Validation and integration

Run from `decomp/`:

```sh
c++ -std=c++17 -Wall -Wextra -pedantic tools/victim_rescue_statistics_check.cpp -o /tmp/victim-rescue-statistics-check
/tmp/victim-rescue-statistics-check
```

The standalone check verifies counter increment/wrap, callback names and order, two distinct timestamp/clock read pairs, live pickup timestamp mutation between calls, and unsigned elapsed-time wrap. This is a native component check, not execution of the original binary. Integrate the call at the beginning of the rescue branch before victim release, using the Player counter and live pickup clock state. No scheduling or statistic-record configuration is inferred.

Lead review additionally re-ran the original `004868C0`/`00486980` queries (local `rescue-statistics-native/helpers.log`) and ported the record value/update core. Mode 0 assigns even an equal value; mode 1 accepts strictly smaller **signed int32** values; mode 2 accepts strictly larger signed values; other modes leave the word unchanged. Missing records have no effect. Addition wraps at 32 bits. Boundary tests include equal values, invalid modes, signed high-bit values and overflow. Name lookup, record configuration and the original return value's unrelated upper bits remain outside this core.
