# Training victim lifecycle after pickup: static trace

This note captures the source-backed lifecycle boundary after Player receives pickup message `0x7532`. Ghidra 12.0.4 queries used a scratch copy of the preserved project in `/private/tmp/fdny-victim-initial-flags-20261011` with `-noanalysis -readOnly`; no live process was attached. The bundled `evidence/original/FDNYFirefighter.exe` SHA-256 is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`; `Genesis.dll` SHA-256 is `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.

## Verified post-pickup routes

- The victim child updater `0045BEA0` returns immediately for state 5. It does not run its state-6 timer, state-7 movement, sound branch, or actor-animation call on that path. This establishes only this updater's behavior; it does not prove that no other subsystem changes a carried victim's pose.
- Training's `escape` Player script route reaches `00451160`. When Player `+0x3534` is nonzero, the function updates rescue counters/statistics, calls `0045C000(victim, player)`, updates the `RC_VictPick` path through `004571A0`, calls `thunk_FUN_004541A0`, clears Player `+0x3534`, calls `0044FCE0(0)`, then checks exits through `004504C0`.
- `0045C000` is the rescue release transform. It sets victim state `+0x120=6`; derives victim frame axes and position from Player frame slot `+0x18`; calls the victim-frame orientation method with `(0,1,0)`; computes the release offset as two separate multiplies by 5 then 24; calls floor placement `0049DA00`; restores actor flags through `00481140(..., true)`; dispatches the `rescue` actor event; writes rescue timer fields `+0x1F8=150`, `+0x1F4=now+2000`, `+0x1F0=now+2150`, `+0x1FC=1`; then calls `00480D40` to reset the actor subobject. Exact frame-method mapping is retained in [`release-frame-method.md`](release-frame-method.md); flag semantics are in [`actor-render-flags.md`](actor-render-flags.md).
- Passout/drop has a separate Player helper route. `0044FF10` checks Player `+0x3534`, calls `0045C1A0(victim, player)`, invokes `0044FCE0(0)`, applies a vertical correction to Player and the victim frame, then clears `+0x3534`. `0045C1A0` sets victim state 4, chooses a motion with one bounded RNG draw (bound 3 when motion count is 6, otherwise the victim's motion count), builds the victim transform from the Player frame, calls floor placement, enables flags through `00481140(..., true)`, and dispatches `passout`. The mapped helper callers and remaining conditions are recorded in [`passout-flow.md`](passout-flow.md).

## Native gap

`src/victim_rescue.h` currently models only the received-pickup transaction: carry pointer, victim state 5, flags 0, `pickup` event and a Player post-event hook. It has no source-mapped carry pose update, Training escape release, or passout/drop transaction. The next concrete native increment is the Training escape/release branch in source order, including the verified release transform and timer/event writes; keep the passout route separate because it uses a different animation/RNG/position sequence. Do not connect either helper to a recurring frame schedule until the original caller order is verified.

## Limits

The child updater's state-5 early return does not establish whether another Player/render subsystem follows the hidden victim during carry. No runtime capture verifies the release/passout timing, exit-trigger schedule, event side effects, or continuous carried pose. This is a static control-flow trace, not proof of a complete playable Training rescue.
