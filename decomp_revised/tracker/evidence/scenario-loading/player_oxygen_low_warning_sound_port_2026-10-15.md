# Player low-oxygen warning sound port (2026-10-15)

Status: the source-mapped one-shot low-oxygen chatter cue now reaches the
shared viewer's audio mixer and is bundled into every rescue and stage package.
The warning's orange gauge layer is also source-mapped and ported; its native
alpha ramp and retail mixer parity remain provisional.

## Retail behavior

The copied `inputs/original/binaries/FDNYFirefighter.exe` remains pinned at
SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The saved `00458400` status updater eases the displayed oxygen value, then,
once it is below `0.25` and its warning latch is clear, calls
`004897C0(Player+0x3540, "RC_OxygenLow", 1)` and sets the latch. The latch
resets when displayed oxygen reaches `0.5`. `004897C0` resolves the chatter
set and selects the sequential playback path `00489110` for flag 1.

`00489110` increments the set index, wraps at the entry count, resolves the
current sound resource, and calls `004A11A0` to play it. Its body has no shared
RNG call. The original
`inputs/original/Data/ResDefs/sound.rrf` maps the one-entry set to
`RC_OxygenLow0`, a 2D sound backed by
`Data/Snd/chatter/o2low.wav` (SHA-256
`17a4a977164ba7dd8fd2f04de0220964ef90f00f58febcefbd59c649396269ba`).

## Port changes

- `tools/export_evs_bundle.py` now always includes `RC_OxygenLow0`, alongside
  the fire pain and passout chatter resources already added outside authored
  EVS sound commands.
- When `updatePlayerOxygenWarning` reports the source one-shot edge, the shared
  scene host calls `SceneAudio.play("RC_OxygenLow0", ...)`. The existing helper
  owns the `<0.25` assertion and `>=0.5` latch reset. Because the retail chatter
  set contains exactly one entry, no synthetic random selection was added.
- The rescue-all builder exports and bundles the sound for all 13 packages.

## Verification and limits

The rebuilt packaged executable SHA-256 is
`fa1d09c3d6c94cb96d044f443156cf458b95dc83698b7594df2e76d75c50383a`, matching
the CMake executable. A package scan found exactly one `RC_OxygenLow0` row in
each of Training, the seven primary rescue maps, and the five linked-stage
packages; each row's WAV hash matches the source hash above. The packaged
mission-catalog check still loads all eight primary rescue choices. The
packaged oxygen HUD render check still draws both gauge layers and changes
25,999 framebuffer pixels.

These checks verify bundling, sound identity, catalog loading, and that the
oxygen HUD renders. They do not capture SDL audio or compare the retail mixer,
warning appearance/blend, or event cadence in a live game.

## Evidence

- Pinned-executable decompilations: `reference/previous-work/decomp/evidence/startup-factories/00458400_FUN_00458400.c`, `reference/previous-work/decomp/evidence/startup-initializers/004897c0_FUN_004897c0.c`, and `reference/previous-work/decomp/evidence/startup-draw-order/00489110_FUN_00489110.c`.
- Sound definition: `inputs/original/Data/ResDefs/sound.rrf`, line 43; source WAV hash also appears in `inputs/original/manifest.csv`.
- The warning overlay source trace and render limitation are documented in [`player_oxygen_low_warning_visual_port_2026-10-15.md`](player_oxygen_low_warning_visual_port_2026-10-15.md).
