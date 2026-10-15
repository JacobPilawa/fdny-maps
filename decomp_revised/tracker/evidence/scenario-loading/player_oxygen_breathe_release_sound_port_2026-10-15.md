# Player oxygen breath and recovery sounds (2026-10-15)

## Plain-language explanation

When oxygen is active, retail plays the 2D `Breathe` sound at each scheduled
oxygen decrease. Turning oxygen off stops the active `Breathe` sound. If the
resource later recovers from the low-resource state all the way to full, retail
plays `OxyRelease`. The shared viewer now follows those source events and
bundles both original WAVs in every rescue and linked-stage package.

## Retail path

Evidence is pinned to `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

- Player initialization `0044D550` configures the sound instance at Player
  `+0x3C4` with the key `Breathe`. `Data/ResDefs/sound.rrf` line 24 defines it
  as 2D `Data/Snd/gasmask.wav`.
- The normal oxygen updater `00451560` calls
  `004A11A0(Player+0x3C4, null)` before subtracting `0.05` when the drain
  deadline is due. It schedules the next drain at the current clock plus
  3000 ms. The trace maps a sound-instance request at each drain tick; its
  continuous-loop behavior is not established here.
- `oxyoff` reaches `0044BDB0`, which sets the drain deadline to
  `0x7fffffff`, calls the separate effect handler `00455AB0` on Player
  `+0x2160`, and calls `004A1080` on the sound instance at `+0x3C4`. The
  pinned thunk at `004A1080` calls
  `FSOUND_StopSound_4` when that instance has a nonzero channel handle.
- Player initialization configures the sound instance at `+0x40C` with the
  key `OxyRelease`; `sound.rrf` line 25 defines it as 2D
  `Data/Snd/gasmaskrelease.wav`. When oxygen refills to at least `1.0`,
  `00451560` requests this sound only if the low-resource byte at `+0x52C` was
  set, then clears that byte.

## Port and verification

`PlayerOxygenState` now exposes ordered sound callbacks at the native call
sites. The shared viewer plays `Breathe` as a Player-owned 2D voice before each
drain, stops that owned voice on `oxyoff`, and plays `OxyRelease` only on full
recovery from a low-resource state. The bundle exporter adds both names from
the source `sound.rrf` to each rescue/stage sound package.

The copied retail WAV hashes are `Breathe`:
`d7156d8d4e69ed6d83981143245145f90759980526723c47f31e6812b7b08ed2`, and
`OxyRelease`:
`c69a6d17befa2dc65225cf8380321ebb7f17ed14ffac19107583d2aa3f852d71`. The
rebuilt rescue-all app and CMake executable match at SHA-256
`33a65577afce9a3a144507aad542067d6357da69df6f2bc98509a24887b95ac8`.

A temporary strict-warning C++ check verified the `700/701 ms` start boundary,
that Breathe precedes a due drain and depletion passout, and that OxyRelease
fires only when a previously low resource reaches full. All 13 packaged sound
manifests contain both 2D resources with matching source hashes. The packaged
mission catalog and `--rescue-check` passed for all 13 rescue/stage packages;
`git diff --check` passed.

The port uses the sound resource names and event edges directly. Its SDL mixer
still approximates the retail sound manager, and no audible retail-versus-port
capture was made. The separate `00455AB0` effect on Player `+0x2160` is now
mapped to the resource-702 visor transition and its `VisorUpDown`/`VisorLock`
events; its blend and global sound-manager mix behavior remain open. See
[`player_oxygen_visor_effect_port_2026-10-15.md`](player_oxygen_visor_effect_port_2026-10-15.md).

## Evidence files

- Fresh headless Ghidra query: ignored local log `.local/oxygen-command-query.log`
  for calls at `0044BD80`, `0044BDB0`, and `00451560`; the earlier saved
  decompilations are in `reference/previous-work/decomp/evidence/startup-factories/`.
- Sound definitions: `inputs/original/Data/ResDefs/sound.rrf` lines 24-25.
- Port: `port/spikes/training-native-prototype/src/player_oxygen.h`,
  `port/spikes/training-native-prototype/src/scene_viewer.cpp`, and
  `tools/export_evs_bundle.py`.
