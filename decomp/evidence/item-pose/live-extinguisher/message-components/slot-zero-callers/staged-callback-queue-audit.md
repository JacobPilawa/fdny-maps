# Owner staged-callback queue audit

Generated 2026-10-11 from the preserved read-only `FDNYFirefighter.exe` Ghidra project. This checks whether the owner's `+0x3C4` callback queue is the missing recurring Fire Think path.

## Queue behavior

Owner constructor `0042B4E0` allocates a 0x4c-byte queue at `owner+0x3C4`. Constructor `0042B2C0` installs primary vptr `004E4158` and a secondary vptr `004DF8E8` at queue `+0x40`. Owner frame method `00429520` calls secondary slot `+4` at `0042959F`, which resolves to `0049C820`.

`0049C820` consumes linked nodes from the queue rooted at the primary object `+0x10`, frees each node, and dispatches its stored object. If the object's byte at `+0x1D` is zero, it invokes the object's primary vtable slot 0 with argument `1`; otherwise it invokes slot `+0x2C`. The same service pass is called during the `TEverGame: Flushing game...` path in `0042A070`.

## Registration evidence and limit

Primary vtable slot `+4` is the node append method `0042B1A0`. The direct reference scan records one computed call to this method inside `0049B9E0` (the ID registry's own queue setup) and vtable data references. A scan of 95 decoded indirect calls through `[register+4]` did not identify a caller that supplies the owner `+0x3C4` queue. The decoded owner-offset scan shows the `+0x3C4` frame drain, flush calls, owner construction/destruction, plus same-offset accesses in unrelated classes; it exposes no FireRescue callback insertion.

The global owner table stores the queue pointer at `00503A78+0x20`, but among the 139 functions that reference `00503A78`, the constructor is the only exported function that accesses that `+0x20` field. This lowers the likelihood that this service is the regular Fire Think scheduler, but does not prove the queue is unused or rule out an indirect owner path.

Fire Think `00449FE0` is only referenced by the FireRescue visual secondary vtable `004E1774`, installed at the FireRescue instance `+0x24` by constructor `0044A330`. No inspected code enqueues that visual subobject on the owner's `+0x3C4` queue. Keep the scheduler unresolved; do not connect `0049C820` to Fire Think based on its generic slot-0 dispatch alone.

## Constructor and staged-entity follow-up

The follow-up xref scan is preserved in `fireresque-constructor-xrefs.tsv`; focused decompilations and instructions are in `fireresque-helper-exports/` and `fireresque-helper-assembly.txt`. It confirms `00432A40`, stored at visual-base offset `+0x40` by `0046F930`, is a release callback: it calls `00401C90` on the object and frees it. It is not an update hook. FireRescue constructor `0044A330` initializes primary vtable `004E1780` and visual secondary vtable `004E1774`; its teardown `0044A290` restores the same tables. The only code/data xrefs to Think `00449FE0` in this scan are the slot-0 word in `004E1774`.

Owner `+0x3C0` service `0049C670` processes staged entities once, registering each and calling primary-vtable offsets `+0x20` then `+0x24`. FireRescue's primary table maps these to placed-fire initialization `00448D60` and base no-op `0043F660`; neither is secondary visual slot 0 (`00449FE0`). Helpers `0049D310` and `0049D330` only copy constructor state/set a membership bit, and entity-data validation `0049D160` does not enqueue a callback. These paths therefore establish initialization and category registration, not recurring Fire Think scheduling.

Raw evidence is retained beside this note: `staged-callback-references.tsv`, `staged-callback-slot4-calls.tsv`, `staged-callback-owner-offsets.tsv`, `staged-callback-global-users/`, `fireresque-constructor-xrefs.tsv`, `fireresque-helper-exports/`, `fireresque-helper-assembly.txt`, `../0049c820_FUN_0049c820.c`, `../app-callback-registration/00429520_FUN_00429520.c`, and `../app-callback-registration/0042aea0_FUN_0042aea0.c`.
