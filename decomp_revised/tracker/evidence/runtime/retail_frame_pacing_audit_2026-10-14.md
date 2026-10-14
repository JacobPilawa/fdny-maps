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

## Open timing source

The producer/writer of `DAT_00502B04` is unresolved. Reviewed Ghidra data
references show reads; a raw scan for common absolute-store encodings in the
pinned executable found no direct write. This does not rule out an indirect
write or an engine callback. Timing API imports and the D3D `EndScene`
presentation path do not establish a frame limiter or VSync behavior.

The port uses SDL elapsed milliseconds, a provisional 100 ms maximum step, and
swap interval 1. Time-scaled physics is supported by the retail consumers, but
the exact retail delta source, long-stall cap, update cadence, and presentation
limit remain open. Do not claim timing parity from elapsed-time scaling alone.

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
