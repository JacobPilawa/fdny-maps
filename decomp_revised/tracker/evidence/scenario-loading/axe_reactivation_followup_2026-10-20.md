# Axe inventory reactivation follow-up (2026-10-20)

## Provenance

Retail executable: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Read-only source trace cross-checked the retained item-selection audit and
Ghidra exports against this pinned executable. No interactive runtime capture
was performed.

## Findings

The existing-Axe branch in `0044D080` sets Axe root byte `+0x1FC` to 1 and
returns. It does not allocate another Axe, run ordinary slot selection, or
explicitly reset a child/action state. Ordinary slot selection reaches
`0045F930`: when `+0x1FC` is nonzero it checks primary-child state `+0x568`, then
secondary-child state `+0x9B0`. Child query `0045FA50` accepts the child only
when its own `+0x368` state is zero. Axe update `0045FAD0` dispatches the
primary child only while `+0x1FC` is enabled and always dispatches the
secondary child.

Ordinary Axe selection through `0044C2A0` calls Axe vtable `+0x3C` (`0045F9B0`),
which activates the Axe and children and refreshes follower state. It then
calls Player action status 5 for the inventory HUD. This path does not call
status 3 or touch Player `+0xA94`. Separately, `004571A0` status 3 enables
Player `+0xA94` and refreshes its internal frame record from time zero, while
status 4 disables it. `00455BC0` stores the current tick, samples four keyframe
values through `00484940(..., 0.0)`, and sends them through `0047EDF0` to
`0047EA10`; status 3 takes the `0047E6D0` record-copy path. The sampled values
are rectangle/frame fields, not an established reset of the Axe ACT mesh or its
3D pose.

A fresh headless caller census on the pinned executable found one direct
status-3 callsite: `0044BC61` in `0044BC40`. Its only direct caller is
`0043D2B0`, which enters the carry transition only when the selected BModel's
use mode is 1. The transition stores that BModel at Player `+0x3440`, sets its
`+0x25C` use state, calls status 3, and then sets Player state 7. Therefore a
later mode-1 liftable pickup re-enables the Axe subobject and refreshes its
internal frame record after status 4; ordinary Axe re-equipping is not the
reactivation route. Status 4 is the throw-side call at `0045398E`. Other direct
status-helper callsites pass 1, 2, 5, 6, 7, or 9; status 6 targets the indexed
child block at Player `+0x218C`, separate from `+0xA94`. The frame record's
visual relationship to `axe.act`/`axel.act` and the port renderer is unproven;
this remains static caller evidence, not an interactive visibility or
callback-cadence check.

The shared host already mirrors the source-backed inventory operations:
`PlayerLoadout::equip("axe")` sets `primaryChildEnabled`, and the
`AxeSwitchGate` preserves the root/child selection check. Do not infer that
re-equipping clears throw status 4, actor state, or child state `+0x9B0`.

## Evidence

- [`item-selection-pcode-audit.md`](../../../reference/previous-work/decomp/evidence/hose-spray/item-selection-pcode-audit.md)
  records factory, root/child gates, update fields, and the controlled original-
  P-code owner oracle.
- [`0044c2a0_FUN_0044c2a0.c`](../../../reference/previous-work/decomp/evidence/hose-spray/prop-model/detach-path-exact/0044c2a0_FUN_0044c2a0.c)
  records slot switching and activation callbacks.
- [`004571a0_FUN_004571a0.c`](../../../reference/previous-work/decomp/evidence/startup-factories/004571a0_FUN_004571a0.c)
  records Player action status dispatch.
- [`0043d2b0_bmodel_use_event_fresh.c`](0043d2b0_bmodel_use_event_fresh.c)
  records the mode-1 gate into the carry transition and subsequent `use` event.
- The current shared implementation is in
  [`scene_viewer.cpp`](../../../port/spikes/training-native-prototype/src/scene_viewer.cpp)
  and [`hose_item_selection.h`](../../../port/spikes/training-native-prototype/src/hose_item_selection.h).

## Remaining questions

The source route that re-enables Player `+0xA94` after status 4 is now mapped to
the next mode-1 liftable pickup. The host mirrors the enable bit with its Axe
visibility state; the sampled rectangle/frame record has no mapped equivalent
in the Axe ACT renderer, and its visible effect is not established. Runtime
callback cadence also remains unverified. Keep host visibility tied to the
observed status callback and do not add a guessed reset on equipment selection.
