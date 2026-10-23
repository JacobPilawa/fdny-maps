# `fdny-trajectory-v1` JSON Lines

UTF-8 text with one JSON object per line. Numbers use a decimal point. Consumers should ignore unknown fields/event types to allow additive extensions. Recorder 1.1 extends the existing v1 schema; it does not change the schema identifier.

Each file contains one run: a `start`, interleaved `sample` / `victim_sample` / `victim_end` events, and normally an `end`. Filename timestamps are UTC; event times are elapsed wall-clock milliseconds since this run started. Event timestamps can repeat, and victim events do not increment the Player sample counter.

## Start

| Field | Meaning |
| --- | --- |
| `type` | `start` |
| `schema` | `fdny-trajectory-v1` |
| `recorder_version` | Writer version, currently `1.1` |
| `stage` | Normalized map basename, e.g. `training` or `mbase` |
| `started_utc` | ISO timestamp ending in `Z` |
| `run` | Counter local to this game process; filenames also contain PID |
| `victim_tracking` | Whether victim hooks were enabled (1.1); does not promise that victims exist in this stage |
| `coordinate_system` | Descriptive convention string |
| `sampling`, `time_basis`, `input_basis` | Descriptions of sampling/timing/input provenance |

## Player sample

| Field | Meaning |
| --- | --- |
| `type` | `sample` |
| `n` | Contiguous Player sample counter, starting at 0 |
| `t_ms` | Elapsed wall-clock milliseconds |
| `game_dt_s` | Observed game frame-delta field, in seconds; not a promise of render frequency |
| `position` | `[x,y,z]` body position, original Genesis3D units, **Y up** |
| `velocity` | Raw `[x,y,z]` velocity field |
| `body_forward` | Body forward basis vector, separate from pitch |
| `pitch_deg` | Raw player view-pitch field, degrees |
| `player_state_raw` | Native state; observed standing 3, crouched 4, climbing 5; preserve other values |
| `ground_contact_raw` | Native contact byte, retained without coercion |
| `tool_slot_raw` | Native inventory slot, not a portable tool-name enum |
| `keys_mask` | Sampled virtual-key bitmask (below) |
| `bounds_min`, `bounds_max` | Optional in v1; measured local AABB vectors in 1.1 when valid |

The world-space hitbox is `position + bounds_min` through `position + bounds_max`. It stays axis-aligned; do not rotate it with the facing vector. A simple body proxy may rotate independently. Legacy files without measured bounds can use min `[-30,-30,-30]`, max `[30,25,30]` for crouched state 4 and `[30,85,30]` otherwise, explicitly treated as an inference. The footprint stays 60×60 units while height changes from 115 to 55.

`keys_mask` uses bit indices 0–16 in this order:

| Bits | Keys |
| --- | --- |
| 0, 1, 2, 3 | W, A, S, D |
| 4, 5, 6, 7 | Up, Left, Down, Right |
| 8, 9, 10, 11 | C, Ctrl, Space, Shift |
| 12, 13 | Left mouse, Right mouse |
| 14, 15, 16 | 1, 2, 3 |

The mask is zero when the foreground window is not in the game's process. Zero therefore cannot distinguish inactivity from loss of focus. This is not an exact input-event stream or a log of resolved in-game commands.

## Victim snapshots and lifetimes (1.1)

`victim_sample` fields are `id` (run-local string such as `v001`), `name` (entity name or fallback), `t_ms`, `position`, `body_forward`, `state_raw`, `bounds_min`, and `bounds_max`. IDs, rather than names, identify lifetimes. A new lifetime may reuse the same game memory; do not use addresses as IDs. Native states include downed 4, carried 5, claimed/released 6 and roaming 7; retain raw values rather than forcing a complete semantic model.

`victim_end` contains `id` and `t_ms`; hide that marker at and after the destruction observation. A victim may stop producing snapshots before file closure; its final position is a **last observed** value, not simulated future movement. Old files without victim events cannot recover those trajectories from map spawn candidates.

## Run closure

`end` contains `reason`, `samples` (Player sample count only) and `elapsed_ms`. Reasons include `next_player_initialization` and `process_exit`; the synthetic test uses `selftest_complete`. Menu time can extend the closing elapsed time beyond the last gameplay sample. Use the last Player sample timestamp for the visible trajectory duration.

Treat a file without `end` as partial/live. Keep complete JSON lines and ignore an unfinished final line, while reporting malformed complete lines. Buffering can lose the final unsaved fraction of a run after a crash/force-quit.

## Playback conventions

- Locate samples by timestamp with binary search; avoid assuming 60 Hz or counting samples as elapsed time.
- Interpolate nearby positions and facing, holding discrete states and bounds until their recorded transition.
- Do not interpolate across long pauses or teleport discontinuities. The included helper breaks joins above 500 ms or 2,000 units.
- Map X/Z to a top-down plane in 2D; retain X/Y/Z in 3D. Both player and map use original engine units.
- Drive marker, hitbox and victim tracks from a shared playback time. Rendering geometry and gameplay-state reconstruction are separate concerns.
