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

The authored-fire initializer's `FireBurn1`–`FireBurn3` selection, WAV resource
definitions, and shared viewer loop-audio port are tracked in
[`firerescue_loop_audio_port_2026-10-15.md`](firerescue_loop_audio_port_2026-10-15.md).

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

### PlayerRescue candidate and eligibility fields

FireRescue's callback asks candidate vtable slot `+0x0C` for a position
buffer. Current-hash PlayerRescue vtable bytes and constructor evidence map
that slot to thunk `0049DB90`; the body routes to `0049D790`. The body
transforms local origin through root transform `+0x24` and returns XYZ at
`+0x68..+0x70`. Player's local origin is canonical translation
`+0x4C..+0x54`; its floor-hit writer refreshes the derived cache after updating
that translation. The fourth float read by the exposure callback is root
`+0x74`; the constructor chain `00453D50 -> 0043F610 -> 0049DF30 -> 0049D8C0`
initializes it to zero, while the getter and cache-copy helper write only XYZ.
The viewer now supplies zero padding. Callback cadence and later aliased writes
remain open. See
[`player_rescue_candidate_getter.md`](player_rescue_candidate_getter.md) and
the fresh export
[`player_fire_exposure_padding_fresh_2026-10-20.c`](player_fire_exposure_padding_fresh_2026-10-20.c).

The spread checks in `00448320` and `00447A90` read the length at `+4` of the
reference-counted string pointer stored at FireRescue root `+0x48`. Constructor
`0046F930` initializes it to a shared empty string. The retail initializer
`00448D60` copies UserData `A_Name` at root `+0x78+4` into this slot when the
name pointer is present. Thus the gate is exactly a nonempty-name check,
although root `+0x48` is a separate string object rather than an alias of
UserData. Dynamic children have their generated `A_Name` slot zeroed and keep
the empty string, so they do not spread recursively. The viewer applies this
gate to authored fires and generated children. See
[`fire_name_gate.md`](fire_name_gate.md) and the retail disassembly excerpt.

### Exposure callback and proximity bookkeeping

`DispatchFireRescueExposureUpdate` skips FireRescue mode 5 and callbacks after
four exposure records. It asks the candidate object's vtable slot `+0x0C` for
its position, compares the candidate and FireRescue origins at `+0x7C/+0x80/+0x84`,
and truncates the largest absolute axis difference toward zero through
`004B9400`. The direct instructions show
that max-axis metric; it is not Euclidean distance.

`UpdateFireRescueProximityAndSpread` stores that truncated distance at
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
routine returns in Player state 6; otherwise it computes a fire-relative angle
and passes it to `0044BB00`. That method sets one of four directional
burn-indicator bytes at Player `+0x353C..+0x353F`; it does not change camera or
movement yaw. The Player updater reads and clears those bytes while gating burn
cooling, and `00456090` maps them to four HUD child effects. The port now
records those sectors and applies the cooling gate, but the child effects'
screen placement and visual style remain unimplemented. The handler then
applies the `RC_Pain` sound/stat deadline on its separate clock gate and
accumulates `1.2 * frameDelta * influence` in Player `+0x3538`. A strict
extended-value comparison above 1.0 invokes the passout handler. The saved
disassembly trace preserves the current-clock addition hidden from Ghidra's
C-like output and the float-store versus extended-precision comparison.

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
at `+0x114` when present, and clears byte `+0x1C8`. A fresh field trace maps
that byte to the visual component's `+0x9C` enabled flag. On every
non-mode-5 update, the routine sets recently-sprayed byte `+0x104`.
`UpdateFireRescueActor` reads the same mode through its embedded
subobject at `+0x4C`; mode 5 returns before that callback updates fire alpha,
size/color, or effects. The visual helper also returns with the component
disabled, stopping later child updates and polygon submissions. This does not
clear child active bits or establish when already-submitted polygons disappear;
see [the visual terminal-state audit](visual-terminal-state.md).

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

The shared `FireSuppression` component captures the nonlinear intensity update
and terminal/stat/sound state changes. Its `dt` argument must be supplied from
the native frame-delta equivalent. `FireEntity::spray` now maps the cleared
`+0x1C8` visual-component flag to `FireVisual.enabled`, stopping subsequent
native child-sprite draws and animation updates. No child destruction, alpha
fade, or immediate removal of submitted geometry is synthesized. The copied
679 KiB suppression oracle is tagged with the same EXE hash; it now checks the
component gate through the entity wrapper.

