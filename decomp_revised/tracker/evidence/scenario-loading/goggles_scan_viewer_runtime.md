# Goggles scan in the shared viewer

## Retail evidence

The pinned `FDNYFirefighter.exe` is SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Goggles vtable `0x004E2938` maps ACTION press slot `+0x14` to
`0x00462E80`, which clears/rebuilds a target list. The routine accepts root
actor kinds 6 and 2, rejects squared player distance greater than the float at
`0x004E1108`, and excludes kind-2 actors whose state is 5 or 6. Directly
reading the pinned EXE's `.rdata` at `0x004E1108` gives bytes `00 24 74 4A`,
or `4,000,000.0f` (a 2,000-unit radius). Accepted records retain the actor,
an actor-kind-specific transform, an active byte, and distance.

`0x00462730` is the goggles `+0x20` callback. Its child resource is ID
`0x2BF` (703), initialized by `00462A60`; copied `rescue.rrf` maps 703 to
`data/sprites/heatpulse.bmp` with `heatpulse_a.bmp`. The updater resolves each
target's current transform, updates its effect child, and activates it.
The full child animation and RNG dispatch remain open.

The target-effect window is exactly 8,500 game-time ticks. In the pinned EXE,
instructions `00462782–004627CB` compute the scan timestamp minus current time
plus `0x2134`, multiply by the double at `004E2930` (`1/8500`), and check
against `004DC50C` (`0.0f`) and `004DFC24` (`1.0f`). This admits the interval
from scan start through 8.5 seconds, including normal 32-bit clock wrap.
The binary bytes at `004E2930` decode to `0.00011764705882352942`; the two
float bounds decode to 0 and 1.

The item-selection helper calls the new item's `+0x3C(1)` callback; goggles
`+0x3C` is `0x00462660`, whose nonzero branch calls the item's `+0x14` method.
This makes selection-triggered scanning a strong static inference. ACTION
press directly reaching `+0x14` is verified independently by the Player input
dispatcher. The `+0x3C(0)` branch deactivates the target-effect children.

The source does not establish general night vision or broad fire/fog material
changes. Training's copied tutorial says to press the left button to scan with
goggles and press 1 after spotting a survivor; that authored text supports an
interactive scan, not a global visibility effect.

## Port integration

`src/goggles_scan.h` represents the source target filter, range, target IDs,
distances, live positions, activation state, and scan clock. The shared viewer
builds candidates from loaded `RCS_VictimRescue` and `RCS_FireRescue` actors in
BSP entity order, using their current preview positions and victim state. A
selection transition into goggles schedules a scan through the mapped
`+0x3C(1) -> +0x14` route; left-click while goggles are selected runs the
verified ACTION scan. Leaving goggles deactivates the host scan state.

`src/goggles_renderer.h` combines the copied retail heatpulse color/alpha pair,
follows the host's current accepted actor positions each frame, and draws the
effect only within the verified 8.5-second window. This does not reproduce the
retail attachment: `00462730` obtains an actor interface through vtable `+0x18`,
transforms the actor-local point at `+0x28` into effect space, then updates and
activates a child resource. The preview instead places a camera-facing quad at
the host actor position plus an estimated vertical offset. The exact local
point, actor transform, and child attachment remain open. The 64-unit world
size, neutral tint, alpha blend, and billboard orientation are preview
approximations. Child animation, random update ordering, kind-specific effect
colors, and retail effect-render ordering also remain open.

The viewer reports survivor/fire target counts in its existing window-title
and transient hint. This is host feedback for the recovered target list, not a
claim about retail HUD presentation. The current candidate list contains only
BSP-loaded fires and victims; dynamic actor registration and exact retail
manager/update order remain open. No global visibility adjustment or fog/fire
visibility change is implemented.

## Reproducible interaction

Launch Oil Rig A with
`"./Open FDNY Scene Preview.command" oil_rig_a`, select the equipped goggles
with `3`, then left-click when authored survivors or fires are within range.
The renderer shows heatpulse markers at approximate host target positions for
up to 8.5 seconds; the window title includes the scan target count. Switching
to another tool deactivates this state. The selection key and status display
are viewer controls, not retail key/HUD mappings.

## Open work

- Finish the `+0x20` effect-child animation, random update ordering, and exact
  actor transform/color behavior; the resource and 8.5-second window are now
  traced and the shared viewer uses the retail heatpulse texture pair.
- Resolve kind-2/kind-6 targets against the live entity roster, including
  dynamic actors, and establish per-item callback ordering.
- Determine whether scan records or effect children alter any authored
  visibility state. No global visibility effect is supported by current
  evidence.
