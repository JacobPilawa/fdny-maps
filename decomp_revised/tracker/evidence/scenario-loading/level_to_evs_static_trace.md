# Static trace: level choice to Training EVS

## Provenance

Fresh read-only Ghidra 12.0.4 queries against FDNYFirefighter.exe, SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e, plus the pinned local game data. The full 346-file Data tree and five selected EXE/DLL/config inputs in the user's FireFighter American Hero 2 folder were compared with the project snapshot; all 351 matched byte-for-byte.

## Control-flow map

thegame.lev key training → 0042F500 indexed level choice → 0042AB00 start selected level → 00403E40 appends .bsp to the key → 00471310 changes extension to .evs (mode 1) → 0047A130 → 004799E0.

The script statement dispatcher 0047A1F0 routes exec to 0047A070 (mode 0) and execsetsonly to 0047A130 (mode 1).

The delayed path is on the selected WorldEntity itself: the resolved World factory constructs it through 0049E240, which keeps the selected BSP path at +0x24 and sets counter 4; its vtable entry at 004E446C is 0049E2C0. That method decrements the counter and calls 00471310 mode 0 on the same `this+0x24` path when it reaches zero, then sets app state +0x6F0 to 3. This proves four callback invocations on that object before the full EVS load, but not their frame cadence. The queue pump at 0049C0E0 and generic slot wrappers 0049C3D0/0049C630 are not established as callers; see [worldentity_callback_dispatch_boundary.md](worldentity_callback_dispatch_boundary.md).

## What the functions establish

1. Startup function 0042B4E0 loads data\resdefs\thegame.lev and calls parser 00423160. The parser constructs 0x28-byte level records, with key at offset 0 and title at +4. Each record receives a sequential ordinal at +0x18 and is passed to generic array-slot helper 0048D9D0. The Training row has key training, title Training, resource rescue.rrf, music ebtwist.mp3, time 500, and map coordinates 52,43.
2. Subsystem constructor 0042B860 calls 004230E0 on its embedded object at +0x6AC. The constructor initializes a dynamic collection with its entry-array pointer at +4 and count at +8. Function 004225A0 linearly scans that collection, matching each record's +0x18 ordinal and returning the matching pointer. Raw x86 stack tracing proves parser 00423160 receives this exact collection: startup passes `ESI+0x6AC` at 0042B761; parser saves ECX at 0042317E; after its stack adjustment it reloads that saved value at 004233D8, adds four at 004233DD, and passes the array field at `subsystem+0x6B0` to append helper 0048D9D0 at 004233E2. The earlier Ghidra P-code output did not resolve this stack alias; the raw instruction path does. Full derivation: [catalog_collection_owner_resolution.md](catalog_collection_owner_resolution.md).
3. Function 0042F500 handles selection events 0 and 1 by calling 004225A0 on the subsystem's +0x6AC collection with the UI object's +0x2C value, then passes the returned record pointer directly to 0042AB00. Other branches adjust the selection value. The handler is table-dispatched (data xref at 004DFD18); exact event-code meanings remain unknown. Parser ordinals start at zero and increment in `thegame.lev` order. Selector update code at 0042F2A1–0042F2D4 resolves the input ordinal and copies the matched record's +0x18 ordinal back to the UI object's +0x2C. Child 2 activation calls selector slot 0 at 0042F600, which seeds from the active record or probes valid catalog ordinals; details and raw instruction anchors are in [selector_initial_ordinal_resolution.md](selector_initial_ordinal_resolution.md).
3. Function 0042AB00 stores the incoming record as the active level record and logs its title. It calls 00403E40 with the record and suffix .bsp to form the EVS lookup path, then passes that path to 00471310 with mode 1. It also creates a World spawn request. Its path accessor chain 0042A050 → 00422DE0 → 004040E0 combines `data\\maps\\`, the selected record's first string field (the level key), and `.bsp` through 00403E40; for Training, this yields `data\\maps\\training.bsp`. The request stores the result of a `World` registry lookup and 0x2711, then enters the global spawn service at DAT_00503A78 + 0x24.
4. Function 00471310 normalizes the path extension to .evs and dispatches mode 0 to 0047A070, or nonzero mode to 0047A130. Its only direct callers found in the pinned program are 0042AB00 and 0049E2C0.
5. Wrappers 0047A070 and 0047A130 call the common parser 004799E0 with mode values 0 and 1. The command dispatcher 0047A1F0 routes the statement exec to wrapper 0047A070 and execsetsonly to 0047A130. In mode 1, the parser extracts each non-comment statement's first token, then calls 00401C30 against the literals `set` and `exec`. That helper returns zero for equal strings and nonzero for unequal strings. The parser skips a statement only when both comparisons are unequal, so only `set` and `exec` statements reach ScriptActor dispatch through 00475310. The comparison is byte-wise; no case normalization appears in this helper.
6. The factory route in [world_factory_resolution.md](world_factory_resolution.md) proves that 0049E240 constructs the WorldEntity for the selected-level request. Its method 0049E2C0 therefore operates on that same object and path, not an unrelated startup object. It decrements the counter at +0x2C on each invocation and calls 00471310 in mode 0 on the stored BSP path at +0x24 when the counter reaches zero; then it sets app state +0x6F0 to 3. This establishes four invocations of this method before that call, not four rendered frames.
7. Parser 004799E0 checks Data\Scripts first and falls back to Data\Maps when that candidate is not usable. The initial selected-level call invokes 00471310 in mode 1 on the path derived from `training.bsp`, so it opens `Data\Maps\training.evs`. Mode 1 admits only `set` and `exec` statements; the opening `exec rescue.evs` is dispatched to mode 0 and loads `Data\Scripts\rescue.evs`, while `touch:` event statements are skipped during the initial pass. The WorldEntity's fourth callback later sends the same `.evs` path through mode 0, which parses the complete Training script. These are static file and parser-path facts, not a live-game trace. Event firing and callback cadence remain open.

