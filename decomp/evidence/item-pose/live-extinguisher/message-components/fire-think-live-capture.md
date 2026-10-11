# Fire Think live caller capture

Current status (2026-10-11): **interactive capture is not ready; do not attach WineDbg to the game.** The user agreed to enter Training for a bounded capture after the capture path is ready. Prior WineDbg attempts stalled on the first interactive command; the user saw a black/frozen screen after the first and a crash during the second. Neither attempt recorded gameplay events. See the preserved attempt folders under `decomp/evidence/live/fire-dispatch-20261011-*`.

## Why the current recorder is not bounded

`tools/record_fire_dispatch.py` has an event-count limit, but its `dbg.command('cont', 600)` can wait up to ten minutes for each debugger stop and there is no end-to-end deadline while waiting for the first Fire Think hit. More critically, `record_startup.Debugger.cleanup()` waits up to 15 seconds for a prompt after an error, then prints a warning and closes its PTY if it cannot confirm `detach`. The WineDbg process/debuggee state is then unknown. A per-event count therefore does not guarantee a maximum user wait or a clean detach.

The check-only mode hashes the installed executable and debugger without attaching. It does not test the interactive command channel or detach behavior. A safe capture requires an independently demonstrated timeout/recovery path on a harmless Wine process before attaching to the game. Do not ask the user to enter Training until that path passes.

## Evidence needed

The static Ghidra audits preserve `00449FE0` as slot 0 of FireRescue's visual secondary vtable `004E1774` at object `+0x24`; they do not identify its recurring caller. A successful capture would record caller/order for `00449FE0` relative to FireRescue primary callback `004483A0`, owner update `00429520`, player view `0044F340`, extinguisher post-view `00461F80`, message drain `0049B610`, Player message/exposure/damage callbacks `00453A70`/`00451640`/`00450FB0`, and shared RNG functions `00404AC0`, `00404B30`, `00498B30`, and `00404960`. Existing event records can capture registers, stack words and return addresses; they do not capture RNG return values or normal-speed cadence because breakpoints pause the process.

The Fire Think callback's caller, cadence and order remain unresolved. Do not connect `FireUpdate::think` or player exposure to the native tick until source-backed scheduling evidence supports it.
