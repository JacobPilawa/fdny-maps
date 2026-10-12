# Training FireRescue Think behavior audit

Read-only Ghidra 12.0.4 review of the preserved `FDNYFirefighter.exe` project on 2026-10-11. `00449FE0` and its route helper `00449340` were inspected with `DecompileFunctions.java`; registration/callback boundaries are cross-referenced against the saved scheduler audit. No live gameplay capture was used.

## Callback roles and Think path

- `00449FE0` is secondary-vtable slot 0 for the FireRescue visual subobject (`004E1774`, object `+0x24`). The primary FireRescue callback `004483A0` is separate (`004E1780`, primary slot `+0x48`); it handles proximity influence and can enqueue player exposure message `0x7531`. The primary callback is not the Think scheduler.
- Think returns without doing work when visual state `+0x4C` is `5`. Otherwise it updates color using strength `+0x104`. When the sprayed flag `+0xE0` is clear and state is not `4`, it decrements divider `+0xF8`; a nonzero value calls `0044B060` and returns. A full update resets the divider to `3`.
- On a full update, route timing/visibility checks run, and state `4` calls `00449340` for route advancement and possible child-fire creation. Think then calls `0044B060`, updates orientation, advances strength with frame delta `[0x00502B04]` (using a state-dependent scale outside state `4`), caps strength at `1.0`, updates alpha, and clears sprayed `+0xE0` and queued-exposure count `+0x94`.
- The orientation helper `004B9400` in Think converts a float to an integer; it is not a shared RNG draw. The Think path contains no direct call to the shared random helper. `00449340` has no direct shared-RNG call in its preserved code, but its factory virtual call can construct a child FireRescue whose initializer may consume RNG.

## RNG and Training boundary

- The embedded Training BSP `RCS_FireRescue` typedef and Ghidra's matching `00448D60` field reads establish `UserData+0x04 == A_Name`. All seven Training fires have nonempty `A_Name`, so each takes the conditional FireBurn sound branch and consumes one shared RNG draw after effect initialization. Effect initialization `0044B270` has shared RNG callsites at `0044B311`, `0044B3BB`, `0044B3DC`, `0044B48C`, and `0044B4AD`; the latter three are in emitter setup paths. The controlled `fire-entity-oracle.json` fixture records 9 draws for its null-name case and 10 for its nonnull-name case, but stubs the floor callback and is not a complete original Training startup capture. See `training-userdata-layout-audit.md` and `bsp-typedefs/training.json`.
- The extracted Training map has seven FireRescue entities, all with `P_NumKeys=0` and zero authored size keys; its preserved records contain no authored route/path properties. Keep route state-4 spread documented for other levels and dynamic cases, but do not assume that this branch is reached in Training from authored data alone.

## What remains unresolved

The recurring caller of `00449FE0` has not been identified. Constructor, entity registration, the primary category callback, and the staged-entity initialization pass do not establish its cadence or its order relative to player movement, tool updates, queued messages, and script actors. The native build renders Training fire visuals and connects selected initialization/effect components; it does not yet schedule this source Think path. Do not claim fire lifecycle or global RNG parity until that schedule and the connected behavior are verified.

Related evidence: [`training-think-registration-audit.md`](training-think-registration-audit.md), [`think-dispatcher-candidate-followup.md`](think-dispatcher-candidate-followup.md), [`scheduler-registration-audit.md`](subobject-init-followup/scheduler-registration-audit.md), [`training-route-attachment/audit.md`](training-route-attachment/audit.md), [`fire-entity-oracle.json`](../fire-entity-oracle.json), and [`rng-audit.json`](../rng-audit.json). Preserved Ghidra decompilations are `00449fe0_FUN_00449fe0.c` and `subobject-init-followup/00449340_FUN_00449340.c`.
