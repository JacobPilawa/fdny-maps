# Passout transaction and herodown tail composition review

This is a bounded source-order review of `victim_passout.h`, `player_herodown_tail.h`, and the current `VictimTransactions` composition against the existing Ghidra trace in `passout-flow.md`. It does not establish a recurring caller or scheduler.

## Position sources remain distinct

At entry to `0044FF10`, the Player passout counter at `+0x3428` increments and the three words at `+0x3400..+0x3408` are copied into Player cached position `+0x4c..+0x54`. The model names those arrays `passoutPosition` and `position`. In the carried branch, `0045C1A0` independently obtains the Player frame through vtable slot `+0x18`: separate frame conversions populate victim frame basis fields, and a third frame read copies Player frame translation `+0x28..+0x30` into victim position. These are distinct source fields and operations; the cached Player position must not stand in for the Player frame getter.

The reviewed evidence does not show when or how Player frame translation at `+0x24/+0x28..+0x30` is synchronized with cached position `+0x4c..+0x54` or source `+0x3400..+0x3408`. The separately documented `+0x0c` path transforms a frame origin into Player `+0x68..+0x70`; it does not prove synchronization among these passout fields. Keep the live frame getter and the cached position arrays separate until an actual source path establishes that relationship.

After `0045C1A0` dispatches `passout`, the caller invokes `0044FCE0(0)`, adds the correction to cached Player X (`+0x4c`), subtracts it from the dropped victim frame translation X (`+0x28`), then clears Player carry pointer `+0x3534`. The herodown spawn position therefore reads the post-correction cached Player position when a victim was carried; without a carried victim it uses the entry copy from `+0x3400`. The suffix uses the already-incremented counter minus one. This source order is represented by sequentially running the passout operation and then the tail, provided the integration does not pre-copy state used by the tail.

## Live reads required across the boundary

The tail component accepts `gameMode`, helper argument and entity records as supplied values. Cleanup state/count were originally snapshots; the reviewed fix now uses live Ops getters at each source read site. In the original caller, the mode/argument gate is reached after the drop event and correction path. Only after that gate does the code enumerate the current entity list and inspect type, script name, and state. Cleanup state/count are used later, after the count/spawn/log branch; cleanup's subobject mode is already exposed as a callback and should remain live at its source point. An integration must therefore obtain each of these values at the corresponding tail branch, after the passout operation and any preceding synchronous callbacks. Passing a list or scalar snapshot captured before the passout event can miss synchronous state changes and does not reproduce source ordering.

Similarly, the current tail's rescue total changes only after the staged spawn operation returns, then the statistic update and log follow. Any composition that binds its entity list or cleanup values before spawn must account for spawn-side synchronous mutations before later source reads. The tail intentionally leaves engine spawn, cleanup diagnostics, selection refresh, Player view reset, and final status internals behind callbacks; it does not infer their effects or add RNG.

## Composition status and integration boundary

`VictimTransactions` currently composes pickup and escape using one shared carried pointer and runtime actor/frame records; it has no passout entry point or herodown-tail operation. The passout component uses pickup-compatible carry state but separate `VictimPassoutPlayerState` cached fields, and its `VictimPassoutActorState` must be bound to the same actor identity as the shared runtime record. A future composition should preserve one live Player-frame getter and the external victim `FrameTransform`, while mapping cached Player `+0x3400` and `+0x4c` explicitly rather than aliasing either to frame translation. It should execute the preamble/drop/correction/clear, then resolve the mode/list/cleanup inputs at the later source points and run the tail.

No evidence reviewed here determines the Player frame synchronization point, recurring invocation cadence, or whether the current transaction structures are refreshed by an unmodeled engine path. Those remain explicit unknowns; this review makes no claim that composition alone closes them.
