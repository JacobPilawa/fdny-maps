# Pending level request and World load path

## Static chain

The Oil Rig final_exit BoundsTriggers issue tell player (finalexitloadmap oil_rig_B/C/D) as the mission advances. The Player ScriptActor handler at 00451870 recognizes finalexitloadmap and calls HandlePlayerFinalExitLoadMap (00450A70). That helper builds data\\maps\\<command-key>.bsp and checks it. If the file is missing, it logs File not found from rescue\\PlayerRescueEnt.cpp.

On success, HandlePlayerFinalExitLoadMap calls SetPendingLevelRequest (0042A260) with two values: its separate context string and data\\resdefs\\rescue.rrf. The setter stores the first value at EverGame+0x660, the resource path at +0x668, and sets the pending flag at +0x68C. When an active level record exists at +0x688, it also copies the label and timing metadata into adjacent pending fields; otherwise it seeds the <Spawned Level> fallback values.

ProcessEverGameUpdate (0042AEA0) checks and clears +0x68C, then passes the pending block at EverGame+0x660 to ProcessLevelLoadRequest (0042AB00). The block starts with a map-name string at +0x00, a label at +0x04, and a resource-definition path at +0x08. 00403E40 reads the first field and appends .bsp; ProcessLevelLoadRequest then reaches the World factory and spawn queue. The path through ProcessEntitySpawnQueue (0049C670) to the WorldEntity BSP loader (0049E4D0) is documented in world_factory_resolution.md. A this-adjustment thunk at 0042BC60 (bytes 8B 89 B4 00 00 00 E9 35 F2 FF FF) points to ProcessEverGameUpdate; data at 004DF9B8 references the thunk.

Fresh outputs: 0042a260_set_pending_level_request_fresh.c and .asm; 0042ab00_process_level_load_request_fresh.c and .asm; 0042aea0_evergame_pending_load_update_fresh.c and .asm; and the Player helper 00450a70_player_finalexitloadmap_fresh.c and .asm. Field evidence and confidence are summarized in pending_level_request_fields.md. All executable evidence is pinned to SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

## Limits and next trace

The pending block's map-name field is identified at EverGame+0x660, and the loader's .bsp builder reads it from offset +0x00. The command argument used by the file-existence check and the separate context string stored in that field are not compared in this static path. Trace the context's producer and capture a successful stage transition before treating them as equal. Runtime completion, callback cadence, and visual parity remain unverified.
