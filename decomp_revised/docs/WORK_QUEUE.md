# Work queue and current handoff

Updated 2026-10-20 UTC. This file records the next decompilation and port tasks so
research can continue without redoing the old project review or recent audits.
The documented compare-first mirror workflow has published prior reviewed
checkpoints to `decomp_revised/` after explicit user authorization. Confirm the
current remote before each checkpoint; preserve `decomp/` and exclude retail
binaries and local Ghidra/build data. The source workspace has no configured
Git remote.

## Dedicated PlayerHose host slice (2026-10-20)

The shared `fdny-scene-viewer` host now recognizes a dedicated PlayerHose map,
resolves its `RCS_HosePath` links and all non-null `ZF0..ZF8_FireModel` names
to BSP model IDs, and runs a separate WindowFire target/cycle/hotspot loop.
The held stream is gated by the active target's collision model. The existing
rescue mission catalog stays eight entries; the default macOS bundle now
includes `city_h`, `const_h`, `dock_h`, and `oil_rig_h` for direct launch by map
key. This is the first playable hose interaction slice, not a completed hose
port.

The shared host follows the authored PlayerHose camera rail and traces the
retail-shaped floor offset. Hotspot setup now follows control-record change
events: the first marker starts the first hotspot, and each D-linked hotspot
activates only when its A-rail anchor is selected. If target cycles finish
first, the host waits at that boundary. Retail
`00490BB0` reports the selected record when it differs from its cache; the
initial null cache therefore reports the first sample. The stream source also
uses `00465EF0`'s frame offset, while host mouse steering remains a provisional
nozzle direction. Rail speed is interpreted as world units per host second
and remains provisional. The host applies the traced zero-start WindowFire
timer ramp and contact subtraction. The frame delta is traced through
`0042aea0` → `0042c080` → `0042bf70`: WinMM millisecond differences are scaled
by `0.001`, paced until the stored delta exceeds `1/60`, and capped at `0.1`.
The manager writes the timer record's `+0x10` float through its `+0x4c` alias
at `00502b04`, explaining why the 55 direct Ghidra references are all reads.
The shared host now applies these thresholds with SDL ticks and `SDL_Delay(1)`.
Focus-pause behavior for absolute-time consumers, SDL tick quantization,
callback ordering, and live retail parity remain open; see
[`player_hose_frame_delta_refs_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_frame_delta_refs_2026-10-20.md).
Mouse-delta scaling, stream material binding, result feedback, and stage
return flow remain open. The `M_MaxPressure` alias and per-update pressure
smoothing are now traced and tracked in the host. Retail scales the stored
aim-velocity vector by `4000 * currentPressure` and records a separate shape scalar
`0.8 + 2.2 * currentPressure`; its strip consumer culls scalar values at or
below 1, passes twice the scalar to the geometry helper, builds
collision-tested strips, and derives a separate 10-to-40-unit contact padding.
The shared host now follows its eight-record, 0.75-second ballistic history
with 25 sampled sections, draws the traced dual-curve quads, and gates
WindowFire contact by the selected model AABB expanded by that radius. Its
texture/resource binding and color/alpha remain provisional. Do not treat the host
build as a live check or retail parity. The full port assumptions and compile
record are in
[`player_hose_port_host_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_port_host_2026-10-20.md).

### Ballistic PlayerHose stream follow-up (2026-10-20)

The current EXE hash-pinned trace resolves the stream's eight-record history,
0.75-second lifetime, 0.1-second sample interval, and 25 nonlinear sections.
The stored `4000 * pressure` vector is velocity; the main path applies the
interpolated source and aim plus `(0,-1000,0)*age^2`. The other curve uses
`velocity + (0,-500,0)` at 0.6 age. Retail submits four-vertex quads in
previous-main, previous-secondary, current-secondary, current-main order and
checks the main curve for collision. The port now draws those dual-curve quads.
`00469F50` appends each curve point and its associated scalar to parallel
arrays; its caller passes twice the interpolated sample shape scalar after the
`<=1.0` cull. Renderer resource ID 3 is traced to a numeric lookup helper, but
the bitmap, complete UV state, and blend/material flags remain open. The
rescue-all macOS app rebuilt with matching standalone/package SHA-256
`b2d7a1f802065a489636729ddb266a7e39df61bef528230ebe8123375bf77716`; its
plist passed validation, its catalog reports eight retail rescue choices, and
its resources include all four hose maps. No tests, interactive run, or retail
comparison was performed. The rescue-player HoseSpray update/contact path is
now partially ported into the shared rescue host. Slot-4 mode/action state,
sampled ballistic stream, previous-frame collision ordering, and terminal
`finalize=1` FireRescue suppression are represented. Camera-based item
attachment and left-mouse action mapping are provisional. The initial 100-ms
effect delay, residual-intensity gate, randomized deadline, shared-RNG velocity
draws, and child spawn request are now ported. Its renderer advances the
source's two-second 3.0 key to the zero-duration 5.0 key and applies flag-`0x8`
deactivation on the next host update. HoseSpray child allocation, render
flags, final texture binding, size-to-world mapping, authored bindings,
attachment-record purpose, exact pool cadence, and retail parity remain open. See
[`rescue_hose_spray_port_2026-10-20.md`](../tracker/evidence/fire-spread/rescue_hose_spray_port_2026-10-20.md).

The rescue-player HoseSpray contact writer is now traced through the shared
stream-collision component: HoseSpray embeds it at `+0x608`, and
`0046D1B0`/`0046C5A0` refresh its hit point, padding, and gate for
`00463EF0`. The latter runs before that refresh in each `UpdateHoseSpray`
call. A raw scan finds 16 authored `touch: tell player (touchhose N)` commands
on `RCS_PropModel` entities in 12 preserved rescue BSPs; existing PropModel
contact and named-actor evidence maps the event route to PlayerRescue
`00451870`. This closes the static authored-entry and contact-vector gaps;
the shared host now creates slot 4 and ports its update/contact/dousing path.
The HoseSpray cooldown, velocity request, and two-key particle track are
ported; particle material, attachment frame, attachment-record purpose, live
cadence, and parity remain open. The host assumes child allocation succeeds, consistent with the source
pool's unbounded configuration for normal operation. Retail passes N to
`00482780` on both allocation
and refresh paths; that helper scales N by 24, divides by attachment-state
`+0x40`, converts the value to an integer, and appends that many 0x30-byte
records to the list at `+0x48`. This supports a 24-scaled count but does not
establish a time unit or the records' purpose. See
[`hose_contact_frame_gap_2026-10-20.md`](../tracker/evidence/fire-spread/hose_contact_frame_gap_2026-10-20.md).
Per-map hashes and literal contexts are in
[`touchhose_authored_trigger_audit_2026-10-20.md`](../tracker/evidence/fire-spread/touchhose_authored_trigger_audit_2026-10-20.md).

The dedicated PlayerHose pressure field is now resolved through the embedded
stream component alias: hotspot `M_MaxPressure` at owner `+0x1834` aliases
component target `+0x3B4`; current stream scalar at `+0x1830` aliases
component `+0x3B0`. The source moves this scalar 20% toward the hotspot target
per update while held and 20% toward zero when released, without a `dt`
factor. `0046D1B0` scales the stored aim velocity by 4000 times current
pressure and independently maps the sample shape scalar from 0.8 to 3.0.
`0046BC10` interpolates the scalar; `0046C5A0` culls values at or below 1.0,
passes twice the scalar into its curve helper, builds four-vertex strips, and
collision-tests them. After a hit, component `+0x18` interpolates from 10 to
40 using current pressure; `00464AA0` expands the target WindowFire AABB by
that radius before queueing contact. The source's `0.75 / 0.1` sample history,
25 nonlinear sections, dual ballistic curves, and quad submission order are
now ported into the host. Strip material and scheduler cadence remain open.
The WindowFire timer
still ignores the payload as a rate input.
See
[`player_hose_pressure_stream_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_pressure_stream_2026-10-20.md).

The shared macOS executable and rescue-all app rebuilt after the pressure
port with matching standalone/package SHA-256
`3bb0928d0a5fa0974980aebeb314b4bf1d4fbf13c55bca29436eced9d674d36a`. The
bundle plist passed; all eight rescue catalog rows and four hose maps remain
packaged. The existing json-c macOS 27.0 versus target 13.3 linker warning
remains. No tests, interactive run, or retail comparison was performed.

The follow-up sample-consumer and contact-radius port rebuilt the shared
macOS target and rescue-all app. Standalone and packaged executable SHA-256
match at `f36c1d04f58384369c3135a410bcfdd54c2970cd422dd3b02a735b3b1d89ceea`;
the plist passed, all eight rescue choices and four hose maps remain packaged,
and the existing json-c deployment-target warning remains. No tests,
interactive run, or retail comparison was performed.

## Rescue PlayerRescue HoseSpray shared-host slice (2026-10-20)

The current-hash mode/action trace distinguishes acquisition, selection, and
spray: `touchhose N` creates slot 4 in mode 2, selecting slot 4 requests mode
3, and vtable action-start/stop callbacks control the independent `+0xC58`
action byte. The shared rescue host now ports those state transitions,
`0046D1B0`'s pressure and 15-section ballistic strip, the shared component
collision gate, previous-frame contact ordering, and terminal FireRescue
suppression at influence `>= 0.01`. Key 4 and held left mouse are provisional
host controls; the camera-derived item frame is provisional. The shared sound
bundle and owned `HoseSpray` action press/stop are ported; held-loop behavior is
provisional. The host now applies the 100-ms initial effect delay and, after
terminal suppression, the residual-intensity `>=0.01` gate and direct unsigned
deadline comparison. When due it advances the millisecond deadline by a
shared-RNG delay in `[10,250]` converted toward zero, then draws velocity in
retail `{last,height,first}` axis order and requests a child. Fresh Ghidra
traces show HoseSpray's size track starts at key 0 with scalar 3.0; its second
key's scalar is 5.0 and is not selected initially. The shared renderer now
passes the source's current 3.0 key scalar as a per-particle preview size,
with the mapping to a full OpenGL quad side still provisional. HoseSpray
particles advance one key per host update, switch to the terminal 5.0 key at
the source's two-second deadline, and deactivate on the next update through
the source flag-`0x8` end path. Extinguisher particles keep their existing
preview lifetime. Pool allocation failure, child `+0xA4` render flags, final
resource selection, exact host/retail timing, and render cadence remain
unresolved.
The `00463EF0` instruction listing preserves the clock addition and conversion
that the C-like export omits. Resource-3 material, authored bindings,
attachment-record purpose, scheduler cadence, and retail parity remain open.
The generic equipment
follower chain is confirmed under Player `+0x34A4`, while the exact writer into
the transform consumed from HoseSpray `+0x58` remains unresolved.
Details and binary/build hashes are in
[`rescue_hose_spray_port_2026-10-20.md`](../tracker/evidence/fire-spread/rescue_hose_spray_port_2026-10-20.md).
The shared macOS executable and rescue-all app rebuilt with matching standalone
and packaged SHA-256 `b8bdedcba9de98a0bd3a217bee40ecb27465c832656dfd58429f140fb530bba8`;
the plist passed, eight rescue choices were emitted by the catalog build, all
four PlayerHose scene directories are packaged, and the HoseSpray WAV hash was
verified in each rescue scene manifest. No tests, interactive run, or retail
comparison was performed.

After the fresh particle-template trace, the shared macOS target and
rescue-all app rebuilt with matching standalone/package SHA-256
`19a4f8e02b096e24e46a4dbce87f006ae44905fdf46a9c2fca56c9f7f520cc8c`. The
plist validated, the catalog contains eight entries, and all four PlayerHose
maps are packaged. The existing Homebrew json-c 27.0 versus deployment target
13.3 linker warning remains. No tests, interactive run, or retail comparison
was performed.

After porting HoseSpray's 2.0-second key-0 transition to the 5.0 terminal
key and flag-`0x8` deactivation on the next host update, the macOS rescue-all
app rebuilt with matching standalone/package SHA-256
`9711510c53ea971da144df5eacea6d9d7e09af127b92bb51ad32ab56b515d43f`. The
plist validated; all eight rescue choices and four PlayerHose maps remain
packaged. `git diff --check` passed. Pool cadence, size mapping, material,
alpha, and retail parity remain unverified. No tests or interactive run was
performed.

The pressure trace establishes a stored aim-velocity magnitude of
`4000 * currentPressure` and a separate `0.8 + 2.2 * currentPressure` shape
scalar. The prior single-ray port treated velocity scale as final reach; the
ballistic-history follow-up below supersedes that interpretation. The
intermediate rescue-all executable SHA-256 was
`39c681b93c13f6bfb79b1b82fbfc8a3f19f55d450992ba509f0521c1786789ac`.

The first gameplay-slice executable compiled successfully with SHA-256
`514dd0ae15bb0c733f7b1b01dca939f26eb5e5d23c1696b187ec6f3af1eb059c`.
The rescue-all app bundle also built successfully; its embedded catalog has
eight rescue entries and its scene resources include all four hose maps. The
build's plist validation passed. Only the existing Homebrew json-c
deployment-target warning appeared. No test suite or interactive run was
performed.

| Scope | Files | Status |
| --- | --- | --- |
| PlayerHose gameplay host | `scene_viewer.cpp`, `player_hose_stage.h`, macOS bundle script, port-host evidence note | Implements anchor-driven hotspot activation, target/cycle flow, authored spline camera rail, source-derived stream point and timestamped dual-curve strip geometry, source-shaped temporary aim rotation, and retail-delta pacing; rail speed calibration, state-frame cadence, SDL-to-steering mapping, strip resource/UV/material binding, mode labels/initial choice, result and return flow remain provisional/open |
| Frame-delta clock trace | `0042bf70_frame_clock_delta_fresh.c`, frame-delta evidence note, shared SDL loop | Source producer and second-based delta traced; `>1/60` pacing and `0.1` cap ported; focus-pause/absolute-time mapping and retail parity remain open |

### Frame-clock source and pacing port (2026-10-20)

The timer alias now resolves `00502b04` to the active `timeGetTime()` clock
record; the shared SDL loop applies the `0.001` millisecond conversion,
`>1/60` minimum update interval, and `0.1` cap. PlayerHose `M_SpotTime`, its
remaining countdown, and `SprayTime` are seconds. Focus-pause effects on other
absolute-time consumers, SDL tick quantization, callback ordering, and retail
parity remain open.

The rescue-all macOS app rebuilt with matching standalone and packaged
executable SHA-256
`7d04b10a561f5a1bafc53551ef601093a8a894d6c8a68498c076cc87e2a2f17b`; its
plist passed, all eight rescue choices and four hose scenes are packaged, and
the existing json-c macOS 27.0/target 13.3 linker warning remains. No tests,
interactive run, or retail comparison was made.

### Anchor event and stream source follow-up (2026-10-20)

The current-hash evidence resolves the `00490BB0` return polarity: the sampler
returns the selected record on a change, then caches that record. The first
record is reported because the constructor initializes the cache to zero.
`004677D0` accepts a changed record when its HosePath node matches the expected
D-linked hotspot or carries `M_IsFirstHotSpot`; it stores the scaled time
budget before state 4 copies that budget into the live countdown. The shared
host now follows those anchor events and no longer activates the next hotspot
as soon as prior target cycles finish. It also uses the source-derived
`00465EF0` stream-point offset and keeps steering in the aim direction.

The shared macOS executable and rescue-all bundle rebuilt at matching SHA-256
`7fc114f3bee70cef91352ae49d85c694c892db8128156ad6759c760b84918018`. The
plist validation passed. No interactive run, test suite, controlled retail
comparison, or parity claim was made. Exact scheduler-frame timing, rail
speed calibration, steering accessors and clamps, WindowFire baseline, and
retail result/return flow remain open.

### Temporary PlayerHose aim-frame rotation (2026-10-20)

Fresh `00466780` evidence distinguishes the rail-derived entity frame from the
temporary aim frame. The shared host now copies its rail camera frame, rotates
the basis around axis 0 by pitch, then around the updated axis 1 by negative
yaw, following `00496DD0` and `004476B0`'s basis-by-axis cross-product order.
The stream source stays tied to the unrotated entity frame through `00465EF0`.
SDL mouse values still stand in for `STEER_PITCH/YAW`, so accessor cadence,
scale/sign mapping, additional view-angle limits, and x87 rounding remain
open.

The standalone and rescue-all executables rebuilt at matching SHA-256
`6e639a00367ce7bff8a95da4b98b7fc6e4805c050c1399715527c32a43692bbc`. The
bundle plist passed validation and retains all four hose scenes and eight
rescue catalog choices. This is compile/package evidence only; no interactive
or retail comparison was made.

### WindowFire selected-target timer (2026-10-20)

The shared host now models the source-derived WindowFire timer: selecting a
target starts `+0x258` at zero and enables its ramp; the ramp adds `2*dt` until
the timer reaches `1.0`, and matching held-stream contact subtracts
`dt/SprayTime`. Completion uses the source's strict below-zero comparison and
preserves zero/negative divisor arithmetic. This replaces the earlier
`1.0 -> 0.0` duration proxy. Callback/entity update ordering, frame-delta
calibration, and live contact cadence remain provisional.

The trace pins `_DAT_004dc50c` to `0.0f`; `00468850` resets the timer and sets
the ramp flag, `00468870` advances the ramp, and `00468680` performs the
contact subtraction and strict-threshold reset. Source excerpts are in
[`playerhose_douse_windowfire_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/playerhose_douse_windowfire_fresh_2026-10-20.c)
and [`playerhose_hotspot_state_machine_fresh_2026-10-20.c`](../tracker/evidence/scenario-loading/playerhose_hotspot_state_machine_fresh_2026-10-20.c).
The rescue-all macOS bundle rebuilt successfully at matching standalone and
packaged executable SHA-256
`598ecd280b436093a5e11d0e0d2775924bbc10a1defcbb0833876399ed3bc52c`; its
plist passed and the eight rescue choices plus four hose scenes remain
packaged. The existing json-c deployment-target warning remains. No tests,
interactive run, or retail comparison was made.

Two read-only Luna audits and the first direct-offset scan did not resolve
`M_MaxPressure`; the later shared-component alias trace does. The scan at
`0x1834` found only the path-record write because the consumer accesses the
same storage through component `+0x3B4`. The direct-offset scan details remain
in [`player_hose_pressure_scan_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_pressure_scan_2026-10-20.md),
and the resolving trace is in
[`player_hose_pressure_stream_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_pressure_stream_2026-10-20.md).
Construction node 36 remains outside the verified forward A rail and
first-hotspot/D progression chain; alternate runtime entry and
`E_NextHotSpotPathDir` semantics remain unresolved.

| Scope | Files / evidence | Status |
| --- | --- | --- |
| WindowFire target timer | `player_hose_stage.h`, `scene_viewer.cpp`, PlayerHose source excerpts and port-host note | Zero-start ramp, contact divisor, and strict threshold implemented; update cadence and visual parity remain open |
| Pressure-field and Construction-branch audits | PlayerHose pressure alias/sample-consumer traces and `const_h` entity graph | Pressure target/current alias, sample-geometry input, and 10-to-40 contact padding traced and ported; exact stream appearance and node 36 runtime entry remain unresolved |
| `windowfire_threshold` (Luna) | Current-hash `00468850`/`00468680` exports and `00468870` instructions | Complete, read-only; zero-start timer, strict zero threshold, and ramp rate integrated; scheduling remains open |
| `max_pressure_trace` (Luna) | `004677D0`, current-hash `+0x1834` instruction scan | Complete, read-only; its immediate-offset scan found the assignment; root's later component-relative alias trace and pressure port are recorded in `player_hose_pressure_stream_2026-10-20.md` |
| `hose_construction_route` (Luna) | Hash-matched `const_h` BSP graph and `004677D0` | Complete, read-only; node 36 is not in verified A/D path; direction-property reader remains unresolved |

## PlayerHose authored rail trace and shared-host camera port (2026-10-20)

Fresh Ghidra 12.0.4 decompilation against retail EXE SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` traces
`00468050/0049D160 -> 004661A0 -> 00490510/00490D00/004905A0` and the runtime
samplers `00490AB0/00490BB0`, plus the anchor consumer in `004677D0`. The PlayerHose constructor validates a 20-byte
UserData record, stores it at `+0x194`, and the builder reads its `+0x10` route
pointer. That layout strongly maps to the authored `FirstPathPoint`, while the
compiled property descriptor offset remains unverified. HosePath offsets
`+0x10/+0x14/+0x18` strongly map to `A_NextPoint`, `B_LookAtPoint`, and
`C_IsAHotSpot`. The builder follows the A chain, closes a null tail to the
first point, creates 32-byte controls and ten subdivisions per segment, and
interpolates LookAtPoint coordinates for the camera.

The exports have 19/34/17/13 HosePath records; the FirstPathPoint A chains
reach 10/19/13/7 nodes. `RCS_CamHose` is a separate runtime helper lookup and
its absence from the BSP tables does not prevent HosePath binding. Retail
samples centered half-difference cubic curves, advances with a `+500` factor,
traces down from Y `+50` to `-2000`, and positions the rail at floor Y `+32`.
The host ports this authored camera rail and emits hotspot events when the
selected control record changes. `004677D0` accepts an event only when its
node matches the expected D-linked hotspot or carries the first-hotspot marker.
If target cycles finish before the expected next anchor, the shared stage waits
at that target boundary.
The host stream source uses the `00465EF0` entity-frame offset, while its
mouse-to-nozzle direction mapping remains provisional. Speed is interpreted
as 500 world units per host second and remains provisional because retail
delta-time calibration is open. Exact camera/state cadence and live parity
remain unverified. Full evidence and BSP hashes are in
[`player_hose_curve_pipeline_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_curve_pipeline_2026-10-20.md).

The function-review tracker now has 393 reviewed functions. This camera
milestone added fresh reviews for `00468050`, `0049D160`, `00468230`, and
`0043B2D0`, with the existing `0048FF30` world probe review linked to the
camera evidence. The later pressure follow-up expanded review `0046D1B0` and
added `0046C000`. No test suite or interactive run was performed; the shared
macOS build is not retail parity.

| Scope | Files | Status |
| --- | --- | --- |
| PlayerHose authored rail | Fresh constructor/builder/sampler/collision excerpts, spline evidence, `player_hose_stage.h`, `scene_viewer.cpp` | UserData and HosePath correspondence mapped from size/layout/runtime access; shared host follows the A-chain and renders the sampled floor-following camera rail; property descriptor, timing calibration, aim interaction, cadence, and retail parity remain open |

The standalone and bundled macOS executables rebuilt with identical SHA-256
`597ca269667e3002d2411c40877f0d5c0eef1ec210f48a39a0f4a68c6a1ae3be`.
The rescue-all bundle's plist passed validation and retained the eight-choice
rescue catalog plus all four hose maps. No interactive run or test suite was
performed.

## PlayerHose hotspot timer mode scale ported (2026-10-20)

