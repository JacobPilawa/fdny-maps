# Victim release frame method

The indirect call in `0045C000` is a frame orientation update, not a destructor.

`0045DDD0` constructs the victim instance and writes `0x004DFC30` into its movement-frame vtable at entity `+0x24`. The preserved vtable export lists slot 2 (`+0x08`) as `0042BD50` and slot 3 (`+0x0C`) as `00496210`. `0045C000` pushes three float arguments `(0, 1, 0)` and calls slot 2. Ghidra's `0045C000` prototype omits these indirect-call arguments, so the callsite assembly is material evidence.

`0042BD50` copies its three scalar arguments into a 3-float vector and invokes slot `+0x0C` on the same frame. In this victim frame that resolves to `00496210`. `00496210` normalizes the vector, stores it as the frame's Y axis at `+0x10`, and reconstructs the X/Z axes with cross products and normalization. Its fallback selects a different existing axis when the current candidate is within absolute dot product `0.999` of the new Y axis; it then rebuilds the perpendicular axes. The call from `0045C000` therefore sets world-up `(0,1,0)` and orthonormalizes the previously written X/Z basis before the routine reads frame `+0x1C` to compute the victim's release position.

The behavior is established; a source-level method name is not. Treat this as a frame `set-up-axis / rebuild-basis` operation and preserve the axis fallback if exact general behavior is needed. The local export `evidence/item-pose/0045a230_FUN_0045a230.c` shows that the vector-copy helper calls `00459B80`; its saved EXE instructions normalize the vector unless its magnitude is already within the helper's accepted interval. `004476B0` is the saved cross-product helper. The raw caller/wrapper/target disassembly is in `release-frame-method-assembly.txt`.

## Actor flag `0x400`

`00481140` passes `0x400` when its boolean is true and `0` when false to `geWorld_SetActorFlags` for the actor pointer at the `+0x170` subobject. This proves which mask is toggled, but the preserved local Genesis data does not document its render or visibility meaning. `evidence/genesis-actor-source/` contains selected pose and frame source only (`pose.c`, `vkframe.c`, `qkframe.c`, `path.h`); `evidence/original/` contains the shipped binaries and game resources, with no Genesis SDK headers or actor-flag enum. Do not label `0x400` as a visibility flag based on this preserved local evidence alone.

## Sources

- `evidence/startup-factories/0045c000_FUN_0045c000.c`
- `evidence/victim-ai-audit/runtime-flow/0045ddd0_FUN_0045ddd0.c`
- `evidence/hose-spray/prop-model/vtable-words.tsv`
- `evidence/player-tool-pose/frame-functions/0042bd50_FUN_0042bd50.c` (the decompiler omits the hidden vector forwarding)
- `evidence/player-tool-pose/view-methods/00496210_FUN_00496210.c`
- `evidence/item-pose/0045a230_FUN_0045a230.c`
- `evidence/item-pose/00495c70_FUN_00495c70.c`
- `evidence/victim-render/vector-cross-helper-assembly.txt`
- `evidence/item-pose/live-extinguisher/message-routing/00481140_FUN_00481140.c`
- `evidence/genesis-actor-source/provenance.json`

The audited executable is `evidence/original/FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`).
