# MBase FireRescue `moveto` path

Checked 2026-10-13 against the pinned EXE and the local hash-matched MBase
scene export. This maps the authored command into the beginning of the native
FireRescue path-motion setup. It maps the path updater and the two trailing values into generated timed-key timestamps.
The callback schedule and exact author-facing timing terms remain open.

## Current shared-host port (2026-10-15)

The shared EVS host now resolves the named `RCS_PathPoint` through its
case-insensitive `A_Name` registry and advances the original FireRescue along
the mapped direction at the rate computed from the first operand. On the
authored MBase and Gas R routes, the source updates that fire's visible position
only after progress advances by more than half its `H_Radius`; the host now
preserves this threshold and still moves to the endpoint on completion. These
routes have no `C_Model`, so the initializer's threshold field is directly
mapped to `H_Radius`. See
[`00448D60_h_radius_threshold.asm`](00448D60_h_radius_threshold.asm). The two
trailing operands are retained as inputs but are not yet used to build retail's
timed visual keys.

The pinned updater also builds FireRescue-shaped configs and submits deferred
map-entity spawn requests as the source actor advances. The later generic
spawn-queue drain (`0049C670`) performs object creation and manager
registration. A follow-up maps the FireRescue factory, handle assignment, and
the limits of its pool/update lifecycle in
[`deferred_child_lifecycle_2026-10-15.md`](deferred_child_lifecycle_2026-10-15.md).
The queued actor's recurring update, per-entity deactivation, reuse, and
emission behavior are not fully mapped, so the host does not spawn trail
actors. Exact transform/floor behavior for the moved source fire and the
external callback cadence also remain open. The packaged MBase and gas_r
`--rescue-check` commands complete
without an `Unimplemented EVS` report, but those controlled rescue transactions
do not exercise or establish natural route timing or retail parity.

## Authored input

`inputs/original/Data/Maps/mbase.evs` contains:

```text
exec rescue.evs
tell player (setashdensity 0)
tell mortarfire1 (moveto mortar1 1 10 120)
```

In `mbase.bsp`, entity 149 is `RCS_FireRescue` with `A_Name=mortarfire1` and
`ZZ_location=588 -144 -2900`. Entity 150 is `RCS_PathPoint` with
`A_Name=mortar1` and `ZZ_location=1904 -232 -3712`. The target and source actor
are therefore present in the same map package. Their authored names do not by
themselves prove every construction field offset; the runtime lookup is mapped
separately below.

## Authored route and size-command coverage

Across the 21 extracted `.evs` scripts, the authored `moveto` uses are limited
to two maps in the current source inventory: MBase has one route to `mortar1`
with trailing values `10, 120`; gas_r's `firespread` queue has four routes to
`PathPoint1` through `PathPoint4`, each with trailing values `99999, 99999`.
This inventory is useful for comparing authored timing patterns; it does not
prove how long a runtime path remains active or what the script author meant by
each route value. The same scan finds twelve FireRescue-targeted `setsize`
commands in `gas_r.evs`. They change FireRescue dimensions with authored values
such as `300 375 300 .6` and `10 10 10 2`. `boat_a.evs` also has `setsize 3`,
but it is inside the `QueExploders` sequence and targets exploder actors, so it
must not be treated as the same FireRescue command contract. The mission-level
scan and implementation states are recorded in
[`rescue_mission_coverage.csv`](../../rescue_mission_coverage.csv).

## Pinned code trace

