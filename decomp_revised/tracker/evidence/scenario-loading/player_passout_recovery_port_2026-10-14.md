# Player state-6 passout recovery port (2026-10-14)

Status: the shared viewer runs the mapped delayed recovery transaction and
renders the source-shaped state-6 camera blend. The Player frame scheduler,
selected-item view offsets, loaded-profile difficulty, and mode-2 HeroDown path
remain incomplete or unverified.

## Plain-language explanation

Retail does not leave a downed Player frozen forever. `00451080` keeps the
Player in state 6 through 1000 ms, then calls `0044FF10(player, 0)` after the
timer passes 1000 ms. The passout code first moves the Player to a separate
safe-position snapshot so a carried victim can be dropped there. The final
Player location comes from a different checkpoint, followed by floor
placement. The viewer now models these two saved positions separately, drops a
carried victim at the safe snapshot, restores the checkpoint, and unlocks
movement after the delayed transaction.

`0049E0A0` receives a collision normal at collision-record `+0x0C`; its Y
component is at `+0x10`. When that component is at least `0.5`, it sets Player
byte `+0x12C`. During normal movement, `00452570` copies the current Player
translation into the `+0x3400` safe snapshot only when all four directional
burn flags are clear, the cooled burn accumulator is below `0.1`, the latch is
set, and oxygen is above `0.5`. `0049DBC0` later copies the transient latch to
`+0x12D` and clears `+0x12C`.

The distinct reset checkpoint `+0x534` is seeded from the starting Player
frame. The authored Player `escape` command can refresh it when the collision
latch is set and oxygen exceeds `0.5`. `0044CD10` restores that checkpoint,
resets movement state, copies the checkpoint into the safe snapshot, and
performs floor placement.

## Port changes

- `Player` now retains the collision latch, safe snapshot, checkpoint position,
  and checkpoint heading as separate fields. The host sets the latch from the
  mapped upward-normal threshold and updates the safe snapshot through the
  source burn/oxygen gate.
- The `escape` handler captures the checkpoint using the mapped latch and
  oxygen condition.
- State 6 skips ordinary Player movement and calls the passout transaction on
  the first host update whose unsigned elapsed time is strictly greater than
  1000 ms. The existing transaction handles victim motion/drop, Player reset,
  floor placement, and the survivor-status message.
- The rescue-all app was rebuilt with this path. The interactive one-second
  camera blend is not rendered because its full retail camera inputs are not
  available. The viewer now uses global mode 1, matching a fresh/reset retail
  profile. It does not load an existing profile's difficulty; its mode-2
  HeroDown spawn/count path remains unsupported and fails explicitly if
  reached.

## Evidence and verification

Retail evidence is from the pinned executable exports:

- `reference/previous-work/decomp/evidence/startup-factories/00451080_FUN_00451080.c`
- `reference/previous-work/decomp/evidence/fresh/0049e0a0_FUN_0049e0a0.c`
- `reference/previous-work/decomp/evidence/fresh/0049dbc0_FUN_0049dbc0.c`
- `reference/previous-work/decomp/evidence/gameplay/00452570_FUN_00452570.c`
- `reference/previous-work/decomp/evidence/fresh/004513a0_player_escape_prepass.c`
- `reference/previous-work/decomp/evidence/player-tool-pose/0044cd10_FUN_0044cd10.c`

The collision-latch lifetime across retail callbacks is not known. The viewer
keeps its host latch visible through the current movement/cooling/script pass
and clears it at the next Player step; that outer cadence is an adapter choice,
not a confirmed retail schedule. Equipped-item view offsets, mode-2
cleanup/spawn behavior, and live callback cadence remain open.

Targeted checks on the rebuilt rescue-all app:

- Packaged executable SHA-256:
  `79226980f99084f01b4d97ff5ab8892194f2e14b07de687dfebee7412443bf7f`.
- `--player-passout-recovery-check`: strict 1000/1001 ms boundary, unsigned
  clock wrap, and state gate pass.
- `--rescue-check` on Training, MBase, Bar R, Gas R, Bank, Boat A, Subway A,
  and Oil Rig A: authored pickup/escape transactions pass, with one controlled
  carried-victim passout drop, checkpoint/floor reset, and re-pickup per map.
- `--mission-catalog-check`: all eight retail rescue choices load.
- `--oxygen-hud-render-check` with Training selected: both HUD layers draw and
  change framebuffer pixels.

These checks exercise the native adapter and authored synthetic route. They do
not verify a natural fire/oxygen passout in interactive retail or establish
frame-scheduler parity.

## 2026-10-15 shared-viewer camera port

The shared viewer now calls the existing `ViewUpdate::transition` path while
Player state is 6. It uses the source factor `1 - elapsed_ms * 0.001`, the
position endpoint `(0,-20,-30)`, target endpoint `(-60,1000,0)`, the Player yaw
frame, head-motion output, and a follower seeded from the visible host camera.
The renderer and static-mesh culling consume the resulting full view basis, so
the transition changes camera position and orientation instead of only
changing the yaw/pitch inputs. The `playerPassoutViewFactor` helper shares the
unsigned elapsed calculation with the strict state-6 recovery deadline.

The selected tool's live view/aim offsets at Player item fields `+0x1E4` and
`+0x1F0` are not yet wired into the shared viewer; this pass supplies zero,
which matches the unselected/default item and the traced static extinguisher
setup. The stateful follower is initialized at passout from the host camera,
and its state before passout has not been scheduled through the retail Player
view updater. These adapter inputs keep the implementation source-shaped but
do not establish full camera parity for every equipped item.

Targeted checks on 2026-10-15:

- Rescue-all app build completed; packaged `--render-check` loaded 121/121
  Training groups and 24/24 textures with zero texture failures. Packaged and
  CMake viewer binaries match at SHA-256
  `79b4f8442f03b714dd05ac86b894a2cf7728febfda1f89dcdad8f5429cf684ef`.
- Packaged `--mission-catalog-check` loaded all eight rescue entries.
- Rebuilt Training `--player-passout-recovery-check` passed factor endpoints
  1.0 at entry and 0.0 at 1000 ms, strict recovery at 1001 ms, clock wrap, and
  the state gate.
- `--view-update-check` could not run because its referenced
  `evidence/view-update-oracle.json` fixture is absent from this checkout. The
  historical view-helper oracle results above remain the saved numeric
  evidence; no new exact matrix comparison was made for this shared-viewer
  integration.

No live camera capture was made. The state-6 timing and endpoints are source
mapped; equipped-item offsets, view-updater cadence, and rendered parity
against retail remain open. The viewer's fixed global mode 1 matches the
fresh/reset retail profile default; loaded-profile difficulty and the mode-2
HeroDown spawn/count branch remain unsupported.
