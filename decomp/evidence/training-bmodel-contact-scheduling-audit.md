# Training BModel contact scheduling audit

Read-only Ghidra 12.0.4 inspection of the preserved `FDNYFirefighter.exe` project, cross-checked against Training entity records. The preserved database was opened with `-noanalysis -readOnly`. The only new function boundaries were defined in a temporary scratch project at `/private/tmp/fdny-touch-scratch-20261011`; the preserved database was not modified. No live gameplay capture was used.

## Verified contact-manager path

- Owner method `0042AEA0` reaches `00429520` on its normal path. When owner byte `+0x64C` is nonzero, it takes an early flush-services path instead.
- `00429520` invokes slot-0 methods on the four managers at owner offsets `+0x08`, `+0x50`, `+0x98`, and `+0xE0`, in that order. The fourth manager is constructed by `0049BF10`; it has nine `0x48`-byte category lists and filter mask `0x20` at owner `+0x124`.
- That manager's slot-0 callback is `0049BD20` (vtable `004E4124`). It removes inactive members, then walks its active objects and their selected category lists. For each matching category bit it calls the active object's vtable slot `+0x48` on each member in that list's insertion order.
- BModel constructor `0043F340` sets type `+0x18` to 4 and category bit 1 (mask `2`) at `+0x20`. Its vtable `004E0DF8` has `0043B620` at slot `+0x48`. Thus the manager pass invokes BModel contact callback `0043B620` against category-list index 1.
- Player constructor `00453D50` sets type `+0x18` to 1. `0043B620` also checks candidate type `+0x18 == 1`, then checks the candidate's virtual callback at `+0x0C` returns non-null before doing the reach-expanded AABB test. The type gate establishes a category of type-1 candidates, not that every such candidate is necessarily a Player.
- The final manager's registration method `0049BC90` puts an entity into the list selected by its type field and appends it to the active list. `00429490` performs registration. `00429520` runs this manager pass before staged-entity admission at `+0x3C0`, so newly staged entities are not admitted by that earlier pass in the same function call.

## Type-1 contact participants

The contact callback's `type == 1` check is not a Player-only test. A whole-listing direct-store scan (`FieldStoreAudit.java 0x18 1`) finds two non-stack dword initializers at object root `+0x18`: Player constructor `00453D50` and the `RCS_PlayerHose` constructor `00468050` (store `004681F4`). The Player factory is `00454080`, paired with the `RCS_PlayerRescue` class row at `004E1868/004E1870`; its constructor publishes the singleton at `DAT_00503A78+0x30`. The hose-path factory `00468230` is in the row at `004E2B40`, whose name string is `RCS_PlayerHose` at `004E2B48`. Its root vtable is `004E2D10`, with ScriptActor subobject vtable `004DFC30` at root `+0x24`.

Both roots' vtable slot `+0x0C` points to thunk `0049DB90`, which jumps to `0049D790`. That helper resolves the transform at root `+0x24`, stores the resulting world point at root `+0x68`, and returns that point pointer. Thus `RCS_PlayerHose` satisfies both visible preconditions in `0043B620` if it is admitted to the type-1 manager list. The Training entity dump contains the `RCS_PlayerHose` typedef (record 251) but no instance; dynamic script creation has not been ruled out.

Do not equate this class with the Hose Spray inventory object. Player method `0044CAB0` creates that item through the distinct constructor `004634F0`; its base path `0045F7E0 → 0046F930` initializes root type `+0x18` to zero, and the derived constructor does not overwrite it. That equipment object therefore fails `0043B620`'s type-1 check. From authored Training data and the traced constructors, Player is the only currently evidenced type-1 participant; exact manager-list membership/order still needs registration or live-schedule proof. The direct-store scan does not rule out type assignments through aliases or copied base state.

## Pair and event lifecycle

In the BModel script updater `0043DC60`, call `00470840` at `0043DD74` drains the contact-pair state in this order:

1. New current pairs absent from previous (`00490300`) dispatch `touch`.
2. Every current pair (`00490380`) dispatches `touching`.
3. Previous pairs absent from current (`004903A0`) dispatch `untouch`.

Pair dispatch resolves the target through `00470B50` and invokes script event dispatch at `004705B0`. The action loop checks ScriptActor byte `+0x74` and stops when it is set. After `00470840` returns, `0043DC60` copies current pairs to previous with `00490430` at `0043DED9`, then clears the current pair count through `0043AED0` at `0043DEE0`.

The contact callback `0043B620` forms a two-word candidate key from candidate offsets `+4` and `+8` through `0049D350`, then inserts it into the current pair list via `004901E0`. Insertion ignores a full list and rejects a duplicate exact key using `004901A0` / comparator `0049D3D0`. The list constructor allocates capacity for 16 pairs. These are source-level insertion and event-order facts; no runtime trace was used.

## Cadence, Training impact, and unresolved boundary

The contact callback is reached once per invocation of the owner's normal manager pass (`0042AEA0 → 00429520 → manager +0xE0 → 0049BD20`). No timer or delta-time gate was found in this traced callback/pass. This supports “once per normal owner manager pass,” not a wall-clock or rendered-frame frequency: the exact cadence of `0042AEA0` has not been established here, and its early `+0x64C` branch bypasses the normal pass.

The ordering between contact-pair production and script event conversion remains unresolved. `0043DC60` is reached through the BModel ScriptActor subobject vtable at `004E0DF0`; its local route is root slot `+0x14` (`004E0E0C`) → thunk `0043AFE0` → shared updater `00470740` → virtual slot 0 → `0043DC60`. The preserved project's xrefs do not identify who dynamically calls root slot `+0x14` or place it relative to `0049BD20`. Therefore the source establishes the local `touch → touching → untouch` conversion order and the producer's manager-pass path separately, but does not establish whether a newly generated contact is dispatched in that same owner update or a later update. The native same-pass latch matches neither a proven original latency nor complete callback order until this boundary is resolved. See [`training-door-contact-event-order-audit.md`](training-door-contact-event-order-audit.md) for the verified local event and queue ordering, and [`scriptactor-common-scheduler-audit.md`](scriptactor-common-scheduler-audit.md) for the wider search scope.

## Ghidra queries and limitations

Queries against the preserved project (all `-noanalysis -readOnly`):

- `DecompileFunctions.java 0042AEA0 00429520 00429490 0049BF10 0043F340 00453D50 0043DC60 00470840 00490300 00490380 004903A0 00490430 0043AED0 0049D350 004901E0 004901A0 0049D3D0 004705B0 00470B50`
- `AddressXrefs.java 0049BD20 0049BC90 0043B620 0043DC60 00470840 00490430 0043AED0`
- `DataValue.java 004E4124 004E0E40`
- Follow-up receiver/type scan: `FieldStoreAudit.java 0x18 1`; `DecompileFunctions.java 00453D50 00454080 00468050 00468230 0044CAB0 004634F0 0045F7E0 0046F930 00496020`; `AddressXrefs.java 00468050 00468230 0044CAB0 0049DB90`; `FindStringReferences.java RCS_PlayerHose`; `DataValue.java 004E1868 004E1870 004E2B40 004E2B48 004E2D10 004E2D1C 004E1C54`; and `InstructionContext.java 0049DB90 8 8` plus `InstructionContext.java 0049D7BC 2 36`.

The preserved project lacked function boundaries at `0043B620`, `0049BD20`, and `0049BC90`; these were defined/decompiled only in the scratch project with separate invocations, `DefineAndDecompile.java 0043B620`, `DefineAndDecompile.java 0049BD20`, and `DefineAndDecompile.java 0049BC90`. Preserved vtable references/data at `004E4124` and BModel slot address `004E0E40` were used to verify the indirect entries. The exact dynamic caller/order for `0043DC60`, exact owner update cadence, and real-time event latency remain unknown.
