# Boat A type-5 MiscEmitter trace (2026-10-20)

## Provenance

- Retail executable: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Boat A BSP: SHA-256
  `e1685b45ef29aa075c688985617b593079a6aad8a6b493bbae327356bd45ad44`.
- Boat A EVS: SHA-256
  `d0e8d416a87bf24710835dac8c394dcbc5fec24021698f4ddf7677ba58f5b453`.
- Ghidra 12.0.4, saved `FDNYPort` project, headless read-only queries with
  `-noanalysis`. The checked-in instruction excerpts are from the current-hash
  program; raw command logs and project data remain under ignored `.local/`.

## Authored Boat A event

`boat_a.evs` line 7 defines `QueExploders`: tell `exploder1`, `exploder2`, and
`exploder3` `setsize 3` and `turnon`, wait 1, then tell the same actors
`turnoff`. Line 9 binds the `explosion` touch trigger to open `explodeb`, issue
`quake 50 1`, play `Explodelarge`, and run `QueExploders`. This is static EVS
content and does not prove the trigger is naturally reached in a retail run.

Boat A BSP entity indices 254–256 are the three `RCS_MiscEmitter` actors. Each
has `C_Type=5`, an authored initial `setsize 6`, and an initial `turnoff`;
EVS overrides the scalar size to 3 before activation. `explodeb` is a separate
BModel target with the existing fencefall/debris child mapping in
[`bmodel_rescue_targets.md`](bmodel_rescue_targets.md).

## Retail command and construction path

`DispatchMiscEmitterScriptCommand` (`004420F0`, embedded ScriptActor vtable
slot +4) parses `setsize` as one scalar. It writes the contained emitter's
owner field `+0x7c` and calls `00441FC0`, which copies that scalar into all
three scale components and refreshes the contained render transform. This is
an immediate uniform scale update, distinct from FireRescue's timed
`setsize x y z duration` form. `turnon` and `turnoff` call contained-emitter
vtable slots +0x18 and +0x1c.

The C_Type dispatch in `00442900` constructs `C_Type=5` as an internal emitter
mode 2: it allocates 0xA8 bytes, calls `00445600(..., 2)`, and stores the
contained pointer at owner `+0x78`. `00445600` installs vtable `004E1438` and
stores the mode at internal `+0x64`. In that vtable, `00444C90` and `00444CB0`
set the active byte `+0x45`; only mode 1 takes the additional sound-manager
path. The update code beginning at `00444D00` exits while inactive and has a
mode-2 branch. The EVS `wait 1` is an authored delayed command before
`turnoff`; no one-second duration is encoded in the mode-2 constructor.

`00445720` creates four child render objects for internal mode 2 by calling
`0047F850(child, 0x502D40, 0)`. It sets child fields `+0xA4=0xA0`, `+0xAD=1`,
and `+0xB0=4`, then appends each child to the emitter's child list. Internal
mode 1 instead creates three children using `0x502D78` and initializes a
`Torch` sound instance; that branch is not the Boat A type-5 path.

`0x502D40` is a descriptor pointer, not an RRF resource ID. Ghidra places it
inside `.data` (`004FF000` base, `0x3000` bytes); its on-disk bytes are zero.
Both a current-hash Ghidra xref query and a raw little-endian immediate scan
found one direct reference in the executable: `00445772`, the child-constructor
argument. That rules out another direct-address initializer in this image, but
does not rule out runtime writes through computed pointers or resource-loader
aliases. `0047F850` passes the descriptor to `0047D370`, which stores it at child
`+0x0C` and copies descriptor `+0x18` to child `+0x18`. `0047D2B0` follows
descriptor `+0x0C` to a frame/resource array and uses descriptor `+0x18` bit 0
to select clamping versus wrapping for the two child indices initialized to 0
and 1. The backing array and runtime descriptor contents remain unknown.

The mode-2 child draw path is now mapped. The internal-emitter vtable slot 9
(`00445490`) iterates its child array at `+0x4C`/count `+0x50` and calls
`0047F960(child, renderContext)` for each child; no separate child render-manager
registration appears in construction or in this parent method. `0047F960`
selects a world/renderer context, evaluates child vertex/UV/color and size
tracks, resolves the selected descriptor index through `0047D820`/`0047D860`
and `00493D00`/`00493650`, then submits a polygon through Genesis
`geWorld_AddPolyOnce` (import slot `004DC0D0`). This is the resource-ID lookup
and polygon-submission shape, but the numeric resource IDs from `0x502D40` are
unknown, so no Boat A bitmap, frame sequence, or faithful visual can be named.

