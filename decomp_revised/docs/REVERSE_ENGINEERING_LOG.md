# Reverse-engineering activity log

This file records material findings, the current handoff point, and delegated work so the same searches are not repeated. Raw evidence is linked from each subsystem note.

## 2026-10-12 — idle update and FireRescue category path

**Input:** `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`; Ghidra 12.0.4; read-only Ghidra MCP queries.

**Mapped:**

- `FUN_00499410` enters the app's two callback passes only when the Windows message queue is idle.
- `FUN_0049A310` dispatches active app-list entries at vtable `+0x74`, removes deferred entries, then calls the owner at virtual `+0x60`.
- The owner vtable at `0x004DF958` points `+0x60` to the six-byte thunk `0x0042BC60`. Fresh raw bytes show it loads `this+0xB4` and jumps to `FUN_0042AEA0`.
- On the ordinary scene path, `FUN_0042AEA0` calls `FUN_00429520`; afterward it performs additional message/resource and scene-state work. The early path guarded by owner byte `+0x64C` skips the ordinary update.
- `FUN_00429520` calls subsystem manager methods at `+0x08`, `+0x50`, `+0x98`, and `+0xE0`, then staged-entity, message, sound, and staged-object services in that order.
- The `+0xE0` manager is initialized by `FUN_0049BF10`; its first vtable entry is `0x0049BD20`. Fresh memory/disassembly and the same-hash legacy decompilation show removal handling followed by nine category lists and member calls at vtable `+0x48`. FireRescue's category bit and primary `+0x48` callback are recorded in the old evidence; this identifies the contact/influence route, not the visual Think scheduler.
- The primary FireRescue table's `+0x14` thunk at `0x0043AFE0` adjusts `this` by `0x24` and jumps to `FUN_00470740`. That wrapper's secondary slot-0 call is the local route toward `0x00449FE0`, but no recurring caller for the wrapper is known.
- `FUN_00499E60` is the post-pass and invokes remaining app-list entries at vtable `+0x70`.

This establishes pre-list callbacks → game-owner update → post-list callbacks on an idle iteration. It does not establish a complete Player/FireRescue/victim schedule or any frame rate. The raw queries and fresh function pseudocode are under `tracker/evidence/runtime-loop/`; details are in [`RUNTIME_MAP.md`](RUNTIME_MAP.md).

**Boundary discipline:** the fresh Ghidra function inventory has no function object at `0x0042BC60`, `0x0043AFE0`, `0x00449FE0`, `0x004483A0`, or `0x0049BD20`. The relevant addresses are thunks or manually bounded code slices in the prior project. Their old exports are kept as legacy evidence and are not counted as fresh function reviews. Their bytes/vtable roles were checked against the pinned current EXE.

## Parallel work completed

Two user-requested GPT-6-Luna reviews ran independently and made no workspace edits:

1. **Genesis source reuse research — complete.** Found `RealityFactory/Genesis3D`, a modified source tree based on Genesis3D 1.1. It is a valuable reference for API behavior and algorithms but not proven to match FDNY's engine. No FDNY-specific recovery surfaced. License/provenance is a gate before incorporating source. Findings are in [`ENGINE_SOURCE_REUSE.md`](ENGINE_SOURCE_REUSE.md).
2. **Legacy Training audit — complete.** Confirmed the old prototype has meaningful source-mapped components and selected composed transactions, but not a source-accurate full Training run or the original recurring actor schedule. Existing prototype documentation calls the scene playable on macOS; this is legacy evidence and was not rebuilt or checked in this clean project. The audit identified FireRescue scheduling and engine-boundary gaps; it did not modify files.

Do not repeat a broad literal scan for FireRescue's `0x00449FE0` caller. The prior project records bounded fixed-offset and decoded-indirect-call audits with no resolved caller. Continue from the vtable/registration or engine-callback boundary, or design a bounded runtime trace.

## 2026-10-12 — Genesis API boundary and frame callback path

**Input:** same pinned EXE SHA-256 and Ghidra 12.0.4 project. Read-only Ghidra MCP calls were made to the project's local headless server. A startup attempt first found a project lock and could not bind under the workspace sandbox; a loopback check with the allowed elevated read-only command confirmed the already running v5.8.0-headless service, so no second server or project database was created.

**Mapped:**

