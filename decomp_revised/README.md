# FDNY Firefighter: American Hero port

This is the clean project workspace for a modern desktop port that preserves the original game's behavior, systems, and presentation as closely as the evidence allows. The first native playable slice will target macOS so progress is visible sooner; Windows and Linux remain required targets for the full port. The existing Training prototype is a workbench to generalize; Training remains a required scenario alongside every campaign rescue.

The initial binary inventory milestone is complete: the four hash-pinned PE files have fresh Ghidra function, section, import/export, and prior-analysis crosswalk inventories. The main EXE currently has 5,812 Ghidra-defined body-bearing functions. This maps the available code; it does not claim the routines are understood or recovered as source.

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
- [Catalog-to-map asset inventory](tracker/level_map_inventory.csv)
- [Copied original inputs and provenance](inputs/original/README.md)
- [Raw retail file comparison copy](inputs/reference/raw-copy/README.md)
- [Wine runtime reference and capture conditions](inputs/runtime-reference/sikarugir/README.md)
- [Assessment of earlier work](docs/LEGACY_WORK_REVIEW.md)

Double-click [Open FDNY Dashboard.command](Open%20FDNY%20Dashboard.command) to start or reuse the local CSV server and open the live dashboard. The page reads `tracker/progress_history.csv`, `tracker/function_reviews.csv`, and `tracker/milestones.csv` directly and refreshes once a minute. Normal reverse-engineering work only updates those tracker files; it does not require regenerating dashboard HTML. Double-clicking `dashboard/index.html` itself opens only its last generated snapshot because browsers block local CSV reads from file pages.

Double-click [Open FDNY Scene Preview.command](Open%20FDNY%20Scene%20Preview.command) to build and launch the MBase geometry/movement preview. It loads the exported map with recovered player movement and collision. Rescue actors, fire behavior, scripts, mission interactions, and completion flow are still being integrated.

## Current phase

**Runtime map and level-to-EVS path.** Across the four PE files, Ghidra defines 7,492 body-bearing functions plus one packed FMOD candidate, alongside 462 external import stubs; the supplied FMOD image is still packed. The review ledger contains 107 EXE functions with address-specific behavior notes. Fresh traces now cover FireRescue random spread, shared suppression, carried-hose and spray-message routes, player fire exposure/passout, and terminal-state visual update gating. Training selection traces through the shared catalog collection, the registered `World` factory, `data\\maps\\training.bsp`, and the WorldEntity BSP loader. The menu's initial selector ordinal is set by the child activation callback. BoundsTrigger class eligibility and its root `+0x48` contact producer through the owner manager are mapped; its embedded updater reaches `touch`/`touching`/`untouch` dispatch, but its dynamic caller and timing remain open. The `tell` branch is traced to ScriptEngine actor lookup and `ScriptActor` dispatch; exact argument roles and delivery to the Player handler remain unproven. The WorldEntity's delayed script load is one-shot after four callbacks, with its scheduler still unresolved after a 53-candidate static scan. MBase's `moveto` command is mapped through the full authored PathPoint name registration and case-insensitive ScriptEngine lookup into FireRescue state-4 path setup and timed-vector interpolation; callback scheduling, exact author-facing timing intent, and runtime frame order remain open. Genesis code at `FUN_10023950` dynamically loads five renderer DLL candidates, resolves `DriverHook`, checks interface fields `+4 == 101` and `+8 == 1`, and invokes subdriver/mode enumeration through table `+0x14/+0x18`. EXE startup calls `geEngine_SetDriverAndMode` at `0x00499BE2`; its Genesis wrapper calls `FUN_10025E00`, which reloads and initializes the selected renderer, retains its handle, and configures fog through table `+0xA8`. No original-game startup capture has been made. The dashboard reads tracker CSVs directly and plots reviewed-function checkpoints over time. The level inventory covers 12 catalog entries and all 17 BSP files; `mbase` is the current Mac rescue-slice candidate, with 228 authored BSP entities inventoried. A map-neutral C++ preview now loads all 17 exported BSP packages with their texture paths, player spawns, and recovered movement/collision; rescue gameplay systems remain to be wired. The [MBase rescue slice note](docs/MBASE_RESCUE_SLICE.md) records the mission-specific loader work.

## Required scenario coverage

Training is included as a required catalog scenario and regression case, alongside all seven campaign rescue missions. It cannot substitute for those campaign rescues. The full port scope remains all 12 selectable catalog entries, every one of the 17 BSP variants, and each scenario's authored mechanics. The [rescue mission tracker](tracker/rescue_mission_coverage.csv) lists the eight rescue-mode scenarios; each is currently geometry-inspector-only and still needs its playable mission systems and parity evidence.

## Working rules

- Keep the copied retail inputs and generated Ghidra databases local. Their hashes and provenance are tracked here.
- Treat Ghidra's names and pseudocode as hypotheses. Preserve addresses, raw instruction evidence, callers, and data references when documenting a claim.
- Track source recovery, component comparison, and integrated game behavior separately. A verified helper is not a verified game loop.
- Use the original executable and data as the behavioral reference. Record the exact binary hash and scenario for every comparison.
- Keep the earlier project intact. Only copy selected implementation files into this workspace when their scope and evidence have been reviewed.
