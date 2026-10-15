# Delayed World EVS pass in the shared viewer

## Plain-language explanation

Some shared rescue equipment commands run too early during map startup: the Player script actor has not registered yet, so the messages are dropped. Retail runs the full map EVS pass again after four World updates. The shared viewer now reproduces that second pass, so the shared goggles and axe commands can reach the Player. Controlled checks confirm the starting equipment inventory across all 13 rescue and linked-stage packages. They do not establish exact retail frame timing or the final selected tool.

## Source basis

- `entity_factory_queue_update_order.md` traces mode-1 EVS before World creation, BSP requests appended after the queue drain, and PlayerRescue registration on a later pump invocation.
- The same trace maps the full mode-0 EVS pass after four WorldEntity `+0x14` callbacks. The exact relation of those callbacks to rendered frames and the later factory-pump invocation remains unknown.
- `player_loadout_command_path.md` maps shared `rescue.evs` to `equip goggles` (except Training) and `select axe`; it also maps each PlayerRescue's authored equipment fields, including Gas R and the Oil Rig stages.

## Host change

`port/spikes/training-native-prototype/src/world_startup.h` supplies the four-update one-shot gate. `scene_viewer.cpp` already ran the selected map EVS in mode 1 during startup; it now runs that map's full mode-0 pass once when the gate completes. Interactive play advances the gate once per host World update. The headless interaction and rescue diagnostics advance the same gate before exercising their routes.

The `--rescue-check` reports resulting inventory and asserts axe, extinguisher, and goggles presence against the 13-map PlayerRescue/shared-script inventory. It reports selection but does not assert it because source-mapped switch gates and unverified callback ordering do not establish the final retail selection.

## Controlled package checks

The packaged rescue-all executable was built on 2026-10-15. Its SHA-256 is `0db1f4450015663d2917511f81ec4c9a9ba952dafa9963cb30264a572a82fc01`. `--rescue-check` passed on all 13 packages. Bank and Subway A `--interaction-check` also passed, and the bundled catalog check loaded all eight retail rescue entries:

| Package | Rescued | Axe | Extinguisher | Goggles | Host-selected |
|---|---:|:---:|:---:|:---:|---|
| training | 2/2 | yes | no | no | axe |
| mbase | 5/5 | yes | no | yes | axe |
| bar_r | 6/6 | yes | no | yes | axe |
| gas_r | 7/7 | yes | yes | no | extinguisher |
| bank | 6/6 | yes | no | yes | axe |
| boat_a | 9/9 | yes | no | yes | axe |
| subway_a | 6/6 | yes | no | yes | axe |
| oil_rig_a | 3/3 | yes | no | yes | axe |
| boat_b | 5/5 | yes | no | yes | axe |
| subway_b | 10/10 | yes | no | yes | axe |
| oil_rig_b | 3/3 | yes | yes | yes | axe |
| oil_rig_c | 6/6 | yes | yes | yes | axe |
| oil_rig_d | 3/3 | yes | yes | yes | axe |

These remain synthetic transaction checks: they do not demonstrate natural player travel/contact, live rescue scheduling, linked-stage transition behavior, or complete mission objectives.

## Limits

The four-update interval is grounded in the retail WorldEntity callback count. The viewer currently advances it on its host frame loop; the retail callback-to-frame relation is not recovered. Selection output is a host observation only. Rendered item transforms, gameplay timing, and full mission completion remain separate parity gaps.