- The EXE calls `geVFile_OpenNewSystem` from four functions. The BSP loader at `FUN_00495210` opens a path using DOS/read-only arguments, passes the returned VFile handle to `geWorld_Create`, closes it, and registers the world with the engine. The raw disassembly confirms the VFile handle is pushed as the world-create argument despite the incomplete imported prototype.
- A second caller, `FUN_00492950`, creates an empty world and registers it with the engine. Bitmap and optional alpha/texture loaders use VFile and Genesis bitmap APIs.
- All six `geWorld_Collision` callers are saved. The uses range from FireRescue's floor placement to zero-extent/vertical/radius-derived probes and a larger multi-step interaction/movement routine whose role remains provisional.
- Three `geActor_SetPose` callers are saved. They pass motion, time, and transform data; one also writes actor scale. This maps the interface usage, not when the engine schedules these operations.
- In the ordinary path, `FUN_0042AEA0` calls the frame dispatcher `FUN_00494670` after the manager pass. That dispatcher drains a selected world's pending callback array through `FUN_004943E0`, prepares active render objects, brackets virtual draw callbacks with `geEngine_BeginFrame`/`geEngine_EndFrame`, then clears the one-shot pre-frame list. The list's producer and element vtable remain unknown; it is a candidate route for actor updates, not a proven FireRescue Think schedule.
- The `geEngine_RenderWorld` caller at `FUN_00492C40` passes the selected world and a camera. The second BeginFrame caller, `FUN_0042D1B0`, draws a bitmap between the frame calls; its UI/loading role remains provisional.

Fresh Ghidra outputs and caller lists are in [`tracker/evidence/genesis-boundary/`](../tracker/evidence/genesis-boundary/README.md). The selected API map is [`tracker/genesis_api_crosswalk.csv`](../tracker/genesis_api_crosswalk.csv). The public Genesis3D 1.1-based VFile and Actor headers clarify the reference API, but do not prove an exact match to FDNY's DLL. No public engine source was copied into this project.

The tracker records 35 reviewed EXE functions over two snapshots at this point in the log. It shows review activity separately from source mapping or port parity. No build or game test was run in this pass.

## Current next work

1. Verify BoundsTrigger manager admission and identify the dynamic caller of its embedded ScriptActor updater, including its order relative to contact production.
2. Resolve the WorldEntity slot `+0x14` caller and exact `tell player escape` recipient path.
3. Continue the Genesis API map at the rendering, camera, actor, and sound boundaries.
4. Reassess the original-game launch wrapper and disc/resource check before planning a bounded Wine runtime capture.

There is no new cross-platform playable build yet. No native build or test was run in this pass.

## 2026-10-12 — guarded input/camera path and callback-list false match

**Input:** same hash-pinned EXE; Ghidra 12.0.4; read-only MCP queries.

`FUN_0042D8C0` is called only from `FUN_0042AEA0`, under the `param_1[0x64C] != 0` branch that returns before the ordinary manager/render path. It reads mouse bindings, updates and clamps cursor coordinates, calls `FUN_00494670`, conditionally updates actor flags/lighting/pose and derives a camera adjustment from `BIP01 R TOE0`, then dispatches a cursor/button input sink. The meaning of the branch guard remains unknown. The pseudocode and caller reference are saved under `tracker/evidence/runtime-loop/`.

The apparent `+0x148/+0x14C` writer `FUN_0043E360` is not established as the `FUN_004943E0` pre-frame-list producer. Its caller's `addtimecmd` script branch adds 16-byte records containing an active flag, time value, reference-counted command string, and owner field. `FUN_004943E0` instead dereferences array entries as vtable-bearing objects and calls slot `+0x08`. `FUN_0043F340` initializes the timed-command list in that object's layout, but there is no object-identity link to the selected render context. The offset-only candidate and its format mismatch are recorded in `tracker/evidence/runtime-loop/0043e360_time_command_list_false_match.md`, with fresh pseudocode and xrefs. The actual pre-frame-list producer remains unresolved.

The user's alternate raw folder has matching hashes for EXE, Genesis, FMOD, and D3DDrv. The Wine wrapper's retail copies match too. Small READMEs record those facts and classify wrapper overlays separately; the full wrapper and user state were not copied. No runtime capture or build was run during this pass.

## 2026-10-12 — Training catalog and event-script anchors

**Input:** hash-pinned EXE and files already copied into `inputs/original/`; read-only Ghidra MCP queries and local text/asset inspection.

