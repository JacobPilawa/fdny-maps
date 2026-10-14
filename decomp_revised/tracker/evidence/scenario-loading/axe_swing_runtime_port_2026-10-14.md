# Axe swing and strike checkpoint (2026-10-14)

## Retail behavior established

The saved Ghidra decompilation is for the pinned retail executable
`FDNYFirefighter.exe` (SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). The
relevant exports are `0045F8F0`, `0045FAD0`, `00460920`, `00460B90`,
`004610E0`, `0045FEC0`, and `0045FB90` in
`reference/previous-work/decomp/evidence/tool-decompiled/`.

- Axe ACTION press `0045F8F0` stores global clock +100 ms. The held route waits
  until the global clock is strictly past that deadline before updating the
  primary Axe child.
- In child Think state 1, `004610E0` calls strike handler `00460B90`. That
  handler calls `0045FEC0(Axe, 1.0)` first. `0045FEC0` advances the child
  animation clock by the engine frame delta and calls `0045FB90`; that helper
  copies the old value at Axe `+0x33C` to `+0x340` and stores the new value at
  `+0x33C`.
- The strike gate is a strict **crossing**: previous `+0x340 < 0.275` and new
  `+0x33C > 0.275`. The earlier checkpoint's “both timers above 0.275” reading
  was incorrect. The viewer now samples the equivalent crossing rather than
  scheduling a fixed click-relative millisecond deadline.
- After passing the gate, the source builds a swept box from Player view-frame
  vectors and queries `00492CE0` with mask `0x43`, collision type `2`, and
  `+/-6` extents. A world miss returns before the Axe Hits statistic and state
  transitions.
- On a world hit, the strike traverses the type-4 candidate list and accepts
  the first BModel whose `+0x240` min / `+0x24C` max world AABB overlaps the
  adjusted impact point expanded by 8 units, inclusive. `0049E770` confirms
  that the impact adjustment copies Player frame `+0x1C`.
- `0043E2A0` requires nonzero UserData `+0x34`, zero UserData `+0x38`
  (`G_Liftable`), and BModel `+0x1EC` outside the near-zero interval. A valid
  hit dispatches `chop`, decrements hit points, and dispatches `break` plus
  `0043D660` when the count reaches zero.

Training authors two-hit, non-liftable BModels for `breakable_chair`,
`breakable_chair1`, `breakable_box`, and `breakable_box1` (entities 112, 113,
212, and 213). The chair entities have authored touch hints saying they can
be chopped.

## Port checkpoint

`scene_viewer.cpp` maps left mouse down to Axe ACTION, uses the source-derived
+100 ms deadline, starts the exported `axel.act` `swing` track after the
deadline, and advances its local swing clock by rendered-frame `dt`. It runs
the swept query only on the sampled strict 0.275-second crossing. The world
query still adapts to the preview's `Collision::trace`; BModel candidate
acceptance now uses the merged world-space child collision bounds expanded by
8, matching the retail min/max AABB check instead of comparing against
`ZZ_location`. Accepted targets receive authored `chop`/`break` dispatch;
the hit leaf updates hit points and starts the target's existing `open` PATH
on break.

The window hint and title now report Axe action/sweep outcomes (swinging, no
world hit, world hit without an eligible breakable, or chopped target), so a
miss is distinguishable from a successful hit during user testing.

The preview camera root was lowered from 27 to -8 units on its camera-up axis
after the exported swing sample at 0.30 seconds clipped its axe head above the
viewport. The refreshed `--tool-orientation-check axel` shows the complete
0.30-second pose in frame and reports 53,610 changed pixels against bind pose.
This is a render-only port diagnostic and an inferred placement adjustment; it
does not recover the retail hand transform or establish the reported facing.

## Confidence and open work

**Verified from saved retail code/data:** press deadline, animation-clock
advance/copy, strict timer crossing, sweep extents/mask/type, impact-frame
copy, inclusive BModel AABB comparison, hit-point/liftable/strike-time gates,
and Training's authored breakable values. All eight primary rescue packages
pass their existing synthetic release check; all five linked stage packages
load with the current scene loader.

**Inferred in the viewer:** left mouse is the Axe ACTION binding; SDL ticks and
rendered-frame `dt` stand in for the retail clocks; the sampled `axel.act`
`swing` track corresponds to the retail primary-child animation; the loaded
scene's collision adapter represents the retail world; the merged BModel list
represents retail type-4 candidate order.

**Verified in the port renderer:** lowering the provisional root keeps the
sampled 0.30-second swing fully in frame, and the image changes relative to
bind pose. This does not verify an interactive click, a collision hit, or
retail pose parity.

**Provisional/open:** child callback cadence and action-release behavior,
runtime type-4 list membership/order, camera-frame mapping, exact live
animation-to-ACT timing, hit sound/follower effects, and the final attached
ActorInfo pose. Retail's equipment pose is composed through
`00497E90 -> 00497F70 -> 00492110 -> 00492450`; the viewer still supplies a
direct camera root. A half-turn made the exported Axe edge-on in the port-only
render and is not a supported correction. A same-view retail comparison or
live matrix sample is still required to solve the reported backward-facing
view and claim 1:1 swing appearance. No interactive retail/user capture was
performed for this checkpoint.
