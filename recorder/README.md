# FDNY gameplay recorder

A 32-bit ASI plugin for **F.D.N.Y. Firefighter: American Hero** that records actual player and survivor state to JSON Lines files. Recording starts automatically when a stage's Player is initialized, with a separate stage-and-UTC-timestamp file for each run/restart.

The plugin records positions rather than reconstructing a path from inputs. A viewer can draw the whole trajectory or scrub through time without reimplementing the game's physics. It also records velocity, body facing, pitch, sampled controls, measured collision bounds, and victim snapshots/lifetimes.

## Quick start

1. Close the game.
2. The game needs a **32-bit ASI loader**. An existing setup that already loads an FOV-fix ASI can use that same loader. Otherwise obtain [Ultimate ASI Loader](https://github.com/ThirteenAG/Ultimate-ASI-Loader) from its official project and follow its installation instructions; `dinput8.dll` is the loader used by the tested setup. The loader and game are separate dependencies, not included here.
3. Install the included plugin with Python 3.9+:

   ```sh
   python recorder/install.py "C:\Games\FDNY"
   ```

   The argument must be the folder containing `FDNYFirefighter.exe`, not this repository. Use `python3` where that is your Python command. The installer validates the game and plugin, backs up an existing recorder/config, and creates an INI with the selected output directory. It does not alter the game executable or loader.

4. Launch the game normally and enter a stage. `FDNYRecorder.log` beside the game should report `READY`; recordings go into the game directory's `Recordings` folder by default.
5. Quit normally to save the final buffer. Choose a resulting `.jsonl` file in a compatible viewer, or inspect it using:

   ```sh
   python recorder/inspect_recordings.py "C:\Games\FDNY\Recordings\YOUR_RUN.jsonl"
   ```

Choose a different writable output folder with `--output-dir`. `--check-only` validates without installing, and `--no-victims` disables victim capture in the generated config. Installation intentionally accepts only the known executable hashes listed in [COMPATIBILITY.md](COMPATIBILITY.md).

### Wine

Run the installer on the host with the game directory's host path. On non-Windows hosts it converts the output path to the usual Wine `Z:` mapping. If your prefix uses a different mapping, supply `--windows-output` explicitly. For example, with a game installed at `C:\Games\FDNY` inside a prefix:

```sh
python3 recorder/install.py /path/to/prefix/drive_c/Games/FDNY \
  --windows-output 'C:\Games\FDNY\Recordings'
```

The loader must be enabled in that prefix. The tested configuration uses `dinput8=native,builtin`; retain the working loader settings in an existing setup. The ASI is a Windows x86 DLL even when running through Wine on another host architecture.

### Manual installation

Copy `bin/FDNYRecorder.asi` beside `FDNYFirefighter.exe`. Copy `FDNYRecorder.ini.example` to `FDNYRecorder.ini` in the same directory and set `OutputDirectory` to a writable path as seen by Windows/Wine. The parent directory must exist; the plugin creates the final output directory itself. This version uses Win32 ANSI file APIs.

To disable capture, close the game and set `Enabled=0`, or remove only `FDNYRecorder.asi`. Keep any loader/FOV-fix files needed by your existing setup.

## Build from source

Install [Zig 0.14.1](https://ziglang.org/download/), put `zig` on `PATH`, then run:

```sh
python recorder/build.py --selftest
```

Or pass `--zig /path/to/zig`. This cross-compiles `x86-windows-gnu` from Windows, macOS, or Linux and writes the plugin plus optional self-test to `recorder/build/`. It does not download or install a compiler. The reference build uses `-O2 -g0 -s -ffp-contract=off`. To install your build instead of the bundled binary:

```sh
python recorder/install.py "C:\Games\FDNY" --plugin recorder/build/FDNYRecorder.asi
```

`bin/SHA256SUMS` identifies the bundled binary and source inputs. No game binaries, BSPs, loader DLLs, or proprietary assets are required to compile the recorder.

## How capture works

`FDNYRecorder.c` contains initialization, guarded memory reads, the buffered JSONL writer, and Player vtable observers. `victim-recorder.h` adds victim spawn/update/destruction observers and a lifetime registry. Original callbacks still run; the wrappers preserve return values, Windows last-error, and x87/SSE state. The plugin modifies callback pointers in process memory, without patching the executable on disk or writing gameplay fields.

Player snapshots are taken after each observed Player root-update callback, not at an assumed fixed render rate. Registered live victims are also sampled from Player updates, independently of AI cadence, with a vtable check and guarded reads. Victim snapshots are rate-limited to approximately 20 Hz, with extra spawn/state-transition observations. Destruction ends the track. Up to 128 victim lifetimes are tracked per run.

The writer buffers up to 64 KiB and writes at roughly one-second intervals while callbacks arrive, plus at run closure. Returning to the menu does not itself close a run; the next Player initialization or ordinary process exit does. Crashes/force-quits can lose the current buffer. Complete JSON lines already written remain readable without an `end` record. A long pause may retain a partial buffer until updates resume or ordinary exit.

This is trajectory/state playback, not deterministic input replay or video capture. Doors, fires, interaction targets, and other world state are not recorded. Body position/facing plus pitch do not describe the final animated camera transform. Sampled virtual keys can miss brief taps and do not resolve custom game bindings.

## Implementing your own viewer

Start with [FORMAT.md](FORMAT.md). `examples/synthetic.jsonl` exercises standing/crouching and a victim track without requiring the game. `integration/recording-data.js` is a dependency-free parser/timeline helper copied from the map viewer; it validates the format, handles partial files, finds samples, interpolates nearby positions/facing, and supplies measured or legacy collision bounds.

```sh
node recorder/integration/read_recording.mjs recorder/examples/synthetic.jsonl
python recorder/inspect_recordings.py recorder/examples/synthetic.jsonl --plot path.svg
```

See [integration/README.md](integration/README.md) for the rendering recipe. The package is independent of the repository's viewer UI; it can be used in another renderer.

## Tests and evidence

Build with `--selftest`, then run the console test with an existing parent directory and a Windows-visible output path:

```bat
recorder\build\recorder-selftest.exe C:\Temp\fdny-recorder-test
```

Under Wine, run that executable with your Wine runtime and a mapped output path. The test uses synthetic objects and never attaches to or launches the game. It verifies signature rejection, callback forwarding/return values, destructor arguments, last-error/FP control preservation, run separation, measured player bounds, victim lifecycle events, and Player-driven victim sampling. It produces three short recordings for inspection.

Player capture was exercised in actual gameplay, including Training restarts, crouching and climbing. The 1.1 victim extension passed synthetic integration checks; real stage behavior and performance still need gameplay validation. Training survivors ordinarily remain stationary: test pickup/carry/release there, and use a stage with roaming survivors to assess roaming traces. Fields labeled `raw` preserve observations rather than promising a complete semantic interpretation.
