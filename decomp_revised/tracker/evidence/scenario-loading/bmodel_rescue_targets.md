# Named rescue BModel targets

## Scope

This inventory joins map-authored `tell <name> open` commands to same-stage `RCS_BModel` records with child animation models. It covers Bank, Boat A, Bar R, Gas R, and the first three stages of the Oil Rig campaign rescue. Source files are hash-pinned in [rescue_bmodel_targets.csv](../../rescue_bmodel_targets.csv); the CSV preserves owner BSPs, entity indices, and exact EVS source lines.

| Rescue map | Authored target | BSP entity | Child animation models | EVS action |
| --- | --- | ---: | --- | --- |
| Bank | `vault` | 441 | `vault` | VaultConsole unlocks then opens |
| Boat A | `explodeb` | 257 | `fencefall`, `debris` | one-shot touch opens, then quake, sound, and queued exploders |
| Bar R | `ClubRafterFall` | 291 | `ClubRafter`, `ClubRafterBounds` | `ClubActions` runs quake, open, and sound |
| Gas R | `PumpExplosion` | 267 | seven pump models | `explosion1` queue |
| Gas R | `PumpExplosion1` | 371 | six pump models | `explosion1` queue |
| Gas R | `PumpExplosion2` | 372 | seven pump models | `explosion1` queue |
| Gas R | `PumpExplosion3` | 373 | five pump models | `explosion1` queue |
| Gas R | `ExplodingCar1` | 266 | `ExplodingCar1` | `explosion1` queue |
| Gas R | `ExplodingCar2` | 274 | `ExplodingCar2` | `explosion1` queue |
| Gas R | `ExplodingCar3` | 352 | `ExplodingCar3` | `explosion2`, gated by `GasIsOn` |

The Oil Rig catalog entry starts stage A, then stage-local exit triggers request B, C, and D. The shared EVS file is byte-identical for A-D; these four BModel actions are bound to triggers and owners in the stage where each action occurs:

| EVS scenario | Target | Owner BSP entity | Child animation models | EVS action |
| --- | --- | ---: | --- | --- |
| Oil Rig B | `catwalk03` | oil_rig_B:82 | `catwalk03` | `WarehouseCatwalks`, first in the queued sequence |
| Oil Rig B | `catwalk01` | oil_rig_B:84 | `catwalk01` | `WarehouseCatwalks`, after 0.5 seconds |
| Oil Rig B | `catwalk02` | oil_rig_B:83 | `catwalk02` | `WarehouseCatwalks`, after another 0.5 seconds |
| Oil Rig C | `TowerStairs` | oil_rig_C:345 | six stair models and two clip models | `StairsDrop`, after 1.5 seconds |

Oil Rig A-D are sequential BSP stages: each `final_exit` sends `finalexitloadmap` for the next stage, while D's lifeboat exit sends `escape`. The catwalk and stair triggers and their BModel owners are in the same stage BSP. Static path validation, pending-level update, and World loader route are mapped; runtime stage completion, the pending block map-name field at EverGame+0x660 is mapped; equality with the checked command key and runtime timing remain open. See [Oil Rig stage and target evidence](oil_rig_a_rescue_script_targets.md) and [pending level update evidence](evergame_pending_level_update.md).

The Boat A and Gas R scripts use these BModel commands alongside fire-size changes, quake, sound, and spread events. Gas R spells the third car target `Explodingcar3` in EVS while its BSP identity is `ExplodingCar3`; the shared registry compares names case-insensitively.

## Shared code route

For every row, the static route is:

```text
map EVS tell <actor> open
  -> 00475560 named dispatch
  -> 00470C50 case-insensitive shared ScriptActor lookup
  -> BModel actor registered by InitializeBModelEntity (0043C670)
  -> ScriptActor dispatcher 00470150, class vtable slot +4
  -> DispatchBModelScriptCommand (0043E680)
  -> BModel vtable slot 0: UpdateBModelEntity (0043DC60)
```

The class command handler and per-frame animation/event update are documented in [BModel update evidence](bmodel_update.md). These static traces establish the intended actor/model relationships and command sequence. They do not establish runtime timing, successful authored event activation, or port parity.

This report covers named BModel `tell <name> open` targets. Subway A's `subdoor` uses self-commands, and Oil Rig C sends `turnon` to three `RCS_MiscEmitter` actors through the shared EVS; see [MiscEmitter command-handler evidence](004420f0_misc_emitter_dispatch.md). Training, MBase, and the other rescue-map systems still need their own interaction traces.

