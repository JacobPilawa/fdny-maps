# Player oxygen resource and authored command path

Updated 2026-10-14. This is a static code/data trace; no retail runtime capture
establishes callback cadence or visual parity. The copied
`inputs/original/binaries/FDNYFirefighter.exe` SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra was not running during this checkpoint. Saved decompilations were
reviewed, and the Player command, update, reset, and numeric-constant evidence
was checked against raw bytes/disassembly from that copied executable.

## Retail behavior

- `HandlePlayerScriptActorCommand` (`00451870`) recognizes `oxyon` and
  `oxyoff`. The authored commands occur on BoundsTrigger actors throughout all
  seven campaign rescue maps and linked Boat/Oil Rig stages; the copied map
  inventory records 12 BoundsTrigger entities in the campaign maps, with
  additional authored bindings in linked stages.
- `oxyon` calls `0044BD80`. Raw instructions set Player `+0x3458` to the global
  clock plus 700 ms, then call `00455AB0(Player+0x2160, true)`.
- `oxyoff` calls `0044BDB0`. Raw instructions set Player `+0x3458` to
  `0x7fffffff`, call `00455AB0(Player+0x2160, false)`, then call
  `004A1080(Player+0x3C4)`. The subordinate object's visual/audio meaning is
  not assigned here.
- Player initialization/reset helper `0044CD10` stores `1.0` at `+0x3450`
  and `0x7fffffff` at `+0x3458`, establishing a full resource with timed drain
  disabled at that point.
- The ordinary Player update path calls `00451560` outside Player state 6.
  If `+0x3458` has expired and `+0x3454` is due, the update subtracts `0.05`
  from `+0x3450`, schedules the next decrement for clock + 3000 ms, and calls
  `00450FB0(player, 0)` only when the new value is strictly below zero (then
  clamps it to zero). If the resource is below `0.8`, Player byte `+0x52c` is
  set. When the drain deadline is disabled, the update adds frame delta times
  `0.1`, clamps at `1.0`, and clears the low-resource byte after recovery.
- Player status updater `00458400` reads the same `+0x3450` value for the
  oxygen display and dispatches `RC_OxygenLow` once below `0.25`; its warning
  gate is reset after the resource recovers. This confirms an oxygen-related
  resource from the retail string, rather than generic zone damage.

The numeric constants were read from the pinned PE image: `004DFA10 = 0.05`
(binary64), `004DF7B8 = 0.1` (binary64), `004E1B20 = 0.8` (binary64),
`004DFC24 = 1.0` (binary32), and `004DC50C = 0.0` (binary32). The Player
update uses the game's clock and frame-delta globals; the port has not mapped
their outer scheduling/cadence to retail frames.

## Port status and remaining work

The shared viewer now has a source-mapped `PlayerOxygenState` in
`port/spikes/training-native-prototype/src/player_oxygen.h`. The Player script
host handles authored `oxyon`/`oxyoff`; its normal non-state-6 update applies
the 700 ms deadline, 0.05 drain, 3000 ms interval, frame-delta refill, 0.8 low
resource flag, and 1.0 clamp. The status warning gate clears at 0.5 and
records/logs the one-shot `RC_OxygenLow` edge below 0.25. This reuses the
previously pinned `004DE6F8 = 0.5` constant mapping; it is not fresh Ghidra
analysis.

Strictly negative depletion clamps to zero and calls the shared
`applyPlayerPassout` component with argument 0; fire continues to use argument
1. The passout adapter preserves the source-selected motion subobject offset,
chatter call, clock/state write, selected-item release order, and viewer
incapacitation state, but the Player ACT/view animation is not rendered. This
uses SDL ticks and the viewer's capped frame delta, whose update cadence and
ordering against retail callbacks remain provisional. The `oxyon` effect
`00455AB0`, `oxyoff` effect `004A1080`, oxygen gauge/status rendering, Player
state-6 recovery, and the delayed carried-victim drop/HeroDown tail remain
unimplemented. No retail runtime comparison was performed.

The next shared work is to trace the `00458400` oxygen status widget's
construction and render path before implementing a gauge. The current port
logs the source-mapped `RC_OxygenLow` edge but does not draw the resource or
warning. Keep the widget's exact appearance and subordinate `00455AB0` /
`004A1080` effects provisional until their consumers are traced and compared
in-game. The delayed carried-victim drop also remains unwired: the state-6
completion needs Player position `+0x3400`, passout host callbacks, and cleanup
and status services that the viewer does not currently map. Fog remains a
separate authored renderer path; oxygen and fog bindings do not prove they
share a trigger mechanic.

Evidence references:

- [`00451870_player_script_command_handler.c`](00451870_player_script_command_handler.c)
- [`0044d550_player_rescue_entity_init_fresh.c`](0044d550_player_rescue_entity_init_fresh.c)
- [`00453d50_player_rescue_object_constructor_fresh.c`](00453d50_player_rescue_object_constructor_fresh.c)
- Saved exports: `reference/previous-work/decomp/evidence/startup-factories/0044bd80_FUN_0044bd80.c`, `00451560_FUN_00451560.c`, and `00458400_FUN_00458400.c`.
- Raw-code checks: `0044BD80..0044BDB0`, `0044CD10`, and `00451560` in the copied, hash-pinned EXE.

## Port build checkpoint (2026-10-14)

The shared scene viewer compiled after this integration with the existing CMake
build tree (`fdny-scene-viewer`). The linker repeated the existing warning that
Homebrew json-c targets macOS 27.0 while this project targets macOS 13.3. The
final CMake executable SHA-256 is
`445ff8314347dc222774740d585705b3f2d1d9c4a8eb3fa3334119fde4204be4`.
`sh tools/build_scene_viewer_app.sh rescue-all` then rebuilt the packaged app;
its executable SHA-256 is
`5bbb5190739afcd26c219e961bd75bd128d463fed91d445d9a68fca083d76dff`. The
packaged `--mission-catalog-check` loaded all eight rescue entries, and the
existing `--rescue-check` passed on the eight primary rescue packages. Those
diagnostics validate catalog loading and the synthetic rescue transaction;
they do not exercise oxygen drain, warning behavior, HUD rendering, or
interactive retail parity.
