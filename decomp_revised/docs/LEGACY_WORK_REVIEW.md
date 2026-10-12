# Review of the previous FDNY work

## What was useful

The previous-work tree remains preserved in the repository's root-level [`decomp/`](../../decomp/) archive. It contains function inventories, Ghidra exports, Training script and startup audits, a partial native Training prototype, and focused mechanics notes. A local working copy was used to review it; the revised project keeps only curated evidence and links back to the archive.

The most useful material for this phase is the address-level Ghidra evidence in:

- ../../decomp/evidence/training-script-command-audit.md
- ../../decomp/evidence/training-script-queue-audit.md
- ../../decomp/evidence/training-startup-integration.md
- ../../decomp/evidence/startup-world/
- ../../decomp/evidence/world-script/
- ../../decomp/evidence/entity-scheduler/
- ../../decomp/evidence/entity-load/

The main script and startup notes identify relevant functions and authored Training commands. The Ghidra outputs are valuable leads because they are tied to the same EXE SHA-256 now pinned in inputs/original. This turn refreshed the selected level-to-EVS functions in the active evidence directory.

## How the old work is being used

- Keep the previous project as a reference archive; do not bulk-copy its prototype or all duplicate reports into the active port.
- Promote a small number of hash-matched function decompilations into tracker/evidence when they answer an active mapping question.
- Treat native tests and startup gates as tests of the native reconstruction. They are not original-process captures and do not prove integrated original timing.
- Treat repeated decompilation copies in nested audit folders as one source of evidence, not independent corroboration.
- Keep static command branches separate from claims that a command or event actually executes in Training.
- Keep the archived Genesis3D source reference separate from the port until binary matching and license review support a reuse decision.

## Carry-over decision

For the current level-to-EVS mapping batch, the active project contains curated, fresh Ghidra outputs for the level path helper, level selection/start, EVS loader, mode wrappers, parser, command dispatcher, and delayed callback. The rest of the old notes remain searchable in the root `decomp/` archive.

## Native Training workbench audit

The prior C++ source was copied to [`port/spikes/training-native-prototype/`](../port/spikes/training-native-prototype/README.md) with its generated Training scene data. Its Mac build compiles with the installed CMake/SDL2/json-c toolchain, and the asset resolver points inside the copied folder. Launching its SDL renderer from the current shell fails with “The video driver did not add any displays,” so it is not yet a verified running build.

The source separates useful mechanics such as player movement, collision queries, actor animation, fire, victims, and equipment into headers. Its `main.cpp` still hard-codes Training startup, entity data, player ID 115, Training scripts, and Training-specific interactions. Its UI uses CoreText/CoreGraphics. The workbench is therefore a source of candidate components and asset-format examples, not a cross-platform game shell. Its only generated world/actor data is Training; it does not provide a campaign asset converter.

Keep the workbench executable and its generated Training fixtures as a separate reference. Promote mechanics into the shared runtime when their call order, platform dependencies, and data contract are supported beyond the Training scene.

## First-pass archive assessment

This is a relevance-based triage, not a line-by-line re-review of every archived pseudocode export. The root `decomp/` archive remains intact. `tracker/legacy_crosswalk.csv` is the index from the earlier address catalog into the pinned revised function inventory; duplicate reports for one address are not treated as independent confirmation. Before redoing an active target, search that crosswalk and the linked archive paths, then verify any reused claim against the pinned binary and its current evidence.

The prior GBSP entity reader was copied into [`tools/extract_gbsp_typedefs.py`](../tools/extract_gbsp_typedefs.py) and reused by [`tools/inventory_level_assets.py`](../tools/inventory_level_assets.py). It extracts the BSP entity epairs and schema records without depending on the Training renderer. That reuse produced the 17-map entity counts in [`level_map_inventory.csv`](../tracker/level_map_inventory.csv) and the first rescue map packet in [`MBASE_RESCUE_SLICE.md`](MBASE_RESCUE_SLICE.md). This parser does not decode world geometry or materials.

The old work also contains `decomp/tools/extract_act.py` and
`map_explorer/source/build_actor_assets.py`. The parser was copied to
[`tools/extract_act.py`](../tools/extract_act.py); a new project-local batch
driver resolves every `Victim:` resource and produces local decoded actor
review files from the hash-pinned ACT inputs. This reuses the parser instead
of copying old generated meshes/textures or depending on the archived folder
at runtime. The resulting coverage and remaining `boy1` lookup gap are
recorded in [`ACTOR_RESOURCE_MAP.md`](ACTOR_RESOURCE_MAP.md) and
[`actor_asset_inventory.csv`](../tracker/actor_asset_inventory.csv).
The copied parser is SHA-256
`28e91021e34c0cb663aaaeae4a2533fa20b83d53eb06a8207568a724d19e21cc`; it is
the fuller of two archived copies and preserves serialized `bitmapId` values.
