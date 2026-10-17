# Player HUD, goggles, equipment and fire-exposure runtime

## Retail evidence

All retail executable references below use `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Fresh Ghidra headless decompilations are saved in this directory as
`0044ee50_player_hint_route_fresh.c`,
`004564c0_hint_window_constructor_fresh.c`, and
`00462a60_goggles_visor_setup_fresh.c`.

Player ScriptActor handler `0044EE50` sends the authored hint string and its
duration to the object at Player `+0x5F4` through `004564C0`. The hint setup
stores the duration, prepares two time-dependent values, clears an existing
GUI text state, then constructs the text widget at `+0x6D8` with the rectangle
`(120, 40)..(520, 440)` in the 640x480 UI coordinate space. The function also
passes two style/mode values of `2`. The Player widget is linked to resource
1020 (`font1.bmp` plus its alpha map), with a 16x24 cell grid and source
glyph order. The meaning of mode `2`, text placement, and the hint's effective
child tint remain under review.

Goggles constructor `00462A60` binds resource 300 (`goggtop.bmp`) and resource
301 (`goggbott.bmp`) and creates 640x32 rectangles at source coordinates
`(0,68)..(640,100)` and `(0,380)..(640,412)`. `rescue.rrf` identifies those
resources as alpha textures. It also initializes resource 703, already traced
to the `heatpulse.bmp` / `heatpulse_a.bmp` survivor/fire target effects. The
viewer now draws both mask textures in those source rectangles while goggles
are selected; tint, alpha conversion, and viewport aspect scaling are
provisional. The target effect remains limited to the source-derived 8.5
second scan window.

The equipment audit confirms axe item constructor `00460A50` creates two child
actors. Ordinary first equip leaves root gate `+0x1FC` clear and activates
secondary model 1 (`axe.act`); the duplicate-equip path enables primary model
6 (`axel.act`) too. The shared viewer now follows this gate for first equip
and duplicate Axe handling.
The extinguisher's `00461690` constructor uses model 2; its post-view update
uses the selected-item follower and the separate action pose path. The retail
action callback advances and samples the active child ActorInfo clock but
does not explicitly choose a motion by name. The viewer advances that clock
and samples the active model's exported `swing` track candidate after
left-click. The retail render transform/scale for either mesh remains open.
The extinguisher contact pose drives spraying. The shared helper `0045F660`
attaches some equipment setup
objects to Player `+0x34A4`, and extinguisher setup `00461830` calls it; the
inventory constructors `00460A50` and `00461690` call common constructor
`0045F7E0`. The retail attached-actor matrix writer and consumer are mapped,
but the live parent/source frame and final camera-space matrix are not. The
viewer currently uses direct camera-relative roots with guessed offsets and,
for the extinguisher, a guessed `0.25` scale. An earlier half-turn candidate
made the Axe edge-on and showed the extinguisher's reverse, so it is no longer
used. These preview roots do not confirm retail-facing parity. ACT hand
attachment, local rotation, scale, and depth behavior remain open. Axe
collision, hit statistics, and door damage are not implemented.

The retail fire-damage route is traced in
[`fire-spread/README.md`](../fire-spread/README.md): FireRescue callback
`004483A0` sends Player message `0x7531`; `00451640` computes a fire-relative
angle and passes it to `0044BB00`, which sets directional burn-indicator state
on Player `+0x353C..+0x353F` rather than changing camera or movement yaw. It
then applies `RC_Pain` chatter and the `Serious Burns` statistic on its
deadline, and accumulates `1.2 * frameDelta * influence`. A strict extended
comparison above 1.0 dispatches passout `00450FB0(player, 1)`. The viewer now
keeps fire contact from changing mouse-look yaw and records the four
source-mapped indicator sectors. `00456090`'s four HUD child widgets are not
yet drawn. It calls the shared message handler from its fire loop, plays the copied `RC_Pain0..3` and
`RC_PassOut0..2` WAVs, and sets the Player to failed state 6 at the source
threshold.

## Native implementation and limits

`scene_viewer.cpp` now draws Player hints from the copied retail 1020 font and
alpha atlases, parses the source color markup, and uses the strict authored
start/end timing on ordinary no-wrap ticks. It omits the prior synthetic dark
panel, which is absent from the examined hint text path. The source rectangle
is used, while centering, wrapping, mode `2`, and tick rollover remain open.
The same viewer draws the two retail goggles mask sprites, equipment ACTs, and
Player fire exposure. The app bundler copies hash-recorded equipment, goggles,
and hint-font assets from `inputs/original/` and includes the seven retail
pain/passout sounds in each selected map's sound package.

Fire spatial falloff uses the retail vertically-compressed ellipsoid formula,
the live FireVisual X/Z size, fire intensity weighting, world-visibility
attenuation, and the 500-unit max-axis gate. One Player candidate is evaluated
per FireRescue; retail's four-record limit is stored on each FireRescue at
`+0xB8`, so the viewer does not cap the whole fire loop at four records. Exact
per-instance record reset timing remains unknown. The host supplies the preview Player's 30-unit horizontal hull
radius as candidate padding, a provisional stand-in for the unresolved fourth
float returned by the retail Player getter. Emitter scale remains unit-valued
because its FireEmitter mapping is unresolved.
Fire callback cadence, record reset ordering, the exact minimum-influence
constant, and alignment of the host movement origin to the retail transform
remain provisional. The `Serious Burns` display is an ephemeral local counter,
not the game's persistent statistics store. Passout stops movement and enters
state 6, but player fall animation, selected-item release, and recovery timing
are not implemented.

The map-neutral viewer built successfully on macOS, and
`tools/build_scene_viewer_app.sh training` rebuilt
`.local/FDNY Scene Preview.app` with the Training scene. The only build warning was the existing
Homebrew `json-c` deployment-target mismatch (library 27.0, build target
13.3). No interactive visual capture was available, so first-person placement,
HUD readability, visor visibility, and in-game fire feel still need a desktop
capture.

An earlier Training build at 2026-10-13 17:22 used a provisional
held-model 180-degree turn; later offscreen comparisons rejected that as the
ordinary preview root. That historical build's packaged and CMake binaries
matched at SHA-256
`3ea209e4f414db809638423e08c556ea7cd7236f07830a90735cb9054ae496a4`, and the
app `Info.plist` passes `plutil -lint`. `--inspect` and the world-scene
`--render-check` pass; the latter does not draw held items. The only build
warning remains the Homebrew `json-c` library deployment target (27.0 versus
13.3). Retail claims above rely on the saved hash-pinned decompilation outputs
and the current headless Ghidra audit. No interactive retail/port visual
comparison was available.
