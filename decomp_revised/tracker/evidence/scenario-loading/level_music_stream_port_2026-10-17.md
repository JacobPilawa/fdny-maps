# Catalog-selected level music port (2026-10-17)

The shared macOS build now loads and loops the MP3 authored for the selected
retail level, applies the oxygen visor's music-channel gain changes, and carries
the selected track through linked BSP stages that do not have their own
`thegame.lev` row. This is a static-source-backed implementation checkpoint,
not a captured audio or timing parity result.

## Retail evidence

The executable is `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The
catalog is `Data/ResDefs/thegame.lev`, SHA-256
`ee6819873cd540e67613774d848506561e2861442b37ef301e27273805d558c6`.

- `0042AEA0` checks the level state at `+0x6F0`; in state 3 it dispatches
  selector 2 with the active level record's `+0x0C` string.
- Selector 2 in `0042A730` replaces the previous stream resource, opens the
  MP3 through `004A0210`, and starts it with looping enabled through
  `004A00F0`.
- `0042A260` copies the active record's `+0x0C` into the pending level block
  at `+0x66C`. `0042AB00` later promotes that block at `+0x660` to the active
  record at `+0x688`. Combined with `0042AEA0`'s selector-2 call, this shows
  why a linked rescue stage with no independent catalog row retains its
  selected mission track.
- Oxygen selector 4 sets category 2 to `0.3` and sends that gain to the
  separate stream channel at manager `+0x690`; selector 5 restores categories
  1 and 2 and the stream to full volume. `0049F2A0` multiplies the gain by
  `255.0`, truncates to an integer, and applies it with `_FSOUND_SetVolume`,
  yielding volume `76/255` while oxygen is on.

The source-backed level assignments are Training `ebtwist.mp3`, MBase
`ebtrauma.mp3`, Bar R `ebtwist.mp3`, Gas R `ebtrauma.mp3`, Bank `ebtwist.mp3`,
Boat A `ebtwist.mp3`, Subway A `ebtrauma.mp3`, and Oil Rig A `ebfun.mp3`.
Boat B, Subway B, and Oil Rig B/C/D are linked stages with no separate
catalog row; the port passes the starting mission's selected music resource
into those stages, matching the copied `+0x0C` field behavior.

The source MP3 hashes are in `inputs/original/manifest.csv`: `ebharmon.mp3`
`673ee806f868259ed4eafd6d0b28ed2fb308053cfdc4208941a460f1cd809e64`,
`ebfun.mp3` `24d181323e56b5d396a9965d9da499c24782c269a30939f7e4957f25278a866c`,
`ebtrauma.mp3` `8eb61c1f23de87d5e820cc2bf493e9f13bd845e02bec1b05207cd444074268af`,
and `ebtwist.mp3`
`ef182c515de598c8303ce5a61f726473cf8ba3dba328715403cb7da9d3de454f`.

## Port and packaging

`build_rescue_mission_catalog.py` now carries each `music:` value into the
mission manifest. `export_level_music.py` copies the selected MP3 next to each
catalogued scene and writes `music.json` with the catalog and asset hashes.
`SceneData` reads that manifest. `SceneAudio` decodes signed-16 stereo PCM with
libmpg123, converts it to the SDL device format, loops it in the existing
callback, and applies the source-derived `76/255` and full-volume selector
values. `runRescueSceneChain` passes the initial level's selection to linked
packages without a catalog row.

The `rescue-all` app build succeeded. The packaged executable SHA-256 is
`45ca9cefb95afb4f6c690f87a6931f663318cfa81c3521a3fcd59b1ac4b1d824`. On this
macOS host, CMake found the static libmpg123 1.33.7 archive; `otool -L` shows
no mpg123 runtime dependency. Each of the eight catalogued packages has the
expected track and matching source/bundled hashes.

## Limits and next steps

- The port pre-decodes the full MP3 into PCM before playback. It does not yet
  reproduce FMOD's stream buffering, decoder behavior, seek/timing, or exact
  gain interpolation.
- No gameplay or audible-output test was run. Compilation and package/hash
  checks confirm wiring and assets only.
- The retail menu track is `ebharmon.mp3`, but startup/menu music remains
  paused with the menu work. No menu playback is claimed.
- The local bundle still links to Homebrew SDL2 and json-c dylibs; the build
  output also warns that this json-c binary targets a newer macOS than the
  app's declared deployment target. Those existing packaging gates remain.

## Evidence files

- [`0042aea0_evergame_pending_load_update_fresh.c`](0042aea0_evergame_pending_load_update_fresh.c)
- [`0042a260_set_pending_level_request_fresh.c`](0042a260_set_pending_level_request_fresh.c)
- [`0042a730_oxygen_audio_mix_selectors_fresh.c`](0042a730_oxygen_audio_mix_selectors_fresh.c)
- [`0042ab00_process_level_load_request_fresh.c`](0042ab00_process_level_load_request_fresh.c)
- [`004a0210_initialize_fmod_sound_resource_fresh.c`](004a0210_initialize_fmod_sound_resource_fresh.c)
- [`004a00f0_start_fmod_sample_or_stream_fresh.c`](004a00f0_start_fmod_sample_or_stream_fresh.c)
