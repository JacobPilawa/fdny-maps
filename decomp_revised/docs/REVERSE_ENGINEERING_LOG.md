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
routine returns in Player state 6; otherwise it computes a fire-relative
angle and calls `0044BB00`, which sets one of four directional burn-indicator
bytes at Player `+0x353C..+0x353F` without changing view yaw. It then applies
the `RC_Pain` sound/stat deadline on its separate clock gate and accumulates
`1.2 * frameDelta * influence` in Player `+0x3538`. A strict extended-value
comparison above 1.0 invokes the passout handler. The native preview now maps
those four sectors and the Player updater's burn-cooling gate; the four HUD
widgets still need visual placement and pixel parity. The saved disassembly
trace preserves the current-clock addition hidden from Ghidra's C-like output
and the float-store versus extended-precision comparison.

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

## 2026-10-13 — Gas R button1 motion join resolved

Decoded the map asset link for entity 309's `button1`: its `%Model%` child is
model index 91. The Gas R GBSP `MOTIONS` chunk declares 209 records but has no
`ModelNum 91` entry. The Genesis3D GBSP loader initializes model motion pointers
to null and assigns them only for serialized motion records; the game binary's
`InitializeBModelEntity` therefore leaves this model's greatest child-motion
endpoint at zero. `CheckBModelReachExpansion` (0043AFF0) returns false while
that extent is strictly between approximately -1e-28 and +1e-28. The BModel
contact check raises authored `E_Reach` to 48 only when this helper returns
true. Thus Gas R button1's effective static reach is its authored **20 units**.
The asset join, chunk hash, inventory output, Ghidra pseudocode, and exact
threshold bytes are recorded in `gas_r_button1_use_path.md`,
`gas_r_model_motions.json`, and the fresh helper export. Genesis3D references
are format/loader corroboration, not evidence that the retail game contains
those source files. Runtime overlap, candidate ordering, update cadence, input
selection, and visible effect timing remain open. No dashboard HTML or native
gameplay code changed.

## 2026-10-13 — BModel motion extents joined across rescue maps

Reused the archived Training door-motion audit and analyzed the copied retail
`Genesis.dll` (SHA-256 `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`)
in a separate headless Ghidra project. Fresh retail-DLL pseudocode at
`1000D570`, `1000F580`, `1000BE60`, and `1000E490` establishes the MOTN/PATH
reader and last-key time extent path. A reusable GBSP audit now joins
`RCS_BModel.C_AnimModelN` names to `%Model%` indexes and serialized MOTIONS
records, decodes their F2 key times as binary32, and computes the root's
greatest child end time using the executable's `0043C670` initialization rule.
Across Training and all seven campaign rescue maps, 76 BModels pass the
hit-point/non-liftable/authored-reach gates; 71 feed effective reach 48 into
`0043B620`, while five motionless ladders/button retain authored reaches.
Gas R button1 is one of the five and remains 20 units. The full cross-map
counts and entity-level joins are in
`bmodel_reach_cross_map.md` and
`bmodel_reach_asset_inventory.json`. Static bounds-test behavior is resolved;
live contacts, candidate selection/order, callback cadence, input handling, and
visible effect parity remain open. No dashboard HTML or native gameplay code
changed.

## 2026-10-13 — Subway A self-bound door events traced

Mapped the Subway A EVS `subdoor` and `brokendoor` variables through the shared
ScriptActor parser. Fresh Ghidra analysis now shows the parser turns colon
event syntax into `onmsgappend`, and the common dispatcher stores or appends
those command lists in the ScriptActor event table. The event lookup later
routes contact-generated `touch` and `untouch` keys to the stored commands. The
newly named helpers are `SetScriptActorEventCommands` (0046F6F0) and
`AppendScriptActorEventCommands` (0046F820); both now have saved Ghidra comments
and pseudocode exports.

