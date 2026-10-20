# FireRescue Think dispatch and state-4 ordering — 2026-10-20

## Pinned source and conclusion

All executable observations below use `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Read-only Ghidra inspection used a disposable copy of the existing project.
This establishes a static callback path and order during a normal owner update;
it does not establish a wall-clock/rendered-frame period.

On the ordinary `ProcessEverGameUpdate` (`0042AEA0`) path, the game calls
`ProcessEntityFactoryQueues` (`00429520`). That function invokes the four
owner managers in order at offsets `+0x08`, `+0x50`, `+0x98`, and `+0xE0`.
The saved pseudocode is in
[`0042aea0_game_update.c`](../runtime-loop/0042aea0_game_update.c) and
[`00429520_world_owner_pass.c`](../runtime-loop/00429520_world_owner_pass.c).
`0042AEA0` has an earlier guarded return that skips this normal manager pass.

## Root update manager route

The four owner fields point to instances of the generic entity manager. Their
freshly inspected vtables and dispatch adapters are:

| Owner field | Slot 0 / member-list pass | Slot +4 / per-member callback | Manager filter |
| --- | --- | --- | --- |
| `+0x08` | `0049C0E0` | `0049C410` | 1 |
| `+0x50` | `0049C0E0` | `0049C3D0` | 2 |
| `+0x98` | `0049C0E0` | `0049C3E0` | 8 |
| `+0xE0` | `0049BD20` contact-list dispatcher | — (unused) | `0x20` |

The common entity registration mask is `0xFFEB` (`0049D420`), which contains
both filter `2` and contact-manager filter `0x20`. The FireRescue constructor
registers its root through that common route. Thus the owner `+0x50` manager
contains FireRescue roots and its callback adapter is relevant to their
ordinary update.

`0049C0E0` walks that manager's active list, invokes its vtable slot `+4` on
each member, then uses the member root's slot `+4` as its retirement check.
For the `+0x50` manager, adapter `0049C3D0` is the thin call
`member->vtable[+0x14](...)`. The FireRescue root's slot `+0x14` reaches the
actor update path through thunk `0043AFE0` and shared update `00470740` to
`UpdateFireRescueActor` (`00449FE0`). Its contact callback is root slot
`+0x48`, address `004483A0` in vtable `004E1780`; the normal manager order
places contact-manager dispatch at owner `+0xE0`, after the `+0x50` Think
pass. The contact path and manager type-list traversal are described in
[`rescue_contact_manager_route.md`](../scenario-loading/rescue_contact_manager_route.md)
and [`0049bd20_entity_contact_list_dispatch_fresh.c`](../scenario-loading/0049bd20_entity_contact_list_dispatch_fresh.c).

Inside `UpdateFireRescueActor`, state 4 calls
`AdvanceFireRescuePathMotion` (`00449340`) before the embedded visual's
timed-vector update (`0044B060` / `0044AE80`). This detail is preserved in
[`fire_rescue_state4_update.md`](../rescue-moveto/fire_rescue_state4_update.md)
and the byte-level listing
[`00449fe0_firerescue_visual_think.objdump.txt`](../runtime-loop/00449fe0_firerescue_visual_think.objdump.txt).

## Shared host change

`updatePreviewFirePathMotion` now runs as the state-4 callback supplied to
`FireEntity::think`. It therefore runs before `FireEntity::think` performs its
ordinary `visual.sizeUpdate`, and it no longer runs as an unconditional
separate scene pass. The route helper marks changed dimensions and leaves the
single ordinary size update to `FireEntity::think`; this removes the previous
second size update for a moved FireRescue.

This is a source-shaped ordering improvement. The host does not yet reproduce
the retail timed-vector key track, exact update cadence, manager insertion
order, deferred child registration/drain, or interleaving with other random
consumers. Those remain provisional and require further source work or a
controlled retail runtime capture. Static owner-pass order must not be read as
a proven 25/30/60 Hz schedule or full retail parity.
