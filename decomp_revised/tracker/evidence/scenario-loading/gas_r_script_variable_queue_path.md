# Gas R: script variables, guarded events, and queued commands

## Provenance

This is a static trace of `Data/Maps/gas_r.evs` (SHA-256
`1f83f48debdbfe916c0b5e818f35e0dbf6cb9046a996dfecd75882e27e3fa8c0`) against
`FDNYFirefighter.exe` (SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). No
original-game event was captured live.

## Authored sequence

The relevant lines in the copied EVS file are:

- Line 6 defines `explosion1` as a `touch` event guarded by `once` and
  containing a `que` command list.
- Line 12 sets `GasIsOn` to `1`.
- Line 14 defines `gasoff`; its authored command list sends several
  `setsize` messages, then sets `GasIsOn` to `0`.
- Line 16 defines `firespread` as `tell Fire42 (que [...])`, with four
  `moveto PathPoint1` through `PathPoint4` commands.
- Line 18 defines `explosion2` as `touch: if GasIsOn 1 (once (que [...]))`.
  The list opens `Explodingcar3`, changes FireRescue sizes, invokes quake and
  sound commands, references `(*firespread)`, waits, then displays a hint.

The EVS file parser reads and dispatches statements in file order through
`DispatchScriptActorCommandWithContext` (`0x00475310`). That bridge calls
`RouteParsedScriptActorCommand` (`0x00470150`), which parses the text with
`ParseScriptActorCommandText` (`0x00474180`). The parser first calls
`ExpandScriptVariableReferences` (`0x004730D0`). Therefore, while the later
`set explosion2` statement is being handled, its `*firespread` reference is
resolved from the global variable table populated by the earlier line 16
definition. The stored `explosion2` command text receives the authored
Fire42 queue command in place of that reference.

## BSP script-property bindings

The hash-pinned local Gas R scene export contains these direct joins from BSP
entity properties to the EVS definitions above:

| BSP entity | Actor name | Script field | BSP value | EVS definition |
|---|---|---|---|---|
| 265, RCS_BoundsTrigger | Explotion1_trigger | S_Script01 | *explosion1 | EVS line 6 |
| 280, RCS_BoundsTrigger | unfog2 | S_Script02 | *explosion2 | EVS line 18 |
| 309, RCS_BModel | button1 | S_Script02 | *gasoff | EVS line 14 |

Entity 265 also authors touch-once hint and sound fields; entity 280 also
binds *outside in S_Script01. Entity 309 binds a use-once success hint in
S_Script01. The map's shared EVS loading pass processes set statements before
World entity construction, and both BoundsTrigger and BModel setup pass their
script fields through the shared ScriptActor command bridge. This connects the
authored map properties to the variable-expansion/parser path statically.
The BModel's use selection and event route is mapped in
[gas_r_button1_use_path.md](gas_r_button1_use_path.md); neither contact nor use
activation has been captured live.

## BoundsTrigger contact-to-event route

Gas R entities 265 (`Explotion1_trigger`) and 280 (`unfog2`) are both
`RCS_BoundsTrigger` records with authored `touch` handlers after EVS variable
expansion. The shared BoundsTrigger class path joins those bindings to the
following static route:

1. BoundsTrigger root vtable slot `+0x48` reaches
   `CheckBoundsTriggerContact` (`0x00440CE0`). For each manager-supplied
   candidate, it obtains the candidate world geometry, checks it against the
   trigger's bounds, and appends matching pair keys to root `+0x100`.
2. The embedded ScriptActor updater at root `+0x24` reaches
   `DispatchBoundsTriggerContactEvents` (`0x00440DC0`). It passes the pair
   container to `DispatchScriptActorContactEvents` (`0x00470840`) and then
   rotates/clears that container.
3. The event converter maps new pairs to `touch`, persistent pairs to
   `touching`, and removed pairs to `untouch`. It stores the paired actor name
   as the ScriptActor context and dispatches the matching event's commands.
   For entity 265 this reaches `explosion1`; entity 280's `touch` entries
   include `outside` and `explosion2`.

The class's root `+0x48` contact method is eligible through the owner manager
when its type/mask are admitted, as documented in
[training_touch_dispatch_boundary.md](training_touch_dispatch_boundary.md).
The successful factory/spawn admission route for both Gas R instances is
traced in [rescue_contact_manager_route.md](rescue_contact_manager_route.md).
The external caller/order for the embedded updater, live contact production,
and contact-to-event timing remain open.

## Shared implementation path

### Global `set` and `if`

- Script command dispatch at `0x0047A1F0` routes `set` to
  `HandleScriptSetCommand` (`0x004750E0`), which stores key/value strings in
  the shared table at `DAT_00503044` through `StoreScriptVariableValue`
  (`0x00470FC0`).
- The same dispatcher routes `if` to `ExecuteScriptIfValueMatches`
  (`0x004758F0`). It finds or creates the named global entry (new entries
  default to the string `0`), compares stored and expected strings
  case-insensitively, and dispatches the nested command only when they match.
  Thus the Gas R condition is a string comparison of `GasIsOn` and `1`.
- `ExpandScriptVariableReferences` scans command text for `*name`, extracts
  the name using whitespace, comma, and bracket/parenthesis delimiters, and
  looks it up without creating a missing variable. When found, it replaces the
  reference with the stored value and continues scanning the rebuilt text.
  Its lookup and string-splice path are `FindScriptVariableEntry`
  (`0x0047CF70`) and `AppendStringObject` (`0x004040E0`).