Joined the event definitions to Subway A's BSP: 17 `RCS_BModel` actors bind
`*subdoor` with 100-unit authored reach and 51 resolved animated child-model
references; each model motion ends at 1.0. Their touch route applies time scale
4, issues `playsnd SubDoor`, and opens the BModel; untouch issues the sound
command and closes it. Twenty-five `RCS_BoundsTrigger` records bind
`*brokendoor` to `playsnd Zapp3d`; all collision models resolve. The existing
BoundsTrigger initializer is now named `InitializeBoundsTriggerEntity`
(00441040). Exact actor indices and `ZZ_location` coordinates are in
`subway_a_interaction_inventory.json` and the static route is in
`subway_a_interaction_path.md`.

Extended the same route through `playsnd`: `HandlePlaySoundCommand` (00477AF0)
uses the executing ScriptActor name, `FindScriptActorSoundOrigin` (00477740)
resolves it through the shared actor registry (`<console>` aliases to
`player`), and `HandleActorSpatialSoundCommand` (004778C0) samples the actor
position before submitting the sound and owner to `GetOrCreateSpatialSoundInstance`
(004749A0). This statically anchors `SubDoor` to each named door and `Zapp3d`
to each named BoundsTrigger. Four fresh pseudocode exports and saved Ghidra
comments preserve the route. The pool behavior is mapped, while playback,
attenuation, and audible timing remain open.

The contact-to-event and event-to-command paths are statically joined. Actual
overlap, callback ordering/cadence, audible output, and visual timing have not
been captured. This advances one Subway A mechanic; all rescue missions and
Training remain in scope, and no mission or port is marked complete. The EXE
function-review ledger now contains 150 functions. No dashboard HTML or native
gameplay code changed.


## 2026-10-13 — Subway A sound resources traced to FMOD calls

Extended the static `playsnd` chain through resource resolution and audio
playback. Fresh Ghidra labels and comments now name the sound-tree lookup
(`FindSoundResourceNodeByName`, 0049F5B0), sound resource resolution
(`ResolveSoundResourceByName`, 0049F850), sample/stream start
(`StartFMODSampleOrStream`, 004A00F0), playback dispatch/defer
(`DispatchOrQueueSoundPlayback`, 004A0BE0), deferred queue drain
(`DrainQueuedSoundRequests`, 004A0D60), sound-instance registration
(`ConfigureAndRegisterSoundInstance`, 004A12F0), 3D update
(`UpdateSound3DAttributes`, 004A10D0), and FMOD attribute wrapper
(`SetFMOD3DAttributes`, 0049F350). Eight current pseudocode exports back the
function-review entries.

Extended `audit_subway_a_interactions.py` to join the EVS `playsnd` names to
`Data/ResDefs/sound.rrf` and hash-resolve each audio file. `SubDoor` maps to
`ROC_SOUND_BUFFER3D 200 1000` and `Data/Snd/subdoor.wav`; `Zapp3d` maps to
`ROC_SOUND_BUFFER3D 200 1000` and `Data/Snd/spark1.wav`. The source hashes and
sound-definition hash are recorded in `subway_a_interaction_inventory.json`.
The two numeric resource arguments are preserved as authored; their exact
falloff meaning is not claimed. The FMOD 3D-attribute call is gated by a byte
copied from the resolved resource, and the resource-parser mapping of that byte
remains open. Runtime contact, playback, falloff, and audible timing still
require capture. This advances only one Subway A mechanic; every rescue and
Training remain in scope, and no mission or port is marked complete.

The EXE function-review ledger now contains 158 functions. The CSV-based
progress history was updated; `dashboard/index.html` was left untouched and
continues to read tracker data when opened.


## 2026-10-13 — Subway A sound parser and distance fields resolved

Followed the sound definitions through the EXE resource parser. `LoadResourceDefinitionsFile` (0049B3B0) dispatches the `ROC_SOUND_BUFFER2D` token with type 0 and `ROC_SOUND_BUFFER3D` with type 1 to `ParseSoundBufferDefinition` (0049B140). The 3D parser consumes two numeric tokens and forwards them with the path/type byte through `FindOrCreateSoundResourceByName` (0049FDA0). `InitializeFMODSoundResource` (004A0210) stores the type byte at resource+9 for non-MP3 samples and loads them with `FSOUND_Sample_Load`; the MP3 path uses `FSOUND_Stream_OpenFile`. Playback copies resource+9 to the instance, where `UpdateSound3DAttributes` gates `FSOUND_3D_SetAttributes`.

