# Named ScriptActor command dispatch

## Purpose

This is a shared command-routing path used by scenario-authored `tell` commands.
It applies across rescue missions and other catalog scenarios; it does not by
itself establish what a named actor's command does.

## Evidence basis

- Executable: `FDNYFirefighter.exe`, SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Ghidra MCP analysis of the open 32-bit x86 program, saved after renaming
  `00470C50` to `FindScriptActorByName` and `00475560` to
  `DispatchScriptCommandToNamedActor`.
- Address-specific pseudocode and raw instruction bytes are described below.

## Recovered route

1. `0047A1F0` compares the command token with `tell`. The matching branch at
   `0047A5D4` obtains two parsed values and calls `00475560`. The adjacent
   `action` route also uses that helper (`0047A45C`).
2. `DispatchScriptCommandToNamedActor` (`00475560`) passes its first value to
   `FindScriptActorByName` (`00470C50`) with global registry
   `DAT_00502FB0`. If lookup succeeds, it sends the second value to
   `00470150` on the returned ScriptActor, together with the source context.
3. `FindScriptActorByName` scans the registry's actor-pointer array at `+0x7C`
   for the count at `+0x80`. It compares the requested value case-insensitively
   with each registered ScriptActor name at actor `+0x24`, returning the first
   match.
4. `InitializeAndRegisterScriptActor` (`0046E320`) stores a supplied identity
   at ScriptActor `+0x24`, stores two additional supplied words at `+0x28` and
   `+0x2C`, then registers the actor through `ScriptEngineAddActor` (`00470DD0`).
   The registry is therefore shared across actor types. Its use by FireRescue
   PathPoint movement (`00448A90`), the `tell` helper (`00475560`), script
   binding (`004753D0`), and Oil Rig's `InitializeMiscEmitterEntity` (`004424D0`)
   proves this path is used by BModels, PathPoints, and MiscEmitters.
5. `ScriptActor` dispatch (`00470150`) first runs the common command handler
   (`0046F9D0`). If that handler declines, machine code at `0047038E` loads the
   parsed command pointer, `00470392` pushes it, and `00470395` calls the
   target's vtable slot `+4`; the return value determines whether the command
   was handled. This is the per-class behavior seam for commands such as
   `open`, `escape`, `setsize`, and `turnon`.

## Cross-scenario fixtures

The same name-then-command route is exercised by authored map data including:

| Scenario | Authored target command | Evidence file |
| --- | --- | --- |
| Training | `tell player escape` | `inputs/original/Data/Maps/training.evs` |
| Bank | `tell vault open` | `inputs/original/Data/Maps/bank.evs` |
| Boat A | `tell explodeb open` | `inputs/original/Data/Maps/boat_a.evs` |
| Bar R | `tell ClubRafterFall open` | [bmodel_rescue_targets.md](bmodel_rescue_targets.md) |
| Gas R | `tell PumpExplosion/ExplodingCar1/2/3 open` | [bmodel_rescue_targets.md](bmodel_rescue_targets.md) |
| Subway A | `set subdoor (cmd [touch: [..., open], untouch: [..., close]])` | `inputs/original/Data/Maps/subway_a.evs` |

Subway A installs a command list on the current `subdoor` ScriptActor. Its
`open` and `close` tokens are self-commands reached through the common
`00470150` dispatcher and the current actor's derived vtable handler; they are
not a named `tell` to `subdoor`.

Oil Rig B's catwalks and Oil Rig C's stairs are stage-local BModels, activated
by triggers in their owning BSPs. The three steam targets are Oil Rig C
`RCS_MiscEmitter` entities whose initializer registers their authored names
and dispatches their bindings; their `turnon` handler is also mapped. Oil Rig
exits request the next stage A→B→C→D through `finalexitloadmap`; the static
pending-level path reaches the World loader. The pending map-name field at EverGame+0x660 is mapped, and raw x86 stack dataflow confirms the checked command key is the key stored there. Runtime WorldEntity handoff and event timing remain open; see [the pending-key dataflow note](finalexitloadmap_pending_key_dataflow_2026-10-20.md).
The gas/bar variable and
event sequences, every rescue mission, all 12 catalog entries, and all 17 BSP
files remain in scope.

## Player identity boundary

The rescue-map recipient is statically identified as `RCS_PlayerRescue`:

- `InitializePlayerRescueEntity` (`0044D550`) passes the embedded ScriptActor
  at entity `+0x140` (`param_1 + 0x50` words) to
  `InitializeAndRegisterScriptActor` with the exact name `Player`.
- The Player constructor (`00453D50`) installs the ScriptActor vtable at object
  `+0x140`; its address point at `004E1C40` has `00451870` in slot `+0x04`, the
  Player script-command handler.
- The distinct `PlayerHose` initializer (`00466D60`) also registers `Player`.
  Hash-checked inventory of all 17 retail BSPs finds exactly one
  `RCS_PlayerRescue` and no `RCS_PlayerHose` in each rescue-map/stage BSP; the
  four hose-map BSPs contain one `RCS_PlayerHose` and no PlayerRescue. There
  are no exact `Player` epair values in the BSPs. See the generated
  [map/class inventory](../../rescue_exit_trigger_inventory.md).
- Remaining direct references to the `Player` string are file/tree readers,
  save serialization, a Player-name lookup for a hint command, or constructor
  data; they do not add another literal-name registration. Among the callers
  of `InitializeAndRegisterScriptActor`, the two literal-name initializers are
  PlayerRescue and PlayerHose.

This resolves the authored rescue maps' static successful-path class-to-handler
route through the WorldEntity factory and spawn queue. It does not observe a
live map load, runtime failure behavior, registry insertion order, or the frame
on which a script command is delivered.

## What this establishes and what remains open

**Established:** `tell`'s first value is the actor-name lookup key; its second
value is the command sent to the first case-insensitive name match. A successful
lookup reaches that object's ScriptActor dispatcher, which passes unhandled
commands to the target-specific vtable slot `+4` with the parsed command
pointer.

**Open:** runtime factory admission, registry insertion order and command
delivery timing; the map-specific effects of Boat A's `explodeb` and Subway A's
`subdoor` self-commands; runtime completion of Oil Rig's `finalexitloadmap`.
Bank, Boat A, Bar R, Gas R, and the statically resolved Oil Rig stage targets
have their named BModel routes mapped. Map-specific runtime effects, cadence,
and visual timing remain open; static routing is not evidence of port parity.

## Next bounded trace

The direct named BModel `open` routes in Bank, Boat A, Bar R, Gas R, and Oil Rig
are joined to their authored child models in [bmodel_rescue_targets.md](bmodel_rescue_targets.md).
Next, verify Oil Rig's runtime stage handoff, then trace Subway A's `subdoor`
self-command route through its class handler. Obtain a live startup/event trace
for factory admission and command delivery across the rescue maps; Training
remains in scope alongside all seven campaign rescues.
