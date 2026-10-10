# FDNY Firefighter Reverse Engineering Guide

This workspace contains a first-pass extraction and visualization pipeline for `FDNY Firefighter: American Heroes` map data.

The important result: the stage maps are Genesis3D/Reality Factory `GBSP` v15 BSP files. They can be decoded directly enough to extract geometry, entities, doors, fires, hose/valves, exits, and survivor locations.

## What Was Built

### GBSP Extractor

Script:

```text
tools/extract_gbsp.py
```

Purpose:

- Reads Genesis3D `GBSP` v15 BSP files.
- Decodes chunks, models, faces, vertices, textures, and binary entity data.
- Exports raw geometry and entity summaries.

Example outputs:

```text
extracted/training/summary.json
extracted/training/entities.txt
extracted/training/geometry.obj
extracted/training/topdown.svg
```

### Annotated Map Renderer

Script:

```text
tools/render_annotated_maps.py
```

Purpose:

- Produces full annotated SVG maps and entity tables.
- These are technically useful but can be visually cluttered because BSP maps have many surfaces.

Outputs include:

```text
extracted/training/topdown_annotated.svg
extracted/training/annotated_entities.csv
extracted/training/important_entities.csv
```

### Simplified PNG Diagram Renderer

Script:

```text
tools/render_simple_maps.py
```

Purpose:

- Produces human-readable PNG diagrams.
- Simplifies walls from BSP vertical faces.
- Shows doors, survivor locations, RNG alt spots, exits, fires, hose, and valves.
- Creates stripped-down survivor-focused maps.

Main outputs:

```text
extracted/training/stage_diagram_simple.png
extracted/training/stage_diagram_gameplay_compact.png
extracted/training/stage_diagram_survivors.png

extracted/subway_a/stage_diagram_simple.png
extracted/subway_a/stage_diagram_gameplay_compact.png
extracted/subway_a/stage_diagram_survivors.png

extracted/subway_b/stage_diagram_simple.png
extracted/subway_b/stage_diagram_gameplay_compact.png
extracted/subway_b/stage_diagram_survivors.png
```

Recommended map files for route planning:

- `stage_diagram_simple.png`: best overall map with labels and gameplay objects.
- `stage_diagram_survivors.png`: cleaner survivor/exit/door-focused map.
- `important_entities.csv`: coordinate table matching labels on the map.

### 3D Viewer

Files:

```text
viewer/index.html
viewer/data/manifest.json
viewer/data/*.json
```

Purpose:

- Interactive simplified 3D stage viewer.
- Uses decoded wall geometry and gameplay markers.
- Loads every exported stage listed in `viewer/data/manifest.json`.
- Supports top-down and fly camera modes.
- Supports layer toggles.
- Supports survivor RNG alt-spot toggles.

Important note: the viewer loads Three.js from a CDN, so the browser needs internet access.

### Rescuebot Specification

Files:

```text
docs/rescuebot_spec.md
extracted/rescuebot_entity_inventory.csv
```

Purpose:

- Documents what is currently known about the `rescuebot` system.
- Uses decoded BSP entities, scripts, resource files, and executable strings.
- No Ghidra/decompilation was used for this pass.

Key findings:

- `rescuebot` is implemented in `rescue\RescueBot.cpp` inside `FDNYFirefighter.exe`.
- Training explicitly disables rescuebot with `setrescuebot 0`.
- Normal rescue maps, including Subway A and Subway B, appear to leave rescuebot enabled by default.
- Victim RNG spawn options are represented by `RCS_VictimAltSpot` entities linked to `RCS_VictimRescue` entities.
- Exact rescuebot timing/selection logic is not proven yet without runtime testing or decompilation.

## How To Run The 3D Viewer

From the repo root:

```bash
cd /Users/jacobpilawa/Desktop/fdny_codex/viewer
python3 -m http.server 8000
```

Then open this URL in a browser:

```text
http://localhost:8000
```

Controls:

```text
Mouse drag    Look around
Mouse wheel   Change camera height / zoom
WASD          Move camera
Arrow keys    Move camera
Q / E         Move down / up
Shift         Move faster
```

Viewer buttons:

