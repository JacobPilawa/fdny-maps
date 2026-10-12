# Player passout transaction

## Source path

`00450FB0` is the Player passout dispatcher reached by `00451640(player, 1)` after the strict exposure threshold. The hash-pinned original is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Raw instructions are appended to `evidence/fire-update/player-exposure-assembly.txt`; the preserved decompilation is `evidence/startup-factories/00450fb0_FUN_00450fb0.c`.

- Parameter 0 stops the animator at Player `+0x454`; parameter 1 uses `+0x430`. Both then resolve `RC_PassOut` through `004897C0` with shuffle flag 1.
- Other values skip those two animation calls but still play `RC_PassOut`.
- Parameter 2 plays `RC_PassOut`, calls `0044FF10(this, 1)`, and immediately returns. It does not write Player `+0x342C` or state `+0x18C` in this function.
- All other parameters read the global clock at `00451042`, then read Player `+0x357C` at `00451047`, store the clock to `+0x342C`, and store state 6 to `+0x18C`.
- For nonzero `+0x357C`, it calls the list helper `0048D490` on Player `+0x3564`, indexes the resulting object pointer, then invokes that object's virtual slot `+0x1C`. The callback occurs after both Player stores.

## Native component and original P-code comparison

`src/player_passout.h` preserves those service and store boundaries. Its callbacks represent animator helper `004A11A0`, shuffled chatter lookup/play, global clock read, active-object index read, active-object slot `+0x1C`, and the parameter-2 `0044FF10(this,1)` handoff. `player_passout-check` loads the hash-pinned five-case fixture and verifies the state/timestamp result, parameter-specific call counts/arguments, active-object branch, and callback order.

`tools/ghidra/PlayerPassoutPcodeOracle.java` executes original Ghidra EmulatorHelper x86 PE32 P-code from `00450FB0`. Cases cover parameters 0, 1, 2, and another value, with and without a linked active object. The saved fixture is `player-passout-pcode-oracle.json`. This verifies the original dispatcher control flow and stores under controlled services; it does not execute the animation, string/chatter, list-bounds, `0044FF10` HeroDown body, or active-object callback bodies. Native scheduling, the active object's concrete runtime meaning, and full passout gameplay remain unresolved.

Reproduce with Ghidra `-noanalysis -readOnly` and `PlayerPassoutPcodeOracle.java <output.json> <original.exe>`. The native comparison runs through `ctest --test-dir build --output-on-failure -R '^player_passout$'`.
