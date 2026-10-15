# Player oxygen audio category mix port (2026-10-15)

## Plain-language explanation

When the player finishes turning oxygen on, retail lowers sound category 2 to
30% and updates its separate streamed music channel. Turning oxygen off restores
categories 1 and 2 and updates that music channel again. The shared viewer now
applies the verified category-volume changes to active and newly started voices.
It does not yet model the manager flag or separate streamed music channel.

## Retail trace

The trace uses the copied `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
At visor-effect completion, `00455F60` dispatches selector 4 for oxygen-on and
selector 5 for oxygen-off. In `0042A730`, selector 4 sets category 2 to
`004DF7B0 * 004FF488`; selector 5 restores categories 1 and 2 to `004FF488`.
The copied float values are `004DF7B0 = 0.3`, `004FF488 = 1.0`, and
`004FF48C = 1.0`.

Selectors 4 and 5 also set and clear the manager byte at `+0x6A8`.
`004A07C0` stores a category volume and visits registered sound instances,
matching their `+0x14` field before updating their FMOD channels. The
`ConfigureAndRegisterSoundInstance` helper at `004A12F0` stores its third
argument at `+0x14`. Source constructors establish these raw IDs:

- Category 1: `VisorUpDown`, `VisorLock`, Player `Breathe`, and the
  `RC_OxygenLow` one-entry chatter sound.
- Category 2: Player `OxyRelease`, Player jump/landing effects, spatial EVS
  `playsnd`/`loopsnd`, and `ambientsnd` requests.

The selectors also call `SetFMODChannelVolume` on the separate stream channel
at manager `+0x690`. `SceneAudio` currently mixes bundled PCM voices and does
not model the manager byte or expose that music stream.

## Port

Each `SceneAudio::Voice` carries the raw category ID. The callback multiplies
the existing spatial gain by the current category gain, so an oxygen transition
also changes voices already playing. Oxygen-on completion plays category-1
`VisorLock` first, then applies selector 4. Oxygen-off completion applies
selector 5. Category gains are protected by SDL's audio-device lock. This ports
the category-volume portion of selectors 4 and 5.

The dummy-device ambient diagnostic exercises the same selector helper. It
checks category 1 stays at 1.0, category-2 ambient voices resolve to 0.3 after
selector 4, and both categories return to 1.0 after selector 5.

## Verification

- `sh tools/build_scene_viewer_app.sh rescue-all` succeeded. The CMake and
  packaged executables match at SHA-256
  `c0581c9561d0e34e34e96e18732560836a1627869b36eddf98a671241df25b90`.
- Packaged `--ambient-audio-check` passed on Gas R (3 loops), Bar R (4), Bank
  (6), Boat A (3), Boat B (2, including its timed `Ambient2` start/stop),
  Subway A (6), and Subway B (6). Each check also passed the category selector
  probe.
- Packaged MBase `--oxygen-hud-render-check` passed: the visor drew one layer
  and changed 335,480 pixels; the three oxygen HUD layers changed 26,849 pixels.
- Packaged `--mission-catalog-check` loaded all eight retail rescue entries.
- `git diff --check` passed before this note was written; rerun after final edits.

These checks establish the viewer's category gain routing and category-volume
selector changes. They do not establish audible output parity, the manager
byte's effects, or the separate music stream's response.

## Evidence files

- [`0042a730_oxygen_audio_mix_selectors_fresh.c`](0042a730_oxygen_audio_mix_selectors_fresh.c)
- Ignored read-only Ghidra logs: `.local/oxygen-selector-dispatch.log`,
  `.local/oxygen-audio-manager-followup.log`,
  `.local/oxygen-audio-group-consumers.log`,
  `.local/oxygen-chatter-group.log`,
  `.local/oxygen-chatter-constructor.log`,
  `.local/oxygen-audio-volume-constants.log`, and
  `.local/audio-instance-registration-xrefs.log`.
- Existing visor animation trace:
  [`player_oxygen_visor_effect_port_2026-10-15.md`](player_oxygen_visor_effect_port_2026-10-15.md).
