# FDNY Firefighter: American Hero port

This is the clean project workspace for a modern desktop port that preserves the original game's behavior, systems, and presentation as closely as the evidence allows. The shared macOS host now packages all eight rescue choices, authored victim and fire systems, tool interactions, rescue transactions, and linked stage transitions. It remains an incomplete port: controlled package checks do not establish natural mission completion or retail parity. Windows and Linux remain required targets. Training is a validation scenario alongside every campaign rescue, not the product scope.

The initial binary inventory milestone is complete: the four hash-pinned PE files have fresh Ghidra function, section, import/export, and prior-analysis crosswalk inventories. The main EXE currently has 5,824 Ghidra-defined body-bearing functions. This maps the available code; it does not claim the routines are understood or recovered as source.

## Start here

- [Project plan and milestones](docs/PROJECT_PLAN.md)
- [Native port work and isolated prototype](port/README.md)
- [Reverse-engineering and port workflow](docs/WORKFLOW.md)
- [Research and references](docs/REFERENCES.md)
- [Reverse-engineering activity log and parallel work](docs/REVERSE_ENGINEERING_LOG.md)
- [Genesis3D source-reuse assessment](docs/ENGINE_SOURCE_REUSE.md)
- [Progress dashboard](dashboard/index.html)
- [Function and binary trackers](tracker/)
- [EXE-to-Genesis API crosswalk](tracker/genesis_api_crosswalk.csv)
- [Training catalog and scenario data anchors](tracker/evidence/scenario-loading/README.md)
- [Level-choice to EVS static trace](tracker/evidence/scenario-loading/level_to_evs_static_trace.md)
- [Gas R script-variable and queue trace](tracker/evidence/scenario-loading/gas_r_script_variable_queue_path.md)
- [Shared rescue contact-manager route](tracker/evidence/scenario-loading/rescue_contact_manager_route.md)
- [Catalog-to-map asset inventory](tracker/level_map_inventory.csv)
- [Copied original inputs and provenance](inputs/original/README.md)
- [Raw retail file comparison copy](inputs/reference/raw-copy/README.md)
- [Wine runtime reference and capture conditions](inputs/runtime-reference/sikarugir/README.md)
- [Assessment of earlier work](docs/LEGACY_WORK_REVIEW.md)

Double-click [Open FDNY Dashboard.command](Open%20FDNY%20Dashboard.command) to start or reuse the local CSV server and open the live dashboard. The page reads `tracker/progress_history.csv`, `tracker/function_reviews.csv`, and `tracker/milestones.csv` directly and refreshes once a minute. Normal reverse-engineering work only updates those tracker files; it does not require regenerating dashboard HTML. Double-clicking `dashboard/index.html` itself opens only its last generated snapshot because browsers block local CSV reads from file pages.

Double-click [Open FDNY Firefighters.command](Open%20FDNY%20Firefighters.command) to open the native title screen and choose from eight source-ordered rescue missions. The title now uses original retail 640x480 art and the retail GAME/SCORES/OPTIONS/EXIT label sheet at source-backed positions. GAME opens the current rescue selector, which still uses a port-created eight-row list instead of the retail single-record briefing/map page. The career, scores, and options pages remain unported; the selected-label fade and keyboard navigation remain approximate. The bundle includes Training, all seven primary rescue maps, and every linked rescue stage; only rescue missions appear in the selector. The old [Open FDNY Scene Preview.command](Open%20FDNY%20Scene%20Preview.command) remains a compatibility launcher. Pass a map key such as `bank` to either launcher to open one exported map directly for development.

The shared host renders BSP textures and authored survivors, supports collision-backed movement, contact pickup, rescue release, authored exits and linked stage transitions, fire suppression/spread, extinguisher use, goggles, and equipment selection. Press **1/2/3** to select an equipped axe, extinguisher, or goggles; these are port controls. With the extinguisher selected, hold the left mouse button and aim at a fire to apply source-derived suppression. With goggles selected, left-click to scan for authored survivors and fire targets. Equipment ACTs draw from camera-relative first-person roots, but their placement, scale, and depth remain provisional. The extinguisher spray particles/audio, exact Axe hit effects/sounds, natural mission completion, carried-body presentation, and material parity remain in progress. Controlled checks and static retail traces are not full gameplay or retail-parity verification.

**BModel motion and contact:** the map-neutral viewer applies retail-shaped contact events and per-owner PATH motion for authored doors. Bank door/vault and Subway A door checks exercise script commands, sound routing, and PATH changes. Press **M** to inspect child PATH data and **N** to clear the inspection overlay; this overlay does not alter gameplay collision.

## Current phase

