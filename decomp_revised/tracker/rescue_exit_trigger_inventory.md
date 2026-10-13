# Authored rescue and stage-exit trigger inventory

Generated from all 17 copied retail GBSP v15 BSPs. Input hashes are checked against `scene_load_coverage.csv`.
This is authored-data evidence: it maps BoundsTrigger scripts and model names, but does not prove runtime trigger admission, overlap cadence, command completion, or mission parity.

- BSP files: 17
- BoundsTriggers with escape/final-exit script bindings: 40
- Rescue-exit script bindings: 33
- Final-exit check bindings: 2
- Stage-transition bindings: 6
- Triggers joined to named `%Model%` entities: 40
- Trigger `C_Model` references without a named model join: 0
- BSPs with no matching exit script: city_h, const_h, dock_h, oil_rig_h
- `RCS_PlayerRescue` entities: 13
- `RCS_PlayerHose` entities: 4
- Exact `Player` epair values: 0

The CSV flags all 8 required catalog rescue scenarios from `rescue_mission_coverage.csv`. The asset-level inventory also includes Boat B, Subway B, and Oil Rig B/C/D transition stages.

| BSP | Rescue scenario | PlayerRescue | PlayerHose | exact Player epairs | Triggers | `escape` | `finalexit` | map transition | model joins | destinations |
|---|---:|---:|---:|---:|---:|---:|---:|---:|---:|---|
| `bank` | yes | 1 | 0 | 0 | 2 | 2 | 1 | 0 | 2/2 | — |
| `bar_r` | yes | 1 | 0 | 0 | 3 | 3 | 0 | 0 | 3/3 | — |
| `boat_a` | yes | 1 | 0 | 0 | 4 | 3 | 0 | 1 | 4/4 | boat_b |
| `boat_b` | no | 1 | 0 | 0 | 2 | 1 | 1 | 0 | 2/2 | — |
| `city_h` | no | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0/0 | — |
| `const_h` | no | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0/0 | — |
| `dock_h` | no | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0/0 | — |
| `gas_r` | yes | 1 | 0 | 0 | 2 | 2 | 0 | 0 | 2/2 | — |
| `mbase` | yes | 1 | 0 | 0 | 3 | 3 | 0 | 0 | 3/3 | — |
| `oil_rig_a` | yes | 1 | 0 | 0 | 2 | 1 | 0 | 1 | 2/2 | oil_rig_b |
| `oil_rig_b` | no | 1 | 0 | 0 | 2 | 1 | 0 | 1 | 2/2 | oil_rig_c |
| `oil_rig_c` | no | 1 | 0 | 0 | 2 | 1 | 0 | 1 | 2/2 | oil_rig_d |
| `oil_rig_d` | no | 1 | 0 | 0 | 1 | 1 | 0 | 0 | 1/1 | — |
| `oil_rig_h` | no | 0 | 1 | 0 | 0 | 0 | 0 | 0 | 0/0 | — |
| `subway_a` | yes | 1 | 0 | 0 | 7 | 5 | 0 | 2 | 7/7 | subway_b |
| `subway_b` | no | 1 | 0 | 0 | 5 | 5 | 0 | 0 | 5/5 | — |
| `training` | yes | 1 | 0 | 0 | 5 | 5 | 0 | 0 | 5/5 | — |

The per-entity CSV keeps the original ordered epairs and exact script slots. Each `C_Model` is joined by exact same-BSP name to `%Model%` records, with model index/origin retained; `ZZ_location` and `E_Reach` are preserved separately. This asset join does not prove runtime factory admission or contact timing.

The executable route is documented in [rescue_completion_and_exit_path.md](evidence/scenario-loading/rescue_completion_and_exit_path.md), with the preserved release/exit component in [the archived decomp](../../decomp/evidence/victim-ai-audit/rescue-deadline-exit-component.md).
