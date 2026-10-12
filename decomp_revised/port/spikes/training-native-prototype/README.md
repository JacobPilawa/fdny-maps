# Training native reconstruction workbench

This is a curated copy of the earlier native Training prototype. It combines
the old C++ reconstruction with the locally generated Training scene data so
the first Mac build can load the actual Training level and exercise the
reconstructed player, fire, victim, door, and tool paths.

This is an early partial reconstruction scoped to Training. It does not
reproduce the original menu, sound, movies, other levels, or the complete
Genesis3D renderer. It is not the main port target or the first playable
acceptance milestone. Reuse individual components only after reviewing their
evidence and removing Training-specific assumptions.

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

The `--render-check` argument renders two hidden frames and reports how many
Training mesh groups and textures loaded. It is a smoke check, not a gameplay
parity test.

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
