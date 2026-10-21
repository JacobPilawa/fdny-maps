# PlayerRescue lifetime during linked-stage flush

## Pinned input and method

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

This is a static reconciliation of saved same-hash Ghidra 12.0.4 exports and
the saved `0049C820` instruction window with direct `objdump` disassembly and
the current binary's `.rdata` bytes. No Ghidra instance was running for this
follow-up, and no live retail session was captured. The byte at `+0x1D` is
described by offset because its member name and full lifetime semantics are
not established.

## Flush queue and callback dispatch

`0042A070` drains the four manager lists with `00429EF0`. For each entity not
already present in the owner work queue, that helper invokes primary service
slot `+0x04` with `(entity, 1)`. The owner tail service constructor
`0042B2C0` installs primary vtable `004E4158` and a nested interface at
service `+0x40` with vtable `004DF8E8`; primary slot `+0x04` is
`SetEntityFactoryRequestPending` (`0042B1A0`), and nested slot `+0x04` is the
work-item drain at `0049C820`. The flush calls that nested slot after the list
drains, so queued items are processed during the flush.

The `0049C820` instructions remove and free each queue node, then inspect the
queued object's byte at `+0x1D`. A zero byte dispatches root vtable slot `+0`
with argument `1`; a nonzero byte dispatches root slot `+0x2C`. This is a
conditional cleanup route, not a general state snapshot or restoration call.

## PlayerRescue path

The PlayerRescue construction chain reaches common initializer `0049D420`,
which stores zero at root `+0x1D`. Factory `00454080` calls
`ConstructPlayerRescueEntity` (`00453D50`) and then `0049D450` to assign the
generic identity handle; that factory body does not set `+0x1D` to one. The
current root-vtable bytes at address point `004E1C48` map slot `+0` to
`00454060` and slot `+0x2C` to `0049D530`.

The direct-displacement scan of the pinned executable found the positive
`+0x1D` store in pooled FireRescue factory `0044A470`; `0049D310` and
`0049D420` clear it. No PlayerRescue positive store was found in this scan.
Computed or indirect writes remain outside this check, so the field's complete
runtime lifetime is not proven.

`00454060` calls PlayerRescue cleanup `00453AE0` and frees the allocation when
its argument has bit 0 set. The flush dispatcher supplies `1`, so the
zero-byte PlayerRescue path cleans up and frees that root. `00453AE0` also
clears the global current-Player pointer before releasing its embedded
components. The alternate `0049D530` path releases the generic identity handle
and sets root `+0x1C`; it does not run for PlayerRescue on the observed
constructor/factory path.

As a useful contrast, the pooled FireRescue factory `0044A470` explicitly sets
its root `+0x1D` to one after selection/construction. That pool-specific byte
write explains why FireRescue reaches a different cleanup callback. It does
not establish the complete reuse lifetime of every entity class.

## Port implication and limits

On the known PlayerRescue path, with `+0x1D` unchanged from its base
initialization, the old object is cleaned up and freed during the linked-stage
flush; a later BSP creates and initializes a fresh PlayerRescue. The shared
host already reconstructs per-scene PlayerRescue state when
`runRescueSceneChain` enters the next package, so this finding does not justify
inventing a carry-over of the old entity. It narrows the question to state
stored elsewhere: profile/score selection, mission statistics,
script variables, loadout sources, and other global services may still survive
or repopulate values. Those transfers remain unresolved, as do indirect writes
to the `+0x1D` byte, queue helper side effects, callback timing in a live run,
and stage-transition presentation. The existing host statistics-map carry is
still a provisional choice, not a retail persistence claim.

## Evidence used

- [`level_transition_flush_2026-10-21.md`](level_transition_flush_2026-10-21.md)
- [`0042b2c0_owner_work_service_constructor_fresh_2026-10-20.c`](../runtime-loop/0042b2c0_owner_work_service_constructor_fresh_2026-10-20.c)
- [`0049c820_service_callback_window_2026-10-20.asm`](../runtime-loop/0049c820_service_callback_window_2026-10-20.asm)
- [`00454080_create_player_rescue_entity_fresh.c`](00454080_create_player_rescue_entity_fresh.c)
- [`00453d50_player_rescue_object_constructor_fresh.c`](00453d50_player_rescue_object_constructor_fresh.c)
- [`00429490_root_manager_registration_fresh_2026-10-20.c`](../runtime-loop/00429490_root_manager_registration_fresh_2026-10-20.c)
- [`memory_004e1c40_player_rescue_vtables.json`](memory_004e1c40_player_rescue_vtables.json), checked against the current retail binary bytes
- Direct disassembly of `00454060`, `00453AE0`, `0044A470`, and the factory/initializer path from the pinned binary
