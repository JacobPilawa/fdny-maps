# Retail frame pacing audit — 2026-10-14

## Pinned evidence

- Retail executable SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- `0049DBC0` reads global `DAT_00502B04` for velocity integration and a
  `dt * dt` acceleration term. Its fresh, hash-pinned decompilation is
  `0049dbc0_player_physics_integrator_fresh.c`.
- Other reviewed consumers include victim movement, BModel animation, fire
  suppression/exposure, and `00482AA0`, `00483580`, and `0049E0A0`. The
  existing Ghidra xref output is `.local/ghidra-frame-clock-refs.log`.
- `00499410` dispatches update/render callbacks while the Windows message queue
  is empty; that normal idle path contains no explicit sleep. `0042AEA0` reaches
  game update and then `00494670`. The latter dispatches frame callbacks and
  brackets rendering with `geEngine_BeginFrame`/`geEngine_EndFrame`. Evidence:
  `tracker/evidence/runtime-loop/00499410_app_idle_loop.c`,
  `tracker/evidence/runtime-loop/0042aea0_game_update.c`, and
  `tracker/evidence/genesis-boundary/00494670_frame_dispatcher.c`.

## Frame-delta source trace (2026-10-20)

The earlier direct-reference scan did not see the writer because the timer
manager stores through an object alias. In the normal update route,
`0042aea0 ProcessEverGameUpdate` calls `0042c080(&DAT_00502ab8)`; that manager
selects a clock record, calls `0042bf70`, and copies its `+0x10` float to
manager `+0x4c`, which is `DAT_00502B04`. The exact timer pseudocode and
constants are in
[`0042bf70_frame_clock_delta_fresh.c`](0042bf70_frame_clock_delta_fresh.c)
and [`player_hose_frame_delta_refs_2026-10-20.md`](../scenario-loading/player_hose_frame_delta_refs_2026-10-20.md).

`0042bf70` reads `timeGetTime()` millisecond differences, handles DWORD wrap,
multiplies by binary64 `0.001`, and stores a binary32 delta. It calls
`Sleep(1)` and resamples while the stored delta is at or below binary64
`1/60`; deltas above binary64 `0.1` are capped to binary32 `0.1`. On loss of
focus `0042bf20` records the pause start, and `0042bf40` offsets the clock base
when focus returns. This establishes the retail shared-delta unit, lower
pacing threshold, upper cap, and focus-pause clock treatment.

The shared SDL host now applies the same millisecond conversion, pacing
threshold, and cap. SDL's tick resolution and focus-pause effects on other
absolute-time consumers still need a controlled check. A same-machine retail
capture remains necessary before claiming measured presentation or timing
parity; this static trace does not establish VSync behavior.

## Port profiling aid and Bank status

The interactive viewer now accepts `--performance-audit`. It prints one-second
summaries for FPS, average/p95/maximum frame time, update time, world draw time,
overlay time, swap time, and the number of world groups/triangles drawn or
culled. These are host CPU/wall timings; OpenGL 2.1 does not expose GPU timing
here, and swap time can include presentation wait.

The static BSP collision models are indexed by their authored minimum X bound
for player sweeps. Moving PATH models are kept in a separate always-checked
list, and candidate models are restored to source order before the existing
trace algorithm runs. On the Bank export, the stationary authored spawn query
intersects two static model bounds from the 334-model set, before moving-model
checks. The broadphase leaves the full BSP leaf and convex-plane tests intact.

The user reported on 2026-10-14 that Bank framerate now seems sorted. The
recent packaged app was rebuilt as
`.local/FDNY Scene Preview.app`; its executable SHA-256 is
`44488b009bcb83bc4fe8d4725edbee1dcffcfcc9b2c2bc9c2ac29e90c98213c1`. The user
report closes the immediate “unplayably slow Bank” blocker for now. A same
machine retail/port FPS recording is still required before claiming measured
performance or frame-pacing parity.

## Verification

- The shared scene viewer builds and the packaged rescue-all app loads all 8
  catalog missions plus their linked-stage bundles.
- `--inspect` spawn/floor/idle checks pass for all 13 packaged rescue/stage
  scenes. Each starts clear, hits the expected floor on model 0, and remains
  grounded through the five-second idle simulation.
- Packaged Bank `--interaction-check` opens/touches the authored door and vault
  sequence and crosses after the authored opening/alignment route. Packaged
  Bank `--rescue-check` releases and counts one survivor.
- Offscreen render and interaction timings include scene/package setup; they
  are not used as live FPS estimates.
