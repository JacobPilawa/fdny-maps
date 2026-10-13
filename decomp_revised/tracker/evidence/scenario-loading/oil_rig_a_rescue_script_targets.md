# Oil Rig rescue stages and targets

## Inputs and scope

- `oil_rig_A.evs`, `oil_rig_B.evs`, `oil_rig_C.evs`, `oil_rig_D.evs`, and the extra `oil_rig_r.evs` all have SHA-256 `871cf09e94cd79cbc15c5593e90b407c12ef71b28011211baa3e41c3f6acd70f`.
- The copied map set contains no `oil_rig_R.bsp`, and the level catalog has no `oil_rig_r` entry. Treat the extra EVS as an orphaned duplicate unless further evidence gives it a role.
- BSP SHA-256 values are pinned in [`rescue_bmodel_targets.csv`](../../rescue_bmodel_targets.csv) and [`rescue_misc_emitter_targets.csv`](../../rescue_misc_emitter_targets.csv).
- Entity records were parsed from copied GBSP files with `tools/gbsp_v15.py`; no retail input was modified.

## Stage progression

The catalogued rescue entry `oil_rig_a` starts at `oil_rig_A.bsp`. `final_exit` BoundsTriggers request each next BSP in order. D ends at the player escape command.

| Stage BSP | Exit entity | Exit target | Script action | Static result |
| --- | ---: | --- | --- | --- |
| `oil_rig_A.bsp` | 67 | `final_exit` | `touch: tell player (finalexitloadmap oil_rig_B)` | requests B |
| `oil_rig_B.bsp` | 92 | `final_exit` | `touch: tell player (finalexitloadmap oil_rig_C)` | requests C |
| `oil_rig_C.bsp` | 362 | `final_exit` | `touch: tell player (finalexitloadmap oil_rig_D)` | requests D |
| `oil_rig_D.bsp` | 73 | `exit_lifeboat2` | `touch: tell player escape` | terminal escape command; no next-stage request found |

The Player ScriptActor handler at `00451870` recognizes `finalexitloadmap` and calls `HandlePlayerFinalExitLoadMap` (`00450A70`). The helper builds `data\\maps\\<map-key>.bsp`, checks that path, and on success calls `0042A260` with a forwarded context value and `data\\resdefs\\rescue.rrf`. That function sets pending byte `this+0x68C`. `ProcessEverGameUpdate` (`0042AEA0`) consumes the byte and calls `0042AB00` with the pending descriptor; the existing World factory trace connects that loader to WorldEntity BSP loading. The static command-to-loader control path is mapped. The pending map-name field at EverGame+0x660 and its loader read are mapped. Equality between the checked command argument and forwarded context, successful runtime stage completion, and timing remain open. See [pending level update evidence](evergame_pending_level_update.md), [Player helper output](00450a70_player_finalexitloadmap_fresh.c), and [update output](0042aea0_evergame_pending_load_update_fresh.c).

## Stage-local scripted targets

The EVS files are byte-identical, while the named actor owners and their activating touch triggers belong to the stage BSP where they are used. These are sequential stage-local interactions, not cross-variant actor references.

### Stage B: warehouse catwalks

`oil_rig_B.bsp` entities 81, 85, 86, and 87 are `quake trigger*` BoundsTriggers. Their script slots bind `*Quake` and `*WarehouseCatwalks`. The sequence at EVS line 4 opens local BModels in order: entity 82 `catwalk03` (child model `catwalk03`, timescale 0.5), entity 84 `catwalk01` (matching child, timescale 2.5), and entity 83 `catwalk02` (matching child, timescale 2.5), with 0.5-second waits between commands.

### Stage C: tower stairs and steam

In `oil_rig_C.bsp`, entity 344 `quake trigger4` binds `*TowerStairs`. EVS line 7 gates the action on `Complex` and `Tower` being clear, then runs `*Quake` and `*StairsDrop`. `StairsDrop` waits 1.5 seconds, opens local BModel entity 345 `TowerStairs` (six stair child models, two clip models, timescale 2), waits one second, then sends `turnon` to `steam01`, `steam02`, and `steam03`.

Entities 346-348 are the three named `RCS_MiscEmitter` actors; each has `C_Type=3` and starts with `S_Script01=turnoff`. The map loader's [`ProcessEntitySpawnQueue`](0049c670_entity_spawn_queue_fresh.c) calls each entity's root vtable slot `+0x20` during its first batch pass. `InitializeMiscEmitterEntity` (`004424D0`) registers the parsed name through `InitializeAndRegisterScriptActor`, initializes the contained emitter, and dispatches non-null `S_Script01` through `S_Script05` bindings. Their `turnon` command reaches `DispatchMiscEmitterScriptCommand` (`004420F0`). Thus the authored stage-local actor registration and class handler are statically mapped.

Fresh initializer outputs are [`004424d0_misc_emitter_init_fresh.c`](004424d0_misc_emitter_init_fresh.c) and [`.asm`](004424d0_misc_emitter_init_fresh.asm); the command handler is described in [`004420f0_misc_emitter_dispatch.md`](004420f0_misc_emitter_dispatch.md).

## Port work remaining

- Trace the producer of the context string stored at EverGame+0x660; capture when the prior BSP unloads and each requested stage loads at runtime.
- Resolve the steam effect types and compare their state changes and visible timing against an original-game capture.
- Implement and capture the four-stage rescue flow, stage-local catwalk and stair motion, steam effects, rescue mechanics, and terminal escape behavior in the cross-platform port.
