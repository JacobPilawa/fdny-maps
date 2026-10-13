# Training FireRescue dispatch and local order

Read-only Ghidra 12.0.4 audit of the original `FDNYFirefighter.exe`, checked against the existing preserved scheduler exports on 2026-10-11. No live process was attached or captured. This note separates the owner callbacks with a proven route from the still-unlocated visual Think dispatcher.

## Proven normal-owner path

On the normal path, owner update `0042AEA0` calls `00429520` (`0042AEA0` xref at `0042AF0A`). The owner’s byte `+0x64C` takes an early flush path instead, so the following order is conditional on reaching the ordinary path. `00429520` calls four manager slots in this order: owner `+0x08`, `+0x50`, `+0x98`, then `+0xE0`. It then calls the staged-entity service at `+0x3C0`, message drain at `+0x3BC`, sound service `004A0660`, and staged-object service at `+0x3C4`.

The `+0xE0` callback is manager `0049BD20`. Its category pass walks nine category lists; for each matching category bit on an active object, it dispatches that object's primary vtable slot `+0x48` against members of the selected list. FireRescue's primary vtable word at `004E17C8` (`004E1780 + 0x48`) is `004483A0`. The source body of `004483A0` is the proximity/influence path: under its state, counter, position, and contact gates it evaluates fire influence and can enqueue message `0x7531` to the candidate's route. This is the exact FireRescue callback reached through the identified regular owner manager.

The local order makes one same-owner guarantee: a `0x7531` enqueued by `004483A0` during the `+0xE0` pass precedes the `+0x3BC` drain in that same `00429520` call. This does not place Player movement, extinguisher contacts, or any visual Think invocation against the owner call.

Staged entities are admitted later in the same owner method. Service `0049C670` removes staged queue entries and registers each object through `00429490`, then makes one pass over the admitted array calling primary slot `+0x20`, followed by a second pass calling primary slot `+0x24`. For FireRescue these entries resolve to initialization `00448D60` and base no-op `0043F660`. Thus, a FireRescue first admitted by this service cannot receive the earlier `+0xE0` callback in that same `00429520` invocation. Neither staged hook is visual Think `00449FE0`.

## Visual Think: local work, caller unresolved

Ghidra data at `004E1774` is `00449FE0`; this is slot 0 of the FireRescue visual secondary vtable installed at object `+0x24`. In the preserved project, `AddressXrefs.java 00449FE0` found only that vtable data word as an incoming reference, not a code call. Prior bounded audits checked the app callback-list path, staged-entity hooks, the owner's `+0x3C4` staged-object queue, constructor callbacks, owner/category dispatch, and a broad set of decoded indirect-transfer candidates; none connected a recurring call to this secondary slot. This is a search result, not proof that no engine-side or runtime-computed dispatcher exists.

If `00449FE0` is invoked, original instructions establish this local order:

1. Visual state `+0x4C == 5` returns immediately. Otherwise it updates the color interpolant.
2. Unless sprayed flag `+0xE0` is set or state is `4`, it decrements divider `+0xF8`. If the byte remains nonzero, it calls visual animation/size update `0044B060` and returns.
3. On a full update it resets the divider to `3`, checks route visibility, and calls route helper `00449340` only for state `4`.
4. It calls `0044B060`, updates orientation, advances strength from frame delta `[00502B04]` when not sprayed, updates alpha through `0044A610`, then clears sprayed `+0xE0` and exposure count `+0x94`.

The state-4 helper `00449340` contains a conditional FireRescue factory path, but the extracted Training fire records have no authored route properties; neither entry into state `4` nor child-fire creation is established for Training from current source evidence. Keep this path for full-game coverage without treating it as a Training runtime event.

The regular owner path therefore proves the primary proximity callback and its order before message delivery; it does not prove that visual Think runs, how often it runs, or its position relative to player/tool callbacks. No wall-clock/frame cadence or whole-run RNG order is claimed.

## Ghidra evidence and scope

- Preserved project queries, all `-noanalysis -readOnly`: `AddressXrefs.java 00449FE0 004E1774 004483A0 00429520 0049BD20`; `DataValue.java 004E1774 004E1780 004E17C8`; `DecompileFunctions.java 0042AEA0 00429520 0049C670 0049B610`.
- The preserved project has no function boundary at `0049BD20` or `004483A0`; their Ghidra decompilations are retained from the temporary project in `../item-pose/live-extinguisher/message-components/entity-manager-dispatch/0049bd20_FUN_0049bd20.c` and `004483a0_FUN_004483a0.c` in this directory. Existing owner order exports are in `../item-pose/live-extinguisher/message-components/owner-deferred-service-trace.md` and `../item-pose/live-extinguisher/message-components/app-callback-registration/00429520_FUN_00429520.c`.
- Think assembly is `think-assembly.txt`; the detailed branch and registration limits remain in `training-think-behavior-audit.md`, `training-think-registration-audit.md`, `subobject-init-followup/scheduler-registration-audit.md`, and `../item-pose/live-extinguisher/message-components/slot-zero-callers/register-transfer-audit.md`.
