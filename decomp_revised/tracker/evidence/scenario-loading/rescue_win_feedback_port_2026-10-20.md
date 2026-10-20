# Rescue standalone win audio port — 2026-10-20

## Retail evidence

Pinned `inputs/original/binaries/FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The hash-pinned `00458400` trace shows the visible PlayerRescue status updater
selecting widget `0x47`, dispatching `RC_ResLvlWin` with its ordered one-entry
set, and requesting the status-owned `RescueWin` instance when Player
`+0x3438` is 1. The source constructor configures `RescueWin` with raw category
1. See [`mission_outcome_status_dispatch_2026-10-19.md`](mission_outcome_status_dispatch_2026-10-19.md)
and [`mission_result_sound_requests_fresh_2026-10-20.c`](mission_result_sound_requests_fresh_2026-10-20.c).

Only the accepted standalone `finalexit` command is used as the port's win
event: `00450A00` passes its remaining-victim gate, sets state 1, and invokes
the mapped completion helper. `UpdateFinalExitAvailability` also sets state 1
in a no-exit-zone branch, so the port does not dispatch win audio merely from
the numeric state. `finalexitloadmap` remains a stage continuation.

The retail `sound.rrf` SHA-256 is
`433596cb437d05512ef84cc67f5a380db57703fd03f0871d3c9eeb1657c07333`.
It maps `RC_ResLvlWin0` to `Data/Snd/chatter/win2.wav` (SHA-256
`e47d1504ea55a00af05c39eeee2e7c44472fd254db0314bf76b91e4bc97b872d`) and
`RescueWin` to `Data/Snd/win2.wav` (SHA-256
`c922ef8fac5f591d9002bf0f322316a679723fd24769009394eed34f00c56178`).
They are distinct files despite sharing a basename.

## Shared host

An accepted standalone `finalexit` now requests the one-entry chatter and the
Player-owned `RescueWin` sound on channel group 1, then suppresses repeat
requests for the rest of that host scene. Both resources are included by
`tools/export_evs_bundle.py` in every scene package because `SceneAudio` keeps
the first loaded sample bank across linked stages. The host request occurs
during the EVS command invocation; it is a timing adapter for the retail
status callback, whose relation to the command/contact pass remains
unverified.

## Limits

This ports audio requests, not audible runtime playback or retail parity. The
win result card's timing fields, placement, animation lifetime, manager
handoff, profile save, and Records return are still open. No duration is
guessed. The no-exit-zone state-1 branch and linked-stage exits do not trigger
this standalone win cue.

## macOS build checkpoint

The rescue-all app rebuilt successfully. Standalone and packaged arm64
executables match at SHA-256
`45b431c679d931ccd846530315ca5c7cd95ff2a711a8f1e709a16f9617dc2f73`, and the
app `Info.plist` passes `plutil -lint`. A direct package sweep found both win
resources in `sounds.json` and as WAV files in all 17 bundled scene packages;
the catalog exporter emitted eight rescue choices. The linker still warns
that Homebrew json-c targets macOS 27.0 while this build targets 13.3. No tests,
interactive gameplay, audible playback, or retail parity check were run.