The mode-2 updater is internal-emitter vtable slot 8 (`00444D00`), which updates
the same child array. No direct RNG call was found in the inspected mode-2
constructor/update path; this does not rule out draws in resource loading or
other indirect paths. The generic manager that schedules the parent callbacks
was not identified, so update/render cadence and order are unknown. The EVS
one-second wait is an authored command delay before `turnoff`; it does not
establish visible-effect duration or direct emitter timing.

## Port status and next trace

The shared scene host now represents authored `C_Type=5` actors with
`MiscEmitterRuntimeState` and routes their startup/EVS `setsize`, `turnon`, and
`turnoff` commands through the common mission dispatcher. It does not construct
the four retail child objects or draw the effect. Resource IDs/frame tracks,
child animation timing, parent callback cadence, and RNG interleaving remain
open. The next implementation slice is the generic child renderer after the
descriptor's runtime contents or exact resource/frame sequence are recovered.
The existing `victim_spawn_rng_port_coverage.md` records the unresolved
MiscEmitter constructor draws; the inspected mode-2 methods have no direct RNG
call, which does not resolve draws from resource loading or indirect paths.

## Follow-up provenance

The update/child/resource traces used Ghidra 12.0.4 headless reads of the
saved `FDNYPort` program and raw current-hash instruction windows. Temporary
logs are under `/private/tmp/fdny_misc_*`; the `.data` RVA/contents were checked
against the hash-pinned PE. The executable is still
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. This is
static evidence only: no runtime memory capture, code change, build, gameplay
run, or retail-parity check was performed.

## Shared host command-state follow-up (2026-10-20)

The shared rescue scene now creates lightweight runtime state for authored
`RCS_MiscEmitter` entities with `C_Type=5` and routes their startup scripts and
later EVS commands through the same host dispatcher. The state starts with the
source-initialized uniform scale `1.0`; active state stays unset until a
source-backed `turnon` or `turnoff` callback writes it. The one-scalar
`setsize` command updates that uniform scale immediately, and `turnon` /
`turnoff` record the internal mode-2 active byte. This covers Boat A's three
`exploder` startup bindings and the authored `QueExploders` state sequence.

This is command and actor-state plumbing only. It does not construct the four
retail child render objects, reproduce descriptor `0x502D40`, or draw an
explosion. Script queue scheduling still follows the host's per-frame actor
adapter; the authored one-second `wait` is not a retail cadence measurement.
The dynamic descriptor resource IDs/frame track and callback schedule remain
the next evidence gaps. The source file for the command-state adapter is
`port/spikes/training-native-prototype/src/misc_emitter_runtime.h`; the shared
dispatcher and type-5 actor registration live in `scene_viewer.cpp`.

## Descriptor and rescue-map audit (2026-10-20)

A read-only scan of the twelve local rescue-stage typed entity exports found
seven `C_Type=5` records in three maps:

| Rescue map | BSP SHA-256 | EVS SHA-256 | Type-5 records | Static command observations |
| --- | --- | --- | --- | --- |
| Boat A | `e1685b45ef29aa075c688985617b593079a6aad8a6b493bbae327356bd45ad44` | `d0e8d416a87bf24710835dac8c394dcbc5fec24021698f4ddf7677ba58f5b453` | entity 150 `explosion`; entities 254–256 `exploder2`, `exploder3`, `exploder1` | `exploder1/2/3` have startup `setsize 6` and `turnoff`; `QueExploders` later sets size 3, turns them on, waits one script second, then turns them off. The separate `explosion` record has null startup scripts. |
| Boat B | `4ad7de96916b5c265a4cdd6c477d96a0b6f9cc6e8151c96ff2e1ef8a31313a50` | `3d1de56a39ed7e92efef26d48c5127517fc5439dc20499f9f7d3eff323ce227c` | entity 94 `A_Name=<null>3` | All five startup script slots are null. No type-5 command binding is established. |
| Oil Rig B | `bfc16ba4e151a814ba2f80d84a3aadb096d9468c83262daab900c48e5043c701` | `871cf09e94cd79cbc15c5593e90b407c12ef71b28011211baa3e41c3f6acd70f` | entities 58/59 `flame2`/`flame3` | `S_Script01` is empty on each record; the preserved Oil Rig B EVS has no direct `flame2`/`flame3` command. This does not establish their initial active state. |

