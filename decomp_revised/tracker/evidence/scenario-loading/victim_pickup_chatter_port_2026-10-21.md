# Victim pickup and release chatter port (2026-10-21)

## Retail request path

Static evidence is pinned to `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Player pickup handler `00450440` calls status action 1 in `004571A0`. That
action enables and samples the resource-55 pickup sprite, then requests
`RC_VictPick` from the Player chatter manager at `Player+0x3540` through
`004897C0` with flag 1. This chatter request is separate from the HUD sprite.

The Player chatter initializer `004895C0` registers `RC_VictPick` and
`RC_VictDrop`. `00489200` builds the per-key set and initializes its cursor at
zero. For flag 1, `00489110` increments the cursor before selection, wraps at
the set length, and submits the selected sound instance through `004A11A0`;
this path makes no shared-RNG call. Therefore, a two-entry set requests index 1
first, then index 0, and repeats in that order. The configured entry order is
inferred from the `sound.rrf` record order; this export does not independently
prove the resource parser's enumeration order.

`inputs/original/Data/ResDefs/sound.rrf`, SHA-256
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`, maps:

| Chatter entry | WAV | WAV SHA-256 |
| --- | --- | --- |
| `RC_VictPick0` | `Data/Snd/chatter/survivor1.wav` | `ef9f375804b7c89b844bde8497e6f14ba92c2a1e81f855ff72e7820184907b2e` |
| `RC_VictPick1` | `Data/Snd/chatter/survivor2.wav` | `4f1d4704714f132004e40f083b88c861b02c44ec8fdbc2f61a61919a2dc4a99e` |
| `RC_VictDrop0` | `Data/Snd/chatter/rescue1.wav` | `9f928ae630d212cc1d1b973a4002000d51d8f3201a4f1efe01bf83fda33998a7` |

The successful release path calls status action 2 after incrementing the
rescue counter. Action 2 hides the pickup sprite and requests `RC_VictDrop`
only when `total - field3530 - rescues` is signed-positive; when total is
uninitialized it counts type-2 entities, stores the total, and refreshes zone
membership first. The drop set has one entry, so its selected WAV is
unambiguous. This evidence establishes a sound-manager request, not audible
playback or a retail mixer category.

## Shared port

The shared scene host now requests `RC_VictPick1`, `RC_VictPick0`, then repeats
that sequence on status action 1. It requests `RC_VictDrop0` after action 2
only when the same remaining-survivor arithmetic is positive, including the
lazy type-2 count and zone refresh. `tools/export_evs_bundle.py` includes all
three resources in every scene package because `SceneAudio` loads the shared
sample bank from the first scene opened. The host routes these 2D requests
through its existing local Player chatter group 1; that category is an adapter
choice, not a verified retail category.

The resource-55 HUD animation remains its separate visible pickup cue. The
status message text and exact status-widget teardown are not added by this
audio slice. The rescue-all macOS app compiled and packaged with standalone
and app-bundle executable SHA-256
`e27c580b1e619f5f1e3a334afd83e7fd5186331f5fcca72c4e08f881ec996adf`. Its
18 packaged sound manifests (17 scene packages plus the enclosing resource
manifest) all contain the three chatter rows. The eight rescue choices and
`Info.plist` validation also completed; the linker warns that json-c targets
macOS 27.0 while this build targets 13.3. No test, audio capture, interactive
rescue, or retail-parity check was performed. Natural pickup cadence and
mission-level progression remain provisional.

## Evidence files

- Current-hash registration, set construction, and playback exports:
  [`hose_stage_message_registration_fresh_2026-10-20.c`](hose_stage_message_registration_fresh_2026-10-20.c)
- Current-hash pickup and release status dispatch:
  [`mission_outcome_state_fresh_2026-10-19.c`](mission_outcome_state_fresh_2026-10-19.c)
- Source sound definitions and original WAV hashes: `sound.rrf` and
  `inputs/original/manifest.csv`.
- Shared pickup transaction:
  [`victim_pickup_message_path.md`](victim_pickup_message_path.md).
