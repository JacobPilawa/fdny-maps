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

## Oxygen status widget source trace

The read-only Ghidra trace against the same hash-pinned executable now covers
the status widget constructor and update helpers. `004576B0` constructs the
meter at status object `+0x28d0` from two geometry entries. Their constructor
arguments include `(12, 0.5, 1, 1)` and trailing tuples `(1, 1, 1)` and
`(1, 0.25, 0)`; the port does not assign names to those raw renderer fields.
The widget starts hidden. Its rectangle helper receives `[15, 111, 304, 400]`
and orders those values as `(15, 304)` to `(111, 400)`, a 96-by-96 input
rectangle centered at `(63, 352)`. The retail renderer's scaling and final
on-screen placement have not been confirmed visually.

`00458400` is called in the Player status update at `0044FE46`. It moves the
displayed value at status `+0x28b0` one quarter of the remaining distance
toward Player `+0x3450`. At values `<= 0.99`, it enables the meter and updates
its second geometry layer; above `0.99`, it hides the meter. `00482430` sends
that layer through a rotation of `-270 * (0.5 - displayedValue)` degrees.
This identifies the value-driven needle geometry; its final retail appearance
still needs an in-game comparison.

A separate warning object lives at status `+0x29f0`. Its constructor adds a
second resource-12 gauge face tinted RGB `(1, 0.25, 0)` alongside the white
face. The updater enables it below displayed oxygen `0.25`, writes the
controller scalar `0.9 * (1 - 4 * (0.25 - displayed)) + 0.1` (from `1.0` at
the threshold to `0.1` at zero), and hides it at or above `0.25`. It also
emits `RC_OxygenLow` once while the displayed value is below `0.25`; the
warning dispatch latch resets at or above `0.5`. The widget registration helper
`0047ecd0` adds UI elements to a manager-owned list (`00493d60`). The shared
frame dispatcher `00494670` walks this manager list, updates active objects
through vtable slot `+0x4`, then calls their render slot `+0x10` during the
engine frame. For the polygon widget, vtable entry `004E3538` points to thunk
`0047E5A0`, which jumps to `0047DF10`. That draw helper obtains the resource
bitmap and submits a four-vertex polygon through `geEngine_RenderPoly` (flags
`0x36`, with an additional conditional pass using `0x0d`). `rescue.rrf` maps
resource 12 to `oxygauge.bmp`/`oxygauge_a.bmp` and resource 13 to
`oxyneedle.bmp`/`oxyneedle_a.bmp`; both alpha BMP palettes are grayscale.
This closes the static route from status widget to engine polygon submission.
Genesis/D3D blend details and an in-game capture remain open.

## Plain-language explanation

When a rescue turns oxygen on, the retail HUD eases its meter toward the
Player's oxygen level instead of snapping instantly. It draws a gauge face and
a needle over the same square, with the needle rotating as oxygen changes. The
retail constructor also adds an orange-tinted copy of the gauge face below
25% oxygen. The native port draws that layer with an alpha ramp derived from
oxygen level; the retail controller's blend meaning is still unknown.

## Port status and remaining work

The shared viewer now has a source-mapped `PlayerOxygenState` in
`port/spikes/training-native-prototype/src/player_oxygen.h`. The Player script
host handles authored `oxyon`/`oxyoff`; its normal non-state-6 update applies
the 700 ms deadline, 0.05 drain, 3000 ms interval, frame-delta refill, 0.8 low
resource flag, and 1.0 clamp. The status warning gate clears at 0.5 and
records/logs the one-shot `RC_OxygenLow` edge below 0.25. Player-owned
`Breathe`/`OxyRelease` audio and the `oxyoff` stop call are now ported; see
[`player_oxygen_breathe_release_sound_port_2026-10-15.md`](player_oxygen_breathe_release_sound_port_2026-10-15.md).
This reuses the previously pinned `004DE6F8 = 0.5` constant mapping; it is not
fresh Ghidra analysis.

