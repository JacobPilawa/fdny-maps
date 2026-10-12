# Player `0x7531` message transaction

## Recovered order

Player handler `00453A70` routes message `0x7531` to `00451640(player, firePoint, influence)`. Raw instructions in `../player-exposure-assembly.txt` show the following order inside `00451640`:

1. Return immediately when Player state `+0x18C == 6` (`0045165B..00451663`).
2. Turn toward the fire point using `0043B2D0`, `0049E770`, two x87 `FPATAN`s, angle wrapping, and `0044BB00`.
3. Read the global millisecond clock and, only when it is unsigned-greater than Player `+0x390`, resolve/play `RC_Pain`, draw one `[1000,2500]` deadline interval, add the current clock, store the converted low dword, and add one `Serious Burns` statistic.
4. Read frame delta and accumulate `1.2f * dt * influence + Player+0x3538`. Store binary32, then compare the still-live extended sum strictly above `1.0`.
5. On threshold, call `00450FB0(player,1)`. That dispatcher selects animator subobject `+0x430`, invokes `RC_PassOut`, reads the clock and selected-item index, stores Player `+0x342C` and state `+0x18C = 6`, then releases a selected item through vtable `+0x1C` if present.

## Native composition and check

`src/player_fire_message.h` now joins the existing pain-deadline, x87 exposure, and Player passout components in that recovered order. Facing, clock and frame-delta reads, pain audio, statistic mutation, and passout dispatch remain explicit `Ops` boundaries. `tools/player_fire_pcode_check.cpp` runs all four preserved `00451742` suffix cases through this composition with a zero-draw `RC_Pain` stub, checks the source callback order and state-6 early return, and invokes the existing native passout component on threshold cases. Its passout body is independently compared with five original `00450FB0` P-code cases by `player_passout-check`.

Focused result on 2026-10-12: `player_fire_pcode-check` reported four original suffix cases plus the failed-state guard, zero-draw `RC_Pain` stub, composed passout body, and zero mismatches. The added `tiny_above_rounds_one` original execution stores `1.0f` and still takes the strict passout branch at `0045184c`. The checked fixture begins after the facing helper and stops at the original passout callsite. The stitched call to the native passout body is therefore a component composition, not one uninterrupted original-function oracle.

## Open edges

- FireRescue influence production and delivery (`004483A0 → 00453A70`), reset of its four-entry exposure-record buffer, and message cadence are not connected to this native transaction. Static evidence does not establish recurring BModel or FireRescue update order.
- The facing-vector transforms and `0044BB00` effects remain injected and lack direct native comparison.
- Training `RC_Pain` set lookup, media playback, possible one/two shared sound draws, and actual `Serious Burns` storage remain unbound. The P-code comparison uses a no-draw pain stub; the injected C++ callback must receive the shared `Random` when assets/services are recovered.
- `RC_PassOut` chatter, selected-tool release/audio behavior during live passout, and full player state/render effects remain outside the stitched check, although the dispatcher order and selected-item virtual slot are source-backed.
- No normal-speed game capture established when `0x7531` is delivered or how often; do not connect the handler to the native frame loop until that producer schedule is recovered.

## Primary references

- `../player-exposure-accumulation-audit.md`
- `native-player-pain-deadline.md`
- `player-passout-transaction.md`
- `original-player-pain-suffix-pcode.json`
- `player-passout-pcode-oracle.json`
