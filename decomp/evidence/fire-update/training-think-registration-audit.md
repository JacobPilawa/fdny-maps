# Training FireRescue Think registration follow-up

Read-only Ghidra 12.0.4 audit of the original executable on 2026-10-11. Missing function boundaries were created only in a temporary project copy under `/private/tmp`; the preserved project remained unchanged. No live Training capture was used.

`00449FE0` is slot 0 of the FireRescue visual subobject vtable `004E1774`, installed at object `+0x24` by constructor `0044A330`. The constructor initializes the primary/secondary vtables, validates UserData through `0049D160`, and registers category metadata through `0049D330`. The primary `+0x48` callback is `004483A0`; it handles proximity influence and can enqueue player exposure message `0x7531`. These steps do not call the secondary Think slot.

I also checked the app callback-list route. Idle update `00499410` calls the current game object's vtable `+0x74` and `+0xA0`. The `+0x74` route reaches `0049A310`, which iterates callback objects and invokes their `+0x74` slots, then dispatches the game object's `+0x60` slot. Callback registration `00499DE0` stores an object through app field `+0x40` using `00498F90`, then calls the registered object's `+0x84` slot. Ghidra references to `00499DE0` are data references at `004DF9FC` and `004E3EE4`; FireRescue installs `004E1780` and `004E1774`, and no constructor reference to this app registration route was found.

This rules out the inspected app callback-list registration path as evidence for FireRescue Think scheduling. It does not establish that no other engine-side or dynamic dispatch exists. The recurring caller, cadence, and order of `00449FE0` remain unresolved; see `training-think-behavior-audit.md` and `subobject-init-followup/scheduler-registration-audit.md`.

A bounded follow-up also checked the shared visual-subobject initializer `0046F930` and its cleanup slot, alongside the app registration path. Neither establishes a recurring call to FireRescue secondary slot 0; query details and the remaining limits are in [`think-dispatcher-candidate-followup.md`](think-dispatcher-candidate-followup.md).

## Entity-route registry follow-up

A separate read-only Ghidra pass checked whether the FireRescue entity-route registry also dispatched visual callbacks. Constructor `0044A330` calls `0049D310` to copy root relation fields and `0049D330` to set category bit 1 at `+0x20`; neither call reaches visual slot 0. `0049D450 → 0049B810` allocates/stores a two-word route ID and generation, while destructor-side `0049D510 → 0049B780` releases the ID and clears the handle. These functions manage message identity/lifetime; they provide no recurring invocation of `00449FE0`.

Ghidra 12.0.4 queried the preserved executable in `/private/tmp/fdny-ghidra-ro-20261011/fdny` with `-noanalysis -readOnly`: decompilation of `0044A290, 0044A330, 0049D310, 0049D330, 0049D420, 0049D450, 0049D510, 0049B780, 0049B810`, plus incoming references to `0049D310, 0049D330, 0049D420, 0049D450, 0049D510`. Logs are `/private/tmp/fire-recheck-registration.log`, `fire-recheck-idreg.log`, and `fire-recheck-routes.log`. This narrows one more candidate path; recurring caller, cadence and order remain unresolved.

## Whole-list slot-zero candidate check

A read-only `FindSlotZeroCalls.java` scan of the executable listing found 91 literal indirect calls through a vtable's first slot. One superficially relevant site is `00448173`; it is inside the unclaimed method beginning `00448110`, whose only incoming reference in the preserved database is vtable word `004E16AC`. Its instructions iterate an array at receiver `+0x24` with count `+0x28`, invoke each non-null element's slot 0 with argument `1`, then call `0043B6E0` to clear that array. The receiver's class and any runtime call schedule remain unidentified, and this code does not establish a FireRescue receiver or a call to `00449FE0`. It is therefore a generic slot-zero candidate, not positive Fire Think dispatch evidence.

The scan cannot resolve slot-zero targets from a receiver's runtime vptr, and it misses indirect calls whose target is assembled through other register sequences. Focused read-only Ghidra context at `00448110` and `00448173` and the vtable data reference at `004E16AC` confirmed the bounded interpretation. No new recurring caller for `00449FE0` was found.
