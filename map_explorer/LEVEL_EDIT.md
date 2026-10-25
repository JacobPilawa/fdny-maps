# Custom Triggers (local only)

Start this folder's `./start.sh` with Python 3 and open the printed local URL. Restart any older server first, then reload the page. A generic static file server does not provide the export API. The hamburger menu **☰ → Custom Triggers** is available when the local backend can find the retail game. It opens a dedicated editing mode with a persistent left tool rail; **Exit** returns to the map viewer.

The backend searches ancestor folders for `FDNY.app`, then `FireFighter American Hero`. For a different installation, run `FDNY_GAME_DIR="/path/to/game/folder/or/FDNY.app" ./start.sh`. The app's Wine game copy takes priority over a loose copy. Files are read from the detected installation; the editor never installs an export itself.

## Make a test

1. Choose **☰ → Custom Triggers**. The current rescue mission loads automatically (otherwise Training); choose another mission and press **Load** when needed. Viewer sidebars give way to a dedicated editor rail and unobstructed canvas.
2. The horizontal placement grid starts at the player spawn's inferred floor height (body Y minus 30). Use **Pick floor** and click visible geometry for an exact surface height, or raise/lower the grid in snap increments. **Box base** aligns it with the selected box. **Top** gives a clear plan view; **Perspective** restores an angled view around the selection or spawn.
3. Press **Add trigger**. A live cube preview follows the pointer on the grid. **Click** to place it, or **click-drag** to draw its X/Z footprint; its bottom stays on the grid. Grid placement also works in empty space, so it is not a promise of a walkable floor. Right-drag pans and scroll zooms; switch out of placement to left-drag orbit. After placement, **Move** drags the red X, green Y or blue Z arrow/shaft. **Resize** drags either face handle on an axis; the opposite face stays fixed. Snap selects 1/8/16/32-unit steps (grid lines have a minimum 16-unit spacing). Shift gives fine transform movement; Escape cancels a drag or leaves placement. Trigger selection, properties and tools stay visible in the rail. **Focus** frames the box.
4. Optionally use **Use existing…** to change an authored volume's hint without adding brush geometry. Existing boxes stay fixed. A touch-hint slot is replaced, or an empty script slot is used; other actions remain. An export fails if no slot is available.
5. **Save project** downloads editable JSON. **Open project** restores it against the same source BSP. Drafts are held in memory until saved. Exit and reopen the editor without losing the draft. Reloading the page requires opening the saved project again.
6. **Export level + splits** downloads a ZIP containing modified retail files, the project, a hash manifest, timing files, README and Python installer. Unzip the entire archive into one folder.

### Custom subsplits

The **Split order** summary lists every exported segment in order: custom checkpoints first, followed by the normal stage finishes marked **Default**. Click a custom split to select its volume. Hint-only volumes appear separately and never enter the split list. The summary and exported split list share the backend's default stage sequence.

New boxes have **Include in splits** enabled. Set their split labels and use **↑ Earlier / ↓ Later** to arrange checkpoint order. Uncheck it for a hint-only box. Old projects remain hint-only until enabled; existing hint boxes can also be marked as checkpoints.

The export includes `timing/custom-splits.lss` (ordered segment names) and `timing/checkpoints.json` (the reader profile). When a sibling `fdny-tas/livesplit/custom_triggers.py` is available, it also bundles `FDNY_Custom_ILs.asl` and `FDNY_Custom_rescue%.asl`. Set `FDNY_TAS_DIR=/path/to/fdny-tas` before starting the server if that checkout lives elsewhere. The stock ASLs stay unchanged; these are generated standalone variants. Without the generator, the ZIP's timing README explains that ASL generation is still required.

After installing the level, open the `.lss` in LiveSplit, add **Control → Scriptable Auto Splitter** in Edit Layout, and select the bundled **FDNY_Custom_ILs.asl**. Use **Game Time** and only one autosplitter. Python/JSON/FDNYCore are not needed while running. The rescue% variant keeps manual resets and needs a run-wide split list if used across missions. Generated scripts pass synthetic action checks and LiveSplit parser/compiler loading; real-game contact timing and cloned-map completion still need gameplay validation. Read `timing/README.txt` in the ZIP.

