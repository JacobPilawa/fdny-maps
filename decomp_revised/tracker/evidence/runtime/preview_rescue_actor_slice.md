# Map-neutral rescue actor slice

Updated: 2026-10-13

## Purpose

The native scene preview now consumes the existing typed BSP exports to place
and draw rescue victims. This is a playable-host step toward the rescue MVP;
it is not a claim that the full rescue scenario or its runtime scheduler has
been recovered.

## Reused evidence and code

- `tools/export_victim_actors.py` decodes the local ACT resources and writes
  model JSON plus BMP bitmaps that SDL can load without an additional image
  library. The decoded copyrighted assets remain under `.local/` and are not
  committed.
- `startup.h` preserves `RCS_VictimRescue` and linked
  `RCS_VictimAltSpot` candidates. The preview uses the existing shared
  pseudo-random generator, source-derived floor placement helper, actor pose
  evaluator, animation timer, and root transform.
- `victim_resource_crosswalk.csv` and the fresh victim factory analysis drive
  the actor selection: `boy1` falls back to Boy; `fatman` selects runtime ACT
  model 11, 12, or 13; other in-scope types map to their shipped model.
  Material slot 0 advances by runtime model ID, matching the mapped factory
  behavior.
- Contact pickup reuses the source-backed player-box/victim-radius overlap
  and message `0x7532` transition. The preview hides the victim after pickup
  and reports the carried name in the window title.

## Roam and spawn implementation

Victim actor creation and spawn selection are represented as separate passes.
The first pass resolves actor type/model and initial motion while preserving
the authored origin as the roam center. The second chooses among the authored
origin and linked alternate spots, floor places the actor, assigns its
per-model material, and dispatches only `S_Script01..05`. Fatman model choice,
spawn-spot selection, and roam animation selection consume the shared preview
RNG in their corresponding victim phases. The viewer advances FireRescue
smoke/sprite setup and named-loop draws before the victim first pass, then
consumes each victim's initial sound-controller deadline before model/motion
selection. Other classes' random draws are not yet integrated, so this does not
claim retail seed-for-seed parity.

`roam <number>` now uses the native candidate rejection sequence: radius from
the original authored origin, actor-box world sweep, nearby fire-obstacle
spheres, and two rotated floor-height probes. Type 6 in the obstacle builder
is confirmed as `RCS_FireRescue`; its virtual radius path reads root `+0x88`.
Static SceneData `H_Radius` seeds the preview radius while the retail UserData
binding remains unproven. Floor-me fire centers use the source-mapped probe.
The target updater now ports the random initial direction, player distance,
angle and line-of-sight gates, candidate retries, and random-direction
fallback. Six-motion ACTs select walk motions 3-5 and their mapped movement
scalar.

The native updater callback period remains unknown, and the preview uses its
own frame tick. The engine's final turn interpolator and world movement helper
are not exact ports yet. Dynamic fire motion/size changes and full cross-class
RNG ordering are also open.

## Current boundary

The scene viewer does not yet dispatch EVS events or run the shared actor
contact scheduler. It auto-applies the mapped pickup transition on overlap,
but has no carried-body presentation, drop/release action, rescue-exit event,
rescue statistic update, fire update loop, or mission-completion gate. Door
opening is part of the same missing EVS/contact/BModel runtime; it is not a
missing door mesh or animation asset. See
[`bmodel_update.md`](../scenario-loading/bmodel_update.md) and
[`victim_pickup_message_path.md`](../scenario-loading/victim_pickup_message_path.md).

## Build and verification state

The C++ viewer target compiles with the new actor path. The Bank app bundle is
rebuilt with the selected scene and local ACT assets. The interactive window
has not been opened from this task, and no scene-wide actor/pickup runtime test
was run, so all-map visual placement and in-window behavior remain to be
confirmed by use. The HTML dashboard was not edited; milestone CSV metadata
records this partial host integration separately from EXE function recovery.