**Runtime map and level-to-EVS path.** Across the four PE files, Ghidra defines 7,502 body-bearing functions plus one packed FMOD candidate, alongside 462 external import stubs; the supplied FMOD image is still packed. The review ledger contains 285 EXE functions with address-specific behavior notes. Fresh traces now cover FireRescue random spread, shared suppression, carried-hose and spray-message routes, player fire exposure/passout, and terminal-state visual update gating. Training selection traces through the shared catalog collection, the registered `World` factory, `data\\maps\\training.bsp`, and the WorldEntity BSP loader. The menu's initial selector ordinal is set by the child activation callback. The shared contact manager registration and nine type-list traversal are mapped through fresh Ghidra boundaries; its class-specific root `+0x48` callbacks cover BModel, BoundsTrigger, and PropModel overlap checks. Their event deltas route to authored `touch`/`touching`/`untouch` commands; live player overlap and the non-BoundsTrigger updater cadence remain open. Named `tell` dispatch uses the first argument as a case-insensitive ScriptActor name and the second as its command. Bank’s `vault` BModel is registered by the BModel initializer and receives `open` through its class-specific handler; animation state progression and timed command dispatch are mapped statically, while runtime cadence, visible timing, and Training’s concrete `player` registry match remain open. The shared path maps 10 same-BSP named BModel `open` targets across Bank, Boat A, Bar R, and Gas R, plus stage-local Oil Rig B catwalks and Oil Rig C stairs and steam actors. The Oil Rig mission’s static exit chain is A→B→C→D; `finalexitloadmap` map-key path validation and the static pending-level-to-World-loader path are mapped; the pending request map-name field at EverGame+0x660 and its loader read are mapped; equality between the checked command argument and forwarded context, runtime stage completion, and timing remain open. See the [rescue target inventory](tracker/evidence/scenario-loading/bmodel_rescue_targets.md), [Oil Rig staged targets](tracker/evidence/scenario-loading/oil_rig_a_rescue_script_targets.md), and [MiscEmitter handler evidence](tracker/evidence/scenario-loading/004420f0_misc_emitter_dispatch.md). Gas R now links authored BoundsTrigger entities 265 and 280 plus BModel 309 to the explosion1, explosion2, and gasoff EVS definitions. Its shared ScriptActor queue order, one-command pump, wait deadline handling, BModel button1 use-event route, and BoundsTrigger contact-to-event implementation are mapped; live contact/selection, external callback cadence, and runtime parity remain open. See [Gas R script evidence](tracker/evidence/scenario-loading/gas_r_script_variable_queue_path.md) and [button1 use trace](tracker/evidence/scenario-loading/gas_r_button1_use_path.md). Subway A binds `open`/`close` as self-commands rather than a named `tell`; its 17 door actors and 25 broken-door sound triggers now join through shared event registration and contact dispatch. Sound resource lookup, deferred playback, sample/stream FMOD calls, and the 3D-attribute API are now traced; `SubDoor` and `Zapp3d` join to hash-pinned `ROC_SOUND_BUFFER3D` definitions and WAVs. The parser confirms both sounds set the FMOD 3D flag and pass `200`/`1000` to `FSOUND_Sample_SetMinMaxDistance` as minimum/maximum distances; runtime audibility and falloff still need capture. See [the Subway A interaction trace](tracker/evidence/scenario-loading/subway_a_interaction_path.md) and [Bank vault dispatch evidence](tracker/evidence/scenario-loading/bank_vault_open_dispatch.md). The WorldEntity's delayed script load is one-shot after four callbacks, with its scheduler still unresolved after a 53-candidate static scan. MBase's `moveto` command is mapped through the full authored PathPoint name registration and case-insensitive ScriptEngine lookup into FireRescue state-4 path setup and timed-vector interpolation; callback scheduling, exact author-facing timing intent, and runtime frame order remain open. Genesis code at `FUN_10023950` dynamically loads five renderer DLL candidates, resolves `DriverHook`, checks interface fields `+4 == 101` and `+8 == 1`, and invokes subdriver/mode enumeration through table `+0x14/+0x18`. EXE startup calls `geEngine_SetDriverAndMode` at `0x00499BE2`; its Genesis wrapper calls `FUN_10025E00`, which reloads and initializes the selected renderer, retains its handle, and configures fog through table `+0xA8`. No original-game startup capture has been made. The dashboard reads tracker CSVs directly and plots reviewed-function checkpoints over time. The level inventory covers 12 catalog entries and all 17 BSP files; `mbase` is the current Mac rescue-slice candidate, with 228 authored BSP entities inventoried. A map-neutral C++ viewer now loads all 17 exported BSP packages and has shared gameplay mechanics running across rescue maps, including victim paths, fire suppression/spread, BModel contacts and motion, and PropModel actors/contacts. Full rescue mission playability and original parity remain incomplete. The [MBase rescue slice note](docs/MBASE_RESCUE_SLICE.md) records the mission-specific loader work.

## Required scenario coverage

Training is included as a required catalog scenario and regression case, alongside all seven campaign rescue missions. It cannot substitute for those campaign rescues. The full port scope remains all 12 selectable catalog entries, every one of the 17 BSP variants, and each scenario's authored mechanics. The [rescue mission tracker](tracker/rescue_mission_coverage.csv) lists the eight rescue-mode scenarios. Shared rescue mechanics now run on the map packages, but no campaign mission yet meets the complete playable-and-parity bar in that tracker.

## Working rules

- Keep the copied retail inputs and generated Ghidra databases local. Their hashes and provenance are tracked here.
- Treat Ghidra's names and pseudocode as hypotheses. Preserve addresses, raw instruction evidence, callers, and data references when documenting a claim.
- Track source recovery, component comparison, and integrated game behavior separately. A verified helper is not a verified game loop.
- Use the original executable and data as the behavioral reference. Record the exact binary hash and scenario for every comparison.
- Keep the earlier project intact. Only copy selected implementation files into this workspace when their scope and evidence have been reviewed.

## GitHub sync

This workspace maps to `decomp_revised/` on `main` in [JacobPilawa/fdny-maps](https://github.com/JacobPilawa/fdny-maps). The repository also contains other projects, so a direct push of this folder is not the sync workflow.

Compare without publishing:

```sh
python3 tools/sync_github.py
```

Commit and push the local project files:

```sh
python3 tools/sync_github.py --push --message "Sync latest decompilation work"
```

The command respects the local Git file inventory and ignore rules, updates only `decomp_revised/`, and verifies the resulting branch. It stops if GitHub has files absent from the local publishable inventory; remote deletions need explicit reconciliation. Retail inputs, Ghidra projects, generated assets, and builds stay local. GitHub authentication must be available to command-line Git.
