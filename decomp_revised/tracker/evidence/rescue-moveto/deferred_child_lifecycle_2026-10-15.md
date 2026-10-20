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

## Normal update-manager enrollment

The common registration route applies category mask `0xFFEB`, which includes
the FireRescue manager filter 2. On the normal owner pass, `00429520` visits
owner field `+0x50` using generic manager slot 0 `0049C0E0`; its slot +4 adapter
`0049C3D0` calls member root vtable slot `+0x14`. For FireRescue that slot
reaches `00449FE0` through `0043AFE0` and `00470740`. The `+0x50` manager pass
precedes contact manager `+0xE0`. The saved trace is in
[`fire_update_dispatch_order_2026-10-20.md`](../fire-spread/fire_update_dispatch_order_2026-10-20.md).

The generic staged-entity drain is owner service `+0x3C0`, after the manager
passes. A child request emitted during the current `+0x50` pass is therefore
not eligible for that already-running pass; once drained and registered it is
eligible for the next applicable owner `+0x50` pass. This establishes static
manager enrollment and next-pass eligibility, but not a wall-clock interval.
The child request's location is copied from the parent visual subobject's
pre-update base `+0x28..+0x30`; the route helper later writes its current
candidate back to that embedded visual. It is not copied from the PathPoint.
See [`00449340_visual_position.asm`](00449340_visual_position.asm) and
[`fire_rescue_timed_vector_size_bounds_2026-10-20.md`](fire_rescue_timed_vector_size_bounds_2026-10-20.md).

## Identity is separate from world/update membership

`0049D450` reads the entity-handle manager at `DAT_00503A78+0x10`, asks
`0049B810` for a two-word handle/generation pair, and stores it at root
`+0x04/+0x08`. Matching cleanup `0049D510` calls `0049B780` with that pair.
This route is separate from the generic world-list registration at `00429490`;
it assigns identity, while the `+0x50` manager route above establishes recurring
Think eligibility. The retained legacy audit records the handle/update boundary in
[`training-think-registration-audit.md`](../../../reference/previous-work/decomp/evidence/fire-update/training-think-registration-audit.md).

The FireRescue root's primary slot `+0x14` is thunk `0043AFE0`, which reaches
shared updater `00470740` and invokes the visual Think path from the owner's
`+0x50` manager. Its exact cadence remains unknown; the bounded follow-up is in
[`root-think-dispatch-query-20261012.md`](../../../reference/previous-work/decomp/evidence/fire-update/root-think-dispatch-query-20261012.md).

## Reuse, removal, and teardown limits

`0044A470` writes byte `+0x1D` to 1 after factory selection. The inspected
factory body does not show a reset of this byte. **2026-10-20 correction:**
the reused object's slot `+0x28` resolves to `004475F0`. That method calls
`0049D310` to copy request `+0x0C` into root `+0x0C` and clear root bytes
`+0x10`, `+0x1C`, and `+0x1D`, then calls `0049D160(request, root+0x78, 0x88)`
to reinitialize the FireRescue UserData region. After factory selection,
`0044A470` sets `+0x1D` and `0049D450` assigns a fresh identity handle. Generic
queue registration then reruns `00448D60`; this visual initializer resets the
timed size track, dimensions, origin and transient visual state before the
child enters normal manager updates. Therefore a reused entry does not simply
retain its old UserData or visual initialization state.

The eligibility check still reads root byte `+0x1C`, but the complete set of
runtime writers and its meaning across a live lifetime are not established.
The destructor route `0044A450 -> 0044A290 -> 0049D510` releases identity and
visual subobjects; the inspected destructor slice does not clear pool byte
`+0x1D`.

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
phase as separate operations. The current host instantiates fresh child
objects, while retail may select and reinitialize retained roots. It should
not yet claim their update, retirement, or reuse parity. The request contains
generated size keys, a route direction, a stagger mode, and source values that
feed its timing track; reuse eligibility and child retirement remain open.
Source route progress and its `H_Radius/2` child-emission gate are documented separately in
[`README.md`](README.md).
