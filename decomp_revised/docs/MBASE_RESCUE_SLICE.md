# MBase rescue slice: data and loader map

Status: current first-rescue candidate for the Mac slice. The catalog, BSP entity records, briefing, and script entry have been inventoried. The scene is not exported or playable yet; geometry, materials, actor/model resolution, and the level-start path still need implementation and source mapping.

## Catalog and files

The `thegame.lev` catalog selects `mbase` as “Military Base -Rescue-,” uses `rescue.rrf`, and gives the mission a five-minute time limit. The matching `Data/Maps/` files are `mbase.bsp`, `mbase.brf`, `mbase.bmp`, and `mbase.evs`. The bitmap is 160 by 120 pixels and should be treated as a briefing/menu image until its runtime use is traced. File names and hashes are in [`level_map_inventory.csv`](../tracker/level_map_inventory.csv); the raw retail Data directory is local and ignored by Git.

`mbase.evs` runs the shared `rescue.evs`, sets ash density to zero, then sends `moveto` to `mortarfire1` with target `mortar1` and timing parameters. The BSP contains an authored fire named `mortarfire1`; the target binding and exact command semantics remain to be traced. The briefing describes a military training facility, spot fires, and trapped cadets.

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

The BSP chunk walk found 25 chunks and an `ENTDATA` chunk (type 16, 61,691 bytes). The existing parser safely extracts entity epairs and typedef metadata, but no reusable geometry/material exporter exists in the revised project yet. Chunk IDs, element sizes, counts, and offsets for MBase are currently recorded in the working analysis; decode them into named structures only after validating the format against the bundled Genesis code/evidence and a second BSP.

## Next mapping work

1. Trace `rescue.rrf`, the level's BSP/EVS paths, and the shared `rescue.evs` load in the original code so the Mac bootstrap follows the real resource path.
2. Build a reusable GBSP reader that emits geometry, collision, texture/material references, model records, and entities for any selected BSP. Start with MBase and cross-check against Training, while keeping Training out of the product milestone.
3. Resolve the MBase `TextureLib`, sky names, BSP model indices, actor names, and resource files to portable paths. Treat any absolute temporary path as source metadata until runtime behavior confirms how it is used.
4. Follow the `moveto`, `setashdensity`, fog, oxygen, door, victim, fire, and escape commands from authored data into their binary handlers. Reuse Training command evidence only where the same handler/data contract applies.
5. Convert MBase through the reusable path and validate a visible world, camera, player spawn, collision, one fire, one victim, and an exit interaction before expanding the scenario.

This keeps MBase as a measured candidate. If its BSP/model/resource dependencies make it a poor first integration slice, compare the same inventory against another rescue level and record the reason for changing candidates.