Strictly negative depletion clamps to zero and calls the shared
`applyPlayerPassout` component with argument 0; fire continues to use argument
1. The passout adapter preserves the source-selected sound-subobject offset,
chatter call, clock/state write, selected-item release order, and viewer
incapacitation state, but the Player ACT/view animation is not rendered. This
uses SDL ticks and the viewer's capped frame delta, whose update cadence and
ordering against retail callbacks remain provisional. The `00455AB0` effect on
Player `+0x2160` is mapped and ported as the resource-702 visor transition with
its two child sounds; mixer selectors 4/5 and blend parity remain open. The
low-oxygen warning face tint and controller formula are now mapped and ported
with an approximate native alpha ramp; retail blend parity remains open.
`004A1080` stops the `Breathe`
sound at Player `+0x3C4`. State-6 recovery and the delayed
carried-victim drop are wired in the separate passout port note; mode-2
HeroDown spawning remains unsupported. No retail runtime comparison was
performed. The gauge and needle now draw with the traced 96-by-96 rectangle,
angle, and retail color / alpha bitmap pairs; blend parity remains provisional.

The static status path now reaches `geEngine_RenderPoly`, and the native viewer
draws the gauge/needle and low-oxygen warning layers. The `--oxygen-hud-render-check`
checks the warning alpha ramp and the source-mapped resource-702 visor
transition's strict timing bounds and pixel output. Neither this render check
nor the synthetic rescue checks prove retail visual parity. Warning/visor
blend parity, global sound-manager selectors 4/5, and live event cadence remain
open. See
[`player_oxygen_visor_effect_port_2026-10-15.md`](player_oxygen_visor_effect_port_2026-10-15.md).
See also [`player_oxygen_low_warning_visual_port_2026-10-15.md`](player_oxygen_low_warning_visual_port_2026-10-15.md).
Fog remains a separate authored renderer path; oxygen and fog bindings do not
prove they share a trigger mechanic.

Evidence references:

- [`00451870_player_script_command_handler.c`](00451870_player_script_command_handler.c)
- [`0044d550_player_rescue_entity_init_fresh.c`](0044d550_player_rescue_entity_init_fresh.c)
- [`00453d50_player_rescue_object_constructor_fresh.c`](00453d50_player_rescue_object_constructor_fresh.c)
- Saved exports: `reference/previous-work/decomp/evidence/startup-factories/0044bd80_FUN_0044bd80.c`, `00451560_FUN_00451560.c`, and `00458400_FUN_00458400.c`.
- Raw-code checks: `0044BD80..0044BDB0`, `0044CD10`, and `00451560` in the copied, hash-pinned EXE.

## Initial port build checkpoint (2026-10-14)

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
earlier diagnostics validated catalog loading and synthetic rescue
transactions; they did not exercise oxygen behavior or interactive retail
parity.

## Oxygen HUD renderer checkpoint (2026-10-14)

`sh tools/build_scene_viewer_app.sh rescue-all` rebuilt and packaged the shared
viewer. Both the packaged app executable and the CMake build executable have
SHA-256
`88dc2baf13fe9158445d93d88e788d450735456984b1fa8843e5ecb91c62eab6`. The
linker repeated the existing warning that Homebrew json-c targets macOS 27.0
while this project targets macOS 13.3.

The app builder now runs `tools/export_oxygen_sprites.py` and bundles the four
verbatim color/alpha bitmaps under `Contents/Resources/actor_exports/oxygen`.
The packaged `--oxygen-hud-render-check`, run against its bundled MBase scene,
loaded all 63 scene textures and drew the gauge and needle layers, changing
25,999 framebuffer pixels. The packaged `--mission-catalog-check` loaded all
eight retail rescue choices. The packaged MBase `--rescue-check` also passed
its synthetic transaction: trigger 159 released victim 158 and the counter
reached 1/5. `git diff --check` passed. These checks establish asset packaging,
polygon drawing, catalog loading, and one synthetic rescue transaction; they
do not establish correct retail blend state, warning appearance, callback
cadence, or in-game visual parity.
