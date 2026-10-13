# Shared rescue entity contact manager route

## Pinned inputs and scope

This is a static trace of `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, joined
to hash-pinned BSP exports. The contact-manager routines were missing Ghidra
function boundaries in the prior project snapshot. Their entries are now
defined, named, typed, saved, and exported in the revised project. No original
game session was captured.

## Factory registration and map instance admission

Startup `0042B4E0` calls `RegisterEntityClassFactories` (`004297E0`). That
registry initializer constructs and appends the `RCS_BModel` descriptor from
`InitializeBModelFactoryDescriptor` (`0043B4B0`) and the
`RCS_BoundsTrigger` descriptor from
`InitializeBoundsTriggerFactoryDescriptor` (`00440D60`). These are the class
names used by Gas R BSP records.

The shared `WorldEntity` loader (`0049E4D0`) enumerates BSP entity records and
submits matching class-factory requests to `ProcessEntitySpawnQueue`
(`0049C670`). On the successful allocation path, that queue registers every
returned entity through `00429490`. BModel construction (`0043F340`) and
BoundsTrigger construction (`00441C60`) both call common base initializer
`0049D420`, which sets the root registration mask to `0xFFEB`.

The copied Gas R scene export identifies entity 240 as `RCS_PlayerRescue`,
entities 265 and 280 as `RCS_BoundsTrigger`, and entity 309 as `RCS_BModel`.
Their constructors set root types 1, 8, and 4 respectively. Because
`0xFFEB` includes the contact manager's `0x20` admission filter, a successfully
created instance of each map record is registered in the appropriate type
list. This closes static instance admission; it does not mean their bounds
currently overlap or their script events have fired.

## Registration and category dispatch

1. The common entity admission routine `00429490` asks an entity's root vtable
   slot `+0x1C` for its registration mask. For the contact manager, owner
   `+0x124` supplies filter `0x20`; matching entities are forwarded through
   the registration interface at owner `+0xE4`, slot `+0x04`.
2. `RegisterContactManagerEntity` (`0049BC90`) chooses the entity's type list
   using root `+0x18`, registers it in that list, and appends a node to the
   manager's active-entity list. The contact manager has nine type lists.
3. `DispatchEntityContactLists` (`0049BD20`) first removes inactive entities
   from the active list. It then visits active entities in list order. For
   every category bit set at the active entity's root `+0x20`, it walks the
   matching type list and calls the active entity's root vtable slot `+0x48`
   for each listed candidate. The manager vtable word `004E4124` points to
   this dispatcher.
4. On the normal owner update path, `0042AEA0` calls the manager collection
   pass at `00429520`, which invokes the four owner managers in order; the
   contact manager is the manager at owner `+0xE0`. This records one static
   dispatch pass and its list traversal order, not a wall-clock or rendered
   frame rate.

The common entity base initializes registration mask `0xFFEB`, which includes
the contact manager's `0x20` filter. The reused [Training scheduling
audit](../../../reference/previous-work/decomp/evidence/training-bmodel-contact-scheduling-audit.md)
traces successful Player admission and distinguishes authored Player
membership from other possible type-1 classes.

## BModel contacts and Gas R `button1`

The BModel root has type 4 and contact category mask `0x2`; its root slot
`+0x48` is `CheckBModelContactCandidate` (`0043B620`). The callback accepts
type-1 candidates with a geometry interface, reads the authored reach, applies
the source-derived conditional 48-unit override, tests the AABBs, and appends
an overlapping candidate pair to BModel root `+0x160`.

Gas R BSP entity 309 (`button1`) authors `E_Reach=20`, `F_HitPoints=2`,
`G_Liftable=0`, and `C_AnimModel0=button1`. The matching `%Model%` entity 92
maps `button1` to BSP model 91. The hash-pinned MOTIONS inventory has 209 model
records and omits model 91, so the engine leaves its motion pointer null and
the BModel's maximum child-motion end remains zero. `CheckBModelReachExpansion`
(`0043AFF0`) only permits the 48-unit override when that endpoint is at or
beyond `+/-1e-28`; entity 309 therefore keeps its authored 20-unit reach. See
the [Gas R model motion inventory](gas_r_model_motions.json) and the full
[button reach derivation](gas_r_button1_use_path.md#child-model-motion-and-effective-reach).

The BModel updater `UpdateBModelEntity` (`0043DC60`) turns contact-pair
changes into ScriptActor contact events, tracks whether a candidate is in
contact, and, for nonzero use mode, adds or removes the BModel interaction
interface at root `+0xA0` in the Player's selection list. The Gas R `use`
binding initializes button1 to generic mode 2. The shared Player scan at
`00459350` queries the registered interface through slot 0 and retains an
eligible query result; action dispatch later reaches its slot `+0x04` and the
`use` event. See the detailed [Gas R button path](gas_r_button1_use_path.md)
and the reused [Player selection audit](../../../reference/previous-work/decomp/evidence/training-liftable-audit/bmodel-selection-pass-audit.md).

This statically joins Gas R button1's successful class-factory path to the
type-4 BModel list and its slot `+0x48` checks over the type-1 list. Runtime
overlap, Player candidate-list membership/order at a particular location, and
input activation have not been captured. The BModel embedded updater's
recurring caller and its order relative to the owner contact pass also remain
unresolved.

## BoundsTrigger contacts and Gas R touch handlers

BoundsTrigger has type 8 and category mask `0x2`; its root slot `+0x48` is
`CheckBoundsTriggerContact` (`00440CE0`). It checks candidate geometry against
the trigger model's bounds and appends overlapping pairs to root `+0x100`.
The embedded ScriptActor at root `+0x24` reaches
`DispatchBoundsTriggerContactEvents` (`00440DC0`), which passes the pair
container to `DispatchScriptActorContactEvents` (`00470840`) and then rotates
and clears it. The shared converter maps new, persistent, and removed pairs
to `touch`, `touching`, and `untouch`, then dispatches each matching event.

Gas R entities 265 (`Explotion1_trigger`) and 280 (`unfog2`) are
`RCS_BoundsTrigger` records whose expanded `touch` handlers include
`explosion1` and `explosion2`. The map data therefore joins both rescue
triggers to the shared BoundsTrigger class path. Successful construction
admits both instances to the type-8 list; their live overlap contacts and
event dispatch on a particular update remain unobserved.

The relative order between `0049BD20` contact production and each embedded
ScriptActor updater is not established. Do not infer same-pass event delivery,
one-frame latency, or a fixed update cadence from these local routes.
Additional BoundsTrigger evidence is in
[training_touch_dispatch_boundary.md](training_touch_dispatch_boundary.md).

## Fresh Ghidra exports

- `004297e0_entity_class_factory_registration_fresh.c` and `.asm`
- `0043b4b0_bmodel_factory_descriptor_fresh.c` and `.asm`
- `00440d60_bounds_trigger_factory_descriptor_fresh.c` and `.asm`
- `0043b620_bmodel_contact_candidate_fresh.c` and `.asm`
- `0049bc90_contact_manager_register_fresh.c` and `.asm`
- `0049bd20_entity_contact_list_dispatch_fresh.c` and `.asm`
- `00440ce0_bounds_trigger_contact_check_fresh.c` and `.asm`
- `00440dc0_bounds_trigger_update_fresh.c` and `.asm`
- `00470840_script_actor_contact_events_fresh.c` and `.asm`
