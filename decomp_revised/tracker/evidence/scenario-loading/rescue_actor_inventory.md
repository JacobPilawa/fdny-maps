# Rescue actor inventory

This is a hash-pinned, entity-level index of player, fire, victim,
victim alternate spot, and PathPoint records across every local BSP.
It preserves raw ordered epairs; it is authored data, not runtime evidence.

- Maps: 17
- Indexed actors: 564
Target classes: `RCS_FireRescue`=430, `RCS_PathPoint`=5, `RCS_PlayerRescue`=13, `RCS_VictimAltSpot`=45, `RCS_VictimRescue`=71

## Per-map class counts

| Map | Player | Fire | Victims | Alt spots | PathPoints |
|---|---:|---:|---:|---:|---:|
| `bank` | 1 | 72 | 6 | 4 | 0 |
| `bar_r` | 1 | 65 | 6 | 4 | 0 |
| `boat_a` | 1 | 47 | 9 | 7 | 0 |
| `boat_b` | 1 | 42 | 5 | 5 | 0 |
| `city_h` | 0 | 0 | 0 | 0 | 0 |
| `const_h` | 0 | 0 | 0 | 0 | 0 |
| `dock_h` | 0 | 0 | 0 | 0 | 0 |
| `gas_r` | 1 | 39 | 7 | 0 | 4 |
| `mbase` | 1 | 11 | 5 | 0 | 1 |
| `oil_rig_a` | 1 | 15 | 3 | 4 | 0 |
| `oil_rig_b` | 1 | 14 | 3 | 1 | 0 |
| `oil_rig_c` | 1 | 24 | 6 | 6 | 0 |
| `oil_rig_d` | 1 | 16 | 3 | 4 | 0 |
| `oil_rig_h` | 0 | 0 | 0 | 0 | 0 |
| `subway_a` | 1 | 55 | 6 | 2 | 0 |
| `subway_b` | 1 | 23 | 10 | 4 | 0 |
| `training` | 1 | 7 | 2 | 4 | 0 |

## Victim and fire types

Victim `D_Type`: `boy`=4, `boy1`=1, `dog`=2, `fatman`=13, `girl`=7, `man`=27, `woman`=17

Fire `F_FireType`: `0`=430

## Authored alternate victim-spot links

These source-data references are checked against `RCS_VictimRescue` `%name%` values on the same BSP. The executable's alternate-spot handler resolves the same name field before registering position and heading; this join does not prove runtime factory admission.

| Map | Alt spots | Local victim name matches | Unresolved |
|---|---:|---:|---:|
| `bank` | 4 | 4 | 0 |
| `bar_r` | 4 | 4 | 0 |
| `boat_a` | 7 | 7 | 0 |
| `boat_b` | 5 | 5 | 0 |
| `city_h` | 0 | 0 | 0 |
| `const_h` | 0 | 0 | 0 |
| `dock_h` | 0 | 0 | 0 |
| `gas_r` | 0 | 0 | 0 |
| `mbase` | 0 | 0 | 0 |
| `oil_rig_a` | 4 | 4 | 0 |
| `oil_rig_b` | 1 | 1 | 0 |
| `oil_rig_c` | 6 | 6 | 0 |
| `oil_rig_d` | 4 | 4 | 0 |
| `oil_rig_h` | 0 | 0 | 0 |
| `subway_a` | 2 | 2 | 0 |
| `subway_b` | 4 | 4 | 0 |
| `training` | 4 | 4 | 0 |

## Authored VictimRescue script values

Exact non-empty `S_Script*` epair values on victim records; these are literals, not interpreted command names.

| Map | Script values |
|---|---|
| `bank` | `roam 1000` × 1; `roam 500` × 1 |
| `bar_r` | `playanim 1` × 3; `roam 300` × 1; `roam 500` × 5; `setskin 2` × 3 |
| `boat_a` | `roam 200` × 1; `roam 700` × 1 |
| `boat_b` | `500` × 1; `roam 1000` × 1; `roam 1500` × 1 |
| `city_h` | — |
| `const_h` | — |
| `dock_h` | — |
| `gas_r` | `100` × 1; `roam 250` × 1; `roam 350` × 3; `roam 400` × 1; `roam 80` × 1 |
| `mbase` | `roam 1000` × 1; `roam 500` × 1 |
| `oil_rig_a` | — |
| `oil_rig_b` | `roam 2400` × 1 |
| `oil_rig_c` | `roam 2400` × 5; `roam 700` × 1 |
| `oil_rig_d` | `roam 5000` × 3 |
| `oil_rig_h` | — |
| `subway_a` | `roam 1000` × 1; `roam 1200` × 1; `roam 200` × 1; `roam 800` × 1 |
| `subway_b` | `roam 1000` × 5 |
| `training` | `pickup: if gotvictim 1 (*Cmd3b)` × 1; `pickup: ifnot gotvictim 1 (*Cmd3c)` × 2; `pickup: set gotvictim 1` × 2 |

The static route for `roam <number>` is documented in [victim_roam_behavior.md](victim_roam_behavior.md). Numeric-only values (for example Boat B `500` and Gas R `100`) remain unresolved, and Training's `pickup:` literals are retained as data without being treated as roam commands.

The CSV contains one row per entity, explicit locations/resource types,
keyframe size/time pairs, script fields, and the full ordered epair list.
Compare its `map_sha256` values with `tracker/level_map_inventory.csv`
before using rows to seed a port build.
