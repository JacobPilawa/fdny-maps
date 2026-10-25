# Custom Triggers (local only)

Start this folder's `./start.sh` with Python 3 and open the printed local URL. Restart any older server first, then reload the page. A generic static file server does not provide the export API. The hamburger menu **☰ → Custom Triggers** is available when the local backend can find the retail game. It opens a dedicated editing mode with a persistent left tool rail; **Exit** returns to the map viewer.

The backend searches ancestor folders for `FDNY.app`, then `FireFighter American Hero`. For a different installation, run `FDNY_GAME_DIR="/path/to/game/folder/or/FDNY.app" ./start.sh`. The app's Wine game copy takes priority over a loose copy. Files are read from the detected installation; the editor never installs an export itself.

## Make a test

1. Choose **☰ → Custom Triggers**. The current rescue mission loads automatically (otherwise Training). Use **Change mission…** at the bottom only when you want a different map. Loading the already-open mission keeps its volumes.
2. Choose **New volume** (**N**). The yellow box is a preview, not a placed object: click the grid for a cube, or drag to draw a footprint. Placement uses the **Height** value; **Pick floor** sets it from a visible surface. **Top / Perspective** change the camera. Right-drag pans and scroll zooms without moving a preview or placed volume.
3. After clicking, the volume is committed and selected. It no longer follows the pointer. Use **Select (V)** to click any placed volume or its row in the **Volumes** list. **Move (G)** lets you drag the box along X/Z or pull its colored X/Y/Z arrows. **Resize (S)** pulls either face handle on an axis and keeps the opposite face fixed. Snap uses 1/8/16/32-unit steps; Shift gives fine movement. Drag empty space to orbit. Right-click does not hide map geometry in this mode.
4. **Copy / Paste** (Ctrl/⌘ C / V) and **Duplicate** (Ctrl/⌘ D) make independent volumes with unique trigger IDs. Paste offsets the new copy; Duplicate leaves the copied-volume clipboard intact. **Delete** removes only the selected placed volume. **Undo / Redo** (Ctrl/⌘ Z / Shift Z, also Ctrl Y) restore placement, movement, resize, deletion, properties and order, up to 50 edits. Undo during a gesture cancels that gesture first. **Escape** cancels a preview/drag; it never deletes a placed volume. **Find (F)** frames the selection; paste and undo also reveal it when off-screen.
5. The sticky toolbar keeps these editing actions visible. Use **Volumes** for geometry editing and **Split order** to review/reorder checkpoints. **Show exact values & hint** exposes numeric coordinates, trigger ID and hint settings when needed. **Edit an authored volume…** edits a map's existing hint in fixed bounds; Duplicate turns it into a new editable volume.
6. **Save project** (Ctrl/⌘ S) downloads editable JSON; **Open project** restores it against the same source BSP. Drafts and copied-volume clipboard are in memory. Exit/reopen keeps the draft; save before reloading. Undo history starts fresh on a different mission or imported project.
7. **Export package** downloads one ZIP containing game files, installer/restorer, saved project, timing files and instructions. Only placed volumes are exported; finish or cancel a preview first. Unzip the entire archive into one folder.

### Custom subsplits

The **Split order** summary lists every exported segment in order: custom checkpoints first, followed by the normal stage finishes marked **Default**. Open the **Split order** tab to see this summary and use the arrow buttons to reorder checkpoints. Click a custom split to edit its volume. The **Volumes** tab lists all placed boxes, including hint-only volumes; those do not enter the split list. The summary and exported split list share the backend's default stage sequence.

New boxes have **Include in splits** enabled. Set their **Label** and use the arrows in **Split order** to arrange checkpoint order. Uncheck it for a hint-only box. Old projects remain hint-only until enabled; existing hint boxes can also be marked as checkpoints.