`SetFMODSampleMinMaxDistance` (004A0040) calls `FSOUND_Sample_SetMinMaxDistance(sample, first, second)`. Caller assembly and parser dataflow preserve source token order, so Subway A's `SubDoor` and `Zapp3d` entries configure minimum distance 200 and maximum distance 1000. Their WAV file hashes, sound-definition hash, and EVS/BSP joins are in `subway_a_interaction_inventory.json`; callsite evidence is in `sound_rrf_type_and_distance_assembly.md`.

This resolves the static resource flag and numeric-field mapping. It does not capture collision timing, actual FMOD falloff/audibility on macOS, or visual parity, and only covers this Subway mechanic. Five fresh function reviews were added; the ledger now contains 163 functions. `dashboard/index.html` and gameplay code remain unchanged.


## 2026-10-13 — D3DDrv texture lifecycle and rescue texture resources

Loaded the copied D3DDrv.dll into Ghidra and followed the version-101.1 table
from DriverHook through texture handle creation, mip surface allocation,
lock/unlock, textured polygon preparation, and begin/end-scene callbacks. The
selected functions are exported to
`tracker/evidence/driver-boundary/d3ddrv_render_functions.c` and reviewed in a
separate hash-pinned `tracker/d3ddrv_function_reviews.csv` ledger. At the time,
the active Ghidra project did not have a save location, so the exports were
kept as the durable record. The persistent `analysis/ghidra/FDNYPort.gpr` was
later reopened and now accepts saves. The Direct3D path describes the shipped
Windows renderer boundary and is not copied as a macOS implementation.

Audited raw rescue.rrf: 114 ROC_ALPHA_TEXTURE entries, 113 distinct numeric
keys, 110 distinct base paths, and 44 optional alpha paths. Every base path is
under Data/Sprites and no case-insensitive base filename matches a BSP texture
name in any of the seven rescue maps. This separates sprite/effect/HUD alpha
resources from the embedded BSP albedo path without claiming the numeric keys
have no consumers. The floor/render symptom was separately fixed in the
portable viewer: collision/render model 0 stays active when a BoundsTrigger
omits its optional model index, and col_* helper groups are hidden. All seven
rescue maps pass grounded movement and offscreen CGL textured-frame checks.


## 2026-10-13 — Explorer remains a comparison reference; retail-derived motion preview

Reaffirmed the provenance rule for `FDNY_trackdown/map_explorer`: use its
same-input scenes only to spot visual or collision discrepancies. Its source,
controls, filters, and inferred behaviors are not port implementation or proof
of retail behavior. All port behavior must be derived from the copied retail
EXE/DLLs and original game data, then implemented and documented in this
self-contained worktree. The detailed scope, hashes, and comparison limits are
in [`MAP_EXPLORER_REFERENCE_AUDIT.md`](MAP_EXPLORER_REFERENCE_AUDIT.md).

Integrated a retail-derived BModel child PATH motion preview in the native
viewer. It links BModel `C_AnimModelN` children to numeric Model indices and
hash-pinned MOTN/PATH tracks, then applies sampled transforms to both render
and collision models. `M` plays/pauses authored paths and `N` resets them to
time zero. This remains a map inspection preview: proximity-triggered door
events, EVS dispatch, original callback cadence, and runtime timing parity have
not been ported. The viewer built successfully and `--inspect` succeeded on
all 17 exported map packages. The persistent `analysis/ghidra/FDNYPort.gpr`
project was reopened and accepts saves; previously reviewed function
boundaries remain saved there.

Parallel research found no public Genesis3D source suitable for transplantation
into the FDNY port: it is useful as format/API context only, and the community
fork is not verified against the shipped FDNY binaries. A provenance audit
also caught an inherited ACT asset decoder: `tools/extract_act.py` matches the
previous `decomp` copy byte-for-byte and is 99.8% line-similar to the
map_explorer parser. It is an inspection utility, not independently derived
port code; its actor output is now explicitly provisional pending retail
Genesis.dll/raw-ACT verification or independent reimplementation. The explorer
viewer, filters, and motion controls remain comparison-only. The full-port
target, including every rescue mission, remains unchanged.