Current-hash x86 bytes at `004677D0`, cross-checked against the existing
Ghidra export, resolve the session mode table applied to authored `M_SpotTime`:
index 0 multiplies by 1.8, index 1 by 1.3, and every other index by 1.0 before
the result is stored as a float countdown. The shared host now applies this
rule and accepts `--hose-mode-index 0|1|2` for a direct hose-map launch. Its
default index 2 is a host choice matching the source's 1.0 branch; the retail
menu labels and initial mode value remain unknown. Do not label these indices
Easy/Normal/Hard without further evidence. Details and the pinned EXE hash are
in [`player_hose_mode_timer_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_mode_timer_2026-10-20.md).

This timer checkpoint added no EXE function review; the tracker stood at 382
reviews then. Later hose traces bring the current count to 392. The rescue-all macOS bundle rebuilt with this timer change, its
`Info.plist` passed validation, the embedded rescue catalog has eight rows,
and the four hose maps are present. The packaged executable SHA-256 is
`61578f31be495ce98c1e951f7afeb09cc7f3c80f097f17d69286a27636332f5e`.
Camera/path motion, WindowFire visual behavior and result/return flow remain
larger PlayerHose gaps; no interactive or retail-parity check was made.

| Scope | Files | Status |
| --- | --- | --- |
| PlayerHose mode-scaled timer | `scene_viewer.cpp`, `player_hose_stage.h`, launcher, mode-timer evidence | Numeric mode table ported; retail labels/selection and host default remain explicitly unresolved |

## Player fire-exposure padding resolved (2026-10-20)

Fresh headless Ghidra output against pinned EXE SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` traces
the PlayerRescue constructor chain through `0049D8C0`, which initializes the
fourth float at root `+0x74` to `0.0f`. FireRescue callback `004483A0` passes
that element `[3]` to `0044A7A0`; getter `0049D790` and cache-copy helper
`00441E90` update only XYZ. The shared port now passes zero padding in both
unobstructed and visibility-attenuated Player exposure calculations, replacing
the former provisional 30-unit movement-hull padding. Exact callback cadence,
later arbitrary aliases, emitter scale and the minimum-influence constant
remain open; no retail watchpoint or natural exposure capture was available.

The fresh pseudocode and trace are in
[`player_fire_exposure_padding_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/player_fire_exposure_padding_fresh_2026-10-20.c)
and [`player_rescue_candidate_getter.md`](../tracker/evidence/fire-spread/player_rescue_candidate_getter.md).
The shared macOS `fdny-scene-viewer` target rebuilt successfully. The resulting
executable at
`port/spikes/training-native-prototype/.local/build-scene-viewer/fdny-scene-viewer`
has SHA-256 `a05e9fbfb5e8f4d467738282a1a44e58e885eda2545fa18d3462b912142a11a8`.
Linking emitted the existing Homebrew `json-c` deployment-target warning
(library built for macOS 27.0, target linked for 13.3). This compile-only check
does not establish fire-damage feel or retail parity.

| Scope | Files | Status |
| --- | --- | --- |
| Player fire falloff padding | `scene_viewer.cpp`, `player_fire_contact.h`, `player_rescue_candidate_getter.md`, fresh export, function-review CSV | Implemented using constructor-backed `0.0f`; callback cadence, possible later aliases, emitter scale, cutoff and live damage behavior remain open |

## Mission outcome status dispatch traced (2026-10-20)

Fresh headless Ghidra 12.0.4 analysis of pinned EXE SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` maps the
Player status callback's result path. When the status is visible, Player
`+0x3438 == 1` selects widget state `0x47`, dispatches `RC_ResLvlWin`, and
requests the `RescueWin` owner sound; state `2` selects `0x48`, dispatches
`RC_ResLvlLose`, and requests `RescueLose`. The separate chatter sets each have
one authored sound entry. Their direct sound files and dispatch chain are
recorded in
[`mission_outcome_status_dispatch_2026-10-19.md`](../tracker/evidence/scenario-loading/mission_outcome_status_dispatch_2026-10-19.md),
with fresh pseudocode in
[`mission_outcome_state_fresh_2026-10-19.c`](../tracker/evidence/scenario-loading/mission_outcome_state_fresh_2026-10-19.c),
the timed widget helper trace in
[`mission_outcome_widget_fresh_2026-10-19.c`](../tracker/evidence/scenario-loading/mission_outcome_widget_fresh_2026-10-19.c),
renderer/resource lookup trace in
[`mission_outcome_widget_resource_fresh_2026-10-19.c`](../tracker/evidence/scenario-loading/mission_outcome_widget_resource_fresh_2026-10-19.c),
and sibling widget caller trace in
[`mission_outcome_widget_callers_fresh_2026-10-20.c`](../tracker/evidence/scenario-loading/mission_outcome_widget_callers_fresh_2026-10-20.c).
The result child's post-duration update rotates its quad around `(320,240)` at
`ratio * 1800` degrees and hands completion to frontend manager state 1/2;
selected decompilation and constants are in
[`mission_outcome_frontend_handoff_fresh_2026-10-20.c`](../tracker/evidence/scenario-loading/mission_outcome_frontend_handoff_fresh_2026-10-20.c).

Decoded-instruction scanning found direct initialization of `+0x3438` to zero
and two final-exit writers setting it to one; no direct state-2 write appeared
in the bounded scan. The direct state-2 writer and meaning, audio replay cadence, relationship to
`time:`/`+0x343C`, and complete score/outro/return path remain unresolved. The
timed widget selector mapping is now traced through the generic renderer into
the authored rescue resource table: `0x47`/71 draws `paperwin-r.bmp`, and
`0x48`/72 draws `paperlose-r.bmp`, both with `paperwin_a.bmp`; the hose set has
its own textures at those IDs. The result child's duration/tail field values,
desktop placement, alpha behavior and rendered runtime appearance remain
unverified. A fresh direct
operand scan found no assignments to the result child's `+0x130/+0x134` timing
fields; alias/computed writes remain possible. The `1000/5000` values in
`0048D330` belong to sibling widget constructors `00458B20` and `004653F0`,
not the Player result child created through `004576B0`/`0048D390`. This is
static source and resource evidence only; no natural mission
completion or retail parity check was made. The current host still uses
`RescueExitState.messageGate` for the message gate and does not implement this
result feedback. Do not wire a guessed timeout or loss transition. The
function-review ledger reached 351 after the sixteen status/chatter/widget
reviews, 355 after the four PlayerRescue exposure-padding reviews, 365 after
ten fresh HoseSpray/PlayerHose contact-path reviews, and 410 after the recent
HoseSpray, result-widget, and frontend-consumer batches.

| Scope | Files | Status |
| --- | --- | --- |
| Mission outcome state and feedback | `mission_outcome_status_dispatch_2026-10-19.md`, fresh Ghidra exports, `rescue_completion_and_exit_path.md`, function-review CSV | Win/loss dispatch, rescue textures, post-duration rotation, and frontend consumer traced: state 1 updates selected profile/catalog data and selects child 5; state 2 selects level-selector child 2. State-2 producer, result-child timing, child-5 content/actions, runtime placement/alpha, replay cadence, and full outcome flow remain open |

The result path is now known to use direct calls: `00458400` invokes
`0048D180` and `0048D1A0` on `PlayerStatus+0x824`, while `004576B0` constructs
that child with `0048D390`. The sibling timing setter's `1000/5000` defaults do
not apply to it. A fresh decoded-offset scan and decompilation now trace the
downstream manager consumer: on result completion, manager state 1 updates the
selected catalog/profile record and selects frontend child 5; state 2 selects
child 2, the traced level selector. Child 5 is constructed by `00432BB0` with
two buttons and menu bitmap ID 54, but its intended role and content are not
identified. The focused export is
[`mission_outcome_frontend_consumer_fresh_2026-10-20.c`](../tracker/evidence/scenario-loading/mission_outcome_frontend_consumer_fresh_2026-10-20.c).
Next, identify child 5's labels/action path and continue tracing result-child
timing initialization through aliases or runtime evidence. Do not port guessed
timing, placement, or a loss-state producer. The bounded scan still found no
direct writer of Player `+0x3438 = 2`; if retail runtime access becomes
available, set a write watchpoint during expiry, passout, and exit paths.

## RescueBot lifecycle implemented from fresh retail trace (2026-10-20)

Fresh Ghidra 12.0.4 analysis of the pinned EXE, plus direct x86 instruction
checks, resolved the recurring RescueBot attempt limit, target selector,
response timers, active-victim deadline, victim state-6 hide transition, and
drop-off call to the shared final-exit refresh. The fresh eight-function
decompilation is saved in
[`rescuebot_lifecycle_fresh_2026-10-19.c`](../tracker/evidence/scenario-loading/rescuebot_lifecycle_fresh_2026-10-19.c).
The shared port now implements this lifecycle in
[`rescue_bot.h`](../port/spikes/training-native-prototype/src/rescue_bot.h)
and [`scene_viewer.cpp`](../port/spikes/training-native-prototype/src/scene_viewer.cpp),
including authored `norescuebot` handling and reuse of the existing final-exit
transaction. Player `+0x3530` is counted against the source mode formula
`int(totalVictims * factor + 0.5)` with factors 0.4, 0.25, or 0.0; the
constant 0.5 at `0x004DE6F8` was read from the pinned binary.

The native path runs only after an explicit `setrescuebot 1`. Training sets
the field to 0; no other preserved rescue map script sets it, and the retail
default byte remains unresolved. The callback's initializer-object identity,
the viewer's per-render-frame cadence, and its crouch/climb Player-state
adapter are provisional. The separate authored `runrescuebot` one-shot path
is still open. No natural RescueBot playthrough or retail parity check was
made.

The macOS `fdny-scene-viewer` target compiles with this change; output SHA-256:
`a91c67e52ccbf31881881e0ea3daca69b76664f98be3abd89c5a556be69f6ae8`.
The macOS rescue-all app was packaged at
`.local/FDNY Firefighters.app`; its bundled executable has the same SHA-256.
The bundle contains 13 rescue map stages and the catalog reports eight
campaign choices in source order. `Info.plist` passes `plutil -lint`. The app
was not launched, so this packaging check does not establish runtime behavior.
The function-review ledger gained five fresh function reviews, bringing its
count to 335. `tools/build_tracker.py` regenerated the function atlas, summary,
and dashboard from the reconciled CSVs. `git diff --check` passes. These build
and static checks are not mission or retail-parity checks.

| Scope | Files | Status |
| --- | --- | --- |
| Recurring RescueBot behavior | `rescue_bot.h`, `scene_viewer.cpp`, `victim_transactions.h`, RescueBot evidence, function-review CSVs | Implemented behind explicit enable; frame cadence and initial object identity provisional |
| RescueBot enable defaults and one-shot command | RescueBot evidence and authored script inventory | One-shot entry rechecked at `00475020` → selector `00455230` on Player `+0x340C`; port dispatch and relation to recurring context `+0x32CC` remain open. Retail default enable byte is unresolved. |

## Hose gameplay paths and RescueBot one-shot audit (2026-10-20)

The separate `runrescuebot` command is confirmed as a debug one-shot:
`0047A1F0` dispatches to `00475020`, which calls selector `00455230` with
Player `+0x340C`. The recurring updater receives Player `+0x32CC`, reads its
enable gate from `+0x32E4`, and applies attempt/deadline handling. The contexts'
aliasing and one-shot drop-off servicing remain unproven; the inventory has no
authored `runrescuebot` command. Keep the two paths separate until those links
are established. The read-only trace is recorded in
[`rescuebot_update_path_2026-10-15.md`](../tracker/evidence/scenario-loading/rescuebot_update_path_2026-10-15.md).

Two separate retail hose paths are present in the binary. PlayerRescue command
handler `00451870` routes `touchhose` to `0044CAB0`, which creates the
`0xD54`-byte HoseSpray item with constructor `004634F0`, attaches it, stores it
in inventory slot 4, and requests mode 2. Selecting slot 4 through the Player
item-switch path requests mode 3; the update runs in modes 1 or 3, while the
action byte is controlled separately. The shared rescue host now ports the
mode/action boundary, 15-section stream, previous-frame contact scan, and
terminal FireRescue suppression. Retail slot-4 and spray input bindings remain
unknown; the host exposes key 4 and held left mouse as provisional controls.
Preserved rescue BSPs contain 16
`touch: tell player (touchhose N)` PropModel bindings across 12 maps. Existing
PropModel contact-event and named ScriptActor traces map the static route to
the PlayerRescue handler; the shared host now models its slot-4 loadout command
and attachment-list count, mode/action transitions, sampled stream, previous-
frame contact, and terminal dousing. The item frame, effect presentation,
attachment-record purpose, and live callback cadence remain open. See
[`touchhose_authored_trigger_audit_2026-10-20.md`](../tracker/evidence/fire-spread/touchhose_authored_trigger_audit_2026-10-20.md).
The separate dedicated-stage path has a first shared-host slice.
All four dedicated hose BSPs instantiate `RCS_PlayerHose`
instead of `RCS_PlayerRescue`. Initializer `00466D60` builds its path/camera/
douse components; update `00466780` reads hose mouse/steering input and uses
`00465EF0` to calculate its aimed point. The examined updater uses
`0046D1B0` for hose visuals but does not call FireRescue suppression directly.
The douse component callback `00464AA0` requires its hit gate at `+0x1C`,
compares the hit volume with a target's virtual bounds, and queues contact
position plus intensity through `00481A60` as message `0x7530`. `00465B90`
creates runtime `RCS_WindowFire` actors from path-point fire references. Their
message handler routes `0x7530` directly to `00468680`, which updates WindowFire's own
timer and geometry when its `+0x2A8` state has `+0xC` and `+0xE` gates set.
This path has no direct call to FireRescue suppression or its doused counter.
In the PlayerHose initializer/update pseudocode, `param_1` is an `int *`, so
`param_1 + 0x520` is the embedded stream component at PlayerHose byte `+0x1480`,
not byte `+0x520`. `0046C5A0` writes its hit XYZ and active flag at PlayerHose
`+0x148C/+0x1490/+0x1494/+0x149C`; `00464AA0` reads this owner state, while
the DouseHose callback object owns a separate contact queue at `+0x28`.
Keep the stream state and callback queue as distinct port objects.
Fresh tracing of `004658A0`, `00465A20`, `00465FB0`, `004674A0`, `004677D0`,
`00468850`, `00466C50`, and `00466CE0` resolves selected/dousing gates,
per-hotspot `Q_NumCycles`, the mode-scaled `M_SpotTime` deadline, and the
dedicated `RC_HoseLvlStart/Win/Lose` result path. `00468870` separately reads
WindowFire linked `+0x10` at `004688E7`; if set while dousing, it calls the
same timer/update routine, but direct `0x7530` messages bypass this gate. The
bounded decoded-code scan found only a zero write to `+0x10` in `00468AC0`
(`00468B01`); it also clears `+0x0D` and `+0x0F`. No nonzero setter surfaced
in the examined WindowFire/manager methods. A recurring-update-gate role is a
control-flow inference only; computed aliases, bulk initialization, opaque
helpers, external writes, and authored meaning remain open. Current BSP exports contain 83 HosePath nodes
and 28 authored hotspots across four dedicated stages. Construction's first
hotspot A_NextPoint loop is `12→13→27→28→33→34→14→35→37→15→40→41→16→43→17→18→45→26→25→12`;
its D_NextHotSpot chain is `12→13→14→15→16→18→26→null`. Hotspot 36 points to
15 on both A and D, so it is an inbound branch to the main A route, not reached
by forward traversal from 12. `004677D0` and `004674A0` support D as the next
hotspot progression link; a read of `E_NextHotSpotPathDir` or runtime entry
from 36 has not been found. One non-hotspot path record also contains ZF fire
references. The hash-pinned trace and per-map counts are in
[`player_hose_stage_lifecycle_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_stage_lifecycle_2026-10-20.md).
The function-review ledger now has 382 reviewed EXE functions; the tracker
atlas, summary, and dashboard were regenerated from the CSVs. The referenced
decompilation remains static evidence, not live parity; the first dedicated
host implementation is recorded at the top of this queue.
The earlier shared-host audit found that `entities.json` preserved the hose
actor fields while the viewer treated `RCS_PlayerHose` only as a spawn marker.
That handoff is superseded by the dedicated host slice at the top of this
queue: scene loading now retains path nodes, resolves `%Model%` target IDs and
preserves ordered ZF/SprayTime slots; the macOS bundle carries all four hose
maps. The implementation remains a provisional free-movement/action host and
does not reproduce the retail spline camera, WindowFire visuals, result widget,
or transition. Keep it separate from Training interactions and rescue-player
HoseSpray transactions.
The local `city_h` export is a clean first authored slice: PlayerHose starts at
`location=3088 -168 2112` with `FirstPathPoint=RCS_HosePath19`, the first
hotspot is node 1 with A/D links to node 2, `Q_NumCycles=2`, and
`M_SpotTime=40`. Across its nine hotspots, all 29 referenced fire names match
the 29 `%Model%` records by `%name%`; these are model entities, not
`RCS_FireRescue` actors. A dedicated WindowFire target list must preserve each
ZF slot and paired SprayTime and resolve its model actor separately from the
rescue fire loader. These are source-matched authored-data facts, not evidence
of complete route reachability or live behavior.
The retail `thegame.lev` source (SHA-256
`ee6819873cd540e67613774d848506561e2861442b37ef301e27273805d558c6`) has 13
records: Training plus eight `rescue.rrf` missions and four `hose.rrf` stages.
The current chooser deliberately keeps only the eight rescue records; the hose
records are `dock_h` (source ordinal 1), `const_h` (5), `city_h` (8), and
`oil_rig_h` (10). The rescue-all bundle now includes all four hose scenes for
direct launch by map key. A retail-authentic hose selector and integration
with the title/briefing/result flow remain open.
Defined-string xrefs also place `RC_HoseLvlStart/Win/Lose` in the
`TRadioChatter` setup at `004895C0`, which registers `TChatterSet` entries via
`00489200`. `004897C0` resolves the message key, and the emitters' nonzero
payload selects the shuffled-entry path at `00489110`, which requests the
chosen sound through manager `004A0BE0`. `Data/ResDefs/sound.rrf` maps the
three keys to `start2_b.wav`, `hosewin2.wav`, and `lose5_b.wav` under
`Data/Snd/chatter/` (SHA-256
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`). This
confirms configuration and a sound request, not audible playback.
`Data/ResDefs/hose.rrf` maps result selectors 71/72 to `paperwin-h.bmp` and
`paperlose-h.bmp` with `paperwin_a.bmp` (SHA-256
`93a1b2dae7d755fdd66fabaee89514de8e420cfb62f2f96132b341c0cce6fe45`), but a
PlayerHose result consumer is not confirmed. The apparent `+0x198` consumer in
the prior widget export is a generic rescue `PlayerStatus` path, not PlayerHose.
The result presentation and mission transition remain unresolved. The
supporting export is
[`hose_stage_message_registration_fresh_2026-10-20.c`](../tracker/evidence/scenario-loading/hose_stage_message_registration_fresh_2026-10-20.c).
Keep this path separate from rescue-player HoseSpray, which calls shared
FireRescue suppression with `finalize=1`; the extinguisher route remains
`finalize=0`.

Fresh trace of `00466780` confirms `MOUSE_LB`, `STEER_PITCH`, and `STEER_YAW`
inputs. Pitch state at PlayerHose `+0x230` subtracts `STEER_PITCH` and is
clamped symmetrically by `+0x238`; yaw at `+0x234` subtracts `STEER_YAW` and
uses the symmetric `+0x23C` limit. Hotspot entry `004677D0` copies
`N_PitchSpread` from the path record's `+0x30` into `+0x238`, and
`M_MaxPressure` from `+0x2C` into `+0x1834`. The pitch value therefore bounds
the steerable pitch in degrees; constructor `00466D60` starts its pitch/yaw
limits at 45/60 degrees, and hotspot entry replaces the pitch limit. Helper
`00496DD0` multiplies the angle by the degrees-to-radians constant and `0.5`
before its sine/cosine rotation. The limit's relationship to path-angle
progression remains open. The pressure value has no direct immediate-offset
consumer because `0046D1B0` reads it through the embedded component's
`+0x3B4` target alias. Stream-owner current scalar at `+0x1830` is component
`+0x3B0`; the source smooths it toward the pressure target while held and
toward zero otherwise. `00465EF0` calculates the nozzle aim and `0046D1B0`
updates the stream sample records, storing a velocity vector scaled by
`4000 * pressure` and a separate `0.8 + 2.2 * pressure` shape scalar. The
shared port follows the source's timestamped ballistic history and renders its
sampled centerline; the strip width/color transfer and retail visual are not
yet recovered.
See [`player_hose_pressure_stream_2026-10-20.md`](../tracker/evidence/scenario-loading/player_hose_pressure_stream_2026-10-20.md),
the hose lifecycle note,
[`hose_contact_and_playerhose_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/hose_contact_and_playerhose_fresh_2026-10-20.c),
and [`player_hose_angle_rotation_fresh_2026-10-20.c`](../tracker/evidence/scenario-loading/player_hose_angle_rotation_fresh_2026-10-20.c).

The time-warning caller `004653B0` passes the decremented hotspot countdown and
literal `5` to shared helper `00481F40`; `004B98B0` floors both the current
value and comparison marker, and the warning branch requires current floor 5
and stored floor 6. A nonzero low-byte return requests `RC_HoseTimeWarn`, so
the source supports a six-to-five countdown-unit crossing. The traced frame
delta is in seconds, so `M_SpotTime` and its remaining countdown are seconds;
the selected chatter entry and audible playback remain unverified.
`sound.rrf` maps warning entries 0/1 to `time4_b.wav` and
`time5_b.wav`. See the fresh
[`hose_pressure_path_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/hose_pressure_path_fresh_2026-10-20.c),
[`hose_time_warning_helper_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/hose_time_warning_helper_fresh_2026-10-20.c),
and [`hose_time_floor_helper_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/hose_time_floor_helper_fresh_2026-10-20.c).

The rescue-player HoseSpray update `004640F0` calls `00463EF0` while item
action byte `+0xC58` is active. That helper reads contact `+0x614..+0x61C`,
padding `+0x620`, and gate `+0x624`, evaluates shared ellipsoid falloff, and
calls `00448970(finalize=1)` at influence `>= 0.01`. The extinguisher message
route remains `finalize=0`; preserve that distinction. A fresh immediate-offset
scan found no direct HoseSpray XYZ writer: `004640F0` passes the `+0x608`
component to `0046D1B0`, which calls collision writer `0046C5A0`. The shared
component stores the hit point, pressure-derived padding, and contact gate at
`+0x0C..+0x1C`, corresponding to item `+0x614..+0x624`. Contact runs before
the next collision refresh, closing the static writer route while runtime
cadence and parity remain open. The WindowFire `+0x10` scan
found a clear at
`00468B01` in `00468AC0`, with no nonzero store in the examined class/manager
methods; `0x7530` calls `00468680` directly while the separate `00468870`
recurring branch is gated by that byte. Its authored role and indirect/runtime
writers remain open. The configured chatter resource names and request path
are traced; result presentation and mission transition remain open. The
HoseSpray contact writer and authored `touchhose` event route are statically
resolved through the shared component, PropModel contact dispatch, and named
Player command path; the shared host now implements the acquired slot-4 route
with separate state. Construction branch entry and its authored controls remain
open.
Evidence is in
[`hose_contact_frame_gap_2026-10-20.md`](../tracker/evidence/fire-spread/hose_contact_frame_gap_2026-10-20.md),
[`hose_contact_and_playerhose_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/hose_contact_and_playerhose_fresh_2026-10-20.c),
[`playerhose_douse_windowfire_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/playerhose_douse_windowfire_fresh_2026-10-20.c),
[`windowfire_link_state_fresh_2026-10-20.c`](../tracker/evidence/fire-spread/windowfire_link_state_fresh_2026-10-20.c),
[`playerhose_hotspot_state_machine_fresh_2026-10-20.c`](../tracker/evidence/scenario-loading/playerhose_hotspot_state_machine_fresh_2026-10-20.c),
and the earlier suppression exports in the same evidence directory.

## Catalog time field wired into rescue deadline input (2026-10-19)

Fresh read-only Ghidra 12.0.4 tracing reconfirmed the catalog path:
`thegame.lev` `time:` is multiplied by 60 into level record `+0x10`, and
Player initializer `0044D550` applies the current mode multiplier into Player
`+0x343C`. Caller tracing found three uses of deadline helper `004541A0`:
Player escape (`00451160`), a RescueBot prompt/deadline routine (`00454BA0`),
and a RescueBot update (`00455580`). The field is therefore a deadline input,
not a confirmed mission countdown. Mission timer HUD, expiry, results and
outro behavior remain untraced.

The rescue chooser now carries the selected record `+0x10` through the full
linked stage chain, replacing the fixed `600.0f` input in the shared rescue
adapter. The macOS rescue-all bundle rebuilt at SHA-256
`4deb4e9bc0ea82352bf2f0971270ce5fefd0c2898023811c0356e686056a60f4`; the
eight retail rows report their source-derived field values through
`--mission-catalog-check`. This is a controlled package check, not a natural
rescue, RescueBot cadence, mission-result, or retail-parity check. Details:
[`catalog_time_rescue_deadline_port_2026-10-19.md`](../tracker/evidence/runtime/catalog_time_rescue_deadline_port_2026-10-19.md).

The tracker audit also found 13 manual instruction-slice reviews recorded in
the reviewed-function count even though the active saved Ghidra project has no
function objects at those addresses. Their rows and evidence are preserved in
`tracker/manual_code_reviews.csv`; the reviewed-function history has been
reconciled against current Ghidra function boundaries. The function atlas and
dashboard were regenerated from the corrected CSVs. Current reviewed-function
count: 330. A fresh read-only `ExportFunctionInventory` output matched the
checked-in EXE inventory exactly: 6,111 total records, including 5,824
body-bearing functions and 287 external entries.

### Delegated audit ledger (2026-10-19 to 2026-10-20)

| Scope | Files / evidence reviewed | Status |
| --- | --- | --- |
| `completion_port_audit` | Rescue completion/exit traces and shared transaction path | Complete, read-only; confirmed the already-integrated victim release and final-exit adapter, while natural completion, result flow, and outro remain unresolved |
| `retail_exit_trace` | `00440CE0`, `00450A70`, `0042A260`, `0042AEA0`, `0042AB00`, linked-stage package evidence | Complete, read-only; static gates and stage loading are verified; authored-key/context identity and live transition timing remain open |
| `mission_gap_audit` | Mission completion and outcome paths in the queue/evidence and pinned EXE | Complete, read-only; selected terminal result flow as the next shared gap and cautioned against inventing a deadline/result path |
| `rescuebot_context_audit` | Player `+0x32CC` recurring RescueBot and `+0x340C` one-shot dispatch | Complete, read-only; confirms separate initialization and one-shot path bypasses recurring gate; context aliasing and live `runrescuebot` behavior remain open |
| `mission_result_state_trace` | Player `+0x3438` field uses and status/chatter callback | Complete, read-only; state-1 final-exit writers and visible win/loss dispatch mapped; no direct state-2 writer in the bounded instruction scan |
| `loss_state_producer` | Player `+0x3438`, `+0x343C`, and direct writer references | Complete, read-only; no direct state-2 writer or link from the deadline helper in the bounded scan; recommends a runtime write watchpoint if runtime tracing becomes available |
| `result_widget_assets` | Widget selectors and `rescue.rrf` / `hose.rrf` resource tables | Complete, read-only; identified win/loss textures under IDs 71/72 and inspected authored headlines; parent renderer trace confirms the selector-to-resource path |
| `result_widget_timing` | Result child constructors/update path and sibling timed-widget callers | Complete, read-only; result path directly calls `0048D390`/`0048D180`/`0048D1A0`; `0048D330` 1000/5000 defaults belong to other caller paths and cannot be transferred |
| `result_widget_init_trace` | `mission_outcome_widget_fresh_2026-10-19.c`, `mission_outcome_widget_callers_fresh_2026-10-20.c`, fresh manager/rotation export | Complete, read-only; confirmed white/hidden initialization, clock/active fields, post-duration viewport-centered rotation, and strict completion timing gate; duration/tail values, desktop placement and alpha remain open |
| `result_state_writer_audit` | Player `+0x3438` direct operand scan, deadline callers, mission outcome evidence | Complete, read-only; found no state-2 writer or deadline-to-loss path in the bounded scan; computed aliases and external/runtime producers remain unexcluded |
| `loss_state_indirect` | Retained `+0x3438` field evidence and documented static paths | Complete, read-only; no additional producer candidate in retained static artifacts; arbitrary aliases remain open, recommends a retail write watchpoint |
| `runrescuebot_trace` | `0047A1F0`, `00475020`, `00455230`, initializer and recurring RescueBot context | Complete, read-only; one-shot uses Player `+0x340C`, recurring callback uses `+0x32CC`; aliasing and one-shot completion servicing unresolved |
| `suppression_core_trace` | `004640F0`, `00463EF0`, `00448970`, HoseSpray offsets and shared viewer loadout | Complete, read-only; confirmed terminal HoseSpray finalize path and nonterminal extinguisher path; shared component collision writer and authored trigger are mapped; shared host ports selection/action state, stream collision, previous-frame contact, terminal dousing, effect cooldown/spawn request, and two-key particle track, with attachment frame, particle material/flags, size mapping, and retail cadence open |
| `touchhose_authored` | Preserved BSP/EVS strings, PropModel contact, named ScriptActor dispatch | Complete, read-only; verified 16 commands in 12 BSPs with full hashes; static authored route reaches PlayerRescue `touchhose`, while runtime cadence remains open |
| `hosespray_retail_path` | Current-hash HoseSpray constructor, update/contact order, slot-4 command path, and `touchhose N` | Complete, read-only; confirms mode 2 acquisition, mode 3 slot selection, mode 1 deactivation, separate action callbacks, prior-frame collision use and `finalize=1`; N scales an attachment-list record count by 24 and divides by owner `+0xCB0`; record purpose and input bindings unresolved |

## View bob fully removed (2026-10-18)

The user still saw bob after the partial fix below and explicitly requested
complete removal. Both playable hosts now use `ViewUpdate::directEye`: Player
position plus fixed standing/crouching eye height and mouse yaw/pitch. Normal
camera construction has no head-motion update, item animation offsets, or
follower output. The shared viewer also passes zero head motion to passout.
The separate Training host previously still applied the full movement bob;
that path now uses the same fixed eye camera. This is an intentional user
preference, rather than a claim of retail camera parity.

Both native targets compile. A temporary native harness exercised actual
`Player::step` and the camera helper on the exported flat room for eight
seconds at 30/60/120 Hz, standing and crouched: all six cases had zero camera
vertical range while walking. The three local preview/game app executables
were replaced atomically and match SHA-256
`9a046b691d6e19765fb21159918d96b48c0d07da7cb0003f79bd6892c1b0ff16`.
An already-running process must be closed and reopened to load the change.

## Earlier partial walking-camera fix (2026-10-18, superseded)

Comparing the current viewer against its committed version identified the
regression: the recent camera edit started routing ordinary walking and
crouching through `HeadMotion` and the thresholded view follower. Retail's
movement records have vertical amplitudes up to 10 units. The prior ordinary
camera used the Player's direct eye position and did not apply those offsets.
The normal path now passes zero head motion and uses the direct `ViewUpdate`
frame position, retaining pitch and held-item offsets but bypassing the
stateful follower. The passout path still uses the source-shaped bob and
follower. The rebuilt app executable matches the build output at SHA-256
`717cb02f64a27f59d7f47be6a024e53ece03bc5f77128cc72d722a85255947a7`;
`git diff --check` passes. No interactive walking capture was run, so the
user should confirm the next launch feels stable. This checkout has no Git
remote configured; the existing publishing handoff documents an automatic
review restriction, so this checkpoint remains local rather than routing
around that restriction.

## Current priority (2026-10-17)

The user asked to pause menu implementation and work on the other original
priorities. Keep the existing retail-menu changes intact, but do not spend the
next iteration on menu pages or selector parity. The user reports that
extinguisher use appears inert and the Axe animation swings toward the player.
Fresh Ghidra review separates the Axe root gate `+0x1FC` from each child's
model-selection flag: ordinary first equip leaves the root gate off and uses
secondary model 1 (`axe.act`); duplicate `equip axe` enables primary model 6
(`axel.act`) as well. The port had incorrectly enabled that gate on the first
equip and always drawn `axel.act`. This checkpoint corrects first-equip state,
model selection, and animation playback; the reported swing direction remains
open because the preview's guessed camera-space ActorInfo root has not been
joined numerically to retail's Player view follower. Both ACTs have positive
early local-Z track values, but each root attachment rotates the first sampled
translation mostly toward negative actor-local Z: approximately `(0.66, 10.89,
-11.62)` for default `axe.act`, and `(-0.60, 10.89, -11.62)` for `axel.act`.
The preview's raw camera-frame root maps actor-local `-Z` onto its view ray,
which points from the eye into the scene. The composed root translation
therefore moves away from the camera, not toward it; a prior note reversed
this sign. Root translation alone does not explain the reported toward-player
swing. The sampled rotations can still move individual vertices toward the
camera, so both signed per-vertex projections are now logged. The final
retail Player-follower-to-ActorInfo matrix is still not joined, so no axis
correction is justified. The signed projection correction and updated audit
fields are recorded in
[`axe_view_ray_sign_recheck_2026-10-17.md`](../tracker/evidence/runtime/axe_view_ray_sign_recheck_2026-10-17.md).
The shared build also applies source-derived Axe view/aim offsets and uses one blended extinguisher pose for
its visible ACT and fire contacts. Eligible
extinguisher contacts now create a two-second host spray quad using the
resource-14 texture pair declared by `rescue.rrf`; the manager link and exact
`geWorld_AddPolyOnce` presentation remain unresolved, and no interactive hit
has been confirmed. A packaged render diagnostic exposed a separate loader
failure: SDL rejects the retail 8-bit `bsmoke.bmp` because ten pixels use
undeclared palette indices 254/255. The asset exporter now converts both
sprites to valid 24-bit copies while leaving source files untouched; the two
missing smoke colors are padded black provisionally. A subsequent packaged
diagnostic loads the scene and all 24 textures, but it does not confirm
suppression. Fresh Axe tracing shows an identity component-local pose
parented to the Player view follower with no authored Axe hand offset. Retail
updates follower `+0x34A4` from the Player view frame at `+0x3460`; its target
vector uses positive Z, while the Genesis camera world-transform relation is
still unknown. Retail and port use the same component-frame-to-ActorInfo basis
packing; a transpose or global basis-order fix is not supported. The Genesis
attachment branch is now resolved: Axe BONE06 takes flag 0, which applies
attachment × sampled rotation and attachment-transformed translation; default
`axe.act` BONE01 also takes this branch. The shared `ActorPose` code matches
both. The remaining Axe gaps are the
camera-world-transform caller/numeric relation to Player view frame `+0x34A4`
and the viewer's guessed `(35, -8, 140)` origin, so do not mark the swing
report fixed or add a sign flip. The opt-in `--fire-camera-audit` also logs
equipment-slot selection and left-button down/up edges, so a future capture
can distinguish an input/selection failure from a missed contact. It logs the
camera pose after `ViewUpdate` (head-motion phase/offset, Player eye base,
follower start and final position, camera basis, and view ray). With Axe
selected it samples the exact default ACT root-bone pose and per-vertex
movement along that view ray through the current preview root. Extinguisher
samples now include delivered influence and the source suppression response
factor; the action frame includes all basis vectors. Capture is still needed
to establish whether the points hit a fire. For the
extinguisher, the retail `.006/.009/.006` values are follower smoothing rates;
transform scale is unit, so multiplying contact offsets by those values would
be incorrect. Current source review finds no deterministic input, queue, or
suppression drop. A fresh field-base check confirms the existing alpha port:
`UpdateFireRescueActor` receives the embedded actor at root `+0x24`, so actor
`+0x104` aliases root suppression strength `+0x128`, and actor `+0xE0` aliases
the root recently-sprayed byte `+0x104`. Retail and the port both double point
influence before the quadratic response, which produces nearly zero
suppression at a full-strength center contact; the formula is source-backed,
but a live capture is still needed to tell this response from a missed contact.
Fresh rendering traces also map the retail RGB override below strength 0.8: it
blends `(0.9, 0.3, 0.1)` toward white across all four flame children. The
shared renderer now applies that tint before recovery. Above the threshold,
the port still uses white instead of retail's animated child RGB tracks, so
pixel parity remains open. Raw pinned EXE globals confirm threshold `0.8` and
vertex scale `255.0`; see
[`visual-terminal-state.md`](../tracker/evidence/fire-spread/visual-terminal-state.md).
The four points may still miss in a live scene because the contact frame's
camera-space relation is provisional. Do not mark the extinguisher report
fixed. A global camera correction could change map orientation, extinguisher
contacts, goggles, and all held tools; a per-tool change could alter facing and
swing path. The latest trace confirms the Player frame stores target-minus-
position in `+Z`, while its screen meaning remains unresolved against Genesis
camera world `+Z`/projection `-cameraZ` and the viewer's OpenGL view transform.
The Player-follower-to-camera-to-ActorInfo matrix join is missing. Do not flip
the frame yet; trace that join before changing Axe axes or extinguisher
geometry. The catalog
level-music path is now wired, including inherited tracks for linked stages
and the oxygen volume change. Menu-track playback remains paused with menu
work. Next priorities are retail rendering and mission lifecycle. See
[`equipment_action_pose_followup_2026-10-17.md`](../tracker/evidence/scenario-loading/equipment_action_pose_followup_2026-10-17.md).
The particle trace and current renderer limits are recorded in
[`extinguisher_particle_render_trace_2026-10-17.md`](../tracker/evidence/scenario-loading/extinguisher_particle_render_trace_2026-10-17.md).
The follower and Genesis camera trace is in
[`equipment_view_frame_trace_2026-10-17.md`](../tracker/evidence/scenario-loading/equipment_view_frame_trace_2026-10-17.md).
The newly traced retail camera-to-ActorInfo consumers are in
[`retail_camera_actorinfo_context_trace_2026-10-17.md`](../tracker/evidence/runtime/retail_camera_actorinfo_context_trace_2026-10-17.md);
they still do not connect the camera context's parent hierarchy to Player
`+0x34A4`, so they support neither a global camera-axis flip nor a per-tool Axe
turn.

The follow-up constructor/caller trace narrows that gap: `00493AF0` creates a
camera node parented to the scene/root object, while `00491EA0` receives its
context from a camera descriptor. Player `0044F340` refreshes `+0x34A4` before
updating a type-5 context record, but no pointer or descriptor assignment
joins that record's `+0x40` parent chain to `+0x34A4`. A matched live matrix is
still required before changing Axe axes or extinguisher contact direction.
The source-shaped extinguisher math also makes a near-center-only hit weak:
raw influence near `1` is doubled, then the fire applies
`1-(1-influence)^2`, which approaches zero at delivered influence `2`. The
other three sample points may still suppress the fire; their live eligibility
and influence remain unknown. The stale Axe pose loop is fixed in source, but
no current interactive capture confirms the user's remaining stationary
camera report. Retail idle bob is two vertical units; substantially larger
motion remains unresolved.

The follow-up check of the user's bob/look report found no source-backed
movement-gate mismatch: retail `00452570` compares the digital movement vector
against binary64 `1e-7` at `0x004E1C10`, uses strafe weight `0.75`, and calls
`0044F340` with active byte 1; the viewer uses the same values. Retail's
directly applied head-motion records themselves reach +10/−5 vertical units.
Do not rescale these records without a same-view retail runtime capture. The
camera follower's runtime parent/matrix is still not joined to Genesis's camera
context, so its visible bob remains a parity gap. The pitch sign is consistent
with the renderer's backward `+Z` basis and visible `-Z` ray, but the user's
mouse report needs an interactive check before it can be called resolved.
The Axe swing is bounded by the ACT swing end time in the current source, and
the rebuilt bundle contains that correction; user-perceived repeat behavior
still needs an interactive check. The extinguisher path has no deterministic
static drop, but contact placement still needs a `--fire-camera-audit` capture.
See
[`first_person_camera_axe_regression_2026-10-17.md`](../tracker/evidence/runtime/first_person_camera_axe_regression_2026-10-17.md).

### Focused report recheck (2026-10-17)

Fresh headless Ghidra work against EXE SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
reconfirmed the camera consumer: vtable slot `004E3B50` points to
`00491C60`, which supplies the stored Genesis camera and node transform to
`geCamera_SetWorldSpaceXForm`. The transform parent/context chain still has no
static or numeric join to Player view follower `+0x34A4`. The pseudocode is in
[`retail_camera_context_trace_2026-10-17.c`](../tracker/evidence/runtime/retail_camera_context_trace_2026-10-17.c).
The no-input port path contains only the source-authored ±2-unit idle breath
after the stale Axe pose-loop fix; no further large-jostle cause is
source-confirmed. Axe direction still depends on the unrecovered camera to
ActorInfo matrix and guessed `(35, -8, 140)` origin. Extinguisher routing has
no deterministic static drop, while center-only contacts can be weak under
the retail equation. A fresh trace confirms the extinguisher local frame's
direction negation and basis math match the port; its parent/world matrix is
still provisional. No live camera/equipment audit log was available, so do
not mark any of the three user reports fixed or add a global/per-tool sign
change. Continue from the audit log when captured; retain menu work paused.

### Current delegated audit ledger (2026-10-17)

| Scope | Files / evidence reviewed | Status |
| --- | --- | --- |
| `bob_scale_audit` | `head_motion.h`, `scene_viewer.cpp`, retail `00452570`/`0044F340`/`00491140`/`00491460` | Complete, read-only; authored amplitudes and movement gate align; runtime scale unresolved |
| `extinguisher_gap_next` | `scene_viewer.cpp`, retail `00461830`/`004614E0`/`00461590`/`00461F80`/`00481A60`/`00449A40`/`00448970` | Complete, read-only; no deterministic drop; contact placement still needs capture |
| `axe_camera_basis` | Retail `00491C60`/`00491D70`/`00491D00`/`00495450`/`00492110`/`00492450`, Genesis camera API | Complete, read-only; camera and held-item matrix consumers traced; source hierarchy join still missing |
| `idle_bob_trace` | `core.h`, `head_motion.h`, `view_update.h`, `scene_viewer.cpp`, prior idle-fall evidence | Complete, read-only; no persistent no-input physics oscillation; found Axe item-pose clock can keep cycling after its finite ACT |
| `camera_idle_trace` | `scene_viewer.cpp`, `HeadMotion`, `ViewUpdate`, retail `00491140`/`00452570` | Complete, read-only; stale Axe pose loop is fixed in source; retail stationary idle is ±2 vertical units; remaining live camera scale/context unverified |
| `axe_orientation_trace` | Ghidra callers/constructors `00491D70`/`00493AF0`/`004956C0`/`00498AC0`/`00493BD0`/`00491EA0`, Player `0044F340` | Complete, read-only; type-5 context helper is parented to the global scene owner, with no static pointer join to Player follower `+0x34A4`; preview's raw root maps the composed negative local-Z swing toward its view ray; retail ActorInfo link remains unresolved |
| `camera_context_factory_followup` | Type-5 factory wrappers `00446D10`/`00464A20`, constructors `00446C40`/`00464660`, base `0049CBE0`/`0049D420`, context `004956C0`/`00493AF0`/`00495450` | Complete, read-only; both type-5 records create a helper camera context under the global scene owner; no link to Player follower found; owner ancestry/runtime identity remain open |
| `extinguisher_effect_trace` | `ExtinguisherPose`, four-point sampler, queue/message 30000, fire contact and suppression | Complete, read-only; no deterministic drop; near-center influence can be doubled near 2 and yield near-zero retail response; live sample-to-fire intersection remains unknown |
| `camera_trace_followup` | `head_motion.h`, `view_update.h`, `core.h`, post-ViewUpdate camera audit, retail camera context trace | Complete, read-only; no additional no-input oscillator found; idle breath is ±2 units; larger motion needs a live sample |
| `axe_trace_followup` | retail camera/ActorInfo pseudocode, `equipment_view_frame_trace_2026-10-17.md`, `actor_pose.h` | Complete, read-only; no supported global or per-Axe rotation fix; numeric camera-to-ActorInfo join remains open |
| `extinguisher_trace_followup` | `scene_viewer.cpp`, `extinguisher_pose.h`, `extinguisher_samples.h`, retail `004612B0` and vector helpers | Complete, read-only; local frame math matches port, no deterministic route loss; parent matrix and live intersections unknown |
| `camera_axe_extinguisher_reaudit` | camera/Axe source, retail camera-to-ActorInfo trace, extinguisher contact/suppression path | Complete; confirmed Axe root translation projects away from the camera along the view ray; corrected audit labels to report both signs; stationary camera reset is in the packaged build; extinguisher still needs live contact evidence |

### Current stopping checkpoint (2026-10-17)

The current rescue-all macOS app was rebuilt with signed Axe depth diagnostics;
the build-tree and packaged executables match at SHA-256
`a5d3d4b169aa4389d2f300ce162b115645604485729f492eb86ef4f4a7093ac9`. The
bundle includes the earlier stale-Axe-pose reset, but it has not been run
interactively here. No gameplay audit log exists in the checked temporary
locations; the saved `fdny-camera-audit*.log` files are Ghidra analysis logs,
not player captures.

Retail idle breathing is the only source-backed stationary camera oscillator
found (±2 world units). The old repeated Axe item-pose clock is cleared before
the next camera update in source and included in this package. If the camera
still moves substantially after the player settles, the next evidence is a
`camera-pose` capture from `--fire-camera-audit`.

The Axe root translation projects away from the camera into the scene; the
sampled rotations and whole-mesh displacement remain unmeasured in a live run.
No global or per-tool orientation change is justified without the numeric
retail view-follower-to-ActorInfo matrix. The extinguisher's local frame,
contact dispatch, and suppression formula match the traced retail path, but
live sample/fire intersection has not been captured. Those gaps affect whether
the current symptoms can be attributed to the guessed preview frame or need a
source-backed correction. Menu work remains paused.

## Immediate regression follow-up (2026-10-17)

The user reported a continuously swinging Axe, excessive view bob, and inverted
vertical mouse look. The one-second `ItemViewPose` clock was being reused for
ACT swing playback even though retail `axe.act`/`axel.act` swing tracks last
about 3/2 seconds; the Axe clock is now monotonic and separate, so the ACT can
reach its endpoint. The newly shared normal camera frame applied pitch with
the wrong sign; the view roll now uses negative input pitch. Head-motion
phases now initialize to the retail constructor's zero values. The source
records themselves contain 10-unit movement amplitudes; do not scale them
without a retail runtime comparison. The macOS rescue-all bundle builds at
matching executable SHA-256
`669bd58dcf34e0f534954c92f4c49e5dd4822c16543ec3b26678eb3d174a45ce`.
Interactive feel and retail visual parity remain unverified; the Axe
camera-to-ActorInfo attachment and its guessed camera-relative origin remain
open. See
[`first_person_camera_axe_regression_2026-10-17.md`](../tracker/evidence/runtime/first_person_camera_axe_regression_2026-10-17.md).

## Stationary camera loop follow-up (2026-10-17)

The latest stationary-camera report exposed a separate Axe lifecycle bug. The
viewer continued `ItemViewPose::update` after the finite Axe ACT ended because
`axeActionStarted` stayed true; the one-second pose clock wrapped and its
hand offsets cycled through `ViewUpdate`. The port now clears the action,
strike-pending flag, and pose on the first frame after `axeSwingActive` ends,
before composing that frame's camera. This is a concrete port-side camera
input fix, built in the rescue-all macOS app at matching executable SHA-256
`761b4b044d47ca46c083e37fa2a3461a11b3ce0b2111c0e8ed402358435b5d43`.
`git diff --check` passes. The idle `HeadMotion` mode still has the
retail-authored 2-unit vertical breathing cycle. The spawn still falls to the
floor rather than being snapped initially, and no gameplay capture has
confirmed the user's remaining camera motion. Exact Axe held/release callback
parity and retail camera hierarchy remain open. See
[`first_person_camera_axe_regression_2026-10-17.md`](../tracker/evidence/runtime/first_person_camera_axe_regression_2026-10-17.md).

## Camera, Axe direction, and extinguisher audit extension (2026-10-17)

Corrected the earlier translation sign reading: the ACT root attachment moves
the first sampled Axe translation toward actor-local `-Z`; the preview's raw
camera root maps that axis onto the visible view ray. This makes the guessed
preview root a plausible cause of the reported swing direction, while the
retail camera-to-ActorInfo matrix remains unresolved. Fresh Ghidra tracing of
both type-5 factories shows their shared helper camera context under the global
scene-owner hierarchy; no static join to Player follower `+0x34A4` was found,
and the global owner's ancestry/runtime identity remain open.

`--fire-camera-audit` now samples the post-`ViewUpdate` camera and head-motion
state, then measures the default Axe ACT root pose and each ACT vertex's
displacement along the candidate view ray. Extinguisher lines include the full
contact frame, doubled message influence, and expected response under the
unchanged retail equation. These measurements are opt-in and add no gameplay
correction. The rescue-all macOS app rebuilt successfully; build-tree and
packaged executable SHA-256 are both
`ea5f1bd5d5fb298ca2256be4ec21480edbf07e033480b555c732dda570de9349`.
`git diff --check` passes. No interactive or retail-parity capture has been
run; the remaining camera jostle, swing direction, and extinguisher contact
are still open. See
[`first_person_camera_axe_regression_2026-10-17.md`](../tracker/evidence/runtime/first_person_camera_axe_regression_2026-10-17.md)
and
[`extinguisher_scene_viewer_runtime.md`](../tracker/evidence/scenario-loading/extinguisher_scene_viewer_runtime.md).

## Active glitch investigation (2026-10-14)

The user clarified that the dirty-space investigation concerns the original
retail game and should prioritize Player/Axe coordinate handling in invalid or
out-of-bounds space; treat the swing as a condition that may expose the fault,
not its presumed gameplay cause.
`0043F810` can write a downward-sweep collision hit
directly to Player transform translation `+0x4C..+0x54`, then mirror it into
the reused `+0x68` sweep cache. An eligible collision response can set Player
latch `+0x12C`; the physics integrator copies it to `+0x12D` and clears it. If the
Player's authored `escape` command runs while `+0x12C` is set and oxygen is
above `0.5`, `004513A0` copies current translation into checkpoint `+0x534`,
which a later reset restores. This is a plausible collision-derived position
restore path, but the event ordering and incident match remain open. Keep this
checkpoint route secondary unless a reproduction shows the Player position
changing through it. There is no source-confirmed assignment from the Axe's
stack-local hit point to Player translation or latch.

When the Axe's primary child is active, a successful world hit detaches its
transform before candidate scanning; a no-candidate hit can retain the cached
visual transform until conditional recovery. The primary child is controlled
by root gate `+0x1FC`, initialized to zero. Ordinary first-time selection
activates only secondary model 1 (`axe.act`). The only direct caller found for
the primary activation gate is the existing-Axe/duplicate `equip axe` branch
in `0044D080`/`0044D152`; Boat A/B startup scripts each author one Axe equip.
After that duplicate path, the detached model-6 primary and Player-attached
secondary can both remain render-eligible at different transforms. This is a
conditional stale/duplicate-looking Axe path, not proof of a Player warp or
the gate state during the Boat incident.
A correction to the earlier passout summary: `0044FF10` first
copies Player snapshot `+0x3400` into the transform, but its tail calls
`0044CD10`, which overwrites the final position from checkpoint `+0x534` and
then applies floor placement. The oxygen-gated snapshot may be used for a
carried-victim drop, but is not the established final passout destination.
Boat B's climbable hook and adjacent ladder have BSP leaves intersecting Player
mask `0x43` outside world-model-0 bounds. Opening/updating their BModels changes
open state and transforms but does not set Genesis's separate `+0xCC` collision
flags; their runtime eligibility remains unknown. The Axe sweep endpoints are
derived from Player frame `+0x34A4`, so invalid Player coordinates could also
place the Axe query in invalid space. Boat A has an office Axe-target cluster
and outlying non-Player-solid propeller-zone brushes. The precise Boat landmark,
action, and destination are unknown. The next useful evidence is a retail
capture of Player translation/latches, checkpoint, Axe frame/parent, and the
winning collision model through a fall, swing, escape event, and any later
reset. A separate retail map scan finds mask-relevant world-model-0 solid leaf
volumes reaching a nominal 128-unit envelope outside reported model bounds on
MBase, Boat A/B, and Gas R. Same-family public GBSPLib source deliberately
builds outside solid leaves with `BSP_CONTENTS_SOLID2`; its padded root bounds
are for frustum culling and do not alone cause retail collision. The retail
model-bound broadphase still must overlap a sweep that traverses those leaf
planes. The Player callback can write an accepted downward-sweep hit directly
to translation, making the edge geometry a shared coordinate-path candidate,
but neither source nor compiler evidence matches it to the incident. Boat B's
hook/ladder remain additional conditional submodel candidates. The current
port has no lower-world-bound recovery: if its sweep finds no solid, Player
position continues integrating and the Axe query follows the post-step camera
position. That query result does not write Player position; the rendered Axe
is camera-relative. Record retail root/frame coordinates and winning
collision model before changing behavior. See
[`dirty_space_axe_warp_trace_2026-10-14.md`](../tracker/evidence/scenario-loading/dirty_space_axe_warp_trace_2026-10-14.md).
Do not implement a behavioral workaround until the position write that matches
the report is observed.

## Delegated audit ledger

| Agent | Scope | Files | Status |
|---|---|---|---|
| `axe_collision_research` | Read-only Axe sweep, candidate list, and model-transform audit for dirty-space copies | `dirty_space_axe_warp_trace_2026-10-14.md`; pinned Ghidra exports | Complete; hit point is stack-local; a successful world hit detaches the Axe transform, and a no-candidate path can retain its cached frame until conditional recovery |
| `axe_visual_origin_trace` | Trace whether separate Axe ACT instances can remain visible after a world hit | `00460A50`, `0045F9B0`, `0045F9F0`, render-path audit, pinned retail binary | Complete; model-6 primary requires duplicate-equip gate path, absent from Boat startup scripts; if enabled, a detached primary can retain its transform while secondary stays attached; runtime state and any Player warp remain unverified |
| `axe_candidate_lifecycle` | Trace candidate gates, target handlers, and possible Player/target transform writes | `00460B90`, `0043E2A0`, `0043D660`, Boat A/B EVS exports | Complete; `+0x38` is `G_Liftable`; no target parenting or Player write occurs in the Axe loop, and reviewed Boat Axe-eligible BModels have no authored `chop`/`break` handlers; dynamic script attachment remains open |
| `axe_gate_callers` | Find callers and ordinary equip paths for root Axe model-6 activation gate | `00460A50`, `0045F9B0`, `0045F9F0`, `0044D080`, Boat A/B startup EVS | Complete; ordinary first selection activates only model 1; only direct gate caller is duplicate `equip axe` path, absent from Boat startup scripts; runtime gate remains unknown |
| `bsp_padding_origin` | Trace compiler origin of outside solid leaves and compare retail map bounds | Pinned retail BSP exports; public same-family GBSPLib source at commit `83a4d532` | Complete; outside node is built SOLID2 with nominal ±128 padding and collision sides are generated for SOLID_CLIP leaves; public header uses root padding for frustum culling, so this does not alone establish a retail collision defect |
| `player_update_dispatch_order` | Verify Player collision, movement integration, and latch clearing order | `00452570`, `0043F6D0`, `0043F680`, Player scheduling audit | Complete; collision precedes movement-delta integration within `00452570`, but the recurring order against latch clearing and ScriptActor events remains unknown |
| `player_warp_research` | Read-only Player movement collision and stale-coordinate audit | `dirty_space_axe_warp_trace_2026-10-14.md`; pinned vtable bytes and Ghidra exports | Complete; `0043F810` floor-hit branch writes directly to canonical transform translation, then refreshes the origin cache |
| `player_origin_writers` | Cross-reference Player origin/cache writers, saved-position restore, and transform synchronization | `dirty_space_axe_warp_trace_2026-10-14.md`; pinned executable and saved read-only Ghidra project | Complete; `0043F810` writes a downward-sweep hit to canonical translation; `0044FF10` temporarily copies `+0x3400`, then `0044CD10` applies checkpoint `+0x534` and floor placement; incident match remains open |
| `player_collision_checkpoint_latch` | Trace Player collision latch and checkpoint writes against dirty-space coordinate hypothesis | `0049E0A0`, `0049DBC0`, `004513A0`, `00451870`; hash-pinned saved Ghidra exports | Complete; collision sets `+0x12C`, integrator copies/clears it, and explicit `escape` can capture current translation as checkpoint; incident and event ordering remain open |
| `boat_bsp_audit` | Crosswalk Boat A/B triggers, rescue actors, and outlying submodel geometry against dirty-space candidates | Boat entity/script/collision exports; dirty-space evidence note | Complete; Boat B hook model 13 extends 416 units above world-model-0 bounds and has 31 total mask-intersecting leaves, but only one outside-bound leaf is mask-relevant; runtime flag/transform and repro match remain open |
| `mbase_oob_collision` | Compare MBase out-of-bounds BSP models against Boat B hook lead | Ignored MBase/Boat B collision and entity exports; source binary hashes recorded | Complete; no named non-world Player-solid submodel outside bounds; world-model mask-solids reach the nominal ±128 envelope, consistent with same-family compiler outside-solid construction; `door19` near `fire7` remains an in-bounds Axe-target lead |
| `gas_oob_collision` | Compare Gas R out-of-bounds BSP models and Axe-target clusters against Boat B | Ignored Gas R/Boat B collision, entity, geometry exports; source hashes checked | Complete; no named non-world solid Gas R model outside bounds; world-model solids reach the nominal ±128 envelope, consistent with same-family compiler outside-solid construction; `Pump8`'s separate outside leaf has contents 0; no tight fire/victim to positive-hitpoint BModel cluster |
| `world_edge_leaf_runtime` | Independently verify world-model edge leaf reachability, hull broadphase, and cross-map envelopes | Pinned Genesis.dll pseudocode; ignored collision exports with BSP hashes checked | Complete; all four BSP root bounds and mask-solid leaves reach a nominal ±128 envelope with small float differences; padded root bounds are not themselves the collision cause, and outside solid leaves may be intentional builder output |
| `boat_model_collision_flags` | Trace Boat B hook/ladder model collision flag lifecycle and touch/open path | `dirty_space_axe_warp_trace_2026-10-14.md`; saved hash-pinned Ghidra exports, Boat B entity/collision exports | Complete; BModel open/update uses `geWorld_OpenModel` for separate +0xC8 state, not `geWorld_ModelSetFlags` at +0xCC; static candidates remain conditional pending live flags or a collision hit |
| `axe_pose_sound_audit` | Retail Axe facing, swing direction, and sound playback trigger | Read-only saved Ghidra exports and copied-EXE disassembly | Complete; retail registers AxeSwing/AxeHitWood/AxeHitOther/WoodDoorBreak, but the playback trigger and final camera-space pose remain open |
| `ext_hud_damage_audit` | Training extinguisher pickup, retail hint window, and damage feedback | Read-only pinned executable audit and current viewer source | Complete; `ext1` is a touch PropModel whose event drain remains open; retail hint font/render path is traced and ported; generic damage overlay remains unresolved |
| `retail_hint_gui_render_trace` | Trace the retail Player hint widget's render callback, text assets, layout, and styling | `tracker/evidence/scenario-loading/player_hint_widget_render_trace.md`; hash-pinned headless Ghidra | Complete; resource 1020, 16x24 glyphs, color markup, and strict timeout are verified and the atlas is ported; mode-2 layout, tint layers, wrapping, and tick rollover remain open |
| `retail_frame_pacing` | Trace retail delta-time consumers, frame pump, and pacing controls | `tracker/evidence/runtime/retail_frame_pacing_audit_2026-10-14.md`; read-only headless Ghidra audit | Complete; delta consumers and queue-empty update/render pump are mapped; delta writer and frame limiter remain unresolved |
| `propmodel_retail_schedule` | Resolve PropModel contact callback, pair drain, and owner cadence | `tracker/evidence/scenario-loading/retail_propmodel_contact_trace.md`; headless Ghidra follow-up | Complete; producer, visibility gate, pair delta drain, and `ext1` join are verified; external updater caller/cadence remain open |
| `gebm_truecolor_trace` | Verify embedded ACT GeBm true-color and color-key layouts | Read-only copied ACT inventory and Genesis.dll trace | Complete; referenced rescue actors include 24-bit BGR and color-key formats; decoder scope is recorded in the Genesis render note |
| `propmodel_port_review` | Review shared PropModel actor/contact integration and trace its VISDATA/PVS gate | Read-only review of exporter, `scene_viewer.cpp`, PropModel headers, and copied BSP data | Complete; lower-level cluster-PVS rejection is integrated; exact runtime area-matrix/current-view semantics remain open |
| `rescue_progression_audit` | Re-audit victim alternate spots, rescue completion, and exit progression for the next shared gameplay seam | Read-only hash-pinned retail exports and authored rescue packages; `rescue_completion_and_exit_path.md`, `victim_spawn_factory_and_order.md` | Complete; all 45 alternate-spot joins resolve and the `escape`/final-exit path is source-mapped; full contact-driven progression remains the next implementation target |
| `bank_live_hotpath_audit` | Independent Bank frame-loop/render/collision hotspot audit | Read-only current source and authored Bank export | Complete; identified 497 unconditional mesh submissions and per-fire render work; mesh frustum culling and cached fire frame durations are integrated, while live frame-time ranking remains open |
| `equipment_draw_trace` | Retail axe/extinguisher draw path, action and ACT motion mapping | Read-only Ghidra audit; `.local/ghidra/equipment-audit.log` | Complete; model draw roots, actor scale and exact axe motion link remain open |
| `fire_damage_live_path` | Why the shared viewer may not enter retail fire exposure | Read-only source audit against saved hash-pinned Ghidra outputs | Complete; ordinary frame loop calls the handler; contact values/cadence and live damage remain unverified |
| `fire_gameplay_audit` | Find a high-impact shared fire gameplay gap | `scene_viewer.cpp`, fire-spread helpers, `00448320`, `004483A0`, `00429520`, `0049BD20` | Complete; retail creates spread children after contact traversal, so the viewer now holds the current Player exposure candidate list to its pre-spread size; live cadence and geometry inputs remain open |
| `victim_gameplay_audit` | Find a high-impact end-to-end victim gameplay gap | `scene_viewer.cpp`, `victim_pickup.h`, `victim_pickup_message_path.md`, PlayerRescue getter evidence | Complete; current-hash getter disassembly confirms Player world-origin alignment with the host movement origin; callback cadence remains unresolved; no unsupported carry pose or origin adjustment was added |
| `equipment_draw_gap` | Current ACT camera placement and retail draw transform | Read-only source audit against saved Ghidra notes | Complete; found current roots behind/below the frustum; moved preview roots into camera-relative view; no live Ghidra instance was available |
| `trigger_volume_render` | Current red BoundsTrigger report, fog separation and render gating | `tracker/evidence/runtime/scene_preview_collision_texture_diagnostics.md` | Complete; current source hides zero-flag trigger models and the Training app was rebuilt; interactive relaunch confirmation remains |
| `axe_action_path` | Retail Axe input, child state and strike caller audit | `axe-strike-and-bmodel-hit-audit.md`, `axe-child-think-audit.md`, Axe port headers | Complete; shared sweep and hit leaf are integrated; child callback cadence and runtime list semantics remain open |
| `axe_strike_integration_followup` | Reassess safe shared-viewer Axe hit integration against saved strike/collision/BModel traces | `00460b90_FUN_00460b90.c`, `00492ce0_collision_wrapper.c`, `0043e2a0_FUN_0043e2a0.c`, Axe port headers | Complete; AABB, mask, hit gate, and event path are integrated; Player frame mapping, live candidate list order, and hit effects remain provisional/open |
| `axe_hit_integration_audit` | Check current Axe integration blockers and authored breakables | Read-only review of current viewer, strike/BModel Ghidra notes, Training data | Complete; first-pass sweep is viable; exact presentation pose, callback cadence, and runtime candidate order still need live comparison |
| `axe_failure_review` | Recheck reported backward pose and inert swing against current ACT and retail traces | Read-only current source, ACT export, saved Ghidra exports; no edits | Complete; corrected timer gate to prior `<0.275` and new `>0.275` crossing; no source-backed facing flip, retail matrix remains open |
| `rescue_shared_gap_audit` | Find a high-value shared rescue mechanic to continue after Axe; trace retail quake consumer | Read-only Ghidra audit and shared port review | Complete; CameraRescue's quake vector owner/updater is verified, but its connection to the rendered geCamera transform remains open |
| `finalexitloadmap_context_trace` | Determine whether the validated stage key reaches the pending loader unchanged | `00451870_player_script_command_handler.c`, `00450a70_player_finalexitloadmap_fresh.c`, `0042a260_set_pending_level_request_fresh.c`, `0047a1f0_script_command_dispatch.c` | Complete; viewer follows the authored key after the retail remaining-victim gate; checked key vs forwarded context equality and live transition timing remain open |
| `hud_goggles_trace` | Current hint/HUD and goggles visor/scan path | `scene_viewer.cpp`, `hint_overlay.h`, `goggles_renderer.h`, `goggles_scan.h`, `0044ee50_player_hint_route_fresh.c`, `004564c0_hint_window_constructor_fresh.c`, `00462a60_goggles_visor_setup_fresh.c` | Complete; render paths are in the packaged Training binary; live pixel visibility and target effect attachment remain unverified |
| `fire_damage_trace` | Whether Player fire exposure runs in ordinary viewer frames | `scene_viewer.cpp`, `player_fire_contact.h`, `player_fire_message.h`, `00451640_fire_exposure_trace.asm`, `docs/REVERSE_ENGINEERING_LOG.md` | Complete; handler is called per rendered frame; provisional spatial inputs and callback cadence leave user report unexplained |
| `player_fire_turn` | Retail `0044BB00` fire-facing side effects and HUD consumers | Read-only hash-pinned EXE instruction audit; `00452570`/`00456090` saved exports | Complete; retail sets four transient sector bytes and never changes camera yaw; exact sector map recovered |
| `held_item_axis` | Axe/ext ACT local axes and retail follower parent evidence | Read-only ACT/root audit; `actor_pose.h`, saved `0045F660` and item-view notes | Complete; direct roots and the half-turn candidate are both unverified against retail; exact retail draw root remains open |
| `held_item_draw_trace` | Inventory constructor/setup/follower chain for axe and extinguisher | Hash-pinned Ghidra headless caller/decompilation audit; `tracker/evidence/scenario-loading/held_item_orientation_audit_2026-10-13.md` | Complete; `0045F660` parent behavior is verified for setup callers, but its link to the visible inventory ACT draw root is open |
| `equipment_actor_render_path` | ActorInfo creation, special-context render callback, and pose root handoff | Hash-pinned local disassembly plus Ghidra headless follow-up; `tracker/evidence/scenario-loading/equipment_actor_render_path_2026-10-13.md` | Complete; `00497F70` → `00492110` matrix writer and `00492450` pose consumer are mapped; exact parent/source frame and per-item orientation remain open |
| `fire_contact_camera_audit` | Current yaw writes, directional-angle convention, and cooling order | Read-only source and hash-pinned retail audit | Complete; no fire path writes camera yaw; interactive response still needs the user's run |
| `retail_held_axes` | Retail view/follower frame and axe/ext ACT draw transform | Read-only saved Ghidra/ACT audit; `held_item_orientation_audit_2026-10-13.md` | Complete; selected-item view frame and follower parent are verified, visible ACT root and final axis remain open |
| `camera_fire_reaudit` | Recheck every camera/player heading writer around fire exposure | Read-only source and pinned retail audit; `current_feedback_audit_2026-10-13.md` | Complete; fire handler receives yaw by value and no fire path writes camera yaw; SDL mouse deltas remain the runtime check |
| `camera_fire_live_gap` | Recheck fire-contact autorotation against current input and movement writers | Read-only viewer and pinned retail audit | Complete; yaw/pitch only change from SDL relative-mouse events; retail state 6 intentionally blends view for 1 s after passout; live event/position comparison remains needed |
| `held_item_matrix_xref` | Continue retail ACT orientation trace through ActorInfo matrix writer | Read-only Ghidra headless on temporary project copy; `equipment_actor_render_path_2026-10-13.md` | Complete; mode-2 path writes ActorInfo matrix through `00497F70`/`00492110`; exact source frame and per-item matrix values remain open |
| `victim_ai_gap` | Check whether the roam sweep bounds are truly model-specific | Read-only pinned Ghidra evidence and startup sources | Complete; correction: retail bounds are fixed at ±24, matching the viewer; no change needed |
| `retail_held_item_orientation` | Recheck axe/ext first-person transforms against retail | `tracker/evidence/scenario-loading/held_item_and_fire_camera_followup_2026-10-14.md`; saved Ghidra decompilation; read-only | Complete; retail attached-pose writer/consumer is mapped, but exact camera-relative pose remains open; no source-backed root correction justified |
| `fire_camera_rotation` | Recheck yaw/pitch writers and first-contact fire diagnostic | Same follow-up note; viewer source and pinned retail fire exports; read-only | Complete; fire path does not rotate retail or viewer camera; audit now records mouse deltas on the exposure-entry frame |
| `held_variant_render_gate` | Retail Axe/axel resource selection and child render gates | Read-only Ghidra decompilation; `.local/ghidra/axe-pair-audit-runtime.log`; follow-up evidence note | Superseded by 2026-10-17 reconciliation: root and child `+0x1FC` fields were conflated; ordinary first equip displays model 1 `axe.act`, while duplicate equip can enable model 6 `axel.act` |
| `fire_camera_input_reaudit` | Recheck camera writers and separate heading from translation/pain feedback | Read-only viewer and hash-pinned retail exports; follow-up evidence note | Complete; fire has no yaw/pitch writer; opt-in audit now samples Player motion and camera position as well as mouse events; live capture remains needed |
| `retail_held_pose_orientation` | Determine whether held ACT roots or retail matrices explain backward-facing tools | Read-only pinned Ghidra and ACT/root audit; `tracker/evidence/scenario-loading/held_item_and_fire_camera_followup_2026-10-14.md` | Complete; port camera roots are confirmed approximations; exact retail root remains open; ordinary Axe model selection corrected to model 1 in the 2026-10-17 follow-up |
| `viewer_camera_rotation_cause` | Trace camera angle writes, SDL relative input, and fire/pain view effects | Read-only viewer and pinned retail audit; same follow-up evidence note | Complete; no fire yaw/pitch writer; next live run needs mouse deltas plus SDL focus/relative-mode state |
| `hand_orientation_audit` | Recheck current held-item facing against retail matrix evidence | Read-only source/ACT audit; `tracker/evidence/scenario-loading/held_item_and_fire_camera_followup_2026-10-14.md` | Complete; direct roots remain provisional and no axis change is source-supported; retail comparison needed |
| `fire_camera_audit` | Recheck current fire-contact camera/input path | Read-only viewer and pinned retail audit; same follow-up evidence note | Complete; only captured SDL motion writes yaw/pitch; no live input trace to explain the reported turn |
| `held_item_retail_transform` | Re-audit why Axe/ext models appear backward; determine whether current axis/root correction is retail-supported | Read-only hash-pinned Ghidra evidence and current viewer source | Complete; ordinary first equip uses model 1; live camera-space ActorInfo matrix is unresolved, so no axis flip or turn is source-justified |
| `fire_camera_writer_audit` | Re-audit all yaw/pitch writers and fire/pain effects in the viewer and retail trace | Read-only current source and saved hash-pinned Ghidra evidence | Complete; SDL relative motion is the only viewer yaw/pitch writer and fire indicators do not rotate retail camera; live input/framing trace still needed |
| `fire_exposure_crosswalk` | Compare viewer exposure candidate limit to retail FireRescue bookkeeping | Read-only saved hash-pinned `004483A0` decompilation and current source | Complete; retail limit belongs to each FireRescue instance; removed viewer-wide frame cap; single-fire damage inputs/cadence remain provisional |
| `rescue_catalog_path` | Identify broad rescue selection/transition seam without inventing stage keys | Read-only package inventory, loader, and finalexitloadmap exports | Complete; chooser and stage sibling packages are wired from catalog/authored keys; retail context equality remains unproven |
| `trigger_visuals` | Recheck red trigger brushes, fog, and Oil Rig B entity 78 | `trigger_brush_render_visibility.md`, `fog_helper_to_driver_path.md`, saved `00441040`/`00440CE0` exports | Complete; red brush gate and fog state/render path are ported; fog startup tuple, exact cadence, and pixel parity remain open; missing-model entity 78 cannot contact through the traced path |
| `held_tool_retail` | Recheck axe/extinguisher facing against retail matrix path | `held_item_and_fire_camera_followup_2026-10-14.md`, saved Ghidra/ACT notes | Complete; active Axe resource is verified, but camera-space matrix and hand placement are not recovered to support a pose correction |
| `held_matrix_deeper` | Recheck retail equipment parent scale, view offsets, and ActorInfo matrix writer | Read-only pinned Ghidra exports and copied-EXE disassembly | Complete; parent scale `(0.006, 0.009, 0.006)` and initial Axe/ext view offsets are mapped; matrix still needs live ActorInfo floats |
| `fire_camera_runtime` | Audit the reported fire-entry camera turn across SDL focus, relative mode, camera basis, and fire feedback | Read-only viewer and pinned retail evidence | Complete; no fire yaw/pitch write or camera-basis sign fault found; unfocused/stale SDL motion was a plausible input edge and now has a focus/relative-mode guard |
| `held_tool_pose_audit` | Recheck current visible Axe/ext facing against the retail mode-2 matrix path | `scene_viewer.cpp`, `AuditHeldToolTransform.java`, held-tool follow-up note | Complete; mode-2 matrix writer is confirmed; current direct roots/offsets remain unverified, and ordinary Axe selection is model 1 after the 2026-10-17 correction |
| `fire_camera_path_audit` | Recheck angle writers and fire-entry event order | `scene_viewer.cpp`, `player_fire_message.h`, pinned retail fire exports, held-tool follow-up note | Complete; SDL relative motion is the sole angle writer; opt-in audit now marks pain/passout; live stationary-input trace is still needed |
| `held_tool_orientation_delta` | Independently recheck backward-facing Axe/extinguisher pose against retail | Current viewer source, ACT assets, pinned Ghidra evidence; read-only | Complete; no source-backed axis/root correction, retail camera-space matrix and same-view capture remain open |
| `victim_movement_verify` | Verify victim roam step and turn helpers against retail instructions | Saved hash-pinned Ghidra output; read-only | Complete; strict target-arrival gate and exponential 0.3-second half-life turn are verified and ported; callback cadence remains open |
| `fire_hud_widget_trace` | Trace HUD child construction, resource IDs, and placement for the four fire sectors | Read-only Ghidra trace; `tracker/evidence/scenario-loading/fire_direction_hud_trace_2026-10-14.md` | Complete; resource 50 pair and exact rotated quads are mapped; first-pass port draws the live sector bytes; retail blend remains open |
| `gebm_truecolor_trace` follow-up | Cross-check natural victim and exit contact against retail source | `0045A070`, `00453A70`, `00450440` exports; `victim_transactions.h`, `scene_viewer.cpp` | Complete; confirmed the outer `+0x3438` dispatcher gate and inner carry gate; direct overlap bypass was identified and routed through the mapper; delivery cadence remains provisional |
| `propmodel_port_review` follow-up | Review BoundsTrigger gating and new victim-message integration | `scene_viewer.cpp`, `00440CE0`, `00440DC0`, `00470840` exports | Complete; confirmed `+0x164` visual gate and contact/render independence; caught and removed an unsupported Player-carry producer gate |
| `victim_message_pump` follow-up | Map the owner `+0x3BC` message drain and its order after contact callbacks | Preserved same-hash Ghidra exports for `00429520`, `0049B610`, and `004DF8CC` | Complete; same `00429520` invocation drains messages submitted during its earlier `+0xE0` contact pass; outer rendered-frame and Player update order remain open |
| `rescue_contact_gap` follow-up | Check final-exit enablement against BoundsTrigger contact semantics | `004504C0`, `00440F50`, `00440CE0`, `00470840`, authored exit inventory | Complete; exit availability is a model/render state, not a contact gate; persistent overlap remains `touching`, so no synthetic `touch` is source-supported |
| `carry_presentation_trace` | Trace what retail displays while Player carries a VictimRescue actor | Same-hash EXE pickup/status exports, Genesis actor-render gate, `scene_viewer.cpp` | Complete; pickup hides and freezes the world victim; Player status key semantics are unresolved and no Player-relative carried-body transform was found |
| `rescue_script_surface_followup` | Find broad missing authored Player/trigger behavior across campaign rescues | `00451870`, `00451560`, `00458400`, copied rescue actor inventory, shared host | Complete; all seven campaign rescues author oxygen toggles; shared oxygen state/commands, smoothed gauge/needle draw, resource-702 visor transition, child sounds, and category-volume selector effects are integrated; catalog MP3 music and oxygen stream gain are now wired; the manager flag, menu playback, audible parity, retail blend/visual parity, warning, cadence, and fog pixel parity remain open |
| `victim_hazard_reactions_followup` | Recheck Player passout and carried-victim drop timing/integration | `00450FB0`, `00451080`, `0044FF10`, `0044CD10`, `victim_transactions.h`, `scene_viewer.cpp` | Complete; fire uses passout argument 1 and oxygen argument 0; the shared host wires the separate `+0x3400` safe snapshot, `+0x534` reset checkpoint, carried-victim drop, floor placement, status tail, and strict state-6 recovery delay. Eight controlled map checks cover drop/reset/re-pickup; natural hazard entry, retail scheduler cadence, mode-2 HeroDown, and live camera parity remain open. See `player_passout_recovery_port_2026-10-14.md`. |
| `music_path_audit` | Compare catalog-authored menu/level music with the shared audio host | Read-only; `thegame.lev`, level-loader and stream helpers, `scene_audio.h` | Complete; four MP3 assets and catalog choices are hash-pinned; selector-2 level start/loop and linked-stage `+0x0C` inheritance are traced and ported, with oxygen stream gain; menu-track startup, FMOD streaming behavior, and audible parity remain open |
| `mission_lifecycle_audit` | Find the highest-impact start/objective/completion/level-end gap | Read-only; `rescue_completion_and_exit_path.md`, `MBASE_RESCUE_SLICE.md`, `mission_selector.h`, and `catalog_time_rescue_deadline_port_2026-10-19.md` | Complete; `time:` reaches Player `+0x343C` and feeds `004541A0` from escape and two RescueBot callers; port carries the selected field through linked stages. It is not a confirmed mission countdown. Result/outro and natural completion remain unresolved. The port replays early shared-EVS loadout tells after four World updates; retail callback cadence and final selected-tool parity remain open |
| `equipment_gap_audit` | Check held-tool rendering/action/audio against current evidence | Read-only; `extinguisher_scene_viewer_runtime.md`, equipment source headers, retail hash-pinned traces | Complete; eligible particle requests draw provisional smoke; packaged SDL rejection of malformed source BMP is fixed by a local 24-bit conversion; natural hit/suppression remain unverified; Axe matrix remains open |
| `axe_animation_trace` | Recheck reported Axe swing direction against the selected ACT and retail view transform | Read-only pinned retail/Ghidra trace; both Axe ACTs, `ItemViewPose`, `scene_viewer.cpp` | Complete; ordinary first equip uses model 1 `axe.act`, model 6 is duplicate-gated, and both have positive early local-Z motion; corrected asset mapping is integrated but camera-space direction remains open pending retail ActorInfo matrix values |
| `extinguisher_failure_trace` | Trace why the shared extinguisher appears inert and find retail effect lifecycle | `00461F80`/pool trace, contact helpers, `scene_viewer.cpp` | Ongoing; no deterministic input/queue/suppression drop found; retail doubles influence before the quadratic response, so near-2 center-contact influence has little effect; retail RGB suppression tint is now ported; parent-frame/cadence and successful live contacts remain unresolved |
| `axe_component_frame_followup` | Resolve the reported swing direction through the retail Player follower and Genesis actor/camera transforms | `equipment_view_frame_trace_2026-10-17.c`; `actor_camera_pose_trace_2026-10-17.c`; read-only headless Ghidra | Ongoing; ActorInfo basis packing and BONE06 attachment × sample composition match the port; retail Player frame stores target-minus-position at +Z, but its screen meaning and Player-to-camera-to-ActorInfo numeric join remain open, so no Axe flip is yet justified |
| `menu_asset_audit` | Identify authentic retail main-menu assets and safe integration points | Read-only `menu.rrf`, local retail menu files, current selector | Complete; bitmap ID 10 title art and alpha texture ID 100 root labels are copied; exact root positions/actions are now traced |
| `menu_code_trace` | Recheck retail menu constructor, level selector update/handler, and state flow | Read-only same-hash exports and saved decompilation | Complete; selector records/actions and root labels, bounds, click dispatch, and page indices are traced; page content, selector rendering, input producers, highlight timing, and menu sound semantics remain open |
| `menu_root_widget_trace` | Pin the retail title control atlas, layout, callbacks, and activation visuals | Read-only Ghidra 12.0.4; fresh `.c` exports and menu bitmap/mask | Complete; action codes 0..3 map to children 1/4/3/8 at authored positions; native root labels are integrated, while secondary pages and exact highlight timing remain open |

## Current state

- **2026-10-17 shared rescue front-end checkpoint:** the default macOS launcher
  builds the rescue-all app and opens a native title screen followed by the
  source-ordered eight-entry rescue selector. The title uses original retail
  bitmap ID 10 and root-control alpha texture ID 100 with GAME/SCORES/OPTIONS/
  EXIT labels and source-backed control positions. GAME currently bypasses the
  career child page and opens the port-created rescue list; SCORES/OPTIONS
  pages and full selector parity remain open. It replaces the old AppleScript
  raw-BSP picker; passing a map key still opens one package for development.
  The old launcher filename remains a compatibility alias. Retail's
  hash-pinned `0042DEC0` constructor, `00430F00` root controls, `00430DF0`
  action callback, and `0042F500` catalog handler are mapped. CMake and packaged binaries match at SHA-256
  `bcbd5e3803e036a2b9b2240ad81e8f162c04773f1104c9617182e6876fd29121`;
  catalog loading (8/8), offscreen title/selector rendering, and controlled
  rescue transactions (13/13) pass. These do not establish interactive launch,
  natural completion, or retail parity. Per-level `time:` reaches Player
  `+0x343C` and three deadline-helper paths, including two RescueBot routines;
  no mission countdown, HUD, expiry or result flow is established. `0044C360`
  remains an unverified no-final-exit path, not a proven outro. See
  [`rescue_front_end_entry_flow_port_2026-10-17.md`](../tracker/evidence/runtime/rescue_front_end_entry_flow_port_2026-10-17.md).

- **2026-10-17 retail menu asset checkpoint:** the earlier title-art build
  carries the original 640x480 `main.bmp` registered as menu RRF resource 10.
  The current root also draws rows from `mcontrols.bmp` plus its alpha mask at
  source-backed positions. Its selected alpha and keyboard mapping remain
  approximate; child page content and the rescue selector are still
  provisional. See
  [`retail_main_menu_art_port_2026-10-17.md`](../tracker/evidence/runtime/retail_main_menu_art_port_2026-10-17.md).
  The rebuilt CMake and packaged executables match at SHA-256
  `b48789b0b0fdd5122eb7336b6579cdb1e2ad16d1e3a6d594f50691420693437e`;
  the existing offscreen menu check renders the retail title bitmap and eight
  mission rows, but does not establish retail behavior.

- **2026-10-17 retail root-control checkpoint:** fresh same-hash traces map
  `00430F00` to original label texture resource 100, 640x480 positions, and
  action codes, with callback `00430DF0` routing actions 0/1/2/3 to menu
  children 1/4/3/8. The macOS title now loads the paired `mcontrols.bmp` /
  `mcontrols_a.bmp` assets and renders the GAME/SCORES/OPTIONS/EXIT rows at
  their retail origins. The selected alpha treatment and key bindings remain
  approximate; GAME still bypasses the career child page, SCORES/OPTIONS pages
  are not ported, and the following eight-row list remains a selector parity
  gap. CMake and packaged executable SHA-256
  `5ce9e3141343f8655b115f4847b7407d34b79399b467a3cfceaac60d5a4e176f` match;
  the bundled menu assets match their source hashes and `Info.plist` lints.
  No render or interactive capture was run after this change. See
  [`retail_menu_widget_trace_2026-10-17.md`](../tracker/evidence/runtime/retail_menu_widget_trace_2026-10-17.md).
  The user clarified that retail matching applies to the menu and the broader
  port; where decompilation or runtime evidence is missing, state which parity
  gaps it affects rather than treating a plausible implementation as retail.
  The user has now paused menu work while equipment, rendering, audio, and
  mission lifecycle take priority.

- **2026-10-17 held-equipment follow-up:** fresh Ghidra tracing separates the
  Axe root gate from each child's model flag. Normal first equip uses model 1
  (`axe.act`); duplicate `equip axe` can enable primary model 6 (`axel.act`).
  The shared viewer no longer enables that gate on first equip and draws the
  child ACTs selected by it, with the active child's `swing` track candidate.
  This corrects asset/action mapping, but the user-reported toward-player swing
  remains unresolved because the preview ActorInfo root is guessed and its
  numeric basis relation to retail's Player view follower is unknown. Do not
  apply a sign flip based on the current camera-root guess. For the
  extinguisher, the verified retail action transition is 300 ms to enter and
  1000 ms to return; eligible hits queue message 30000 and request a pool
  particle. The shared viewer now draws a provisional two-second resource-14
  smoke quad and logs selection, button edges, sample points, contact delivery,
  and suppression strength/alpha in `--fire-camera-audit`. No deterministic
  source failure was found, but an interactive successful contact remains
  unverified. CMake `fdny-scene-viewer` and the packaged rescue-all app built
  at matching SHA-256
  `f8e006e636d1ef0101b0284904dc2927f98047c68bee7834a8e8885e8b6736f7` (linker
  warning: Homebrew `json-c` targets macOS 27 while this build targets 13.3).
  No interactive check or retail parity claim. See
  [`equipment_action_pose_followup_2026-10-17.md`](../tracker/evidence/scenario-loading/equipment_action_pose_followup_2026-10-17.md).

- **2026-10-17 extinguisher bundle-load follow-up:** the first packaged Axe
  orientation diagnostic exposed a startup failure before the scene loaded:
  SDL rejects retail `bsmoke.bmp` because its header declares 254 palette
  entries while ten pixels use indices 254/255. The local exporter now
  converts both source sprites to 24-bit BMP without changing retail inputs;
  the two undeclared smoke colors are provisionally padded with black and
  recorded in the generated manifest. After rebuilding, the packaged
  `--tool-orientation-check axe` loaded the scene with 121/121 render groups
  and 24/24 textures and saved the three port-only Axe frames. This verifies
  asset loading and scene startup, not extinguisher contact/suppression or
  retail visual parity. Build and packaged executable remain SHA-256
  `f8e006e636d1ef0101b0284904dc2927f98047c68bee7834a8e8885e8b6736f7`; the
  exported sprite hashes and limitations are in
  [`extinguisher_particle_render_trace_2026-10-17.md`](../tracker/evidence/scenario-loading/extinguisher_particle_render_trace_2026-10-17.md).

- **2026-10-15 portable wide-integer checkpoint:** `clock.h` and
  `extended.h` now use the shared `wide_integer.h`; the portable two-word
  fallback matches the macOS compiler's native arithmetic in 100,000 unsigned,
  100,000 signed, and 10,000 GameClock comparisons. Both native and forced-
  fallback CMake builds pass on macOS, along with the fallback `--rescue-check`
  on all 13 Training/rescue/stage packages. Linux and Windows native builds
  remain open. See
  [`cmake_platform_boundary_2026-10-15.md`](../tracker/evidence/runtime/cmake_platform_boundary_2026-10-15.md).

- **2026-10-15 FireRescue route-update checkpoint:** the shared host now keeps
  the original FireRescue visual at its prior position until route progress
  advances by more than half `H_Radius` on authored no-`C_Model` routes, then
  updates it and still snaps to the PathPoint on completion. The `00448D60`
  initializer writes `(H_Radius,H_Height,H_Radius)` at root `+0x108..+0x110` in
  this branch; MBase `mortarfire1` has radius 100 and Gas R `Fire41` radius 45.
  The existing packaged checks pass MBase (5/5 rescues), Gas R (7/7), and the
  eight-entry mission catalog. They confirm startup command dispatch and
  controlled rescue transactions, not natural movement or retail timing. Child
  spawn-queue ownership/lifetime/world setup, the model-derived threshold path,
  trailing timed-key effects, exact floor behavior, and callback cadence remain
  open. The host queues qualifying Player `0x7531` exposures through the full
  FireRescue candidate pass and delivers them after generated spread-child
  creation, matching the mapped owner message-drain boundary. It now also
  tracks each named fire's previous `+0x120` distance and changes the shared
  nearby-fire count on `<=750` boundary crossings before each spread attempt.
  Already-counted terminal fires remain counted while resident. CMake and
  packaged executables match at SHA-256
  `440b04d12211b499332ab6e7c4fb6d44fa929ff7a70abba623fb9710982476f5`. See
  [`00448D60_h_radius_threshold.asm`](../tracker/evidence/rescue-moveto/00448D60_h_radius_threshold.asm)
  and [`rescue-moveto/README.md`](../tracker/evidence/rescue-moveto/README.md).

- **2026-10-15 fire contact pass boundary:** the hash-pinned retail update
  orders contact-manager callbacks before the entity-spawn queue and mapped
  message drain (`00429520`). The shared viewer snapshots the FireRescue
  candidate count before spread children are appended and queues qualifying
  Player exposure messages until after the represented candidate scans. The
  host now drains generated children, then FireRescue messages, then VictimRescue
  messages at the post-candidate boundary. Retail factory order places BModel,
  BoundsTrigger, and PropModel callbacks before FireRescue; the host now
  collects those three contact candidate groups before its FireRescue callback,
  followed by VictimRescue candidates. Class-specific ScriptActor drains remain
  unresolved. Children first take part in Player exposure on the next host
  frame. See
  [`contact_manager_factory_order_audit_2026-10-15.md`](../tracker/evidence/scenario-loading/contact_manager_factory_order_audit_2026-10-15.md).
  The implementation boundary and remaining uncertainties are recorded in
  [`ordered_contact_candidate_pass_port_2026-10-15.md`](../tracker/evidence/scenario-loading/ordered_contact_candidate_pass_port_2026-10-15.md).
  The rebuilt CMake
  and packaged app binaries match at SHA-256
  `440b04d12211b499332ab6e7c4fb6d44fa929ff7a70abba623fb9710982476f5`.
  Packaged MBase (5/5) and Gas R (7/7) rescue checks and the eight-entry
  catalog pass. These controlled checks do not simulate natural fire contact.
  Owner cadence, live list order, visibility results, and the fourth Player
  exposure value remain open; the candidate world-origin alignment is now
  verified. See
  [`fire_contact_pass_order_port_2026-10-15.md`](../tracker/evidence/fire-spread/fire_contact_pass_order_port_2026-10-15.md).
  The persistent proximity count is scoped to the host's stable one-Player
  scene list. Actor teardown/reuse, retail candidate membership/order, natural
  spread timing, and live retail parity remain open; see
  [`proximity_count_port_2026-10-15.md`](../tracker/evidence/fire-spread/proximity_count_port_2026-10-15.md).

- **2026-10-15 BModel bound construction:** retail `0043B7A0` unions the
  min/max fields from each child model, then converts the aggregate through
  `004819A0` before `0043B620` tests root `+0x240`. The host uses the matching
  exported GBSP `MODELS` bounds as its union inputs. The exporter data lineage
  is verified; Genesis runtime extents and final box conversion remain open.
  See
  [`bmodel_contact_bound_construction_2026-10-15.md`](../tracker/evidence/scenario-loading/bmodel_contact_bound_construction_2026-10-15.md).

- **2026-10-14 all-victim rescue checkpoint:** `--rescue-check` now runs every
  authored victim through the mapped `0x7532` pickup message and an authored
  escape touch on the eight primary packages. Packaged counts pass: Training
  2/2, MBase 5/5, Bar R 6/6, Gas R 7/7, Bank 6/6, Boat A 9/9, Subway A 6/6,
  and Oil Rig A 3/3. The packaged catalog check loads all eight choices. The
  CMake and app executables match at SHA-256
  `545f1c75eebbc8e251d3338b5308e7170af413a086038546498286ab92a08a37`.
  MBase reaches its no-final-exit message gate only after all five releases;
  this does not verify natural travel/contact, retail scheduling, or an outro.

- **2026-10-15 delayed EVS startup checkpoint:** the shared viewer now executes
  the full map EVS pass after four host World updates. Its `--rescue-check`
  verifies authored axe/extinguisher/goggles inventory and passes all-victim
  transactions on the 13 rescue/stage/Training packages. Bank and Subway A
  interaction checks and the bundled eight-entry catalog check also pass.
  Retail callback cadence and final selected-tool parity remain open. The
  executable SHA-256 is `0db1f4450015663d2917511f81ec4c9a9ba952dafa9963cb30264a572a82fc01`.
  See [`delayed_world_evs_pass_port_2026-10-15.md`](../tracker/evidence/scenario-loading/delayed_world_evs_pass_port_2026-10-15.md).

- **2026-10-15 ambient loop checkpoint:** the shared viewer recognizes authored
  `ambientsnd` commands, uses the retail integerized coordinates and loop flag,
  starts persistent spatial voices, and updates their listener as the Player
  moves. The shared bundle exporter includes each ambient resource's WAV and
  sound metadata. All seven maps with ambient commands pass the dummy-device
  `--ambient-audio-check`: Gas R (3), Bar R (4), Bank (6), Boat A (3), Boat B
  (2), Subway A (6), and Subway B (6). Each bundled sample resolves to a
  looping 3D voice, and fixed source/listener updates pass. Boat B's additional
  `Ambient2` loop and stop commands pass a controlled timed PATH check. The
  packaged app's eight-entry mission catalog and Boat B audio check pass; its
  CMake and app binaries match at SHA-256
  `2e7f23d8c5b92378fc0e0dbf617804b58b278e2f4a691fb8a90be56dca9065a2`.
  The 13-package `--rescue-check` suite also passes. These checks do not verify
  captured output or retail audio parity.
  See
  [`ambient_sound_loop_port_2026-10-15.md`](../tracker/evidence/scenario-loading/ambient_sound_loop_port_2026-10-15.md).

- **2026-10-15 oxygen audio mix checkpoint:** fresh same-hash Ghidra evidence
  maps oxygen-on selector 4 to category 2 at 0.3 and oxygen-off selector 5
  to categories 1 and 2 at 1.0. The shared voice mixer now tags source-mapped
  categories and applies the same mix to active voices. All seven authored
  ambient maps pass `--ambient-audio-check`, including the selector attenuation
  and restoration probe; MBase oxygen HUD render and all-eight-entry catalog
  checks pass. The separate streamed music channel was wired in a later
  checkpoint; captured retail audio parity remains open. CMake and packaged app binaries match at SHA-256
  `c0581c9561d0e34e34e96e18732560836a1627869b36eddf98a671241df25b90`.
  See
  [`player_oxygen_audio_category_mix_port_2026-10-15.md`](../tracker/evidence/scenario-loading/player_oxygen_audio_category_mix_port_2026-10-15.md).

- **2026-10-15 case-insensitive EVS variable checkpoint:** global variable
  names now resolve without regard to case, and `if` values use the retail
  case-insensitive string comparison. This resolves authored lowercase
  `*bonusXX` entity references against uppercase definitions in
  `bonusitems.evs`. The rebuilt rescue-all app passes `--rescue-check` on all
  13 bundled packages with no unresolved bonus-variable warnings; its mission
  catalog check loads all eight retail rescue entries. CMake and bundled app
  binaries match at SHA-256
  `23dba30c0f82de337966de8b6c3394bd7534fddfa1267b8ec6044796863e9c60`.
  The controlled checks do not validate live bonus pickup contact, rewards, or
  retail event timing. See
  [`case_insensitive_script_variables_port_2026-10-15.md`](../tracker/evidence/scenario-loading/case_insensitive_script_variables_port_2026-10-15.md).

- **2026-10-15 final-exit checkpoint:** the pinned retail `finalexit` command
  opens the gate and refreshes the first owned item when no survivors remain;
  with survivors left, it shows their count for seven seconds. The viewer now
  follows both branches, and `finalexitloadmap` uses the same seven-second
  incomplete hint. Controlled Bank/Boat B checks verify final-exit hints and
  inventory refresh; stage checks verify the incomplete gate. All 13 bundled
  rescue/stage checks pass. RescueBot evidence also resolves `norescuebot`:
  it sets the selector's exclusion byte; the selector requires victim state 4/7,
  at least 600 units of separation, and a blocked world segment. The
  selector still has no authored campaign call and Training sets its count to
  zero. CMake and app binaries match at SHA-256
  `a91cb032e94562f489baa5fe080a5ead6652dc2df38bc10343b20aa33c17707e`.
  Natural exit contact, stage pending-context identity, RescueBot scheduling,
  and full retail parity remain open. See
  [`rescue_completion_and_exit_path.md`](../tracker/evidence/scenario-loading/rescue_completion_and_exit_path.md)
  and [`victim_roam_behavior.md`](../tracker/evidence/scenario-loading/victim_roam_behavior.md).

- **2026-10-14 Bank performance checkpoint:** optimized the retail-gated FireRescue
  visibility path, cached authored collision-model metadata for runtime traces,
  replaced per-vertex immediate rendering calls with client arrays, and added
  conservative view-frustum culling for static mesh groups. Moving/posed BModels
  bypass culling. Fire sprite duration tables are now allocated at fire
  initialization rather than once per drawn fire per frame (72 avoidable
  allocations in Bank). Bank render, interaction, and victim-release checks
  pass; the render-check framebuffer hash matches across the render-only
  optimizations. A sorted static-model AABB broadphase now narrows Player
  sweeps, while posed PATH models stay on a separate complete check path and
  candidate order is preserved. A one-second `--performance-audit` reports
  frame/update/world/overlay/swap timings and draw/cull counts. The user reports
  Bank framerate now seems sorted. Retail delta consumers are time-scaled, but
  the delta writer and presentation limit remain unresolved; the viewer's
  100 ms clamp and VSync remain provisional. Do not claim measured retail
  frame-pacing parity. The packaged rescue-all app was rebuilt, and all 13
  rescue/stage spawn and five-second grounded idle checks pass. See
  [`retail_frame_pacing_audit_2026-10-14.md`](../tracker/evidence/runtime/retail_frame_pacing_audit_2026-10-14.md)
  and [`bank_runtime_performance_followup_2026-10-14.md`](../tracker/evidence/runtime/bank_runtime_performance_followup_2026-10-14.md).
- **2026-10-14 PropModel actor/contact checkpoint:** the shared exporter now
  includes all 15 actor resources referenced by PropModels in the 13 rescue
  stage packages. The ACT decoder handles the verified paletted and 24-bit BGR
  GeBm layouts, and the renderer uploads RGBA so color keys can be preserved.
  World PropModels draw from their authored location, heading, pitch, and live
  `setscale`; `show`/`hide` also change the retail-derived model/contact gate.
  Player-point/radius contacts dispatch in retail's `touch`, `touching`, then
  `untouch` category order. The contact path now resolves the Player and
  PropModel-center BSP leaves and applies the source-backed cluster VISDATA
  rejection. The runtime area-visibility matrix, Player frame-point alignment,
  and updater's external caller/cadence remain open; positive PVS results are
  permissive rather than full retail visibility parity. Build and Training
  render checks pass.
  See [`propmodel_world_actor_port_2026-10-14.md`](../tracker/evidence/scenario-loading/propmodel_world_actor_port_2026-10-14.md).
- **2026-10-14 rescue contact and trigger checkpoint:** natural victim overlap
  now submits `0x7532` through a generation-checked Player route, then applies
  the retail message gate (`+0x3438`) and carry-pointer gate (`+0x3534`) in
  order. The producer still queues overlaps while carrying so the Player
  handler owns that rejection. BoundsTrigger contact dispatch is grouped into
  retail's `touch`, `touching`, and `untouch` passes; its visual pulse now
  requires the authored `tell player escape` gate. Contact and render state
  stay independent. The same-hash owner vtable trace now verifies that a
  message submitted during the `+0xE0` contact pass is drained by `+0x3BC`
  later in the same `00429520` invocation. The host uses the same local order;
  rendered-frame cadence and Player update ordering remain open. Final-exit
  availability changes render/model state only; a Player who remains inside
  gets persistent `touching`, so retail requires a new overlap edge for another
  `touch`. See
  [`victim_contact_message_port_2026-10-14.md`](../tracker/evidence/scenario-loading/victim_contact_message_port_2026-10-14.md).
  Carry presentation is also source-checked: pickup sets the Genesis actor
  flags to zero, so the normal world render pass hides the victim. State 5
  skips its child animation updater. No source-backed Player-relative victim
  pose was found; keep the world actor flags authoritative and leave the
  `RC_VictPick` status visual meaning open. See
  [`actor-render-flags.md`](../reference/previous-work/decomp/evidence/victim-ai-audit/actor-render-flags.md)
  and [`victim_pickup_message_path.md`](../tracker/evidence/scenario-loading/victim_pickup_message_path.md).
  The packaged `--rescue-check` now passes on Training and all seven campaign
  rescue maps through the mapped pickup queue and authored exit touch. The
  mission-catalog check loads all eight retail rescue choices; Training render,
  trigger-visibility, and extinguisher-orientation checks pass. The packaged
  rescue-all app was rebuilt after oxygen HUD integration. That checkpoint's
  executable SHA-256 was
  `88dc2baf13fe9158445d93d88e788d450735456984b1fa8843e5ecb91c62eab6`.
- **2026-10-14 Player oxygen port checkpoint:** the shared host now routes
  authored `oxyon`/`oxyoff` through the Player command handler and updates the
  source-mapped oxygen scalar/timers in `player_oxygen.h`; a cross-map audit
  found 12 BoundsTrigger entities across the seven campaign rescue maps, with
  additional authored bindings in linked stages. Retail initializes the oxygen
  value to 1, starts with timed drain disabled, drains 0.05 every three seconds
  after `oxyon`'s 700 ms delay, and refills by frame delta × 0.1 after
  `oxyoff`. Depletion calls the parameter-0 passout route; the status updater
  uses the same value and dispatches `RC_OxygenLow` below 0.25. The viewer now
  plays the one-entry `RC_OxygenLow0`/`o2low.wav` cue from all 13 bundled
  packages. Depletion now reaches shared passout argument 0, while fire retains
  argument 1. The initial
  rescue-all build checkpoint used executable SHA-256
  `5bbb5190739afcd26c219e961bd75bd128d463fed91d445d9a68fca083d76dff`. Its
  packaged catalog and synthetic rescue checks passed on the eight primary
  maps. Later renderer checkpoint hashes are historical; see the all-victim
  rescue checkpoint at the start of Current state for the current executable.
  The native port draws the retail oxygen gauge and needle at the traced
  rectangle and rotation. The app builder exports and bundles the four retail
  sprite bitmaps; `--oxygen-hud-render-check` passes from a bundled scene and
  reports the visor effect, gauge layers, and low-oxygen overlay with changed
  pixels. Category selectors 4/5 are ported through the shared mixer and
  control level-music gain; the manager flag, warning/visor blend parity,
  audible music parity, and Player cadence/order remain open. State-6
  recovery now uses the strict >1000 ms gate, mapped `+0x3400` safe snapshot,
  separate `+0x534` reset checkpoint, carried-victim drop, floor placement, and
  status tail. The host currently models constructor-default cleanup state and
  mode 0; mode-2 HeroDown, equipped-item view offsets, and retail update cadence
  remain open. Eight controlled rescue checks also exercise one passout/drop/reset/
  re-pick-up per map; they do not prove natural passout or retail timing.
  Evidence:
  [`player_passout_recovery_port_2026-10-14.md`](../tracker/evidence/scenario-loading/player_passout_recovery_port_2026-10-14.md)
  and [`player_oxygen_resource_and_script_path_2026-10-14.md`](../tracker/evidence/scenario-loading/player_oxygen_resource_and_script_path_2026-10-14.md).
- **GitHub handoff:** checkpoints use `python3 tools/sync_github.py` for a
  compare-first mirror into `decomp_revised/`, followed by a reviewed push and
  remote SHA verification. User authorization for occasional pushes is
  recorded in the active goal. This checkout has no configured Git remote; the
  helper preserves `decomp/` and ignored retail inputs, Ghidra data, and builds.
- **2026-10-14 Axe/rescue checkpoint:** left-click now waits through the
  source-mapped +100 ms Axe action deadline, starts the exported `axel.act`
  swing, and samples the strict animation-time crossing `prior < 0.275 < new`
  before running the `AxeStrikePose`/`AxeWorldCollision` sweep. Candidate checks
  use the retail inclusive 8-unit expansion around each loaded world AABB,
  followed by the `G_Liftable`, hit-point, and root `+0x1EC` gates. Accepted
  targets receive authored `chop`/`break` dispatch; the retail `0043D660`
  boundary starts their BModel `open` PATH. Onscreen Axe feedback reports hit
  and miss outcomes. SDL `dt`, left-click/held mapping, camera-frame conversion,
  candidate list identity/order, ACT-to-retail timer alignment, and the retail
  attached hand matrix remain provisional; no 1:1 Axe claim is made. The
  provisional camera-up root was lowered from 27 to -8 units after the 0.30 s
  exported ACT sample clipped above the view. The staged diagnostic now shows
  the full pose in frame and 53,610 pixels different from bind pose. This checks
  the port animation render, not an interactive strike. CMake and staged app
  executables match at SHA-256
  `3b109e7dacf4d1dc3cb63b7671022cb11cb4ea44270efe8b91adc5694385fade`.
  Synthetic pickup/release checks passed on Training and all seven campaign
  rescue maps. Bank's door approach check dispatches entity 541's authored touch
  without M, plays its sound, and reaches PATH 1/1; the straight approach stops
  24.011 units off center at static world model 0, while lateral alignment then
  crosses. This is a controlled preview fixture, not an interactive retail
  comparison. Retail `0044C360`'s first-populated-item refresh remains wired
  after rescue delivery. `finalexitloadmap` now gates on
  remaining survivors and chains Boat A→B, Subway A→B, and Oil Rig A→B→C→D by
  authored key into sibling packages. All six authored touch bindings pass a
  controlled remaining-survivor gate and load their requested package; all 13
  bundled rescue/stage packages pass the rescue check. Bank and Boat B now also
  verify the incomplete `finalexit` count hint for seven seconds and the
  successful gate plus first-owned-item refresh; `finalexitloadmap` incomplete
  hints use the same seven-second window. The viewer adapter does not prove
  retail's separate pending-context string equals that key; live transition
  contact/timing remain open. See
  [`axe_swing_runtime_port_2026-10-14.md`](../tracker/evidence/scenario-loading/axe_swing_runtime_port_2026-10-14.md)
  and [`rescue_completion_and_exit_path.md`](../tracker/evidence/scenario-loading/rescue_completion_and_exit_path.md).

  **User check:** open `.local/FDNY Scene Preview Axe Update.app`, choose
  Training, press `1`, approach a chair or box with the authored Axe hint, then
  left-click once. Report whether the tool still faces backward and which
  onscreen result appears (`no world hit`, `no eligible breakable`, or
  `chopped`). This is the interactive check needed before treating the visible
  swing or hit path as working.

- The latest Training app rebuild includes the audit-only pain/passout feedback
  markers, per-FireRescue exposure-candidate
  correction, no-`C_Model` BoundsTrigger handling, and the SDL focus/relative-mode
  guard for camera input. Fire passout now releases an active extinguisher
  ACTION and ignores mouse-look deltas during Player state 6, matching the
  reviewed release route while preventing more SDL-driven turn during
  incapacitation. The viewer now uses the retail fresh-profile DifficultyLevel
  1 pitch cap (±40°) and state-4 crouch eye height (10; standing 70) instead of
  ±60° and 30. Loaded-profile pitch limits and the state-5/spectating exception
  remain unported.
  The earlier Axe iteration was also packaged in `.local/FDNY Scene Preview
  Axe Update.app`. The standard rescue-all app has since been rebuilt with the
  PropModel cluster-PVS gate; that checkpoint's executable SHA-256 was
  `b44840eced38f76a3639271ea593b77cbc856fe0614ed37b5b9341a441da8529`.
  The packaged `--victim-roam-math-check` passes the strict arrival and
  exponential heading cases; packaged `--render-check` loaded 121/121 groups
  and 24/24 textures without GL errors. `--fire-direction-render-check` drew
  all four retail sector quads in all 13 Training/rescue packages, changing
  about 130,518 framebuffer pixels per package with no OpenGL errors.
  Alpha/blend and live indicator cadence still need retail comparison.
  Packaged render and held-model orientation diagnostics pass; they do not
  establish live 1:1 visual/gameplay parity. The tools' camera-space roots and
  extinguisher scale remain provisional. Retail's one-second state-6 camera
  blend is still not rendered by the shared viewer, and a live fire-entry
  trace is still needed to separate passout view movement from SDL input. The
  pitch/crouch corrections cannot explain an automatic yaw turn. See
  `tracker/evidence/scenario-loading/held_item_and_fire_camera_followup_2026-10-14.md`.

- Fire exposure has no source-confirmed write to the viewer's mouse-look yaw. A fresh
  hash-pinned retail audit shows `0044BB00` only sets one of four Player bytes
  (`+0x353C..+0x353F`): front `[-45°,45°]`, rear `[-180°,-135°]` or
  `[135°,180°]`, positive-angle side `(45°,135°)`, and
  negative-angle side `(-135°,-45°)`. `00452570` tests these bytes to gate burn
  cooling, then clears them; `00456090` sends them to four HUD child effects.
  The shared viewer now records the source-mapped sectors and calls its
  source-derived cooling component. The port now draws the retail `fireprox`
  color/alpha pair in the source-mapped four viewport locations, rotated by
  each sector's recovered angle. The matching Player bytes drive visibility.
  Exact retail alpha sampling/blend and interactive parity remain open. An opt-in
  `--fire-camera-audit` logs exposure transitions and all captured relative
  mouse deltas, including the first exposure frame, and per-250-ms samples of
  heading, pitch, exposure/influence, Player position/velocity/delta,
  grounded/crouched state, movement input, camera position, SDL focus/relative
  mode, sampled view-angle changes, and mouse-explained angle changes per
  sample. Audit mode also tags pain and passout callbacks. A live run is needed
  to distinguish input-driven yaw from movement or pain feedback. The refreshed
  Training executable hash is
  `b44840eced38f76a3639271ea593b77cbc856fe0614ed37b5b9341a441da8529`.

- Held ACT rendering is not confirmed 1:1. Correction from 2026-10-17 Ghidra
  follow-up: this earlier interpretation conflated the Axe root gate with the
  child's model-selection flag. Ordinary first equip leaves root gate `+0x1FC`
  off and activates secondary model 1 `axe.act`; the duplicate-equip branch
  enables primary model 6 `axel.act` as well. The shared viewer now follows
  that gate for equip state, model choice, and action animation. The preview's
  guessed camera-root candidate still maps early local-Z swing motion toward
  the viewer; the retail camera-space ActorInfo matrix remains unresolved.
  Extinguisher still uses its direct camera-root candidate because that shows
  its label. Retail's exact camera-space ActorInfo matrix is not recovered:
  the mode-2 writer is mapped through `00497F70` → `00492110` → `00492450`,
  but the live source frame, hand origin, scale, and depth behavior remain
  open. The packaged Training check loads 121/121 render groups and 24/24
  textures; it does not prove retail-facing parity. The current Training app
  hash is `4d14e55c52c68e5dca1e88a7ec9e45a903d85bb286e4832dd07eb2533a5e020d`.

- The normal Training render check's spawn view has no trigger brush, so it
  cannot confirm the red-volume report by itself. A focused model-flag render
  check covers entity 106/model 3; its current packaged result is recorded
  below. Static joins resolve 25/25 Training BoundsTriggers to models using
  `col_red`; `scene_viewer.cpp` initializes their retail-derived model flags
  to zero and gates drawing on the inferred normal-render bit. Enabled exit
  pulses use approximate SDL frame sampling. See
  `tracker/evidence/runtime/trigger_brush_render_visibility.md`.

- Repeated checks against the packaged Training app found entity 106/model 3
  solid red when forced visible and absent at flags zero; all 1,024,000 frame
  pixels change. The normal render check uploaded all 24 textures, submitted
  121/121 groups, and drew 28 fire sprite quads. This confirms the current app
  package contains the trigger model-flag gate, not retail renderer parity.
  Oil Rig B entity 78 (`zone control room`) has no `C_Model`; retail
  `00441040` copies root fields `+0xC8/+0xCC/+0xD0` into equal min/max fields
  on its null-model path, and `00440CE0` requires a model pointer before
  contact. The port now skips this record without fabricating bounds. Its
  fog/oxygen scripts do not fire through the traced BoundsTrigger contact
  route. The mapping of those root fields to exported `ZZ_location` is open.

- The map-neutral Mac scene preview uses authored spawn data, recovered movement, and BSP collision. All seven rescue maps pass collision-backed spawn and five-second grounded-movement checks. The exporter and C++ loader preserve per-face identity, TEXINFO values, raw lightdata, and source-derived lightmap coordinates. The viewer composites static and EVS-animated face lightmaps through a shared atlas using the traced multiply blend; Training, Bank, and Bar R render checks uploaded 1,575, 9,474, and 7,063 lit faces respectively. These checks validate the port's draw path, not retail pixel parity. Trigger contact/collision handling and rendering are separate.

- Same-key EVS include chains and referenced WAVs are now bundled for all 13
  rescue mission and stage packages: Training, the seven catalog rescues, Boat
  B, Subway B, and Oil Rig B-D. The current C++ loader inspects all 13 with
  scripts loaded and no texture failures. BoundsTrigger C_Model bounds resolve
  across all 17 BSP packages; the 13 rescue packages build their authored
  `makezone` groups. The current all-victim `--rescue-check` passes across all
  13 packages. These synthetic checks do not verify live contact scheduling,
  linked-stage handoff, complete mission objectives, effects, or retail parity.
  Boat B's button entity registers `addtimecmd 0.1 (loopsnd Ambient2)` and
  `addtimecmd 29 (stopsnd Ambient2)` through the current BModel command handler;
  the package startup check no longer reports them unimplemented. The targeted
  Boat B `--ambient-audio-check` advances a copy of the button PATH through
  both keys and confirms the loop starts and stops; live path timing remains
  unverified.

- The map-neutral preview now builds typed victim actors from the BSP's
  victim/alternate-spot records and hash-pinned ACT resource crosswalk. It
  selects initial motions/material variants, preserves the two victim spawn
  phases, registers alternate spots, and dispatches the five native spawn
  script fields. Authored `roam` commands now use the traced origin radius,
  FireRescue obstacle spheres, victim-box sweep, floor-height probes,
  player-relative target gates, random fallback, and walk selection. The fresh
  hash-pinned retail trace verifies the state-7 step helper's strict arrival
  test and normalized movement and the turn helper's exponential
  `0.5^(dt/0.3)` old-heading weight; the shared viewer uses those formulas.
  A focused arithmetic diagnostic passes, but callback cadence still uses the
  viewer's rendered-frame delta. Static `H_Radius` seeds fire obstacle radii
  while its retail UserData binding is still unproven; victim callback cadence
  and all-class RNG ordering remain open. The viewer now constructs authored
  FireRescue actors in entity order, applies source-shaped floor placement,
  animates their retail sprite frames, and runs the recovered FireUpdate
  transition in the rendered frame loop. The 38 retail flame/glow BMPs are
  hash-recorded in the local asset bundle. Holding left mouse with the
  extinguisher selected now uses the recovered four-point pose sampler,
  padded fire falloff, 32-record message 30000 queue, and nonterminal
  suppression transition. The viewer camera supplies the extinguisher parent
  frame, and action cadence remains a rendered-frame approximation; those
  host inputs are provisional. Random proximity spread now follows the mapped
  004483A0 → 00448320 → 00447A90 contact path, including its 750-unit attempt
  gate, candidate sweep, child shape/lifetime, and deferred creation. Its
  once-per-rendered-frame cadence and dynamic-child RNG/lifecycle remain
  provisional. Current-hash getter disassembly confirms Player::p matches the
  retail PlayerRescue candidate origin. Fresh constructor tracing resolves its
  fourth exposure value at root +0x74 to zero; callback cadence remains open.
  The gate at
  root +0x48 maps to nonempty FireRescue A_Name; unnamed authored fires and
  generated children are gated out. PlayerRescue slot +0x0C is
  now mapped through current-hash disassembly to body 0049D790; the viewer
  passes its matching movement origin unchanged. Spread follows
  the retail nonempty A_Name gate; generated unnamed children do not become
  spread parents. Player message `0x7531` now reaches the traced pain, burn
  accumulation, and passout path; retail pain/passout WAVs are bundled. Fire
  contact scheduling, emitter scale and the minimum influence constant remain
  provisional. The viewer now uses zero padding from the Player getter's
  constructor-initialized +0x74 field. Live damage feel still needs a desktop
  capture. A read-only audit confirmed this path is called in the
  ordinary rendered frame loop; plausible non-contact causes remain the
  approximate volume/scale/visibility inputs, an inactive fire state,
  or a player already in failed state. No interactive observation identified
  which applies. Spray particles and original item
  charge/refill parity remain open. Earlier MiscEmitter and other class RNG
  draws are still missing.
  The preview now bundles the map's
  EVS include chain and referenced WAVs, dispatches BModel and BoundsTrigger
  `touch`/`touching`/`untouch` events, and applies `open`/`close` plus per-owner
  time scale to retail PATHs. Bank's `VaultConsole` can issue its named
  `tell vault open` route; E/Space is the host action. Subway A's broken-door
  trigger dispatches its authored `Zapp3d` command. Synthetic interaction
  checks pass on Subway A and Bank; all 43 Subway A and 14 Bank trigger bounds
  resolve from `C_Model`; `makezone` builds six Subway A and ten Bank named
  zones. Callback cadence and wall-clock timing remain provisional, and exact
  retail contact bounds are still open. Authored `tell player escape` now
  reaches the shared victim-release and exit-gate transaction; synthetic
  headless checks pass on Training and all seven campaign rescue BSPs, releasing
  every authored victim (2/2, 5/5, 6/6, 7/7, 6/6, 9/9, 6/6, and 3/3). MBase
  trigger 159 supplies the authored escape binding; its no-final-exit gate
  opens after 5/5, with outro semantics unverified. These checks do not verify
  natural travel/contact, live trigger scheduling, carried-body presentation,
  or mission completion. Player exposure is now wired through
  the source-shaped message handler; persistent stats, passout animation/item
  release and carried-body presentation remain open. Terminal suppression sets
  mode 5 and clears root `+0x1C8`, which aliases the visual component's enabled
  byte at `+0x9C`. The shared host now mirrors that flag to its fire renderer,
  stopping subsequent child updates and draws. Child destruction, alpha fade,
  and the lifetime of already-submitted polygons remain open. `M`/`N` remain a
  global PATH inspection control.
  The preserved 2,160-call suppression oracle passes through the FireEntity
  wrapper. The packaged rescue-all app was rebuilt at SHA-256
  `ecfb5374652253ade1128a5576e74b5ca69760e0b1ed752fa52700bea480d179`; Bank
  and Gas R rescue checks pass. A hidden SDL render check could not create a
  display in the unattended environment. See
  `tracker/evidence/fire-spread/visual-terminal-state.md`.
  The MBase macOS app bundle had previously passed the headless rescue check;
  the Bank app bundle includes the fire assets and its packaged binary passed
  the earlier render and teleport-based interaction checks. The source
  `--interaction-check` now approaches entity 541 with `Player::step`, triggers
  its touch/OpenDoor route without M, waits for the PATH endpoint, centers at
  the aperture, and crosses using the live collision pose. This controlled
  route does not confirm ordinary approach timing or reproduce the user's
  impassable-door report. The app bundle was rebuilt for Training on
  2026-10-13 after the HUD, camera-relative per-item held model roots, goggles,
  and provisional player-hull exposure-padding changes; the
  launcher also rebuilds the selected map before opening. Source/evidence are in
  `scene_viewer.cpp`,
  `tools/export_victim_actors.py`, `tools/export_equipment_actors.py`,
  `tools/export_goggles_sprites.py`, `tools/export_fire_sprites.py`,
  `tools/export_evs_bundle.py`,
  `tracker/evidence/scenario-loading/fire_entity_viewer_runtime.md`, and
  `tracker/evidence/scenario-loading/bmodel_door_runtime_port.md` plus
  `tracker/evidence/scenario-loading/extinguisher_scene_viewer_runtime.md`.
  The latest Training rebuild includes per-item Axe/extinguisher roots; its
  packaged and CMake executables match SHA-256
  `4c9564098bda8b8395752a2983904eb6c0a93e815e3c7df45c5995242369208d`.
  `--inspect` loaded all 121 render groups, `--render-check` uploaded 24/24
  textures without GL errors, and `--tool-orientation-check` drew both held
  ACTs under both candidate roots. These do not establish retail visual parity.

- The shared viewer now runs ordered `PlayerRescue` script fields through the
  Player command host. Authored axe/extinguisher/goggles slots populate through
  the retail-mapped `equip` path, and `select` updates shared slot state;
  viewer keys 1/2/3 choose an equipped tool. Equipment ACT assets now export
  from `rescue.rrf`; the Axe left-click records the source-derived root ACTION
  deadline (global clock +100 ms), starts the named `swing` track after that
  gate, and runs its sweep on the source-mapped strict timer crossing
  `prior < 0.275 < new`. The extinguisher's ACT
  is drawn in a camera-relative preview position. The
  current axe bone hierarchy put the model below the view and the extinguisher
  contact-frame origin behind the OpenGL camera; their presentation roots now
  place both models in the first-person view and clear world depth. Current
  source draws the selected model-6 Axe and extinguisher with direct camera
  roots. A later ACT comparison found the half-turn makes the Axe edge-on and
  the extinguisher's reverse face the camera; neither render candidate is
  confirmed against retail. Exact held-item matrices and user comparison
  remain open.
  The extinguisher action/contact pose still drives spray. These roots and
  held-item depth behavior are visual approximations; the visible retail draw
  root, flags, exact local rotation, and scale remain open. Axe damage now
  reaches the shared BModel hit leaf, with on-screen hit/miss feedback; exact
  callback timing, runtime candidate ordering, and hit audio remain open. Spray effects/audio remain
  open. Ghidra confirms goggles resources 300/301
  are full-width 640x32 screen rectangles; the viewer draws their copied alpha
  masks while goggles are selected. Left-click populates the source-filtered
  kind-2/kind-6 scan list within 2,000 units, then draws resource 703 at
  approximate host target positions for the source-derived 8.5-second window.
  Visor tint, marker size/placement and child animation remain provisional.
  Authored hints now draw from the retail resource-1020 atlas and alpha map,
  parse RGB markup, and use the source strict duration bounds; the synthetic
  panel is removed. Mode-2 layout, tint layers, wrapping, and tick rollover remain
  open. The viewer camera parent and rendered-frame cadence remain
  provisional. Training's
  `setashdensity` and `setrescuebot` script fields also reach their existing
  player state. The shared host now runs the delayed full mode-0 EVS pass after
  four host World updates, and inventory checks pass across all 13 rescue,
  stage, and Training packages. Retail callback cadence, final selected tool,
  and per-item deactivation callbacks remain open. See
  `tracker/evidence/scenario-loading/player_loadout_command_path.md`,
  `tracker/evidence/scenario-loading/delayed_world_evs_pass_port_2026-10-15.md`,
  `tracker/evidence/scenario-loading/extinguisher_scene_viewer_runtime.md`,
  and `tracker/evidence/scenario-loading/goggles_scan_viewer_runtime.md`.

- Read-only follow-up audits verified that the packaged Training binary
  includes the retail-atlas hint text, camera-relative equipment ACT draw calls,
  goggles visor masks and heatpulse target markers, and the per-frame Player
  fire-exposure handler. These audits confirm integration, not that those
  pixels or contact events were observed interactively. Goggles only have a
  source-backed target scan effect; general night vision is unsupported by
  current retail evidence. The Axe's exported `swing` track, source-shaped
  strict timer crossing, collision sweep, and BModel hit transaction now run
  in the shared viewer. The primary animation's ACT mapping, camera-space
  matrix, Genesis runtime hit point, candidate-list population/order,
  strike-service effects, and callback cadence remain open. See
  `tracker/evidence/scenario-loading/current_feedback_audit_2026-10-13.md`.

- 250 EXE functions have address-specific behavior notes; function activity is
  tracked separately from source recovery and port parity.
- Forty-four hash-pinned Genesis.dll functions are now decompiled and
  recorded in the separate `tracker/genesis_function_reviews.csv` engine
  ledger, including the MOTN/PATH reader, key-channel samplers, and quaternion
  interpolation entry point.
  They are not included in the EXE-only 250-function dashboard count.
- The new `.lev`/`.rrf` loader pass adds thirteen address-specific function
  reviews across catalog parsing, resource tokenization, bitmap/model records,
  menu initialization, and briefing/resource requests. Its progress history
  records 228 reviewed functions for the CSV-driven dashboard after the
  hint/goggles GUI functions and the fire-direction indicator path were
  reviewed.
- The entity-factory queue order is now mapped across a selected-level load:
  mode-1 EVS runs before World creation, WorldEntity appends BSP requests after
  the current pump's drain phase, and PlayerRescue requests are handled by a
  later pump invocation. Early shared `rescue.evs` tells to Player are dropped
  before Player registration. The shared viewer now runs the delayed full
  mode-0 pass after four host World updates; its inventory check passes on all
  13 rescue/stage/Training packages. Exact retail callback cadence and final
  selected-tool parity remain open. See
  `tracker/evidence/scenario-loading/entity_factory_queue_update_order.md` and
  `tracker/evidence/scenario-loading/delayed_world_evs_pass_port_2026-10-15.md`.
- The four code-bearing PE inputs have fresh hash-pinned inventories. The
  level-loading and renderer boundaries are mapped through the shipped
  Genesis.dll and D3DDrv.dll: GBSP v15 loading, bitmap decode/registration,
  actor material registration, Direct3D texture-handle creation, mip-level
  lock/unlock, textured polygon submission, and frame callbacks. The raw
  rescue.rrf audit separates sprite/effect/HUD alpha pairs from the embedded
  BSP face-to-TexInfo-to-TEXTURES base-image path. Ordinary face lightmap
  allocation, style accumulation, UV construction, and multiply composition
  are now traced and ported; alpha/reflective branches, effective Z-enable,
  row orientation, and pixel parity remain open. See
  `tracker/evidence/runtime/d3ddrv_render_path.md` and
  `tracker/evidence/runtime/genesis_render_asset_path.md`.
  FireRescue random spread, suppression, both hose/message contact routes,
  proximity gating, exposure dispatch, and the player burn/passout receiver
  now have fresh address-specific evidence. The shared owner-update to
  contact-manager traversal is also mapped through `0049BD20`, including the
  FireRescue category-bit callback path; the viewer still lacks equivalent
  typed contact lists, and rendered-frame cadence, Think/message ordering,
  child registration, and cross-system RNG order remain open.
- The ScriptActor name registry is shared across classes. `tell`'s first value
  is the case-insensitive actor name and the second is its command. Bank's
  `tell vault open` reaches the BModel ScriptActor handler; the handler's
  `open` branch changes BModel command state or queues an open action. The per-frame update maps state 3/4 progression, child-model motion, and one-shot timed command dispatch. A new map ledger joins 10 named `open` targets to their BSP actors across Bank, Boat A, Bar R, and Gas R. Retail PlayerRescue equip/select commands are statically traced from BSP/EVS through inventory state and now update the shared viewer's authored loadout; item callbacks, final selection after delayed EVS, live cadence, and runtime parity remain open. See tracker/evidence/scenario-loading/player_loadout_command_path.md.
- Subway A now has a static entity-to-handler trace for 17 automatic door BModels and 25 broken-door sound BoundsTriggers; all 51 animation children and all trigger collision models resolve. Sound-name lookup, deferred playback, sample/stream FMOD calls, and the 3D-attribute API are traced, and `SubDoor`/`Zapp3d` join to hash-pinned 3D-buffer definitions and WAVs. The resource-gate path and min/max-distance setter are statically mapped; runtime audibility/falloff and the remaining Subway rescue mechanics remain open. See `tracker/evidence/scenario-loading/subway_a_interaction_path.md`.
- The shared viewer now loads map EVS bundles, runs BModel `touch`/`untouch` bindings, applies authored time scale and named `open`/`close` commands, samples per-owner PATH state into render/collision, and plays bundled WAV resources with an approximate stereo/falloff path. Subway A's synthetic check passes. The earlier Bank teleport fixture passed `*Door` and `VaultConsole -> tell vault open -> vault` checks. The revised approach harness triggers `touch` and PATH motion without M, then deliberately waits for the full-open endpoint and centers with a side-step before it crosses; it does not test ordinary live approach timing or the user's reported route. Live contact, exact retail bound construction, callback ordering/cadence, audio parity, and `RCS_BoundsTrigger` scheduling are still open. See `tracker/evidence/scenario-loading/bmodel_door_runtime_port.md`.
- The retail Genesis.dll path pipeline is decompiled through `gePath_Sample`; the exporter decodes `MOTN 0.F0` / `PATH 0.F2` tracks for every copied BSP and preserves chunk 24 unchanged. Coverage is 1,235 paths / 2,470 tracks across 17 maps; all rotation tracks select slot 4 and all translation tracks slot 1, with 124 compressed-time lists per channel and no looping paths. A reusable nonloop mode-1 sampler ports quaternion sign continuity/slerp and translation tangent/cubic sampling from the retail DLL. The map-neutral viewer joins BModel `C_AnimModelN` children to those tracks and applies gameplay transforms to rendering and collision. `M`/`N` sample a separate inspection timeline; live BModel poses take precedence in the renderer, and touch/named commands drive gameplay state. Timing parity remains open. See `tracker/evidence/genesis-boundary/genesis_path_tracks_and_port_decoder.md`, `tracker/evidence/scenario-loading/bmodel_door_preview_gap.md`, and `tracker/evidence/scenario-loading/bmodel_door_runtime_port.md`.
- The old C++ Training workbench remains useful as the source-backed gameplay
  implementation base. It is still a Training host, not a game-wide runtime;
  Training remains a required catalog target and regression case alongside all seven campaign rescues.
- The legacy map explorer was audited as a reference viewer: all 17 BSP inputs
  are byte-identical to the current copied maps. Its geometry/material/motion
  views can help localize mismatches, but its rendering filters, static motion
  previews, and collision overlays are not retail behavior and are not port
  code. The audit and hashes are in
  [`MAP_EXPLORER_REFERENCE_AUDIT.md`](MAP_EXPLORER_REFERENCE_AUDIT.md).
- Provenance follow-up: the actor exporter currently imports an inherited ACT
  parser that is near-identical to `map_explorer/source/tools/extract_act.py`.
  Its decoded files are provisional inspection data, not independently
  derived port evidence. Verify or rederive the nested VFS, BODY, motion, and
  GeBm layouts from hash-pinned retail Genesis.dll behavior and raw ACT data
  before treating actor asset loading as source-backed. Do not use the legacy
  viewer/parser as port implementation.
- Its 1,005 used texture images across those maps were decoded and compared to
  the port BMP exports; all dimensions and RGB texels match exactly. This rules
  out image-palette conversion as the source of a texel-content mismatch. For
  the shared visible set, 179,562 polygons / 507,849 triangles also match in
  model, texture index, and per-corner UV. This shifts the visual investigation
  to lighting/lightmaps, alpha/material flags, GPU sampling, and dynamic model
  state. See `tracker/evidence/runtime/legacy_texture_rgb_comparison.md`.
- The GBSP exporter produces all 17 local BSP packages. `SceneData` preserves
  every exported JSON document, validates package identity/schema, and resolves
  package asset paths without case assumptions.
- The standalone viewer loaded all 17 packages through the C++ loader with
  zero unresolved mesh texture paths. Fresh floor sweeps and idle movement
  checks pass on all seven rescue maps; BMP decode succeeds for every unique
  texture file. Offscreen CGL render checks now confirm OpenGL texture upload
  and varied framebuffer output on all seven. See
  `tracker/evidence/runtime/scene_preview_collision_texture_diagnostics.md`.
- The user requires every rescue mission and map/mechanic to remain in scope,
  with a cross-platform desktop port and macOS as the first playable target.
- Gas R BSP entities 265, 280, and 309 are joined to the explosion1, explosion2, and gasoff EVS definitions. ScriptActor queue pop/order and wait deadlines are mapped, alongside BModel button1 use handling, type-1 contact candidate checks, and shared BoundsTrigger overlap-pair → touch/touching/untouch dispatch. The named-model motion join resolves button1's effective reach to 20 units. Live contact/selection, updater cadence, and original-game effect captures remain open. See the Gas R evidence linked in the scenario-loading README. The same retail Genesis.dll MOTN/PATH extent decode is now joined across Training and all seven rescue maps: 71 of 76 authored-gate BModels receive a static 48-unit reach; five motionless ladder/button models retain authored values. Per-map/entity results are in `tracker/evidence/scenario-loading/bmodel_reach_cross_map.md`, its JSON inventory, and the rescue coverage CSV. This closes only the static reach calculation; live contact and callback cadence remain open.
- The shared Script host now routes the retail FireRescue `setsize x y z duration` form to the targeted actor's timed `FireSize` track, including the authored key reset/current-size anchor and the `clock + duration * 1000` target. This enables Gas R `button1 -> gasoff`'s six size commands without a map-specific branch. The port uses SDL ticks and host frame updates; live button use, retail visual timing, and original-game effect parity remain open. Boat A's separate exploder `setsize 3` path is not covered. See `tracker/evidence/scenario-loading/firerescue_setsize_script_port.md`.
- `tracker/rescue_mission_coverage.csv` explicitly tracks Training plus all
  seven campaign rescue missions. All eight load as geometry packages in the
  inspection viewer; none is yet a playable rescue port. `level_map_inventory.csv`
  and `scene_load_coverage.csv` cover the other catalog entries and all 17 BSPs.
- `tracker/rescue_actor_inventory.csv` maps 564 authored player, fire, victim,
  alternate-victim-spot, and PathPoint entities across all 17 BSPs. Fire/victim
  counts reconcile to the level inventory. Raw epairs and positions are
  retained for map-neutral spawn conversion; current-hash code confirms the
  VictimRescue and VictimAltSpot factory path and registration-before-spawn order.
- Victim `roam <number>` traces from the class-specific ScriptActor fallback
  through obstacle collection, initial-position checks, animation selection,
  target selection, and state-7 movement. Obstacle kind 6 is tied to
  `RCS_FireRescue`, and its virtual radius getter reads root `+0x88`; the static
  host uses authored `H_Radius` pending proof of the retail UserData binding.
  The viewer now uses the source-derived candidate/retry/player gates and fire
  clearances. `forcespawnspot` selects from the victim's registered spawn list.
  The actor inventory summarizes the exact
  non-empty victim script values across every BSP; the alternate-spot name
  resolver joins all 45 authored spots to a victim on the same BSP. Numeric-only
  fields and update cadence remain unresolved. The current-hash factory trace confirms alternate-spot registration before victim spawn selection across the authored rescue BSPs. See
  `tracker/evidence/scenario-loading/victim_roam_behavior.md` and `tracker/evidence/scenario-loading/victim_spawn_factory_and_order.md`.
- The shared victim pickup path now maps contact overlap through Message Mapper
  message `0x7532`, the Player's empty-carry gate, and the VictimRescue `pickup`
  event. The shared escape/release path updates rescue statistics and victim
  state, then applies final-exit gates. A generated 17-BSP inventory records 40
  authored exit triggers, joins all 40 `C_Model` values to named BSP models, and
  records Boat, Subway, and Oil Rig stage destinations. `tell player` now traces through the static PlayerRescue factory and handler path;
  trigger/update cadence and live mission parity remain open. See
  `tracker/evidence/scenario-loading/victim_pickup_message_path.md`,
  `tracker/evidence/scenario-loading/rescue_completion_and_exit_path.md`, and
  `tracker/rescue_exit_trigger_inventory.md`.
  The Player's resource-55 pickup-status sprite is now bundled and animated
  from its four source-traced keyframes; a packaged render check and rescue
  transaction check pass on all 13 Training/rescue packages. HUD blend and
  update cadence remain open, and this does not resolve the carried body's
  Player-relative pose. See
  `tracker/evidence/scenario-loading/player_victim_pickup_status_hud_2026-10-15.md`.
  The current packaged and CMake executables match at SHA-256
  `33a65577afce9a3a144507aad542067d6357da69df6f2bc98509a24887b95ac8`.

## Delegated audit ledger (2026-10-13)

- **Fire viewer integration audit — complete, read-only.** Reviewed
  `scene_viewer.cpp`, the FireEntity/FireRenderer components, and the app
  bundler. Its recommended entity-order initialization, explicit sprite root,
  and draw integration are implemented and documented in
  `tracker/evidence/scenario-loading/fire_entity_viewer_runtime.md`.
- **Material alpha and Bank collision correlation — complete, read-only.**
  Reviewed the scene exporter, world mesh batching, texture loading, trigger
  mesh join, pinned Genesis/D3D render evidence, and the new model-0 approach
  trace. Confirmed the collision hit is not the `wal_Stco_wall` start-view face
  candidate; retail world sweeps and animated BModel transforms are separate,
  but the exact Bank BSP leaf/aperture remains unknown. No agent edits were
  made.
- **Trigger visibility audits — complete, read-only.** `trigger_visual_audit`
  joined all 25 Training BoundsTriggers to their rendered BSP models and
  confirmed each includes authored `col_red`; `trigger_retail_flags` traced
  retail `ModelSetFlags(0)` setup and the mode-gated update pulse. Integrated
  in `scene_viewer.cpp` and documented in
  `tracker/evidence/runtime/trigger_brush_render_visibility.md`. The normal
  render bit and host pulse cadence remain inferred/provisional.
- **Fog renderer audit — source trace and first shared port complete.**
  `fog_renderer_trace` followed `fogenable`/`fogdisable` through
  `004789D0`/`00478AC0`, the state setters, `0049CF10` interpolation and camera
  far-clip update, and the Genesis `geEngine_SetFogEnable` call. The shared
  viewer now applies the two-second transition, retail channel quantization,
  authored `*DefaultFogParams`, and camera far clip. Startup targets and Gas R
  inside/outside triggers are covered by packaged checks. The helper-to-driver
  path is recorded in `tracker/evidence/runtime/fog_helper_to_driver_path.md`;
  the constructor's initial tuple, exact cadence, and pixel-level parity remain
  open.
- **Ladder runtime audit — complete.** The pinned Player sweep supplies mask
  `0x43` with null optional model filters; the Genesis model loop has no mapped
  ladder-specific bypass. Training entity 108 keeps authored `E_Reach=5`, and
  model 5's solid leaf contents `9` matches the Player mask when the model is
  enabled. The runtime model flag remains unknown. A two-unit hull-clearance
  position can still overlap the expanded contact bounds, so the fixture now
  samples those boundaries while preserving collision. See
  [`training_ladder_contact_fixture_2026-10-15.md`](../tracker/evidence/scenario-loading/training_ladder_contact_fixture_2026-10-15.md).
- **Bank collision/material follow-up — complete, read-only.**
  `bank_contact_review` checked the live-loop ordering and found no M-to-
  collision bypass; the ordinary user route remains untested.
  `bank_retail_collision_trace` confirmed retail Player bounds at
  `0044D550` as mins `(-30,-30,-30)` and maxs `(30,85,30)`; the port matches
  them. `mesh_metadata_audit` reviewed source face/span correspondence, and
  `retail_material_trace` found no live Ghidra session or source-backed retail
  blend/depth/lightmap semantics. Their scopes were read-only; findings are
  recorded in the Bank evidence notes.

## Next tasks, in order

### User-reported desktop blockers (2026-10-14)

- **Held tools and fire-camera turn (current user report):** the latest Training
  package includes an SDL focus/relative-mode guard: the viewer releases mouse
  capture on focus loss, restores it on focus gain when capture was requested,
  and ignores relative motion unless capture, relative mode, and input focus
  are all active. Its pitch cap now matches a fresh DifficultyLevel 1 profile
  (±40°), and crouch view height is 10 as in state 4. Packaged Training render
  and Axe/ext orientation diagnostics pass. Fresh Ghidra helper decompilation
  confirms retail's hierarchical equipment component/parent-frame matrix
  writer, but no live ActorInfo camera-space
  values are available to justify a facing/root correction. Retail fire
  contact does not write yaw/pitch; retail state 6 intentionally blends the
  view for 1 second after passout. The shared viewer now runs this interval
  through the mapped full-frame `ViewUpdate` path; selected-item offsets and
  live camera parity remain open. The user-reported live turn remains open
  until it is correlated with passout vs. relative mouse
  input. The latest package was rebuilt from source; `--render-check` loaded
  121/121 groups and 24/24 textures, and Axe/ext orientation checks drew 110
  and 313 triangles. These and the hint/trigger checks are port-only checks.
  Same-view retail tool comparison and a still-input
  `--fire-camera-audit` remain required before marking parity. Latest
  packaged/CMake binary SHA-256:
  `b44840eced38f76a3639271ea593b77cbc856fe0614ed37b5b9341a441da8529`.
- **Door motion/collision:** Bank's authored door route now applies its source-derived openaway direction and PATH motion from contact; M/N only control the inspection timeline. The fresh continuous route to entity 541 dispatches touch, plays `OpenDoor`, and reaches PATH 1 without M, then stalls on static world model 0 at X offset 24.011. The endpoint/alignment control crosses at X=-719.419. Retail exports confirm swept collision checks static and transformed models and BModel PATH updates model transforms; the exact BSP aperture and live ordinary route remain open. Reproduce from a new app launch and compare touch dispatch, PATH/render/collision pose, player position, and world hits. Callback timing remains provisional.
- **Bank start/render appearance:** the user expects an open start but sees an opaque enclosure. The fresh EVS-enabled Bank offscreen check submitted all 497 mesh groups, decoded/uploaded 105 base textures, and uploaded 9,474 face lightmaps through the shared atlas. The camera still shows a dark enclosure that has not been matched to the user's interactive view. Retail alpha/reflective state remain open. A pinned Genesis/D3DDrv trace confirms row-preserving uploads and positive retail V progression through source rows; the exporter lightmap V flip was removed. Final pixel parity remains open. Keep the visual diagnosis distinct from player collision. See the Bank start-view surface audit and Genesis face-lightmap handoff.
- **Bank performance:** the Bank hot path now skips FireRescue LOS traces outside the retail 500-unit gate/falloff, caches collision-model bounds, draws mesh buffers through OpenGL client arrays, culls static groups outside the camera frustum, and caches fire sprite durations at initialization instead of allocating once per fire draw per frame. Moving/posed BModels bypass culling. Bank render, door/vault interaction, and victim-release checks pass; the render-check image is unchanged. The user now reports Bank framerate seems sorted. The shared `00502B04` source is traced to WinMM elapsed milliseconds, a `>1/60` pacing gate, and a `0.1` cap, now ported through SDL; same-route retail FPS, focus-pause timing, and presentation behavior remain open. See [`bank_runtime_performance_followup_2026-10-14.md`](../tracker/evidence/runtime/bank_runtime_performance_followup_2026-10-14.md) and [`retail_frame_pacing_audit_2026-10-14.md`](../tracker/evidence/runtime/retail_frame_pacing_audit_2026-10-14.md).
- **Extinguisher, hint windows, and damage feedback:** Training entity 161 `ext1` is authored as PropModel `D_Model=2` (`ext.act`) at `(780,-164,-1748)`, with 70-unit touch radius and `touch:*RefillExt`; it has no `E_Use`. That macro equips/refills the extinguisher and plays `Steam4`. The viewer now exports and draws world PropModels and routes player-point/radius contact through authored script events, including `ext1`. Its contact path applies the lower-level cluster-PVS rejection, while the runtime area matrix/current-view semantics and update cadence remain open. Training build and render checks pass, but no live contact or retail capture has confirmed the pickup yet. Retail hint routing `0044EE50→004564C0` constructs a GUI text widget at Player `+0x6D8`; its resource-1020 atlas, glyph grid, color markup, and strict timeout are ported, while mode-2 placement and overflow remain open. No generic full-screen damage overlay was found in the bounded audit. See [`propmodel_world_actor_port_2026-10-14.md`](../tracker/evidence/scenario-loading/propmodel_world_actor_port_2026-10-14.md) and the delegated ledger above.
- **Fog and remaining red surfaces:** Training's solid red blocks are now
  joined to 25/25 authored BoundsTrigger models using `col_red`; retail setup
  sets those model flags to zero, and the shared viewer honors that model
  state. A repeated packaged check reproduces the forced-visible red brush and
  its removal at flags zero. Oil Rig B entity 78 has no `C_Model`; retail's
  contact check requires the model pointer, so the port skips it without
  fabricating bounds. The flag-to-normal-render bit remains inferred. Keep
  investigating Bank/Gas R alpha-128 red faces and implement face alpha/blend/depth
  generically. Fog is separate: Gas R binds `fogenable 5 320 60 40 40` to
  `inside` and `fogdisable` to `outside`; Bank/Boat also author default fog
  tuples. The dispatcher passes five script values to `004789D0` and
  `fogdisable` calls `00478AC0`; the shared viewer now applies the two-second
  tuple transition, generated channel quantization, and camera far-clip update.
  The constructor's initial tuple, exact cadence, and pixel-level parity remain
  open. See
  `tracker/evidence/runtime/trigger_brush_render_visibility.md` and
  `tracker/evidence/runtime/fog_helper_to_driver_path.md`.
- **Ladder interaction:** the viewer's `--interaction-check` now samples
  hull-clearance boundaries around the solid Training ladder while preserving
  its five-unit authored contact reach. The symmetric source-derived point
  `(-284,-384,-44)` clears model 5 by two units and remains inside contact
  bounds; nearby world collision can reject a candidate. The macOS viewer
  rebuilt, and Training's packaged `--interaction-check` selected
  `(-212,-384,-44)`, enabled climbing, moved upward `3.15762` units for one
  `Player::step`, and disabled climbing on untouch. Live callback scheduling,
  model flags, animation, and camera behavior remain open. See
  [`training_ladder_contact_fixture_2026-10-15.md`](../tracker/evidence/scenario-loading/training_ladder_contact_fixture_2026-10-15.md).
1. **Confirm shared doors and triggers against an interactive run.** The current
   packaged rescue-all app passes `--interaction-check` for Bank and Subway A.
   Bank entity 541 receives touch, plays `OpenDoor`, and reaches PATH 1; the
   straight route stops on static world model 0 at a 24.011-unit lateral
   offset, while the centered control crosses. Subway A entity 274 opens at
   speed 4, closes on untouch, and trigger 515 dispatches `Zapp3d`. These are
   controlled headless checks. Exact BSP aperture, ordinary user route,
   callback cadence, contact timing, live exit scheduling, and full mission
   completion remain open. Do not change the Player hull or disable the wall
   without new source evidence.
2. **Close remaining oxygen and hazard parity gaps.** The shared oxygen
   resource, authored commands, and state-6 passout/drop/reset transaction are
   integrated in the host. Retail `00458400` smooths the resource into
   Player status widget `+0x28d0`; its 96-by-96 rectangle, center, rotating
   needle, render dispatch, `geEngine_RenderPoly` path, and resource IDs 12/13
   are source-traced. The viewer draws the retail gauge and needle bitmaps.
   Resource 702's oxygen visor transition now uses the source-mapped UV crop,
   640-by-256 rectangle, exact endpoints, strict timing window, and child
   sounds. `--oxygen-hud-render-check` verifies its midpoint timing and pixel
   output alongside the gauge layers and low-warning overlay. The orange
   resource-12 warning face and its source controller formula are mapped; the
   host uses an approximate alpha ramp. Retail warning/visor blend parity and
  category-volume effects of selectors 4/5 now control the viewer's group mix;
  the manager flag remains open; separate level-music gain is wired and
  documented in the 2026-10-17 checkpoint. The
   source-triggered `RC_OxygenLow` one-entry chatter cue now plays its authored
   `RC_OxygenLow0`/`o2low.wav` resource through the shared audio mixer and is
   bundled in all 13 rescue/stage packages; retail mix/timing parity remains
   open. The separate Player sound instances also play `Breathe` on each due
   drain tick, stop it on `oxyoff`, and play `OxyRelease` on full recovery from
   a low-resource state. The four 2D oxygen/visor WAVs are source-hash checked
   in all 13 packages. Mixer output parity remains open. See
   [`player_oxygen_low_warning_sound_port_2026-10-15.md`](../tracker/evidence/scenario-loading/player_oxygen_low_warning_sound_port_2026-10-15.md)
   and
   [`player_oxygen_breathe_release_sound_port_2026-10-15.md`](../tracker/evidence/scenario-loading/player_oxygen_breathe_release_sound_port_2026-10-15.md)
   and
   [`player_oxygen_visor_effect_port_2026-10-15.md`](../tracker/evidence/scenario-loading/player_oxygen_visor_effect_port_2026-10-15.md).
   Retail state 6 calls `0044FF10(player, 0)` only when unsigned elapsed time is
   strictly greater than 1000 ms. The viewer now wires the safe-position and
   checkpoint fields, carried-victim drop, default cleanup/status tail, and
   timed one-shot recovery. The state-6 camera blend now uses the mapped view
   helper; equipped-item view offsets, mode-2 HeroDown spawning, retail callback
   cadence, and camera parity remain open. The passout adapter check is
   controlled and does not represent a natural hazard event. See
   [`player_passout_recovery_port_2026-10-14.md`](../tracker/evidence/scenario-loading/player_passout_recovery_port_2026-10-14.md)
   and the oxygen/held-tool camera evidence.
3. **Material and render parity.** The current Bank render check submits every
   exported mesh group and decodes/uploads all referenced textures, but its dark
   obstruction is not matched to the user's start view. Face identity, TexInfo
   alpha/flags, and lightmap references now survive the export and loader path.
   A hash-pinned Genesis.dll trace verifies face LIGHTDATA's `LightOfs + 1`
   prefix, RGB sample planes, per-style Q8 accumulation, and a point-query
   sample grid; the copied maps validate the source layout across all 17 BSPs.
   Face projection and descriptor lightmap minima are mapped. A fresh,
   hash-pinned Ghidra trace now follows that descriptor through Genesis's
   direct/deferred queue into D3DDrv `+0x80`; the driver's installed Genesis
   `+0xB0` callback fills the face RGB buffer and returns an update flag.
   Both ordinary base UV construction and the secondary lightmap UV formula
   are mapped from the shared raw TEXINFO projection pair; the lightmap path
   adds 8 units, subtracts descriptor minima, and divides by `16 * 2^mipShift`.
   The D3DDrv trace now maps both ordinary composition routes: a two-pass
   fallback with ZERO/SRCCOLOR blending and a two-stage TEXTURE×CURRENT path.
   Frame begin sets ZWRITE and LESSEQUAL, linear min/mag filtering, and the
   ordinary alpha blend; the lightmap stage clamps address U/V. Effective
   ZENABLE, mip filtering, runtime path for the selected device, the optional
   face sub-branch, host render-call rate, channel convention, float-to-int rounding,
   and full pixel parity remain open. Genesis advances style strings once per
   `geEngine_RenderWorld` call; the host cadence is not known. See
   [`genesis_lightdata_rgb_decode_2026-10-15.md`](../tracker/evidence/runtime/genesis_lightdata_rgb_decode_2026-10-15.md)
   and [`genesis_face_lightmap_driver_handoff_2026-10-15.md`](../tracker/evidence/runtime/genesis_face_lightmap_driver_handoff_2026-10-15.md).
   Keep player collision separate from appearance and do not add guessed
   visibility rules.
   The RGB texture and shared face/UV comparison is recorded in the legacy
   texture comparison evidence; use the old explorer only to localize a
   mismatch.
4. **Desktop actor check.** The rebuilt MBase bundle includes the map and
   decoded victim ACT assets. Confirm victim placement, motion, contact pickup,
   and authored escape trigger behavior in the interactive macOS window. Try
   walking into a victim, then through `escape1` (bounds center near
   `1906, -124, -2788` in the viewer's displayed coordinates). The title should
   show carry and rescue state; carried-body presentation remains missing.
   Record the map/camera when a base
   texture binding still appears wrong; the current audit found no proven
   face-to-texture index mismatch. Lighting, lightmaps, alpha, and window
   presentation parity remain visual follow-ups.
5. **Runtime catalog/resource resolver.** The `.lev`/`.rrf` loader trace is
   recorded in `tracker/evidence/scenario-loading/lev_rrf_catalog_loader_trace_fresh.md`.
   Implement catalog-selected `SceneData` creation with explicit case aliases for
   BSP, briefing, EVS, actors, motions, sounds, and textures. Retain the original
   resource spelling and source hash on each resolved row.
6. **Promote the viewer into the shared host.** The viewer now loads actors,
   starts the traced victim roam update, and applies proximity pickup. Extract
   reusable startup, movement, collision, renderer, and asset loading from the
   Training `main.cpp`; keep the current Training executable as a regression
   fixture. Reconcile the ported roam behavior with exact native turn and move
   helpers; no map name or Training assumption belongs in the shared loader.
7. **Wire rescue entities and script startup.** Derive common typed player,
   fire, victim, bounds-trigger, BModel, prop, and exit records from the raw
   ordered epairs. Preserve unknown classes and properties. Trace MBase's
   EVS/shared-EVS commands and original callback cadence before dispatching.
   Retail `moveto` traces through FireRescue command parsing, shared ScriptActor
   name lookup, state-4 update, timed-key generation, and visual vector
   interpolation. The shared viewer now resolves PathPoints and moves the
   original fire with source-derived progress; for authored no-`C_Model` route
   fires it also applies the `H_Radius/2` visual-update gate. Player exposure
   messages now drain after queued spread-child creation. Retail route-trail
   reuse/deactivation and recurring update ownership remain unresolved; exact
   transform/floor behavior, timing-key use, and callback cadence remain open.
   Bank's `vault open` command maps from map EVS
   through the shared registry to the BModel-specific handler. The PathPoint initializer
   passes its authored name into the ScriptActor field searched by that lookup.
   MBase's `setashdensity 0` route now maps to the clamped Player scalar at
   `+0x5b0`; the MBase `tell` is delayed until mode 0, but its order relative to
   Player registration remains open. The previous ash-pool analysis and native
   clamp implementation are reused.
   Determine the remaining PathPoint field meanings, the exact authoring/runtime
   intent of the two trailing `moveto` timing values, the external
   scheduler/cadence for `UpdateFireRescueActor`, and the ten `S_Script`
   event-command semantics. The VictimRescue `roam` path is statically mapped;
   continue with runtime capture of Bar R's unhandled `setskin 2` fields,
   numeric-only victim script values and victim update cadence; the current-hash
   factory path and registration-before-selection order are now mapped. Capture live command timing and the final selected tool per scenario; the static
   Player identity/recipient route is now mapped, and every rescue mission and
   stage remains in scope. Ordered PlayerRescue equipment commands now update
   shared viewer inventory state, and the delayed full EVS pass now runs after
   four host World updates. Retail callback cadence, final selected-tool
   parity, item actions, and goggles visibility remain open. The authored
   `ambientsnd` path now registers source-positioned FMOD-style loop requests in
   the shared viewer audio host; full audio-output parity remains open. See
   `tracker/evidence/scenario-loading/ambient_sound_loop_port_2026-10-15.md`.
8. **Complete the rescue gameplay path across the catalog.** Controlled checks
   now exercise pickup, release, passout recovery, and authored stage commands
   on all 13 rescue/stage packages. The six linked-stage bindings pass their
   remaining-victim gate and load matching siblings. Live rescue travel,
   carried-body presentation, trigger scheduling, full mission objectives, and
   retail parity remain open across Training and the seven campaign rescues.
   Named FireRescue startup now selects and plays its pinned `FireBurn1`–`3`
   3D loop resources; `--scene-audio-check` passes all 13 bundles and verifies
   370 named-fire voices. See
   `tracker/evidence/fire-spread/firerescue_loop_audio_port_2026-10-15.md`.
   Check each mechanic while comparing behavior with pinned EXE evidence.
9. **Cover the complete campaign and platforms.** Track each rescue catalog
   scenario, then all hose/other catalog entries and the remaining BSP
   variants. The shared viewer now includes SDL2's cross-platform OpenGL header;
   the macOS viewer rebuild passes, while Linux/Windows builds remain
   unverified. Add native Windows/Linux builds against the same scenario
   contracts; see `tracker/evidence/runtime/cmake_platform_boundary_2026-10-15.md`.

## Parallel review results already incorporated

### Current held-tool and fire-camera audit (2026-10-14)

| Agent | Scope | Files reviewed | Status |
| --- | --- | --- | --- |
| `hand_pose_audit` | Trace retail equipment matrix and compare viewer root | `scene_viewer.cpp`, `view_update.h`, `head_motion.h`, retail transform evidence | Complete, read-only; no supported facing correction; live ActorInfo values remain missing |
| `camera_audit2` | Resolve state-6 blend timing and required view inputs | `00451080` decompilation, view-call fixture/dataflow, fire adapter | Complete, read-only; verified one-second passout blend; current adapter lacks live inputs |
| `passout_view_adapter` | Assess safe state-6 view integration | `scene_viewer.cpp`, `ViewUpdate`, native workbench | Complete, read-only; full-frame camera renderer and selected-item release/view state are missing |
| `tool_pose_matrix_trace` | Resolve retail frame parentage for the backward Axe/ext models | Fresh headless Ghidra pass; `.local/held-transform-audit2.log`; equipment actor path note | Complete, read-only; child animation parent and component frame parent are separate; live component/ActorInfo values remain missing |
| `fire_camera_runtime_trace` | Recheck the runtime yaw/pitch path and state-6 behavior | `scene_viewer.cpp`, `00451080`, passout and view-call notes | Complete, read-only; fire has no yaw writer; passout has a one-second retail view transition; stationary live trace remains needed |
| `camera_crouch_review` | Verify current pitch cap and crouch camera-height edits against retail evidence | `scene_viewer.cpp`, pitch-writer README, held-tool camera note | Complete, read-only; ±40° is the fresh-profile default and height 10 matches state 4; neither causes yaw rotation |
| `hand_pose_trace` | Recheck the retail Axe/ext transform chain for a supported orientation correction | Ghidra project copy/log, held-tool camera note, `scene_viewer.cpp` | Complete, read-only; mode-2 component reaches ActorInfo but live matrix/native forward axis remain unknown; no flip/offset edit justified |
| `held_pose_fresh_trace` | Recheck ACT local axes against the retail equipment matrix chain | Current ACT exports, pinned Ghidra notes, `scene_viewer.cpp` | Complete, read-only; both ACTs have rotated root attachments and final camera-space pose is still missing; no facing correction justified |
| `camera_passout_trace` | Recheck retail state-6 blend endpoints/timing and viewer angle writers | `00451080`, `00450FB0`, `00451640`, `scene_viewer.cpp`, `view_update.h` | Complete, read-only; one-second passout view transition verified, but full live frame inputs are missing; fire/pain do not write viewer yaw/pitch |

The latest source-backed iteration follows the retail passout transaction and
camera defaults:
entering state 6 releases the selected item's `+0x1C` callback. The shared
viewer now ends an active extinguisher action and rejects mouse-look while
incapacitated. It still does not render retail's one-second `00451080` view
blend, because the selected-item offsets and full live camera frame are not
available. The held tool roots remain provisional; Ghidra's `0042D2D0` parent
pointer at frame `+0x40` and the Axe child's animation parent at `+0x0C` belong
to distinct chains. The current Training binary is hash-matched, and the
packaged render, Axe/ext orientation, hint, and trigger checks pass. The
fresh-profile pitch cap and state-4 crouch height follow source-backed
defaults. These checks do not establish retail visual parity; held roots,
loaded-profile pitch state, state-6 frame rendering, and the live camera
symptom remain open.

### Current execution ledger (2026-10-13)

| Agent | Scope | Files reviewed | Status |
| --- | --- | --- | --- |
| `door_evidence_audit` | Audit retail BModel command/contact behavior and Subway A/Bank fixtures | `bmodel_update.md`, `bmodel_door_speed_semantics.md`, `subway_a_interaction_path.md`, `bank_vault_open_dispatch.md`, `rescue_contact_manager_route.md`, fresh Ghidra exports | Complete; evidence integrated into the runtime port note |
| `door_host_architecture` | Identify the shared viewer integration seam and build/launch route | `scene_viewer.cpp`, `scene_data.h`, `gbsp_motion_path.h`, `bmodel_contact.h`, Training workbench notes | Complete; architecture integrated, no overlapping edits |
| `rescue_crash_audit` | Read-only audit of the headless `--rescue-check` crash | `scene_viewer.cpp`, `victim_transactions.h`, release/frame helpers | Complete; static review only because its environment lacked exports; root reproduced and fixed the no-GL headless actor-loading fault, then recorded the passing MBase check |
| `carry_pose_audit` | Read-only audit of retail carry presentation and actor render behavior | `victim_pickup_message_path.md`, `post-pickup lifecycle evidence`, `scene_viewer.cpp`, actor render flags | Complete; pickup hides the world actor as source-backed; no retail carry anchor or continuous pose path is established, so do not invent transform math |
| `door_m_runtime_audit` | Audit reported door that sounds on approach but moves only after `M` | `scene_viewer.cpp`, BModel runtime notes, Bank/Training authored EVS | Complete; `openaway` host gap is implemented; follow-up found the synthetic fixture selected distant entity 365, so the check now selects nearest start door 541; live approach and collision remain unresolved |
| `bank_trigger_render_audit` | Identify Bank start enclosure and assess whether alpha is responsible | `bank/entities.json`, `world_mesh.json`, trigger model resolver, bounds flags evidence | Complete; the fresh normal render submits all 497 world mesh groups. Red exit-trigger faces with alpha 128 are a candidate, not a match to the user's view; retail material semantics remain open |
| `door_live_path_audit` | Explain the M-only door report against current live and headless paths | `scene_viewer.cpp`, `core.h`, `scripts.h` | Complete; M/N affect inspection only, while the harness waits for full-open and aligns before passage; exact live route remains open |
| `bank_start_render_audit` | Recheck Bank camera, render obstruction, and bundle provenance | `scene_viewer.cpp`, Bank exports, app bundle, launch scripts | Complete; a fresh offscreen render shows an unmatched dark polygon with all 497 mesh groups submitted. Exact user-view match and material behavior remain open |
| `fire_integration_audit` | Read-only review of fire simulation/contact integration seam | Fire entity, suppression, exposure helpers, scene viewer, fresh EXE evidence | Complete; no edits; terminal visual shutdown is unsupported and contact scheduling remains open |
| `victim_equipment_audit` | Read-only review of victim carry/rescue and equipment/goggles integration seam | Victim transaction helpers, Player loadout trace, scene viewer, fresh EXE evidence | Complete; no edits; authored PlayerRescue loadout integration implemented by root; item behavior remains open |
| `extinguisher_port_seam` | Read-only review of generalizing Training extinguisher contacts into the shared viewer | `main.cpp`, `scene_viewer.cpp`, extinguisher/fire helpers, fresh suppression evidence | Complete; no edits; identified the camera-parent/cadence boundary; shared message path integrated |
| `goggles_visibility_audit` | Read-only trace of goggles inventory callbacks and visibility effects | Player loadout evidence, goggles initializer/vtable, scene viewer | Complete; no edits; slot 3 is verified, visibility semantics remain unknown |
| `fire_spread_seam` | Read-only audit of random spread integration and callback dispatch | `scene_viewer.cpp`, FireEntity components, fire-spread evidence, shared contact manager | Complete; no edits; dispatcher traversal is source-mapped and spread is now wired provisionally; candidate lifecycle, RNG ordering, and owner cadence still block faithful parity |
| `goggles_trace` | Read-only trace of goggles vtable, selection callbacks, and scan visibility behavior | Pinned EXE vtable/data, goggles exports, Player selection path | Complete; no edits; verified target filter/range and callbacks; resource 703 and the 8.5-second effect window now drive the shared heatpulse renderer; child animation and broad visibility remain open |
| `goggles_effect_review` | Read-only review of preview marker fidelity against the retail target update | Goggles renderer, scan runtime note, pinned EXE disassembly | Complete; no edits; corrected host billboard description; retail actor-local transform and child attachment remain open |
| `fire_exposure_integration` | Read-only review of Player fire exposure/passout delivery seam | `player_fire_message.h`, exposure/passout helpers, scene viewer, fresh FireRescue/Player evidence | Complete; no edits; documented missing cadence, LOS, player-effect/audio/stat bindings, and passout host services; do not wire guessed automatic scheduling |
| `spread_trace_audit` | Read-only audit of current-hash random spread semantics and deferred creation seam | `00447A90_fresh.c`, `00448320_fresh.c`, contact-manager route, scene viewer | Complete; no edits; confirmed shape/queue details and the pointer-at-root-+0x48 / pointed-to-+4 gate; root follow-up mapped it to string length |
| `player_fire_candidate_trace` | Earlier read-only trace of PlayerRescue candidate position for spread and fire exposure | Player vtable/getter exports, FireRescue callback evidence, scene viewer | Superseded by `player_position_getter`; the earlier claim that PlayerRescue's slot mapping was unproven is corrected by current-hash vtable bytes |
| `fire_48_subobject` | Trace FireRescue root +0x48 construction and default state | `0044a330_firerescue_constructor.c`, `0046f930_FUN_0046f930.c`, startup initialization | Complete; read-only; found the shared empty-string pointer; initializer setter chain was completed with retail disassembly |
| `fire_gate_writers` | Trace writes to the spread gate and shared state | `00448320_fresh.c`, `00447A90_fresh.c`, `0046e320_FUN_0046e320.c`, `00448d60_FUN_00448d60.c` | Complete; read-only; initial shared-state interpretation was superseded after resolving 00448D60's string setter |
| `fire_name_gate` | Map the spread gate to authored FireRescue A_Name | `00447A90_fresh.c`, `00448320_fresh.c`, FireRescue UserData audit, retail 00448D60 disassembly | Complete; root follow-up proved the embedded string receives A_Name and the gate reads its length; unnamed generated children retain an empty string |
| `player_position_getter` | Read-only trace of PlayerRescue candidate vtable slot +0x0C and exposure padding | `memory_004e1c40_player_rescue_vtables.json`, `00453d50_player_rescue_object_constructor_fresh.c`, `player_fire_exposure_padding_fresh_2026-10-20.c` | Complete; current-hash slot/getter map origin to root +0x4C..+0x54 and fourth falloff value to constructor-zeroed +0x74; port uses zero; callback cadence and exhaustive later-alias audit remain open |
| `mbase_moveto_audit` | Audit the authored MBase FireRescue route against the shared host command/runtime | `mbase.evs`, MBase BSP entities, `rescue-moveto` evidence, `scripts.h`, `scene_viewer.cpp` | Complete; audit mapped PathPoint registration, `moveto` setup, state-4 progress gate, and deferred FireRescue request creation; generic drain `0049C670` later creates/registers queued objects. Follow-up confirms `0049D450` assigns route identity, root slot `+0x24` is a no-op, and per-instance reuse/deactivation plus recurring update ownership remain unresolved. Shared host applies the `H_Radius/2` gate; model-derived threshold, exact transform/floor behavior, and callback cadence remain open |
| `bank_door_audit` | Audit the reported Bank M-only door symptom against live loop and controlled harness | `scene_viewer.cpp`, `bmodel_door_runtime_port.md`, Bank interaction evidence | Complete; no edits; M/N only affect inspection state; continuous aimed route opens PATH but hits static world model 0 at a 24-unit lateral offset; aligned control crosses; live route remains open |
| `bank_collision_retail` | Trace retail swept collision and BModel transform path against Bank door result | Pinned EXE/Genesis exports, Bank entity/model inventory, `bmodel_door_runtime_port.md` | Complete; read-only; swept hull checks static and transformed models, BModel PATH updates Genesis model transforms; exact leaf and live route remain open |
| `bank_contact_review` | Audit Bank live-loop contact/PATH ordering and reported M workaround | `scene_viewer.cpp`, Bank door route and interaction fixture | Complete; read-only; no M-to-collision path; live ordinary route still needs capture |
| `hose_catalog_audit` | Identify safe catalog and bundle seams for the four hose maps | `build_rescue_mission_catalog.py`, `build_scene_viewer_app.sh`, `mission_selector.h`, `scene_viewer.cpp`, current scene exports | Complete; read-only; its separate direct-runner recommendation is implemented with all four hose maps packaged; a retail hose catalog/selector remains open |
| `hose_runtime_data_audit` | Find a source-backed first playable PlayerHose data slice | `city_h/entities.json`, `scene_data.h`, `startup.h`, `scene_viewer.cpp`, lifecycle evidence | Complete; read-only; city_h gives a linear first hotspot path and its 29 ZF names resolve to `%Model%`; no edits; live route, exact camera, timing, collision, and WindowFire effect presentation remain open |
| `hose_pressure_trace` | Trace dedicated PlayerHose pressure, pitch limits, and steering input | `00466780`, `004677D0`, hose lifecycle, pressure and sample-consumer exports | Complete; pressure alias, stored velocity scale `4000 * pressure`, separate sample shape scalar, 0.75-second/eight-record history, 25-section dual-curve quads, and 10-to-40 contact padding traced and ported; texture/resource binding, blend flags, component gate/cadence, and pitch/path-angle relation remain open |
| `hose_host_seam` | Identify the smallest shared-host PlayerHose integration slice | `scene_viewer.cpp`, `startup.h`, hose lifecycle exports, Training hose subcomponents | Complete; read-only; recommendation integrated as a separate route in the shared game host; authored spline/camera and stage result remain open |
| `hose_city_data` | Map city_h's authored path, targets, and loader representation | `.local/exports/city_h/entities.json`, `scene_data.h`, lifecycle evidence | Complete; read-only; confirmed 19 path nodes, nine hotspots and 29 `%Model%` target references; no edits |
| `hose_mode_labels` | Trace PlayerHose session mode index to retail labels/default | Pinned `004677D0`, related PlayerRescue initialization, menu and EVS evidence | Complete; read-only; exact timer factors are source-backed, but labels/default are not; difficulty-like role is suggestive only |
| `hose_curve_trace` | Trace PlayerHose builder, sampled curve records, and runtime sampler | `004661A0`, `00490510`, `004905A0`, `00490D00`, `00490AB0`, `00490BB0`, `004677D0` | Complete; corrected the cache-change return polarity; builder/samplers and +0x194 UserData/HosePath mapping are documented; shared camera rail and anchor-driven hotspot entry are ported; delta and countdown units are seconds with the source pacing/cap ported, while focus-pause mapping, state-frame timing, mouse aim, and live parity remain open |
| `hose_anchor_entry` | Trace `00490BB0` record-cache semantics and `004677D0` hotspot entry timing | Current pinned EXE, Ghidra trace, PlayerHose spline and state exports | Complete, read-only; selected record is returned only on change, initial null cache reports the first record, first/D-linked anchors enter hotspot state, and state-4 countdown copies the stored timer budget; exact frame gap remains open |
| `hose_aim_camera` | Separate PlayerHose view frame, steering frame, and nozzle/stream source point | `00466780`, `004677D0`, `00465EF0`, `00496DD0`, fresh hose exports | Complete, read-only; stream source point uses the entity frame offset while bounded steering rotates a temporary aim frame; accessor cadence, effective limits, and live orientation remain open |
| `hose_effect_trace_audit` | Independently audit rescue HoseSpray deadline and spawn mapping | `00463EF0_fresh.c`, `00463EF0_hose_contact.asm`, `random.h`, `rescue_hose_spray_effects.h`, `scene_viewer.cpp` | Complete, read-only; corrected the direct unsigned deadline gate and `{last,height,first}` velocity order; clock-plus-random deadline and shared RNG order match; allocation and final particle rendering remain unresolved |
| `spray_template_trace` | `00463700`, `0047D2B0`, `0047D330`, `0047DB10`, `0047D6D0`, `0047D8A0`, `0047D820`, `0047F500`, `0047F960`, `0047FEA0`; HoseSpray renderer | Complete, read-only; corrected the initial non-interpolated scalar to key 0's 3.0 (key 1 is 5.0); `004B9400` calls are numeric conversions, not RNG; shared host now ports one-key-per-update advancement, the 2.0-to-0.0-second key timing, and flag-`0x8` deactivation; quad mapping, final material, exact effective timing, and pool cadence remain open |
| `camhose_data_binding` | Check whether the four BSP exports and host loader expose `RCS_CamHose` spline inputs | `.local/exports/{city_h,const_h,dock_h,oil_rig_h}`, `scene_property_matrix.csv`, `SCENE_DATA_CONTRACT.md`, `scene_viewer.cpp` | Complete; read-only; confirmed no CamHose BSP entities, then constructor/UserData and HosePath offsets established route binding; CamHose is a separate helper and not the route-node source |
| `bank_retail_collision_trace` | Resolve retail Player hull and swept collision path for Bank obstruction | Pinned EXE/Genesis exports, `core.h`, Bank door evidence | Complete; read-only; retail mins `(-30,-30,-30)`/maxs `(30,85,30)` match the port; exact leaf and live route remain open |
| `mesh_metadata_audit` | Independently audit world-mesh face-span source correspondence | Exporter, geometry/world-mesh JSON, C++ loader | Complete; read-only; validated field mapping and recommended all-map metadata checks; no renderer change |
| `retail_material_trace` | Search pinned retail evidence for TexInfo alpha and lightmap render semantics | Ghidra exports, material/driver evidence, Bank and Training exports | Complete; read-only; 2026-10-15 hash-pinned trace maps descriptor/RGB handoff, shared raw projection source and UV formula, two-pass ZERO/SRCCOLOR fallback and two-stage TEXTURE×CURRENT composition, clamp/linear filtering, ZWRITE and LESSEQUAL, one style-sequence step per geEngine_RenderWorld call, and the selector `wMaxSimultaneousTextures > 1`; effective ZENABLE, mip filter, selected-device path, optional face branch, host render-call rate, channel convention, rounding, and full pixel parity remain open |
| `next_gameplay_slice` | Select a reusable authored campaign mechanic and trace its script route | Gas R BSP/EVS, `gas_r_button1_use_path.md`, `gas_r_script_variable_queue_path.md`, shared viewer host | Complete; no edits; recommended generic FireRescue `setsize` support for Gas R button1's six authored commands |
| `fire_setsize_audit` | Independently audit retail timed-size semantics and the shared host seam | `00449A80_fresh.c`, `00447920_fresh.c`, `0044B620_fresh.c`, `0044AE80_fresh.c`, `scene_viewer.cpp`, `fire_size.h` | Complete; read-only; confirmed key reset/insert/interpolation and timestamp ordering; highlighted existing past-time key insertion bug fixed in this checkpoint |
| `mbase_acceptance_audit` | Review whether the MBase synthetic rescue check supports an acceptance claim | MBase EVS/BSP, `scene_viewer.cpp`, exit transaction | Complete; found the prior check selected only the first of five victims and that MBase has no authored final-exit binding; current all-victim harness checks the no-exit gate without claiming an outro |
| `retail_mbase_goal_audit` | Trace MBase retail rescue threshold and final-exit behavior | Same-hash Player constructor, rescue count/update exports, MBase exit inventory | Complete; five victims and no RescueBot completions reach the message/input gate after all rescues; no authored final-exit handler or verified mission outro |

- **Genesis driver reuse audit:** public Genesis3D's shared `Dcommon.h`
  exposes a `geBoolean` out-pointer hook (`DRV_Driver **`), `_fastcall`
  callbacks, and table version 100/3. FDNY checks 101/1 and uses observed
  table slots at `+0x14/+0x18/+0x20/+0xA8`; public-source field names support
  the initial enumeration/init roles but later offsets and source identity
  remain unresolved. Public source is an API/algorithm reference only; no code
  was copied. See
  [`public_source_reuse_audit.md`](../tracker/evidence/driver-boundary/public_source_reuse_audit.md),
  [`ENGINE_SOURCE_REUSE.md`](ENGINE_SOURCE_REUSE.md) and
  [`public_driver_comparison.md`](../tracker/evidence/driver-boundary/public_driver_comparison.md).
- **Rescue integration audit:** the highest-value shared seam is the
  level-neutral loader/catalog resolver. The old renderer and collision
  consumers can use the exported schemas; the Training host's startup,
  scripts, actor catalog, and Apple UI remain specific to that workbench. The
  new `SceneData` package loader and viewer implement the first half of this
  recommendation. The shared escape transaction now runs through MBase's
  authored EVS route in a synthetic check; complete rescue gameplay remains
  open.

## Data ledgers

- `tracker/scene_load_coverage.csv`: generated by building
  `fdny-scene-viewer` and running `python3 tools/build_scene_load_coverage.py`.
  It records what the C++ package loader actually opened for each local map.
- `tracker/level_map_inventory.csv`: catalog and BSP relationship, entity
  counts, file hashes, and exporter coverage.
- `tracker/function_reviews.csv` and `tracker/progress_history.csv`: function
  review activity. `dashboard/index.html` reads these CSVs when opened; update
  the CSV data to show progress without regenerating the HTML.
- `tracker/genesis_function_reviews.csv`: hash-pinned Genesis.dll engine
  decompilation ledger; separate from the EXE-only dashboard activity count.
