# MiscEmitter script command handler

## Input

- `FDNYFirefighter.exe` SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Ghidra project program: `FDNYFirefighter.exe`, x86:LE:32, image base `00400000`.
- Address: `004420F0` through `004424C7`.

## Class and vtable evidence

The class descriptor at `004E10E8` contains the string `RCS_MiscEmitter` and factory pointer `00442B40`. Factory `00442B40` allocates the 0x88-byte owner and calls constructor `00442900`. The constructor initializes the embedded ScriptActor at owner `+0x24` and installs vtable `004E1150`. The first two entries are `00441DE0` (update thunk) and `004420F0` (command handler); `004420F0` is therefore ScriptActor vtable slot `+4` for this class. Root entity vtable slot `+0x20` at `004E1178` points to `InitializeMiscEmitterEntity` (`004424D0`), called in the first pass of `ProcessEntitySpawnQueue` (`0049C670`). That initializer registers parsed `A_Name` through `InitializeAndRegisterScriptActor` and dispatches authored `S_Script01` through `S_Script05` bindings.

The constructor reads `C_Type` and creates a contained emitter implementation at owner `+0x78`. The ScriptActor handler reaches that pointer at `this+0x54`.

## Command behavior

The handler compares command names case-insensitively and recognizes `setsize`, `turnon`, `turnoff`, `setrot`, `setpitch`, and `setstrength`.

- `turnon` calls the contained emitter interface's vtable slot `+0x18`.
- `turnoff` calls slot `+0x1C`.
- `setsize`, `setrot`, and `setpitch` copy the corresponding command values into the emitter transform fields, then call `00441FC0` to refresh the root entity.
- `setstrength` passes the parsed value through the contained emitter interface's slot `+0x28`.

Boat A's three `exploder` entities use `C_Type=5`. For this type,
`setsize` parses one scalar, stores it at owner `+0x7c`, and calls `00441FC0`;
that helper applies a uniform scale immediately. The type-5 constructor selects
internal emitter mode 2, whose activation/deactivation handlers toggle active
byte `+0x45`. Internal mode 2 creates four child render objects using the
descriptor pointer at `0x502D40`; the descriptor's dynamic renderer binding and
visible effect remain unresolved. See
[`misc_emitter_type5_exploder_2026-10-20.md`](misc_emitter_type5_exploder_2026-10-20.md)
and [the raw setup instruction excerpt](misc_emitter_type5_setup_fresh_2026-10-20.asm).

The Oil Rig C-stage EVS invokes `turnon` for `steam01`, `steam02`, and `steam03` on line 6. Their `RCS_MiscEmitter` records are in the same `oil_rig_C.bsp` stage and each has `C_Type=3` plus an initial `turnoff` binding. `InitializeMiscEmitterEntity` (`004424D0`), invoked by the map-entity spawn queue at root vtable slot `+0x20`, registers each entity's parsed `A_Name` and dispatches its authored script bindings. This closes the static name-registration and command-handler route. The handoff between Oil Rig stages and resulting visible timing remain open. See [`oil_rig_a_rescue_script_targets.md`](oil_rig_a_rescue_script_targets.md), [fresh initializer output](004424d0_misc_emitter_init_fresh.c), and [spawn queue evidence](0049c670_entity_spawn_queue_fresh.c).

