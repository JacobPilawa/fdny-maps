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

## 2026-10-13 — FireRescue random spread and player exposure traced

Reused the prior fire-spread audit as a roadmap, preserved its selected exports
and documentation under `reference/legacy-evidence/fire-spread/`, then checked
the active hash-pinned EXE in Ghidra. Two executable addresses had no function
boundaries: the FireRescue exposure callback `0x004483A0` (recovered from the
FireRescue vtable) and Player message handler `0x00453A70` (recovered from the
Player vtable). Both boundaries and their observed prototypes are now in the
Ghidra database. Fresh evidence follows `0x004483A0 -> 0x00448320 ->
0x00447A90` for callback-driven proximity/random spread, and the `0x7531`
message from exposure-volume evaluation through `0x00453A70` to
`0x00451640` for Player burn accumulation and passout. The placement geometry,
retry/key timing, spatial falloff, attenuation, and exposure threshold are
recorded in `tracker/evidence/fire-spread/`; runtime callback cadence, global
RNG ordering, and Genesis collision/visibility details remain open.

This adds six function reviews and recovers two previously missing function
boundaries, bringing the EXE review ledger to 99 and the main-EXE defined
function count to 5,810. Earlier fire-spread findings were reused selectively;
the missing legacy `evidence.zip` was not recreated or treated as evidence. The
`ComputeFireVolumeFalloff` MCP prototype was normalized to a C `float` for a
readable export after `float10` caused a false hidden return-storage argument;
the raw x87 `ST0` return is preserved in its assembly trace and explicitly
documented. No HTML dashboard changes were made.

## 2026-10-13 — Fire suppression routes checked against reusable C++

Reused the prior suppression model and selected old exports instead of
reconstructing its work. Fresh Ghidra analysis resolved `0x00448970`'s full
stack ABI: contact xyz are present but unused, while influence and finalize
control a nonlinear intensity reduction using global frame delta
`DAT_00502B04`. The carried HoseSpray passes `finalize=1`; the `0x7530`
FireRescue message receiver at `0x00449A40` passes `finalize=0`. The same
strength transition serves both tools, while only the direct hose path can
mark a fire fully doused. The message receiver was missing a function boundary
and is recovered from vtable slot `+0x08`.

Recovered the `UpdateHoseSpray` boundary at vtable slot `+0x24`
(`0x004640F0`) and labeled its direct fire-contact helper
`ApplyHoseFireContact` (`0x00463EF0`). The generic contact producer
`AppendAndSendSprayContact` (`0x00481A60`) stores xyz plus influence in a
bounded 16-byte record and dispatches message `0x7530`; its three callsite
owners remain under review. Fresh C output and disassembly are saved under
`tracker/evidence/fire-spread/`.

The old C++ suppression state transition is a useful reuse candidate. Its
caller-provided `dt` must match the native global frame delta, and its immediate
visual shutdown is a port-side behavior not written by `0x00448970`; the visual
update path still needs comparison. The old same-hash oracle and selected
address-matched exports are now preserved under
`reference/legacy-evidence/fire-spread/`. No oracle or tests were run.

This adds five function reviews and recovers two missing boundaries. The review
ledger now contains 104 functions; the main EXE has 5,812 defined function
boundaries. Training remains in scope alongside all seven rescue missions; the
shared suppression path applies across scenarios, while mission-specific data
and runtime parity remain to be mapped. The HTML dashboard was not changed.

## 2026-10-13 — FireRescue terminal visual update gate

Fresh Ghidra tracing linked suppression state `+0x70` to the embedded actor callback state at `+0x4C`. State 5 branches directly to the callback epilogue before its alpha/effect update path. Named and saved the alpha, sprite-disable, and visual-effects helpers; added the address-specific audit and corrected the `+0x1C8` field description. The exact render-side disappearance/fade and callback ordering remain open. No native gameplay behavior was changed and no dashboard HTML was edited.

## 2026-10-13 — Gas R script state and queue path mapped

Fresh Ghidra traces now link the Gas R EVS's ordered set statements to the
shared variable table, show *firespread expanded while the later explosion2
command is parsed, and map the GasIsOn equality gate and actor-context once
guard. The common que command now traces to the ScriptActor queue array
insertion helper. The queue consumer/cadence, mission-specific BoundsTrigger
bindings, and live event/parity capture remain open. The new evidence, fresh
C/assembly exports, and review ledger rows are stored under
tracker/evidence/scenario-loading/ and tracker/.

## 2026-10-13 — Gas R ScriptActor queue and wait scheduler mapped

Fresh Ghidra work traced queue insertion through head removal and stable
compaction, then through the one-command pump in the ScriptActor update gate.
The common wait command parses a float duration, scales seconds by 1000, adds
the current game clock, and stores a deadline. The updater pauses that actor's
queue until expiry and then can process one command. BSP entities 265, 280,
and 309 now join the shared explosion1, explosion2, and gasoff scripts to
their authored BoundsTrigger/BModel properties. External updater cadence,
actual contact/use firing, and visual timing remain runtime gaps. No dashboard
HTML or gameplay implementation was changed.

## 2026-10-13 — Gas R button1 use event mapped

Reused the archived Training BModel selection audit and refreshed the rescue
interaction seam in the active Ghidra program. Entity 309's `use` property
selects generic BModel use mode 2 because `G_Liftable=0`; its interaction
vtable exposes the axis-box candidate query and dispatches the `use` event
without entering the mode-1 carry path. Fresh decompilation and assembly now
cover the BModel action/query handlers, event-presence lookup, event command
iteration, and nested `cmd` dispatch. Joined the authored `gasoff` macro to
its six FireRescue size commands and `GasIsOn=0`. Live input selection, update
cadence, and effect parity remain open. The Ghidra program was saved; no
dashboard HTML or native gameplay code changed.

## 2026-10-13 — Gas R BoundsTrigger contact dispatch mapped

Fresh Ghidra pseudocode and assembly now cover the BoundsTrigger candidate
overlap test (`00440CE0`), the embedded ScriptActor contact updater
(`00440DC0`), and shared contact conversion (`00470840`). The authored Gas R
triggers 265 and 280 join to the shared pair-list route, which maps new,
persistent, and removed pairs to `touch`, `touching`, and `untouch`. The
per-instance manager registration, dynamic updater caller/order, and runtime
cadence remain open. While reconciling the reused BModel selection work, the
`0043DB40` prototype was corrected: the shared Player scan at `00459350`
passes one query-interface output slot, so the prior extra stack parameter was
removed. Updated fresh pseudocode and Ghidra comments were saved. No dashboard
HTML or native gameplay code changed.

## 2026-10-13 — Shared rescue contact manager mapped

Reused the archived Training contact and selection audits, then defined and
saved current Ghidra boundaries for CheckBModelContactCandidate
(0043B620), RegisterContactManagerEntity (0049BC90), and
DispatchEntityContactLists (0049BD20). The fresh manager pseudocode maps
entity category registration, inactive-member removal, nine type-list walks,
and root slot +0x48 callbacks. This joins Gas R button1 to the shared
type-1 candidate path and its BModel contact updater; entity 309's authored
reach is 20 with hit points 2 and G_Liftable=0, while the embedded motion
endpoint needed to settle the conditional 48-unit override remains undecoded.
The common BoundsTrigger path is documented alongside it. Exact Gas R
instance membership, updater order/cadence, and live effects remain open. No
dashboard HTML or native gameplay code changed.
