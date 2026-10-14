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
Accepted contacts also advance the source-shaped effect timer and consume its
random emitter-offset draws; the resulting particle request is not rendered.

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

The map-neutral viewer has no rendered Player/tool actor or retail player-view
follower yet. It supplies its current camera position and yaw/pitch basis as
the tool follower target. The left-button action is a viewer binding, and the
contact/action cadence follows the rendered host frame. Those parent-pose and
schedule choices are provisional; contact/falloff/suppression math is reused
from the source-backed components. Actual particle rendering and extinguisher
audio remain open.

`FireEntity::spray` no longer immediately disables flame sprites at terminal
suppression. Retail code sets mode 5 and clears byte `+0x1C8`; the byte's
meaning and the renderer's terminal fade/removal behavior remain unknown.
`FireEntity::think` returns early in mode 5, so do not equate that state with
instant visual disappearance.

## Try it

Build and launch any exported rescue scene with:

```sh
./Open\ FDNY\ Scene\ Preview.command mbase
```

Press **2** to select the extinguisher, aim the viewer camera at a fire, then
hold the left mouse button. A successful hit lowers the fire's live strength
and sprite alpha over subsequent updates. The decoded extinguisher ACT draws
at a separate camera-relative presentation root with a provisional 180-degree
rotation around camera up, based on the user's backward-model report. That
render transform is independent of the source-derived contact pose and remains
unverified against retail; the 0.25 scale and screen offsets are also
provisional. Spray particles remain absent. Random spread and the Player `0x7531`
exposure/pain/passout path are now connected to the shared viewer. Fire
exposure cadence, the retail candidate's extra falloff radius, emitter scale,
passout animation, and the persisted `Serious Burns` statistic remain open;
details are recorded in
[`player_hud_goggles_equipment_fire_viewer_runtime.md`](player_hud_goggles_equipment_fire_viewer_runtime.md).

## Build verification

The `fdny-scene-viewer` target compiled, and
`tools/build_scene_viewer_app.sh mbase` regenerated the MBase app bundle.
The linker reports that Homebrew's `json-c` library was built for macOS 27
while this target links for macOS 13.3; the build still completes. Interactive
visual placement and fire-exposure feel have not been captured.
