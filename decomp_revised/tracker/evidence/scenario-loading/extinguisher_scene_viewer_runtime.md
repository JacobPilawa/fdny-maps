# Extinguisher suppression in the shared scene viewer

## Integrated behavior

When the BSP-authored PlayerRescue loadout contains an extinguisher and slot 2
is selected, holding the viewer's left mouse button starts the existing
extinguisher action window. The shared host reuses the Training workbench's
source-derived `ExtinguisherPose`, `ExtinguisherSamples`,
`ExtinguisherContacts`, `FireContact`, `FireMessageQueue`, and
`ExtinguisherEffects` components.

Each active host frame it advances the active pose, computes the four authored
spray sample points, tests each point against the live FireRescue size,
position, and alpha with padding 60, and accepts influence at or above 0.001.
Accepted contacts carry twice the computed influence in a 16-byte message
30000 record. The receiver resolves the BSP entity route and calls the shared
suppression transition with `finalize=false`, matching the extinguisher
message path. The shared fire update then changes its strength and alpha.
Accepted contacts also advance the source-shaped effect timer, consume its
random emitter-offset draws, and issue a visible host particle request.

The shared follow-up now consumes that request. Eligible contacts spawn a
two-second particle at the current `FireEntity::origin` with the source-mapped
velocity draws. The host renders a provisional camera-facing quad using the
resource-14 `bsmoke.bmp` / `sm_alpha.bmp` pair declared in `rescue.rrf`. The
resource-manager link, exact quad orientation/extent, alpha interpolation,
blend flags, engine clipping, and host-origin equivalence remain open. See
[`extinguisher_particle_render_trace_2026-10-17.md`](extinguisher_particle_render_trace_2026-10-17.md).

The contact queue is reset and drained within the host frame. Its route uses
the stable BSP entity index plus a host generation value; it does not recreate
the retail pointer registry. The viewer's initial charge is 1.0, matching the
existing Training host, and its `toolcmd extinguisher refill` command restores
that value. Retail initial charge and refill parity across all scenarios are
still open.

## Evidence and provisional host inputs

The retail chain is recorded in [the fire-spread audit](../fire-spread/README.md):
`0x00461F80` samples the tool pose and scales accepted influence by two;
`0x00481A60` queues four-word message 30000 records;
`0x00449A40` dispatches them to `0x00448970` with `finalize=0`; and
`0x0044A7A0` computes the padded fire falloff. The suppression transition is
nonlinear and uses the global frame delta. The extinguisher world transform
reuses the constructor pose and item follower mapped in the existing Training
workbench.

The map-neutral viewer has no rendered Player/tool actor. It supplies its
current camera frame as the tool follower target. The left-button action is a
viewer binding, and the contact/action cadence follows the rendered host frame.
Those parent-pose and schedule choices are provisional; contact/falloff/
suppression math is reused from the source-backed components. The 2026-10-17
follow-up now blends the displayed ACT and contact frame together and routes
the three extinguisher action sounds and displays a source-shaped spray effect,
but an interactive suppression hit has not been observed. The visible scale,
camera-space attachment, particle texture lookup, and particle presentation
remain provisional. See
[`equipment_action_pose_followup_2026-10-17.md`](equipment_action_pose_followup_2026-10-17.md).

Static failure review found no deterministic input, queue, or suppression
drop in the current path. Retail follower values `.006/.009/.006` are
per-axis smoothing rates, while its transform scale is `(1,1,1)`; they must not
be applied to the contact offsets. The unresolved question is whether the
camera-parented points intersect a fire in a live frame. A useful capture needs
selected slot and ACTION state, the four points and influences, append/queue
delivery, and fire strength/alpha before and after the next fire update. The
retail contact message uses `finalize=false`, so a weak accepted hit can look
subtle until the fire presentation updates. Do not mark the reported behavior
fixed without a successful contact capture.

`FireEntity::spray` no longer immediately disables flame sprites at terminal
suppression. Retail code sets mode 5 and clears byte `+0x1C8`; the byte's
meaning and the renderer's terminal fade/removal behavior remain unknown.
`FireEntity::think` returns early in mode 5, so do not equate that state with
instant visual disappearance.

## Packaged texture-load correction

The first packaged orientation diagnostic on 2026-10-17 failed before scene
startup because SDL rejected the retail `bsmoke.bmp`: the source BMP declares
254 palette entries but ten pixels use undeclared indices 254/255. The
exporter now makes local 24-bit BMP copies of both indexed sprites, preserving
declared palette colors and padding those two missing smoke colors with black.
The retail source files remain unchanged. The exact retail loader behavior for
the two malformed palette indices is unknown, and the resource-14-to-particle
pool association remains a candidate mapping. A later packaged Axe render
diagnostic loaded the scene, all 121 groups, and 24 textures. This resolves the
SDL startup error; it does not verify that an extinguisher hit suppresses fire.

