# Rescue HoseSpray `touchhose N` attachment-list trace (2026-10-20)

Retail binary: `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
This is a static Ghidra/disassembly trace; it does not establish runtime
callback cadence or compare against a live retail run.

## Player command

`0044CAB0` receives the numeric `touchhose` argument as an integer. It reads
HoseSpray from Player inventory slot 4 (`Player+0x3564`, slot offset `+0x10`).
If the slot is empty, it allocates `0xD54` bytes, runs constructor `004634F0`,
attaches the item through vtable slot `+0x08`, stores it in slot 4, and calls
`00482780(item+0xC70, float(N))`. The existing-item branch also calls
`00482780(item+0xC70, float(N))` when its refresh condition is met. Both paths
then request HoseSpray mode 2 and the hose sound. This is a dedicated command;
the ordinary `equip` name selector continues to recognize only axe,
extinguisher, and goggles.

For an existing item, `+0xC68` values 2 and 3 take the “Already have hose”
branch. Otherwise `0044CAB0` compares
`(item+0xCB0) * (item+0xCC0) * (1/24)` with N, and rebuilds the list when that
product is below N. Here `+0xCC0` is the record count.

## Attachment-state helper

`00482780` receives the HoseSpray embedded state at item `+0xC70`. Its
arithmetic computes `N * 24 / state[+0x40]`; relative to HoseSpray this
denominator is item `+0xCB0`. It passes the converted integer to
`00485D20(state, count)`. The conversion helper `004B9400` is already
reviewed in
[`004b9400_convert_x87_float_to_integer_toward_zero_fresh.c`](../scenario-loading/004b9400_convert_x87_float_to_integer_toward_zero_fresh.c)
and converts toward zero. `00485D20` frees and rebuilds the intrusive list
rooted at state `+0x48`, adding the requested number of `0x30`-byte records.
The list’s count is also read at HoseSpray owner `+0xCC0` by
`UpdateHoseSpray` (`004640F0`).

The data constant at `004E1858` is binary64 `1/24`; the raw bytes were checked
against the pinned executable. This scale is consistent with a 24-unit
attachment-list count calculation. It does not, by itself, give N a time or
distance unit.

## What remains open

The per-record runtime use, whether owner `+0xCB0` changes after its default
20.0 setup, and why authors selected 15, 20, 25, 30, or 50 for particular
triggers remain unresolved. Do not map N to physical stream reach, mode, or
seconds without another source path or live measurement. The authored
map list and event route are recorded in
[`touchhose_authored_trigger_audit_2026-10-20.md`](touchhose_authored_trigger_audit_2026-10-20.md).
