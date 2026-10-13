# Shared scene-data contract

Status: **export schema and C++ scene-package loader implemented; rescue gameplay host is not yet map-neutral.**
This contract covers every selectable catalog level and every local BSP
variant. Training is a validation input. MBase is the first new rescue
integration candidate, not a special runtime path.

## Current exported package

`tools/export_gbsp_scene.py <map.bsp> --out .local/exports/<map>` writes a
source-hash-pinned GBSP v15 package. All 17 BSP files currently in
`inputs/original/Data/Maps/` have current local exports. The package contains:

- `report.json`: source SHA-256, chunk offsets/counts, map counts, exporter
  schema version, and the remaining undecoded payload categories.
- `geometry.json`: decoded BSP models, render nodes, faces, planes, vertices,
  texture info, texture records, sky references, and related index arrays.
- `world_mesh.json`: triangulated material groups. Each group retains model
  index, original texture name, texture index, a portable texture file path,
  positions, and projected UVs.
- `collision.json`: models, nodes, leaves, leaf sides, planes, and the
  prototype's current contents mask.
- `entities.json`: class/name/property lookup plus the original ordered epair
  sequence, including duplicate keys.
- `texture_manifest.json` and `textures/`: indexed palette base-level BMPs.
  Exported file paths are case-insensitively unique for macOS filesystems.
- `chunks/`: byte-for-byte chunk payloads; the original BSP also remains in
  the hash-pinned local input tree.

The `world_mesh` structure and collision arrays already match the prototype's
low-level consumers. `src/scene_data.h` now loads the hash-pinned scene package,
keeps every exported document (including ordered entity epairs and unknown
fields), validates the shared schema, and resolves asset paths with the
original on-disk spelling on case-sensitive and case-insensitive filesystems.
`fdny-scene-viewer` consumes that interface without booting the Training host;
it displays any exported map with recovered player movement and BSP collision.
This is a geometry and movement preview, not a rescue mission runtime.

The aggregate property-presence report is
[`scene_property_matrix.csv`](../tracker/scene_property_matrix.csv), generated
from all 17 source-hash-matched exports. It shows that the core authored fields
used by `RCS_PlayerRescue`, `RCS_FireRescue`, and `RCS_VictimRescue` are present
consistently across records in those classes. Bounds triggers have one record
without `C_Model`, and `RCS_BModel` child references vary from `C_AnimModel0`
through higher numbered fields. The shared loader should therefore allow an
absent trigger model and collect BModel children as a sparse list. Field
presence does not prove identical values or behavior.

## Runtime contract to implement

The eventual catalog loader should resolve an entry to a `SceneData` value containing:

1. **Identity and provenance:** package key, actual BSP filename and SHA-256,
   plus the related catalog/RRF/briefing/script metadata when the catalog
   resolver is added.
2. **Render world:** mesh groups, material files, sky references, and retained
   lighting/visibility source data.
3. **Collision world:** BSP model transforms and collision structures with a
   versioned interpretation for contents flags.
4. **Authored entities:** typed common fields derived from epairs while
   retaining the complete ordered source epairs and source entity index.
5. **Resources and behavior:** actor definitions, motions, model aliases,
   scripts, sounds, and other resources resolved through a case-aware path
   manifest. Missing or unknown data must stay visible as unresolved entries.
   The ACT source chain and current actor coverage are tracked in
   [`ACTOR_RESOURCE_MAP.md`](ACTOR_RESOURCE_MAP.md); preserve entity `D_Type`
   through this layer so the runtime can select a victim definition.
6. **Extension data:** unknown classes, properties, and opaque chunks retained
   for later engine-specific mapping instead of being silently discarded.

Class-specific runtime systems subscribe to common entity types such as player,
fire, victim, bounds trigger, BModel, prop, path point, and escape guide. A map
without a given class simply has no instances of that system. Catalog mode and
authored map data select behavior; the loader should contain no `Training` or
`MBase` branches.

## Integration order

1. Implement runtime catalog selection and a case-aware resource resolver that
   preserves actual disk spelling for BSP, RRF, briefing, EVS, texture, and
   actor references.
2. Adapt the existing gameplay host to consume `SceneData`; the standalone
   viewer already demonstrates the shared package interface for geometry,
   textures, player spawn, movement, and collision across every exported BSP.
3. Convert every map's epairs into typed spawn records and register class
   factories. Select player spawn, fire, and victim records by class and
   authored fields; retain unhandled records in the scene.
4. Parse the selected EVS and its shared scripts, bind commands to common
   systems, and recover the original scheduling/callback order.
5. Resolve ACT/SRF/texture/model resources and decode MOTIONS. All observed
   victim ACTs are decoded to local review assets; the unknown `boy1` type's
   first-resource fallback, model-14 renderer ACT paths, and per-model material
   cycling are mapped. Next bind the actor/material catalog to scene creation,
   then add BModel, door, tool, sound, and mission-flow behavior as their
   source contracts are established.
6. Validate every rescue mission, then the remaining hose and catalog
   maps, menus, results, and platform builds. Record scenario status per map;
   export completion alone is not gameplay completion.

## Current gaps

`src/main.cpp` still hard-loads the Training script/motion/actor assets and has
Training-specific startup, zone, hose-command, and actor assumptions. The
standalone viewer loads MBase and every other local BSP package, but no rescue
fire, victim, scripts, door interactions, or mission completion flow is wired
through `SceneData` yet. MBase raw entities do not yet contain all derived
fields the host expects. The exporter has not decoded lightmaps, visibility
semantics, MOTIONS, or game resource aliases. See [`NATIVE_CODE_REUSE_AUDIT.md`](NATIVE_CODE_REUSE_AUDIT.md),
[`MBASE_RESCUE_SLICE.md`](MBASE_RESCUE_SLICE.md),
[`level_map_inventory.csv`](../tracker/level_map_inventory.csv), and
[`scene_property_matrix.csv`](../tracker/scene_property_matrix.csv) for current
component and per-map evidence. Actor type resolution is tracked separately in
[`ACTOR_RESOURCE_MAP.md`](ACTOR_RESOURCE_MAP.md) and
[`victim_resource_crosswalk.csv`](../tracker/victim_resource_crosswalk.csv).