Fresh exports and instruction listings for the suppression routine, message
receiver, hose contact helper, and HoseSpray update are saved beside this note.
Selected old exports and the hash-tagged old oracle/model are preserved in
[`reference/legacy-evidence/fire-spread/`](../../../reference/legacy-evidence/fire-spread/README.md).
The 2026-10-20 contact-frame audit initially left the rescue-player HoseSpray
contact producer open. The shared Player host now handles authored `touchhose`
mode-2 acquisition, slot-4 mode-3 selection, separate action callbacks, a
15-section stream, previous-frame collision contact, and terminal FireRescue
dousing. Its camera-derived nozzle frame and left-mouse action are provisional;
authored bindings, item attachment composition, resource-3 material, action
audio, cooldown effects, and particles remain open. See the
[shared-host port note](rescue_hose_spray_port_2026-10-20.md). A follow-up maps the embedded shared stream component:
HoseSpray item `+0x608` feeds `0046D1B0`/`0046C5A0`, which write the collision
point and gate at item `+0x614..+0x624` consumed by `00463EF0`. It also
connects `touchhose` to HoseSpray slot 4 and traces the distinct dedicated
`RCS_PlayerHose` contact message to `RCS_WindowFire`; its local timer/geometry
response is not FireRescue suppression. A fresh state-machine trace resolves
the authored cycle counter, spot-time win/loss path, and
`RC_HoseLvlStart/Win/Lose` outputs; the `+0x10` writer, live Construction
route, and result presentation remain open. The stage keys
appear in retail `TRadioChatter` registration, and their nonzero send path
requests a selected sound from the sound manager. See
[`hose_contact_frame_gap_2026-10-20.md`](hose_contact_frame_gap_2026-10-20.md)
and its [HoseSpray/PlayerHose export](hose_contact_and_playerhose_fresh_2026-10-20.c)
plus [PlayerHose/WindowFire export](playerhose_douse_windowfire_fresh_2026-10-20.c),
and the [hotspot lifecycle note](../scenario-loading/player_hose_stage_lifecycle_2026-10-20.md)
with its [fresh state-machine export](../scenario-loading/playerhose_hotspot_state_machine_fresh_2026-10-20.c).
The [stage message-registration export](../scenario-loading/hose_stage_message_registration_fresh_2026-10-20.c)
shows the key lookup and sound-manager request, but not the configured hose
sound name or a mission transition.

The authored rescue `touchhose N` route and per-map hashes are in
[`touchhose_authored_trigger_audit_2026-10-20.md`](touchhose_authored_trigger_audit_2026-10-20.md).
The current-hash handler trace shows that N scales the HoseSpray attachment
record count through `00482780`/`00485D20`; record purpose and argument units
remain unresolved in
[`touchhose_attachment_list_trace_2026-10-20.md`](touchhose_attachment_list_trace_2026-10-20.md).

The current-hash stream trace separates stored velocity magnitude
(`4000 * currentPressure`) from the shape scalar (`0.8 + 2.2 * currentPressure`)
and traces the 0.75-second ballistic path in
[the PlayerHose pressure and stream note](../scenario-loading/player_hose_pressure_stream_2026-10-20.md).
The interpolation excerpt is preserved in
[`playerhose_stream_shape_interpolation_fresh_2026-10-20.c`](playerhose_stream_shape_interpolation_fresh_2026-10-20.c).

### Random child placement and shape

`ProcessFireRescueSpread` first checks the name gate, current size X `>= 0.01`,
and the deadline at `+0x124` against the global game clock. Its
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

The dispatch route is now statically joined through the shared contact manager:
normal owner update `0x0042AEA0` calls manager collection `0x00429520`, which
invokes contact manager `0x0049BD20`. That manager walks active entities and,
for each category bit on the active root, calls its `+0x48` callback against
the matching type list. FireRescue's constructor marks category bit 1, and its
primary `+0x48` entry is the exposure/spread callback above; PlayerRescue roots
are type 1. This closes the static callback traversal and candidate-list
category, not the rendered-frame cadence or relative ordering with FireRescue
Think, message delivery, and RNG-consuming systems. The viewer still lacks the
shared manager lists, so it cannot claim original spread/exposure timing.
Collision/visibility semantics remain partly inside Genesis3D.

The owner pass calls the contact manager before the entity-spawn queue and the
mapped-message drain. This means a spread child created from a FireRescue
contact cannot join that same pass's contact candidates. The viewer now
snapshots the fire candidate count before it creates queued children and uses
that count for Player exposure dispatch. The narrow port correction and its
remaining timing limits are recorded in
[`fire_contact_pass_order_port_2026-10-15.md`](fire_contact_pass_order_port_2026-10-15.md).

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

## Plain-language explanation

The fire system asks the player actor for a world-space contact point. The
source getter returns the Player frame's own origin, and the shared viewer
already keeps that same point as `Player::p`. This closes the origin-alignment
question without changing the collision hull; contact timing and one separate
exposure input still need source or runtime evidence.

Fixed-path `moveto` deposits and the separate `setsize` command are tracked in
[`../rescue-moveto/README.md`](../rescue-moveto/README.md). Random nearby
spread is a distinct mechanic and must be covered too. The shared spread
population changes as each named fire crosses the 750-unit boundary; it is not
a fresh count of whichever fires appear active. In the viewer, root `+0x120`
is represented by a per-fire previous-distance field and the aggregate count
advances during the candidate pass. A previously counted fire in terminal
mode 5 stays counted until cleanup in the retail lifecycle; the viewer keeps
it resident until scene reload. See
[`proximity_count_port_2026-10-15.md`](proximity_count_port_2026-10-15.md).
