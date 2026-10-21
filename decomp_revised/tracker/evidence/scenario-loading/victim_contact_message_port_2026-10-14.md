# Victim contact message route and BoundsTrigger event integration

## Retail evidence

The hash-pinned `FDNYFirefighter.exe` exports establish two different gates in
the Player pickup route. VictimRescue callback `0045A070` rejects victim states
5 and 6, verifies candidate entity type 1, asks the candidate vtable slot
`+0x0C` for its world box, checks the authored pickup radius, and submits
message `0x7532` with the victim pointer through the Message Mapper. Player
dispatcher `00453A70` first rejects all messages while Player `+0x3438` is
nonzero; if open, it routes `0x7532` to `00450440`. That handler then checks
Player carry pointer `+0x3534` before accessing the victim payload and starting
the pickup transition. Thus the mapper route must retain both the message gate
and the carry-empty gate, in that order.

The evidence used in this pass is the saved Ghidra decompilation and pinned
raw-copy trace. `list_instances` returned no running Ghidra instance, so no new
interactive database edits or fresh exports were made.

BoundsTrigger contact has a separate producer/update path. `00440CE0` produces
contact pairs independently of model render flags. `00440DC0` drains the pair
list through `00470840` in three event groups: new `touch`, continuing
`touching`, then removed `untouch`. BoundsTrigger setup sets root `+0x164` only
for a trigger whose authored `touch` contains `tell player escape`; the
render pulse in `00440F50` is guarded by that byte, while contact remains
available for all valid triggers.

Final-exit availability does not gate contact production. `UpdateFinalExitAvailability`
(`004504C0`) only calls `00440F50(trigger, true)` after the rescue threshold
passes; `00440F50` changes the visual/model state. Neither that helper nor the
contact producer reads the viewer's `enabledExitTriggers` set equivalent. If
the Player stays inside an exit while availability changes, its pair remains
persistent and the event converter produces `touching`, not a new `touch`.
Authored exits that bind their action to `touch` therefore require leaving and
re-entering to fire it again. This is a static consequence of the retail event
mapping, not a live-play capture. Do not gate contacts by exit availability or
synthesize another touch on enablement.

## Shared port integration

Natural victim overlap now uses the existing `publishVictimProximity` seam to
queue `0x7532`. A local `EntityIdRegistry` provides the Player's route and
generation, and a `FireMessageQueue` record carries a pointer to the stable
`std::map` victim actor. The receiver resolves the route before invoking
`VictimTransactions::receivePickupPayload`, which preserves the `+0x3438` then
`+0x3534` gates. The producer continues to submit overlaps while the Player is
carrying; queued messages reach the receiver and are rejected by its carry
gate as the retail route does. The existing synthetic rescue check now enters
pickup through this mapped message path before it exercises the authored exit
touch.

The preserved same-hash Ghidra exports map owner `+0x3BC`'s nested vtable
`004DF8CC` slot `+4` to `0049B610`, which drains mapped messages and invokes
the routed receiver. `00429520` runs all four manager passes, including the
contact manager at `+0xE0`, before calling that `+0x3BC` drain. Thus a `0x7532`
submitted by `0045A070` during that contact-manager pass is delivered later in
the same `00429520` invocation. This is a verified same-call boundary; the
outer update's rendered-frame cadence and Player movement's order relative to
the owner pass remain unresolved. The host drains after its per-frame
candidate scan to match that local producer-before-drain order, but the mapping
to host frames remains provisional. The host supplies Player movement origin
as the origin element of the `+0x0C` record and its source-backed mins/maxs.
PlayerRescue initialization `0044D550` sets mins `(-30,-30,-30)` and maxs
`(30,85,30)`; the `CROUCH` handler writes max Y 25 at `+0x88`, and successful
uncrouch restores 85. The getter writes only origin XYZ at `+0x68..+0x70`,
leaving the adjacent padding and six bound fields for this shared callback
record. This statically supports the host `VictimPickupBox` shape. Contact
cadence and live pickup timing remain unverified.

The queue-service trace is preserved in the [owner deferred-service audit](../../../reference/previous-work/decomp/evidence/item-pose/live-extinguisher/message-components/owner-deferred-service-trace.md), the [0049B610 drain export](../../../reference/previous-work/decomp/evidence/item-pose/live-extinguisher/message-components/0049b610_FUN_0049b610.c), and the [00429520 owner pass](../../../reference/previous-work/decomp/evidence/item-pose/live-extinguisher/message-components/app-callback-registration/00429520_FUN_00429520.c). Those exports are from the same executable hash recorded above; the preserved [owner-call inventory](../../../reference/previous-work/decomp/evidence/item-pose/live-extinguisher/message-components/owner-vtable-indirect-call-inventory.md) records SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` and read-only Ghidra provenance.

The shared BoundsTrigger scan now groups contact events in retail's
`touch`/`touching`/`untouch` order. It still samples overlap once per host
frame, because the caller and cadence of the retail embedded updater remain
unresolved. Trigger render pulses now require `carryModeGate` in addition to
carry/final-exit state, matching the retail `+0x164` guard. The port keeps
contact detection separate from rendering.

## Packaged checks

The rescue-all app was rebuilt from the shared viewer source. Its packaged
binary SHA-256 is
`d17694cd49e68f929d537b05dc83e0fdff5a53076d42c413ae5b7ec5ca64e4c8`.
The packaged `--rescue-check` passed on Training and all seven campaign rescue
maps; that diagnostic now publishes the first victim's overlap through the
mapped queue, then dispatches the authored exit touch. The mission catalog
check loaded all eight retail rescue choices. Training render, forced-visible
versus hidden trigger, and extinguisher orientation checks also passed. These
checks cover the port's construction and synthetic transaction seams; they do
not establish live contact cadence or original-game visual/gameplay parity.

## Remaining work

- Compare the verified owner-call message drain against the Player movement
  update and rendered-frame cadence; only the local same-call ordering is
  closed.
- Compare natural pickup positions across all seven rescue missions, Training,
  and later rescue stages. The PlayerRescue box layout and standing/crouched
  extents are now mapped statically; callback cadence and live pickup timing
  remain open.
- Compare rendered trigger visibility and multi-trigger event ordering against
  an original-game capture.
- Extend the transaction through release animation, rescue statistics, exit
  activation, sounds, and mission change for every rescue package. Pickup's
  world-actor suppression is verified; no Player-relative carried-body pose is
  source-backed, and the `RC_VictPick` status key's visible meaning remains
  open.

## Source exports

- [`0045A070` VictimRescue overlap callback](0045a070_check_victim_pickup_overlap_fresh.c)
- [`00453A70` Player message dispatcher](../fire-spread/00453A70_fresh.c)
- [`00450440` Player pickup handler](00450440_handle_victim_pickup_message_fresh.c)
- [Message Mapper allocation](0049b4c0_message_mapping_add_fresh.c)
- [Message Mapper wrapper](0049d4e0_send_mapped_entity_message_fresh.c)
- [BoundsTrigger contact and event trace](training_touch_dispatch_boundary.md)
- [BoundsTrigger render gate](../runtime/trigger_brush_render_visibility.md)
