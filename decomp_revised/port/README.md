# Native port work

The product goal is the full game, with a native macOS playable slice first and
Windows/Linux support as later acceptance gates. The first playable slice must
cover a rescue campaign path. Military Base (`mbase`) is the first integration
candidate because it is the first rescue entry after Training and its BSP has a
manageable rescue-object mix. Its geometry, exported textures, player spawn,
and recovered movement/collision now load in a map-neutral preview; mission
systems and catalog-driven startup are the remaining integration work.

## Current contents

- [`spikes/training-native-prototype/`](spikes/training-native-prototype/README.md)
  is a copied, single-level Training workbench with a substantial integrated
  gameplay reconstruction. It compiles as a Mac executable with CMake, SDL2,
  and json-c, though a window has not launched in the current shell. Its
  gameplay components are candidates for direct reuse; its Training-specific
  bootstrap, data path, and Apple CoreText UI need replacement or generalizing.
  See the [native C++ reuse audit](../docs/NATIVE_CODE_REUSE_AUDIT.md).
- `spikes/training-native-prototype/src/scene_data.h` and
  `scene_viewer.cpp` provide a level-neutral package loader and BSP
  geometry/movement preview. All 17 local packages load with zero unresolved
  texture paths. The preview does not yet create fires/victims or run scripts.

## Main port checkpoints

1. Reconcile every catalog entry with its BSP, briefing, EVS scripts, RRF
   resource table, and auxiliary map variants. See
   [`tracker/level_map_inventory.csv`](../tracker/level_map_inventory.csv).
2. Recover a repeatable local asset extraction path from the original data.
3. Add runtime catalog/resource selection and adapt the gameplay host to the
   shared `SceneData` contract; use `mbase` for the first rescue integration.
4. Feed the selected mission through the existing source-backed player,
   collision, fire, victim, and interaction components where their data
   contracts match; extend the component set for MBase-specific behavior.
5. Restore the original menu/briefing/result flow, then expand through every
   level and carry the same runtime to Windows and Linux.

The dashboard and trackers remain evidence-oriented. Function recovery,
component comparisons, playable scenarios, and platform builds are separate
measures; none should be collapsed into a single decompilation percentage.