The only Ghidra data reference to `data\\resdefs\\thegame.lev` currently found is in `FUN_0042B4E0`. That startup routine calls `FUN_00423160`; the parser's error path labels `main\\LevelManager.cpp:0x57`. Fresh pseudocode shows parsing of `menumusic:`, `level:`, `title:`, `rrf:`, `music:`, `time:`, and `mapcoords:` keys into 0x28-byte level records. Training's copied record resolves to `rescue.rrf`, `ebtwist.mp3`, time `500`, and map coordinates `(52, 43)`.

`FUN_0049E4D0` is referenced from a vtable slot and logs `ROCWorldEntity` / `WorldEntity.cpp`. It passes its `+0x24` path to `FUN_00495210`, which loads a BSP world, then iterates world entities and stages spawn records. This is the strongest current world-load route, but the actual caller is virtual and unobserved; the catalog entry has not yet been connected to this object.

The copied Training `.evs` and BSP data contain script-related material: `training.evs` begins with `exec rescue.evs` and includes hints and a touch condition; the BSP contains entity properties including `exec doors.evs` and `equip axe`; `rescue.evs` pulls in shared script files. The code path from selecting the catalog record to loading the BSP/EVS sources and interpreting those commands is not yet connected. Hashes and evidence are in `tracker/evidence/scenario-loading/`.


## 2026-10-12 — level selection to EVS static map

