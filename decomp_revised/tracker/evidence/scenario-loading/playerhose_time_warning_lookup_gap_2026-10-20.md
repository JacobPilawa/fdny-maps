# PlayerHose time-warning lookup gap

## Finding

The pinned retail executable detects the PlayerHose countdown boundary at six
to five seconds and attempts to dispatch the key `RC_HoseTimeWarn`. The
constructor-populated chatter manager used by this caller does not register
that key. The exact lookup returns null, and the dispatcher enters playback
only when it receives a non-null set. The examined path therefore does not
request either globally configured `RC_HoseTimeWarn` sound. This is a
source-backed static conclusion; no live level or audible check was made.

## Retail source path

The evidence is pinned to `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

- `00466D60` calls `004895C0` with `param_1 + 0x82`, the PlayerHose chatter
  manager at byte offset `+0x208`.
- `004895C0` registers exactly these 13 keys for that manager:
  `RC_OxygenLow`, `RC_VictPick`, `RC_VictDrop`, `RC_PassOut`,
  `RC_ResLvlStart`, `RC_ResLvlWin`, `RC_ResLvlLose`, `RC_TimeWarn`,
  `RC_HoseLvlStart`, `RC_HoseLvlLose`, `RC_HoseLvlWin`, `RC_Pain`, and
  `RC_ResTimeWarn`. `RC_HoseTimeWarn` is absent.
- `004653B0` calls the shared boundary helper with threshold 5. When it reports
  the transition from floor 6 to floor 5, the caller constructs the exact key
  `RC_HoseTimeWarn` and calls `004897C0` with mode byte 1.
- `004897C0` calls `00488E30` to search the supplied manager. It enters a
  playback branch only if the result is non-null. `00488E30` scans the manager
  entries for an exact key match and returns zero after an unsuccessful scan.

The floor helper and frame-delta traces establish that the source boundary is
six-to-five seconds. `Data/ResDefs/sound.rrf` nevertheless defines
`RC_HoseTimeWarn0` and `RC_HoseTimeWarn1` as `time4_b.wav` and `time5_b.wav`
(RRF SHA-256 `433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`).
Those global resource definitions do not add a key to the manager constructed
by `004895C0`.

## Port correction

Removed the host's synthetic six-to-five-second "Hotspot time warning" toast
and its one-shot flag from
[`player_hose_stage.h`](../../../port/spikes/training-native-prototype/src/player_hose_stage.h).
The host continues to decrement the hotspot timer and use its timeout loss
path. This change does not claim that the full PlayerHose call graph has no
later manager mutation; post-construction writes to `+0x208` and live playback
were not audited.

## Retained pseudocode

The relevant Ghidra exports and their SHA-256 hashes are:

- [`hose_stage_message_registration_fresh_2026-10-20.c`](hose_stage_message_registration_fresh_2026-10-20.c):
  `80aeffba087a48d676eb8356cc3857846bdeee29fba1d50efd448bc05e2e7753`
- [`hose_pressure_path_fresh_2026-10-20.c`](../fire-spread/hose_pressure_path_fresh_2026-10-20.c):
  `c61d19bfaa00d1f385a5384183c1d2f65d85351acb71f63445e2963a0354e980`
- [`hose_time_warning_helper_fresh_2026-10-20.c`](../fire-spread/hose_time_warning_helper_fresh_2026-10-20.c):
  `3a266991ef999dfddafe6108202ba2f269ae88fdddd3f37dd7de1279c91121a6`
- [`hose_time_floor_helper_fresh_2026-10-20.c`](../fire-spread/hose_time_floor_helper_fresh_2026-10-20.c):
  `e681e37206a278a2eb3716a479cea235dbc7e2480392124dd2d26309c37cb534`

## Build record

The rescue-all macOS app rebuilt after removing the synthetic toast. The
standalone and packaged executables have matching SHA-256
`710299cb6b0daf0b00fe307402c7b8d725110e0c4669eb5cc911960d7fbc1153`.
`Info.plist` validation passed. The build retains the existing warning that
Homebrew json-c was built for macOS 27.0 while this project links for 13.3.
No tests, interactive gameplay, audible check, or retail parity check were
performed.