### `once` and `que`

- `once` dispatches through `ExecuteScriptCommandOncePerContext`
  (`0x00475CE0`). It checks a table local to the ScriptActor at `+0x54`.
  The guard key is formatted as `%i_%s` from the current value at ScriptActor
  `+0x78` and the nested command text. A missing key is created and the nested
  command is dispatched; an existing key suppresses it. The table's lifetime
  and reset policy are not established here.
- The common command router `HandleCommonScriptActorCommand`
  (`0x0046F9D0`) handles `que` by calling `QueueScriptActorCommand`
  (`0x0046E9D0`). That handler passes the payload to
  `AppendScriptActorQueueEntry` (`0x0046E750`) on the ScriptActor's queue at
  `+0x30`. The helper stores a retained command reference in the first null
  slot, growing the array if needed.
- The outer `explosion2` `que` contains the expanded Fire42 `que` as a
  nested `tell`. The static trace establishes both queue insertion paths; it
  connects the authored nested sequence to the actor queue. Queue removal,
  stable ordering, one-command pumping, and the wait deadline gate are traced
  below; external updater cadence remains unresolved.

### Queue consumption and wait timing

- PopNextScriptActorQueueEntry removes slot zero, returns that command, and
  calls CompactNullablePointerArray, which preserves the order of the remaining
  non-null entries. QueueScriptActorCommand inserts into the first empty slot,
  so each ScriptActor queue is an ordered command sequence.
- ProcessOneQueuedScriptActorCommand pops at most one command and dispatches it
  through RouteParsedScriptActorCommand. UpdateScriptActorWaitAndQueue invokes
  that pump when the actor is ready. This establishes per-call queue behavior,
  not how often the external engine calls the updater.
- HandleCommonScriptActorCommand parses wait values with ParseFloatFromString.
  Assembly shows the float multiplied by DAT_004E0ED4 (1000.0), added to the
  current clock DAT_00502AFC, rounded, and stored at ScriptActor+0x48; wait
  state is set at +0x4C. The updater pauses while the deadline is in the
  future, clears the state at expiry, then can process one next command in that
  same updater call.
- The Gas R authored delays therefore enter a deadline path as seconds scaled
  by 1000. Runtime cadence, visible delay, and ordering against other actor
  callbacks still require a runtime capture.

## Remaining evidence gaps

- The authored bindings are now identified at BSP entities 265, 280, and 309.
  The shared BoundsTrigger contact-to-event path is statically mapped;
  capture actual contact production and identify the updater's caller, ordering,
  and cadence. Verify the BModel candidate's live selection and use activation.
- Identify the external caller and cadence for `UpdateScriptActorWaitAndQueue`;
  the local per-call queue and wait deadline behavior is statically mapped.
- Capture whether the original game actually fires these events, and compare
  the visible FireRescue, BModel, sound, quake, and hint results.
- Preserve the static sequence as evidence; do not treat it as proof of
  runtime timing or parity.

## Fresh Ghidra evidence

- Variable storage/condition/once/lookup:
  `00470fc0_script_variable_store_fresh.c`,
  `004750e0_script_set_handler_fresh.c`,
  `004758f0_script_if_equals_fresh.c`,
  `00475ce0_script_once_per_context_fresh.c`,
  `00475ce0_script_once_per_context_fresh.asm`,
  `0047cf70_script_variable_lookup_fresh.c`.
- Variable expansion and parsing:
  `004730d0_expand_script_variables_fresh.c`,
  `00474180_parse_script_actor_command_fresh.c`,
  `00470150_script_actor_router_fresh.c`.
- Queue route:
  `0046f9d0_common_script_commands_fresh.c`,
  `0046e9d0_script_queue_handler_fresh.c`,
  `0046e750_script_queue_array_append_fresh.c`,
  `0046e750_script_queue_array_append_fresh.asm`,
  `0046e530_script_queue_pop_fresh.c`,
  `0046e530_script_queue_pop_fresh.asm`,
  `00470a20_pointer_array_compact_fresh.c`,
  `004704a0_process_one_queued_command_fresh.c`,
  `00470740_scriptactor_wait_queue_update_fresh.c`,
  `00470740_scriptactor_wait_queue_update_fresh.asm`,
  `0046f9d0_wait_deadline_assembly_excerpt.asm`, and
  `0040cc50_parse_float_fresh.c`.
- BModel use path:
  `0043db40_bmodel_use_action_fresh.c`,
  `0043db40_bmodel_use_action_fresh.asm`,
  `0043d2b0_bmodel_use_event_fresh.c`,
  `0043d2b0_bmodel_use_event_fresh.asm`,
  `0046e3d0_script_actor_has_event_fresh.c`,
  `0046e3d0_script_actor_has_event_fresh.asm`,
  `004705b0_script_actor_event_dispatch_fresh.c`,
  `004705b0_script_actor_event_dispatch_fresh.asm`,
  `0046e470_nested_script_dispatch_fresh.c`, and
  `0046e470_nested_script_dispatch_fresh.asm`.
- BoundsTrigger contact path:
  `00440ce0_bounds_trigger_contact_check_fresh.c`,
  `00440ce0_bounds_trigger_contact_check_fresh.asm`,
  `00440dc0_bounds_trigger_update_fresh.c`,
  `00440dc0_bounds_trigger_update_fresh.asm`,
  `00470840_script_actor_contact_events_fresh.c`, and
  `00470840_script_actor_contact_events_fresh.asm`.
