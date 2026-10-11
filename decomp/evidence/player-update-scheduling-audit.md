# Training Player movement scheduling audit

Read-only Ghidra 12.0.4 inspection of `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`) on 2026-10-11. The preserved project was opened with `-noanalysis -readOnly`; no live game capture was used.

## Verified object paths

- Allocator `00454080` allocates `0x3580` bytes and calls Player constructor `00453D50`. That constructor calls base initializer `0043F610`, then installs the primary Player vtable `004E1C48` at object `+0`, ScriptActor vtable `004DFC30` at `+0x24`, and the movement-subobject vtable `004E1C40` at `+0x140`.
- The movement method `00452570` is slot 0 of the `+0x140` subobject vtable. The preserved incoming-reference query returns the vtable word `004E1C40` as its only reference. It found no direct code call to that entry.
- Player root vtable word `004E1C5C` (offset `+0x14`) points to wrapper `0044FE20`. Its instructions test the dword at Player `+0x3438`; when zero, `0044FE59–0044FE5F` pass `Player+0x140` to shared ScriptActor updater `00470740`. If the embedded wait flag at `+0x4C` is clear on entry, `00470740` invokes vtable slot 0, `00452570`. If the flag is set on entry, it checks deadline `+0x48` against clock `00502AFC` and skips slot 0 for that invocation, even when the deadline has just expired and the flag is cleared. The local path is therefore `Player root +0x14 → 0044FE20 → Player+0x140 / 00470740 → slot 0 / 00452570`, gated by the Player dword and wait state.
- Player primary vtable word `004E1C90` (offset `+0x48`) points to `0043F670`, an empty function. The owner category dispatcher `0049BD20` invokes root slot `+0x48` for selected members, so that root callback cannot itself be the Player movement implementation. Membership and callback frequency for the Player were not established here.
- Player level initialization `0044D550` is primary vtable slot `+0x20`. It registers the ScriptActor named `Player` through `0046E320`; the inspected body does not call `00452570` or the app callback registration method `00499DE0`. ScriptEngine name/event registration is not evidence that the child movement vtable is updated.
- A bounded direct-store audit for Player `+0x3438` found constructor input at `0044D550` and constant-1 stores at `004504C0` and `00450A00`. The latter is reached by the Player `finalexit` command branch in `00451870`; that command is absent from preserved Training scripts. `004504C0` is reached by the Training `escape`/rescue path and sets 1 when its scan accumulates no final-exit zones. The Training exit-condition result and the field's full semantic name are not inferred here.
- App idle `00499410` calls app slots `+0x74` and `+0xA0` in sequence when no Windows message is pending. App method `00499E60` walks its registered callback list and calls each member's slot `+0x70`. The list entries have not been identified as the Player movement subobject, and this path does not establish a call to its slot 0.

## Conclusion and limits

The local Player movement call chain is now source-mapped, but the regular caller that invokes Player root slot `+0x14` is not. The verified owner root callback is a no-op, and the inspected app callback list does not identify the movement subobject. Static xrefs do not rule out a computed engine dispatcher or an indirect registration path not identified here. The movement cadence and its order relative to owner callbacks, message drains, Fire Think, and victim child updates therefore remain open. Do not treat the native per-frame call as original scheduling evidence. The bounded shared ScriptActor search is recorded in [`scriptactor-common-scheduler-audit.md`](scriptactor-common-scheduler-audit.md).

This refines the movement scheduling boundary noted in [`training-ladder-audit.md`](training-ladder-audit.md) and [`item-pose/live-extinguisher/message-components/owner-vtable-indirect-call-inventory.md`](item-pose/live-extinguisher/message-components/owner-vtable-indirect-call-inventory.md).

## Ghidra query provenance

- `AddressXrefs.java 00452570 00453D50 004E1C40 0044D550 00499E60 00499DE0 0043F670 0049D330 0044FE20 004E1C5C 00450A00 004504C0`.
- `FieldStoreAudit.java 0x3438` (three direct MOV stores; register-fed stores are outside this bounded scan).
- `DataValue.java 004E1C40 004E1C90` and adjacent primary-vtable words.
- `DecompileFunctions.java 00454080 00453D50 0043F610 0044D550 0043F670 00499410 0049A310 00499E60 00499DE0 00498F90 0049D330 00470740 004504C0 00450A00 00451870`.
- `InstructionContext.java 0044FE20 10 20` verifies the `+0x3438` gate and call to `00470740` at `0044FE5F`; `DataValue.java 004E1C5C` verifies the root `+0x14` target.
- The focused Ghidra logs were written outside the repository under `/private/tmp/fdny-player-*-ghidra.log`; the preserved project remained unchanged.
