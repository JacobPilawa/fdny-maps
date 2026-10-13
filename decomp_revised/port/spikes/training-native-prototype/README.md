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
the shared `SceneData` loader. It uses the recovered player movement and BSP
collision code and does not start Training's scripts, fire/victim systems, or
mission flow. From the project root, build it with:

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
to release or recapture the mouse. The window title reports texture uploads,
grounded/falling state, and the current player position. Replace `mbase` with any key in
`.local/exports/` to inspect another map. Run the same command with `--inspect`
to decode and inventory a package, probe the authored spawn against the BSP
collision hull, and simulate five seconds of idle movement without opening a
window. Interactive startup also prints the texture paths that fail during
image decoding or OpenGL upload instead of silently falling back to gray. A fresh checkout
must first copy the retail inputs locally and export the desired BSP with
`tools/export_gbsp_scene.py`.

For a desktop launch, double-click the project-root `Open FDNY Scene Preview.command`.
Its macOS map picker lists exported BSP packages and opens the selected one in
the viewer. The picker can be skipped from Terminal by passing a map key, such
as `./Open FDNY Scene Preview.command bank`. The executable and selected scene
are assembled into a local app bundle before launch; the launcher invokes the
binary from Terminal so it uses the desktop display session.

For the map-neutral viewer, run `--render-check` to upload the scene's BMPs,
render the authored spawn view, report framebuffer coverage, and save a BMP
capture beside the scene package as `<package>.render-check.bmp`. On macOS this
uses an offscreen CGL framebuffer, so the check works even when the process has
no desktop display. It verifies the actual OpenGL draw path and is not a
gameplay parity test. The separate Training executable has its own
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
