# Training native reconstruction workbench

This is a curated copy of the earlier native Training prototype. It combines
the old C++ reconstruction with the locally generated Training scene data so
the first Mac build can load the actual Training level and exercise the
reconstructed player, fire, victim, door, and tool paths.

This executable remains a partial Training reconstruction. The port itself
must cover every rescue mission, every other selectable map, and the game
systems needed to run them. Training is a validation fixture and a source of
reusable gameplay code. Promote that behavior behind a shared scene/runtime
contract, while retaining evidence for each component and its data assumptions.

## Build and launch on this Mac

Install CMake, SDL2, json-c, and a C++17 compiler. With Homebrew:

```sh
brew install cmake sdl2 json-c
./run.sh
```

The command configures and builds into `build/`, then attempts to open a window.
The current Codex shell compiled the binary but SDL reported that its video
driver had no displays, so interactive launch remains unverified here.
Press Escape to release or recapture the mouse. Use WASD to move, mouse to look,
Space or right mouse to jump, Shift to use, C to crouch, 1 to select the axe,
2 to select the extinguisher, and left mouse to use the active tool.

To build separately without starting the window:

```sh
cmake -S . -B build
cmake --build build -j 4
```

## Map-neutral rescue scene preview

The separate `fdny-scene-viewer` target loads any exported BSP package through
the shared `SceneData` loader. It uses recovered player movement and BSP
collision code, and creates rescue victims from map entities plus decoded ACT
assets. From the project root, build it with:

```sh
cmake -S port/spikes/training-native-prototype -B port/spikes/training-native-prototype/.local/build-scene-viewer
cmake --build port/spikes/training-native-prototype/.local/build-scene-viewer --target fdny-scene-viewer -j 4
```

Then launch a map (the local MBase package is already exported in this
workspace):

```sh
port/spikes/training-native-prototype/.local/build-scene-viewer/fdny-scene-viewer .local/exports/mbase
```

