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
status 3 or touch Player `+0xA94`. Separately, `004571A0` status 3 enables and
resets Player `+0xA94`, while status 4 disables it. The preserved throw/carry
route calls status 3, but no source-backed link connects ordinary Axe selection
to that status callback.

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
- The current shared implementation is in
  [`scene_viewer.cpp`](../../../port/spikes/training-native-prototype/src/scene_viewer.cpp)
  and [`hose_item_selection.h`](../../../port/spikes/training-native-prototype/src/hose_item_selection.h).

## Remaining question

Trace the runtime/manager route that invokes Player status callbacks and
determine when Axe visibility becomes active after a throw. Until a source link
is found, keep host visibility tied to the observed status callback and do not
add a guessed reset on equipment selection.