Checkpoint triggers use actual game events, not virtual position tests. Their touch script sets a dedicated `FDNY_CP_<id>` variable to `1` and displays the hint. The exported map EVS initializes each variable to `0` on load. IDs derive from trigger names and remain stable when labels/order change; renaming a trigger changes its ID. The JSON identifies variables, labels, trigger names, bounds, source/runtime maps and the exported BSP hash. It supplies no speculative memory offsets. Place checkpoints away from the initial player spawn so the reader can observe their zero state before the first touch. The generated reader follows the native global script-variable table, arms after all markers are zero, and counts them once in listed order. It blocks splits on missing/out-of-order markers; restart the level to recover. Simultaneous observations are queued in profile order, which does not establish exact contact timestamps. Cloned-map wins use the player mission-result field; stock maps keep upstream detection. Live gameplay validation remains necessary.

For Training, the split list is your checkpoints followed by `training complete`. Multi-stage missions retain subsequent stock stages after the edited first stage. Pair the `.lss` with its generated custom ASL, not the unmodified stock ASL. The timing README describes setup and validation limits.

Hint-only boxes use `touch: hint <seconds> (<text>)`; checkpoints wrap the hint and variable assignment in a `touch: cmd [...]` script. Use 1–240 printable ASCII characters, without parentheses. Duration is 1–30 seconds. This deliberately small proof of concept supports up to 32 trigger edits and nonblocking axis-aligned boxes. It is not a geometry editor, arbitrary script editor, or native-port mod exporter.

## Install and restore

Quit the game. In a Terminal opened in the extracted export folder:

```sh
python3 install.py "/path/to/FDNY.app"
python3 install.py "/path/to/FDNY.app" --install
```

The first command only checks files. The second installs them after validating the export and existing file hashes. A direct `FireFighter American Hero` game-folder path works too. Select **Custom trigger test**, which occupies the former **Pier Fire -Hose-** first Hose slot, and walk into the box. The slot now uses rescue resources and the chosen rescue map.

To undo the test, quit the game and run from the same extracted export:

```sh
python3 install.py "/path/to/FDNY.app" --restore
```

The installer backs up overwritten files under `.fdny-level-edit-backup` inside the game folder, records added files, and refuses to restore changed files. Keep this backup and the export until you restore. **Restore before exporting another version**, then install the new export. This keeps baseline hashes and backups consistent.

For manual inspection, all generated game files live under the ZIP's `Data/` directory. It includes `Maps/fdny_edit.bsp`, the source briefing/preview and map script when present, namespaced script dependencies, and `ResDefs/thegame.lev`. Avoid dragging only a BSP over `dock_h.bsp`: the Hose catalog would still select Hose resources. Use the provided installer to replace the full required set and preserve a verified restore path.

## Format and scope

`level_edit.py` reads GBSP v15 records and preserves raw entity key/value bytes, pair order, duplicate keys and chunk order. Existing-text-only editing changes the entity chunk. New volumes clone a supported nonblocking trigger brush into appended, independent models, nodes, planes, leaves, faces, vertices and corner colors; existing geometry records remain byte-identical. Node bounds account for the retail loader's 128-unit padding. The trigger links to the new model through a `%Model%` entity and `C_Model`.

The source rescue map stays installed unchanged. The exported map key is `fdny_edit`. Only the first Hose catalog entry changes; its menu position is preserved. Its resource set becomes `rescue.rrf`, with the source mission's music/time settings. Copied EVS dependencies are namespaced, and source-specific script `LEVEL` conditions are retargeted. Other script conditions and exits retain their authored behavior. In particular, exits to later stages still lead to stock maps. Native executable logic that recognizes particular level names is not rewritten, so a cloned mission is not promised to behave identically in every respect. Game saves/unlock behavior and triggering in the retail executable need user testing.

The local server binds to loopback and rejects cross-site API requests. Static builds include only the UI modules, with the button hidden and no export backend or retail files added to deployment. Original retail assets in an export are for the owner's local testing, not redistribution.

## Checks

```sh
python3 source/check_level_edit.py
python3 source/check_level_api.py
node source/check_level_editor.mjs
node source/check_recording.mjs
node source/check_recording_render.mjs
node source/check_features.mjs
```

These cover byte-exact round trips of all 17 retail maps; two independent new boxes and text-only edits for all eight rescue missions; plane/vertex/topology checks; catalog preservation; archive hashes; temporary installation/restoration including changed-file refusal; the local HTTP API; CPU-side grid previews, grid heights, footprint drawing, pointer capture/cancellation, axis transforms, snapping and disposal; and existing recording/map behavior. They do not launch the game or drive the browser. Visual testing of the dedicated editor mode and live gameplay remain manual.
