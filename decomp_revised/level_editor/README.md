# FDNY Level Workshop

The editor reads the decoded library in the neighboring FDNY map explorer and
lets you build custom levels for this repository's native port. It does not
write to the explorer or original game data, and it does not produce a retail
Windows `.bsp` file.

## Start here

Double-click `Open FDNY Level Editor.command` in the repository root and leave
its Terminal window open. The editor runs on `http://127.0.0.1:4190/`. If it was
already running during this revision, stop the old Terminal process with Ctrl-C,
start the command again, and refresh the editor page to load the new asset API.

Saved projects live in `level_editor/levels/`. The editor opens the Empty Test
Room by default. Choose **New level** for a fresh room. Your saved levels stay
in the dropdown, with separate project JSON files so you can copy or back them
up. The server only binds to 127.0.0.1.

## Build with the library

The left Asset Browser has four tabs:

- **Shapes** places editable solid floors, walls, blocks, ramps, fires, survivor
  actors, and the player start.
- **BSP assets** searches brush models from the 17 maps in the map explorer.
  Search by source map, entity name, type, or BSP model ID. The selected model
  appears in the inspector and inventory thumbnails with its decoded source
  geometry, UVs, and textures. Choose Place, then click the level to make an instance. The model keeps its original
  multi-surface mesh, UVs and source materials. Collision uses a fitted convex
  box around the original BSP brush bounds.
- **Actors** lists the survivor and fire setups already used in the maps. A
  survivor preset carries its actor type and `roam` radius into the new level;
  character previews use extracted ACT geometry and textures. A fire preset
  carries its original radius, height, smoke and spray settings.
- **Textures** searches the decoded retail texture library across all 17 maps. Select one and then
  click the level object whose material you want to override. Use the curved
  arrow in the inspector to restore that object's original/default material.

Search and the source map/type filters update the item grid as you type. BSP
cards show small textured projections of the actual mesh; selecting an item
shows a larger, rotatable 3D preview. BSP objects keep their individual source
materials unless you explicitly choose a material override. Fire previews use
an extracted retail flame sprite. Survivor previews use ACT bind-pose meshes
and textures; runtime animation is not shown in the editor.

Click a library card to see its preview. **Select** is the default camera mode:
left-drag orbits, right-drag pans, and the wheel zooms. Click an object to
select it. Use **Move** to drag the selected object on a horizontal plane, and
**Place** to add the selected asset with camera navigation still available.
The placement height field sets the target Y plane. Adjust position, rotation,
scale, dimensions, surface texture, fire radius/height/spray time, survivor
type and roam radius in the inspector. Mark ceilings and roofs in the inspector;
the viewport roof toggle hides them consistently regardless of camera angle.
Grid snap can be disabled or changed from 8 to 128 game units. Use the scene
hierarchy to select covered objects. Duplicate/Delete, Undo/Redo, arrow-key
nudging, 15-degree toolbar turns, and arbitrary rotation entry are available.
The roof toggle only affects the editor preview.

Fire size is edited as gameplay radius and height, and its initial strength,
smoke, floor alignment, spray time and spread setting are available in the
inspector. Initial strength is a native-port authoring property; it sets the
starting suppression state and the matching fire tint/opacity. Named fires enable the
native runtime's existing spread path. Survivors can be man, woman, girl, boy,
or dog. Roaming uses the port's existing authored `roam <radius>` behavior.
Custom props are BSP brush-model instances, not scripted door controllers.

## Save and play

**Save level** compiles the active project into a native scene package and
refreshes the app's custom catalog. Restart the native game, open **GAME**, and
press Down past the eight original Rescue choices to find custom levels. The
chooser paginates as more levels are added. **Playtest** saves the level and
launches the native viewer directly on that package. A running viewer does not
hot-reload editor changes.

Project source files are saved at `level_editor/levels/`. Generated level
packages are at `.local/exports/custom_*/`. The normal rescue-all app builder
copies saved custom packages and a generated `custom_missions.json` into the
app bundle. The eight-entry retail catalog remains source-ordered. The native
chooser, rather than the retail Windows game, loads custom levels.

## Coordinate and terrain notes

X/Z are horizontal; Y is up. Brush positions mark the bottom center. The player
start uses the player body origin, so Y=32 begins with the hull above a floor
whose top is at Y=0. Floors are solid brush boxes; ramps are solid wedges that
rise toward +Z and can be turned freely. A ramp normal with vertical component
below 0.5 can be too steep for the current player movement code. Imported brush
collision uses a fitted convex box around each imported brush’s source bounds.
The box rotates with the model; nonuniform scaling adjusts its three axes.

This version builds geometry from native BSP model surfaces, plus reusable
fires and survivors. It does not edit source retail maps or preserve their
mission scripts. Doors, exits, triggers, lighting bake, CSG subtraction,
arbitrary mesh import, animated brush paths, and mission completion authoring
remain future work. Gameplay is bounded by the unfinished state of the native
port. Existing game-data packages and local decoded assets are required for
texture overrides and source brush models. No npm install or internet access is
required.
