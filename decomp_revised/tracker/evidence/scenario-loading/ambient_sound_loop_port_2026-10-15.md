# Authored ambient sound loops in the shared viewer

## Plain-language explanation

`ambientsnd` starts a 3D sound loop at the map coordinates written in EVS. The
retail script path truncates each decimal coordinate toward zero, then enables
FMOD loop mode 2. The shared viewer now registers one persistent mixer voice
for each command and updates its listener position as the Player moves.

The viewer uses the bundled sound resource's 3D distance values, but its
linear attenuation and stereo panning remain approximations of FMOD. The
headless rescue checks verify EVS parsing and loop registration. The separate
dummy-device audio check confirms bundled samples become looping 3D voices and
accept listener updates; it does not compare captured output with retail.

## Retail source path

The code evidence is pinned to
`inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

- Script command dispatcher `0047A1F0` recognizes `ambientsnd`, reads its
  sound name and three coordinate operands, integer-casts the coordinates,
  and calls `00475210`.
- `00475210` converts its three integer coordinate inputs to binary32 world
  coordinates and calls the sound instance manager `004749A0` with optional
  mode `2`.
- `004749A0` stores the sound name, fixed source position, owner/context, and
  game-clock time, then configures and registers the sound instance through
  `004A12F0` and `004A11A0`.
- `004A11A0` passes the nonzero mode to `DispatchOrQueueSoundPlayback`
  (`004A0BE0`). `StartFMODSampleOrStream` (`004A00F0`) maps a non-null loop
  request to FMOD loop mode `2` before starting a sample.

The relevant saved decompilations are
[`004749a0_spatial_sound_instance_fresh.c`](004749a0_spatial_sound_instance_fresh.c),
[`004a00f0_start_fmod_sample_or_stream_fresh.c`](004a00f0_start_fmod_sample_or_stream_fresh.c),
and the ambient command branch in
[`0047a1f0_script_command_dispatch.c`](0047a1f0_script_command_dispatch.c).
The shared resource lookup, 3D attribute, and playback route is summarized in
[`subway_a_interaction_path.md`](subway_a_interaction_path.md).

## Port and verification

The shared viewer's `ambientsnd` handler in
[`scene_viewer.cpp`](../../../port/spikes/training-native-prototype/src/scene_viewer.cpp)
normalizes comma-separated coordinates, reads numeric coordinate operands,
and truncates them to signed integers before creating a loop. Each command
gets a unique owner key so two loops using the same sound name keep distinct sources.
The frame loop calls `SceneAudio::updateOwned` with the static source, current
Player listener, and current yaw. `SceneAudio::loopOwned` sends the request to
the existing 3D resource mixer. The rescue-all app builder's
[`export_evs_bundle.py`](../../../tools/export_evs_bundle.py) now includes
`ambientsnd` resource names when it copies map-specific WAV assets and 3D
distance metadata.

The compiled shared viewer's `--rescue-check` passed the delayed-startup and
synthetic all-victim rescue transaction checks in all 13 bundled mission
packages. It registered ambient requests in Gas R (3), Bar R (4), Bank (6),
Boat A (3), Boat B (2), Subway A (6), and Subway B (6). Its
`--ambient-audio-check` opens SDL's dummy device, starts each request from the
bundled WAV, verifies that its voice is looping and 3D, then checks source and
listener state after a listener update. All seven maps passed. On Boat B, the
same diagnostic advances a copy of the authored `button` BModel path across
keys `0.1` and `29`, dispatches `loopsnd Ambient2` and `stopsnd Ambient2`, and
verifies that the voice starts and is removed. This validates the controlled
timed-command route, not live path timing or audible parity with retail.
