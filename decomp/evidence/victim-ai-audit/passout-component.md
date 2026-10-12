# Carried-victim passout/drop component

`src/victim_passout.h` isolates the carried-victim route in Player helper
`0044FF10` and victim helper `0045C1A0`. On helper entry it increments Player
`+0x3428` and copies `+0x3400..+0x3408` to position `+0x4c..+0x54`. If the
shared pickup carry pointer is populated, the component sets victim state 4,
selects one motion through `Random::bounded` (bound 3 for motion count 6,
otherwise the count), and passes speed 1.0 or 0.2 to the animation callback.
It then delegates the two frame conversions, frame basis update, Player-frame
position copy, floor placement, actor flags enable, and `passout` event in the
observed order.

After `0045C1A0` returns, it calls the Player rescue reset (`0044FCE0(0)`),
adds the source correction to Player position X (`+0x4c`), subtracts the same
correction from the dropped victim frame X translation (frame `+0x28`), then
clears Player `+0x3534`. Ghidra defines `004E1698` as binary32 `30.0`. These
two X writes remain separate and ordered after the reset callback.

An earlier version labeled these coordinates Y. The `0044FF10` assembly sets
`ESI = this + 0x4c`, then loads, adds, and stores through `[ESI]`. The victim
write uses the frame pointer returned by virtual slot `+0x18`, then loads and
stores `[frame + 0x28]`. In the established frame layout, translation starts
at `+0x28` in X/Y/Z order, so `+0x28` is X; the earlier note confused this
first component offset with Y. See `passout-flow.md` for the instruction
window and correction.

The mode-gated `herodown` count/spawn branch and later Player helper cleanup
are outside this carried-victim transaction. No update cadence or runtime
reachability is inferred. Callback internals for frame conversion, animation,
floor placement, actor API, and event dispatch remain unported.

## Evidence

- Existing detailed trace: [`passout-flow.md`](passout-flow.md), including
  `0044FF10` and `0045C1A0` caller/helper fields, and caller limitations.
- Fresh Ghidra 12.0.4 read-only decompilation of both entries used
  `-noanalysis -readOnly` against a scratch copy at
  `/private/tmp/fdny-victim-release-query` because the preserved project was
  locked. The helper decompilation shows state 4, conditional bounded draw,
  frame calls/writes, floor, flags true, then `passout`. The caller shows its
  counter/position preamble; helper call; `0044FCE0(0)`; Player X addition;
  victim frame X subtraction; carry clear; then the separate mode/argument
  gated branch.
- `DataValue.java 004E1698` on the same preserved-project copy returned
  `FLOAT32 30.0000000` (`0x41F00000`).
- Shared random implementation is [`random.h`](../../src/random.h); its
  `bounded` operation matches source `00404B30`, including no stream draw for
  nonpositive bounds.

## Validation and integration

Compile and run:

```sh
c++ -std=c++17 -Wall -Wextra -pedantic decomp/tools/victim_passout_order_check.cpp -o /tmp/victim-passout-order-check
/tmp/victim-passout-order-check
```

The check covers both animation branches and their RNG bounds/speeds, exact
draw counts, no-carry preamble behavior, callback order, actor flags/event,
and the reset → Player correction → victim-frame correction → carry-clear
boundary. Include the header where the helper is integrated, passing the same
`Random` stream used by the rest of the game. Do not schedule it until the
original caller cadence is established.
