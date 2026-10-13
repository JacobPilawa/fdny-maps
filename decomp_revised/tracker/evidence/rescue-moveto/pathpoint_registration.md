# MBase PathPoint name registration

Checked 2026-10-13 against the hash-pinned `FDNYFirefighter.exe` and local
MBase BSP export. This closes the constructor-to-name-registry link used by the
`moveto` command. The state-4 update call is mapped separately; its runtime
scheduler and full parity behavior remain open.

## Authored data

`inputs/original/Data/Maps/mbase.evs` contains:

```text
tell mortarfire1 (moveto mortar1 1 10 120)
```

In the local MBase BSP export, entity 149 is `RCS_FireRescue` with
`A_Name=mortarfire1`. Entity 150 is `RCS_PathPoint` with `A_Name=mortar1` and
ten `S_Script01` through `S_Script10` values, all `<null>`.

## Runtime registration trace

Binary: `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

1. `ConfigurePathPointEntity` (`0x00442C90`) reads the PathPoint entity's
   property data, obtains its authored location, and passes the first name
   string to the shared ScriptActor initializer. The MBase PathPoint's first
   authored name property is `A_Name=mortar1`. The initializer then visits the
   ten script fields; for each non-null value it calls the shared script-actor
   command helper.
2. `InitializeAndRegisterScriptActor` (`0x0046E320`) stores the supplied name
   string at the embedded ScriptActor offset `+0x24`, copies two supplied
   32-bit values to `+0x28` and `+0x2C`, and adds the actor to the ScriptEngine.
   The identity field is directly confirmed by `FindPathPointByName`
   (`0x00470C50`), which compares a requested string with each actor's
   `+0x24` name.
3. `ScriptEngineAddActor` (`0x00470DD0`) logs “added actor to ScriptEngine”
   and stores the pointer in the actor array at `+0x7C`, reusing an empty slot
   or growing the array. The array length is at `+0x80`.
4. `SetFireRescuePathPointMove` (`0x00448A90`) calls
   `FindPathPointByName` with the `moveto` target, so the authored `mortar1`
   value reaches the exact registry lookup used by the MBase command.
5. `ScriptEngineRemoveActor` (`0x0046E190`) forwards actor destruction to the
   registry removal helper, which clears the matching slot.

The previous work archive contains two address-matched pseudocode copies for
`0x0046E320` under the player-hose and prop-model investigations. They agree
with the newly retrieved pinned decompilation for the name assignment and
ScriptEngine insertion. They are useful corroboration, not original source.

Fresh pseudocode is stored in this directory as
`00442C90_fresh.c`, `0046E320_fresh.c`, `00470DD0_fresh.c`, and
`0046E190_fresh.c`. Four address-specific reviews were added. Ghidra labels and
plate comments were saved to the local project database.

## Open questions

- The two values copied to actor offsets `+0x28/+0x2C` have not been assigned
  semantic names; do not assume these two words fully describe the authored
  three-dimensional location.
- The `S_Script01` through `S_Script10` command behavior remains to be traced
  from the shared script-actor helper and the relevant event callbacks.
- The runtime scheduler/cadence and exact order of state-4 updates remain open;
  see [`fire_rescue_state4_update.md`](fire_rescue_state4_update.md).
