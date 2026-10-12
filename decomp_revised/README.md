# FDNY Firefighter: American Hero — revised decompilation workspace

This folder contains the evidence-first decompilation and cross-platform port research started on 2026-10-12. The sibling [`decomp/`](../decomp/) directory remains the earlier archive and is unchanged.

The public GitHub copy contains authored analysis, function inventories, progress CSVs, and tooling. Retail EXE/DLL files, extracted game data, local runtime overlays, and Ghidra project databases stay on the research machine; their hashes and provenance are documented here.

# FDNY Firefighter: American Hero port

This is the clean project workspace for a modern desktop port that preserves the original game's behavior, systems, and presentation as closely as the evidence allows. The first playable build is planned for Windows, macOS, and Linux, with Training as the first complete scenario.

The initial binary inventory milestone is complete: the four hash-pinned PE files have fresh Ghidra function, section, import/export, and prior-analysis crosswalk inventories. The main EXE has 5,807 body-bearing functions. This maps the available code; it does not claim the routines are understood or recovered as source.

## Start here

- [Project plan and milestones](docs/PROJECT_PLAN.md)
- [Reverse-engineering and port workflow](docs/WORKFLOW.md)
- [Research and references](docs/REFERENCES.md)
- [Reverse-engineering activity log and parallel work](docs/REVERSE_ENGINEERING_LOG.md)
- [Genesis3D source-reuse assessment](docs/ENGINE_SOURCE_REUSE.md)
- [Progress dashboard](dashboard/index.html)
- [Function and binary trackers](tracker/)
- [EXE-to-Genesis API crosswalk](tracker/genesis_api_crosswalk.csv)
- [Training catalog and scenario data anchors](tracker/evidence/scenario-loading/README.md)
- [Level-choice to EVS static trace](tracker/evidence/scenario-loading/level_to_evs_static_trace.md)
- [Copied original inputs and provenance](inputs/original/README.md)
- [Raw retail file comparison copy](inputs/reference/raw-copy/README.md)
- [Wine runtime reference and capture conditions](inputs/runtime-reference/sikarugir/README.md)
- [Assessment of earlier work](docs/LEGACY_WORK_REVIEW.md)

Double-click [Open FDNY Dashboard.command](Open%20FDNY%20Dashboard.command) to start or reuse the local CSV server and open the live dashboard. The page reads `tracker/progress_history.csv`, `tracker/function_reviews.csv`, and `tracker/milestones.csv` directly and refreshes once a minute. Normal reverse-engineering work only updates those tracker files; it does not require regenerating dashboard HTML. Double-clicking `dashboard/index.html` itself opens only its last generated snapshot because browsers block local CSV reads from file pages.

## Current phase

**Runtime map and level-to-EVS path.** The four PE files have 7,487 analyzable body functions and 462 external import stubs; the supplied FMOD image is still packed. The review ledger contains 66 EXE functions with address-specific behavior notes. Training selection traces through the shared catalog collection, the registered `World` factory, `data\\maps\\training.bsp`, and the WorldEntity BSP loader. The menu's initial selector ordinal is set by the child activation callback. BoundsTrigger class eligibility and its root `+0x48` contact producer through the owner manager are mapped; its embedded updater reaches `touch`/`touching`/`untouch` dispatch, but its dynamic caller and timing remain open. Training's `tell player escape` reaches parser helper `00472270`; delivery to the Player handler is not established. The WorldEntity's delayed script load is one-shot after four callbacks, with its scheduler still unresolved after a 53-candidate static scan. Genesis code at `FUN_10023950` dynamically loads five renderer DLL candidates, resolves `DriverHook`, checks interface fields `+4 == 101` and `+8 == 1`, and invokes the subdriver callback at table `+0x14`; that callback also requests mode records through `+0x18`. The later selected-driver path and table lifetime remain open. No original-game startup capture has been made. The dashboard reads tracker CSVs directly and plots reviewed-function checkpoints over time.

## Working rules

- Keep the copied retail inputs and generated Ghidra databases local. Their hashes and provenance are tracked here.
- Treat Ghidra's names and pseudocode as hypotheses. Preserve addresses, raw instruction evidence, callers, and data references when documenting a claim.
- Track source recovery, component comparison, and integrated game behavior separately. A verified helper is not a verified game loop.
- Use the original executable and data as the behavioral reference. Record the exact binary hash and scenario for every comparison.
- Keep the earlier project intact. Only copy selected implementation files into this workspace when their scope and evidence have been reviewed.
