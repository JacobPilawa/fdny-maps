# Training door motion audit

Read-only Ghidra 12.0.4 audit of the preserved `FDNYFirefighter.exe` project on 2026-10-11. The BModel script handler and motion helpers were inspected with `DecompileFunctions.java`; the call paths are summarized below. No live gameplay capture was used.

## Source behavior

- `RCS_BModel` script handling enters at `0043E680`. Its handler receiver is the ScriptActor subobject at object `+0x24`. `settime` writes receiver `+0x1B4` (BModel base `+0x1D8`); `settimescale` writes receiver `+0x1B8` (BModel base `+0x1DC`). Training's shared `Door` macro sets the motion timescale to `3.0`.
- The `openaway` command calls `0043D6A0`. If BModel state `+0x70` is `4`, the routine clears the ScriptActor queue/wait and appends the string `openaway`, then returns without directly changing the state. Otherwise it reads player position, iterates model IDs from the BModel array at `+0x1C0` (count `+0x1C4`), obtains model motion, calls `geMotion_GetPath(motion, 0)`, and proceeds only when `gePath_GetKeyframeCount(path, 1)` returns 2. It compares the player against transformed endpoints/axis, may transpose the motion transform and modify keyframe 1, then writes state `3` at `+0x70`.
- The `close` command calls `0043DA70`. If state `+0x70` is not `3`, it writes state `4` and returns. If state is `3`, it clears the ScriptActor queue/wait and appends `close`; this branch does not directly write a new state.
- The shared ScriptActor updater `00470740` processes at most one queued command per callback. This makes the queue effects source-backed but does not reveal how often BModel motion advances or when the queue owner is called relative to player and trigger callbacks.

## Training and native boundary

Training binds the `DoorOpen` and `DoorClose` macros to authored doors. The native `Door::update` instead opens from proximity, chooses a direction from its local geometry, advances a local fraction, and reverses when the player leaves. It does not yet reproduce the original BModel keyframe/state/ScriptActor path. Treat the current motion as a playable approximation; exact motion timing, keyframe evaluation, event order, and collision/render parity remain open. The original BModel motion advancement path was not identified by this audit.

Raw command implementations are available in the Ghidra project at `0043E680`, `0043D6A0`, and `0043DA70`. The native queue evidence is in [`training-script-queue-audit.md`](training-script-queue-audit.md); the concise command table is in [`training-script-command-audit.md`](training-script-command-audit.md).
