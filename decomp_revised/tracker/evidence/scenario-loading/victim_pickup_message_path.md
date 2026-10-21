# Shared VictimRescue pickup path

Status: **static overlap-to-carry path mapped and routed through the shared `0x7532` message boundary; same-owner-call delivery order is verified, while rendered-frame cadence, Player update order, and live parity remain open.**

Evidence is pinned to `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The seven campaign rescue maps and Training remain in scope. The actor inventory covers all 17 BSPs, including the four linked Oil Rig stages and the out-of-catalog Boat B/Subway B assets. The older `victim-ai-audit/training-pickup-flow.md` is useful source-backed work: its `0x0045A070` pseudocode was checked against the same hash and re-imported as a fresh function boundary here, and its Player/pickup analysis has been selectively carried forward rather than treating the old Training prototype as the port target.

## Shared contact-to-carry route

The VictimRescue root vtable at `0x004E25B8` points to `CheckVictimPickupOverlap` (`0x0045A070`) in slot `+0x48` (`0x004E2600`). The clean Ghidra import had not defined this code as a function; the old work had three exports at the address. Fresh Ghidra now defines and decompiles the 284-byte body. It returns when victim state at `+0x120` is 5 or 6, requires the candidate entity's field `+0x18` to equal 1, then calls that candidate's vtable slot `+0x0C` for a pointer to its world origin and hull extents. For the type-1 PlayerRescue candidate, slot `+0x0C` is `0049DB90 -> 0049D790`; the getter returns a record beginning at root `+0x68` with origin XYZ at indices `[0..2]`, padding at `[3]`, mins at `[4..6]` (`root +0x78..+0x80`), and maxs at `[7..9]` (`root +0x84..+0x8c`). `0044D550` initializes those bounds to `(-30,-30,-30)` and `(30,85,30)`. The `CROUCH` handler stores 25 at root `+0x88`, and successful `0044BCE0` uncrouch restores 85. The host `VictimPickupBox` uses the same origin and extents, including the crouched top height. The callback expands the victim location at `+0x4C..+0x54` by the configured proximity value (using `60.0` when the float-validation helper returns below 1) and performs inclusive overlap comparisons on all three axes. A passing overlap submits message `0x7532` with the victim pointer through the Message Mapper.

The Message Mapper wrapper at `0x0049D4E0` forwards the recipient ID pair, message ID, and payload to the global mapper at `0x0049B4C0`. Fresh `DispatchEntityContactLists` (`0x0049BD20`) walks active roots in contact-manager order. For each category bit at root `+0x20`, it traverses that entire typed candidate list and calls the active root's slot `+0x48`; the dispatcher does not perform a spatial overlap test before the callback. Thus the host's per-victim `publishVictimProximity` scan is pair-complete for the ordinary authored rescue maps, which contain one PlayerRescue and active VictimRescue roots, and the callback itself applies the overlap predicate. The host does not model contact-manager membership/category gates, extra type-1 candidates, or exact active-list order. The map export order is not proven to equal retail entity-set enumeration order, so simultaneous-overlap ordering is still open.

Preserved same-hash Ghidra exports map owner `+0x3BC` nested vtable `004DF8CC` slot `+4` to message drain `0049B610`. `00429520` invokes all four manager callbacks (including contact manager `+0xE0`) before this drain. Therefore, when `0045A070` submits `0x7532` from that contact-manager pass, the receiver is called later in the same `00429520` invocation. This resolves the same-call boundary, but not outer rendered-frame cadence or Player update ordering.

Player message handler `DispatchPlayerMessage` (`0x00453A70`) first checks that Player `+0x3438` is zero before it routes any supported message, including `0x7532`, to `HandleVictimPickupMessage` (`0x00450440`). The pickup handler then accepts the message only when its carry pointer at `+0x3534` is empty, stores the victim pointer, and calls `BeginVictimPickup` (`0x0045BEF0`). It also performs the carry effect, updates Player subobject/status state, rebuilds zone membership, and records the current clock at `+0x5C4`. These are separate outer message and inner carry gates; the port preserves that order.

`BeginVictimPickup` writes victim state 5 at `+0x120`, clears the actor flags word through `0x00481140`, and dispatches event key `pickup` on the embedded ScriptActor at victim `+0xD4`. The common ScriptActor event dispatcher (`0x004705B0`) looks up registered event commands and dispatches them through the shared command router. This event call is not Training-only; authored event contents differ by map.

The natural viewer overlap path now publishes `0x7532` through a generation-
checked route and message queue before it reaches the Player receiver. Its host
drain follows the candidate scan in the same host frame, matching the
source-backed producer-before-drain order; the relation between a host frame,
the retail owner invocation, and Player update cadence remains provisional.
The integrated contact/message path,
including BoundsTrigger event ordering and the separate trigger render gate, is
recorded in [`victim_contact_message_port_2026-10-14.md`](victim_contact_message_port_2026-10-14.md).

## Training's authored pickup events

Training victim entities 191 and 194 author `pickup:` event records. They conditionally invoke `Cmd3b` or `Cmd3c` based on `gotvictim`, then set `gotvictim` to 1. `training.evs` defines those hints as exit directions. This is one map-specific use of the shared pickup event. It supplements the seven campaign rescue maps and does not replace their rescue, fire, trigger, and map-specific mechanics.

The earlier work also maps escape from the Training exit through Player handler `0x00451870` to `0x00451160`, which updates rescue statistics and releases the carried victim through `0x0045C000`. Its exact animation/timer schedule remains blocked on runtime callback cadence. See the archived [full Training pickup/rescue trace](../../../reference/previous-work/decomp/evidence/victim-ai-audit/training-pickup-flow.md) for its bounds, release-frame, actor-flag, passout, and native-component analysis; those native components are not being treated as a complete playable port.

## Limits

- The candidate entity type value `1` is preserved as an observed gate; the static Player message route supports the pickup interpretation, but a live original-game capture has not been made.
- The callback and message code are statically mapped; delivery within the same owner invocation is verified when the producer runs in the contact-manager pass. Rendered-frame cadence and Player update order remain unresolved.
- Training's `pickup:` script syntax is an authored event case. The 17-BSP inventory and all-mission rescue coverage remain the broader port scope.
- Pickup sets the victim's Genesis actor flags to zero, which suppresses its normal world-render submission; state 5 also skips the victim child updater. This render behavior is implemented through the existing flag filter. Status action 1 now maps the resource-55 screen cue and requests the `RC_VictPick` chatter through the shared audio adapter; successful release requests `RC_VictDrop` when survivors remain. The entry-order assumption, audible playback/mixer behavior, status-message text, and any Player-relative carried-body transform remain unresolved. Natural rescue timing and original-versus-port parity are not inferred from these static paths.

## Plain-language explanation

The pickup message is delivered to the Player only when its message gate is open and it is not already carrying someone. Accepting it marks the victim as carried, runs the map-authored pickup event, and updates Player status; resource 55 now supplies the source-traced on-screen pickup cue, and the shared port requests the source-mapped pickup and release chatter. Retail audible playback and mixer behavior remain unverified. The remaining-survivor status text and the carried person's Player-relative body pose remain unresolved.

The resource 55 sprite and its center-to-corner keyframe animation are documented
in [player_victim_pickup_status_hud_2026-10-15.md](player_victim_pickup_status_hud_2026-10-15.md).
The chatter requests and their resource mappings are documented in
[`victim_pickup_chatter_port_2026-10-21.md`](victim_pickup_chatter_port_2026-10-21.md).

## Fresh pseudocode exports

- [Victim overlap callback](0045a070_check_victim_pickup_overlap_fresh.c)
- [Player pickup-message handler](00450440_handle_victim_pickup_message_fresh.c)
- [Victim pickup transition and event](0045bef0_begin_victim_pickup_fresh.c)
- [Message Mapper record allocator](0049b4c0_message_mapping_add_fresh.c)
- [Global Message Mapper wrapper](0049d4e0_send_mapped_entity_message_fresh.c)
- [Player message dispatcher](../fire-spread/00453A70_fresh.c)
- [ScriptActor pickup-event dispatcher](004705b0_script_actor_event_dispatch_fresh.c)
