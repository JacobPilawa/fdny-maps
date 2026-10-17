# Held equipment action and pose follow-up (2026-10-17)

This checkpoint follows the user's report that the extinguisher appears
nonfunctional and the Axe swing moves toward the player. Retail evidence was
checked against `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra 12.0.4 traces used temporary headless projects; the saved user Ghidra
project was not opened or modified.

## Axe

- Retail constructs two Axe children. The child at Axe `+0x200` has a child
  variant flag set and loads model 6 (`axel.act`); the child at `+0x648` has
  that flag clear and loads model 1 (`axe.act`). The root Axe `+0x1FC` gate is
  a separate field, initialized to zero. With that ordinary gate state,
  `0045F9B0` activates/updates only model 1 and always dispatches the secondary
  child. Duplicate `equip axe` at `0044D152` sets the root gate and enables the
  model-6 child too. The viewer previously conflated the root and child flags,
  enabled the root gate on first equip, and always rendered model 6. It now
  preserves the gate on first equip, matches the duplicate branch, and renders
  model 1 for ordinary use (both children when the duplicate gate is enabled).
  The same-hash decompilations are
  [`00460A50`](../../../reference/previous-work/decomp/evidence/tool-decompiled/00460a50_FUN_00460a50.c),
  [`004600F0`](../../../reference/previous-work/decomp/evidence/tool-decompiled/004600f0_FUN_004600f0.c),
  [`00460250`](../../../reference/previous-work/decomp/evidence/tool-decompiled/00460250_FUN_00460250.c),
  and
  [`0045F9B0`](../../../reference/previous-work/decomp/evidence/tool-decompiled/0045f9b0_FUN_0045f9b0.c).
- `0045F660` initializes the tool component-local transform to identity and
  parents it to the Player view follower; it supplies no fixed hand offset or
  Axe-specific root correction. Both extracted Axe ACTs contain a named
  `swing` track with positive early local-Z translation. The retail action
  callback advances and samples the active child's ActorInfo clock but does
  not explicitly choose a motion by name. The `swing` track is therefore a
  strong mapping, not an explicit name found in the callback.
- Retail's attached actor passes through `00497F70` → `00492110` → `00492450`.
  Genesis `geActor_SetPose` (export `10007180`, implementation `10010650`)
  composes the sampled motion-root transform with the supplied ActorInfo pose
  and applies sampled per-bone channels. The engine's camera projection uses
  negative camera-space Z as its forward half-space. Fresh frame-helper output
  does not add a sign inversion to the vector written into Player view-frame
  basis `+0x1c..+0x24`.
- The exact matrix passing from the Player view follower through the mode-2
  component into ActorInfo and Genesis has not been numerically reconstructed.
  The viewer still uses the camera axes directly and places the ACT with a
  guessed camera-relative origin `(35, -8, 140)`. Under the current camera-root
  candidate, the attachment-composed root translation projects along the view
  ray, which points from the eye into the scene; that translation moves away
  from the camera. A previous report incorrectly described positive
  view-ray projection as motion toward the viewer. Per-vertex rotation can
  still change that result, so the audit now reports both signed projections
  for the entire mesh. The source-backed model correction does not itself
  prove the perceived direction or retail parity. Static traces do not yet
  prove which matrix conversion reproduces retail. A half-turn previously made the
  exported Axe edge-on in the port renderer, so that visual guess is not a
  source-backed correction.
- Follow-up confirmed that retail's component-frame-to-ActorInfo basis packing
  matches the port's `actorRootFromFrame` conversion. Retail copies the three
  frame basis vectors into ActorInfo rows; Genesis `geXForm3d_Multiply` and
  `geXForm3d_Transform` use the corresponding 3x4 basis convention. There is no
  supported transpose or basis-order fix at this boundary. A deeper trace
  recovered Genesis's attachment-validity test: `FUN_1000FA60` sets flag 1
  only when the ACT attachment quaternion's X/Y/Z components are all in
  `(-0.0001,+0.0001)`. Axe `BONE06` converts to approximately
  `(0.8280,-0.5255,0.1626,-0.1086)`, so it takes flag 0; Genesis then applies
  attachment × sampled rotation and transforms sampled translation through
  the attachment matrix. This matches `actor_pose.h` for the gated model-6
  child. It does not settle the default model-1 attachment or the reported
  direction. The remaining unknowns are the camera world-transform
  caller/numeric relation to Player view frame `+0x34A4` and the preview's
  guessed `(35,-8,140)` root origin. Do not add an unverified sign flip.
  Pinned inputs are Genesis.dll
  SHA-256 `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`
  and retail ACT assets: `axe.act` SHA-256
  `754eea0f17f3cca7ea33ec32e117212fd05fe34ebdac89d86f811f65184fef87`,
  `axel.act` SHA-256
  `cfdc33f89ee4840dbbde05118b9e096c43538be214c0fb1ddc043f664de193c1`.

The retail view-frame evidence and impact of a correction are recorded in
[`equipment_view_frame_trace_2026-10-17.md`](equipment_view_frame_trace_2026-10-17.md).
The Genesis actor and camera evidence is in
[`actor_camera_pose_trace_2026-10-17.c`](../../genesis-boundary/actor_camera_pose_trace_2026-10-17.c).

## Extinguisher

Retail setup `00461830` initializes a local pose at `(-30, -18, 40)` with
direction `(.2, -1, .1)`. Action start `004614E0` opens a 300 ms transition;
release `00461590` starts a 1000 ms return. Think `00461F80` samples the prior
ActorInfo frame, constructs four spray contact points, queues message 30000
for accepted fire contacts, and requests pool particles after its cooldown
gate.

The shared viewer aligns one interpolated extinguisher pose for its ACT and
contact sampling and routes the three retail action sounds. It now consumes
eligible particle requests and draws a visible two-second smoke quad with the
resource-14 `bsmoke.bmp` / `sm_alpha.bmp` candidate pair. The pool spawn
origin, cooldown, random velocity draws, zero acceleration, and two-second
lifetime are source-mapped. The runtime pool-to-RRF association, exact quad
orientation and size, alpha track over time, blend/clip behavior, and
FireEntity origin equivalence are still provisional. No successful live
suppression hit has been captured, so the user's report is not marked fixed.
See [`extinguisher_particle_render_trace_2026-10-17.md`](extinguisher_particle_render_trace_2026-10-17.md).

The opt-in `--fire-camera-audit` now logs equipment selection and left-button
down/up edges, plus an active extinguisher frame every 250 ms: action/charge,
frame origin and direction, all four points per live
fire, fire bounds/alpha/strength, point influence and eligibility,
message-30000 delivery, and strength/alpha before and after delivery. Its
summary reports accepted and delivered contacts. This instrumentation is not
a live capture. Run the packaged Training scene from a terminal, select slot
2, and hold the action over a visible fire to determine whether contacts miss
or suppression changes without obvious feedback.

Follow-up ruled out a proposed parent-scale correction. `00483A80` writes
`.006, .009, .006` at follower `+0x5C` and sets interpolation flags at `+0x6C`;
these are smoothing rates, not hierarchical transform scale. The follower
constructor `00483B00` calls `0042D2D0`, which initializes transform scale at
`+0x34..+0x3C` to `(1,1,1)`. `00497300` uses that unit scale when composing the
extinguisher component under the follower, so the port must not multiply its
local contact offsets by `.006/.009/.006`. Current input, four-point contact,
queue, and nonterminal suppression routes have no deterministic drop found in
static source review. A live contact-frame sample is still needed to
distinguish an aimed-away nozzle from a weak accepted hit.

The same-hash follow-up confirms the accepted contact's raw influence is
doubled before message 30000 reaches the fire, where the source response uses
`1-(1-influence)^2`. A point near the fire center can therefore arrive with an
influence near 2 and produce almost no reduction; values near 1 produce the
largest response. The port currently matches this retail calculation, so
changing the curve would break the available source evidence. This may explain
why the tool feels inert when aimed at the center, but the contact frame and
the user's reported case have not been captured live.

### Field-base confirmation

A follow-up checked the offsets against the FireRescue object bases before
considering a separate visual-alpha fix. `UpdateFireRescueActor` receives the
embedded actor subobject at root `+0x24`, so its actor-relative `+0x104` is the
root suppression-strength float at `+0x128`; actor-relative `+0xE0` is the
root recently-sprayed byte at `+0x104`. Thus retail's recovery and alpha input
share the suppression-strength field, and the existing port mapping is
source-aligned. The separate-alpha hypothesis was a base-offset error; no
change to the suppression or alpha math is justified. This confirms the
equations only, not that the viewer's four contact points intersect the fire.

## Build and remaining work

The macOS `fdny-scene-viewer` target and rescue-all app bundle compiled after
correcting first-equip Axe model/gate handling and adding extinguisher
selection/button-edge audit events. Build output and packaged executable
match at SHA-256
`f8e006e636d1ef0101b0284904dc2927f98047c68bee7834a8e8885e8b6736f7`. Homebrew
`json-c` reports a macOS 27 build target while this app targets macOS 13.3. No
interactive input, live suppression contact, or retail pixel comparison was
run; the build confirms compilation and packaging only.

The first packaged orientation diagnostic then exposed an SDL startup error on
the retail resource-14 `bsmoke.bmp`: the 8-bit BMP declares 254 palette entries
but ten pixels use palette indices 254/255. The sprite exporter now creates
valid local 24-bit BMPs, preserving declared colors and padding those two
undeclared colors with black; the retail files are untouched. The packaged
`--tool-orientation-check axe` subsequently loaded all 121 render groups and
24 textures and saved the camera-root, half-turn, and 300 ms swing frames.
This is a port asset-loading/render diagnostic only. It does not establish a
successful extinguisher contact or retail pose match. The missing palette
colors and final Axe camera-to-ActorInfo transform remain explicit parity
limitations.

Next, trace the camera world-transform caller and compare its basis to the
Player follower before correcting Axe direction. A global view conversion would
affect map camera orientation, extinguisher contact direction, goggles
placement, and every held tool; a per-tool change would affect both the
model's facing and animation path. For the extinguisher, capture one contact
frame's action state, four points/influences, queue delivery, and fire strength
before/after the next update, then verify suppression and spray placement
across the rescue missions. Menu work remains paused at the user's request.

## 2026-10-17 follow-up: contact basis, camera semantics, and fire tint

The extinguisher sample audit compared retail `00496B50` with
`ExtinguisherSamples::points`. Retail rebuilds X as normalized
`cross(rotated-Y, rebuilt-Z)` after it rebuilds Z from original X and rotated
Y. The port leaves that final X at its old value. However, the four sampled
contact points use only the rebuilt Y and Z, so this final-X difference does
not change the current point coordinates. The local point offsets, sample
spacing, and matrix packing otherwise match. The actionable failure remains
the unresolved parent frame and frame-evaluation timing; no extinguisher
contact geometry change is justified by this X-only difference.

An independent view-frame trace confirms `0044F340` supplies target-minus-
position to `00496360`, which stores the normalized vector in Player-frame
`+0x1C` without an explicit inversion. This proves a positive target direction
in the Player frame; it does not prove that the vector is Genesis camera-world
forward or the visible ray. The viewer's `ViewUpdate::transition` puts the
same vector in `cameraFrame.axis[2]`, while `RenderScene` transposes that basis
and OpenGL projects visible depth along local `-Z`. The retail Player follower
has not been joined numerically to the Genesis camera-world transform and the
final held-item ActorInfo matrix. This convention tension could affect both
the Axe swing and extinguisher contacts, but a frame flip would also change
the rendered view and contact orientation. Keep both reports open pending
that join.

The renderer audit also recovered the retail fire tint. Below suppression
strength `0.8`, the callback blends `(0.9, 0.3, 0.1)` toward white and applies
the RGB override to four flame children before recovery. Raw values in the
pinned EXE confirm `_DAT_004E1680=0.8f` and `_DAT_004E3524=255.0f`. The shared
`FireEntity`/`FireRenderer` now carry and draw the same normalized tint. When
retail clears the override, the port falls back to white instead of evaluating
the children's authored RGB tracks; fire pixel parity remains open. This gives
stronger suppression feedback but does not establish that contacts are
reaching a fire. Detailed helper traces are linked from
[`visual-terminal-state.md`](../fire-spread/visual-terminal-state.md).

## 2026-10-17 extinguisher local-frame recheck

A fresh headless Ghidra trace against the pinned EXE reconfirmed
`004612B0`'s local-frame construction. It negates and normalizes the supplied
direction (`0042D2B0`, `00446390`), forms the basis with cross products
(`004476B0`), and orthogonalizes it through `004964B0`. The shared
`ExtinguisherPose::fromVectors` performs the same direction negation and
cross-product sequence, so no local direction sign correction is supported.
The pinned pseudocode is preserved in
[`extinguisher_pose_frame_trace_2026-10-17.c`](extinguisher_pose_frame_trace_2026-10-17.c)
and
[`extinguisher_pose_vector_helpers_2026-10-17.c`](extinguisher_pose_vector_helpers_2026-10-17.c).

This check does not join that local frame's runtime parent/ActorInfo matrix to
the rendered camera. The contact points can still face or sit away from a fire
because the viewer's tool follower parent remains provisional; only a live
sample/contact capture can distinguish that from the source-derived weak
center response. The Axe camera-to-ActorInfo gap remains separate and open.
