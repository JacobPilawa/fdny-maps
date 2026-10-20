# Rescue timeout loss audio port — 2026-10-20

## Retail evidence

Retail `FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The saved, hash-pinned outcome trace maps countdown expiry in `00452570` to
Player state `+0x3438 = 2`. When PlayerStatus has no active result child,
`00458400` selects widget 0x48, advances chatter key `RC_ResLvlLose` and
requests owner sound `RescueLose`.

A read-only Ghidra 12.0.4 headless follow-up opened a disposable copy of the
saved FDNYPort project and decompiled `004A12F0`/`004A11A0`; the retained
analysis project was not changed. `004A12F0` writes its third and fourth
arguments to sound-instance `+0x10` and `+0x14`. PlayerStatus constructor
`004576B0` configures `RescueLose` with arguments `(0, 1)`, so the shared audio
adapter uses category 1. The status callback calls `004A11A0` with a null
optional suppression flag; that bypasses its already-playing check and
forwards the request. The full selected decomp excerpts are in
[`mission_result_sound_requests_fresh_2026-10-20.c`](mission_result_sound_requests_fresh_2026-10-20.c).

The one-entry chatter mapping and distinct direct sound are in the retail
`sound.rrf`: `RC_ResLvlLose0` maps to
`Data/Snd/chatter/lose4_b.wav`, while `RescueLose` maps to
`Data/Snd/lose2.wav`. Both names are registered in
[`mission_outcome_status_dispatch_2026-10-19.md`](mission_outcome_status_dispatch_2026-10-19.md).

## Shared port

The rescue host watches the existing `RescueExitState.messageGate`. On the
first host update where it is state 2, it requests `RC_ResLvlLose0` as a
Player chatter voice and owner-aware `RescueLose` on category 1, then records
that the timeout audio was dispatched. This covers countdown-driven loss
across the shared PlayerRescue host and does not treat state 1 as a win.
`tools/export_evs_bundle.py` includes both sound resources in every scene
package because `SceneAudio` retains the sample bank loaded from the first
scene.

## Limits

This is source-shaped audio dispatch, not retail runtime parity. The host
emits both requests on the same paced update that exposes state 2; the retail
status callback's relative ordering/cadence against `00452570` remains
unresolved. No audible capture was run. The 0x48 result widget's lifetime,
screen placement, alpha, and completion behavior are unresolved, as is the
frontend return. The no-exit path also writes state 1, so the port does not
connect that numeric gate to a win presentation or profile merge.

## Build

The macOS rescue-all app compiled with matching standalone and packaged
executable SHA-256
`323cb101463aebd5614c041ab90e0740bbc4e1c31c236b8c5c76c3205ac0026d`.
`Info.plist` validation passed, and a manifest check found both loss resources
in all 17 retail rescue scenes. The existing json-c macOS 27.0 versus 13.3
deployment-target warning remains. This verifies compilation and bundled
resources only; it does not verify timeout playback, a natural mission loss,
result presentation, or retail parity.
