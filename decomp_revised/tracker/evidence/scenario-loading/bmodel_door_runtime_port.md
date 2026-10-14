# Shared BModel and BoundsTrigger runtime port checkpoint

## Retail evidence used

- `FDNYFirefighter.exe` SHA-256:
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- `Genesis.dll` SHA-256:
  `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
- `InitializeBModelEntity` (`0043C670`) sets the initial frame to the minimum
  child-motion endpoint and writes `1.0f` to the per-BModel speed field at owner
  `+0x1DC`. The fresh pseudocode is
  [`0043c670_bmodel_init_fresh.c`](0043c670_bmodel_init_fresh.c).
- `DispatchBModelScriptCommand` (`0043E680`) maps `open`/`close` to states 3/4
  and `settimescale` to that owner speed field. `UpdateBModelEntity`
  (`0043DC60`) advances the frame by the global delta at `00502B04` times that
  speed, clamps at the authored motion endpoints, and samples each child PATH.
  See [`bmodel_update.md`](bmodel_update.md) and
  [`bmodel_door_speed_semantics.md`](bmodel_door_speed_semantics.md).
- `DispatchScriptActorContactEvents` (`00470840`) maps new/persistent/removed
  contact pairs to `touch`/`touching`/`untouch`. The BModel contact callback
  tests the Player bounds against the BModel bounds expanded by effective
  `E_Reach`; see
  [`rescue_contact_manager_route.md`](rescue_contact_manager_route.md).
- `RCS_BoundsTrigger` is type 8 and tests type-1 candidates using the same
  overlap helper. Its contact producer records pairs against the trigger's
  authored model bounds, and its embedded ScriptActor dispatches the shared
  events. Subway A has 43 trigger entities; 25 `brokendoor` triggers bind a
  touch event that expands to `playsnd Zapp3d`. The class route and its
  unresolved scheduler are described in
  [`rescue_contact_manager_route.md`](rescue_contact_manager_route.md).
- Subway A's `subdoor` EVS binding applies `settimescale 4`, plays `SubDoor`,
  and opens on touch; untouch plays the sound and closes. Its 17 BModels have
  three animated children each. Bank's `VaultConsole` checks and sets
  `VaultUnlocked`, then sends `tell vault open` to BModel entity 441. See
  [`subway_a_interaction_path.md`](subway_a_interaction_path.md) and
  [`bank_vault_open_dispatch.md`](bank_vault_open_dispatch.md).
- Bank's shared `doors.evs` defines `DoorOpen` as
  `cmd [flush, openaway, playsnd OpenDoor]` and `DoorClose` as
  `que [close, playsnd CloseDoor]`. Eleven Bank BModels use `*Door`; each has
  one animated child with a two-key rotation path. The vault's separate
  `*VaultDoor`/`tell vault open` route is not this contact macro.
- `openaway` is dispatched by `0043E680` to `0043D6A0`. The latter checks
  every child rotation PATH and only changes a child whose rotation has exactly
  two keys. It derives the near-to-far horizontal direction from the owner's
  unioned child root bounds contracted by 128, rotates it by the relative
  keyframe transform, and transposes keyframe 1 when that direction points
  toward the player. It queues itself while the BModel is closing. See
  [`training-door-motion-audit.md`](../../../reference/previous-work/decomp/evidence/training-door-motion-audit.md)
  and [`0043d6a0_FUN_0043d6a0.c`](../../../reference/previous-work/decomp/evidence/entity-dispatch/0043d6a0_FUN_0043d6a0.c).

## Port behavior

`port/spikes/training-native-prototype/src/scene_viewer.cpp` now loads each
BModel's children, retail PATHs, initial path range, authored reach, and a
world AABB formed from the exported child-model AABBs. It dispatches a
per-owner `touch`/`touching`/`untouch` edge from the live Player box, applies
`open`/`close`/`openaway`/`opentowards` and per-owner speed commands, and samples the resulting child
poses into both renderer and collision state. A named ScriptActor registry
resolves `tell vault open`. Press **E** or **Space** near a BModel with a `use`
binding; this reaches Bank's authored console sequence. BoundsTriggers resolve
their `C_Model` through the map's `%Model%` table to exported collision bounds
and dispatch player `touch`/`touching`/`untouch` events. Subway A's
`brokendoor` touch reaches its authored `Zapp3d` command and bundled WAV.
Subway's `SubDoor` and other bundled EVS WAV resources play through SDL audio.
The host's source-backed `makezone` command adds each named BoundsTrigger
volume to the shared zone tree; Subway A builds six named zones and Bank builds
ten from their authored trigger commands.

An earlier Bank door issue was a host gap: `DoorOpen` reached the supported
`playsnd` after `openaway` was reported unimplemented. The viewer now ports the
source direction choice across every child with a two-key rotation PATH,
preserves the endpoint change, and advances ScriptActor queues once per
rendered frame so the `DoorClose` queue can run. BModel owner poses update in
the normal gameplay loop and feed the same `Collision::poses` map used by
rendering and collision. `M` samples an inspection PATH timeline and `N`
clears it; renderer-owned live BModel poses now take precedence, so inspection
cannot mask the pose used for collision. Contact edge logs print the owner,
state, speed, and PATH time at touch, and active PATH state/time progress is
logged while a door moves.

The user subsequently reported that a live Bank door still plays its approach
sound but remains impassable until `M` is pressed. Their latest note reiterates
that ordinary approach/contact should cause the animation and collision to
move; `M` is inspection-only and does not change gameplay state or collision.
The first synthetic Bank fixture selected entity 365, about 3,300 units from the
authored spawn, rather than a start-area door. The nearest eligible `*Door` is
entity 541, about 421 units from spawn. The current `--interaction-check` now
walks from the authored spawn using `Player::step`, updates BModels, dispatches
contact edges, and samples live PATH poses into collision. On the current Bank
package, approach automatically dispatches entity 541's `touch`, plays
`OpenDoor`, and reaches PATH time 1 without M. A first direct diagonal attempt
kept the player at x=-697 (24 units from the door-bounds center) and could not
cross; a trace hit static world model 0 at `(-697.16, -33.90, 809.77)` with
normal `(0, 0, 1)`. The revised fixture approaches straight to the automatic
touch, waits for the authored PATH endpoint, applies a short keyboard-like
lateral input to center at the opening, then walks through with `Player::step`
and the live collision pose. That pass succeeds. The harness waits up to 180
synthetic frames for the endpoint before centering and crossing; it demonstrates
the contact/PATH/collision path only under fully-open, aligned conditions and
does not establish ordinary live approach timing. A read-only source audit
checked the current M/N and gameplay paths: M/N only change `previewMotionTime`,
detached `inspectionPoses`, and preview playback; each frame's live BModel
update separately samples child PATHs into the same `Collision::poses` map used
before `Player::step`. The current source contains no M-to-collision command
path. The off-center trace does not
identify a rendered face or prove an engine defect: it may be a normal jamb
collision. Retail static world sweeps and animated BModel transforms are
separate, so world model 0 can still block a poorly aligned route. The user's M
workaround remains unresolved until their interactive route and exact app are
checked.

The interactive loop logs `BModel contact result` immediately after `touch`
and logs `BModel live PATH` when state changes or time advances by at least
0.1. To narrow the live report, capture those lines for the touched door with
the player's position and the obstruction during the same approach. The audio
cue can precede visible movement because the authored contact command begins
with `playsnd`; a contact result that remains state 0 differs from an advancing
PATH whose door leaf has moved but another collision model still blocks
passage.

The Bank start has two nearby green trigger brushes, `lbbyzone` entity 466/model
300 and `bankzone` entity 467/model 301. Both use `obj_suvgreen`; the earlier
viewer resolved and suppressed both model IDs in `renderScene`, while retaining
their AABB contacts. The visual suppression has since been removed from both
viewer render loops. The collision exclusion remains separate from rendering.
No fresh build or render check has been run after this change.

A static start-view ray audit found a candidate ordinary world surface before
the trigger brushes: model 0 `wal_Stco_wall`, face 4947/TexInfo 2107, about 405
units from the start eye. The preview converts authored heading to radians and
uses inverse view yaw, which points heading 0 along -Z; retail `C_Heading`
semantics remain unverified.
That TexInfo has alpha 255 and its face has `light_ofs = -1`. This is not a
pixel-to-face match for the user's window. The offscreen render-check frame at
`.local/exports/bank.render-check.bmp` shows a large dark polygonal obstruction
but does not map it to this face or match the user's interactive screenshot. A
source audit found the renderer preserved model IDs through batching and
skipped meshes whose IDs were in the hidden BoundsTrigger set. That visual
filter has since been removed; trigger meshes now pass through the ordinary
render path. The source has not been rebuilt or visually checked after this
change. Export and app-bundled `entities.json`,
`geometry.json`, and `world_mesh.json` have matching hashes. The bundled
executable is older than the current build executable (`8515b3b1...e6842`
versus `9c58160a...f3d1`); the provided launch command rebuilds and copies the
binary before launching, so a stale already-running or directly launched
process remains a possibility. Identify the visible group in a fresh capture
before treating this as an alpha defect. Broader material alpha, lightmap, and
face-flag mapping remains open; see
[`bank_start_view_surface_audit.md`](bank_start_view_surface_audit.md).

`tools/export_evs_bundle.py` copies the same-key map EVS source and its
recursive `exec` includes into each scene package, and copies the WAV resources
named by `playsnd` commands from the copied `sound.rrf`/`Data/Snd` inputs. The
package JSON retains the input paths and SHA-256 hashes. The Mac app builder
runs this exporter before bundling the scene.

## Verification

- The viewer builds with CMake.
- `fdny-scene-viewer .local/exports/subway_a --interaction-check` passed. The
  host dispatched the authored touch list, applied speed 4, reached PATH time
  1, dispatched untouch, and returned the three-child door to PATH time 0.
  The same check resolves trigger 515's `C_Model` bounds and dispatches its
  touch event to `Zapp3d`. Startup also builds Subway A's six named zones from
  its `makezone` commands.
- The earlier Bank teleport fixture selected nearest eligible `*Door` entity
  541 (421.2 units from the authored spawn bounds); synthetic touch opened and
  untouch closed it through `openaway`/queued `close`, dispatched `OpenDoor`
  and `CloseDoor`, and sampled the child PATH into the shared render/collision
  pose. It also validated `VaultUnlocked=1` and `tell vault open` to entity
  441. That fixture did not prove a live approach or passage.
- The revised source `--interaction-check` walks from the authored spawn with
  `Player::step` and the interactive loop's BModel update/contact ordering. It
  confirms entity 541 receives touch without M, plays `OpenDoor`, reaches PATH
  time 1, then crosses the opening after waiting for the endpoint and strafing
  to center. This is a controlled headless route, not an interactive capture;
  it does not establish ordinary approach timing, and the user's reported M
  workaround still needs reproduction with the exact launched app.
- The current build adds a separate uninterrupted approach aimed at the
  authored door-bounds center. It makes no stop, wait, or side input: entity
  541 receives touch at `(-697, -33.9, 908.148)`, plays `OpenDoor`, and reaches
  PATH `1/1`; the player then stops at `(-697, -33.9, 810.28)`, 24.011 units
  off the authored bounds center in X. A fresh swept trace hits static world
  model 0 at `(-697.029, -33.9, 809.773)`, normal `(0, 0, 1)`. After the
  fixture restores player, door, collision pose, and script queue state, the
  existing control route still crosses at `(-719.419, -33.9, 721.447)` after
  waiting for the endpoint and applying lateral input. This narrows the
  synthetic failure to the straight approach's world-collision corridor; it
  does not identify the exact BSP leaf or establish that retail would use the
  same approach/callback timing.
- A read-only retail trace of pinned exports confirms the Player sweep wrapper
  passes its swept hull to `geWorld_Collision` with mask `0x43`, type `2`.
  Genesis selects its swept-box path when extents are present, checks static
  and transformed model records, and excludes models only when disabled by
  model flags. The retail BModel updater advances child PATHs through
  `geWorld_SetModelXForm`, so animated leaves feed collision transforms. These
  paths make a remaining static model-0 jamb/world hit plausible, but do not
  prove the port's exact collision surface or whether the interactive retail
  route should pass at this offset. No live Ghidra database or retail capture
  was available for this follow-up; it used the saved hash-pinned exports.
- A separate pinned-EXE check resolves the standing Player hull initialized at
  `0044D550`: mins `(-30,-30,-30)`, maxs `(30,85,30)`. `0043F810` passes these
  fields to `00492CE0` before the Genesis swept-hull path. The port's standing
  hull matches these bounds, so shrinking it is not supported as a fix. Exact
  collision leaf/plane and the user's live Bank route remain unverified.
- An earlier `--render-check` uploaded 101/101 textures, but predates this
  build. The current `--interaction-check` decoded all 105/105 Bank textures
  in inspect mode and skipped GL upload. The packaged app's executable has now
  been refreshed from the current source build; its SHA-256 matches
  `13635bb4299491eed67a52f49a55b5c10a4fc93bec6d8c4d10d6a871c2ee01c7`. The
  exact packaged Bank executable/resources then passed `--interaction-check`
  with the same route result. The current app has not yet had an interactive
  visual check. Neither capture identifies the surface in the user's
  interactive screenshot.
- `--inspect` loads both scene packages with zero texture decode failures and
  grounded spawn checks. It resolves all 43 Subway A and 14 Bank
  BoundsTrigger model bounds and builds six Subway A / ten Bank named zones.
- Bank's `--inspect` reports the spawn clear and the floor sweep hitting world
  model 0. The trigger model is still present in the shared collision data;
  whether retail's Genesis model flags disable collision for it remains
  unverified, so this checkpoint does not change trigger collision handling.

The Subway A and earlier Bank fixtures inject contacts at authored model
bounds; the revised Bank fixture walks with `Player::step`, derives its contact
from the live overlap loop, and tests both a continuous aimed approach and an
endpoint/alignment control. The continuous route opens the leaf but stops at
static world model 0; the control aligns at the aperture and passes. This
headless check does not capture a retail or interactive desktop session, and
callback timing remains provisional.

### Fresh packaged interaction checks (2026-10-14)

After the oxygen integration, `sh tools/build_scene_viewer_app.sh rescue-all`
rebuilt the packaged executable at SHA-256
`5bbb5190739afcd26c219e961bd75bd128d463fed91d445d9a68fca083d76dff`. Its
Bank `--interaction-check` reproduced the continuous-route result above and
passed the centered control: entity 541 received touch after 47 approach
frames, played `OpenDoor`, and crossed after lateral alignment. The check
reported zero texture decode failures across 105 Bank textures. The Subway A
`--interaction-check` applied touch, speed 4, and open to entity 274, closed on
untouch, returned its PATH to time zero, and dispatched `Zapp3d` from trigger
515; all 81 textures decoded. These checks support shared-host behavior under
controlled conditions. They do not verify ordinary interactive approach
timing or retail callback cadence.

## Provisional behavior and open gaps

- The host uses clamped rendered-frame `dt` as the global BModel delta. Retail
  delta units, updater frequency/order, and the callback cadence remain
  unknown, so visible duration and event latency are not timing-parity claims.
- Quaternion multiplication plus conjugation represents the XForm multiply and
  transpose in `0043D6A0`; the result is source-shaped, but its float bits have
  not been compared against the shipped Genesis DLL. Child rotational centers
  come from exported collision-model origins, and the owner direction bounds
  come from each child's root node with the source's 128-unit inset.
- ScriptActor queue advancement is provisionally once per rendered frame. The
  retail updater processes at most one queued command per callback, but the
  callback scheduler and its cadence remain unresolved.
- The contact box is reconstructed by unioning the exported child model
  AABBs. The overlap formula and reach gate are source-backed, but the exact
  retail BModel bound-construction path (including helper `0043B7A0`, if it is
  the relevant initializer) has not yet been mapped against this union.
- BoundsTrigger contact uses each `C_Model` collision model's exported AABB
  and authored `E_Reach`. The overlap helper is source-mapped, but live Player
  contacts and exact world bounds have not been captured in the game. The host
  currently evaluates contacts once per rendered frame; retail callback
  ordering and cadence remain unknown.
- SDL playback uses the retail sound definitions and samples, but applies a
  linear distance rolloff and simple stereo pan. FMOD rolloff/audibility parity
  remains open; audio has not yet been heard in an interactive desktop run.
- The host's contact scheduling, queued/reversal cadence, `addtimecmd`, and
  other actor commands remain incomplete. The MBase `tell player escape`
  route now reaches the shared release/exit transaction and passes a synthetic
  headless dispatch check; live exit contact, carried-victim presentation,
  mission completion, and full fire/equipment behavior remain incomplete.