```text
Top-down camera  Switch to overhead map view
Fly camera       Switch to controllable fly-through view
```

Layer toggles:

```text
walls
 doors
 fires
 hose_valves
 exits
 survivors
 alt_spots
 floor_outlines
```

Survivor RNG dropdowns:

- `show all`: show base survivor and all known RNG alt spots.
- `base only`: hide alt spots for that survivor.
- `A#`: show only that specific alternate spot.

## How To Regenerate Outputs

### Regenerate Every Stage

The current all-stage build script processes every `.bsp` file in the game map folder and regenerates the extractor outputs, annotated SVGs, simplified PNG diagrams, PDF map/table sheets, 3D viewer JSON, and `viewer/data/manifest.json`.

```bash
python3 tools/build_all_maps.py
```

This currently generates outputs for 17 map files:

```text
bank
bar_r
boat_a
boat_b
City_h
const_h
dock_h
gas_r
mbase
oil_rig_A
oil_rig_B
oil_rig_C
oil_rig_D
oil_rig_h
subway_a
subway_b
training
```

Each stage gets its own output directory under `extracted/<stage>/`. For example:

```text
extracted/bank/stage_diagram_simple.png
extracted/bank/stage_diagram_survivors.png
extracted/bank/topdown_stage_clear.svg
extracted/bank/bank_topdown_stage_clear_with_table.pdf
extracted/bank/bank_topdown_annotated_with_table.pdf
extracted/bank/annotated_entities.csv
extracted/bank/geometry.obj
```

The 3D viewer dropdown is driven by:

```text
viewer/data/manifest.json
```

When the batch script completes, refresh the browser page at `http://localhost:8000` to see the expanded stage list.

### Extract Training

```bash
python3 tools/extract_gbsp.py \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/training.bsp" \
  --out extracted/training
```

### Extract Subway A and Subway B

```bash
python3 tools/extract_gbsp.py \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/subway_a.bsp" \
  --out extracted/subway_a

python3 tools/extract_gbsp.py \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/subway_b.bsp" \
  --out extracted/subway_b
```

### Regenerate Simplified PNG Maps

```bash
python3 tools/render_simple_maps.py Training \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/training.bsp" \
  extracted/training

python3 tools/render_simple_maps.py Subway_A \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/subway_a.bsp" \
  extracted/subway_a

python3 tools/render_simple_maps.py Subway_B \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/subway_b.bsp" \
  extracted/subway_b
```

### Regenerate 3D Viewer Data

```bash
python3 tools/export_viewer_data.py Training \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/training.bsp" \
  viewer/data/training.json

python3 tools/export_viewer_data.py Subway_A \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/subway_a.bsp" \
  viewer/data/subway_a.json

python3 tools/export_viewer_data.py Subway_B \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/subway_b.bsp" \
  viewer/data/subway_b.json
```

## Label Meaning

Common labels on maps/viewer:

```text
S#  Player/start entity
V#  Survivor base spawn
A#  Survivor RNG alternate spawn spot
E#  Exit arrow
H#  Hose
W#  Valve
F#  Fire marker
D#  Door/model marker
```

For full label details, use:

```text
extracted/<map>/important_entities.csv
extracted/<map>/annotated_entities.csv
```

## Current Limits

- Geometry is simplified from BSP surfaces, not rebuilt from original editor brushes.
- Door markers are entity points, not perfect door outlines.
- Fire markers are shown as dots, not exact volumes.
- Survivor markers are spawn/origin locations; victims with `roam N` can move after spawning.
- `RCS_VictimAltSpot` indicates possible RNG spawn positions, but exact RNG selection probability is not yet decompiled.
- Rescuebot exact timing/eligibility is not fully proven yet.
- Trigger labels are entity origins. For `RCS_BoundsTrigger`, the real trigger is usually a referenced 3D brush/model volume named by `C_Model`; the full-annotation PDF draws that top-down trigger footprint as a translucent dashed rectangle when the referenced `%Model%` can be resolved.

## Suggested Next Work

