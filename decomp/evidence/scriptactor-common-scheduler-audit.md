# ScriptActor common scheduling audit

Read-only Ghidra 12.0.4 audit of `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`), using the preserved project with `-noanalysis -readOnly`. No native code or live game state was used.

## Actor registration is not a proven update list

- `0046E320` writes the registered actor's parent/name fields, then calls `00470DD0(&DAT_00502FB0, actor)`. `00470DD0` appends the actor pointer to ScriptEngine storage at `+0x7c`, with count at `+0x80`; it fills a zero slot first or grows the array.
- ScriptEngine constructor `00470ED0` initializes that storage and registers its console actor through the same append helper. This proves registration and list membership, not per-tick update dispatch.
- The inspected consumers walk that array for event name lookup (`00470B50`), script-actor/name lookup (`00470C50`), and contact events (`00470840 → 00470B50 → 004705B0 → 00470150`). The paths dispatch named script actions. They do not call a registered actor's root `+0x14` updater or its child ScriptActor slot 0.
- Direct code xrefs to the shared ScriptActor updater `00470740` are at `00443603`, `00446FE2`, `0044FE5F`, `0043AFE3`, `0043FA76`, `00442BB3`, and `0045999D`. The known Player and BModel root callbacks are wrappers among these; the xref set itself has no call from ScriptEngine registration/list traversal. It is not exhaustive for indirect calls.

## Main game callback candidate

- The traced app-idle path reaches owner callback thunk `0042BC60 → 0042AEA0` through app slot `+0x60` after the app's registered `+0x74` callback update. That proves an app-idle route into the owner manager, not a ScriptActor root `+0x14` dispatch. Separately, app slot `+0xA0` reaches callback-list walker `00499E60`, which calls each registered member's slot `+0x70`; these are distinct dispatch interfaces. The callback and pending-message behavior is detailed in [`item-pose/live-extinguisher/message-components/owner-vtable-indirect-call-inventory.md`](item-pose/live-extinguisher/message-components/owner-vtable-indirect-call-inventory.md).
- `0042AEA0` first calls global object slot `+0x14` at `0042AED4`. Startup `004992D0` creates the object stored at `*DAT_00503A78` with constructor `004395C0`; that constructor installs vtable `004E0B44`. The verified vtable word at `004E0B58` (slot `+0x14`) points to `00439330`.
- Constructor `004395C0` registers input actions including `MOVE_FORWARD`, `MOVE_BACKWARD`, `MOVE_LEFT`, `MOVE_RIGHT`, `ACTION`, and `JUMP`. Method `00439330` dispatches its input subobject's slot `+0x14` and calls `SetCursorPos(200, 200)`. So this frame-path `+0x14` call is on the input object and is not evidence of a ScriptActor scheduler.
- On the normal path, `0042AEA0` calls the four owner-manager passes through `00429520`; the known entity pass is the `+0x48` category callback (`0049BD20`). Its staging service `0049C670` calls each new entity's slots `+0x20` then `+0x24`. Those inspected paths do not dispatch root `+0x14`.

## Bounded conclusion and limits

This pass did not identify a shared recurring dispatcher for Player root `+0x14 → 0044FE20`, BModel root `+0x14 → 0043AFE0`, or victim child slot 0 `0045BEA0`. It did rule out the ScriptEngine actor-registration array and the traced input, owner-category, and staged-entity paths as positive evidence for that scheduler. The root-slot caller, cadence, and order relative to contact, fire, and victim updates remain unresolved. See [`player-update-scheduling-audit.md`](player-update-scheduling-audit.md), [`training-bmodel-contact-scheduling-audit.md`](training-bmodel-contact-scheduling-audit.md), and [`victim-ai-audit/victim-child-update-scheduling-audit.md`](victim-ai-audit/victim-child-update-scheduling-audit.md).

`FindIndirectCallSlot.java` performed a whole-listing scan for fixed textual `CALL`/`JMP` operands containing `+ 0x14`; it returned 53 candidates. The full candidate output is preserved in [`scriptactor-common-scheduler-audit/fixed-offset-call-candidates.md`](scriptactor-common-scheduler-audit/fixed-offset-call-candidates.md). The known main-frame call at `0042AED4` and Player use-candidate geometry query at `00459519` were inspected. This syntax scan does not find indirect calls whose slot displacement is assembled in registers or otherwise computed, and it does not establish receiver types or reachability. No negative conclusion is drawn from the scan alone.

## Ghidra query provenance

- All preserved-project commands used Ghidra 12.0.4, JDK 21, `-noanalysis -readOnly`.
- `DecompileFunctions.java 0046E320 00470DD0 00470ED0 00471250 00470740 00499DE0 00499E60 00499410 0042AEA0 00429520 0042A070 0049C670 00429490 00439330 004395C0 00470B50 00470C50 00470840 004705B0 00470150`.
- `AddressXrefs.java 00470740 0046E320 00470DD0 0049D160 0049D330 0049BD20 0049BD30 00502FB0 00503A78 00439330 004E0B58`.
- `DataValue.java 004E0B44 004E0B58`; `InstructionContext.java 0042AED4 6 6`, `0042BC66 10 20`, and the `00470740` incoming-call contexts listed above.
- `FindIndirectCallSlot.java + 0x14` was run against the preserved instruction listing. The script is retained in [`../tools/ghidra/FindIndirectCallSlot.java`](../tools/ghidra/FindIndirectCallSlot.java).
