# VictimRescue owner Think suppression gate

## Provenance

Read-only Ghidra 12.0.4 inspection of the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project (`-readOnly -noanalysis`)
against `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. No
retail runtime capture was made.

## Source-backed default

VictimRescue construction at `0045DDD0` calls common initializer `0049D8C0`,
which calls root initializer `0049D420`. That initializer sets the Think
eligibility mask at root `+0x14` to `0xFFEB` and explicitly writes zero to
root byte `+0x10` (`MOV byte ptr [EAX + 0x10],CL`, with `ECX` zeroed). The
VictimRescue initializer then installs its primary vtable and embedded actor
updater without another direct write to this byte.

The root Think slot adapter `00459990` reads byte `+0x10`. If nonzero, it
returns without forwarding to the embedded updater at root `+0xD4`; if zero,
it forwards through `00470740`. This gate therefore suppresses the victim
state update, attached sound-controller update, and actor-animation update
performed below that adapter. The function slice is saved in
[`00459990_victim_roam_think_adapter_2026-10-21.asm`](00459990_victim_roam_think_adapter_2026-10-21.asm),
and the downstream dispatcher is in
[`0045bea0_victim_actor_update_dispatch_fresh_2026-10-21.c`](0045bea0_victim_actor_update_dispatch_fresh_2026-10-21.c).

## Port impact and limits

`PreviewVictim` has no explicit field for root `+0x10`, and the shared host
currently calls its victim update loop for all loaded victims except pickup
and release states. The source-backed constructor default is zero, matching
that ordinary spawned-victim behavior. A broad register-plus-displacement
scan found 2,031 candidate stores containing `+0x10`; most are unrelated
stack, object, or compiler-generated accesses, so it does not prove that no
generic or indirect writer can set the byte later. The inspected constructor
and authored-command route did not establish such a writer or a reachable
mission transition that sets the byte.

No host change is justified yet: silently skipping or enabling updates without
a source-backed state transition would be a guess. Keep the byte's possible
nonzero behavior as an open static/runtime gap. Ordinary constructor-default
matching is not a retail runtime or full-game parity claim.

## Analysis record

- Read-only headless command used the saved project and `FindOffsetWrites.java`
  with displacement `0x10`; output was written under `/private/tmp` and was not
  added to the mirror.
- Fresh decompilation of `0045DDD0`, `0049D8C0`, and `0049D420` was also written
  under `/private/tmp`; only the hash-pinned conclusions above are retained.
- No host code, build, tests, gameplay run, or runtime parity check changed in
  this audit.