Binary: `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

- `FUN_00449A80` compares the command token case-insensitively. The `moveto`
  path reads the target name and three numeric operands, then calls
  `FUN_00448A90`. The neighboring `stopmove` and `setsize` branches are present
  in the same function. The `setsize` parser reads three integer components
  plus a fourth float duration and calls `FUN_00447920`; Ghidra's first
  C-like output hid the duration's x87 use. Ghidra now labels this function
  `HandleFireRescueMotionCommand`.
- `FUN_00448A90` uses `FUN_00470C50` to resolve the requested target. It checks
  against the already-active path point and emits
  `ERROR! Recursive PathPoint.` from `rescue\FireRescueEnt.cpp:699` if the
  same target is requested again. For a new target it reads the target vector,
  computes displacement from the current FireRescue vector, stores the active
  target and normalized displacement, calculates distance divided by the first numeric value when it is positive, copies
  the two trailing numeric values to object offsets `+0xB0/+0xB4`, clears two
  counters, and enters state 4. Ghidra now labels it `SetFireRescuePathPointMove`.
- `FUN_00470C50` iterates the shared ScriptActor registry at `this+0x7C` with count at `this+0x80`
  and compares the requested name case-insensitively with each object's name
  string reached through `+0x24`. It returns the matching registry entry or
  null. Ghidra now labels it `FindScriptActorByName`; PathPoint movement is one
  consumer of this name registry, alongside named `tell` dispatch and script
  binding.
- MBase supplies operands `1`, `10`, and `120`. Operand 1 is used as the
  positive divisor in the distance calculation. The trailing values 10 and 120
  are consumed in `AdvanceFireRescuePathMotion`: each float value is multiplied
  by `1000.0f`, added to a base integer key time, rounded by `FUN_004B9400`,
  and stored as a generated timed-key timestamp. `AddFireRescueMotionKey`
  confirms that these keys use the same integer clock domain as `DAT_00502AFC`.
  The 1000 multiplier strongly suggests seconds converted to millisecond-like
  game-clock units; whether the values mean delay, duration, or phase remains
  open. The Ghidra C-like decompilation omits the x87 arithmetic; the saved
  disassembly trace is `00449340_timing_key_offsets.asm`.
- `FUN_00447920` is reached from FireRescue `setsize`, not `moveto`: it clears
  motion flags, resets the embedded timed-vector track, and records the current
  three-component value at global time plus 10. The direct disassembly shows
  that its fourth stack argument is a float duration: it is multiplied by
  `1000.0f`, added to `DAT_00502AFC`, passed through `FUN_004B9400`, and used
  for the requested-value key. `FUN_0044AE80` consumes the timed keys by
  interpolation. The 1000 multiplier strongly suggests seconds converted to
  millisecond-like game-clock units, but the script's author-facing unit is an
  inference. Fresh C-like output still hides the x87 duration use even after
  the prototype was corrected. See
  [`00447920_timing_key_offsets.asm`](00447920_timing_key_offsets.asm).
  Ghidra labels the helpers `SetFireRescueSizeMotion` and
  `AddFireRescueMotionKey`.

Fresh Ghidra pseudocode exports are stored beside this file as
`00449A80_fresh.c`, `00448A90_fresh.c`, `00470C50_fresh.c`,
`00447920_fresh.c`, and `0044B620_fresh.c`. The five labels and plate comments
were saved to the project-local Ghidra database.

## Reused earlier exports

The previous work produced multiple Ghidra C-like exports for several of these
same addresses. Selected comparison copies are preserved inside the revised
folder at [`reference/legacy-evidence/rescue-moveto/`](../../../reference/legacy-evidence/rescue-moveto/):
`00449A80`, `00448A90`, `0044B620`, `00449340`, `00449FE0`, `0044AE80`, and
`004B9400`. They are historical decompilations, not source. The fresh
`0044AE80` and `0044B620` bodies match the selected old exports after function
renaming; the old `00449340` and `00449FE0` were already used as corroborating
comparisons. The assembly-only `+0xB0/+0xB4` timing reads are direct new
findings because the C-like decompilation omits their implicit x87 operations.

## FireRescue timed-vector consumer

The path helper writes the embedded visual subobject's vector keys through
`AddFireRescueMotionKey` at `root+0x12C`. In the same FireRescue callback,
`UpdateFireRescueActor` calls `FUN_0044B060` on that subobject; the latter calls
`UpdateFireRescueTimedVectorTrack` (`0x0044AE80`) when a track is active. This
routine advances the current key when its timestamp expires, selects the next
active key, calculates the normalized time between them, linearly interpolates
the vector, applies per-axis scale fields, and writes the position when the
change exceeds its threshold. This maps the internal route-state-to-position
interpolation path; the engine's recurring dispatch of the callback remains
unknown. Fresh pseudocode is `0044AE80_fresh.c`.

## Still open

- The author-facing terms and runtime effect of the two trailing `moveto`
  values after their proven contribution to generated key timestamps. Compare
  MBase's `10, 120` with gas_r's `99999, 99999` and obtain runtime captures if
  needed. The FireRescue `setsize` duration is separately shown to scale into a
  future timed key; its authored time unit and frame-visible effect still need
  parity evidence.
- The state-4 callback is now mapped to `UpdateFireRescueActor` at
  `0x00449FE0`, which calls `AdvanceFireRescuePathMotion` at `0x00449340`.
  The recurring dispatcher, cadence, and order relative to other game updates
  remain unknown; see
  [`fire_rescue_state4_update.md`](fire_rescue_state4_update.md).
- The PathPoint `A_Name` to runtime string at object `+0x24` is now mapped
  through the entity initializer and ScriptEngine registration; see
  [`pathpoint_registration.md`](pathpoint_registration.md).
- The meanings of the two values copied to PathPoint offsets `+0x28/+0x2C`
  remain unresolved.
- For a FireRescue initializer with a non-null `C_Model`, the first component
  used by the `00449340` progress gate is derived through a separate model path;
  that branch is not yet mapped. The authored `moveto` fires in MBase and Gas R
  have no `C_Model`.
- The precise script `tell` argument/target delivery behavior. The separate
  player handler maps `setashdensity`; this trace begins at the FireRescue
  command function identified by the `moveto` string reference.

This finding informs both the MBase first slice and rescue maps such as `gas_r`
that also contain authored `RCS_PathPoint` entries. Do not hard-code `mortar1`
or these entity indexes into the shared scene loader.

## Plain-language explanation

MBase tells one fire to move toward a named point when the level starts. The
viewer now finds that point by name and moves the original fire in steps after
it has traveled more than half its authored radius, then reaches the endpoint.
The original game also queues additional FireRescue entities along the route
for a later generic spawn drain. Their eventual world setup, ownership,
lifetime, and timing still need to be ported, so this does not establish full
FireRescue route parity.
