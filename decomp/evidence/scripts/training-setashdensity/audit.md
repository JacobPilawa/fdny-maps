# Training `setashdensity`

## Scope and evidence

Static analysis of the local read-only Ghidra 12.0.4 project for `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). The Ghidra decompilations and instructions named below were refreshed with `analyzeHeadless -noanalysis -readOnly`; local raw exports are ignored by the Git mirror.

Training's preserved entity records contain three commands: Player entity 115 starts with `setashdensity 0`; BoundsTrigger 170 sends `setashdensity 0.5` to Player on touch; BoundsTrigger 189 sends `setashdensity 0` on touch. These are authored command values, not observations from a live run.

## Original path

- Player's command handler is `00451870`. Its `setashdensity` branch reads one float argument with `0040CC50`; that helper copies the token and calls `sscanf` with `%f`, then `%g` if the first conversion fails. On success the handler calls `0044BC90` with the Player base (`this - 0x140`) and parsed float.
- `0044BC90` adds `0x55c` to the Player base and calls `00446350`. `00446350` stores at child offset `+0x54`, absolute Player offset `+0x5b0`. It clamps values above the scalar at `004DFC24` (`1.0f`) to 1 and values below `004DC50C` (`0.0f`) to 0. The raw x87 comparison sequence is retained in `assembly.txt`.
- Player constructor `00453D50` calls `004469F0` on Player `+0x55c`; this initializes the child but does not assign its `+0x54` field. Player primary vtable `004E1C48` slot 8 (`+0x20`) is `0044D550`, the Player initialization method. It calls `004465D0` on the same child; that method builds the 100-entry object list and assigns child `+0x54 = 1.0f`. Thus the verified post-initialization default is 1.0 before the Training Player startup command overwrites it with 0.
- The Player movement callback `00452570` is slot 0 of the secondary vtable `004E1C40` installed at Player `+0x140`. Its call at `00452926` passes `this + 0x41c` to `004467E0`; secondary base `+0x140` plus `+0x41c` is the same Player `+0x55c` child. `004467E0` updates the pool, then assembly at `004468A2–004468B3` loads pool count `+0x3c`, multiplies it by density `+0x54`, and converts the result through `004B9400` before scanning that many leading slots. `004465D0` constructs 100 slots, so the authored values produce scan limits of 0, 50, and 0. An inactive slot in the scanned prefix can spawn through `00446470`; this path has conditional shared-RNG calls. The density-to-prefix behavior is source-resolved, but pool lifecycle, visible particle rendering, and the Player callback's full-frame schedule remain open.

## Native status and limits

`Player::ashDensity` now starts at the source-backed 1.0 default. The native host uses `strtof`'s prefix conversion semantics to preserve the original `%f`/`%g` scan behavior, then stores the state and applies the same verified clamp. The existing `--script-check` runs startup and reports 0.0 after Player entity 115's command. The two BoundsTrigger commands are event bindings and are not exercised by this startup-only check.

This ports the command state only; the native pool, its source-driven update/count rule, RNG calls, and visible particles are not yet wired. No original live capture or visual parity result is claimed. The refreshed local raw Ghidra exports are in `functions/`, with the emitter/update instructions in `effect-assembly.txt` and Player vtable words in `player-vtable.tsv`; all raw exports remain ignored by the Git mirror. The `004B9400` conversion's decompilation and instructions are also preserved under `../rng/004b9400_FUN_004b9400.c` and `../fire-update/player-exposure-timer-followup/random-conversion-assembly.txt`.