## 2026-10-13 — Shared viewer applies authored PlayerRescue loadouts

Connected the ordered `RCS_PlayerRescue` `S_Script01..10` fields to the shared
viewer command host. The viewer now routes `equip` and `select` into the
source-mapped one-based axe/extinguisher/goggles slot state, and applies the
Training/Gas R `setashdensity` and Training `setrescuebot` commands to existing
Player fields. Added 1/2/3 viewer controls and selected-tool title text; those
keys are a host input choice, not recovered retail bindings.

The current-hash traces map retail `0044D080` equip, `0044D270` selection,
`0044C2A0` active-slot transition, and `0044D550` PlayerRescue registration.
Integration and remaining limits are recorded in
[`player_loadout_command_path.md`](../tracker/evidence/scenario-loading/player_loadout_command_path.md).
The viewer compiled successfully with CMake. No tests were run. Tool object
construction/animation/use, per-item callback parity, goggles visibility, and
the delayed full mode-0 EVS pass remain open. The 1/2/3 state alone is not a
complete equipment implementation.

## 2026-10-13 — Shared extinguisher suppression and terminal visual correction

Connected selected extinguisher use in the map-neutral scene viewer to the
existing source-derived extinguisher pose and contact helpers. Held left-click
advances the active pose, samples four contact points, applies the 60-unit
padded fire-volume falloff, and queues message 30000 with twice the accepted
influence. The host resolves that record to the matching BSP FireRescue and
calls the shared suppression transition with `finalize=false`. Accepted
contacts also consume the source-shaped effect timer and offset random draws;
the emitter and audio are not rendered. The camera-derived parent transform,
initial one-unit charge reused from the Training host, and rendered-frame
cadence remain provisional. The current evidence and launch steps are recorded
in [`extinguisher_scene_viewer_runtime.md`](../tracker/evidence/scenario-loading/extinguisher_scene_viewer_runtime.md).

Removed the immediate sprite disable that followed terminal suppression in
`FireEntity::spray`. Fresh retail evidence confirms mode 5 and a cleared byte
at `+0x1C8`, then an early return before normal alpha/effect updates; it does
not establish instant visual disappearance. Renamed the port field to
`unknownByte1C8` so the unresolved byte is not treated as a fire-visibility
flag. The shared viewer and Training targets compiled successfully. No gameplay
checks were run. Random spread, player exposure, particle drawing, and exact
terminal render lifecycle remain open.

## 2026-10-13 — Goggles scan state and fire callback dispatch audit

Read-only audits joined the normal owner update through contact-manager pass
`0049BD20` to FireRescue's `+0x48` callback. The active entity's category bits
select type lists for that pass; this maps callback traversal but not rendered
frame cadence, Think/message order, dynamic child registration, or global RNG
order. Random fire spread and player exposure remain unwired in the viewer.

The goggles trace verified ACTION press at vtable `+0x14 -> 00462E80`, its
kind-2/kind-6 candidate filter, and the strict upper bound from EXE constant
`004E1108` (`4,000,000.0f` squared distance). Victim states 5 and 6 are
excluded. Selection-triggered scanning is supported by the `+0x3C(1)` callback
path and `00462660` calling `+0x14`; the effect appearance remains an inference
boundary. Added a shared scan-state component and wired the shared viewer's
selection/ACTION path to current BSP-loaded fires and victims. The viewer shows
a host target count; it does not yet render retail scan effects or alter global
visibility. Details and launch steps are in
[`goggles_scan_viewer_runtime.md`](../tracker/evidence/scenario-loading/goggles_scan_viewer_runtime.md).

The shared scene-viewer target compiled successfully after the integration.
No gameplay tests were run. Weekly usage was 28% used at the close of this
turn; the five-hour window was 30% used.

## 2026-10-13 — Goggles heatpulse target rendering

