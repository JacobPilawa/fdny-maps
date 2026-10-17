# Equipment, audio, and mission-lifecycle gap audit — 2026-10-17

Three read-only audits compared the shared host with the latest checked-in
retail evidence. No source files were changed by those audits. Static retail
references use `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Held equipment

The shared host already selects Axe/extinguisher/goggles from authored
PlayerRescue inventory and routes the mapped Axe strike and extinguisher
contact/suppression paths. `ExtinguisherEffects::afterContact` creates an
emitter-offset request after accepted contacts and consumes the traced random
draws, but the host does not render the particle request. Exact particle asset,
placement, and audio trigger remain open; do not claim visual or sound parity
from the current action/suppression path. Axe sound resources are registered,
but playback triggers are unresolved. Held-model root/orientation/scale remain
unverified against a retail view capture.

Evidence: [`extinguisher_scene_viewer_runtime.md`](../scenario-loading/extinguisher_scene_viewer_runtime.md),
[`player_loadout_command_path.md`](../scenario-loading/player_loadout_command_path.md),
and [the shared effect component](../../../port/spikes/training-native-prototype/src/extinguisher_effects.h).

## Background music

`thegame.lev` authors a separate `menumusic:` value and per-level `music:`
values. Hash-pinned catalog trace `00423160` stores the level music string at
record `+0x0C`; the menu value is parser-owned/global. Retail helper traces map
`.mp3` resources to `FSOUND_Stream_OpenFile` and playback to
`FSOUND_Stream_PlayEx`. Oxygen selectors also adjust a separate stream channel
at sound-manager `+0x690`.

The copied retail assets and hashes are tracked in
[`inputs/original/manifest.csv`](../../../inputs/original/manifest.csv):
`ebharmon.mp3`, `ebfun.mp3`, `ebtrauma.mp3`, and `ebtwist.mp3`. The shared
`SceneAudio` host currently mixes bundled WAV voices and has no MP3 stream slot
or catalog-selected music. The retail callsites that start menu music, switch
to level music, loop/stop it, and restore it after a stage remain open. Stream
lifecycle timing must stay provisional until those callers are traced.

Evidence: [`lev_rrf_catalog_loader_trace_fresh.md`](../scenario-loading/lev_rrf_catalog_loader_trace_fresh.md),
[`004a0210_initialize_fmod_sound_resource_fresh.c`](../scenario-loading/004a0210_initialize_fmod_sound_resource_fresh.c),
[`004a00f0_start_fmod_sample_or_stream_fresh.c`](../scenario-loading/004a00f0_start_fmod_sample_or_stream_fresh.c),
and [the shared audio host](../../../port/spikes/training-native-prototype/src/scene_audio.h).

## Mission lifecycle

Retail `thegame.lev` includes per-level `time:` values. The catalog parser
stores the parsed/scaled value at record `+0x10`, and the MBase slice note
identifies its authored value as five minutes. This is not yet a traced
countdown consumer, objective HUD, expiry handler, or result flow. Shared
victim release and authored final-exit/stage commands are integrated, but
controlled `--rescue-check` transactions do not prove natural mission
completion. The no-final-exit call at `0044C360` remains unverified as an
outro.

Evidence: [`MBASE_RESCUE_SLICE.md`](../../../docs/MBASE_RESCUE_SLICE.md),
[`rescue_completion_and_exit_path.md`](../scenario-loading/rescue_completion_and_exit_path.md),
and [`level_to_evs_static_trace.md`](../scenario-loading/level_to_evs_static_trace.md).

## Next implementation boundaries

1. Trace the extinguisher emitter resource and owner draw path, then render the
   source-described effect from the existing request. Keep its audio trigger
   open until traced.
2. Trace menu/level stream callsites and choose a cross-platform MP3 decode
   path before wiring authored music fields. Do not guess start/stop/loop timing.
3. Trace the `time:` consumer, objective/result events, and level-end behavior
   before adding a countdown HUD or success/failure presentation.