Bank, Bar R, Gas R, MBase, Subway A/B, and Oil Rig A/C/D have no
`C_Type=5` records in the inspected typed exports. This inventory is scoped to
that C_Type and the twelve rescue BSPs; it does not count other MiscEmitter
modes or prove runtime reachability. Boat B and Oil Rig B widen the value of a
generic type-5 child renderer, but neither provides an authored visible-time
sequence in the reviewed EVS.

The source resource definitions do not identify the descriptor's frames by
numeric range alone. `hose.rrf`, SHA-256
`93a1b2dae7d755fdd66fabaee89514de8e420cfb62f2f96132b341c0cce6fe45`, declares
alpha textures 120–137 as `exp-00` through `exp-17`. `rescue.rrf`, SHA-256
`8ab12b7c148c4d984a912525e432967556c9267af5a1b0ea86451ed1ee123f33`, reuses
IDs beginning at 120 for `fflame` textures. These are separate RRF definitions;
the active library namespace selected by the mode-2 child is not yet known, and
no evidence ties either sequence to descriptor `0x502D40`. Treat the `exp-*`
sequence as a candidate only.

A separate current-hash Ghidra/headless descriptor trace and raw immediate scan
found no static initializer or resource-loader registration for `0x502D40`.
Ghidra marks `0x502D40` undefined zero-filled `.data` with one direct reference
at `00445772`; that path passes it to `0047F850` from `00445720`. The descriptor
consumer reads `+0x0C` as the frame-record array, `+0x18` as a clamp/wrap flag,
and resolves selected records as numeric resource IDs. Resource lookup through
`00493D00`/`00493650` consumes renderer manager arrays; it does not fill this
descriptor. A range-wide Ghidra reference query over `0x502D00..0x502DFF`
found descriptor references at `0044B382` (`0x502D08`), `00445772`
(`0x502D40`), `00445823` (`0x502D78`), `00445B48` (`0x502D94`),
`00445A5F` (`0x502DB0`), `00445CBA` (`0x502DCC`), and `00458E40`
(`0x502DB0`). The `00445xxx` calls pass the latter three descriptors to
`0047FE70`; Ghidra did not assign those calls to containing functions. The
`00445823` path passes `0x502D78` to `0047F850`. Two more references at
`0044B3C9` and `0044B49A` read scalar `0x502D0C`. `0047FE70` stores its
descriptor pointer and optionally registers the object through `00493D60`,
which sets its registration byte and appends it to the renderer list without
populating the descriptor. `0047F850` and `0047D370` likewise only store/read
the descriptor. The corrected call instruction address is `00445CBA` (its
relative-call operand begins at `00445CBB`).

A range-wide scan of all Ghidra instructions found no constant-address STORE
into `0x502D00..0x502DFF`; the raw-immediate scan found the same references and
no extra reference to the range base. This negative result covers direct Ghidra
references, instruction immediates, constant-address P-code stores, and the
inspected sibling construction/registration paths. Computed-pointer writes and
initialization outside this executable remain possible. There is no remaining
static setup lead in these paths; runtime descriptor capture or an external
initializer is required before assigning resource IDs, frame cadence, or effect
lifetime. If neither is available, move to another supported gameplay gap
rather than guess. No renderer change or timing claim is supported by this
audit.

## Follow-up provenance

The asset and map inventory used local typed exports from the hash-pinned
original BSP/RRF files. The rescue BSP hashes are recorded above; the two RRF
hashes are recorded with their candidate resource mapping. The static descriptor
trace used the same executable SHA-256 recorded under Provenance and the saved
`FDNYPort` Ghidra program. These checks were static/read-only; no retail runtime
capture or live gameplay parity check was performed.

The rescue-all macOS app rebuilt after this shared-host change. Standalone and
packaged executable SHA-256 both equal
`d2a7f0c32b8e5f772ed32a8fd68ecafbdd57f3dfe33b1130681cb630956c2d61`. The
Info.plist validation passed, the build emitted all eight rescue choices, and
all 17 retail scene packages plus the existing custom test-room package were
copied. The linker warned that installed json-c targets macOS 27.0 while the
target is 13.3. No tests, interactive run, runtime emitter capture, or retail
parity check was performed.