Resolved the goggles child resource from `00462A60`: resource `0x2BF` (703)
maps in the copied `rescue.rrf` to `heatpulse.bmp` and `heatpulse_a.bmp`.
Pinned-executable disassembly at `00462782–004627CB` confirms the effect
window: `(scan time - current time + 8500) / 8500` is bounded by 0 and 1,
using the double reciprocal at `004E2930` and float bounds at `004DC50C` and
`004DFC24`. The target update reads the current actor transform and activates
its child each frame. Binary/hash and instruction details are saved in
[`00462730_goggles_effect_window.asm`](../tracker/evidence/scenario-loading/00462730_goggles_effect_window.asm).

The shared viewer now draws the copied retail heatpulse pair at approximate
host target positions during that window and removes it on tool deactivation.
Retail `00462730` transforms an actor-local point through the current actor
transform and updates a child resource; the preview uses a camera-facing quad
with an estimated vertical offset, so that transform and child attachment are
not reproduced. Marker size, offset, blend/tint, billboard orientation, child
animation, RNG ordering, and global visibility behavior remain provisional or
open. The Ghidra MCP bridge
reported no running instance, so this check used the saved Ghidra decompilation
and direct disassembly of the hash-pinned project executable. The macOS viewer
target built successfully; no gameplay tests were run.

## 2026-10-13 — Random FireRescue spread connected to the shared viewer

Added a first shared-host implementation of the current-hash proximity-spread
path. The source trace is `004483A0` (FireRescue primary `+0x48` contact
callback) → `00448320` (max-axis proximity and 750-unit bookkeeping) →
`00447A90` (intensity roll, child placement, and timed size keys). The viewer
now keeps each named fire's previous proximity distance and updates a shared
nearby-fire count as callbacks run in candidate order; this replaces the
earlier frame-by-frame recount. It attempts spread after player movement, uses
the BSP world sweep, and queues children until the contact pass completes.
Already-counted mode-5 fires remain counted while resident, as retail skips
their callback until cleanup. The implementation is bounded to the viewer's stable
single-Player fire list; teardown, reuse, and full candidate membership remain
open. This follows the mapped owner order where the contact manager finishes
before the staged factory queue drains. Details and the exact transition
instructions are in
[`proximity_count_port_2026-10-15.md`](../tracker/evidence/fire-spread/proximity_count_port_2026-10-15.md).

The fresh, hash-pinned `00447A90` export establishes the strict size/deadline
gates, failed-roll retry at `now+500`, radius and angle draws, sweep from
20 units above the root with a +/-1 box over 200 units, and accepted-hit
tolerances. For `v = U(0,1)*intensity`, peak size is
`b=(10+90*(1-v))*U(.9,1.1)`, `x=b*(.25+.75*v)*U(.9,1.1)`,
`y=(10+30*v)*U(.8,1.2)`, `z=b`; key intervals are 750–3000 ms and
1000–6000 ms. The integer helper `004B9400` truncates toward zero, matching
the host timestamp conversion.

This implementation is bounded and not a parity claim. The manager callback
currently runs once per rendered frame. Current-hash PlayerRescue vtable bytes
map root slot `+0x0C` to thunk `0049DB90`; an archived body export supports
the transform-origin getter at `0049D790`, and the viewer uses `Player::p` as
its world-origin adapter. Getter bytes, the fourth float used by exposure, and
exact host-to-retail transform alignment remain open. The initial gate audit
established only that the callback dereferences root `+0x48` and tests
pointed-to `+4`; it had not yet traced the initialization setter. That
checkpoint temporarily allowed all fires through. The policy was superseded
by the retail setter-chain trace in the later “Resolved FireRescue spread name
gate” entry. The proximity count is reconstructed from active host actors,
and spawned-child initialization, absolute host tick keys, collision routing,
and RNG ordering remain provisional. Player exposure is still open; its
static `0x7531` and passout paths are not connected to an automatic host
cadence. Random proximity spread through `004483A0` is distinct from the
state-4 route-motion callback supplied as a no-op in `FireEntity::think`.

