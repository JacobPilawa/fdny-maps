# Training escape release component

`src/victim_release.h` models the Player escape branch `00451160` when Player
`+0x3534` is nonzero and the called victim release helper `0045C000`. It keeps
the verified sequence explicit: rescue statistics; victim state 6; two Player
frame-derived axis writes; frame up-axis `(0,1,0)`; separate release-offset
multiplies by 5 and 24; Player-frame position write; floor placement; actor
flags `0x400`; `rescue` event; four rescue timer writes; actor-subobject reset;
then the Player path/post-effect callbacks, carried-pointer clear,
`0044FCE0(0)` rescue reset, and exit check. The Player carry state and actor
base state reuse `VictimPickupPlayerState` and `VictimPickupActorState`, so
pickup and release share one carry pointer and one actor flags/state record.

The component delegates frame conversion, frame orthogonalization, floor
placement, flags API, event dispatch, and adjacent Player effects to callbacks.
It does not calculate a continuous carried pose, implement helper internals,
or schedule the transaction. Passout/drop (`0044FF10` / `0045C1A0`) remains a
separate route.

## Evidence

- `post-pickup-lifecycle-static.md` records the Training escape caller and
  helper side effects. Raw caller export: `rescue-flow/00451160_FUN_00451160.c`.
- The release helper was decompiled with Ghidra 12.0.4 from a scratch copy of
  the preserved project using `-noanalysis -readOnly`, entry `0045C000`.
  The project was locked, so the query used
  `/private/tmp/fdny-victim-release-query`; the query returned the sequence
  represented above. In particular it sets `+0x120=6`, writes the frame data,
  calls the up-axis method with `(0,1,0)`, multiplies the oriented offset by
  `5.0` and then `24.0`, places on floor, enables flags, dispatches `rescue`,
  stores `+0x1F8=150`, `+0x1F4=now+2000`, `+0x1F0=now+2150`, `+0x1FC=1`, then
  calls actor reset `00480D40`. The current time global is loaded after the
  synchronous `rescue` dispatch and after the `+0x1F8=150` write; the component
  therefore samples time with the `readClock()` callback at that point.
- A fresh Ghidra 12.0.4 `-noanalysis -readOnly` query of `00451160` on the same
  scratch project confirmed the caller order: after `0045C000`, it invokes
  `004571A0`, `thunk_FUN_004541A0`, clears Player `+0x3534`, calls
  `0044FCE0(0)`, then calls exit check `004504C0`. Thus the integration
  callback `resetPlayerRescue()` represents exactly `0044FCE0(0)`.
- `release-frame-method.md` documents the indirect frame method evidence and
  its general-basis limits. The reset helper/event/timer effects are retained
  as calls/writes without assigning additional semantics.

## Validation and integration

Run `c++ -std=c++17 -Wall -Wextra -pedantic decomp/tools/victim_release_order_check.cpp -o /tmp/victim-release-order-check && /tmp/victim-release-order-check`.
This verifies the callback sequence, up-axis arguments, flags/state/timer
boundary values, and that the clock is sampled after the rescue event (the
test event changes it to a wraparound value). It also checks carried-pointer
clearing, pickup→release visibility transitions, and that the null-carry branch has no side effects. Include
`src/victim_release.h` where Player escape delivery is integrated. The caller
must provide native callbacks in source order; do not connect it to an update
loop until the original scheduling path is verified.
