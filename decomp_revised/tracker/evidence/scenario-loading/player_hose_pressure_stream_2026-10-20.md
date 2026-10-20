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
timestamp and two XYZ vectors. The fresh sample consumer is
[`playerhose_stream_component_consumer_fresh_2026-10-20.c`](../fire-spread/playerhose_stream_component_consumer_fresh_2026-10-20.c).
The interpolation and curve-array helpers are preserved in
[`playerhose_stream_component_helpers_fresh_2026-10-20.c`](../fire-spread/playerhose_stream_component_helpers_fresh_2026-10-20.c).
`0046BC10` interpolates the sample and returns the pressure field. The
`0046C5A0` consumer passes twice that scalar to `00469F50` while appending
stream-curve points, filters low-pressure segments, builds four-vertex strips,
and performs world collision along the generated path. This resolves the
native sample-consumer path and confirms pressure changes stream-geometry input;
the exact vertex-to-pressure rendering transfer and live appearance remain
open.

The same consumer sets component `+0x18` after a world hit using current
pressure at `+0x3B0` and constructor values `+0x38C = 10`, `+0x3AC = 40`:

```text
contactRadius = 40 * currentPressure + (1 - currentPressure) * 10
```

`00464AA0` compares each eligible WindowFire target's world AABB against the
impact point expanded by this radius on all three axes, then queues the impact
point and current pressure as message `30000`. Pressure therefore affects
whether a target receives contact as well as the geometry input. The
`0x7530` WindowFire handler does not use the message payload to scale its timer:
while linked active/selected gates are set, its timer change remains
`frameDelta / SprayTime`.

The authored pressure values in the current hose BSP exports are `1` for
`city_h`, `dock_h`, and `oil_rig_h`; `const_h` uses `.35`, `.4`, `.45`, `.5`,
`.75`, and `1`. These are data values, not a verified physical unit range.
The four hash-pinned BSP exports are listed in
[`player_hose_curve_pipeline_2026-10-20.md`](player_hose_curve_pipeline_2026-10-20.md).

## Shared-host port

`PlayerHoseStage` tracks the source-shaped current scalar and active target.
Entering a hotspot sets its target from that node's `M_MaxPressure`; the host
updates once per frame toward the target while its hose action is active and
toward zero otherwise, using the source's per-update 0.2 interpolation. Its
contact radius now follows the component's 10-to-40 interpolation, and the
selected target's exported world AABB is tested against that expanded impact
point before the WindowFire timer can advance. The simple blue line still uses
pressure as a provisional opacity proxy; the source exposes a pressure-scaled
stream-geometry input, but the renderer's exact appearance has not been
recovered.

Remaining work: recover the component state gate and scheduler cadence, trace
the remaining stream-geometry helpers and render-state transfer, and compare
the host contact volume against controlled samples. No runtime or retail
comparison was performed for this checkpoint.
