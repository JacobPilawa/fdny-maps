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
state. This is a coarse stage-transition approximation, not a parity claim.

The trace does not establish which profile values, cumulative statistics,
Player loadout, or other game-level state persist across the retail flush. It
also does not establish a successful live transition, exact callback timing,
or the presentation during world replacement. Keep those questions open until
they are traced or captured.

## Fresh export

- [`level_transition_flush_fresh_2026-10-21.c`](level_transition_flush_fresh_2026-10-21.c)
- Existing caller context: [`0042ab00_process_level_load_request_fresh.c`](0042ab00_process_level_load_request_fresh.c)
- Existing owner update context: [`0042aea0_evergame_pending_load_update_fresh.c`](0042aea0_evergame_pending_load_update_fresh.c)