1. Use runtime testing to confirm survivor RNG and rescuebot behavior.
2. If needed, use Ghidra on `FDNYFirefighter.exe` and focus on strings documented in `docs/rescuebot_spec.md`.
3. Improve wall simplification by reconstructing room polygons from BSP faces or leaf portals.
4. Add screenshots/video captures from the game to validate map orientation and room boundaries.

## Detailed Output File Reference

This section explains the generated data files in detail. These files are intended to make the map images auditable: every label on a map should be traceable back to decoded BSP entity data.

### `extracted/rescuebot_entity_inventory.csv`

Path:

```text
extracted/rescuebot_entity_inventory.csv
```

What it is:

This is a cross-map inventory of rescue-mode entities pulled from every decoded `.bsp` map that contains relevant victim/player/rescuebot data. It is not specific to Training or Subway; it scans all BSP maps in `Data/Maps` and extracts rows for:

```text
RCS_PlayerRescue
RCS_VictimRescue
RCS_VictimAltSpot
```

It also includes any rows containing rescuebot-related script text, such as `setrescuebot`.

Why it exists:

The goal is to understand the rescue system globally, not just in one level. This file lets you compare how survivors, alt-spawn spots, and player rescue starts are represented across all rescue stages.

Important conclusion from this file:

- Training has `setrescuebot 0` on its `RCS_PlayerRescue` entity.
- No other decoded map row currently shows `setrescuebot 0`.
- This strongly suggests Training disables rescuebot explicitly, while normal rescue maps leave it enabled by default.

Columns:

```text
map
```

The BSP map stem where the entity was found. Examples:

```text
training
subway_a
subway_b
bank
bar_r
boat_a
```

This corresponds to files like:

```text
Data/Maps/training.bsp
Data/Maps/subway_a.bsp
```

```text
entity
```

The zero-based entity index inside the decoded BSP entity list. This is useful when cross-referencing with `entities.txt` or `summary.json`.

Example:

```text
training,115,RCS_PlayerRescue,...
```

This means entity `115` in `training.bsp` is the `RCS_PlayerRescue` entity.

```text
class
```

The entity class name from the BSP entity data.

Common classes in this file:

```text
RCS_PlayerRescue
RCS_VictimRescue
RCS_VictimAltSpot
```

Meaning:

```text
RCS_PlayerRescue    Player/controller start for rescue maps
RCS_VictimRescue    A survivor/victim entity
RCS_VictimAltSpot   An alternate spawn spot linked to a survivor
```

```text
name
```

The most useful display/internal name found for that entity. Usually this comes from `A_Name` or `%name%`.

Examples:

```text
Victim1
victim1
RCS_PlayerRescue1
RCS_VictimAltSpot3
```

Important nuance:

- `A_Name` is often a human/editor-style name like `Victim1`.
- `%name%` is often an internal generated entity id like `RCS_VictimRescue2`.
- `RCS_VictimAltSpot.A_Victim` links to the internal `%name%`, not necessarily the human `A_Name`.

```text
location
```

The entity location in engine coordinates, formatted as:

```text
X Y Z
```

Example:

```text
1180 -532 -1452
```

Coordinate interpretation:

```text
X   horizontal map axis
Y   vertical/elevation axis
Z   horizontal map axis
```

The top-down maps project `X/Z` and treat `Y` as height.

For `RCS_VictimRescue`, this is the base survivor spawn/origin.

For `RCS_VictimAltSpot`, this is an RNG alternate spawn location.

For `RCS_PlayerRescue`, this is the rescue-mode player start/controller location.

```text
type
```

For `RCS_VictimRescue`, this is the victim archetype from:

```text
Data/ResDefs/victims.srf
```

Known victim types found in the game include:

```text
boy
woman
man
fatman
girl
dog
herodown
```

The victim type controls model, beckon sounds, rescue sounds, and speech names. For example, `victims.srf` maps `fatman` to a model and uses man-style beckon/rescue sounds.

For non-victim rows, this column is usually empty.

```text
radius
```

Usually the victim `J_Radius` field. This appears to be a bounding/obstacle interaction radius for `RCS_VictimRescue` entities.

Examples:

```text
0.0
20
24
40
60
```

Interpretation:

