# Pending level request and World load path

## Static chain

The Oil Rig `final_exit` BoundsTriggers issue `tell player (finalexitloadmap oil_rig_B/C/D)` as the mission advances. The Player ScriptActor handler at `00451870` recognizes `finalexitloadmap` and calls `HandlePlayerFinalExitLoadMap` (`00450A70`). That helper builds `data\\maps\\<map-key>.bsp` and checks the path. If the file is missing, it logs `File not found` from `rescue\\PlayerRescueEnt.cpp`.

On the success branch, `00450A70` calls `0042A260` on global context `DAT_00502AAC`, passing a forwarded context value and `data\\resdefs\\rescue.rrf`. `0042A260` sets byte `this+0x68C`. `ProcessEverGameUpdate` (`0042AEA0`) checks this byte, clears it, and calls `0042AB00` with the level descriptor stored at `this+0x660`.

`0042AB00` logs `TEverGame: Loading level` from `main\\evergame.cpp`, sets its active level pointer, resolves the `World` factory, and pushes a `World` spawn request to the queue. The selected-level factory and WorldEntity trace in [`world_factory_resolution.md`](world_factory_resolution.md) maps that request through `ProcessEntitySpawnQueue` (`0049C670`) to the WorldEntity BSP loader (`0049E4D0`). A this-adjustment thunk at `0042BC60` (bytes `8B 89 B4 00 00 00 E9 35 F2 FF FF`) points to `ProcessEverGameUpdate`; data at `004DF9B8` references the thunk.

Fresh update-function outputs are [`0042aea0_evergame_pending_load_update_fresh.c`](0042aea0_evergame_pending_load_update_fresh.c) and [`.asm`](0042aea0_evergame_pending_load_update_fresh.asm). The Player helper output is [`00450a70_player_finalexitloadmap_fresh.c`](00450a70_player_finalexitloadmap_fresh.c) and [`.asm`](00450a70_player_finalexitloadmap_fresh.asm). All executable evidence is pinned to SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Limits and next trace

This maps the static command-to-pending-load-to-World-loader control path. The forwarded context value's identity and exact map-key field inside the pending descriptor still need a register/data-flow audit. No original-game runtime capture has confirmed successful stage transitions, frame timing, or visible parity.
