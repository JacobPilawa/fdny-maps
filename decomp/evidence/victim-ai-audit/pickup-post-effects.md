# Training pickup post-effects

Read-only Ghidra 12.0.4 follow-up for the Player message handler on the exact `evidence/original/FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6fca72fe5f7064ca8122fda7d41e7f6d2fc3e`). Queries used the preserved project with `-noanalysis -readOnly`; no live game was attached.

## Exact handler order

Player message handler `00450440` first checks whether Player `+0x3534` is empty. If so, it stores the victim pointer there, then runs the following calls in order:

1. `0045BEF0(victim)` writes victim state `+0x120=5`, calls `00481140(victim+0x170, false)` to store actor flags word 0, and dispatches the child `pickup` event through `004705B0`.
2. `0044FCE0(1)` walks the global entity list. For type 3 it calls `00446EC0(entity,1)`, which stores byte `entity+0x12D=1` and forwards 1 to `00497DE0(entity+0xD0)`. For type 8 it calls `00440F50(entity,1)`; that helper acts only when `entity+0x164` is nonzero and stores byte `entity+0x165=1`. It calls `geWorld_ModelSetFlags(model,0)` only on the false-parameter path, so this true call does not establish a model-flag write.
3. `00490E60(Player+0x198, value)` stores the value at subobject `+0x4C`, clears `+0x50`, and sets byte `+0x48=1`. Ghidra's expression in `00450440` reduces to 20 when Player field `+0x18C==4`, and 50 otherwise. The raw expression is `((int(Player+0x18C != 4) - 1) & 0xFFFFFFE2) + 0x32`; the branch values follow from the integer mask, not the local variable names.
4. `004571A0(Player+0x5F4, (int*)1, null)` takes its parameter-1-equals-1 branch: it calls `0047DC90(this+0x964,1)`, writes the current clock to `this+0xA88`, samples the list at `this+0xA8C` through `00484940`, advances the related state with `0047EDF0`, and looks up status key `RC_VictPick` at owner `+0x3540` through `004897C0(..., true)`. These exact fields and helper calls are known; a stronger UI interpretation is not assigned here.
5. `00484260(Player+0x5D8)` refreshes map-zone membership. Its loop clears prior member arrays, then examines global entities of type 2, skipping state 5 and 6, and appends the others to the zone selected from each entity's frame position. The carried victim has already entered state 5 when this runs.
6. Player `+0x5C4` receives the current global clock.

This order is local to the already-delivered message handler. It does not identify when the Player movement callback runs or when message `0x7532` is produced. Training's `pickup` handlers run inside the preceding event dispatch; see [`training-pickup-flow.md`](training-pickup-flow.md).

## Native boundary

`src/victim_rescue.h` preserves the earlier handler transaction through the synchronous event and exposes one post-event callback. The actions above are not implemented by that callback yet. Keeping it explicit avoids treating an event-order stub as a completed carrying, UI, trigger, or zone system. The Training victim raw actor flags are initialized to `0x400` and set to zero on pickup; actor construction evidence is in [`actor-render-flags.md`](actor-render-flags.md).

## Ghidra query provenance

- `DecompileFunctions.java`: `00450440`, `0045BEF0`, `0044FCE0`, `00446EC0`, `00440F50`, `00490E60`, `004571A0`, `00484260`, `00484940`, `0047EDF0`, `004897C0`.
- The saved `004705B0` event-dispatch route and the zone manager's surrounding type/offset mapping were checked in the existing victim rescue-flow and training-zone audits. No claim is made about unresolved caller cadence.