- Larger values likely mean a larger collision/search radius around the victim.
- Some victims use `0.0`, which may mean default/no explicit radius.
- Exact gameplay effects are not fully proven yet without code-level analysis.

```text
victim_link
```

Only meaningful for `RCS_VictimAltSpot` rows.

This links an alternate spawn spot to the internal `%name%` of an `RCS_VictimRescue` entity.

Example:

```text
RCS_VictimRescue2
```

If a row is:

```text
subway_a,565,RCS_VictimAltSpot,...,victim_link=RCS_VictimRescue2
```

Then that alt spot belongs to the survivor whose internal id is `RCS_VictimRescue2`.

This is why the viewer can offer survivor RNG dropdowns.

```text
script1
script2
script3
script4
```

The first four script slots from the entity. These are raw game script commands or event hooks.

Examples:

```text
equip axe
setrescuebot 0
roam 1000
pickup: ifnot gotvictim 1 (*Cmd3c)
pickup: set gotvictim 1
```

Common meanings:

```text
equip axe              Give/select axe at level start
setrescuebot 0         Disable rescuebot for this level/entity context
roam 1000              Victim can roam after spawn; number is likely roam distance/range/time parameter
pickup: ...            Script runs when player picks up a survivor
<null>                 Empty/null script field from the original entity data
```

The scripts are copied as decoded; they are not interpreted by the extractor.

```text
spawn_message
```

The entity spawn message field. Usually `10000` in decoded maps.

Observed forms:

```text
spawn_message
ZZ_spawn_message
```

The exact engine meaning is not fully proven yet. It appears to be part of the entity spawning/initialization metadata. The inventory normalizes either field into `spawn_message`.

### `extracted/<map>/annotated_entities.csv`

Examples:

```text
extracted/training/annotated_entities.csv
extracted/subway_a/annotated_entities.csv
extracted/subway_b/annotated_entities.csv
```

What it is:

A per-map table of all entities that the annotation renderer considered relevant enough to label or classify. This includes more than just survivor routing data. It may include:

```text
START
VICTIM
ALT
FIRE
HOSE
VALVE
EXIT
DOOR
TRIGGER
PROP
```

This is the complete table used for the `topdown_annotated.svg` and the new PDF map sheets with tables.

Columns:

```text
label
```

Short map label assigned by the renderer.

Examples:

```text
S1
V1
A1
F12
D4
T7
```

These labels are generated per category and per map. They are not original game ids.

```text
category
```

Renderer category assigned from the entity class and scripts.

Meaning:

```text
START    RCS_PlayerRescue, player/controller start
VICTIM   RCS_VictimRescue, survivor base spawn
ALT      RCS_VictimAltSpot, survivor RNG alternate spawn
FIRE     RCS_FireRescue
HOSE     RCS_PropModel with hose/touchhose script
VALVE    RCS_PropModel with makevalve script or valve name
EXIT     RCS_EscapeArrow
DOOR     RCS_BModel or model entity interpreted as a door
TRIGGER  RCS_BoundsTrigger
PROP     Other relevant prop model
```

```text
entity_index
```

Zero-based entity index in the decoded BSP entity list.

Use this to find the complete raw entity in:

```text
extracted/<map>/entities.txt
extracted/<map>/summary.json
```

```text
class
```

Original entity class from BSP data.

Examples:

```text
RCS_PlayerRescue
RCS_VictimRescue
RCS_VictimAltSpot
RCS_FireRescue
RCS_PropModel
RCS_BModel
RCS_BoundsTrigger
RCS_EscapeArrow
```

```text
name
```

Best display name chosen by the renderer. Usually one of:

```text
A_Name
%name%
A_Victim
C_Model
C_AnimModel0
```

Examples:

```text
Victim1
RCS_VictimRescue2
valve1
hose1
door6
hinttrigger
```

```text
x
y
z
```

Numeric engine coordinates parsed from `location`, `ZZ_location`, or `origin`.

The map diagrams use:

```text
x -> horizontal map axis
z -> horizontal map axis
y -> height/elevation
```

```text
victim_link
```

For `ALT` rows, this is the linked victim internal id, such as:

```text
RCS_VictimRescue2
```

For other rows, this is usually empty.

```text
scripts
```

