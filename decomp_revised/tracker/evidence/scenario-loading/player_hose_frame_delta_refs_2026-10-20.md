# Retail game frame-delta clock (2026-10-20)

Retail `FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra 12.0.4 headless reference scan of global `00502b04` produced 55 direct
code references, all classified as `READ`. The writer is an object-field
alias, which is why the direct-reference scan does not show a store. In the
normal update route, `00499410` dispatches the game update through
`0042aea0 ProcessEverGameUpdate`; at `0042aec0..0042aec5`, that updater passes
`&DAT_00502ab8` to `0042c080`. The clock manager chooses its active timer
record, calls `0042bf70`, and copies that record's float at `+0x10` to manager
offset `+0x4c`, exactly global `00502b04`.

`0042bf70` reads `timeGetTime()` in milliseconds, subtracts its prior tick,
handles DWORD wrap, multiplies by binary64 `0.001`, and stores the result as
binary32 delta. It sleeps with `Sleep(1)` while the stored delta is less than
or equal to binary64 `1/60`; when the delta exceeds binary64 `0.1`, the cap
branch adjusts its millisecond clock fields and stores `0.1f`. The verified
constants are `004dfa28=0.001`, `004dfa20=0.016666666666666666`,
`004df7b8=0.1`, `004dfa30=2^32`, and `004dfa18=1000.0`. The update delta is
source-defined in seconds, with a lower pacing threshold and a 100-ms upper
bound.

The focus path is also traced: `00499410` calls `0042bf20` when the app loses
focus and `0042bf40` on regain. These store the pause start with
`timeGetTime()` and add the paused duration to the timer base. The viewer does
not yet mirror this pause offset for its other absolute-time consumers.

Selected decompilation is preserved in
[`0042bf70_frame_clock_delta_fresh.c`](../runtime/0042bf70_frame_clock_delta_fresh.c).
The repeatable direct-reference scanner is
[`TraceFrameDeltaRefs.java`](../../../tools/ghidra-scripts/TraceFrameDeltaRefs.java),
and constants were decoded with
[`ReadFloatBits.java`](../../../tools/ghidra-scripts/ReadFloatBits.java).
Full headless logs remain in ignored `.local/ghidra/` files.

The references include the PlayerHose hotspot countdown at `004674A0`, the
WindowFire contact timer at `00468680`, and its ramp at `00468870`. Other
consumers use the same global for player movement, fire exposure, victim
movement, and visual updates. The shared host now uses the traced millisecond
conversion, `>1/60` pacing gate, and 0.1-second cap via SDL ticks and
`SDL_Delay(1)`. SDL clock quantization, focus-pause behavior for non-delta
timers, update callback ordering, and live retail parity remain open.

The rescue-all macOS build compiled with matching standalone and packaged
executable SHA-256
`7d04b10a561f5a1bafc53551ef601093a8a894d6c8a68498c076cc87e2a2f17b`; its
plist passed, with eight rescue choices and four hose scenes packaged. No
tests, interactive run, or retail comparison were made.
