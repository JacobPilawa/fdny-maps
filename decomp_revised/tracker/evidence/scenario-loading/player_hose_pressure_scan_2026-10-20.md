# PlayerHose `M_MaxPressure` operand scan (2026-10-20)

Retail `FDNYFirefighter.exe` was checked in the current Ghidra project and
independently hashed at
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

Ghidra 12.0.4 headless ran the project's `SearchPlayerOffsets.java` against
`FDNYFirefighter.exe` with the exact operand `1834`. The instruction-text scan
returned one match:

```text
00467b8a 004677d0 FUN_004677d0 :: MOV dword ptr [ESI + 0x1834],EAX
```

This is `004677D0` copying the selected HosePath record's `M_MaxPressure`
value into PlayerHose `+0x1834`. The scan found no direct instruction-text
read of that offset. The earlier local scan used `0x194` and was unrelated;
it is not absence evidence for the pressure field. The new result still cannot
exclude computed offsets, helper-mediated accesses, or aliases through another
pointer, so the downstream gameplay effect is unresolved. `M_MaxPressure`
must not be equated with stream-owner intensity at `+0x1830` without further
evidence.

The command used the tracked `tools/ghidra-scripts/SearchPlayerOffsets.java`
against `analysis/ghidra/FDNYPort`; its output and run logs remain in ignored
`.local/ghidra/` data and are not included in the mirror.
