# Contact-manager factory traversal audit (2026-10-15)

## Pinned retail order

This static audit uses `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
No original-game session was captured.

`RegisterEntityClassFactories` (`004297E0`) appends factory descriptors in a
fixed order. For the contact-producing classes currently represented in the
shared viewer, the relevant descriptor positions are:

| Registry position | Factory descriptor | Root type | Contact callback path |
|---:|---|---:|---|
| 1 | `RCS_BModel` (`0043B4B0`) | 4 | `0043B620` |
| 2 | `RCS_BoundsTrigger` (`00440D60`) | 8 | `00440CE0` |
| 3 | `RCS_PropModel` (`004436A0`) | 0 | `00443700` |
| 9 | `RCS_PlayerRescue` (`0044BFF0`) | 1 | player-root `+0x48` is outside this audit |
| 11 | `RCS_FireRescue` (`00448070`) | 6 | `004483A0` -> `00448320` |
| 12 | `RCS_VictimRescue` (`0045A010`) | 2 | `0045A070` |

The class names at positions 1, 2, 3, 9, 11, and 12 are joined from the
hash-pinned registry initializer and the corresponding descriptor
constructors. The WorldEntity loader (`0049E4D0`) walks that descriptor list,
enumerates each matching `geEntity` set, and appends spawn requests. The queue
(`0049C670`) consumes requests FIFO and registers each created root through
`00429490`. Contact registration (`0049BC90`) appends the root to the active
list, so these static map actors enter the manager grouped by descriptor order.
The per-class entity order is the entity-set enumeration order; equivalence
between that order and the exported JSON order has not been established.

`DispatchEntityContactLists` (`0049BD20`) visits the resulting active list in
order. For each active root category bit it walks the matching typed candidate
list and calls root vtable slot `+0x48`. BModel, BoundsTrigger, PropModel,
FireRescue, and VictimRescue contact paths test type-1 candidates. Their
relative callback grouping is therefore BModel, BoundsTrigger, PropModel,
PlayerRescue's active-list position, FireRescue, then VictimRescue, subject to
any other active roots and category bits in the list.

## Host comparison

The viewer's represented candidate scans are split across the rendered-frame
loop. They now run in the verified relative class order: BModel,
BoundsTrigger, PropModel, FireRescue, then VictimRescue. VictimRescue submits
its `0x7532` pickup messages immediately after the FireRescue pass and before
the separate BModel/BoundsTrigger/PropModel ScriptActor event drains. Those
event drains remain in their existing host paths because their retail caller
and cadence are unresolved. Exact order within each class and whether the
host's rendered-frame sample matches a retail owner pass remain open.

This turn moves FireRescue child creation and queued Player `0x7531` handling
to the host's post-candidate boundary. Victim overlap publishes `0x7532`
before that boundary; generated children are drained first, then FireRescue
messages, then victim messages. This follows the verified owner order in
`00429520`: complete contact-manager callbacks, staged-entity queue, then
mapped-message drain. The order of FireRescue versus VictimRescue message
submissions follows their factory positions. The host does not combine these
IDs in one global Message Mapper queue, so cross-ID enqueue and dispatch
semantics remain provisional.

## Remaining boundary

The class-specific callbacks produce pair deltas; BModel, BoundsTrigger, and
PropModel later drain those pairs through their own ScriptActor update paths.
The recurring caller and cadence of those drains, and their order relative to
the manager pass, remain unresolved. The host currently dispatches some
contact events directly in its per-class scans. Moving those scans or their
event effects into factory order could change script side effects before the
FireRescue candidate check, so this audit does not claim that callback-event
timing is fixed. PlayerRescue's active callback behavior, dynamic/reused
registration, exact entity-set ordering, and retail rendered-frame cadence
also remain open.

The represented typed contact candidates now follow the verified factory
order, while class-specific event drains remain separate. Do not merge those
drains into the manager traversal until their external scheduling is
source-backed. See [the manager route](rescue_contact_manager_route.md),
[the FireRescue post-pass port](../fire-spread/fire_contact_pass_order_port_2026-10-15.md),
[the PropModel contact trace](retail_propmodel_contact_trace.md), and
[the victim message route](victim_pickup_message_path.md).

## Evidence inputs

- `004297e0_entity_class_factory_registration_fresh.c` and `.asm`
- `0049e4d0_world_entity_load.c`
- `0049c670_entity_spawn_queue_fresh.c`
- `0049bc90_contact_manager_register_fresh.c` and `.asm`
- `rescue_contact_manager_route.md`
- `retail_propmodel_contact_trace.md`
- `victim_pickup_message_path.md`
- `../fire-spread/README.md`

## Plain-language explanation

Retail checks authored BModels, trigger volumes, props, fires, and victims in
that class order during its contact pass. The shared host now submits victim
pickup messages after fire messages but before its separate script-event
drains; both message types are still delivered after queued fire children are
processed. This aligns the statically traced callback order without claiming
that one host frame matches a retail update tick.
