# Training FireRescue Think registration follow-up

Read-only Ghidra 12.0.4 audit of the original executable on 2026-10-11. Missing function boundaries were created only in a temporary project copy under `/private/tmp`; the preserved project remained unchanged. No live Training capture was used.

`00449FE0` is slot 0 of the FireRescue visual subobject vtable `004E1774`, installed at object `+0x24` by constructor `0044A330`. The constructor initializes the primary/secondary vtables, validates UserData through `0049D160`, and registers category metadata through `0049D330`. The primary `+0x48` callback is `004483A0`; it handles proximity influence and can enqueue player exposure message `0x7531`. These steps do not call the secondary Think slot.

I also checked the app callback-list route. Idle update `00499410` calls the current game object's vtable `+0x74` and `+0xA0`. The `+0x74` route reaches `0049A310`, which iterates callback objects and invokes their `+0x74` slots, then dispatches the game object's `+0x60` slot. Callback registration `00499DE0` stores an object through app field `+0x40` using `00498F90`, then calls the registered object's `+0x84` slot. Ghidra references to `00499DE0` are data references at `004DF9FC` and `004E3EE4`; FireRescue installs `004E1780` and `004E1774`, and no constructor reference to this app registration route was found.

This rules out the inspected app callback-list registration path as evidence for FireRescue Think scheduling. It does not establish that no other engine-side or dynamic dispatch exists. The recurring caller, cadence, and order of `00449FE0` remain unresolved; see `training-think-behavior-audit.md` and `subobject-init-followup/scheduler-registration-audit.md`.