Every export is one ZIP containing the custom game files, installer/restorer, editable project, checkpoint JSON, LiveSplit split list, and both standalone custom ASLs (`timing/FDNY_Custom_ILs.asl` and `timing/FDNY_Custom_rescue%.asl`). Generation is entirely contained in this viewer: `livesplit/custom_triggers.py` uses the bundled, unmodified v2.2 templates from [RichardFranksComputer/fdny-tas](https://github.com/RichardFranksComputer/fdny-tas/tree/660b272efd921b8aa38b862fce1dd77c7031824b/livesplit). No sibling checkout or network access is needed. The template revision is recorded in the generator; generated scripts and game assets are export outputs, not source templates.

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

### What installation changes

The installer copies only the `Data/` files listed in `manifest.json`. Each ZIP's main README lists the exact paths. It does not patch the game executable, DLLs or ASIs, and does not install LiveSplit files into the game.

| Game-relative path | Action |
| --- | --- |
| `Data/Maps/fdny_edit.bsp` | Add the cloned source map with custom trigger geometry and scripts. The original source BSP stays in place. |
| `Data/Maps/fdny_edit.evs` | Add the rewritten map script and checkpoint zero-initialization (present when the source has a script or checkpoints exist). |
| `Data/Maps/fdny_edit.brf` | Add the copied source briefing. |
| `Data/Maps/fdny_edit.bmp` | Add the copied source preview image. |
| `Data/Scripts/fdnyedit_*.evs` | Add the map's required script dependencies, with namespaced references. The exact set depends on the source mission. |
| `Data/ResDefs/thegame.lev` | Replace the catalog with a version whose first Hose slot points to `fdny_edit`, title `Custom trigger test`, and `rescue.rrf`. Other catalog entries are preserved. |
| `.fdny-level-edit-backup/` | Create backups of overwritten files and `state.json` recording hashes and added paths. Normally the only stock file replaced is the catalog. |

Before copying, it verifies the package hashes and expected original game-file hashes; without `--install` it only reports that check. During installation it backs up overwritten files, records added paths, then copies the listed files into the game directory. If a copy raises an I/O error, it attempts to roll back. Restore verifies installed/backup hashes, puts overwritten originals back, deletes added files, and removes the backup. It refuses changed files instead of overwriting later edits.

This reuses the first Hose menu slot; it does not add another menu entry or overwrite the source rescue mission. Exits to subsequent stages still load stock maps. Only one custom test can be installed at a time; restore before installing another export.

## Format and scope

`level_edit.py` reads GBSP v15 records and preserves raw entity key/value bytes, pair order, duplicate keys and chunk order. Existing-text-only editing changes the entity chunk. New volumes clone a supported nonblocking trigger brush into appended, independent models, nodes, planes, leaves, faces, vertices and corner colors; existing geometry records remain byte-identical. Node bounds account for the retail loader's 128-unit padding. The trigger links to the new model through a `%Model%` entity and `C_Model`.

The source rescue map stays installed unchanged. The exported map key is `fdny_edit`. Only the first Hose catalog entry changes; its menu position is preserved. Its resource set becomes `rescue.rrf`, with the source mission's music/time settings. Copied EVS dependencies are namespaced, and source-specific script `LEVEL` conditions are retargeted. Other script conditions and exits retain their authored behavior. In particular, exits to later stages still lead to stock maps. Native executable logic that recognizes particular level names is not rewritten, so a cloned mission is not promised to behave identically in every respect. Game saves/unlock behavior and triggering in the retail executable need user testing.

The local server binds to loopback and rejects cross-site API requests. Static builds include only the UI modules, with the button hidden and no export backend or retail files added to deployment. Original retail assets in an export are for the owner's local testing, not redistribution.

## Checks

```sh
python3 source/check_level_edit.py
python3 source/check_level_api.py
node source/check_level_editor.mjs
python3 -I source/check_livesplit.py
node source/check_recording.mjs
node source/check_recording_render.mjs
node source/check_features.mjs
```

These cover byte-exact round trips of all 17 retail maps; two independent new boxes and text-only edits for all eight rescue missions; plane/vertex/topology checks; catalog preservation; archive hashes; temporary installation/restoration including changed-file refusal; the local HTTP API; CPU-side uncommitted previews, grid heights, footprint drawing, pointer capture/cancellation, body/axis transforms, snapping, copy/paste/duplicate, undo/redo, keyboard shortcuts, selection, off-screen recovery and disposal; and existing recording/map behavior. They do not launch the game or drive the browser. Visual testing of the dedicated editor mode and live gameplay remain manual.
