# Frame-delta source and idle-loop order

Snapshot: 2026-10-12. Read-only Ghidra 12.0.4 queries used the hash-pinned FDNYFirefighter.exe (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). The preserved project copy at `/private/tmp/fdny-ash-transform-analysis/fdny` was opened with `-noanalysis -readOnly`; no live game was used.

## Global timestep source

The clock wrapper begins at `00502AB8`. Its per-update timestep output at field offset `+0x4C` is address `00502B04`, the global read by ash integration `0047F2A0` and other gameplay routines.

`0042C080` updates this object. It chooses one of the two clock subobjects at the wrapper base or base `+0x08`, calls `0042BF70` on that subobject, then copies the selected subobject's field `+0x10` to wrapper field `+0x4C` (`param_1[0x13] = piVar2[4]` in Ghidra's decompilation). This field-based store explains why the preserved reference query shows reads, but no direct static WRITE reference, at `00502B04`.

`0042BF70` measures elapsed time with `timeGetTime`, scales the elapsed millisecond value with the constant at `004DFA28` (binary64 `0.001`) and stores the resulting binary32 interval in subobject field `+0x10`. If the measured interval is greater than binary64 `0.1` at `004DF7B8`, it substitutes binary32 `0.1`. It also waits with `Sleep(1)` while the interval is at or below a separate configured threshold. The active subclock selection and window-focus pause/resume behavior remain clock state, not ash-specific behavior.

## Application ordering

WinMain `0042C800` enters application loop `00499410`. When there is no pending Windows message, that loop calls the application object's `+0x74` callback and then its `+0xA0` callback. The `+0x74` walker `0049A310` dispatches its registered members and tail-dispatches through application slot `+0x60`; thunk `0042BC60` loads the owner at `+0xB4` and jumps to `0042AEA0`.

The owner callback `0042AEA0` first calls `0042C080(&00502AB8)`, updating `00502B04`. It then calls the input object's virtual `+0x14` and, unless its early-exit flag is set, runs owner manager method `00429520`. The app's `+0xA0` callback-list walk occurs after the `+0x74` method returns. Therefore the clock update precedes this owner manager pass on the recovered application idle path.

## Scope and limits

This closes the static provenance of the global frame delta and places its update relative to the owner manager pass. The local ash route is still `00452570 → 004467E0 → 0047F960 → 0047F2A0` subject to source-level gates, but this trace does not identify who recurrently calls Player root slot `+0x14` or how often the ash pool runs. It does not establish a fixed rendered-frame period, live Training clock values, or runtime parent mutation. The Player cadence and combined live motion/transform/geometry/render-list parity remain open. The related Player route search and its negative-result bounds are recorded in `../player-update-scheduling-audit.md`.

## Ghidra queries

Using the preserved project copy with `-noanalysis -readOnly`:

- `AddressXrefs.java 0042C080 0042BF70 00502B04` shows clock callers and global direct references.
- `DecompileFunctions.java 0042C080 0042BF70 0042AEA0 00499410 0049A310 00499E60` shows the field transfer and idle-loop order.
- `AddressXrefs.java 0042AEA0 00499410 00502AB8 00502AD8 00502AF8` checks the owner callback and clock globals.
- `DataValue.java 004DFA28 004DFA2C 004DF7B8 004DF7BC` verifies the two binary64 constants from their low/high words.

The broader app callback path was already documented in `../player-update-scheduling-audit.md` and `../fire-update/application-callback-idle-loop-ghidra-audit.md`; this note adds the clock transfer and its ordering. Ghidra query output is kept outside the repository to avoid storing redundant full decompilations.
