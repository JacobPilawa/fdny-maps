# Player oxygen visor effect (2026-10-15)

## Retail trace

The trace is pinned to the copied `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Read-only Ghidra 12.0.4 queries confirm that Player status initialization
`004576B0` calls `00456680` on status `+0x1B6C`; `00456680` constructs
resource `0x2BE` and configures child sounds `VisorUpDown` (`+0x134`) and
`VisorLock` (`+0x158`).

`rescue.rrf` line 73 maps resource 702 (`0x2BE`) to the copied color/alpha
pair `shield.bmp` and `shield_a.bmp`. Their SHA-256 hashes are
`f91b1c0e89365bc0e90a6454f76afbeacfacdfe3ae95da7b17dd7ecd9715068c` and
`aca848c349ad76da4ecd6cc9957f9fae16affb333b0320a0d475a5d5487f7d89`.
`sound.rrf` lines 34-35 map the child sounds to 2D resources
`visorupdown.wav` and `visordownend.wav`; their copied hashes are
`126e40bb53f787228d7bf6422d974bfee9d1c9fd0a68b0b83a5f89cbc79f4004` and
`4afefc800e9aca0d786acbfb553ad98c7ca401becd9114caaf4444e01d0b3c54`.

The `oxyon` handler `0044BD80` calls `00455AB0(Player+0x2160, true, 1.0)`;
`oxyoff` handler `0044BDB0` calls it with `false, 0.5`, then stops
`Player+0x3C4` (`Breathe`). `00455AB0` starts at clock minus 10 ms, converts
the duration from seconds to milliseconds with constant `004E0ED4 = 1000.0`,
sets an active latch, and requests `VisorUpDown` at both transition starts.
Its y endpoints are `-256 -> 736` for `oxyon` and `736 -> -256` for `oxyoff`.

While `start < clock < end`, `00455F60` interpolates the y position, truncates
it toward zero, uses UV bounds `(0.1, 0.1)..(1, 1)`, and sends rectangle
`(0, y, 640, y + 256)` to the polygon renderer. Constant `004E1FD0` is 256.0.
At transition end it plays `VisorLock` only for the oxygen-on direction, then
dispatches global sound-manager selector 4 or 5. Selector 4 attenuates sound
category 2; selector 5 restores categories 1 and 2. The category mix behavior
is ported and documented in
[`player_oxygen_audio_category_mix_port_2026-10-15.md`](player_oxygen_audio_category_mix_port_2026-10-15.md).
The manager's separate streamed-music channel remains outside the current host
mixer.

## Port

`PlayerOxygenVisorEffect` now preserves the source timing, strict visibility
window, endpoints, integer-truncated interpolation, and one-shot completion
edge. The viewer draws resource 702 between the fire-direction HUD and oxygen
gauge, plays `VisorUpDown` on `oxyon` and `oxyoff`, and plays `VisorLock` when
the oxygen-on transition completes. The oxygen sprite exporter now includes
the original resource-702 pair, and each rescue/stage sound bundle includes
both 2D visor resources.

The native renderer uses ordinary OpenGL alpha blending. It draws the source
UV rectangle and GUI geometry, but retail blend/filter state and a live retail
capture remain unverified. The sound-category mix is ported; retail audio
capture parity and the separate streamed-music channel remain open.

## Verification

- `sh tools/build_scene_viewer_app.sh rescue-all` succeeded. The CMake and
  packaged executables match at SHA-256
  `7f06dae47d70475a3b6caecad89b6520d7b06738edda4490054e4f045cd738ae`.
- Packaged MBase `--oxygen-hud-render-check` passed: the visor effect drew one
  layer and changed 335,480 framebuffer pixels; the two oxygen HUD layers
  changed 25,999 pixels. The check verifies both directions' midpoint y=240,
  strict start/end visibility, and the oxygen-on completion edge.
- `--mission-catalog-check` loaded all eight retail rescue entries.
- Packaged `--rescue-check` passed on Training, the seven primary rescues, and
  all five linked stages (13 packages total).
- All 13 packaged sound manifests include both 2D visor sounds with source
  hashes matching the copied WAVs. The resource-702 color/alpha exports match
  the copied source hashes.
- `git diff --check` passed.

### Selector-mix follow-up

The next build, SHA-256
`c0581c9561d0e34e34e96e18732560836a1627869b36eddf98a671241df25b90`, adds the
category 1/2 gains selected at visor completion. All seven authored ambient
maps pass `--ambient-audio-check`, including its selector-4 attenuation and
selector-5 restoration probe. Packaged MBase `--oxygen-hud-render-check` and
`--mission-catalog-check` also pass. The separate streamed-music channel and
audible retail parity remain open; see
[`player_oxygen_audio_category_mix_port_2026-10-15.md`](player_oxygen_audio_category_mix_port_2026-10-15.md).

These checks establish source-resource selection, packaging, expected effect
timing, and native overlay drawing. They do not establish the retail alpha
blend, audio-manager mix response, natural interactive event timing, or pixel
parity.

## Evidence files

- [`00455ab0_oxygen_visor_effect_fresh.c`](00455ab0_oxygen_visor_effect_fresh.c)
- Ignored local read-only Ghidra logs: `.local/oxygen-command-query.log`,
  `.local/oxygen-effect-init.log`, `.local/oxygen-effect-helpers.log`,
  `.local/oxygen-render-helpers.log`, and `.local/oxygen-constants.log`.
- Source resource tables: `inputs/original/Data/ResDefs/rescue.rrf` and
  `inputs/original/Data/ResDefs/sound.rrf`; asset hashes are in
  `inputs/original/manifest.csv`.
- Port: `port/spikes/training-native-prototype/src/player_oxygen.h`,
  `port/spikes/training-native-prototype/src/oxygen_hud_renderer.h`,
  `port/spikes/training-native-prototype/src/scene_viewer.cpp`,
  `tools/export_oxygen_sprites.py`, and `tools/export_evs_bundle.py`.
