# Training BModel throw scalar oracle

This is a bounded Ghidra p-code execution of the original `FDNYFirefighter.exe`, supplementing [`throw-path-ghidra-audit.md`](throw-path-ghidra-audit.md). It captures the full scalar setup routine at `0043B580`, then runs `0043D330` only until its first call to `0043BAA0`: active interpolation stops at `0043D522`, while completion stops at `0043D46D`. The engine/model mutator, script event, and timer-cleanup calls are not executed. The fixture and reproducible script are [`throw-scalar-oracle.json`](throw-scalar-oracle.json) and [`ThrowScalarOracle.java`](../../tools/ghidra/ThrowScalarOracle.java).

The fixture identifies the program SHA-256 as `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. It was generated with Ghidra 12.0.4 `EmulatorHelper`, x86 p-code, and x87 control word `0x037f`. The script was run with `-noanalysis -readOnly` against a separate scratch copy at `/private/tmp/fdny-liftthrow-ghidra-20261011`; the preserved Ghidra database was not modified. These are p-code results under the stated x87 setup, not a live or hardware execution trace.

## Verified setup fields

The instruction listing and p-code probe agree on these stores in `0043B580`:

- `+0x208` receives the current 32-bit tick from `DAT_00502AFC`.
- `+0x210` receives binary32 `-384.0` (`0xC3C00000`).
- `+0x218` receives the starting middle coordinate from `+0x1F4`.
- `+0x25C` is set to mode 2.
- `0043B2D0` computes endpoint subtraction (`+0x1F0..+0x1F8` minus `+0x1FC..+0x204`). The caller squares those three components, sums them, takes x87 square root, multiplies by binary32 `0x3A888889` (`0.00104166672` at `004E0BB8`), and stores binary32 duration at `+0x20C`.
- `+0x214` receives `((endMiddle - startMiddle) - duration² * -384.0) / duration`, rounded at its final float store. The code uses the duration still in the x87 register after storing `+0x20C`; it does not reload the rounded `+0x20C` value before computing `+0x214`.

The last detail is observable for the probe from `(0,0,0)` to `(480,0,0)`: `+0x20C` stores exactly `0.5f` (`0x3F000000`), while `+0x214` stores `192.0000152587890625f` (`0x43400001`), one binary32 ULP above 192. The executable constant used for the duration multiplier is therefore important to preserving the original operation sequence; an algebraically simplified C++ formula based on the stored `0.5f` would not reproduce this bit pattern.

## Verified update fields and tick boundary

For active mode (`+0x25C == 2`), `0043D330` obtains the signed 32-bit difference `DAT_00502AFC - +0x208`. If negative, it adds the binary64 constant at `004DFC90` (`0x41F0000000000000`, 4294967296.0) to account for one 32-bit tick rollover. It multiplies the result by the binary64 constant at `004DFA28` (`0x3F50624DD2F1A9FC`, 0.001) without first storing elapsed seconds as a float.

It compares that extended-precision product against the binary32 duration at `+0x20C`. The source-level completion condition is strict `duration < elapsed`. But for an exactly representable duration of `0.5f`, a 500 ms tick is already treated as complete in the Ghidra p-code fixture: multiplying integer 500 by the stored binary64 `0.001` retains an extended-precision result slightly above 0.5 before the compare. At the same 500 ms tick, the next binary32 duration (`0.5000000596046448f`, bits `0x3F000001`) remains active; it is complete at 501 ms. This boundary is based on the original instruction sequence and constants, not on comparing rounded decimal displays.

While active, the updater uses `t = elapsed` when `+0x220 == 0` and `t = duration - elapsed` otherwise, divides `t` by duration and stores that interpolation ratio as a float local, then evaluates the two endpoint coordinates linearly and the middle coordinate as `(t * -384 + +0x214) * t + +0x218`. It calls `0043BAA0` with that point. On completion, before the first `0043BAA0` call, it selects endpoint `+0x1FC` when `+0x220 == 0` and `+0x1F0` otherwise. The previous audit traces the later selector flip, `drop` event, and timer-object cleanup.

The fixture also records a 32-bit rollover sample (`0xFFFFFFF0` to `0x00000018`, delta 40 ms), both selector directions, the 499/500/501 ms boundary, and raw float output bits. It does not establish update cadence: `0043D330` still has an unresolved caller at `0043DDC6`, and no wall-clock/player-frame schedule is proven here.

## Native implementation status and next step

The isolated native `decomp/src/throw_scalar.h` now transcribes setup and the scalar prefix of update. `--throw-scalar-check` matches all 24 setup fields and 64 update branch/point fields in this p-code fixture; see [`throw-scalar-native-check.md`](throw-scalar-native-check.md). This is not gameplay/model-transform parity or a live/hardware comparison. A separate static Ghidra trace follows the vtable method `0043DC60` to `0043D330`, records the `0043BAA0` transform/bounds update, and confirms the `drop` event and timer cleanup order; see [`throw-runtime-ghidra-audit.md`](throw-runtime-ghidra-audit.md). Its dynamic dispatcher and frame cadence remain unresolved, so the scalar check stays isolated from gameplay scheduling until that order is sourced.
