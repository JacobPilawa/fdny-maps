# BModel per-frame update and Bank vault motion

## Scope and pinned inputs

- Executable: `FDNYFirefighter.exe`, SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Function body: `0043DC60-0043E297`; next function begins at `0043E2A0`.
- This is static Ghidra and raw-x86 analysis. No callback cadence or visual timing was captured at runtime.

## Virtual slot and object identity

`BModelEntityConstructor` (`0043F340`) installs the table at `004E0DF0` on the embedded ScriptActor at owner `+0x24`. Reading the table's first 32 bytes yields these first two pointers:

| Table offset | Pointer | Role |
| --- | --- | --- |
| `+0x00` | `0043DC60` | BModel per-frame update |
| `+0x04` | `0043E680` | `DispatchBModelScriptCommand` |

The update method receives the embedded ScriptActor in ECX. The containing BModel owner is therefore `this - 0x24`; references to the ScriptActor's `+0x4c` command state correspond to owner `+0x70`.

## Update behavior

The method returns when the BModel has no child models. It updates visibility and environment state, then advances the current animation frame at ScriptActor `+0x1b4` using the global delta at `00502B04` multiplied by speed at `+0x1b8`. Authored endpoints are read at `+0x1c4` and `+0x1c8`.

The update interprets command state 3 as forward/open movement and state 4 as reverse/close movement. It clamps at the corresponding endpoint and clears the state when the movement completes. State 5 wraps through the authored frame range. These states correspond to the `open` and `close` cases previously mapped in `DispatchBModelScriptCommand`; state 5 is an update-loop state.

For each child model, the routine obtains its Genesis motion, samples the path at the current frame, and calls `geWorld_SetModelXForm`. Thus the `open` command is consumed by an animation path rather than directly toggling a door transform.

## Timed command records

The embedded ScriptActor stores a pointer array at `+0x124` and a count at `+0x128`; on the BModel root these are `+0x148` and `+0x14C`. They are separate from root `+0x124`, which is the initialized sound instance. An active record has an active byte at `+0`, a key frame at `+4`, a shared command string object at `+8`, and an opaque context pointer at `+0xc`. The update compares the key frame's distance from the new frame with the frame step from the previous value. When the key frame is closer to the updated frame than the frame step from the previous value, it calls `DispatchScriptActorCommandWithContext` (`00475310`). That helper stores the supplied context at ScriptActor `+0x78` and routes the command through the common `00470150` dispatcher. It then clears the record's active byte, so the entry fires once.

The `addtimecmd` branch in `DispatchBModelScriptCommand` (`0043E680`) passes
the BModel root to `0043E360`. That helper reuses the first inactive 0x10-byte
record or grows the array, then stores the active byte, key frame, retained
command string, and owner context. The `removealltimecmds` branch calls
`0043C260` from `0043EB22`; it clears the active byte on each counted record
but leaves the count, pointer array, allocated records, strings, and contexts
in place. Thus dispatch and removal deactivate entries, and insertion can
reuse their slots. No available evidence links trajectory completion's `drop`
event to `removealltimecmds`.

## External update scheduler search

The local BModel route is established: root slot `+0x14` points to thunk
`0043AFE0`, which adjusts to the ScriptActor and enters shared updater
`00470740`; its slot-zero callback reaches `0043DC60`. No positive external
recurring caller for that route has been identified. The inspected owner
manager pass at `00429520` dispatches four managers through slot zero; its
contact traversal (`0049BD20`) invokes BModel root slot `+0x48` instead. A
hash-pinned literal `[reg+0x14]` scan found 53 candidates but no proven BModel
receiver. These checks do not exclude computed dispatch or unexamined update
loops, so update cadence and ordering relative to contact remain unknown.

## Bank route joined to this method

The Bank console EVS sets `VaultUnlocked` and sends `tell vault open`. BSP entity 441 supplies the BModel name `vault`, its animation model `vault`, and its `VaultDoor` script binding. `InitializeBModelEntity` registers the embedded ScriptActor under that authored name. The named-command route reaches `DispatchBModelScriptCommand` at vtable slot `+4`; the handler changes the open/close state or queues a reversal, and this slot-0 method advances the corresponding animation and timed script entries.

See [Bank vault command dispatch](bank_vault_open_dispatch.md), [fresh pseudocode](0043dc60_update_bmodel_entity_fresh.c), and [raw x86 listing](0043dc60_update_bmodel_entity.asm).

## Remaining checks

- The static endpoint, frame-step, and one-shot entry behavior is mapped. Runtime callback frequency, frame delta behavior, and exact visible Bank-door timing remain open.
- The same static constructor/vtable/update route covers named `open` targets in Bank, Boat A, Bar R, and Gas R. See [the rescue target inventory](bmodel_rescue_targets.md). Runtime activation and map-specific visible effects remain unverified.
- The actual Windows/compatibility runtime and cross-platform port still need parity captures.
