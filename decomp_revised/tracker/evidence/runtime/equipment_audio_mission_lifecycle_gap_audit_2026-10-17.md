# Equipment, audio, and mission-lifecycle gap audit — 2026-10-17

Three read-only audits compared the shared host with the latest checked-in
retail evidence. No source files were changed by those audits. Static retail
references use `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Held equipment

The shared host selects Axe/extinguisher/goggles from authored
PlayerRescue inventory and routes the mapped Axe strike and extinguisher
contact/suppression paths. First Axe equip now uses the retail model-1 `axe.act`
child; model 6 remains behind the duplicate-equip gate. The reported swing
direction remains open because the camera-world-to-Player-follower-to-ActorInfo
matrix join and the port's guessed root placement have not been numerically
matched. Axe sound resources are registered, but playback triggers are
unresolved.

The host also draws provisional smoke quads for accepted extinguisher
particle requests, using the resource-14 texture candidates exported as
SDL-readable 24-bit BMPs. Static review found no deterministic action, queue,
or suppression-delivery drop. Retail and port both double contact influence
before `1-(1-influence)^2`, which yields almost no suppression near a full
center contact; this counterintuitive response may explain an inert-feeling
spray, but a live contact trace is still required to verify the reported case.
The current contact frame remains provisional and may miss the fire. Do not
claim the reported behavior fixed. Details and audit instructions are in
[`equipment_action_pose_followup_2026-10-17.md`](../scenario-loading/equipment_action_pose_followup_2026-10-17.md).

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

The catalog binary SHA-256 is
`ee6819873cd540e67613774d848506561e2861442b37ef301e27273805d558c6`. Its menu
track is `ebharmon.mp3`; per-level values are: Training `ebtwist.mp3`, MBase
`ebtrauma.mp3`, Bar R `ebtwist.mp3`, Gas R `ebtrauma.mp3`, Bank
`ebtwist.mp3`, Boat A `ebtwist.mp3`, Subway A `ebtrauma.mp3`, and Oil Rig A
`ebfun.mp3`. The other catalog entries are dock_h `ebfun`, const_h `ebfun`,
city_h `ebfun`, and oil_rig_h `ebtrauma`.

The copied retail assets and hashes are tracked in
[`inputs/original/manifest.csv`](../../../inputs/original/manifest.csv):
`ebharmon.mp3` (`673ee806f868259ed4eafd6d0b28ed2fb308053cfdc4208941a460f1cd809e64`),
`ebfun.mp3` (`24d181323e56b5d396a9965d9da499c24782c269a30939f7e4957f25278a866c`),
`ebtrauma.mp3` (`8eb61c1f23de87d5e820cc2bf493e9f13bd845e02bec1b05207cd444074268af`),
and `ebtwist.mp3` (`ef182c515de598c8303ce5a61f726473cf8ba3dba328715403cb7da9d3de454f`).
The shared `SceneAudio` host now decodes and loops each selected rescue
mission's catalog MP3, applies the separate stream-channel gain at 76/255 while
oxygen is on, and restores full gain when oxygen is off. Retail traces establish
the level-entry selector-2 call and loop flag. The pending-level setter copies
the active record's `+0x0C` music string into linked stage requests, and the
port passes that selected track to Boat B, Subway B, and Oil Rig B/C/D packages
that have no independent catalog row. The implementation uses predecoded PCM,
not FMOD streaming; audible playback has not been checked. Menu music remains
unimplemented while the user has paused menu work.

Implementation, source hashes, and build limits are in
[`level_music_stream_port_2026-10-17.md`](../scenario-loading/level_music_stream_port_2026-10-17.md).

Evidence: [`lev_rrf_catalog_loader_trace_fresh.md`](../scenario-loading/lev_rrf_catalog_loader_trace_fresh.md),
[`004a0210_initialize_fmod_sound_resource_fresh.c`](../scenario-loading/004a0210_initialize_fmod_sound_resource_fresh.c),
[`004a00f0_start_fmod_sample_or_stream_fresh.c`](../scenario-loading/004a00f0_start_fmod_sample_or_stream_fresh.c),
and [the shared audio host](../../../port/spikes/training-native-prototype/src/scene_audio.h).

## Mission lifecycle

Retail `thegame.lev` includes per-level `time:` values. The parser stores the
parsed float times 60 at record `+0x10`; Player initializer `0044D550` copies
that value into Player `+0x343C`, applies a mode multiplier, and three callers
use it through deadline helper `004541A0` (Player escape plus two RescueBot
routines). This is not a confirmed mission countdown, and no objective HUD,
expiry handler, or result flow is established. The shared port now passes the
selected catalog value through linked rescue stages. Shared victim release and
authored final-exit/stage commands are integrated, but controlled
`--rescue-check` transactions do not prove natural mission completion. The
no-final-exit call at `0044C360` remains unverified as an outro. See
[`catalog_time_rescue_deadline_port_2026-10-19.md`](catalog_time_rescue_deadline_port_2026-10-19.md).

Evidence: [`MBASE_RESCUE_SLICE.md`](../../../docs/MBASE_RESCUE_SLICE.md),
[`rescue_completion_and_exit_path.md`](../scenario-loading/rescue_completion_and_exit_path.md),
and [`level_to_evs_static_trace.md`](../scenario-loading/level_to_evs_static_trace.md).

## Next implementation boundaries

1. Capture one extinguisher action/contact frame to determine whether points
   intersect a fire, message 30000 is delivered, and suppression strength changes.
   Continue tracing the camera-to-ActorInfo matrix before changing Axe axes.
2. Trace menu-track startup and exact audio output behavior when menu work
   resumes; the level track and oxygen stream gain are now wired.
3. Trace downstream consumers of Player `+0x3420` and RescueBot scheduling,
   then trace objective/result events and level-end behavior before adding a
   countdown HUD or success/failure presentation.
