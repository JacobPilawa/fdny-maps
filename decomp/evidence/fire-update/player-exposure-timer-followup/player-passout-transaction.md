# Player passout transaction

## Source path

`00450FB0` is the Player passout dispatcher reached by `00451640(player, 1)` after the strict exposure threshold. The hash-pinned original is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Raw instructions are appended to `evidence/fire-update/player-exposure-assembly.txt`; the preserved decompilation is `evidence/startup-factories/00450fb0_FUN_00450fb0.c`.

- Parameter 0 stops the animator at Player `+0x454`; parameter 1 uses `+0x430`. Both then resolve `RC_PassOut` through `004897C0` with shuffle flag 1.
- Other values skip those two animation calls but still play `RC_PassOut`.
- Parameter 2 plays `RC_PassOut`, calls `0044FF10(this, 1)`, and immediately returns. It does not write Player `+0x342C` or state `+0x18C` in this function.
- All other parameters read the global clock at `00451042`, then read Player `+0x357C` at `00451047`, store the clock to `+0x342C`, and store state 6 to `+0x18C`.
- Player `+0x3564` is the item-pointer array and `+0x357C` is its selected index. For nonzero `+0x357C`, it calls list helper `0048D490`, resolves that selected item, then invokes its virtual slot `+0x1C` after both Player stores. `0044F340` uses this same array/index pair for the selected tool's ACTION pressed/held/released callbacks at `+0x14/+0x18/+0x1C`, identifying the passout call as ACTION release.
- Training release implementations are now mapped for the extinguisher and Hose Spray. Extinguisher vtable `004E28C0` maps `+0x1C` to `00461590`; when active, it stops its animation/audio, clears `+0x600`, and stores timer/pose state. Hose Spray vtable `004E2998` maps `+0x1C` to `00463300`; when active, it stops its sound and clears `+0xC58`. The reviewed callback bodies make no shared-RNG calls. This does not identify which tool is selected in a live passout or establish audio timing.

The item-array and action-slot cross-check is in `evidence/player-tool-pose/0044f340_FUN_0044f340.c`; passout's indexed lookup is in `evidence/player-tool-pose/state-offsets/00450fb0_FUN_00450fb0.c`. Training slot 1 is the Axe, slot 2 the extinguisher, and slot 4 Hose Spray (`evidence/hose-spray/item-selection-pcode-audit.md`). Extinguisher slot words and callback body are in `evidence/item-pose/live-extinguisher/vtable-methods/vtable-pointer-words.tsv` and `evidence/tool-decompiled/00461590_FUN_00461590.c`. Hose Spray's constructed vtable and release body are in `evidence/hose-spray/hose-spray-constructed-vtables.tsv` and `evidence/tool-decompiled/00463300_FUN_00463300.c`. Axe's `+0x1C` release body is not mapped; therefore the known extinguisher/Hose callbacks do not form a complete selected-slot adapter, and this passout path is not connected to the native `main.cpp` interaction loop.

## Native component and original P-code comparison

`src/player_passout.h` preserves those service and store boundaries. Its callbacks represent animator helper `004A11A0`, shuffled chatter lookup/play, global clock read, selected-item index read/release, and the parameter-2 `0044FF10(this,1)` handoff. `player_passout-check` loads the hash-pinned five-case fixture and verifies the state/timestamp result, parameter-specific call counts/arguments, selected-item branch, and callback order.

`tools/ghidra/PlayerPassoutPcodeOracle.java` executes original Ghidra EmulatorHelper x86 PE32 P-code from `00450FB0`. Cases cover parameters 0, 1, 2, and another value, with and without a linked selected item. The saved fixture is `player-passout-pcode-oracle.json`. This verifies the original dispatcher control flow and stores under controlled services; it does not execute animation, string/chatter, list-bounds, `0044FF10` HeroDown, or selected-item release bodies. Native scheduling, live selected-slot identity, real tool/audio integration, and full passout gameplay remain unresolved.

Reproduce with Ghidra `-noanalysis -readOnly` and `PlayerPassoutPcodeOracle.java <output.json> <original.exe>`. The native comparison runs through `ctest --test-dir build --output-on-failure -R '^player_passout$'`.
