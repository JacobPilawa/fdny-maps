# Work queue and current handoff

Updated 2026-10-13. This file records the next decompilation and port tasks so
research can continue without redoing the old project review or recent audits.

## Current state

- 112 EXE functions have address-specific behavior notes; function activity is
  tracked separately from source recovery and port parity.
- The four code-bearing PE inputs have fresh hash-pinned inventories. The
  level-loading chain and a partial Genesis/D3D driver boundary are mapped.
  FireRescue random spread, suppression, both hose/message contact routes,
  proximity gating, exposure dispatch, and the player burn/passout receiver
  now have fresh address-specific evidence; callback cadence and runtime
  parity remain open.
- The ScriptActor name registry is shared across classes. `tell`'s first value
  is the case-insensitive actor name and the second is its command. Bank's
  `tell vault open` reaches the BModel ScriptActor handler; the handler's
  `open` branch changes BModel command state or queues an open action. The per-frame update maps state 3/4 progression, child-model motion, and one-shot timed command dispatch. A new map ledger joins 10 named `open` targets to their BSP actors across Bank, Boat A, Bar R, and Gas R; runtime cadence, visible timing, and the concrete `Player` registry match remain open.
- The old C++ Training workbench remains useful as the source-backed gameplay
  implementation base. It is still a Training host, not a game-wide runtime;
  Training remains a required catalog target and regression case alongside all seven campaign rescues.
- The GBSP exporter produces all 17 local BSP packages. `SceneData` preserves
  every exported JSON document, validates package identity/schema, and resolves
  package asset paths without case assumptions.
- The standalone viewer loaded all 17 packages through the C++ loader with
  zero unresolved mesh texture paths. It uses the recovered player movement
  and collision implementation. It is an inspectable geometry slice, not a
  playable mission.
- The user requires every rescue mission and map/mechanic to remain in scope,
  with a cross-platform desktop port and macOS as the first playable target.
- `tracker/rescue_mission_coverage.csv` explicitly tracks Training plus all
  seven campaign rescue missions. All eight load as geometry packages in the
  inspection viewer; none is yet a playable rescue port. `level_map_inventory.csv`
  and `scene_load_coverage.csv` cover the other catalog entries and all 17 BSPs.

## Next tasks, in order

1. **Runtime catalog/resource resolver.** Trace the actual `.lev` and `.rrf`
   loaders, then implement catalog-selected `SceneData` creation with explicit
   case aliases for BSP, briefing, EVS, actors, motions, sounds, and textures.
   Retain the original resource spelling and source hash on each resolved row.
2. **Promote the viewer into the shared host.** Extract reusable startup,
   movement, collision, renderer, and asset loading from the Training
   `main.cpp`; keep the current Training executable as a regression fixture.
   No map name or Training assumption belongs in the shared loader.
3. **Wire rescue entities and script startup.** Derive common typed player,
   fire, victim, bounds-trigger, BModel, prop, and exit records from the raw
   ordered epairs. Preserve unknown classes and properties. Trace MBase's
   EVS/shared-EVS commands and original callback cadence before dispatching.
   `moveto` now traces through FireRescue command parsing, shared ScriptActor
   name lookup, state-4 update, timed-key generation, and visual vector interpolation. Bank's `vault open` command now maps from map EVS through the shared registry to the BModel-specific handler. The PathPoint initializer
   passes its authored name into the ScriptActor field searched by that lookup.
   Determine the remaining PathPoint field meanings, the exact authoring/runtime
   intent of the two trailing `moveto` timing values, the external
   scheduler/cadence for `UpdateFireRescueActor`, and the ten `S_Script`
   event-command semantics.
4. **Build one complete rescue path, then cover every rescue mission.** MBase
   is first for loader integration; `bank`, `bar_r`, `gas_r`, `boat_a`,
   `subway_a`, and `oil_rig_a` remain required campaign scenarios. Include
   Training as a required catalog scenario. Reach visible actors, fire state,
   tool interaction, victim rescue, and mission completion for each while
   checking behavior against pinned EXE evidence.
5. **Cover the complete campaign and platforms.** Track each rescue catalog
   scenario, then all hose/other catalog entries and the remaining BSP
   variants. Add Windows/Linux builds against the same scenario contracts.

## Parallel review results already incorporated

- **Genesis driver reuse audit:** public Genesis3D's shared `Dcommon.h`
  exposes a `geBoolean` out-pointer hook (`DRV_Driver **`), `_fastcall`
  callbacks, and table version 100/3. FDNY checks 101/1 and uses observed
  table slots at `+0x14/+0x18/+0x20/+0xA8`; exact public slot compatibility and
  source identity remain unresolved. Public source is an API/algorithm
  reference only; no code was copied. See
  [`ENGINE_SOURCE_REUSE.md`](ENGINE_SOURCE_REUSE.md) and
  [`public_driver_comparison.md`](../tracker/evidence/driver-boundary/public_driver_comparison.md).
- **Rescue integration audit:** the highest-value shared seam is the
  level-neutral loader/catalog resolver. The old renderer and collision
  consumers can use the exported schemas; the Training host's startup,
  scripts, actor catalog, and Apple UI remain specific to that workbench. The
  new `SceneData` package loader and viewer implement the first half of this
  recommendation. Rescue-gameplay wiring remains open.

## Data ledgers

- `tracker/scene_load_coverage.csv`: generated by building
  `fdny-scene-viewer` and running `python3 tools/build_scene_load_coverage.py`.
  It records what the C++ package loader actually opened for each local map.
- `tracker/level_map_inventory.csv`: catalog and BSP relationship, entity
  counts, file hashes, and exporter coverage.
- `tracker/function_reviews.csv` and `tracker/progress_history.csv`: function
  review activity. The dashboard can read these CSVs live; HTML refreshes are
  occasional and must not displace reverse-engineering work.
