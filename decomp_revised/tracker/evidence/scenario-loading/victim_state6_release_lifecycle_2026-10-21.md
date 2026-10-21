# VictimRescue state-6 release lifecycle

Evidence is a fresh, read-only Ghidra 12.0.4 export from the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project, opened with `-readOnly
-noanalysis`. The input `FDNYFirefighter.exe` SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The
source resource hashes are `victims.srf`
`c8138b5c2a5a9307fd322f15bcc5e88762690819e539a132363903f5452e6924` and
`sound.rrf` `433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`.
decompiler output is preserved in
[`victim_release_lifecycle_and_motion_context_fresh_2026-10-21.c`](victim_release_lifecycle_and_motion_context_fresh_2026-10-21.c).
No retail runtime capture was made.

## Regular Player release

`0045C000` writes victim state 6, enables Actor flag `0x400`, dispatches the
`rescue` event, then initializes `+0x1F8` to 150, `+0x1F4` to the current
global clock plus 2,000 ms, `+0x1F0` to the clock plus 2,150 ms, and `+0x1FC`
to 1. It resets the actor animation context after those writes. The state
dispatcher `0045BEA0` reaches `004599B0` for state 6.

`004599B0` returns while `+0x1F0` is `UINT32_MAX`. Otherwise it waits until
both `+0x1F4` and `+0x1F0` are strictly less than the global clock. On the
first due update it passes the non-null sound instance at `+0x154` to
`0048D8E0`, then writes zero to `+0x1F4`. The wrapper calls
`RequestSoundInstancePlayback(instance, null)`. `004A11A0` only short-circuits
when its optional argument is exactly 1; null is forwarded to the sound
manager. `004A0BE0` resolves the instance's configured resource and starts the
sample, updates its spatial attributes, and unpauses it, or queues the request
while the manager is busy. The `VictimRescue` sound controller is constructed
from the type record's `Rescue` key, so this is a one-shot per-type rescue
voice request rather than a stop. Each due Think call subtracts exactly 5 from
`+0x1F8`. A nonzero remainder sets `+0x1F0` to current clock plus that
remainder and toggles `+0x1FC`; zero clears `+0x1FC` and sets `+0x1F0` to
`UINT32_MAX`. `00481140` maps the byte to Actor flags `0x400` or zero. The
dispatcher then reaches the shared actor-animation update `00480D50`.

The shared host now advances these fields in
[`victim_release.h`](../../../port/spikes/training-native-prototype/src/victim_release.h)
from the VictimRescue Think phase in
[`scene_viewer.cpp`](../../../port/spikes/training-native-prototype/src/scene_viewer.cpp).
It applies each due flag update to the preview actor, so the released victim
eventually leaves the normal render path. Host timing remains provisional:
the loop uses `SDL_GetTicks` and one modeled Think pass per viewer iteration,
while the retail callback interval and within-manager order are unknown. The
first source deadline is 2,150 ms after release; subsequent decrements happen
once per due Think pass, so `5` is a counter decrement, not a measured 5 ms
frame cadence. No retail parity claim is made.

## RescueBot state 6 and audio boundary

`0045BF70` is a separate state-6 entry. It writes `UINT32_MAX` to `+0x1F0`,
resets the animation context, and clears Actor flag `0x400`; it does not
initialize the regular release timer or active byte. The host now mirrors the
sentinel in the actor record when RescueBot starts carrying a victim, so this
path does not enter the regular release pulse.

The host now requests the same authored Rescue sample on that first due update,
using the victim entity as the owner and the released victim position as its 3D
source. The per-type keys are mapped from `Data/ResDefs/victims.srf`; their
3D sample definitions come from `Data/ResDefs/sound.rrf`, and the bundle
exporter includes the voices in each scene's shared sample bank. The host does
not reproduce the retail manager's deferred queue or controller object, and
rendered-frame cadence and audible parity remain unverified. Separately, the
non-state-6 `0048D8F0` Beckon update is still not connected to host playback.
The sound request wrapper and manager path are preserved in
[`mission_result_sound_requests_fresh_2026-10-20.c`](mission_result_sound_requests_fresh_2026-10-20.c)
and [`004a0be0_dispatch_or_queue_sound_fresh.c`](004a0be0_dispatch_or_queue_sound_fresh.c).

The rescue-all macOS app compiled and packaged after this port. The standalone
and app-bundle arm64 executables match at SHA-256
`ec2629d9c08e754eeee0f431b39bf3aa584233522575a85dc8d28926314dab0f`. All 17
scene sound manifests include the six configured Rescue resources; eight
rescue choices and `Info.plist` validation passed. The linker warns that
json-c targets macOS 27.0 while this app targets macOS 13.3. Compile/package
evidence only; no gameplay tests, interactive runtime, audio capture, or
retail parity check was run.