## Training touch-script registration

The copied Training BSP contains RCS_BoundsTrigger entities with touch commands including tell player escape and hint text. Fresh read-only Ghidra output shows BoundsTrigger setup function 00441040 iterating script fields from its resource properties and passing non-null values to 00475310 on the embedded ScriptActor object at +0x24.

Function 00475310 stores the supplied context pointer at ScriptActor +0x78 and calls 00470150 in mode 0. The common dispatcher substitutes *ME using the ScriptActor context, parses the command, and calls 0046F9D0. If that common handler declines, 00470150 calls the object's vtable slot +4; if that also declines, it logs an unknown command with source label script\scriptactor.cpp:0x7f. BoundsTrigger setup then calls 0046ED70 to check whether its touch-command list contains the exact string tell player escape and stores the result at trigger +0x164.

The BoundsTrigger root-vtable trace now establishes a static contact-to-event route for the trigger class: its root +0x48 method builds overlap pairs at +0x100, and its embedded ScriptActor updater passes them to 00470840 for touch/touching/untouch dispatch. This does not establish the dynamic caller or cadence of the embedded updater, nor a live event firing for a specific entity. The +0x164 membership flag has separate model-flag consumers; it is not the contact-list dispatch mechanism. See [training_touch_dispatch_boundary.md](training_touch_dispatch_boundary.md).

The Player script handler `00451870` has an escape branch through `004513A0` to `00451160`. The final routine requires a carried-victim pointer at Player `+0xD4D`; if present, it updates rescue statistics, transitions the victim through `0045C000`, clears carry state, and refreshes exit-zone state with `004504C0`. Script dispatcher `0047A1F0` sends a `tell` command to `004752A0` → `00472270`, but no edge from there to Player recipient lookup or handler `00451870` is proven. See [training_tell_recipient_boundary.md](training_tell_recipient_boundary.md).

## Confidence and open edges

The direct calls, catalog collection owner, selector initialization, selected-record argument, Data\\Maps\\<key>.bsp construction, World factory and initialization dispatch, mode values, mode-1 `set`/`exec` filter, delayed counter, and BoundsTrigger contact-to-event route are supported by hash-pinned pseudocode, raw instruction bytes, and archived same-hash Ghidra exports. Runtime callback cadence and a live event firing have not been captured from the original game. Do not treat the earlier native startup regression as an original-game trace.

## Evidence files

- Player escape path: 00451870_player_script_command_handler.c, 004513a0_player_escape_prepass.c, 00451160_player_rescue_complete.c.

- Function decompilations: 00401c30_script_token_compare.c, 00403e40_level_path_suffix.c, 004040e0_map_key_path_join.c, 00422de0_selected_level_map_path.c, 00429c00_named_class_registry_lookup.c, 0042b4e0_subsystem_startup.c, 004230e0_level_collection_constructor.c, 004225a0_level_record_lookup.c, 0048d9d0_array_slot_append.c, 0042f500_level_selection_load.c, 0042ab00_level_startup.c, 0049d130_spawn_request_path_setter.c, 0049e240_world_startup_constructor.c, 0049e2c0_delayed_script_load.c, 0049e4d0_world_entity_load.c, 00471310_evs_path_mode_dispatch.c, 0047a070_script_mode0_wrapper.c, 0047a130_script_mode1_wrapper.c, 004799e0_script_file_parser.c, 0047a1f0_script_command_dispatch.c.
- World request and WorldEntity xrefs: selected_world_spawn_xrefs.txt.
- Mode-filter literals and xrefs: script_mode_filter_xrefs.txt.
- Caller/data xrefs: startup_xref_summary.txt.
- Training script-bridge callers: script_dispatch_xrefs.txt.
- Script command router: 0046f9d0_common_script_commands.c.
- Player escape decompilations: 00451870_player_script_command_handler.c, 004513a0_player_escape_prepass.c, 00451160_player_rescue_complete.c.
- Source data and hashes: README.md.
