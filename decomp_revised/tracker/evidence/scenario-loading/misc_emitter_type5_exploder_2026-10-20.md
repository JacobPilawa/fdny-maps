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

The shared scene host currently routes `setsize` only to `PreviewFire` at
`scene_viewer.cpp:3976`; it has no `RCS_MiscEmitter` runtime representation.
The command/state behavior is source-backed, but a visible effect cannot be
implemented faithfully without the descriptor's numeric resources and track
data. Next recover the runtime descriptor contents through a resource-loader
trace or memory capture, then port the generic actor and its authored commands
in the shared mission host. The existing
`victim_spawn_rng_port_coverage.md` also records missing MiscEmitter constructor
draws; this trace finds no direct RNG call in the inspected mode-2 methods but
does not resolve all resource initialization or queue-order draws.

## Follow-up provenance

The update/child/resource traces used Ghidra 12.0.4 headless reads of the
saved `FDNYPort` program and raw current-hash instruction windows. Temporary
logs are under `/private/tmp/fdny_misc_*`; the `.data` RVA/contents were checked
against the hash-pinned PE. The executable is still
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. This is
static evidence only: no runtime memory capture, code change, build, gameplay
run, or retail-parity check was performed.
