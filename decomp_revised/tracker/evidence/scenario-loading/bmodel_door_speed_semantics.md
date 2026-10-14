# Retail BModel door speed and reversal semantics

## Source identity

- `FDNYFirefighter.exe`: SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- `Genesis.dll`: SHA-256
  `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
- Authored Subway A script: `inputs/original/Data/Maps/subway_a.evs`;
  Bank script: `inputs/original/Data/Maps/bank.evs`.
- This note uses the checked-in fresh Ghidra exports and retail map/Genesis
  evidence. The corresponding viewer implementation and current validation
  limits are recorded in
  [`bmodel_door_runtime_port.md`](bmodel_door_runtime_port.md).

## Speed command dataflow

At `0043E680` (`DispatchBModelScriptCommand`), BModel-specific commands are
checked after the common ScriptActor command handler at `0047A1F0`. The
`settimescale` branch parses one float argument through `00473D20` and
`0040CC50`, then stores the parsed value at embedded ScriptActor `+0x1B8`
(BModel owner `+0x1DC`, since the embedded ScriptActor is owner `+0x24`).
This is a per-BModel speed multiplier, not a write to the shared frame-delta
global.

The same dispatcher handles `open` and `close`. `open` sets state 3 at
embedded `+0x4C` (owner `+0x70`), unless the object is closing (state 4), in
which case it clears pending commands and queues an `open` reversal. `close`
routes through `0043DA70`; on an ordinary non-opening state it selects state
4, while an opening state queues a close reversal. `openaway` and
`opentowards` call `0043D6A0`; their direction-specific transform selection
is not documented by this note.

`UpdateBModelEntity` at `0043DC60` advances an opening frame as
`frame += *(float *)0x00502B04 * speed` and a closing frame as
`frame -= *(float *)0x00502B04 * speed`. It clamps at the authored frame
endpoints in embedded `+0x1C4/+0x1C8`, then clears state 3/4 on completion.
The method samples each child model's Genesis path and applies it through
`geWorld_SetModelXForm`. Therefore the script's speed changes the rate of
progress over the authored path-time interval; it does not alter the motion
key times or the Genesis interpolation mode.

## Concrete map consequences

Subway A's `subdoor` event is authored as
`touch: [settimescale 4, playsnd SubDoor, open]` and
`untouch: [playsnd SubDoor, close]`. Its 17 BModels each have three animated
children whose exported retail path maxima are 1.0. The first touch therefore
sets the individual BModel speed field to 4 before starting the path; the
untouch event reverses toward the lower endpoint and does not restore speed.
The value remains 4 for subsequent movement unless another command changes
it. The checked-in evidence supports a fourfold multiplier relative to the
same global frame delta, but cannot convert that to seconds without the
runtime update cadence and the value/units of `0x00502B04` at runtime.

Bank's `VaultConsole` issues `tell vault open` after setting
`VaultUnlocked=1`. That resolves via `00475560` -> `00470C50` -> `00470150` to
the named `vault` ScriptActor and its BModel handler at `0043E680`. This route
does not itself issue `settimescale`. `InitializeBModelEntity` (`0043C670`)
sets the embedded BModel speed field to `1.0f` after computing the minimum
child-motion endpoint, so Bank's statically mapped initial speed is 1.0. The
fresh pseudocode is [`0043c670_bmodel_init_fresh.c`](0043c670_bmodel_init_fresh.c).
This value is an exact initialization field, but it still cannot be converted
to visible seconds without the runtime value/units of `0x00502B04` and the
original update cadence.

## Retail motion boundary and runtime gap

Genesis reader/sampler evidence identifies `geMotion_CreateFromFile`
(`1000D570`), `gePath_CreateFromFile` (`1000F580`), and `gePath_Sample`
(`1000E380`). The copied BSP maps use PATH key mode 1: quaternion slerp at
`10004E30` and cubic Hermite translation at `100157F0`. Subway A paths are
nonlooping and span 0..1. The path sampler takes the BModel's current frame;
it does not provide the outer game-loop cadence.

The runtime gap is precise: static evidence establishes the speed write,
frame-step formula, endpoint clamp, event command order, and retail PATH
sampling. It does not establish the actual value/unit of global delta
`0x00502B04`, the frequency/order of `0043DC60` callbacks, or the visible/audio
duration of Bank/Subway doors on the target runtime. The viewer now has
synthetic Bank/Subway interaction checks, but these do not establish live
retail contact or timing parity. No rescue-door implementation should claim
seconds-accurate timing until a retail runtime capture supplies those values.
Relevant source exports: [BModel dispatcher](0043e680_bmodel_dispatch_fresh.c),
[BModel updater](0043dc60_update_bmodel_entity_fresh.c), [Bank route](bank_vault_open_dispatch.md),
[Subway route](subway_a_interaction_path.md), and [retail PATH sampler](../genesis-boundary/genesis_path_tracks_and_port_decoder.md).
