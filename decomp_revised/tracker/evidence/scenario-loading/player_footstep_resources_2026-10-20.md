# Player footstep resource packaging (2026-10-20)

## Source evidence

The retail executable is pinned at SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Player initialization `0044D550` appends four sound names, `FootStep1` through
`FootStep4`, to the Player sound list at root `+0x540` (count `+0x544`), as
shown in
[`0044d550_player_rescue_entity_init_fresh.c`](0044d550_player_rescue_entity_init_fresh.c).
The movement callsite invokes `0044EA90` at `0045331A`; that helper selects
from this list and requests playback after its timer and selector gates.
Selection and `Total Steps` increment are coupled, so these files are needed
by the eventual shared producer.

The retail sound definition file is
`inputs/original/Data/ResDefs/sound.rrf`, SHA-256
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`.
Lines 26–29 define the resources as 2D buffers:

| Resource | Retail WAV | Source SHA-256 |
| --- | --- | --- |
| `FootStep1` | `Data/Snd/footgen1.wav` | `472be55fe30cd6b5788e2a1254c774ac8b3cd05e8e7023ec82940b6837698b9c` |
| `FootStep2` | `Data/Snd/footgen2.wav` | `1fa81fc99eb577d78f3bea101562c55097c7025eef275480e711b053722609fc` |
| `FootStep3` | `Data/Snd/footgen3.wav` | `af0c23ca8a9c8682d363b1f2ab2fd423ea7f93037364c6e5770dea0d53879ce6` |
| `FootStep4` | `Data/Snd/footgen4.wav` | `fcd2b38a762fa577fbef3eb5058c4b354ebc7c90a060ecd747f808b9a9107569` |

## Shared-port change

`tools/export_evs_bundle.py` now includes all four names in every scene audio
bundle. This is required because the shared `SceneAudio` instance loads its
sample map from the first scene package and leaves it immutable for the
session. The macOS rescue-all app builder re-runs this exporter for its 17
scene packages.

The shared rescue loop now reads the preceding head-motion sample gate,
applies the 100 ms selector timer and shared-RNG retry against the host's
current `+0x558` adapter value, increments `Total Steps`, and requests the
selected `FootStep1`–`FootStep4` sample. The selector and writer are described
in [`player_total_steps_and_headmotion_2026-10-20.asm`](player_total_steps_and_headmotion_2026-10-20.asm).

This is a source-shaped integration, not retail parity. The first `+0x198`
byte value remains unknown; the host deterministically starts it false. The
host treats `+0x558` as zero because initialization writes zero and no later
static direct writer was found; computed writers are not excluded. The same
host RNG is used, but other source random consumers and callback interleaving
are incomplete, and the Player cadence is provisional. The output of
`HeadMotion` remains excluded from the rendered camera under the earlier
camera-bob removal direction. The host passes SceneAudio group 1 by analogy
with other Player one-shots; a source group assignment for these footstep
instances has not been established. It also uses the raw digital forward/side
input magnitude as the source movement-vector threshold adapter; full vector
construction remains part of the incomplete Player movement port.

