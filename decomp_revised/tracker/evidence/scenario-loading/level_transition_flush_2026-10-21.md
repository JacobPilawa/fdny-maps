# Static level-transition flush order

## Provenance

Fresh headless Ghidra pseudocode for `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, exported
from a disposable copy of the saved project. The primary saved project was not
modified. No game runtime was captured.

## Observed order

`ProcessEverGameUpdate` (`0042AEA0`) clears pending byte `+0x68C` and calls
`ProcessLevelLoadRequest` (`0042AB00`). At `0042ABE4`, the loader calls
`0042A070` after setting the pending record as active and entering load state
2. The flush helper logs `TEverGame: Flushing game...`, then:

1. Walks the FMOD channel range in `004A03E0` using per-channel wrapper
   helpers; their effects are unknown. Separately, it calls `004A08F0` for
   wrapper types 1 and 2; that helper clears matching wrapper-array slots.
2. Calls slot `+0x08` on two queue interfaces at EverGame
   `+0x3C0` and `+0x3BC`.
3. Flushes the four manager lists embedded at EverGame `+0x0C`, `+0x54`,
   `+0x9C`, and `+0xE4` through `00429EF0`, passing the owner service at
   `+0x3C4`. The helper clears list counters and nodes; for entity pointers not
   already in the service's pending-work queue it invokes service slot `+0x04`
   with `(entity, 1)`. Exact class-specific callback effects are not inferred.
4. Calls the `+0xE4` manager's slot `+0x08`, then the nested service at
   `+0x3C4 + 0x40`, slot `+0x04`, with `(0, 0)`.
5. Visits entries in the registry at global service `+0x14` through
   `0049F040`, calls `004944E0` on the object at global `+0x04`, and runs
   `00470ED0` on the script-related global at `0x00502FB0`. The individual
   virtual/helper side effects in these three calls remain only partly mapped.
6. Clears the active level-record pointer at EverGame `+0x688`.

After the flush returns, `ProcessLevelLoadRequest` stores the requested record
back at `+0x688` (`0042AC19`) and calls `00471030` (`0042AC1F`). That routine
stores the current `DIFFICULTY` and `LEVEL` values in the script-variable
store. The loader then initializes the request's resource definition and
queues the selected BSP through the World factory path documented in
[`world_factory_resolution.md`](world_factory_resolution.md).

## Nested world-resource and script-engine effects

A fresh read-only decompilation of the called bodies on the same executable
hash resolves part of step 5. `004944E0` calls `00493EA0` on its argument
plus `0x08`, then clears the argument's dword at `+0x170`. `00493EA0`
processes its active actor array at `+0x7C` (length `+0x80`): for each
non-null actor whose byte `+0x0C` is set, it calls vtable slot 0 with
argument 1, then clears the slot. It logs when a non-null actor has that byte
unset. It also destroys the `geActor` and frees records in the `+0x30` list,
detaches and destroys bitmaps in the `+0x48` list, clears the corresponding
collections, and removes/frees the `geWorld` at `+0x78`. Actor and resource
callbacks may have effects beyond these direct operations.

`00470ED0` clears the ScriptEngine actor array at `+0x7C`, runs the container
cleanup at `+0x94`, clears the array at `+0xB0`, and then registers the
ScriptEngine object again through `00470DD0`. The shared array-clear helper
`0043B6E0` calls its configured per-element callback for occupied entries;
the container helper also releases nested contents. Therefore the wrapper
does not provide a complete account of indirect effects. More specifically,
the flush passes `&DAT_00502FB0`, so ScriptEngine `+0x94` is absolute
`0x00503044`. `StoreScriptVariableValue` (`00470FC0`) passes that same address
to `FindScriptVariableEntry`. Thus `00470ED0` directly clears the script
variable container through `0043EE40`: it empties its entry array through
`0043B6E0`, releases its auxiliary allocation, and zeros container fields
`+0x04..+0x14`. This corrects the initial interpretation of the `+0x94`
field. After this clear, `00471030` writes `DIFFICULTY` and `LEVEL` back into
the same container. The raw entry instructions load the object at
EverGame `+0x6E4`, read its `+0x2C` dword, and pass that integer to
`004443E0`; the helper formats it through a `%i` string before storing the
result as `DIFFICULTY`. The profile-menu selection handler writes the selected
profile pointer to `+0x6E4` through `004239C0`; the static route is documented in
[`profile_selection_pointer_writer_2026-10-21.md`](profile_selection_pointer_writer_2026-10-21.md).
No live menu selection or loaded `.rb` value was captured, so the runtime value
is not established here. `LEVEL` is sourced
from the first string member of the active level record at EverGame `+0x688`.
The table's old entries do not carry through this flush; map scripts may
populate new entries as the next level starts. This does not establish
mission-stat or profile retention: world-resource destruction and
per-element/container callbacks may still have other side effects.
The caller and format-helper instruction anchors are preserved in
[`004443e0_difficulty_format_fresh_2026-10-21.asm`](004443e0_difficulty_format_fresh_2026-10-21.asm).

The fresh callee trace used a disposable headless Ghidra project and the
pinned executable hash above; the primary saved project was not modified.
This is static code evidence, not a runtime transition capture.

## Player recreation and owner work-queue boundary

The same pinned executable's generic spawn path creates one `RCS_PlayerRescue`
from each rescue or transition-stage BSP. `0049C670` calls the registered
factory (`00454080`), which constructs a fresh PlayerRescue root (`00453D50`);
its first initializer pass (`0044D550`) registers the embedded ScriptActor as
`Player`. The hash-checked BSP inventory contains 13 such PlayerRescue records
across rescue and stage maps. See
[`player_rescue_factory_and_spawn_path.md`](player_rescue_factory_and_spawn_path.md).

The PlayerRescue root uses common eligibility mask `0xFFEB`, which includes
the owner `+0x50` manager's filter `2`. The generic registration routine
`00429490` passes the entity pointer directly to the matching manager's slot
`+0x04`; the corresponding manager list at EverGame `+0x0C` is one of the four
lists drained by the flush.

`00429EF0` removes each value from a manager list and searches the owner
service queue at `owner + 0x10` for the same first-dword pointer. If no matching
queue node exists, it calls owner slot `+0x04` with `(entity, 1)`;
`SetEntityFactoryRequestPending` (`0042B1A0`) appends that pointer and flag to
a 0x0C-byte queue node. This is pending-work de-duplication, not a retained-ID
list. A static follow-up resolves the PlayerRescue callback branch: its normal
factory path leaves byte `+0x1D` zero, so `0049C820` calls root slot `+0` with
argument 1. PlayerRescue slot `+0` is `00454060`, which runs cleanup and frees
the root. A later BSP creates a fresh PlayerRescue. This establishes entity
lifetime, not the fate of separate Player fields, loadout sources, mission
statistics, or profile/global state. See
[`linked_stage_player_rescue_lifetime_2026-10-21.md`](linked_stage_player_rescue_lifetime_2026-10-21.md).

## Port implication and limits

Retail performs an in-process runtime flush before queuing the next World.
The shared host currently returns from one `run()` invocation and enters the
sibling package through `runRescueSceneChain`, retaining inherited music and
the selected level duration while reconstructing the rest of its local scene
state. The host creates a fresh `Scripts` instance for each scene, so its
script-variable lifetime resets at the stage boundary too; it currently seeds
`DIFFICULTY` as `1` and replaces `LEVEL` with the scene key. Retail instead
reseeds `LEVEL` from the active level record and derives `DIFFICULTY` from the
selected profile object at EverGame `+0x6E4`, field `+0x2C`. The profile-object
selection path and value are still open, so the host's `DIFFICULTY="1"` seed
has not been compared with retail. This is a coarse stage-transition
approximation, not a parity claim.

The trace narrows direct resource and ScriptEngine cleanup, but does not
establish which profile values, cumulative statistics, Player loadout, or
other game-level state persist across the retail flush. It also does not
establish a successful live transition, exact callback timing, or the
presentation during world replacement. Keep those questions open until they
are traced or captured.

## Fresh export

- [`level_transition_flush_fresh_2026-10-21.c`](level_transition_flush_fresh_2026-10-21.c)
- Existing caller context: [`0042ab00_process_level_load_request_fresh.c`](0042ab00_process_level_load_request_fresh.c)
- Existing owner update context: [`0042aea0_evergame_pending_load_update_fresh.c`](0042aea0_evergame_pending_load_update_fresh.c)
