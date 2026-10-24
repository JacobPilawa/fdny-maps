# Retail trigger editor (local only)

Start this folder's `./start.sh` with Python 3 and open the printed local URL. Restart any older server first, then reload the page. A generic static file server does not provide the export API. The tools menu shows **Level Edit**, immediately after **Upload recording**, when the local backend can find the retail game.

The backend searches ancestor folders for `FDNY.app`, then `FireFighter American Hero`. For a different installation, run `FDNY_GAME_DIR="/path/to/game/folder/or/FDNY.app" ./start.sh`. The app's Wine game copy takes priority over a loose copy. Files are read from the detected installation; the editor never installs an export itself.

## Make a test

1. Choose **⋯ → Level Edit**, choose a rescue mission and press **Load**. The viewer enters 3D.
2. Press **Add trigger**, then click a visible floor. The bottom of the box sits on that surface. Orbit, zoom, hide surfaces or use the normal height controls to expose the desired location. A box is orange when selected, teal otherwise, and visible through walls.
3. Set its name, hint text, display duration, center and dimensions. Y is vertical, in engine units. **Place on surface** repositions it; **Focus** brings it into view. Click a box or use the edited-trigger dropdown to select it.
4. Optionally use **Edit existing hint…** to change an authored volume's hint without adding brush geometry. Existing boxes stay fixed. A touch-hint slot is replaced, or an empty script slot is used; other actions remain. An export fails if no slot is available.
5. **Save project** downloads editable JSON. **Open project** restores it against the same source BSP. Drafts are held in memory until saved. Collapse with the up arrow, resize from the panel's lower-right corner, or close and reopen it without losing the draft.
6. **Export game files** downloads a ZIP containing modified retail files, the project, a hash manifest, README and Python installer. Unzip the entire archive into one folder.

Messages currently use `touch: hint <seconds> (<text>)`. Use 1–240 printable ASCII characters, without parentheses. Duration is 1–30 seconds. This deliberately small proof of concept supports up to 32 trigger edits and nonblocking axis-aligned boxes. It is not a geometry editor, arbitrary script editor, or native-port mod exporter.

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

These cover byte-exact round trips of all 17 retail maps; two independent new boxes and text-only edits for all eight rescue missions; plane/vertex/topology checks; catalog preservation; archive hashes; temporary installation/restoration including changed-file refusal; the local HTTP API; CPU-side 3D placement and disposal; and existing recording/map behavior. They do not launch the game or drive the browser. Live gameplay and visual testing remain manual.
