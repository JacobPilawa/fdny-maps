# Work queue and current handoff

Updated 2026-10-13. This file records the next decompilation and port tasks so
research can continue without redoing the old project review or recent audits.

## Current state

- The interactive Mac scene preview is the first shell milestone: it loads exported BSP geometry and textures, starts from the authored player spawn when present, and uses the recovered movement and BSP collision path. The map picker currently lists exported map packages; connecting it to the game catalog will make rescue-mission selection explicit. The GUI still needs a successful launch in a normal desktop session. This preview is designed to grow into the shared runtime, then gain authored actors, scripts, tools, and mission flow.

- 210 EXE functions have address-specific behavior notes; function activity is
  tracked separately from source recovery and port parity.
- The entity-factory queue order is now mapped across a selected-level load:
  mode-1 EVS runs before World creation, WorldEntity appends BSP requests after
  the current pump's drain phase, and PlayerRescue requests are handled by a
  later pump invocation. Early shared `rescue.evs` tells to Player are dropped
  before Player registration. The delayed full EVS pass still needs ordering
  against that later pump to settle the initial selected tool. See
  `tracker/evidence/scenario-loading/entity_factory_queue_update_order.md`.
- The four code-bearing PE inputs have fresh hash-pinned inventories. The
  level-loading chain and a partial Genesis/D3D driver boundary are mapped.
  FireRescue random spread, suppression, both hose/message contact routes,
  proximity gating, exposure dispatch, and the player burn/passout receiver
  now have fresh address-specific evidence; callback cadence and runtime
  parity remain open.
- The ScriptActor name registry is shared across classes. `tell`'s first value
  is the case-insensitive actor name and the second is its command. Bank's
  `tell vault open` reaches the BModel ScriptActor handler; the handler's
  `open` branch changes BModel command state or queues an open action. The per-frame update maps state 3/4 progression, child-model motion, and one-shot timed command dispatch. A new map ledger joins 10 named `open` targets to their BSP actors across Bank, Boat A, Bar R, and Gas R; the static PlayerRescue factory and Player equip/select paths now join BSP commands to inventory state; final tool state, live cadence, and runtime parity remain open. See tracker/evidence/scenario-loading/player_loadout_command_path.md.
- Subway A now has a static entity-to-handler trace for 17 automatic door BModels and 25 broken-door sound BoundsTriggers; all 51 animation children and all trigger collision models resolve. Sound-name lookup, deferred playback, sample/stream FMOD calls, and the 3D-attribute API are traced, and `SubDoor`/`Zapp3d` join to hash-pinned 3D-buffer definitions and WAVs. The resource-gate path and min/max-distance setter are statically mapped; runtime audibility/falloff and the remaining Subway rescue mechanics remain open. See `tracker/evidence/scenario-loading/subway_a_interaction_path.md`.
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
- Gas R BSP entities 265, 280, and 309 are joined to the explosion1, explosion2, and gasoff EVS definitions. ScriptActor queue pop/order and wait deadlines are mapped, alongside BModel button1 use handling, type-1 contact candidate checks, and shared BoundsTrigger overlap-pair → touch/touching/untouch dispatch. The named-model motion join resolves button1's effective reach to 20 units. Live contact/selection, updater cadence, and original-game effect captures remain open. See the Gas R evidence linked in the scenario-loading README. The same retail Genesis.dll MOTN/PATH extent decode is now joined across Training and all seven rescue maps: 71 of 76 authored-gate BModels receive a static 48-unit reach; five motionless ladder/button models retain authored values. Per-map/entity results are in `tracker/evidence/scenario-loading/bmodel_reach_cross_map.md`, its JSON inventory, and the rescue coverage CSV. This closes only the static reach calculation; live contact and callback cadence remain open.
- `tracker/rescue_mission_coverage.csv` explicitly tracks Training plus all
  seven campaign rescue missions. All eight load as geometry packages in the
  inspection viewer; none is yet a playable rescue port. `level_map_inventory.csv`
  and `scene_load_coverage.csv` cover the other catalog entries and all 17 BSPs.
- `tracker/rescue_actor_inventory.csv` maps 564 authored player, fire, victim,
  alternate-victim-spot, and PathPoint entities across all 17 BSPs. Fire/victim
  counts reconcile to the level inventory. Raw epairs and positions are
  retained for map-neutral spawn conversion; current-hash code confirms the
  VictimRescue and VictimAltSpot factory path and registration-before-spawn order.
- Victim `roam <number>` now traces from the class-specific ScriptActor fallback
  through obstacle collection, initial-position checks, animation selection,
  target selection, and state-7 movement. `forcespawnspot` selects from the
  victim's registered spawn list. The actor inventory summarizes the exact
  non-empty victim script values across every BSP; the alternate-spot name
  resolver joins all 45 authored spots to a victim on the same BSP. Numeric-only
  fields and update cadence remain unresolved. The current-hash factory trace confirms alternate-spot registration before victim spawn selection across the authored rescue BSPs. See
  `tracker/evidence/scenario-loading/victim_roam_behavior.md` and `tracker/evidence/scenario-loading/victim_spawn_factory_and_order.md`.
- The shared victim pickup path now maps contact overlap through Message Mapper
  message `0x7532`, the Player's empty-carry gate, and the VictimRescue `pickup`
  event. The shared escape/release path updates rescue statistics and victim
  state, then applies final-exit gates. A generated 17-BSP inventory records 40
  authored exit triggers, joins all 40 `C_Model` values to named BSP models, and
  records Boat, Subway, and Oil Rig stage destinations. `tell player` now traces through the static PlayerRescue factory and handler path;
  trigger/update cadence and live mission parity remain open. See
  `tracker/evidence/scenario-loading/victim_pickup_message_path.md`,
  `tracker/evidence/scenario-loading/rescue_completion_and_exit_path.md`, and
  `tracker/rescue_exit_trigger_inventory.md`.

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
   MBase's `setashdensity 0` route now maps to the clamped Player scalar at
   `+0x5b0`; the MBase `tell` is delayed until mode 0, but its order relative to
   Player registration remains open. The previous ash-pool analysis and native
   clamp implementation are reused.
   Determine the remaining PathPoint field meanings, the exact authoring/runtime
   intent of the two trailing `moveto` timing values, the external
   scheduler/cadence for `UpdateFireRescueActor`, and the ten `S_Script`
   event-command semantics. The VictimRescue `roam` path is statically mapped;
   continue with runtime capture of Bar R's unhandled `setskin 2` fields,
   numeric-only victim script values and victim update cadence; the current-hash
   factory path and registration-before-selection order are now mapped. Capture live command timing and the final selected tool per scenario; the static
   Player identity/recipient route is now mapped, and every rescue mission and
   stage remains in scope.
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
  review activity. `dashboard/index.html` reads these CSVs when opened; update
  the CSV data to show progress without regenerating the HTML.
