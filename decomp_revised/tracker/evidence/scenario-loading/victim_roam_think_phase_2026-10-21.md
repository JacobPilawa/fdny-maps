# VictimRescue roam update phase

## Source and route

Retail executable `inputs/original/binaries/FDNYFirefighter.exe` has SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Fresh read-only Ghidra 12.0.4 analysis of the saved `FDNYPort` project and
direct PE/vtable bytes maps the roaming callback:

- `InitializeVictimRescueEntity` (`0045DDD0`) calls `0049D8C0`, which calls the
  common root initializer `0049D420`; that initializer writes eligibility
  mask `0xFFEB` at root `+0x14`. The generic spawn queue passes constructed
  roots to `00429490`, whose filter-2 registration path appends eligible roots
  to owner manager `+0x50`.
- The constructor installs primary vtable `004E25B8`. Its virtual slot
  `+0x14`, table address `004E25CC`, points to `00459990`. The raw instruction
  slice shows that this adapter forwards to the embedded actor updater at root
  `+0xD4` through `00470740`, unless root byte `+0x10` is set.
- The embedded updater vtable at root `+0xD4` is `004E25B0`; its first entry
  is `0045BEA0`. That state dispatcher calls `0045B220` for victim state 7,
  and `004599B0` for state 6. The current-hash decompilation is saved in
  [`0045bea0_victim_actor_update_dispatch_fresh_2026-10-21.c`](0045bea0_victim_actor_update_dispatch_fresh_2026-10-21.c)
  and the forwarding bytes in
  [`00459990_victim_roam_think_adapter_2026-10-21.asm`](00459990_victim_roam_think_adapter_2026-10-21.asm).
- Normal owner pass `00429520` invokes owner Think manager `+0x50` before
  contact manager `+0xE0`; the early-return path in `0042AEA0` can still skip
  the normal manager pass.

## Shared host change

`updateRoamingVictims` previously ran after the loop's contact pass. It now
runs after pending ScriptActor contacts are drained but before the host samples
BModel, BoundsTrigger, PropModel, FireRescue, and VictimRescue contacts. This
places victim movement on the source-supported Think-before-contact side of
the phase boundary. The same host iteration's victim pickup overlap can now
use the moved actor position, as retail's later contact pass can.

This is a coarse phase correction. Exact wall-clock cadence, order within the
owner Think list, early-return behavior in ordinary play, and retail-versus-
host collision results remain unverified. The shared port still advances with
its host frame delta; no timing or runtime-parity claim is made. No retail
runtime was captured.

The macOS rescue-all app rebuilt after the host change. Standalone and packaged
arm64 executables match at SHA-256
`f81d1d2885f53f5249202266b8e1f8bc5e83e633d01d6be7ce2cd508b63c5d9f`;
`Info.plist` validation passed, eight rescue choices were emitted, and the
bundle contains all 17 retail scene packages plus the local custom test-room
package. The linker still warns that json-c 0.19 targets macOS 27.0 while the
build target is 13.3. This is compile/package evidence only; no gameplay test,
interactive run, or retail comparison was performed.

## References

- [Victim roam behavior and numeric gates](victim_roam_behavior.md)
- [Owner Think/contact phase order](root_plus14_owner_scheduler_trace_2026-10-20.md)
- [Victim factory, spawn order, and registration](victim_spawn_factory_and_order.md)
- Fresh Ghidra 12.0.4 read-only logs: `/private/tmp/fdny-victim-root-update-route.log`, `/private/tmp/fdny-victim-think-registration.log`, `/private/tmp/fdny-victim-root-mask.log`