Use WASD to move, mouse to look, Space or right mouse to jump, C to crouch,
F to snap to the nearest walkable collision floor below the player, and Escape
to release or recapture the mouse. Six-motion survivors run authored `roam`
commands through a collision-checked waypoint approximation and are picked up
automatically on contact using the recovered proximity bounds and message
`0x7532` state transition. The window title reports survivor count and who is
carried, plus the selected tool. The authored PlayerRescue loadout populates
axe/extinguisher/goggles slots; press **1/2/3** to select an equipped slot.
Those keys are viewer controls. Left-click with the axe selected to queue its
retail +100 ms ACTION gate, then play the exported ACT `swing` track and attempt
the source-shaped BModel sweep when the sampled animation clock crosses 0.275
seconds. The retail crossing is previous time `<0.275` and new time `>0.275`;
the port uses rendered-frame `dt` as its clock. The provisional Axe root is
lowered so this exported swing stays in the viewport through the hit window;
retail hand placement and the reported backward-facing pose remain open. An
onscreen hint and title report whether the swing hit the world, found an
eligible breakable, or chopped a named target. Eligible BModels receive their
authored `chop` and `break` events and open their PATH when hit points reach
zero. ACT-to-retail animation mapping, frame cadence, camera-frame mapping,
candidate order, and the backward-facing hand pose remain provisional. With
the extinguisher selected, hold the left mouse button to run the source-derived sampled contact and suppression path;
its decoded ACT now draws from a camera-relative first-person root. The axe
also draws in the first-person view. These presentation offsets, model scale,
and depth behavior are provisional because retail draw roots remain open. With
goggles selected, the retail top/bottom visor masks draw, and left-click scans
nearby actors for the 8.5-second heatpulse window. Scripted hints draw in an
onscreen panel. Fire contact reaches the recovered Player pain/burn/passout
handler and bundled chatter sounds; exposure currently uses the preview's
30-unit hull radius as provisional candidate padding. Fire callback cadence,
goggle tint, exact Axe hit timing/sounds, and the passout animation remain
provisional or open. If a rescue map has no final exit zone, delivery now
force-reselects the first populated equipment slot. Authored Boat A→B, Subway
A→B, and Oil Rig A→B→C→D exit commands now load sibling scene packages after
their remaining-survivor gate. Retail's separate pending-context string
identity and live transition contact/timing remain open.
BModel `touch`/`untouch` events now drive per-owner retail PATH motion;
Subway A doors open/close, Bank `*Door` actors respond to contact, and Bank's
console opens the named vault. Press **E** or **Space** near a scripted console
to use it. Press **M** to inspect every BModel PATH in a renderer-only overlay;
it does not change gameplay collision. Press **N** to clear inspection without
resetting gameplay motion. BoundsTrigger models start with the retail zero
model-flag state; enabled final-exit triggers pulse through the mapped
visibility windows. The render-bit interpretation and host update cadence are
still provisional. Authored AABB contacts and player-solid collision remain
separate from visibility. Replace `mbase` with any key in
`.local/exports/` to inspect another map. Run the same command with `--inspect`
to decode and inventory a package, probe the authored spawn against the BSP
collision hull, and simulate five seconds of idle movement without opening a
window. `--interaction-check` runs the headless Subway A and Bank
event/command/PATH checks, including Subway A's broken-door `Zapp3d` trigger.
Authored BoundsTrigger `makezone` commands also build named map zones from
their brush bounds for the rescue-exit runtime. The shared script host also
routes FireRescue `setsize x y z duration` commands into the recovered timed
size track; Gas R's authored `button1 -> gasoff` script uses this path, though
live button use and original-game effect parity remain unverified.
Interactive startup also prints the texture paths
that fail during image decoding or OpenGL upload instead of silently falling
back to gray. A fresh checkout
must first copy the retail inputs locally and export the desired BSP with
`tools/export_gbsp_scene.py`. The app builder also runs
`tools/export_victim_actors.py` if ACT assets are missing, converts their
bitmaps to SDL-readable BMP files, and bundles `.local/actor_exports` beside
the selected map. These asset exports remain local-only and are not committed.
The original roam target selection and callback timing, contact callback
cadence, exact door/trigger bounds, FMOD-equivalent audio, carried-victim
presentation, and rescue-exit completion remain open. Interaction checks
inject events at authored bounds; live desktop contact and retail timing parity
have not been captured.

For a desktop launch, double-click the project-root `Open FDNY Scene Preview.command`.
Its macOS map picker lists exported BSP packages and opens the selected one in
the viewer. The picker can be skipped from Terminal by passing a map key, such
as `./Open FDNY Scene Preview.command bank`. The executable and selected scene
are assembled into a local app bundle before launch; the launcher invokes the
binary from Terminal so it uses the desktop display session.

For the map-neutral viewer, run `--render-check` to upload the scene's BMPs,
render the authored spawn view and visible victim actors, report framebuffer coverage, and save a BMP
capture beside the scene package as `<package>.render-check.bmp`. On macOS this
uses an offscreen CGL framebuffer, so the check works even when the process has
no desktop display. It verifies the actual OpenGL draw path and is not a
gameplay parity test. To compare a trigger model with its flags forced on and
with the retail initialization value of zero, run
`--trigger-render-check <BoundsTrigger entity index>`; it saves both BMP frames
beside the package and reports changed pixels. The separate Training executable has its own
`--render-check` smoke check for Training actors and fire.

## Local assets

`assets/training/` is generated from the local game-data copy and includes
retail-derived textures and actor images. It is kept out of the public GitHub
mirror. A fresh checkout needs the owner's local Training data placed here
before the executable can load the level. The source and build instructions
remain in the repository.

The prototype source was copied from
`reference/previous-work/decomp/native-training-prototype/`. The local scene
data was copied from the prior `FDNY_trackdown/decomp/assets/training/` output.
This isolated workbench builds from its own folder and does not depend on
either archive path at runtime.