**Input:** FDNYFirefighter.exe SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`; Ghidra 12.0.4; read-only queries to the project-local GhidraMCP service. The service reported the pinned executable at `inputs/original/binaries/FDNYFirefighter.exe`.

**Mapped:**

- The user's additional raw folder matches the pinned EXE, Genesis.dll, fmod.dll, D3DDrv.dll, and the Training level file, BSP, and EVS byte-for-byte. No replacement copy was needed.
- `FUN_0042F500` indexes a level-choice collection and passes the selected record to `FUN_0042AB00` for event values 0 or 1. Its call is indirect through a data table at `0x004DFD18`; exact event meanings and list identity remain open.
- `FUN_00403E40` appends a suffix to the record's first string field. Since `FUN_00423160` stores the level key there, `training` produces `training.bsp` in `FUN_0042AB00`; `FUN_00471310` then changes the extension to `.evs` and routes mode 1 through `FUN_0047A130`.
- `FUN_00471310` has two direct callers: `FUN_0042AB00` and `FUN_0049E2C0`. Mode 0 goes to `FUN_0047A070`; both wrappers call `FUN_004799E0`. Script statement `exec` dispatches to mode 0 and `execsetsonly` to mode 1 in `FUN_0047A1F0`.
- Constructor `FUN_0049E240` initializes a four-count; `FUN_0049E2C0` decrements per invocation and calls the EVS loader with mode 0 at zero. This is four callbacks, not a measured frame cadence. Its relation to the selected Training world remains open.

Fresh pseudocode and xrefs are in `tracker/evidence/scenario-loading/level_to_evs_static_trace.md` and neighboring C files. The previous Training startup gate is a native regression, not an original-game capture. No Wine run, build, or test was performed in this mapping batch.

**Tracker:** initial module inventory milestone M1 is complete. The function-review ledger and time-series dashboard now show 48 reviewed EXE functions; these are address-specific review records, not a source-recovery percentage. The tracker/dashboard were rebuilt once after this coherent batch.

## 2026-10-12 — Level selection collection and raw input check

Read-only Ghidra queries decompiled the level-choice collection constructor 004230E0, ordinal lookup 004225A0, and generic array-slot helper 0048D9D0. The selection handler 0042F500 uses 004225A0 on the subsystem's +0x6AC collection and passes the returned record directly to 0042AB00 for events 0 and 1. The catalog parser separately creates 0x28-byte records, assigns sequential ordinals at +0x18, and appends through 0048D9D0. This establishes selection-to-loader flow; the local array owner at the parser append call still needs to be bound directly to the +0x6AC collection.

The raw Desktop folder was compared with the pinned project inputs: all 346 Data files plus the five selected EXE/DLL/config files matched by SHA-256 (351/351, no differences). No duplicate raw assets were copied. The focused static map now has 51 fresh function reviews. The progress dashboard was regenerated once for this three-function batch; its counts remain activity measures, not source recovery or port completion.

## 2026-10-12 — Training trigger script dispatch

Fresh read-only Ghidra output maps BoundsTrigger setup at 00441040 through the ScriptActor bridge 00475310 into command dispatch 00470150. The dispatcher substitutes *ME, tries common command handling, then calls a type-specific vtable fallback if needed. Training BSP strings include RCS_BoundsTrigger entries whose touch commands include tell player escape; setup checks for that exact command with 0046ED70 and stores the result at +0x164. This is static registration/classification evidence only; live trigger timing and the flag's later meaning are unknown.

The evidence now covers 55 reviewed EXE functions. The progress chart was refreshed once for this four-function event-dispatch batch. The active Ghidra project is still the pinned retail EXE; no original-game runtime capture, build, or test was performed.

## 2026-10-12 — Player escape behavior

Fresh Ghidra pseudocode for 00451870, 004513A0, and 00451160 shows the Player-side escape branch and its effect. The completion routine proceeds only when Player +0xD4D holds a carried-victim pointer; it increments the Rescues statistic, updates rescue-time values, transitions the victim through 0045C000, clears carry state, and refreshes exit-zone state with 004504C0. The earlier command audit links Training's tell player escape text to this branch, but the common forwarding edge and a live trigger remain unverified.

The review ledger now contains 58 functions. The generated dashboard remains at 55 until the next larger batch, following the user's request for less frequent HTML updates.

Fresh pseudocode for common command router 0046F9D0 shows base ScriptActor control and message handling before derived fallback. The review ledger is now at 59 functions; the generated dashboard remains at 55 for the next larger batch.

**Next:** verify catalog-list identity, connect the selected-level path to the WorldEntity/BSP spawn method, then trace EVS event binding and original callback timing. The user requested a pause if either ordinary usage window reaches 1% remaining; the latest limit check was 13% of the five-hour window used and 93% of the weekly window used. No usage-reset credits were used.

## 2026-10-12 — EVS mode-1 statement filter

Fresh read-only Ghidra evidence resolves the mode-1 parser filter. The parser extracts a statement token and compares it against `set` and `exec` through 00401C30; that helper returns zero for equal byte strings and nonzero otherwise. Non-comment statements reach ScriptActor dispatch only when the token matches either accepted word. Other statements are skipped. This documents parser behavior only; it does not establish which `set` effects occur during a live Training run.

The function-review ledger now contains 60 entries. The generated dashboard remains at 55 until a later larger batch, per the user's request for less frequent HTML updates.

## 2026-10-12 — Selected-level BSP spawn request

Fresh read-only Ghidra output shows the selected-level loader building `data\\maps\\<level-key>.bsp` through 0042A050 → 00422DE0 → 004040E0 → 00403E40. For Training, the World spawn request therefore carries `data\\maps\\training.bsp`; the loader resolves the name `World` through a registry lookup and submits the request to the shared spawn service. WorldEntity method 0049E4D0 reads its `+0x24` path, passes it to BSP loader 00495210, and submits the loaded map's entity requests through that same service. This strongly connects Training selection to the BSP load/spawn path, although the registry/factory dispatch to the exact WorldEntity vtable method is still unproven.

The review ledger now contains 64 functions. The dashboard remains at 55 pending a larger batch. Next: resolve the World request's factory/vtable dispatch and parser collection owner, then trace Training event binding and callback timing. The latest usage check showed 14% used in the five-hour window and 94% used in the weekly window; no reset credits were used.

## 2026-10-12 — World factory dispatch resolved

The selected-level route is now linked through the `World` registry entry to the concrete WorldEntity BSP loader. The queue drain calls request factory virtual slot +8; the hash-pinned descriptor vtable at `004E44B0` maps that slot to factory `0049E300`. That factory constructs a WorldEntity through `0049E240`, which stores the selected BSP path at +0x24. The object's initialization slot +0x20 reaches `0049E4D0`, which loads that path through `00495210`. Training's path is `data\\maps\\training.bsp`. The exact trace and raw table bytes are recorded in [`world_factory_resolution.md`](../tracker/evidence/scenario-loading/world_factory_resolution.md).

At the World-factory checkpoint the review ledger contained 66 functions and M2 still had catalog ownership, Training event binding, and callback cadence open. The following entry resolves catalog ownership and the selector ordinal path; the remaining open items are the initial menu ordinal writer, Training event binding, and callback cadence. This remains a static successful-path result, not a live game trace.

## 2026-10-12 — Catalog owner and selector ordinal resolved

The catalog parser and the indexed level-choice collection are the same object. Startup passes `ESI+0x6AC` into parser 00423160. The parser saves incoming ECX at 0042317E; after a stack shift, it reloads that saved pointer at 004233D8, adds four, and calls array helper 0048D9D0 at 004233E2. Thus the append receiver is array field `subsystem+0x6B0` inside the collection initialized at `subsystem+0x6AC`; selector lookup 004225A0 reads that same array. This closes the earlier P-code-only ownership uncertainty.

The parser assigns sequential zero-based ordinals to `level:` records. Selector lookup uses the UI object's `+0x2C` value, and the adjacent menu update routine writes the selected record's `+0x18` ordinal back into that field. The path that seeds the menu's first value remains open. Full evidence is in [`catalog_collection_owner_resolution.md`](../tracker/evidence/scenario-loading/catalog_collection_owner_resolution.md).

The function-review count remains 66; this checkpoint improves the dataflow map rather than adding a reviewed function. The live dashboard reads tracker CSVs and updates while open through its local-server launcher; opening the HTML file directly shows a static snapshot. At this checkpoint M2's remaining questions were the initial selection writer, Training event binding, and runtime callback cadence.

## 2026-10-12 — Training touch flag consumers and dispatch boundary

A direct-call sweep against the pinned EXE found the only two callers of `00440F50`: `0044FD4E` in the carry/reset pass `0044FCE0`, and `00450984` in final-exit processing `004504C0`. The helper reads BoundsTrigger `+0x164`, writes mode to `+0x165`, and can update Genesis model flags. These callers explain consumers of the touch-command membership flag, but neither produces contact events. The embedded ScriptActor vtable at `004E1088` dispatches derived unhandled commands to `00441A60`, which handles `makezone`; this is also not evidence for touch-event execution.

The generic BModel contact path is separately mapped: it creates current contact pairs and later converts them into `touch`, `touching`, and `untouch` actions. The dynamic updater caller, producer/consumer timing, and any connection to the BoundsTrigger root callback remain open. The root vtable begins at `004E1090`; inspecting its event methods is now the next bounded trace. Evidence and same-hash provenance are in [`training_touch_dispatch_boundary.md`](../tracker/evidence/scenario-loading/training_touch_dispatch_boundary.md).

The review ledger remains at 66 functions, so no function-count checkpoint was added. The live dashboard reads tracker CSVs while open through its local-server launcher; this work changed the milestone CSV and evidence text only. No game launch, build, or test was run.

## 2026-10-12 — Selector initialization and BoundsTrigger event route resolved statically

Raw instruction tracing closed the selector's initial ordinal path. Scene child 2 is constructed by `0042DEC0`; when the scene switcher `0042D3E0` activates it, the selector's slot-0 callback `0042F600(this,1)` either resolves the active catalog record's ordinal through `0042F280` or probes candidate ordinals until one resolves. The constructor itself does not initialize `+0x2C`. See [`selector_initial_ordinal_resolution.md`](../tracker/evidence/scenario-loading/selector_initial_ordinal_resolution.md).

The BoundsTrigger-specific contact/event path is also mapped from raw instructions and vtable bytes. Root slot `+0x48` (`00440CE0`) detects overlap and fills its `+0x100` contact list. Root slot `+0x14` reaches the embedded ScriptActor updater, whose slot 0 (`00440DC0`) passes the list to `00470840` for `touch`, `touching`, and `untouch` dispatch, then rotates and clears it. This corrects the preceding note: the trigger path is distinct from the BModel pair producer but does reach the same generic event dispatcher. Manager admission, dynamic updater caller, and producer/consumer timing remain open. The separate `+0x164`/`00440F50` model-flag consumers are documented apart from event delivery in [`training_touch_dispatch_boundary.md`](../tracker/evidence/scenario-loading/training_touch_dispatch_boundary.md).

These traces used raw instructions; they do not add fresh Ghidra function-review entries. The review ledger remains at 66, so no function-count checkpoint was added. No dashboard HTML was regenerated, and no game launch, build, or test was run.

## 2026-10-12 — WorldEntity callback scheduling boundary refined

The WorldEntity method `0049E2C0` is a one-shot delayed script load: its constructor initializes the counter to four, and the method loads the full EVS file on the fourth invocation, marks itself done, and sets app state `+0x6F0`. Raw constructor bytes confirm the root vptr `004E4458`; vtable slot `+0x14` points to the method. The interval between calls is not established.

The prior queue-pump candidate `0049C0E0` invokes member slot `+4` for queue work and removal checks, not WorldEntity slot `+0x14`. `0049C3D0` is a generic `+0x14` virtual wrapper and `0049C630` is a counter/slot-`+4` limiter, but neither has a proven WorldEntity receiver or registration edge. The app-owner manager and staged-entity service also do not provide the missing slot `+0x14` call. Full evidence and candidate limits are in [`worldentity_callback_dispatch_boundary.md`](../tracker/evidence/scenario-loading/worldentity_callback_dispatch_boundary.md).

The established statement is four method invocations before one mode-0 EVS load; there is no verified frame, time, or “120 updates” cadence. This is static analysis only, with no change to the 66-function review count and no dashboard HTML update.

## 2026-10-12 — BoundsTrigger updater caller audit and driver source lead

A bounded follow-up of saved xrefs and virtual-dispatch candidates found no concrete recurring caller for BoundsTrigger root slot `+0x14` at vtable word `004E10A4`. The direct manager pass dispatches slot `+0x48` for contact production; entity staging uses `+0x20/+0x24`; ScriptEngine's actor list supports named lookup and event dispatch but is not an update list in the inspected paths. The computed-slot possibility remains open. This preserves the key runtime uncertainty: contact-list draining cannot yet be ordered relative to contact production. Evidence is in [`training_touch_dispatch_boundary.md`](../tracker/evidence/scenario-loading/training_touch_dispatch_boundary.md).

The public Genesis3D source tree also contains `Engine/Drivers/{D3D7xDrv,D3DDrv,OpenGl,WireFrame}`. These directories are useful for comparing the legacy renderer boundary, but do not identify a matching implementation for FDNY's `D3DDrv.dll`; no external source was copied. The links and reuse limits are recorded in [`ENGINE_SOURCE_REUSE.md`](ENGINE_SOURCE_REUSE.md) and [`REFERENCES.md`](REFERENCES.md).

No new Ghidra function review was added, so the ledger remains at 66. The dashboard HTML was untouched; the CSV-backed live page continues to refresh when opened through its launcher. No game launch or build was run.

Correction (2026-10-12): the preceding branch attribution was wrong. `0047A305` → `004752A0` → `00472270` belongs to `help`. The literal `tell` comparison at `0047A4DC` reaches `00475560` at `0047A5D4`; that function performs ScriptEngine actor lookup at `00470C50` and dispatches through `00470150` on lookup success. The argument roles and edge to Player handler `00451870` remain open. The corrected raw-instruction trace is in [`training_tell_recipient_boundary.md`](../tracker/evidence/scenario-loading/training_tell_recipient_boundary.md). No reviewed-function count was added.

## 2026-10-12 — D3DDrv initialization boundary mapped

The hash-pinned `D3DDrv.dll` has one export, `DriverHook` at RVA `0x6300`. Raw x86 disassembly shows it writes `0x1001F040` through its argument, initializes globals at `0x100422C8`, `0x100422CC`, `0x1001F0EC`, and `0x1001F050`, passes the status text `D3DDrv:  No error.` through a formatting helper, and returns 1. This maps the plugin boundary at the export, but not the caller or the internal table's layout and slot behavior. The evidence, exact hash, and raw instructions are in [`d3ddrv_driverhook_boundary.md`](../tracker/evidence/driver-boundary/d3ddrv_driverhook_boundary.md). It adds no EXE function review and makes no public-source identity claim. The milestone CSV now records this boundary; the dashboard HTML was not changed.

The follow-up whole-program scan for WorldEntity slot `+0x14` has 53 indirect call/jump candidates. The apparent main-frame candidate resolves to the input handler vtable, the generic `0049C3D0` wrapper has no saved incoming refs, and two nearby world-load hits lack a proven WorldEntity receiver. No recurring caller or cadence is established; the one-shot after four method invocations remains the limit of current evidence. Details are in [`worldentity_callback_dispatch_boundary.md`](../tracker/evidence/scenario-loading/worldentity_callback_dispatch_boundary.md).

## 2026-10-12 — Driver loading and public source reuse bounded

Raw import/string inspection of the pinned `Genesis.dll` found `LoadLibraryA` and `GetProcAddress`, plus strings `EnumSubDrivers` and `DriverHook` around a version-mismatch message. The EXE and Genesis import tables do not statically import `D3DDrv.dll`; a dynamic discovery path is plausible, but no loader callsite, export invocation, or output-table consumer was proven by this pass. The focused local trace is [`driverhook_caller_table_trace.md`](../tracker/evidence/driver-boundary/driverhook_caller_table_trace.md).

The public RealityFactory/Genesis3D tree is a modified Genesis3D 1.1 reference with separate D3DDrv and D3D7xDrv directories. The D3DDrv source listing was not inspectable in this check, so its hook signature and table layout cannot yet be compared with the pinned DLL. The public tree is not an established binary match or drop-in port source; license terms also require a deliberate review before reuse. Details and primary links are in [`public_driver_comparison.md`](../tracker/evidence/driver-boundary/public_driver_comparison.md). No external code was copied. The milestone CSV now points to the targeted Ghidra xref query; no function-review checkpoint was added because the 66-entry EXE ledger did not change, and the dashboard HTML remains untouched.

## 2026-10-12 — Genesis renderer callbacks traced through copied records

A byte-level absolute-pointer scan confirmed code references to `DriverHook` and `EnumSubDrivers`. In Genesis `FUN_10023950`, the engine loads the candidate DLL, resolves `DriverHook`, calls it with an output-pointer address, checks returned dwords `+4 == 101` and `+8 == 1`, and invokes table method `+0x14` with callback `0x100238A0`. That callback copies a subdriver record into engine state and invokes table method `+0x18` with callback `0x10023770`; the second callback copies per-mode records. The engine copies enumerated strings/records before `FreeLibrary`. Record strides and caps (12 subdrivers, 32 modes per subdriver, strings shorter than 512 bytes) are preserved in [`driverhook_caller_table_trace.md`](../tracker/evidence/driver-boundary/driverhook_caller_table_trace.md). The callback roles are behavior-based provisional names; later selected-driver loading and table lifetime remain open.

This local mapping explains the dynamic renderer discovery boundary but does not identify the exact driver source. The current official Genesis3D site states that Genesis3D 1.1 may be developed without licensing obligations; the community source snapshot still has historical license text and later contributions, so exact source provenance should be recorded. Search also found Styx3D, but its own README describes a Jet3D rework (the separate Genesis3D 2.x line) and lists SDL3/software-driver work as incomplete. These external sources are linked and classified in [`REFERENCES.md`](REFERENCES.md); no code was copied. `FUN_10023950` and the callback code were inspected via raw instructions, so the EXE function-review time series remains at 66 and no dashboard HTML change was needed.

## 2026-10-12 — Selected renderer initialization and scheduler candidate disposition

The second Genesis `DriverHook` path is now traced through selected-driver initialization. `FUN_10025E00` reloads the selected DLL, resolves `DriverHook`, validates table fields `+4 == 101` and `+8 == 1`, writes callbacks at `+0xB0/+0xB4`, and calls table method `+0x20`. For D3DDrv, this slot points to `0x10005DD0`, which reads a context, may obtain dimensions through `GetClientRect`, stores selected values in globals, and returns success or failure. Genesis retains the loaded handle after success. Structure field names, exact method signatures, and runtime renderer choice remain open. Evidence is in [`selected_driver_initialization_trace.md`](../tracker/evidence/driver-boundary/selected_driver_initialization_trace.md).

The fixed-slot lead at `0046269D` was rejected as evidence for the BoundsTrigger scheduler. It is a conditional self-vtable call outside a child-array loop, and no receiver or runtime edge ties it to ScriptActor or BoundsTrigger. The recurring updater caller and its timing are still unresolved. See [`fixed_slot_candidate_0046269d_rejection.md`](../tracker/evidence/scenario-loading/fixed_slot_candidate_0046269d_rejection.md).

No EXE function-review entry was added (66 remains). The dashboard HTML was not updated; no game launch or build was run.

## 2026-10-12 — EXE renderer selection callsite mapped

The EXE calls imported `geEngine_SetDriverAndMode` from `FUN_004998B0` at `0x00499BE2`; the function is reached during startup/window initialization. The import's correct IAT VA is `0x004DC1E4` (the nearby `0x004DC3B8` candidate is USER32 `LoadCursorA`). Genesis export `0x100263E0` calls selected-driver helper `FUN_10025E00`; the post-init table `+0xA8` call is now identified as fog-settings dispatch based on the six fields also written by `geEngine_SetFogEnable`. The individual settings and downstream D3DDrv method remain unnamed. The caller/wrapper trace is detailed in [`selected_driver_initialization_trace.md`](../tracker/evidence/driver-boundary/selected_driver_initialization_trace.md). The 66-entry review ledger and dashboard HTML are unchanged; no build or game launch was performed at that checkpoint.

## 2026-10-12 — Mac-first rescue slice and full map inventory

The project plan now sequences a Mac playable rescue slice first and keeps Windows/Linux as final targets. The Training reconstruction is copied into `port/spikes/training-native-prototype/` as a mechanics workbench, not the main game target. Its CMake build succeeds with the installed Mac toolchain, and its copied Training asset path resolves; SDL reports no displays when the current shell tries to create a window, so the workbench is not counted as running.

The new `tools/inventory_level_assets.py` produces `tracker/level_map_inventory.csv` from the hash-pinned local Data folder. It records 12 selectable catalog entries and all 17 BSP files, including five BSPs not listed directly in the catalog. It identifies case mismatches for `city_h`/`City_h.bsp` and `oil_rig_a`/`oil_rig_A.bsp`, and same-key EVS files are absent for `dock_h`, `city_h`, and `oil_rig_h`; the raw Desktop comparison confirms those files are absent in both copies. The user prefers to prioritize rescue missions. `mbase`, the first rescue catalog entry after Training, is the first candidate pending map/entity dependency extraction.

The tell branch and selected-driver fog table dispatch are now corrected in the runtime map. No EXE function review was added (66 remains), so the dashboard HTML and reviewed-function time series were left unchanged.

## 2026-10-12 — all-map scene extraction and scope lock

**Scope:** every rescue mission, every other selectable catalog map, and all mechanics needed by those scenarios. Training is a validation fixture, not a port boundary. MBase remains the first new rescue integration candidate.

**Mapped and extracted:** the shared GBSP v15 exporter now emits typed render geometry, collision structures, ordered entity epairs, and indexed base textures for all 17 local BSP files. Reports retain each input BSP hash and chunk inventory. One case-insensitive texture filename collision in `const_h` exposed a macOS filesystem issue; export v2 now adds stable texture indices/files to mesh groups and produces distinct paths. The native mesh loader accepts that path while retaining the old Training asset fallback. `tracker/level_map_inventory.csv` now reports all 17 current exports.

**Genesis source reuse:** a second read-only source comparison confirmed the public Genesis3D 1.1-derived `DriverHook` table uses version 100/3 while the pinned FDNY `Genesis.dll` checks for 101/1. Public code is useful for API/layout/algorithm references, not a drop-in engine or driver. Keep tracing FDNY-specific behavior and replace the graphics/platform boundary for the cross-platform port. Details and source links are in [`ENGINE_SOURCE_REUSE.md`](ENGINE_SOURCE_REUSE.md) and [`public_driver_comparison.md`](../tracker/evidence/driver-boundary/public_driver_comparison.md).

**Still open:** no rescue map is loaded by the C++ runtime yet. Implement the shared catalog/`SceneData` loader, compare script/entity/actor/motion contracts across all rescue maps, resolve resources, and feed the existing player/collision/render/fire/victim systems. The current scope and schema are in [`SCENE_DATA_CONTRACT.md`](SCENE_DATA_CONTRACT.md); per-map evidence is in [`level_map_inventory.csv`](../tracker/level_map_inventory.csv).

## 2026-10-12 — reused ACT decoder and mapped rescue actor assets

The previous `FDNY_trackdown` archive contained an ACT parser, which was overlooked in the first reuse pass. The parser from `decomp/tools/extract_act.py` was copied into the clean project as `tools/extract_act.py`; the project now has a batch adapter that reads the pinned `victims.srf`/`rescue.rrf` chain, emits decoded review assets only under ignored `.local/actor_exports/`, and records compact source-hash and format coverage in [`actor_asset_inventory.csv`](../tracker/actor_asset_inventory.csv). No decoded retail meshes or textures are committed.

All six types used by the 71 map-authored victim entities decoded successfully, along with the separately defined `herodown` actor: 7 ACTs, 32 motions, 17 embedded textures. All sampled actor tracks report interpolation mode 1 for translation and rotation, within the modes currently accepted by shared `ActorPose`. The inventory retains material slot names, opaque bitmap IDs, and bitmap-entry names for the next resource-binding pass. `subway_b`'s single `boy1` entity still has no `Victim:` definition and remains unresolved.

This materially reduces the risk of replacing the Training-only `vic_man` asset binding: the existing generic pose evaluator has decoded inputs for every mapped victim type. Runtime loading/selection is not implemented, the texture/material identifier mapping still needs source-backed confirmation, and no rescue scene is playable yet. M4 remains in progress; no EXE review checkpoint or dashboard HTML change was made.
