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

`0046D1B0` derives two separate values from current pressure before writing
the primary sample history at component `+0x168`. Constructor values
`+0x370 = 0` and `+0x390 = 4000` scale the aim direction to a stored velocity
vector of magnitude `4000 * currentPressure`; they do not specify the final
travel distance. Values `+0x374 = 0.8` and `+0x394 = 3` make the sample's final scalar
`0.8 + 2.2 * currentPressure`. `0046C000` stores the source point, scaled
direction vector, and this separate shape scalar in each timestamped sample
(record float `+0x1C`). The fresh sample consumer is
[`playerhose_stream_component_consumer_fresh_2026-10-20.c`](../fire-spread/playerhose_stream_component_consumer_fresh_2026-10-20.c).
The interpolation and sample-buffer helpers are preserved in
[`playerhose_stream_component_helpers_fresh_2026-10-20.c`](../fire-spread/playerhose_stream_component_helpers_fresh_2026-10-20.c).
`0046BC10` interpolates both vectors and the shape scalar. In `0046C5A0`,
segments whose interpolated scalar is at most `0.8 * 1.25 = 1.0` are cleared;
retained segments pass twice the shape scalar (from `1.6` to `6.0` for pressure
from zero to one) to `00469F50`, update four-vertex strips, and run world
collision along the generated path. This separates pressure-scaled velocity from
the strip's scalar input. The exact transfer from the stored scalar to vertex
width, color, or texture and the live appearance remain unresolved.

The same consumer sets component `+0x18` after a world hit using current
pressure at `+0x3B0` and constructor values `+0x38C = 10`, `+0x3AC = 40`:

```text
contactRadius = 40 * currentPressure + (1 - currentPressure) * 10
```

`00464AA0` compares each eligible WindowFire target's world AABB against the
impact point expanded by this radius on all three axes, then queues the impact
point and current pressure as message `30000`. Pressure therefore affects
whether a target receives contact as well as stream reach and shape input. The
`0x7530` WindowFire handler does not use the message payload to scale its timer:
while linked active/selected gates are set, its timer change remains
`frameDelta / SprayTime`.

The authored pressure values in the current hose BSP exports are `1` for
`city_h`, `dock_h`, and `oil_rig_h`; `const_h` uses `.35`, `.4`, `.45`, `.5`,
`.75`, and `1`. These are data values, not a verified physical unit range.
The four hash-pinned BSP exports are listed in
[`player_hose_curve_pipeline_2026-10-20.md`](player_hose_curve_pipeline_2026-10-20.md).

## Sampled ballistic path

`0046C2F0` constructs the history with a `0.75` second lifetime and `0.1`
second minimum interval, allocating eight 32-byte records. `0046C000` primes
the empty ring with repeated initial samples, then adds a sample after each
0.1 second interval. This time base is consistent with the separately traced
WinMM clock converted from milliseconds by `0.001`.

`0046D6B0` creates 25 strip sections and stores each history parameter as
`0.5*u + 0.5*u*u`, for `u = section / 24`. `0046BC10` looks up the source,
velocity and shape at `now - age`, where `age = parameter * 0.75`. For the main
path it evaluates `source + velocity*age + (0,-1000,0)*age*age`; the
`-1000` coefficient comes from half of the constructor's `(0,-2000,0)` vector.
The other strip curve evaluates
`source + (velocity + (0,-500,0))*(0.6*age) + (0,-1000,0)*(0.6*age)^2`.
`0046C5A0` submits each quad in previous-main, previous-secondary,
current-secondary, current-main order and collision-tests consecutive main
path samples. It clears later strip polygons after a hit. At full pressure and
a normalized horizontal aim, the main path's nominal forward travel at age
`0.75` is 3000 units, with about 562.5 units of downward displacement.

The strip vertices are initialized white (`255` per channel). Constructor
`0046D6B0` requests renderer resource ID 3 through `00493D00` → `00493650`.
The current `hose.rrf` maps ID 3 to `hdstream.bmp` and `hdstream_a.bmp`; ID 5
is the separate `hosewater.bmp`. `0046C5A0` scrolls U by `-delta*6` and writes
the four corner UVs, in strip order, as `(u,0)`, `(u+.15,0)`, `(u,.9)`, and
`(u+.15,.9)`. Pair hashes, asset dimensions, and the host binding are recorded
in [`playerhose_stream_texture_binding_2026-10-20.md`](playerhose_stream_texture_binding_2026-10-20.md).
The alpha bitmap is twice the color bitmap's width, so host normalized
resampling, filter/wrap behavior, AddPoly flag `0x20`, blend state, and live
appearance remain provisional. `00469F50` appends a supplied curve position
and scalar into parallel arrays; its caller supplies twice the interpolated
sample shape scalar after the threshold check.

## Shared-host port

`PlayerHoseStage` tracks the source-shaped current scalar and active target.
Entering a hotspot sets its target from that node's `M_MaxPressure`; the host
updates once per frame toward the target while its hose action is active and
toward zero otherwise, using the source's per-update 0.2 interpolation. Its
stream path keeps eight timestamped source/velocity/shape samples, updates
them at 0.1 second intervals, and evaluates the source-shaped 25-section
dual-curve ballistic strip over the 0.75 second history. The host draws each
quad in the traced vertex order and collision-tests its main path, stopping
after the first hit. Dedicated PlayerHose now loads the source-bound resource-3
color/alpha pair and applies the traced U scroll and UV corners. Alpha
resampling, filtering, wrapping, AddPoly flag `0x20`, blending, cadence, and
visual parity remain provisional. Rescue PlayerRescue HoseSpray stays on its
separate preview renderer. The shared host checks the selected target AABB against the pressure-derived
10-to-40 contact padding. Source sample and path math are static evidence; no
interactive or retail comparison was performed for this checkpoint.