The independent `spread_trace_audit`, `fire_name_gate`, and
`player_position_getter` read-only reviews are recorded in the work-queue
ledger. The macOS
`fdny-scene-viewer` target rebuilt successfully. The linker warned that the
Homebrew `json-c` dylib was built for macOS 27.0 while the requested target is
13.3; deployment compatibility remains unverified. No gameplay run or test was
performed. The eligibility pointer initialization was resolved in the later
entry below; callback cadence, child lifecycle, and player exposure remain
open.

## 2026-10-13 — Fire contact candidate and gate mappings corrected

Read-only review of current-hash PlayerRescue vtable bytes and constructor
evidence mapped the FireRescue candidate call at vtable slot `+0x0C` to Player
thunk `0049DB90`. The archived thunk/body exports support a transformed-origin
getter returning XYZ at `+0x68`; the adjacent `+0x10` slot instead points to
`0049D7F0`, a separate buffer getter. The viewer now routes spread position
through an explicit `Player::p` world-origin adapter. Getter machine code,
the exposure fourth float, and exact transform alignment remain unverified.

A separate audit confirmed that the spread gate dereferences root `+0x48`
before checking the pointed-to `+4` word. At that checkpoint the setter had
not been followed, so the viewer provisionally permitted every authored fire
and dynamic child. The later retail disassembly trace in “Resolved FireRescue
spread name gate” supersedes this interpretation: root `+0x48` is a separate
string object populated from UserData `A_Name`, and its `+4` field is string
length. The exported inventory has 60 unnamed or empty-name records among 430
FireRescue entities; these actors are correctly excluded from random spread.

The Ghidra bridge reported no running instance, so this correction used the
hash-pinned vtable/constructor exports and saved historical getter exports;
it did not refresh getter bytes. No tests or gameplay runs were performed.
The macOS `fdny-scene-viewer` compile-only build passed. Linking warned that
the Homebrew `json-c` dylib targets macOS 27.0 while this target is 13.3.

## 2026-10-13 — Resolved FireRescue spread name gate

Followed FireRescue root `+0x48` from construction through runtime setup.
`0044A330` constructs the embedded subobject at root `+0x24`; `0046F930`
initializes its string field at subobject `+0x24` (root `+0x48`) to the shared
empty string. The pinned retail initializer `00448D60` checks UserData
`A_Name` at root `+0x78+4`, constructs a string from it, then calls `0046E320`
to store that string in the embedded field. `00401D80` writes string length at
implementation `+4`, exactly the word tested by the spread routines.

The local retail EXE hash matches the current pinned analysis hash. No Ghidra
instance was running, so the initializer call sequence and setter were
verified from the retail bytes using `objdump` and cross-referenced with the
saved Ghidra C exports; the annotated listing is
[`00448D60_name_setup.asm`](../tracker/evidence/fire-spread/00448D60_name_setup.asm).
The initial “unmapped shared state” interpretation was wrong because the
embedded pointer was followed only to its default empty string, not through
the later per-fire setter.

The shared viewer now allows spread only for authored fires with nonempty
`A_Name`. The spread factory zeroes the generated child definition's `A_Name`
field, so those children retain the empty gate and do not spread recursively.
The eligible-fire count uses the same name gate. This corrects both the prior
name-filter removal and the permissive all-fire assumption. Callback cadence,
host candidate-origin alignment, and complete RNG/lifecycle parity remain
open. No gameplay test was run. The compile-only macOS build passed for
`fdny-scene-viewer`; the linker warned that Homebrew `json-c` targets macOS
27.0 while this build targets 13.3.

## 2026-10-14 — Shared PropModel actor and contact path

Fresh read-only Ghidra work pinned the PropModel candidate producer
`00443700`, model enable gate, Player point getter, `geWorld_MightSeeLeaf`
gate, and the contact-pair drain through `00444050` / `00470840`. The drain
dispatches new, continuing, and removed contacts in `touch`, `touching`,
`untouch` groups. Training entity 161 `ext1` is a touch PropModel using model
2 (`ext.act`) with a 70-unit radius and a refill macro that equips/refills the
extinguisher and plays `Steam4`. The root slot `+0x14` updater's external
caller and cadence remain unresolved.

