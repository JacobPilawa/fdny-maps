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

`0x502D40` is passed as a pointer to a static descriptor, not established as a
small RRF resource ID. The saved program image shows zeroed BSS bytes at that
address; `0047F850` passes it to `0047D370`, which stores the descriptor pointer
and reads its `+0x18` field. The dynamic descriptor initialization and the
actual renderer resource/path are not identified by this trace. The four
children's visible appearance, position/update cadence, and one-second visible
exposure are therefore still unresolved.

## Port status and next trace

The shared scene host currently routes `setsize` only to `PreviewFire` at
`scene_viewer.cpp:3976`; it has no `RCS_MiscEmitter` runtime representation.
Adding only an active flag and scalar scale would not reproduce the child
effect. Next trace the descriptor initialization and mode-2 child update/draw
consumer, including any shared RNG use. Then add a generic cross-platform
MiscEmitter implementation that handles the map-authored parameters and
commands. The existing `victim_spawn_rng_port_coverage.md` also records missing
MiscEmitter constructor draws; this trace does not resolve those draws.

This checkpoint is static source/data analysis only: no code change, build,
gameplay run, or retail-parity check was performed.
