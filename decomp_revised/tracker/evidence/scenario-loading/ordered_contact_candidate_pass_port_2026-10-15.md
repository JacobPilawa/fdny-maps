# Source-ordered contact candidate pass

## Plain-language explanation

Retail appends contact actors to its active list in entity-class registration
order. The shared viewer used to evaluate Player exposure to fires before it
collected contact candidates for BModels, BoundsTriggers, and PropModels. It
now collects those three candidate groups first, then runs the FireRescue
spread and exposure callback, then reaches VictimRescue pickup candidates.
This aligns the represented cross-class candidate order with the mapped retail
manager order. The actor scripts that respond to contact still run after the
candidate pass because their retail update timing has not been recovered.

## Retail evidence

The pinned `FDNYFirefighter.exe` SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
`RegisterEntityClassFactories` (`004297E0`) registers `RCS_BModel`,
`RCS_BoundsTrigger`, and `RCS_PropModel` at positions 1–3, followed by
`RCS_FireRescue` at position 11 and `RCS_VictimRescue` at position 12.
`WorldEntity` enumerates those descriptors, the spawn queue consumes their
requests FIFO, contact registration appends roots to its active list, and
`DispatchEntityContactLists` (`0049BD20`) visits that list in order. The
hash-pinned route is summarized in
[`contact_manager_factory_order_audit_2026-10-15.md`](contact_manager_factory_order_audit_2026-10-15.md).

`00429520` finishes contact-manager callbacks before it processes the entity
spawn queue and drains mapped messages. The port continues to defer generated
fire children and queued FireRescue/Player and VictimRescue/Player messages to
that later boundary; see
[`fire_contact_pass_order_port_2026-10-15.md`](../fire-spread/fire_contact_pass_order_port_2026-10-15.md).

## Port change

In `scene_viewer.cpp`, BModel, BoundsTrigger, and PropModel contact state is
collected before `runFireContactPass()`. The BModel, BoundsTrigger, and
PropModel script events are then dispatched from the staged contact results.
Victim pickup candidate publication remains later in the frame, before the
existing mapped-message drain.

This changes the order of candidate evaluation, not the established retail
frame schedule. Player update alignment, dynamic/reused contact registration,
within-class entity order, and class-specific ScriptActor drain cadence remain
unverified. The checks below are controlled package diagnostics, not a live
retail timing comparison.
