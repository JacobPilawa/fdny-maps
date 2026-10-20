# Training trigger brush render visibility

Updated 2026-10-13. This note records the red-volume investigation and the
shared renderer change. It does not establish complete material or fog parity.

## Verified cause

The Training export at `.local/exports/training/` contains 25
`RCS_BoundsTrigger` entities. Joining each `C_Model` through `%Model%` to
`world_mesh.json` resolves all 25; every joined model has a render group using
the authored `col_red` texture. This explains why these trigger brushes appear
as solid bright red blocks in the current viewer. It is a BSP-model render
path issue, not a red debug overlay created by the contact code.

The viewer already resolves those same authored model indices in
`loadBoundsTriggers`, but `renderScene` submitted every world mesh group
without a world-model flag state. No `glFog` call or fog configuration exists
in this renderer, and Training has no `fogenable`/`fogdisable` command in its
exported scripts. Fog therefore does not explain the Training blocks.

## Retail flag path

Pinned `inputs/original/binaries/FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

- Fresh Ghidra decompilation of `InitializeBoundsTriggerEntity` at `00441040`
  is saved in
  [`00441040_bounds_trigger_registration.c`](../scenario-loading/00441040_bounds_trigger_registration.c).
  Raw x86 confirms that `00441079` clears `EBX`, then `00441109–0044110B`
  pushes `EBX` (zero) and the resolved model pointer before calling the
  Genesis import `geWorld_ModelSetFlags` at `0044110B`.
- Fresh Ghidra pseudocode and assembly for `DispatchBoundsTriggerContactEvents`
  at `00440DC0` are saved in
  [`00440dc0_bounds_trigger_update_fresh.c`](../scenario-loading/00440dc0_bounds_trigger_update_fresh.c)
  and
  [`00440dc0_bounds_trigger_update_fresh.asm`](../scenario-loading/00440dc0_bounds_trigger_update_fresh.asm).
  The updater only changes flags when embedded ScriptActor `+0x141` is set,
  which maps to BoundsTrigger root `+0x165`. It sets the model flags to the
  boolean phase value. The raw phase is on for `[500,600)`, `[700,800)`, and
  `[900,1000)` milliseconds modulo 1000; it is off during the other intervals.
  Retail callers of `00440F50(true)` include the final-exit path described in
  [`training_touch_dispatch_boundary.md`](../scenario-loading/training_touch_dispatch_boundary.md),
  but the helper only writes root `+0x165` when root `+0x164` is set. Setup
  initializes `+0x164` only for an authored `touch: tell player escape`
  binding, so final-exit eligibility by itself does not enable the render pulse.
- Pinned `inputs/original/binaries/Genesis.dll` SHA-256:
  `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
  Raw x86 at `1003C160` stores the supplied flags word at model offset `+0xCC`;
  getter `1003C170` reads the same word. This verifies storage, not the shipped
  renderer's interpretation of every flag bit. A separate collision trace
  proves bit `0x10` excludes models from swept-hull and point/ray collision;
  its renderer effect remains unknown. See
  [`genesis_collision_path.md`](genesis_collision_path.md).

## Port change and confidence

`scene_viewer.cpp` now initializes a per-model flag map from the resolved
BoundsTrigger records, starting each model at zero as retail does. The general
world-mesh draw loop skips a model when the normal-render bit is absent. Models
without a ported `ModelSetFlags` write keep the existing draw behavior. No
texture-name, model-number, or class-name render filter was added; the change
ports the authored state transition from the trigger entity setup.

The viewer mirrors the three retail pulse windows only when the trigger has
the authored `tell player escape` binding and is in carry/final-exit state.
Sampling that state once per rendered frame is provisional because the retail
BoundsTrigger updater's caller cadence and exact clock source remain unknown.
The viewer still keeps trigger collision/contact processing independent from
render visibility. Final-exit membership alone does not enable a trigger model.

The render-bit interpretation is **inferred**, not verified in FDNY's shipped
renderer. In the public Genesis3D comparison, `World.c` writes `Model->Flags`
and its normal render pass skips submodels unless `GE_MODEL_RENDER_NORMAL` or
`GE_MODEL_RENDER_ALWAYS` is set. That source is a comparison only; FDNY's
renderer may differ. See [upstream `World.c`](https://raw.githubusercontent.com/RealityFactory/Genesis3D/master/World/World.c).

## Separate fog and material work

Fog is a distinct open renderer path. Other authored mission scripts, such as
Gas R, use `fogenable` and `fogdisable`; retail command dispatch and the
Genesis-to-driver fog-setting route are recorded in
[`selected_driver_initialization_trace.md`](../driver-boundary/selected_driver_initialization_trace.md).
The viewer currently has no matching fog state. A follow-up trace now connects
the retail `fogenable`/`fogdisable` helpers through interpolation and camera
far-clip updates to `geEngine_SetFogEnable`; the exact authored value meanings,
driver range conventions, and visual effect remain open. See
[`fog_helper_to_driver_path.md`](fog_helper_to_driver_path.md). Fog can affect
scene distance and appearance, but this trace does not account for the discrete
red trigger brushes. Bank/Gas R red-face candidates, face-level TexInfo alpha,
blend/depth rules, lightmaps, and exact FDNY model-flag semantics still require
investigation. Do not extend this fix into a `col_red` filter.

## Validation

- Static export join: 25/25 Training BoundsTrigger entities resolve to models;
  25/25 resolved models include a `col_red` render group.
- `fdny-scene-viewer .local/exports/training --render-check` completed with 25
  triggers loaded, 121/121 mesh groups loaded, and 24/24 textures uploaded.
  The inspected 1280x800 spawn-camera frame shows concrete walls but no
  trigger brush; that generic camera check did not directly verify a trigger
  in view. A focused `--trigger-render-check 106` then compared Training's
  hint trigger entity 106/model 3 with the flag forced to 1 and with flags set
  to zero. The forced-on image was solid red across the 1280x800 frame, while
  the zero-flag image showed the surrounding textured scene; all 1,024,000
  pixels differed. This confirms the port's per-model render gate suppresses
  that authored red brush when applying the retail initialization value. The
  frame pair is saved locally as
  `.local/exports/training.trigger-visible.png` and
  `.local/exports/training.trigger-hidden.png`; the generic image remains
  saved locally as
  `.local/exports/training.render-check.png`.
- The macOS target compiles successfully. The existing link warning reports
  Homebrew json-c built for macOS 27.0 while this target is macOS 13.3. The
  focused render comparison tests the port's flag gate; it is not a capture
  from the original retail renderer.
- The current rescue-all package repeats the Training `--render-check` and
  forced-visible/flags-zero `--trigger-render-check 106` successfully. Its
  final binary hash and broader victim/trigger integration checks are recorded
  in [`victim_contact_message_port_2026-10-14.md`](../scenario-loading/victim_contact_message_port_2026-10-14.md).
