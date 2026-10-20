# Deferred FireRescue child lifecycle boundary

## Pinned executable and scope

Read-only follow-up against `inputs/original/binaries/FDNYFirefighter.exe`,
SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. This
joins the route-child request to the generic world spawn drain, FireRescue
factory, route-handle manager, and known update/destruction paths. It does not
capture a live retail session.

## Request, factory, and generic registration

`AdvanceFireRescuePathMotion` (`00449340`) constructs an `RCS_FireRescue`
UserData-shaped request and submits it through the deferred factory queue. The
generic drain `0049C670` removes each request, calls the registered class
factory through descriptor slot `+0x08`, registers the returned root through
`00429490`, and only then runs root vtable slots `+0x20` and `+0x24` in two
batch passes. The generic queue implementation is preserved in
[`0049c670_entity_spawn_queue_fresh.c`](../scenario-loading/0049c670_entity_spawn_queue_fresh.c).

For `RCS_FireRescue`, the class descriptor is installed through
`RegisterEntityClassFactories` (`004297E0`) and its FireRescue registration
initializer (`00448070`). Descriptor slot `+0x08` points to factory
`0044A470`. It scans a retained object array at descriptor `+0x24` with count
at `+0x28`. A reusable entry is selected through its byte at root `+0x1C`;
the factory calls that object's virtual slot `+0x28` with the new request. If
no reusable entry is selected, it allocates `0x1CC` bytes and constructs with
`0044A330`. The factory retains fresh roots in that array, sets root byte
`+0x1D`, calls `0049D450`, and returns the root to the generic drain.

The common registration in `00429490` occurs before the two root initialization
passes. FireRescue primary vtable `004E1780` maps slot `+0x20` to
`InitializeFireRescueEntity` (`00448D60`) and slot `+0x24` to `0043F660`, the
shared base no-op. Thus the second queue initialization pass is not evidence
that this child receives a special update callback.

## Identity is separate from world/update membership

`0049D450` reads the entity-handle manager at `DAT_00503A78+0x10`, asks
`0049B810` for a two-word handle/generation pair, and stores it at root
`+0x04/+0x08`. Matching cleanup `0049D510` calls `0049B780` with that pair.
This route is not the generic world-list registration at `00429490`, and the
saved evidence does not connect the handle manager to recurring FireRescue
Think dispatch. The retained legacy audit records the handle/update boundary in
[`training-think-registration-audit.md`](../../../reference/previous-work/decomp/evidence/fire-update/training-think-registration-audit.md).

The FireRescue root's primary slot `+0x14` is thunk `0043AFE0`, which reaches
shared updater `00470740` and can invoke the visual Think path. The recurring
caller for this root slot, its list ownership, and its cadence remain unknown;
the bounded follow-up is in
[`root-think-dispatch-query-20261012.md`](../../../reference/previous-work/decomp/evidence/fire-update/root-think-dispatch-query-20261012.md).

## Reuse, removal, and teardown limits

`0044A470` writes byte `+0x1D` to 1 after factory selection. The inspected
factory body does not show a reset of this byte. It calls the reused object's
slot `+0x28`, but the full reinitialization effects and how `+0x1C` changes
during a live lifetime are not established here. The destructor route
`0044A450 -> 0044A290 -> 0049D510` releases identity and visual subobjects; the
inspected destructor slice does not clear pool byte `+0x1D`.

The descriptor teardown method `00448110` walks its retained object array,
calls each entry's slot 0 with argument 1, then clears the array. That is
factory-descriptor teardown, not a proven per-entity deactivation path. The
review did not identify a source-backed path that removes a child from the
world manager, resets `+0x1D`, or establishes when it can be reused. The
earlier legacy spread audit's normal Think cleanup applies a separate
name-gated path and does not close these ownership questions; see
[`MECHANICS.md`](../../../reference/previous-work/fire_spread/MECHANICS.md).

## Port boundary

The host may preserve the queued request data and generic create/register/init
phase as separate operations. It should not yet present route children as
ordinary persistent fires or claim their update, retirement, or reuse parity.
The request contains generated size keys, a route direction, a stagger mode,
and source values that feed its timing track; the child visual and recurring
callback path still depend on the unresolved lifecycle above. Source route
progress and its `H_Radius/2` child-emission gate are documented separately in
[`README.md`](README.md).
