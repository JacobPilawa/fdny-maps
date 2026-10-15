# CMake and wide-integer platform portability — 2026-10-15

## Scope

This checkpoint improves dependency discovery and removes the shared
gameplay's compiler-specific 128-bit integer dependency. It does not establish
Windows or Linux build support. The full port still requires Windows and Linux,
with a macOS playable slice as the first target.

## Source findings

- `port/spikes/training-native-prototype/CMakeLists.txt` now checks SDL2 and
  json-c through pkg-config, CMake package targets, and standard include and
  library paths. Compiler flags are selected by compiler family.
- The Training workbench is Apple-only because its menu uses CoreText and
  CoreGraphics. The map-neutral scene viewer is the shared target.
- `src/clock.h` and `src/extended.h` now use `src/wide_integer.h`. Toolchains
  with native unsigned 128-bit integers use them; other toolchains use the
  portable two-word unsigned implementation. Signed clock arithmetic uses a
  portable sign/magnitude wrapper over that type. CMake no longer rejects a
  compiler for lacking `__int128` or `__uint128_t`.
- The non-Apple viewer uses SDL2 and desktop OpenGL. Its offscreen framebuffer
  render checks remain CGL-only, so the current build path does not provide
  those checks on other systems.

## Verification

The host is macOS arm64 with Apple clang 14.0.3 and CMake 4.4.3.

The pkg-config configure and build succeeded:

```sh
cmake -S port/spikes/training-native-prototype -B .local/build-gameplay-port
cmake --build .local/build-gameplay-port --target fdny-scene-viewer fdny-training -j4
```

The no-pkg-config fallback configure and viewer build also succeeded:

```sh
cmake -S port/spikes/training-native-prototype -B .local/build-portability-fallback -DCMAKE_DISABLE_FIND_PACKAGE_PkgConfig=TRUE
cmake --build .local/build-portability-fallback --target fdny-scene-viewer -j4
```

The portable integer implementation was then forced on macOS for both targets:

```sh
cmake -S port/spikes/training-native-prototype -B .local/build-wide-fallback -DCMAKE_CXX_FLAGS=-DFDNY_FORCE_PORTABLE_WIDE_INTEGER
cmake --build .local/build-wide-fallback --target fdny-scene-viewer fdny-training -j4
```

A temporary parity harness compared 100,000 unsigned arithmetic cases,
100,000 signed arithmetic cases, and 10,000 randomized GameClock updates
against the compiler's native 128-bit arithmetic; all matched. The fallback
viewer passed `--victim-roam-math-check` and `--rescue-check` for all 13 bundled
rescue, linked-stage, and Training packages: Training 2/2, MBase 5/5, Bar R
6/6, Gas R 7/7, Bank 6/6, Boat A 9/9, Subway A 6/6, Oil Rig A 3/3, Boat B 5/5,
Subway B 10/10, and Oil Rig B/C/D 3/3, 6/6, and 3/3. These rescue checks are
controlled transactions, not natural travel or retail parity. Bar R, Boat A,
Bank, and the Oil Rig packages also logged existing unimplemented EVS commands;
all rescue checks still returned success.

The Training `--clock-check` runner could not use its expected
`evidence/clock-oracle.json`, which is absent from this checkout; the temporary
native-arithmetic parity harness provided the clock comparison instead.

No Linux or Windows compiler was available for a native or cross build here.
The results confirm the host targets, fallback integer implementation, and
both CMake dependency-discovery paths on macOS only. They do not verify launch,
input, rendering, or runtime parity on another operating system.

## Shared OpenGL header follow-up

The viewer now includes SDL2's `SDL_opengl.h` instead of selecting raw Apple
and non-Apple `gl.h` headers in `scene_viewer.cpp`. The Apple-only offscreen
renderer still enables extension prototypes and includes CGL declarations;
the shared window renderer uses SDL's platform OpenGL declarations. This
removes a Linux/Windows header assumption from the shared translation unit,
but does not prove those platform builds.

After the include change, the existing macOS CMake target rebuilt successfully:

```sh
cmake --build .local/build-scene-viewer --target fdny-scene-viewer -j2
```

The linker reported that the installed json-c dylib targets macOS 27.0 while
the build deployment target is 13.3. Linking still succeeded. Linux and
Windows toolchains remain unavailable in this workspace.

## Remaining platform gates

- Build and launch the viewer with Linux GCC and Clang, and verify SDL2,
  OpenGL, asset path case, and interactive behavior.
- Configure, build, and launch the shared runtime with Windows/MSVC; verify
  SDL2, OpenGL, filesystem, input, audio, and packaged assets.
- Add platform-appropriate offscreen render checks where useful; current
  `--render-check` infrastructure uses CGL on macOS.
