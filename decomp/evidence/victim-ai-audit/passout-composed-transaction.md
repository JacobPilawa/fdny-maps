# Composed Training passout and herodown transaction

Snapshot: 2026-10-11. This composes the Player caller `0044FF10`, carried-victim helper `0045C1A0`, and the post-drop herodown/cleanup tail as source-ordered operations. It is an explicitly invoked transaction, not a recurring frame update or a complete playable rescue path.

## Source order preserved

`VictimTransactions::passout` now runs the recovered components against the same Player/carry state and runtime actor records used by pickup and escape:

1. Read Player position `+0x3400`; increment `+0x3428`; copy the position to cached Player `+0x4c..+0x54`, and write that copy at the same point in the host adapter.
2. If a victim is carried, set state 4; choose a motion using `Random::bounded(3)` for six motions, otherwise the source motion count; apply the source speed; read the Player frame separately for each basis/position operation; then place the dropped hull on the Training BSP floor.
3. Set the Genesis actor flags word to `0x400` and synchronously dispatch `passout`.
4. Call the Player rescue reset. Add the value at `004E1698` (`30.0f`) to cached Player X and write that field before the following victim-frame operation.
5. Reread Player carry pointer `+0x3534` after callbacks, resolve that receiver's frame, subtract 30 from its X translation, then clear carry. There is no source null check at this later dereference.
6. Read global mode and inspect the current entity list only for mode 2 / helper argument 0. The existing tail then performs the capped `POFighter` staging/count/stat/log branch, live `+0x357C` cleanup reads, optional nested-state transition and selection refresh, followed unconditionally by Player view reset and rescue-status completion.

Cached position, source position `+0x3400`, and frame translation remain separate values. Host position writes now occur at the source writes rather than after the full helper returns. Spawn, animation, frame access, status/UI, and cleanup internals are required host services; no default behavior or RNG draw is invented for them.

## Evidence and validation

`--victim-transactions-check` exercises the composed operation with a controlled callback that changes the carry receiver during rescue reset. It confirms that the later X correction reaches the newly resolved actor frame, not the initially passed actor; it also checks the passout RNG draw, flags/state, cached position writes, floor-failure fallback, and unconditional tail callbacks. Its supplied `PlayerFrame` point intentionally takes the already source-defined failed-placement fallback `{0,1,0}`; successful native Training floor placement remains covered separately by `victim-frame-ops-check` and the six-case Genesis.dll P-code fixture.

The passout prefix's 32 original P-code cases / 576 word comparisons, the herodown-tail order check, and all 18 CTest checks pass. These are component/integration comparisons with supplied frame/UI/engine services; they do not establish a live callback changing carry, original recurrence/cadence, automatic Training scheduling, or full-run RNG parity.

## Remaining work

- Bind the required host services to the real runtime Player, animation, spawn, status, and cleanup implementations.
- Establish the original Player/actor recurring caller and callback order before scheduling this operation.
- Capture a complete Training passout sequence from equivalent source input before claiming whole-level parity.
