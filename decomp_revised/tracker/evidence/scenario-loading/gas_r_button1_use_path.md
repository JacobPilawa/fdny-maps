# Gas R button1 use path

## Provenance

Static trace against `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` and
Gas R `gas_r.bsp` / `gas_r.evs`. No original-game input or event was captured.
This trace reuses the archived BModel use-selection audit, then cross-checks
the relevant function boundaries and instructions in the active Ghidra project.

## Authored button and script

BSP entity 309 is an `RCS_BModel` named `button1`. It has child animation model
`button1`, `E_Reach=20`, `G_Liftable=0`, and two script properties:

- `S_Script01 = use: once (hint 8 (Good job you shut off the gas main.))`
- `S_Script02 = *gasoff`

Gas R EVS line 14 defines `gasoff` as a `use` event whose `cmd` list sends six
`setsize` commands to named FireRescue actors and then sets `GasIsOn` to `0`.
The parser expands `*gasoff` while processing the BModel script property, after
the EVS `set gasoff` statement has populated the shared variable table. The
earlier `GasIsOn` variable is initialized to `1` on line 12.

## Static interaction route

1. `InitializeBModelEntity` (`0x0043C670`) registers the BModel's authored
   name and sends its script properties through the shared ScriptActor
   dispatch path. After binding, it checks for the `use` event with
   `HasScriptActorEvent` (`0x0046E3D0`). When a use event exists and use mode is
   still zero, it sets the BModel use mode at owner `+0x258` to `2`. The
   `G_Liftable=0` field means the liftable initializer does not replace that
   generic mode.
2. The BModel interaction vtable is installed at owner `+0xA0`. Its slot 0,
   `GetBModelUseAction` (`0x0043DB40`), supplies the axis-box query interface
   at owner `+0x260` for any nonzero use mode. Mode 1 asks the Player lift
   gate for an action; other nonzero modes return action code `1000`. The
   shared Player scan (`0x00459350`) calls the slot with one pointer to its
   retained query-interface slot. The corrected `__thiscall` prototype has
   one stack argument, `query_interface_out`; the earlier Ghidra signature
   carried an unused second stack argument. The meaning of action `1000` is
   not identified by this trace.
3. The shared contact manager dispatches each active entity's root slot
   `+0x48` over the type lists selected by its category mask. BModel root type
   4 has category mask `0x2`, so it checks type-1 candidates through
   `CheckBModelContactCandidate` (`0x0043B620`). A matching candidate geometry
   is tested against the BModel bounds and an overlap pair is added to root
   `+0x160`. Gas R entity 309 authors `E_Reach=20`, `F_HitPoints=2`, and
   `G_Liftable=0`. Its user-data gates allow the conditional 48-unit reach
   override, but that branch also depends on the maximum child-motion endpoint;
   the embedded BSP MOTIONS payload is not decoded, so the effective reach is
   still 20-or-48 unresolved.
4. The general BModel updater (`0x0043DC60`) converts contact-pair changes to
   ScriptActor contact events, tracks contact state, and adds or removes the
   interaction subobject at owner `+0xA0` when use mode is nonzero. The shared
   Player selection path queries each active candidate, retains eligible
   candidates, and dispatches its saved action through slot `+0x04`. This is a
   class-level static route; the dynamic updater schedule and a live Gas R
   candidate list are not known.
5. BModel slot `+0x04`, `DispatchBModelUseEvent` (`0x0043D2B0`), calls the
   Player lift/carry transition only for mode 1. It then dispatches the `use`
   event on the embedded ScriptActor at owner `+0x24` for all modes. Gas R's
   mode 2 therefore reaches the script event without entering the lift path.
6. `DispatchScriptActorEvent` (`0x004705B0`) looks up the named event and
   routes each associated command through `RouteParsedScriptActorCommand`
   (`0x00470150`). The `cmd` handler processes its child commands by calling
   `DispatchNestedScriptActorCommand` (`0x0046E470`) for each one. Thus the
   statically authored handler path reaches the FireRescue size commands and
   the final `GasIsOn = 0` assignment. `once` wraps the hint command; the
   independent `gasoff` event binding is outside that guard.

This joins Gas R's map entity, EVS macro expansion, generic BModel interaction
selection, and shared event-command dispatcher. It does not prove the button
appears in the player's live candidate list, that the original game dispatches
the event on a particular input, or that the resulting fire changes match a
port.

## Fresh Ghidra evidence

- `0043c670_bmodel_init_fresh.c` and `0043c670_bmodel_init.asm`
- `0043db40_bmodel_use_action_fresh.c` and `.asm`
- `0043d2b0_bmodel_use_event_fresh.c` and `.asm`
- `0046e3d0_script_actor_has_event_fresh.c` and `.asm`
- `004705b0_script_actor_event_dispatch_fresh.c` and `.asm`
- `0046e470_nested_script_dispatch_fresh.c` and `.asm`
- Shared queue, variable, parser, and wait behavior:
  [Gas R script-variable and queue trace](gas_r_script_variable_queue_path.md)
- Reused static selection groundwork:
  `reference/previous-work/decomp/evidence/training-liftable-audit/bmodel-selection-pass-audit.md`

## Remaining evidence gaps

- Trace the exact Gas R contact candidate production and prove when the Player
  receives `button1` in its ordered interaction array.
- Identify the recurring callers and order for `UpdateBModelEntity` and the
  Player selection updater.
- Capture the original game accepting use on entity 309, the two `use` bindings
  firing, the six fire-size updates, and the gas-state result.
- Compare those effects and timing with the cross-platform port.
