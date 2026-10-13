# BModel reach expansion across required rescue maps

The executable's conditional BModel interaction reach has now been resolved
against the motions actually serialized in the required rescue BSPs. This
includes Training as a regression scenario and all seven campaign rescues.

## Result

Across the eight maps there are 216 `RCS_BModel` entities. Seventy-six pass the
authored gates in `CheckBModelReachExpansion`: `F_HitPoints > 0`,
`G_Liftable == 0`, and authored `E_Reach < 48`. Seventy-one of those evaluate
true and use an effective 48-unit reach in the BModel candidate bounds test.
Five retain their authored reach because no linked child motion contributes a
nonzero end time.

| Map | BModels | Authored-gate candidates | Effective reach 48 | Authored reach retained |
| --- | ---: | ---: | ---: | ---: |
| Training | 28 | 3 | 3 | 0 |
| MBase | 13 | 0 | 0 | 0 |
| Bar R | 24 | 6 | 5 | 1 |
| Gas R | 23 | 11 | 10 | 1 |
| Bank | 47 | 30 | 30 | 0 |
| Boat A | 26 | 9 | 9 | 0 |
| Subway A | 47 | 17 | 14 | 3 |
| Oil Rig A | 8 | 0 | 0 | 0 |
| **Total** | **216** | **76** | **71** | **5** |

The five authored reaches retained by the helper are:

- Bar R entity 169 (`Ladder01`): 5 units.
- Gas R entity 309 (`button1`): 20 units; child model 91 has no motion record.
- Subway A entity 288 (`Ladder`): 24 units.
- Subway A entity 323 (`Ladder5`): 5 units.
- Subway A entity 401 (`Ladder13`): 24 units.

The three Subway ladders and Bar R ladder have no child-motion records. Gas R
button1 is likewise motionless. Their authored reaches therefore remain the
candidate-test input. All other eligible entities have a linked motion with a
positive end time, including the 30 eligible Bank BModels.

## Evidence and method

The retail `Genesis.dll` copy is SHA-256
`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
Fresh headless Ghidra analysis of that exact DLL mapped `geMotion_CreateFromFile`
(`1000D570`), `gePath_CreateFromFile` (`1000F580`),
`geMotion_GetTimeExtents` (`1000BE60`), and `gePath_GetTimeExtents`
(`1000E490`). The saved pseudocode and assembly are in
[`genesis_motion_functions_fresh.txt`](../genesis-boundary/genesis_motion_functions_fresh.txt).
The retail parser reads MOTN `0.F0` records and PATH `0.F2` rotation and
translation key arrays. The asset inventory decodes each channel's last
keyframe time as a 32-bit `geFloat`, then uses the same maximum end-time
aggregation as the retail extent routines.

Fresh executable pseudocode at `0043C670` initializes the BModel's maximum
child-motion end at zero and keeps the greatest child end. The helper at
`0043AFF0` rejects BModels with no hit points or with `G_Liftable != 0`; it
returns true when the stored end time is at or beyond either `-1e-28` or
`+1e-28`. Caller `0043B620` raises authored `E_Reach` to 48 only when the helper
returns true and the authored reach is smaller than 48. Gas R's exact button1
join is retained in [the focused interaction report](gas_r_button1_use_path.md).

This establishes the static reach passed into the bounds overlap calculation.
It does not prove live contact, candidate identity/order, callback cadence,
input selection, or a visible gameplay effect. Those runtime questions remain
open. The hash-pinned per-entity and per-model results are in
[`bmodel_reach_asset_inventory.json`](bmodel_reach_asset_inventory.json),
reproducible with [`audit_bmodel_reach_assets.py`](../../../tools/audit_bmodel_reach_assets.py).
