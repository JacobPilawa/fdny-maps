# Fire contact pass and deferred child registration

## Pinned retail path

Retail executable SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

`0042AEA0` invokes the owner update at `00429520`. That pass visits the four
registered managers; the contact manager at owner `+0xE0` dispatches
FireRescue's `+0x48` callback through `0049BD20`. Its callback reaches
`004483A0`, which updates proximity and may attempt spread through `00448320`
before it submits Player message `0x7531`.

After the manager callbacks, `00429520` processes the entity-spawn queue via
owner `+0x3C0`, then drains mapped messages via owner `+0x3BC`. A FireRescue
child requested during the contact callback is therefore created after the
contact traversal. It can receive contact on a later owner pass, not the pass
that created it. The saved ordering is in
[`00429520_world_owner_pass.c`](../runtime-loop/00429520_world_owner_pass.c),
[`0042aea0_game_update.c`](../runtime-loop/0042aea0_game_update.c), and
[`rescue_contact_manager_route.md`](../scenario-loading/rescue_contact_manager_route.md).

## Plain-language explanation

The game checks existing fires against the Player, queues any new fire created
by spread, and only then adds those new fires to the world. The new fires do not
damage or spread against the Player during the check that created them; they
first take part in the next contact-manager pass.

## Port change

The viewer snapshots the active fire count, then visits those FireRescue
candidates in vector order. Each iteration evaluates that fire's spread
proximity/attempt and its Player exposure callback before continuing to the
next fire. It stores each qualifying `(FireRescue entity, influence)` as a
pending `0x7531` message. After all represented contact candidates, it drains
generated spread children and dispatches queued messages. Victim overlap
submits `0x7532` before this boundary; the host then drains FireRescue messages
followed by victim messages. This preserves the source owner boundary of
contact callbacks, entity creation, and mapped messages. Children first take
part in Player fire contact on the next host pass. Messages stay in FireRescue
candidate order; after one message enters Player state 6, later queued messages
reach the handler and are ignored by its native state gate. The FireRescue
callback continues to collect qualifying messages while the Player is
incapacitated; state 6 is handled by the receiver, not the producer.

The source uses two related thresholds: its nearby-fire population counter
includes a truncated distance of exactly 750, while `ProcessFireRescueSpread`
attempts a roll only for a distance strictly below 750. Retail stores each
named FireRescue's last distance at root `+0x120` and changes shared
`DAT_00502B90` on transitions across the inclusive `<=750` population gate.
The host now preserves that per-fire previous distance and updates one shared
count in candidate order before each spread attempt. Mode 5 skips the callback,
so an already-counted fire remains in the population while it stays resident;
its host entry is not removed before the scene reload. See
[`proximity_count_port_2026-10-15.md`](proximity_count_port_2026-10-15.md).

## Verification boundary

The manager, spawn-queue, and message-drain ordering is verified from the
hash-pinned EXE. The port groups spread and exposure work in one per-fire
candidate pass and defers child creation and Player effects until after its
other represented candidate scans. The factory audit shows that the host
currently runs FireRescue candidate evaluation before BModel, BoundsTrigger,
and PropModel scans, even though retail registers those classes first; see
[`contact_manager_factory_order_audit_2026-10-15.md`](../scenario-loading/contact_manager_factory_order_audit_2026-10-15.md).
Class-specific ScriptActor drain scheduling, the single Player position,
visibility, and host-frame cadence remain provisional. Actor destruction and
reuse are not modeled; the persistent counter is scoped to the viewer's stable
fire vector and reset when it loads a scene. Dynamic retail candidate
membership and exact per-class list order remain unverified.
Owner-update cadence relative to rendered frames, FireRescue list order in a
live session, Genesis visibility results, Player candidate frame alignment,
and cross-system random ordering remain unverified. This update does not claim
matching natural fire damage timing or full retail parity.
