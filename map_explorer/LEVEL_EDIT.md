# Practice checkpoints

Requires an FDNYCore build with practice-checkpoint file support.

1. Open **☰ → Custom Triggers**. Choose a rescue mission.
2. **New volume (N)**: click the placement grid, or drag a footprint. **Pick floor**
   sets its height. Use **Move (G)** and **Resize (S)** with the axis handles.
3. Give it a label. Every placed volume is a timed practice checkpoint.
4. **Export checkpoints** downloads `<mission>.txt`. Put it in the game's
   `Checkpoints` folder beside `FDNYFirefighter.exe` (create the folder if needed).
5. Start/restart that mission. Press **V** to enable timestamps once; FDNYCore
   saves the preference. Each native checkpoint shows `Label mm:ss:ddd` for four
   seconds when reached. Restarting resets the timer and checkpoint hits.

Example: `Checkpoints/training.txt`. Replace that exact file to update the set,
then restart Training. Delete it and restart to remove the set. One set per
mission, up to 32 checkpoints. No other game files need replacing after FDNYCore
is installed. No console import, custom BSP, installer or LiveSplit is involved.

**Fly (F)** uses the viewer's normal controls: WASD, Q/E, drag/arrows to look,
Shift faster and scroll to change speed. F returns to orbit; **Find (.)** frames
the selected volume. Click empty space to deselect, or a placed box to select it.
Copy/paste/duplicate and undo/redo work with their normal Ctrl/⌘ shortcuts.
**Save project / Open project** keep editable JSON separate from the game file;
older projects retain their volumes and labels. Exit/reopen preserves the draft
in memory; save before reloading the page.

FDNYCore uses native `RCS_BoundsTrigger` contacts and the game's existing clock.
The clock starts at the first playing frame, pauses with the game's clock, and
resets on every level entry. Three digits display milliseconds; resolution is
still the game's frame/clock resolution. A checkpoint placed over spawn waits
until you leave and re-enter. Other checkpoints count on their first entry.
Toggling off suppresses timestamps; toggling back on keeps hits already recorded
in the current attempt. The setting is practice-only. Console equivalents are
`fd_checkpoints 1`, `fd_checkpoints 0` and `fd_checkpoints_status`.

The public viewer exports directly in JavaScript. It needs no local game files,
backend or downloaded runtime. Labels use 1–80 printable ASCII characters, without
parentheses or `|`. Checkpoint files are readable bounds/label rows with a versioned
mission header; FDNYCore validates the whole file before creating any triggers.

Checks: `node source/check_level_editor.mjs`, `node source/check_checkpoints.mjs`;
FDNYCore tests cover file loading, native-event notifications, timer formatting,
once-per-attempt behavior and toggles. Final visual/native clock behavior needs
local gameplay verification.
