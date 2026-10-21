# VictimRescue state-6 release lifecycle

Evidence is a fresh, read-only Ghidra 12.0.4 export from the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project, opened with `-readOnly
-noanalysis`. The input `FDNYFirefighter.exe` SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The
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
first due update it requests stop for the non-null sound instance at `+0x154`
through `0048D8E0`, then writes zero to `+0x1F4`. The wrapper requests sound
playback with a null command. Each due Think call subtracts exactly 5 from
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

The host does not retain the VictimRescue sound instance at `+0x154`, so the
one-time source stop request is still missing. The timer, visibility flags,
and final hide have been ported; sound-instance ownership and audible parity
remain open.