## Try it

To capture the live contact path, launch the packaged Training scene from a
terminal with `--fire-camera-audit`:

```sh
".local/FDNY Firefighters.app/Contents/MacOS/fdny-scene-viewer" \
  ".local/FDNY Firefighters.app/Contents/Resources/scenes/training" \
  --fire-camera-audit 2>&1 | tee /private/tmp/fdny-extinguisher-audit.log
```

Select slot 2 and hold the left mouse button over a visible fire. The trace
prints whether slot 2 selected, each left-button down/up edge, the sampled
points and influences, accepted/delivered contacts, and suppression
strength/alpha changes. It also records the post-ViewUpdate camera and head
offset, and—when Axe is selected—the ACT root pose and per-vertex movement along
the view ray. Each extinguisher sample reports raw influence, doubled message
influence, expected response coefficient, and expected per-frame strength
delta. `action-input edge=down selected=extinguisher` confirms the input
reached the active tool. A zero-contact summary points to the provisional
pose/contact geometry; accepted contacts without delivery point to message
routing; a delivered strength decrease means suppression ran even if the
visual change is subtle. Stop the app from the terminal with Control-C, then
share the `fire-camera-audit` lines from the terminal or log.

To launch any exported rescue scene without audit logging, use:

```sh
./Open\ FDNY\ Scene\ Preview.command mbase
```

Press **2** to select the extinguisher, aim at a fire, then hold the left mouse
button. Eligible contacts now produce visible spray particles and call the
source-backed suppression transition, but an interactive successful hit is
not yet verified. The visible ACT shares the contact pose; its 0.25 scale and
attachment remain provisional. Random spread and the Player `0x7531`
exposure/pain/passout path are connected to the shared viewer. Fire exposure
cadence, the retail candidate's extra falloff radius, exact spray quad size,
alpha, blend and clipping, passout animation, and the persisted `Serious Burns`
statistic remain open;
details are recorded in
[`player_hud_goggles_equipment_fire_viewer_runtime.md`](player_hud_goggles_equipment_fire_viewer_runtime.md).

## Build verification

The `fdny-scene-viewer` target and rescue-all app bundle compiled on 2026-10-17
with the live contact audit plus slot-selection and button-edge logging; the
executable and packaged app match at SHA-256
`f8e006e636d1ef0101b0284904dc2927f98047c68bee7834a8e8885e8b6736f7`. The
linker reports that Homebrew's `json-c` library was built for macOS 27 while
this target links for macOS 13.3; the build still completes. Interactive
suppression, spray placement, and fire-exposure behavior have not been captured.
The post-conversion packaged diagnostic loads the scene and textures, but is
not an interactive extinguisher check.

## Expanded camera and contact audit build (2026-10-17)

The same rescue-all build now includes the post-ViewUpdate camera and ACT
per-vertex audit fields described above. Build-tree and packaged app
executables match at SHA-256
`ea5f1bd5d5fb298ca2256be4ec21480edbf07e033480b555c732dda570de9349`.
`git diff --check` passed. No interactive suppression or visual check was run;
the inert-extinguisher report remains open until the audit shows a delivered
contact and the corresponding fire response.

## Focused report recheck (2026-10-17)

Static review again found the selected-item/button edge, four contact samples,
message-30000 queue/delivery, and nonterminal fire suppression connected in
the shared viewer. The retail response curve is intentionally nonmonotonic:
raw influence is doubled, then `1 - (1 - influence)^2` is applied. A center
sample near raw influence 1 can therefore produce almost no strength loss.
Changing that equation or clamping the influence would break the available
retail evidence. The unresolved problem is whether the current camera-parented
sample points intersect the fire and what values they deliver. No live capture
was available at the documented audit-log path, so the user's report remains
open and no aim/contact correction was guessed.

The local pose builder itself was rechecked from retail `004612B0`: its
direction negation, normalization, cross products, and final orthogonalization
match `ExtinguisherPose::fromVectors`. Hash-pinned pseudocode is recorded in
[`equipment_action_pose_followup_2026-10-17.md`](equipment_action_pose_followup_2026-10-17.md).
This rules out a local direction-sign fix but leaves the tool's parent/world
matrix and live fire intersection unresolved.
