# Shared VictimRescue pickup path

Status: **static overlap-to-carry path mapped; event dispatch is shared, while callback cadence and runtime parity remain open.**

Evidence is pinned to `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The seven campaign rescue maps and Training remain in scope. The actor inventory covers all 17 BSPs, including the four linked Oil Rig stages and the out-of-catalog Boat B/Subway B assets. The older `victim-ai-audit/training-pickup-flow.md` is useful source-backed work: its `0x0045A070` pseudocode was checked against the same hash and re-imported as a fresh function boundary here, and its Player/pickup analysis has been selectively carried forward rather than treating the old Training prototype as the port target.

## Shared contact-to-carry route

The VictimRescue root vtable at `0x004E25B8` points to `CheckVictimPickupOverlap` (`0x0045A070`) in slot `+0x48` (`0x004E2600`). The clean Ghidra import had not defined this code as a function; the old work had three exports at the address. Fresh Ghidra now defines and decompiles the 284-byte body. It returns when victim state at `+0x120` is 5 or 6, requires the candidate entity's field `+0x18` to equal 1, then calls that candidate's vtable slot `+0x0C` for a bounds pointer. It expands the victim location at `+0x4C..+0x54` by the configured proximity value (using `60.0` when the float-validation helper returns below 1) and performs inclusive overlap comparisons on all three axes. A passing overlap submits message `0x7532` with the victim pointer through the Message Mapper.

The Message Mapper wrapper at `0x0049D4E0` forwards the recipient ID pair, message ID, and payload to the global mapper at `0x0049B4C0`. Fresh `DispatchEntityContactLists` (`0x0049BD20`) walks its active entities and category contact lists, calling matching objects' root-vtable slot `+0x48`. The owner update (`0x00429520`) invokes manager passes. This maps the static callback route, not rendered-frame cadence or delivery latency.

Player message handler `DispatchPlayerMessage` (`0x00453A70`) routes `0x7532` to `HandleVictimPickupMessage` (`0x00450440`). The Player handler accepts the message only when its carry pointer at `+0x3534` is empty, stores the victim pointer, then calls `BeginVictimPickup` (`0x0045BEF0`). It also performs the carry effect, updates Player subobject/status state, rebuilds zone membership, and records the current clock at `+0x5C4`.

`BeginVictimPickup` writes victim state 5 at `+0x120`, clears the actor flags word through `0x00481140`, and dispatches event key `pickup` on the embedded ScriptActor at victim `+0xD4`. The common ScriptActor event dispatcher (`0x004705B0`) looks up registered event commands and dispatches them through the shared command router. This event call is not Training-only; authored event contents differ by map.

## Training's authored pickup events

Training victim entities 191 and 194 author `pickup:` event records. They conditionally invoke `Cmd3b` or `Cmd3c` based on `gotvictim`, then set `gotvictim` to 1. `training.evs` defines those hints as exit directions. This is one map-specific use of the shared pickup event. It supplements the seven campaign rescue maps and does not replace their rescue, fire, trigger, and map-specific mechanics.

The earlier work also maps escape from the Training exit through Player handler `0x00451870` to `0x00451160`, which updates rescue statistics and releases the carried victim through `0x0045C000`. Its exact animation/timer schedule remains blocked on runtime callback cadence. See the archived [full Training pickup/rescue trace](../../../reference/previous-work/decomp/evidence/victim-ai-audit/training-pickup-flow.md) for its bounds, release-frame, actor-flag, passout, and native-component analysis; those native components are not being treated as a complete playable port.

## Limits

- The candidate entity type value `1` is preserved as an observed gate; the static Player message route supports the pickup interpretation, but a live original-game capture has not been made.
- The callback and message code are statically mapped; exact callback cadence and message delivery frame remain unresolved.
- Training's `pickup:` script syntax is an authored event case. The 17-BSP inventory and all-mission rescue coverage remain the broader port scope.
- No visible carry animation, audio, rescue timing, or original-versus-port parity is inferred from these static paths.

## Fresh pseudocode exports

- [Victim overlap callback](0045a070_check_victim_pickup_overlap_fresh.c)
- [Player pickup-message handler](00450440_handle_victim_pickup_message_fresh.c)
- [Victim pickup transition and event](0045bef0_begin_victim_pickup_fresh.c)
- [Message Mapper record allocator](0049b4c0_message_mapping_add_fresh.c)
- [Global Message Mapper wrapper](0049d4e0_send_mapped_entity_message_fresh.c)
- [Player message dispatcher](../fire-spread/00453A70_fresh.c)
- [ScriptActor pickup-event dispatcher](004705b0_script_actor_event_dispatch_fresh.c)
