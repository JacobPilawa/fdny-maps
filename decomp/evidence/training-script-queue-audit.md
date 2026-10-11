# Training ScriptActor queue audit

Read-only Ghidra 12.0.4 audit on 2026-10-11 for `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6fca72fe5f7064ca8122fda7d41e6f2fc3e`). The command handlers were inspected with `DecompileFunctions.java`, the multiplier with `DataValue.java`, and caller references with `AddressXrefs.java`. No live gameplay capture was used.

## Original behavior

- `0046F9D0` handles ScriptActor commands. Its `que` branch loops over each child command and appends it to the actor's command container through `0046E9D0`; it does not run command children inline. The `cmd` branch loops over the children and dispatches each immediately through `0046E470`.
- `0046F9D0` handles `wait` by parsing its numeric argument, multiplying by the data at `004E0ED4`, adding the current game clock, converting the deadline through `004B9400`, then storing it at actor `+0x48` and setting the wait flag at `+0x4C`. `DataValue.java` read the referenced constant as float32 `1000.0` (bits `0x447A0000`, bytes `00 00 7A 44`). The native VM now uses the same absolute millisecond clock value as the game loop. Training's `wait 3` therefore uses a 3,000 ms deadline from that clock. Fractional-wait conversion/rounding parity has not been checked.
- `flush` clears the actor queue at `+0x30` and clears the wait flag at `+0x4C`. `idle` clears the wait flag without clearing the queue.
- `00470740` checks whether an active wait deadline at `+0x48` is at or before global clock `00502AFC`; if so, it clears the flag. When no longer waiting, it invokes virtual slot 0, copies the wait state into `+0x50`, and calls `004704A0` only if the wait flag remains clear.
- `004704A0` gets one head item from the queue at `+0x30` through `0046E530`, then dispatches that item through `00470150`. This path consumes at most one queued item per `00470740` callback.
- Ghidra xrefs place calls to `00470740` in multiple host classes, including callsites `0043AFE3` and `0044FE5F`. Exact relative order among those host callbacks and script-trigger dispatches is not established here.

Training uses both queue forms: the shared `DoorClose` macro is `que [close, (playsnd CloseDoor)]`, and Training trigger entity 197 uses `touch: que [ wait 3, (*Cmd3a) ]`. The prior native interpreter treated `cmd` and `que` the same and scheduled `wait` as a global delayed action, so it did not model ScriptActor queue state.

## Native implementation and limit

`src/scripts.h` now separates immediate `cmd` execution from per-entity `que` storage, processes at most one queued command per native `Scripts::advance` call, holds commands until a deadline computed from the absolute game clock in milliseconds, and implements `flush`/`idle` state changes. Its `wait 3` deadline is `currentTimeMs + 3000`.

The current host calls `Scripts::advance` once per render/update loop, before trigger dispatch, and visits native queue owners by entity ID. This is a practical integration point, not a verified reproduction of the original owner callback order. Exact script-queue timing versus player movement, door update, trigger callbacks, and other actor callbacks remains open. Do not count this as full Training scheduling parity. Build and focused checks on 2026-10-11 passed: `--script-queue-check` (mismatches=0), `--script-check` (52 scripted entities, 30 variables, no unsupported commands), and `--clock-check` (3,000 original updates, mismatches=0).
