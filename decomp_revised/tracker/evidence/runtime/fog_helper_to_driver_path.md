# Authored fog helper to render-driver path

Updated 2026-10-13. Evidence is pinned to the retail executable SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` at
`inputs/original/binaries/FDNYFirefighter.exe`. A Ghidra instance was not
running during this follow-up; the persisted Ghidra decompilations cited below
were checked against raw x86 from the pinned executable with `objdump`.

## State path

- Shared ScriptActor dispatch at `0047A1F0` recognizes `fogenable` and
  `fogdisable`. The command's five integer values reach `004789D0`; disabling
  reaches `00478AC0`. See
  [`0047a1f0_script_command_dispatch.c`](../scenario-loading/0047a1f0_script_command_dispatch.c).
- `004789D0` resolves an object whose type field at `+0x18` is 5, converts the
  five command integers to floats, and calls `0049C960`. The first setter
  parameter is not stored by `0049C960`; the setter fixes target `+0x114` to
  `1.0`, stores its remaining four parameters at `+0x118..+0x124`, snapshots
  current values from `+0xEC..+0xFC`, records a 2,000-unit interval in
  `+0xE4/+0xE8`, clears interpolation byte `+0xE1`, and sets active byte
  `+0xE0`.
- `00478AC0` resolves the same object type and calls `0049CA10`. That setter
  stores `1990.0` and `2000.0` at `+0x114/+0x118`, copies the remaining three
  target values from current state, resets the interval and interpolation
  byte, and clears active byte `+0xE0`.
- `0049CF10` consumes this state. While `+0xE1` is clear, it interpolates the
  five current values at `+0xEC..+0xFC` between the saved tuple and target
  tuple. At the interval end it copies the target tuple. It also updates a
  camera far-clip value from current `+0xF0` plus a constant.
- At `0049D109..0049D116`, `0049CF10` calls the Genesis import through
  `0x004DC1E8` (`geEngine_SetFogEnable`). The call receives the active byte,
  three independently generated channel values masked with `0xF0`, and the
  current values at `+0xEC` and `+0xF0`. Thus the retail fog state reaches the
  selected render driver; the last three interpolated values are not passed
  directly as the call's channel arguments.
- Raw caller bytes at `00464A07..00464A10` call `0049CF10` on `ESI`, then call
  `0049CD80` on the same object. This places the consumer in an update
  sequence, but does not establish its cadence or clock.

The persisted Ghidra pseudocode for `0049C960`, `0049CA10`, and `0049CF10` is
under `reference/previous-work/decomp/evidence/startup-factories/`. Raw
disassembly can be reproduced with:

```sh
objdump -d --start-address=0x004789d0 --stop-address=0x00478b40 inputs/original/binaries/FDNYFirefighter.exe
objdump -d --start-address=0x0049c960 --stop-address=0x0049d130 inputs/original/binaries/FDNYFirefighter.exe
objdump -d --start-address=0x00464900 --stop-address=0x00464a20 inputs/original/binaries/FDNYFirefighter.exe
```

## Port implications and limits

The viewer currently has no authored fog state or fog rendering. The trace
supports routing `fogenable`/`fogdisable` through shared runtime state and
applying it during rendering. It does not yet identify the five authored
values' semantic names, Genesis/D3D driver range conventions, the visual result
on the shipped driver, update cadence, or interpolation edge timing. Those
remain provisional; do not assign guessed OpenGL fog meanings to the values.

Fog is a separate renderer feature from Training's red trigger brushes. The
brushes are authored `col_red` BSP models; absence of fog can affect scene
distance and appearance, but the fog trace does not explain why those discrete
trigger volumes were drawn as solid red geometry.
