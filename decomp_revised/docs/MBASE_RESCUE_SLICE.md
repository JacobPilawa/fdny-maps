# MBase rescue slice: data and loader map

Status: current first-rescue candidate for the Mac slice. Catalog, entity records, briefing, script entry, geometry, collision, mesh UVs, and embedded textures have repeatable local exports. The new map-neutral C++ scene package loader and viewer now load MBase, its textures, player spawn, and BSP collision. It is not a playable rescue mission yet: fire/victim creation, map scripts, models/animations, interactions, and mission flow remain open.

## Catalog and files

The `thegame.lev` catalog selects `mbase` as “Military Base -Rescue-,” uses `rescue.rrf`, and gives the mission a five-minute time limit. The matching `Data/Maps/` files are `mbase.bsp`, `mbase.brf`, `mbase.bmp`, and `mbase.evs`. The bitmap is 160 by 120 pixels and should be treated as a briefing/menu image until its runtime use is traced. File names and hashes are in [`level_map_inventory.csv`](../tracker/level_map_inventory.csv); the raw retail Data directory is local and ignored by Git.

`mbase.evs` runs the shared `rescue.evs`, sets ash density to zero, then sends `moveto mortar1 1 10 120` to `mortarfire1`. The BSP contains `RCS_FireRescue` entity 149 (`A_Name=mortarfire1`, location `588 -144 -2900`) and `RCS_PathPoint` entity 150 (`A_Name=mortar1`, location `1904 -232 -3712`). Static Ghidra tracing maps the FireRescue command parser through case-insensitive PathPoint lookup into the movement-state initializer. The first numeric operand is used as a positive divisor for the distance calculation; operands 10 and 120 are stored at `+0xB0/+0xB4`, whose downstream meanings and update cadence remain open. See [`rescue-moveto`](../tracker/evidence/rescue-moveto/README.md). The briefing describes a military training facility, spot fires, and trapped cadets.

## BSP entity scope

`mbase.bsp` is GBSP version 15 and contains 246 entity records: 18 `%typedef%` definitions and 228 non-typedef records. The entity inventory records the complete per-class counts. Relevant authored entities include:

| Class | Count | Port relevance |
|---|---:|---|
| `RCS_PlayerRescue` | 1 | Rescue player setup; authored script equips the axe |
| `RCS_FireRescue` | 11 | Mission fire state and suppression behavior |
| `RCS_VictimRescue` | 5 | Rescue actors; includes `roam` commands |
| `RCS_BoundsTrigger` | 12 | Exit triggers, door/ladder contacts, oxygen zones, and fog events |
| `RCS_BModel` | 13 | Doors, ladders, and other moving/interactive world models |
| `RCS_PropModel` | 8 | Hose, valves, and bonus objects |
| `RCS_EscapeArrow` | 12 | Exit guidance |
| `RCS_PathPoint` | 1 | Authored path point |
| `light` | 138 | Baked/runtime light records to interpret in the scene exporter |
| `%Model%` | 25 | Named BSP model definitions, separate from gameplay entity classes |

The worldspawn records sky texture names and `TextureLib=C:\WINNT\Temp/textures.txl`. That absolute path is not a portable asset location. The BSP's model references include IDs beyond the 25 `%Model%` records, so model-index meaning must be checked against the BSP model chunks and game entity loader before conversion.

The BSP contains 25 chunks and an `ENTDATA` chunk (type 16, 61,691 bytes). The shared GBSP v15 reader now validates every chunk and preserves its payload. The typed export contains 26 BSP models (world plus the 25 authored `%Model%` definitions), 10,023 faces, 11,930 vertices, 5,465 render nodes, 5,491 leaves, 18,018 leaf sides, 5,735 planes, 71 texture records, and all 246 entity records. Geometry, collision, entity properties/ordered epairs, typedefs, world mesh groups, and palette-decoded BMPs are written to `.local/exports/mbase/`, which is ignored by Git because it contains extracted retail assets. The full on-disk chunk map and evidence are in [`GBSP_FORMAT_MAP.md`](GBSP_FORMAT_MAP.md).

The exporter matches the old Training prototype's 121 mesh groups and vertex positions exactly; UVs differ by at most `1.78e-15`. The decoder also reproduces all 29 Training texture BMP images pixel-for-pixel. This validates the shared mesh and indexed-texture extraction against the old work. The C++ viewer loaded all 95 MBase mesh groups and resolved all referenced textures, found the authored player spawn at `(-132, -36, 140)`, and loaded 26 collision models, 5,465 nodes, and 5,491 leaves. This validates the generic package boundary and movement/collision hookup; it does not verify Genesis lighting, transparency, motion data, fire/victim behavior, or full MBase runtime behavior.

## Next mapping work

1. Trace `rescue.rrf`, the level's BSP/EVS paths, and the shared `rescue.evs` load in the original code so the Mac bootstrap follows the real resource path.
2. Decode the serialized model motions and map entity model aliases to the BSP models without relying on Training IDs.
3. Resolve MBase actor names, ACT/SRF assets, shared scripts, and sky/resource references to portable paths. Treat the absolute temporary `TextureLib` value as source metadata until runtime behavior confirms how it is used.
4. Continue from the mapped `moveto`/`setashdensity` entry points through the FireRescue update callback and the still-open `+0xB0/+0xB4` timing fields; then trace fog, oxygen, door, victim, fire, and escape commands from authored data into their handlers. Reuse Training command evidence only where the same handler/data contract applies.
5. Adapt the gameplay host to the reusable `SceneData` API, then validate MBase's player, one fire, one victim, an authored trigger/door interaction, and mission script before expanding the scenario. Keep all rescue maps in the campaign acceptance set.

MBase remains the first integration slice because it is already selected and now loads through the shared scene viewer. The campaign acceptance set includes every rescue entry; this sequence is not a scope reduction.
