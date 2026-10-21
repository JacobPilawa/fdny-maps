# Carried victim render boundary

## Provenance

Fresh read-only Ghidra 12.0.4 headless analysis imported
`FDNYFirefighter.exe` into the disposable project
`/private/tmp/fdny_carried_trace/FDNYCarriedTrace`. The input SHA-256 is
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
`00450440`, `0045A070`, `0045BEF0`, `0045BEA0`, `0045C000`, and `00459990`
were decompiled from that import. A separate instruction scan for the
`0x3534` displacement wrote
[`player_carried_victim_pointer_xrefs_2026-10-21.txt`](player_carried_victim_pointer_xrefs_2026-10-21.txt).
No game runtime or retail image capture was used.

## Pickup and ordinary actor rendering

`00450440` accepts the mapped pickup message only when Player `+0x3534` is
empty, stores the victim pointer there, and calls `0045BEF0`. `0045BEF0`
sets VictimRescue state `+0x120` to 5, clears its ActorInfo flags through
`00481140(..., 0)`, then dispatches the authored `pickup` ScriptActor event.
Victim state dispatcher `0045BEA0` returns immediately in state 5, so this
victim receives neither the state-specific update nor the common attached
sound and animation update on that route.

The shared viewer's ordinary actor draw predicate requires ActorInfo flags
`0x1400`; the pickup clear therefore removes the victim from that draw path.
This matches the traced retail visibility change. It does not establish that
the retail game draws the carried body through another Player, equipment, or
HUD path.

## Direct carry-pointer references

The fresh displacement scan found ten instructions. `00450440` is the pickup
gate/store; `00451160` reads the pointer on Player escape and clears it after
release; `0044FF10` reads it for passout drop and clears it; `0044CD10`
clears it during Player reset. One candidate at `0045AE50` uses displacement
`+0x3534` from its local base and is not established here as a Player-pointer
access. No direct `+0x3534` operand appears in an actor render or matrix
writer among the scan results. The scan only matches explicit x86 displacement
operands; it does not rule out an indirect alias or a separate actor/equipment
render route.

`0045C000` computes a release location using Player and victim frame helpers,
sets state 6, enables Actor flag `0x400`, dispatches `rescue`, and starts the
release visibility pulse. That is a one-time release transaction; it does not
provide a continuous carried pose while state 5 is active.

## Port decision and open trace

Keep the source-backed state-5 visibility suppression. There is no supported
Player-relative transform to attach to the viewer camera or Player position,
so adding one would invent a gameplay pose. The exact retail appearance while
carrying remains open until a separate Player/equipment render route is
identified or a retail runtime capture is available. Natural pickup,
travel, and rescue progression also still lack end-to-end retail parity
validation; this static boundary does not close those campaign gaps.

Related evidence: [pickup message path](victim_pickup_message_path.md),
[fresh victim state dispatcher](victim_release_lifecycle_and_motion_context_fresh_2026-10-21.c),
and [camera pose ancestry](../genesis-boundary/actor_camera_pose_trace_2026-10-17.md).
