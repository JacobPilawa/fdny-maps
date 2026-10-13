# Training catalog and scenario asset anchors

These are static observations from the copied retail data and the hash-pinned `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). No game was launched in this pass.

## Code evidence

- `FUN_0042B4E0` has the only data reference to the executable string `data\\resdefs\\thegame.lev` at `0x004DF924` and calls `FUN_00423160` during subsystem startup. The parser's error path labels its source `main\\LevelManager.cpp:0x57`.
- `FUN_004230E0` initializes the level-choice collection embedded at app/subsystem offset +0x6AC. `FUN_004225A0` searches its entries (array pointer at collection +4, count at +8) for a record whose ordinal at +0x18 matches the requested value.
- `FUN_00423160` parses the catalog's `menumusic:`, `level:`, `title:`, `rrf:`, `music:`, `time:`, and `mapcoords:` records. It builds a 0x28-byte per-level record and assigns sequential ordinals at +0x18; raw x86 stack tracing proves it appends those records to the exact +0x6AC collection used by the selector. Observed fields include the level key at +0, title at +4, `data\\resdefs\\`-prefixed resource definition at +8, music at +0xC, time at +0x10, enabled flag at +0x14, and two map coordinates at +0x20/+0x24. See [catalog_collection_owner_resolution.md](catalog_collection_owner_resolution.md).
- `FUN_0042F500` handles the level-choice event. For event values 0 or 1, it calls `FUN_004225A0` on the +0x6AC collection with the UI object's +0x2C ordinal, then passes the returned record pointer directly to `FUN_0042AB00`. The adjacent selector update routine copies the matched record's +0x18 ordinal back to the UI object's +0x2C. Scene child activation seeds the initial ordinal through `0042F600`; see [selector_initial_ordinal_resolution.md](selector_initial_ordinal_resolution.md).
- `FUN_00495210` is the BSP-to-`geWorld_Create` path. The selected level key now traces through `FUN_00422DE0` and `FUN_004040E0` to `data\\maps\\<key>.bsp`; the level loader submits a `World` request with that path to the global spawn service.
- `FUN_0049E4D0` is a WorldEntity method in the table at `0x004E4460`; its own log strings identify `ROCWorldEntity` and `ROC\\ROC AI\\Entity\\WorldEntity.cpp:0x36`. Its dispatch from the selected-level request is resolved in [world_factory_resolution.md](world_factory_resolution.md): the `World` descriptor maps through factory `0049E300` to this WorldEntity, whose initialization invokes `0049E4D0`. That method passes its `this+0x24` path to `FUN_00495210`, then enumerates the resulting world's models/entities and stages spawn records through the same global service.

The parser, collection constructor, ordinal lookup, array helper, callers, callees, path xref, and adjacent key-string bytes are saved in this directory. These are Ghidra outputs and data references, not recovered source.

## Script dispatch evidence

Common script dispatcher 0046F9D0 handles base control and message/queue commands; commands it does not handle return to the ScriptActor layer for derived fallback.

The copied Training BSP contains RCS_BoundsTrigger entities with touch commands including tell player escape and hint text. Fresh function 00441040 shows BoundsTrigger setup passing its non-null script fields through 00475310 on the script subobject at +0x24. The bridge, DispatchScriptActorCommandWithContext (00475310), stores a context pointer at script-subobject +0x78 and calls 00470150. Its callers include EVS parsing, BoundsTrigger and PathPoint setup, BModel initialization, and BModel per-frame timed events; see [fresh pseudocode](00475310_scriptactor_context_dispatch_fresh.c).

Function 00470150 substitutes *ME using the ScriptActor context, sends the parsed command through common dispatcher 0046F9D0, and calls the object's vtable slot +4 if the common handler declines. If the fallback also declines, it logs an unknown command with source label script\scriptactor.cpp:0x7f. After registering fields, 00441040 asks 0046ED70 whether the touch-command list contains the exact string tell player escape and stores the boolean at trigger offset +0x164.

This proves a static Training asset-to-ScriptActor dispatch path and a command-presence check. It does not prove that a particular entity is instantiated, that its touch condition becomes true, or what later code means by the flag at +0x164.

Raw vtable and instruction inspection maps the BoundsTrigger-specific path: root slot +0x48 (`00440CE0`) checks overlap and appends pairs at root +0x100; root slot +0x14 reaches the embedded ScriptActor updater, whose slot 0 (`00440DC0`) passes that list to `00470840` for `touch`/`touching`/`untouch` dispatch and then rotates and clears it. Shared manager registration checks root mask `0xFFEB` against filter `0x20`; BoundsTrigger type 8 is therefore eligible for the manager list that dispatches root slot +0x48. Per-instance list membership remains unverified. The updater's dynamic caller, its order relative to contact production, and runtime cadence remain open. The `+0x164` flag is a separate later model-flag path: `0044FCE0` calls `00440F50` during carry/reset, and `004504C0` calls it for eligible final-exit triggers. See [training_touch_dispatch_boundary.md](training_touch_dispatch_boundary.md).

The selector's initial ordinal is also traced: scene child 2 opens through `0042D3E0`, which invokes selector slot 0 at `0042F600`; that method seeds from the active record or probes for a valid catalog ordinal. Full details are in [selector_initial_ordinal_resolution.md](selector_initial_ordinal_resolution.md).

Player handler `00451870` contains the `escape` branch through `004513A0` to `00451160`, which completes rescue only when the Player carries a victim. The `tell` branch reaches `00475560`: its first value is the target actor name, its second value is the command, and lookup-success dispatch enters `00470150`. Bank's `vault` target is tied to the BModel-specific `open` handler at `0043E680` and per-frame animation/timed-event update at `0043DC60`; the shared route also covers 10 named `open` targets across Bank, Boat A, Bar R, and Gas R. See [bank_vault_open_dispatch.md](bank_vault_open_dispatch.md), [bmodel_update.md](bmodel_update.md), and the [rescue target inventory](bmodel_rescue_targets.md). For Training, the particular registry object named `player` and its order relative to the Player handler remain unresolved. The separate `004752A0` → `00472270` chain belongs to `help`. See [training_tell_recipient_boundary.md](training_tell_recipient_boundary.md).

## Training data anchors

The copied manifest identifies Training as `level: training`, title `Training`, resource definition `data\\resdefs\\rescue.rrf`, music `ebtwist.mp3`, time `500`, and map coordinates `(52, 43)`. Relevant copied-file hashes from `inputs/original/manifest.csv`:

| Input | SHA-256 |
|---|---|
| [`Data/ResDefs/thegame.lev`](../../../inputs/original/Data/ResDefs/thegame.lev) | `ee6819873cd540e67613774d848506561e2861442b37ef301e27273805d558c6` |
| [`Data/Maps/training.bsp`](../../../inputs/original/Data/Maps/training.bsp) | `32986212fa875764ca3d5c1630e54ad6315abaea70983203601192cec603a963` |
| [`Data/Maps/training.brf`](../../../inputs/original/Data/Maps/training.brf) | `6f21b924b8eb5113fa15e4eec658c3123657ad54840d5dc202c8be3e53f02348` |
| [`Data/Maps/training.bmp`](../../../inputs/original/Data/Maps/training.bmp) | `5e93dcb10d7ddb92be73e2abbc1c3cede97bcb906323f25e6d6228107676dc1c` |
| [`Data/Maps/training.evs`](../../../inputs/original/Data/Maps/training.evs) | `0c01a1f8d88ef7a994be382d223fd5302bd29301496362613fe963332b336061` |
| [`Data/Scripts/rescue.evs`](../../../inputs/original/Data/Scripts/rescue.evs) | `4b6d986fb9eb04f593956d7dcf3b8290ada0858a777dfaa29e204f1f4c0195c7` |

The `.evs` files are CRLF ASCII. The selected-level loader first reads `Data\\Maps\\training.evs` in mode 1. That mode passes only `set` and `exec` statements to ScriptActor dispatch, so the opening `exec rescue.evs` is followed while `touch:` event text is skipped in this pass. The nested `Data\\Scripts\\rescue.evs` load runs in mode 0 and includes `tools.evs`, `bonusitems.evs`, and `doors.evs`. After four invocations of the selected WorldEntity's delayed callback, the same path is loaded in mode 0, allowing the full Training EVS statements to be parsed. These are static parser and call-path findings; they do not establish callback cadence or a live event firing.

## Next trace

Identify the dynamic caller of the BoundsTrigger embedded ScriptActor updater and the WorldEntity slot `+0x14` caller. Resolve the concrete registry object and dispatch order for Training's `tell player escape`. The selector initializer, parser-to-selector collection link, World factory path, four-invocation delayed mode-0 load, and mode-1 filter are mapped. See [worldentity_callback_dispatch_boundary.md](worldentity_callback_dispatch_boundary.md), [training_tell_recipient_boundary.md](training_tell_recipient_boundary.md), and [level_to_evs_static_trace.md](level_to_evs_static_trace.md).
