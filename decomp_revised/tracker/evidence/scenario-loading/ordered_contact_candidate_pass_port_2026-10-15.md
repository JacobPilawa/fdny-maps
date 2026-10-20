# Source-ordered contact candidate pass

## Plain-language explanation

Retail appends contact actors to its active list in entity-class registration
order. The shared viewer used to evaluate Player exposure to fires before it
collected contact candidates for BModels, BoundsTriggers, and PropModels. It
now collects those three candidate groups first, then runs the FireRescue
spread and exposure callback, then reaches VictimRescue pickup candidates.
This aligns the represented cross-class candidate order with the mapped retail
manager order. Static owner scheduling now shows the `+0x50` ScriptActor Think
manager before the `+0xE0` contact manager. The shared host drains the preceding
iteration's BModel, BoundsTrigger, and PropModel pairs before collecting the
current iteration's pair deltas, then holds those deltas for the next
iteration. This follows the recovered phase boundary while retaining a
provisional host interval.

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

In `scene_viewer.cpp`, pending BModel, BoundsTrigger, and PropModel ScriptActor
events drain before current contact-pair sampling and `runFireContactPass()`.
Current pair deltas are stored for the next paced host iteration. BoundsTrigger
and PropModel events keep the shared `touch`, `touching`, `untouch` pass order.
Victim pickup candidate publication remains after FireRescue contact handling
and before the existing mapped-message drain.

The owner pass order and next-owner-pass phase delay are static retail results.
The host uses one paced update iteration for that phase and has not established
retail wall-clock cadence, Player alignment, dynamic/reused contact
registration, within-class entity order, or live contact parity. The checks
below are controlled package diagnostics, not a live retail timing comparison.
