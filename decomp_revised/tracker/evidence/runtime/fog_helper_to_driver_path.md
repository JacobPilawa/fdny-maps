# Authored fog helper to render-driver path

Updated 2026-10-15. Evidence is pinned to the retail executable SHA-256
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

## Plain-language explanation

Mission scripts change fog over two seconds. Enabling fog targets a start
distance of 1, the authored end distance, and the authored RGB values.
Disabling fog moves the distances to 1990 and 2000 while preserving color, then
turns the driver setting off at the end of that transition. The camera far clip
tracks the current end distance plus 20. Before calling the renderer, retail
truncates each interpolated color channel and clears its lower four bits, so a
script value of 50 reaches the driver as 48.

## Selected D3D driver behavior

The pinned `D3DDrv.dll` SHA-256 is
`7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7`.
Its table slot `+0xA8` points to `0x10003690`, which stores the incoming
start/end distances and three color channels. The callback at `0x1000800f`
sets render state 28 from the enable flag and render state 34 from the packed
color. Microsoft's [Direct3D render-state enumeration](https://learn.microsoft.com/en-us/windows/win32/direct3d9/d3drenderstatetype)
names these values `D3DRS_FOGENABLE` and `D3DRS_FOGCOLOR`.

The vertex preparation path at `0x10007910` uses the stored start/end and a
transformed vertex depth to write fog alpha. It follows a linear ramp from
fully visible at the start distance to fully fogged at the end distance, with
the result bounded to 0–255. The selected driver receives channels in RGB
order. The current end field is `+0xF0`, and the far-clip update adds the
`20.0` float at `0x004E1604`.

This matches Microsoft's documented [linear vertex fog](https://learn.microsoft.com/en-us/windows/win32/direct3d9/vertex-fog),
whose start/end distances are in camera space. The OpenGL implementation uses the compatibility
pipeline's linear fog for camera-space depth. It remains a renderer-port
approximation because no retail frame has been compared pixel-for-pixel.

## Port implications and limits

The shared scene viewer routes `fogenable` and `fogdisable` through a
five-value fog state, advances its two-second interpolation, applies retail's
RGB quantization during world rendering, and sets camera far clipping to
current end plus 20. It resolves the authored `*DefaultFogParams` operand.
Packaged rescue checks assert the startup fog targets for Bank, Boat A, and
Boat B. Gas R's trigger-owned inside/outside commands use the same host state.

The port starts its current tuple at zero because the retail object's
constructor defaults have not yet been traced. Exact frame scheduling, the
initial tuple, and pixel-level output parity remain open. When a transition
passes through equal start/end values, the OpenGL renderer adjusts end by
0.001 units to avoid a degenerate fog ramp.

Fog is a separate renderer feature from Training's red trigger brushes. The
brushes are authored `col_red` BSP models; absence of fog can affect scene
distance and appearance, but the fog trace does not explain why those discrete
trigger volumes were drawn as solid red geometry.
