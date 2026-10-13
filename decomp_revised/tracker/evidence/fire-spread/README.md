# FireRescue exposure and random spread

## Pinned input and evidence quality

The fresh analysis is against `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, in the
project-local Ghidra program. The previous workspace had an unusually detailed
fire-mechanics audit. I copied its two explanatory documents and two relevant
old pseudocode exports into
[`reference/legacy-evidence/fire-spread/`](../../../reference/legacy-evidence/fire-spread/).
Its referenced `evidence.zip` is absent from the prior-work folder, so the old
assembly-only conclusions remain labeled as carried-over until checked again.
The callback, distance gates, random-placement geometry, child creation, timed
keys, and retry schedule below were checked again against the active hash-pinned
program; current Ghidra exports are saved beside this note.

## Current call path

```text
FireRescue vtable 0x004E1780, slot +0x48 (data at 0x004E17C8)
  -> DispatchFireRescueExposureUpdate 0x004483A0
     -> UpdateFireRescueProximityAndSpread 0x00448320
        -> ProcessFireRescueSpread 0x00447A90
```

Ghidra found the pointer at `0x004E17C8` as a data xref to `0x004483A0`.
That address was executable code but had no function boundary in the active
database; it is now defined with its observed `__thiscall` prototype and
documented. The callback calls `0x00448320` at `0x0044844C`, and the latter
calls `0x00447A90` at `0x0044838E`.

### Exposure callback and proximity bookkeeping

`DispatchFireRescueExposureUpdate` skips FireRescue mode 5 and callbacks after
four exposure records. It asks the candidate object's vtable slot `+0x0C` for
its position, compares the candidate and FireRescue origins at `+0x7C/+0x80/+0x84`,
and rounds the largest absolute axis difference. The direct instructions show
that max-axis metric; it is not Euclidean distance.

`UpdateFireRescueProximityAndSpread` stores that rounded distance at
FireRescue `+0x120`. On entry/exit across 750 units it adjusts global
`DAT_00502B90`, and on every qualifying callback whose new distance is
strictly below 750 it calls the random-spread routine. Thus spread attempts are
candidate/callback driven, with a separate retry deadline inside
`ProcessFireRescueSpread`; the routine is not a free-running per-fire timer.

The exposure callback separately gates its detailed volume calculation at 500
units. It multiplies the helper result by
`intensity + (1 - intensity) * 0.25`, attenuates by `0.1` when the
world-visibility query reports obstruction, stores the FireRescue pointer and
influence in an eight-byte record, and sends message `0x7531`.

`ComputeFireVolumeFalloff` (`0x0044A7A0`) computes the spatial term. Its
Ghidra C-like export uses a normalized `float` return signature because setting
`float10` through the current MCP inserted a false hidden return-storage
parameter and distorted the decompilation. The native instructions return the
value in x87 `ST0` with extended precision (`RET 8`); see
[`0044A7A0_falloff_assembly_excerpt.asm`](0044A7A0_falloff_assembly_excerpt.asm)
and the old export for comparison. Do not infer native return storage from the
normalized prototype. With
`r = (volume[+0x44] + volume[+0x4C]) * 0.5 + candidatePadding` and
`d² = dx² + (2*dy)² + dz²`, it returns
`scale/r * (r - sqrt(d²))` when the unpadded radius is at least 0.01 and
`d² <= r²`; otherwise it returns zero. The scale is read through
`volume[+0x60] + 0xD0`. The helper is also called at `0x00463EF0`, so this
is a shared fire-volume falloff routine rather than a callback-only helper.

The Player vtable message handler `DispatchPlayerMessage` (`0x00453A70`)
routes `0x7531` payload word 0 as the FireRescue pointer and word 1 as the
influence. It obtains the fire source point from the embedded visual subobject
at `+0x12C`, then calls `ApplyPlayerFireExposure` (`0x00451640`). That
routine returns in Player state 6; otherwise it turns toward the fire, applies
the `RC_Pain` sound/stat deadline on its separate clock gate, and accumulates
`1.2 * frameDelta * influence` in Player `+0x3538`. A strict extended-value
comparison above 1.0 invokes the passout handler. The saved disassembly trace
preserves the current-clock addition hidden from Ghidra's C-like output and the
float-store versus extended-precision comparison.

### Spray suppression and contact routes

`ApplyFireSuppressionContact` (`0x00448970`) is now checked with its full
five-word stack signature. The first three words are the contact position; the
routine does not read them. It uses the global frame delta `DAT_00502B04`,
FireRescue definition `+0x18` as the duration, and the fourth stack argument
as influence:

```text
response = 1 - (1 - influence)^2
intensity = float32(oldIntensity - (frameDelta / duration) * response)
if intensity < 0: intensity = 0
```

The final comparison is strict against 0.1. If the fifth argument `finalize` is
nonzero and the stored intensity is below that threshold, the routine changes
mode `+0x70` to 5, increments `Fires Doused`, stops the loop sound referenced
at `+0x114` when present, and clears recently-sprayed byte `+0x1C8`. On every
non-mode-5 update, it sets byte `+0x104`. This routine does not directly turn
off the fire visual or its animations; any such presentation transition belongs
to another update path and remains to be traced.

The receiver `DispatchFireRescueSprayMessage` (`0x00449A40`) was another
missing Ghidra boundary. FireRescue primary vtable slot `+0x08` at `0x004E1788`
points to it. It accepts message `0x7530` only when mode is not 5, reads a
four-float payload `[contactX, contactY, contactZ, influence]`, and calls the
shared suppression routine with `finalize=0`. The generic producer at
`0x00481A60` stores the three contact coordinates and influence in a 16-byte
record, dispatches message 30000, and advances a queue capped at 32 records.
Its three callsites are `0x0044619C`, `0x0046229C`, and `0x00464B60`; their
individual source owners still need caller-level review.

The carried HoseSpray takes a separate direct route. Its primary update entry
`UpdateHoseSpray` (`0x004640F0`, vtable slot `+0x24` at `0x004E29BC`) calls
`ApplyHoseFireContact` (`0x00463EF0`) while the spray-action byte `+0xC58` is
active. That helper scans FireRescue candidates, evaluates the shared ellipsoid
falloff at the hose contact point with item padding, requires influence at
least 0.01, then calls `ApplyFireSuppressionContact` with `finalize=1`. A later
remaining-intensity/effect-timer gate controls its hose-hit effect path. Thus
both tools reduce the same intensity through the same function, but only this
hose call requests final dousing.

The earlier C++ `FireSuppression` component already captures the nonlinear
intensity update and terminal/stat/sound state changes, so it is worth reusing.
Its `dt` argument must be supplied from the native frame-delta equivalent. Its
`FireEntity::spray` wrapper's immediate `flameEnabled`/animation shutdown is a
port-side choice, not a direct write in `0x00448970`; compare the visual update
before treating that timing as source-matched. The copied 679 KiB suppression
oracle is tagged with the same EXE hash and is preserved for a later focused
reconciliation; it was not rerun in this pass.

Fresh exports and instruction listings for the suppression routine, message
receiver, hose contact helper, and HoseSpray update are saved beside this note.
Selected old exports and the hash-tagged old oracle/model are preserved in
[`reference/legacy-evidence/fire-spread/`](../../../reference/legacy-evidence/fire-spread/README.md).

### Random child placement and shape

`ProcessFireRescueSpread` first checks an actor/definition field, current size
X `>= 0.01`, and the deadline at `+0x124` against the global game clock. Its
first uniform draw is compared with the FireRescue value at `+0x128`. A failed
roll sets `+0x124 = now + 500`; a later geometry miss does not update that
deadline in this routine.

For a successful roll, let `h = +0x128`, `U ~ uniform[0,1)`, and
`m = min(sizeX, sizeZ)`. The direct code gives:

```text
v = U * h
radius = 0.5 * m * (1 + v)
angle = uniform(0, 2*pi)
candidate = origin + (radius*cos(angle), 20, radius*sin(angle))
```

The candidate is swept downward 200 units with a box from `(-1,-1,-1)` to
`(1,1,1)`. A collision result is accepted only if the returned point is at
least 0.001 units below the candidate and its X/Z offsets are each no more than
0.1. This code checks geometry; no material or fuel classification is visible
in the path.

After a valid surface result, the factory creates an `RCS_FireRescue` child.
The child receives a three-key size track: zero at time 0, a randomized peak
after 750–3000 ms, and a collapsed shape after another 1000–6000 ms. The peak
dimensions depend on `v` and independent random factors; the final key keeps
one quarter of the peak Z component. The parent retry deadline uses the end
key time and the proximity counter. The x87 instruction trace is preserved in
[`00447A90_spread_assembly_excerpt.asm`](00447A90_spread_assembly_excerpt.asm);
the exact zero-count x87 case and cross-system RNG ordering still need runtime
parity work.

The current evidence proves the native code path and formulas, not how often
the original runtime dispatches the exposure callback. Collision/visibility
semantics remain partly inside Genesis3D.

Fresh, hash-pinned pseudocode is saved as `00447A90_fresh.c`,
`00448320_fresh.c`, `004483A0_fresh.c`, `0044A7A0_fresh.c`,
`00453A70_fresh.c`, and `00451640_fresh.c`. The two assembly traces preserve
spread placement/timing and Player exposure arithmetic.

## Reused earlier analysis

The archived `MECHANICS_legacy.md` and `SUMMARY_legacy.md` cover fire spawn,
suppression, recovery, route deposits, random growth, RNG, player exposure, and
mission-authored examples. That breadth is valuable for port planning. The
selected old `0x00447A90` and `0x00448320` pseudocode exports were compared
with current fresh output; they are equivalent after names are normalized.
Older derived claims not rechecked here must keep their historical status.

The Ghidra export of `ApplyPlayerFireExposure` hides an implicit current-clock
addition and the extended-precision threshold comparison; its assembly trace is
the authority for those details.

Fixed-path `moveto` deposits and the separate `setsize` command are tracked in
[`../rescue-moveto/README.md`](../rescue-moveto/README.md). Random nearby
spread is a distinct mechanic and must be covered too.