The shared exporter now resolves 15 PropModel ACT resources and 22 bitmap
entries across the 13 rescue/stage packages. The asset scan corrected the
earlier assumption that the failure was in `ext.act`: its textures are
paletted; other actors use GeBm format 10, 24-bit BGR, including color-keyed
records. The Genesis.dll Ghidra trace confirms that color-key bytes are read
in the bitmap's pixel format. `extract_act.py` and the actor renderer now
preserve that decoded RGBA payload.

The viewer draws all available PropModel actors in bind pose at authored
location and heading/pitch, applies live replacement `setscale`, and honors
`show`/`hide` as render and contact gates. Player-point/radius contact now
dispatches event categories in the retail order. The port resolves the
Player-point and PropModel-center leaves and applies the lower-level cluster
VISDATA rejection. Positive PVS results still omit the runtime area-visibility
matrix and exact current-view cache semantics; `Player::p` remains a provisional
Player-frame point, and PropModel script actors advance on the host frame loop.
Thus source-shaped contact wiring and a partial view-leaf gate are implemented,
but live extinguisher pickup and retail timing parity are not confirmed.

The rescue-all app rebuilt successfully. Training render-check uploaded all
24 BSP textures, and the packaged mission-catalog check loaded all eight
retail rescue choices across 13 scene/stage packages. These are port checks,
not a live gameplay or retail comparison. The user reports Bank framerate now
seems sorted; retail same-route FPS and frame pacing remain unmeasured.

## 2026-10-14 — Rescue delivery and alternate-spot audit

A read-only subagent re-audit found the rescue completion seam in the existing
shared implementation: Player `escape` reaches `004513A0` / `00451160`, which
updates rescue count and time statistics, releases the carried victim, clears
carry state, and calls the final-exit availability gate `004504C0`. That gate
compares remaining victims to the authored threshold and enables eligible
exit triggers. The supported `finalexitloadmap` command also has its own
remaining-victim gate. Live trigger contact and retail update cadence remain
unverified; validating full contact-driven delivery and exit progression
across packages is the next shared gameplay target.

The same audit confirmed two-pass alternate-spot setup: VictimRescue registers
its authored origin, matching A_Victim spots append alternate points, and a
later pass selects a random entry. All 45 authored alternate-spot references
join across the 13 rescue/stage packages; no authored `forcespawnspot` command
was found. The existing evidence is in
[`rescue_completion_and_exit_path.md`](../tracker/evidence/scenario-loading/rescue_completion_and_exit_path.md),
[`victim_spawn_factory_and_order.md`](../tracker/evidence/scenario-loading/victim_spawn_factory_and_order.md),
and [`victim_roam_behavior.md`](../tracker/evidence/scenario-loading/victim_roam_behavior.md).

## 2026-10-14 — Victim message and BoundsTrigger event integration

The saved retail exports distinguish the Player message boundary from its
pickup handler: `00453A70` first gates messages on Player `+0x3438`, then routes
`0x7532` to `00450440`; that handler checks the carry pointer at `+0x3534`
before beginning pickup. The natural shared-viewer overlap scan now publishes
`0x7532` through the generation-checked Message Mapper model and resolves it
back to the Player receiver. It continues to submit overlapping victims while
carrying so the Player-side gate rejects later messages. The existing synthetic
rescue check now uses that path. Host delivery currently drains after the
candidate scan in the same frame; exact retail scheduler timing and Player
world-box frame alignment remain provisional.

BoundsTrigger contact callbacks are grouped in retail's
`touch`/`touching`/`untouch` dispatch order. Its render pulse now requires the
authored escape binding which initializes root `+0x164`; contact production
remains independent from render flags. BoundsTrigger updater caller and
cadence remain unresolved. No additional retail functions were reviewed in
this integration pass. The source and current port notes are recorded in
[`victim_contact_message_port_2026-10-14.md`](../tracker/evidence/scenario-loading/victim_contact_message_port_2026-10-14.md).
