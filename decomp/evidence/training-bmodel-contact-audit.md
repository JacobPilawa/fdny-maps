# Training BModel contact radius and motion gate

Read-only Ghidra 12.0.4 inspection of `FDNYFirefighter.exe`, the extracted Training BSP entities, and the Training model motion records. No live gameplay capture was used. The preserved Ghidra project stayed read-only; function-boundary recovery was done in a scratch copy under `/private/tmp`.

## Source path

- Callback `0043B620` reads `E_Reach` from BModel UserData `+0x30`. It calls `0043AFF0`; when that helper returns true and `E_Reach < 48.0f`, the callback replaces the reach with 48 (`0x42400000`) before calling inclusive bounds helper `004817A0`.
- `0043AFF0` first requires `F_HitPoints` at UserData `+0x34` to be nonzero and `G_Liftable` at `+0x38` to be zero. It then checks BModel `+0x1EC` against the 64-bit doubles at `004E0B60` and `004E0B68`.
- Ghidra raw bytes decode those constants as `-1.0e-28` (`19 60 50 BE F6 B0 1F BA`) and `+1.0e-28` (`19 60 50 BE F6 B0 1F 3A`). Assembly branches make the helper true at or beyond either endpoint and false only strictly inside the open interval. Unordered input also follows the true branch.
- The constructor `0043C670` initializes `+0x1E8` to `1000.0f` and `+0x1EC` to `0.0f`. For each configured `C_AnimModel` child it calls `geWorld_ModelGetMotion`; if a motion exists, it calls `geMotion_GetTimeExtents`, retains the smallest start at `+0x1E8`, and retains the greatest end at `+0x1EC`. Since that end field starts at zero and only increases, any positive child motion end proves the helper gate is true.

The embedded Training `RCS_BModel` typedef in `bsp-typedefs/training-bmodel.json` verifies the UserData offsets/names. `C_AnimModelN` names map to `%Model%` records and their BSP model indices; the Training motion tracks use those same indices (`map_explorer/source/feature_data.py`, `map_explorer/source/build_data.py`, `map_explorer/data/animations/training.json`). The native asset `assets/training/motions.json` omits key times for a few compressed child tracks, so the native gate only needs a verified positive endpoint; when all endpoints are unavailable and a candidate depends on them, it fails explicitly rather than guessing.

## Training result

| Entity | Name | Child models | `E_Reach` | Hit points | Liftable | Motion evidence | Effective reach |
|---|---|---:|---:|---:|---:|---|---:|
| 112 | `breakable_chair` | 9 | 50 | 2 | 0 | Child motion ends range from 0.2 through 0.85 | 50: strict `<48` radius substitution does not run |
| 113 | `breakable_chair1` | 9 | 0 | 2 | 0 | Child motion ends range from 0.2 through 0.85 | 48 |
| 212 | `breakable_box` | 10 | 20 | 2 | 0 | Several decoded child tracks end at 1.0; compressed siblings do not affect the positive-end proof | 48 |
| 213 | `breakable_box1` | 10 | 0 | 2 | 0 | Several decoded child tracks end at 1.0; compressed siblings do not affect the positive-end proof | 48 |

Entities 109 and 111 are liftable and have zero hit points, so they fail the helper's property gate. Training ladders 108 and 110 also have zero hit points. The native BModel contact path now derives the relevant child model IDs and motion endpoints from the extracted assets, applies effective reaches 50/48/48/48 to entities 112/113/212/213, then runs the source-mapped inclusive AABB comparison.

## Verification and limits

`--bmodel-contact-check` covers inclusive AABB boundaries, NaN rejection, strict reach threshold, hitpoint/liftable gates, motion-time tolerance, and 48-unit substitution. `--bmodel-contact-asset-check` derives the Training entity values from the extracted `entities.json` and `motions.json` and checks all four results above. These validate static source/data composition; the original contact candidate schedule, queue rotation, and frame cadence are still open in `training-ladder-audit.md`.

Ghidra queries: `InstructionContext.java 0043B620 004817A0 0043AFF0 0043C670`, `FieldStoreAudit.java 0x1ec`, `DataValue.java 004E0BBC`, and a temporary `DataDouble.java 004E0B60 004E0B68`. `0043C670` was defined/decompiled only in the scratch project because the preserved database had no function boundary at that address.
