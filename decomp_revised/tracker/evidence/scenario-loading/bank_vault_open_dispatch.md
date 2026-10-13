# Bank vault command to BModel handler

## Inputs

- `FDNYFirefighter.exe` SHA-256:
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- `bank.bsp` SHA-256: `02a4b351279a9a4a1242050d3a7622f8d10149201646f890f310729a189d27b7`.
- `bank.evs` SHA-256: `5b392ff7e8470f44f3e4130b99483835181abe34588ecab5a5795dec9198a020`.
- The BSP entity parse used `tools/gbsp_v15.py` directly on the copied input;
  it did not modify the retail data.

## Authored Bank sequence

`inputs/original/Data/Maps/bank.evs` defines:

1. `VaultUnlocked` starts at `0`.
2. `VaultDoor` handles touch by showing the locked-vault hint while the value
   remains `0`.
3. `VaultConsole` handles touch with a use hint. Its `use` branch checks for
   `VaultUnlocked == 0`, shows the unlock hint, changes the variable to `1`,
   then issues `tell vault open`.

The ordered entity records in `bank.bsp` connect those script variables to
concrete objects:

| BSP entity | Relevant authored fields |
| --- | --- |
| 441, `RCS_BModel` | `A_Name=vault`, `C_AnimModel0=vault`, `S_Script01=*VaultDoor`, location `(-1211, 0, -1928)` |
| 488, `RCS_BModel` | child animation model `vaultunlock`, reach `10`, touch hint, `S_Script02=*VaultConsole`, location `(-2230, 240, -259)` |
| 278, `%Model%` | model name `vault` |
| 308, `%Model%` | model name `vaultunlock` |

This supplies a concrete map fixture for the generic named-actor route: the
console updates EVS state and sends the command to the named vault BModel.

## BModel identity and command handler

The fresh raw x86 boundary at `0043C670` ends at `0043D2A4`, followed by the
next function at `0043D2B0`. Ghidra's recovered function,
`InitializeBModelEntity`, identifies `sharedents\\BModelEnt.cpp` in its log
paths. When it has model children, it copies the name from the parsed BModel
definition at `this+0xEC`, passes the embedded ScriptActor at `this+0x24` and
that name to `InitializeAndRegisterScriptActor` (`0046E320`), then initializes
the model bounds and per-child state. It passes non-null script properties
through the shared script setup path. The function's full fresh pseudocode is
[`0043c670_bmodel_init_fresh.c`](0043c670_bmodel_init_fresh.c).

`BModelEntityConstructor` (`0043F340`) initializes a ScriptActor subobject at
`this+0x24` and installs its BModel-specific vtable pointer at `004E0DF0`.
Pointer `004E0DF4`, slot `+4`, is `DispatchBModelScriptCommand` (`0043E680`).
This is the handler slot called by shared ScriptActor dispatch `00470150` after
the common command handler declines. Fresh pseudocode for the constructor and
handler is in [`0043f340_bmodel_constructor_fresh.c`](0043f340_bmodel_constructor_fresh.c)
and [`0043e680_bmodel_dispatch_fresh.c`](0043e680_bmodel_dispatch_fresh.c).

The BModel handler recognizes `open`, `close`, `openaway`, `opentowards`,
`loop`, and several time-command controls. For `open`, it iterates the parsed
command-associated entries. If the BModel state at root offset `+0x70` is `4`,
it clears the pending command list through `0046E450` and appends `open` through
`0046E9D0`/`0046E750`. Otherwise, it writes state `3` at root `+0x70`. The
corresponding `close` branch writes state `4` unless the current state is `3`,
in which case it clears the pending list and appends `close`. The per-frame
vtable slot at `0043DC60` advances states 3/4 across the authored frame
endpoints, applies Genesis model transforms, and dispatches one-shot timed
script entries. See [BModel update evidence](bmodel_update.md). Runtime
callback cadence and the exact visible Bank-door timing remain open.

## Route and limits

On subsequent entity updates, vtable slot 0 reaches `UpdateBModelEntity`
(`0043DC60`), which advances the authored animation and dispatches timed
script entries. Full update evidence is in [bmodel_update.md](bmodel_update.md).

The static path is:

```text
VaultConsole use event
  -> EVS checks VaultUnlocked == 0
  -> EVS sets VaultUnlocked = 1
  -> tell vault open
  -> 00475560 resolves "vault" through 00470C50
  -> 00470150 dispatches "open"
  -> BModel ScriptActor vtable slot +4 at 004E0DF4
  -> DispatchBModelScriptCommand at 0043E680
  -> BModel ScriptActor vtable slot 0 at 004E0DF0 on a later update
  -> UpdateBModelEntity at 0043DC60
```

The actor registry is shared across ScriptActor classes and returns the first
case-insensitive name match. `InitializeBModelEntity` proves that the authored
name is registered for a BModel with model children. The script actor handler
address and command semantics are static code evidence; they do not establish
that Bank is playable in the port or prove the door's visible motion timing.

This trace is reusable across the rescue maps: `boat_a` has a named `explodeb`
command, while Subway A binds `open`/`close` as self-commands on `subdoor`.
See [`named_script_actor_dispatch.md`](named_script_actor_dispatch.md) for the
shared route and the corrected cross-map fixtures.
