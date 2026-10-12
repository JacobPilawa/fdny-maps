# Victim proximity message producer

`victim_proximity.h` represents one call to victim owner callback `0045A070`; it adds no tick, scheduler, or cadence. It reuses the actual `victimPickupBox` and `sourceVictimPickupAabbOverlap` helpers from `victim_pickup.h`. Caller supplies a recipient-handle getter, a stable victim payload address, the Player world-box getter, a callback returning Training's already-resolved radius 60, and the victim center callback.

The source gate and access order are explicit: reject victim states 5/6; require recipient entity type 1; call that entity's vtable `+0x0C` world-box getter; stop on null; resolve radius through `00459B40`; then read victim center and test inclusive AABB overlap in X, Z, Y order. Only after overlap passes does route lookup `0049D350` supply `(route,generation)`; `0049D4E0` calls mapper `0049B4C0` with message `0x7532` and victim pointer payload. The mapper's append sends flag zero. If all 64 records are occupied, source logs `ROCMessageMapper: No free messages!` and returns without queueing; `queueFull` exposes that outcome through a diagnostic callback.

Ghidra 12.0.4 was queried read-only against the preserved EXE in a scratch project. `InstructionContext.java 0045A070 14 22` freshly confirms the state 5/6 jumps, type-1 test, virtual `+0x0C` call and null return before radius handling. The pre-existing raw callback decompilation `runtime-callbacks/0045a070_FUN_0045a070.c` supplies the six inclusive comparisons and final queue call (Ghidra has no defined function boundary at this entry, so `DecompileFunctions` reports no function). Fresh decompilation of `0049D350`, `0049D4E0`, and `0049B4C0` confirms route/generation extraction, mapper arguments, flag 0, 64-slot search, and full-queue diagnostic. Fresh `0049B7E0` confirms queued route resolution validates generation before returning the receiver. Thus a queued record whose route is reused with a new generation remains unresolved at delivery.

The native check compiles with the minimal `V`/`Player` fixture while exercising the production `victim_pickup.h`, `FireMessageQueue`, and `EntityIdRegistry` code; it does not recreate geometry, queue storage, or ID allocation. It tests rejected-state/type route/getter suppression, null getter result, getter-before-radius-before-center-before-route, route-generation change during the Player getter, inclusive/outside boundaries on X/Y/Z, FIFO messages/payload pointers for several victims, full queue behavior, and an old generation after route reuse. Run:

```sh
c++ -std=c++17 -Wall -Wextra -Werror decomp/tools/victim_proximity_check.cpp \
  -o /private/tmp/victim_proximity_check
/private/tmp/victim_proximity_check
```

Integration: from a verified owner callback invocation, pass the actual victim state and type-1 recipient; make the Player getter use the existing `0049DB90 → 0049D790` world-box path; make the radius callback return the value already resolved by the source-specific radius helper; provide the current victim center and stable address of the victim object; and resolve the recipient `EntityIdRegistry::Handle` only when the publisher requests it after overlap. Drain the shared `FireMessageQueue` through `EntityIdRegistry` later using the existing runtime path. The timing of the callback relative to Player movement and the recurring victim root updater remains unresolved.
