# PlayerHose pressure alias and stream sample path (2026-10-20)

Retail `FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Fresh Ghidra 12.0.4 headless decompilation of the current saved program is
preserved in
[`playerhose_pressure_stream_fresh_2026-10-20.c`](../fire-spread/playerhose_pressure_stream_fresh_2026-10-20.c).
This is static analysis, not a live hose capture or a retail-parity result.

## Field alias and update

`00466D60` passes PlayerHose `+0x1480` to the stream-component setup path.
`00466780` later calls `0046D1B0` with that same embedded component pointer
(`param_1 + 0x520`, or byte offset `+0x1480`). Hotspot selection in
`004677D0` copies the authored path node's `M_MaxPressure` value at node
`+0x2C` into PlayerHose `+0x1834`. Since the component starts at `+0x1480`,
its target field at component `+0x3B4` aliases that owner field. The component
current scalar at `+0x3B0` aliases PlayerHose `+0x1830`.

`0046D1B0` updates the scalar on each component call. With `MOUSE_LB` active
and the component's stream gate open, it moves the current value 20% of the
remaining difference toward the target:

```text
current += (M_MaxPressure - current) * 0.2
```

Without active input, it moves 20% toward zero. This interpolation is per
component update and has no frame-delta multiplier in the traced expression.
The `0.2` is the binary32 conversion of the double constant at `004E27D8`.
The source does not clamp the scalar. Its update cadence and the component's
runtime gate are not measured here.

## Consumers and gameplay scope

`0046D1B0` passes the scalar to `0046C000` as the final field of each
time-stamped stream sample (record float `+0x1C`); the sample also stores a
timestamp and two XYZ vectors. The sample writer is confirmed, but the retail
water renderer and the visual meaning of that fourth field remain unresolved.
`00464AA0` also passes the current scalar as the intensity payload through
`00481A60` to the WindowFire message. The traced WindowFire `0x7530` handler
does not use that payload: its timer change is `frameDelta / SprayTime` while
its linked active/selected gates are set. Thus the known pressure path controls
the stream sample input and message payload, but it does not scale the traced
WindowFire completion timer.

The authored pressure values in the current hose BSP exports are `1` for
`city_h`, `dock_h`, and `oil_rig_h`; `const_h` uses `.35`, `.4`, `.45`, `.5`,
`.75`, and `1`. These are data values, not a verified physical unit range.
The four hash-pinned BSP exports are listed in
[`player_hose_curve_pipeline_2026-10-20.md`](player_hose_curve_pipeline_2026-10-20.md).

## Shared-host port

`PlayerHoseStage` now tracks the source-shaped current scalar and active target.
Entering a hotspot sets its target from that node's `M_MaxPressure`; the host
updates once per frame toward the target while its hose action is active and
toward zero otherwise, using the source's per-update 0.2 interpolation. The
host feeds that value to its existing simple blue line's opacity, clamped to
`[0,1]` only at the renderer boundary. The line-alpha mapping is a temporary
visual proxy; no source supports this specific color/alpha transfer. Contact
completion still follows held input and matching target model, consistent with
the traced WindowFire timer's ignored intensity payload.

Remaining work: recover the component state gate and scheduler cadence, trace
who consumes the stream sample records and how the retail water visual uses
their pressure field, then replace the host alpha proxy. No runtime or retail
comparison was performed for this checkpoint.