Combined non-empty script fields from the entity, joined with `|`.

Example:

```text
touch: tell player (touchhose 15) | touch: hint 4 (...)
```

This lets you see why the renderer classified a prop as `HOSE`, `VALVE`, etc.

### `extracted/<map>/important_entities.csv`

Examples:

```text
extracted/training/important_entities.csv
extracted/subway_a/important_entities.csv
extracted/subway_b/important_entities.csv
```

What it is:

A filtered version of `annotated_entities.csv` that removes the noisier categories:

```text
TRIGGER
PROP
```

This is usually the better table for route planning because it focuses on:

```text
START
VICTIM
ALT
FIRE
HOSE
VALVE
EXIT
DOOR
```

### `extracted/<map>/entities.txt`

What it is:

A human-readable dump of every decoded BSP entity in that map. It is the closest thing currently available to a text export of the map's entity layer.

Use this when `annotated_entities.csv` does not include enough detail.

Example structure:

```text
entity 191
A_Name: victim1
classname: RCS_VictimRescue
D_Type: man
location: 1104 -60 -4976
S_Script01: pickup: ifnot gotvictim 1 (*Cmd3c)
...
```

### `extracted/<map>/summary.json`

What it is:

A structured JSON summary of the BSP file.

It includes:

```text
header
chunks
models
counts
textures
entities
```

Useful fields:

```text
header.tag       GBSP
header.version   15
counts.faces     number of decoded faces
counts.verts     number of decoded vertices
counts.entities  number of decoded entities
textures         texture names used by the BSP
entities         full decoded entity dictionaries
```

### `viewer/data/<map>.json`

Examples:

```text
viewer/data/training.json
viewer/data/subway_a.json
viewer/data/subway_b.json
```

What it is:

Preprocessed JSON for the browser-based 3D viewer.

It contains:

```text
bounds
walls
floors
markers
```

Fields:

```text
bounds   min/max X/Z values for camera framing
walls    simplified wall line segments extruded by the viewer
floors   simplified floor polygon outlines
markers  gameplay labels and entity metadata
```

Marker fields include:

```text
label
category
entity_index
class
name
x
y
z
victim_link
scripts
internal_id
victim_label
```

The `victim_label` field is important for RNG toggles. It links an `ALT` marker like `A1` back to a display victim like `V1`.

## Map + Table PDF Sheets

Additional PDF sheets were generated so the clear top-down map and the corresponding entity table live together in one document.

Files:

```text
extracted/<stage>/<stage>_topdown_stage_clear_with_table.pdf
extracted/<stage>/<stage>_topdown_annotated_with_table.pdf
```

What they contain:

- A clear top-down map section based on the same decoded data as `topdown_stage_clear.svg`.
- A large table below the map using all rows from `annotated_entities.csv`.
- Searchable/selectable PDF text for the table.
- PDF map panels are vertically flipped relative to the earlier first-pass PDFs so they read as a conventional top-down view rather than bottom-up.
- `<stage>_topdown_annotated_with_table.pdf` is the larger full-annotation variant. It includes the same annotation categories as `topdown_annotated.svg`, including `TRIGGER` and `PROP`.
- In the full-annotation PDF, marker text is intentionally compact: only the table label, such as `W2`, `T4`, or `P10`, is drawn in black on top of the symbol.
- In the full-annotation PDF, `TRIGGER` symbols show the entity origin point and, when available, a dashed translucent rectangle showing the referenced trigger model's X/Z footprint.

Regenerate them with:

```bash
python3 tools/build_all_maps.py
```

To regenerate one PDF manually, run:

```bash
python3 tools/render_map_table_pdf.py Bank \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/bank.bsp" \
  extracted/bank/annotated_entities.csv \
  extracted/bank/bank_topdown_stage_clear_with_table.pdf
```

To regenerate one full-annotation PDF manually, run:

```bash
python3 tools/render_map_table_pdf.py Bank \
  "FDNY Firefighter 1.0/FireFighter American Hero/Data/Maps/bank.bsp" \
  extracted/bank/annotated_entities.csv \
  extracted/bank/bank_topdown_annotated_with_table.pdf \
  --mode full
```
