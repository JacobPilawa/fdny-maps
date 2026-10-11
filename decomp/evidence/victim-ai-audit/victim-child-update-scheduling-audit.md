# Victim child updater scheduling audit

Read-only Ghidra 12.0.4 audit of `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). Queries ran against a scratch copy of the preserved Ghidra project with `-readOnly -noanalysis`; no database or native gameplay code was changed.

## Verified paths

- Victim constructor `0045DDD0` initializes the child at victim `+0xd4`, then installs child vtable `004E25B0` there. Vtable slot 0 (`004E25B0`) points to updater `0045BEA0`. The audited reference query found the updater address referenced by that vtable word; constructor/destructor paths also reference the table.
- Staged entity service `0049C670` drains its staging list, then invokes root-object vtable slots `+0x20` and `+0x24`. For the victim root vtable, `004E25D8` and `004E25DC` point to `0045B690` and `0045D5B0`: actor setup and alternate-spawn setup. This path does not call child slot 0 in the inspected function.
- Owner category iterator at `0049BD20` tests parent entity membership bits and dispatches the selected parent vtable slot `+0x48` (`CALL [EDX+0x48]` at `0049BEB2`). Victim vtable word `004E2600` points to `0045A070`, its primary owner callback. The callback is distinct from child updater `0045BEA0`.
- Victim actor setup `0045B690` calls `0046E320` with the child object at `victim+0xd4`. `0046E320` assigns the actor name/parent fields and calls `00470DD0` on global ScriptEngine `00502FB0`. `00470DD0` appends the child pointer to the engine array at `+0x7c`, count `+0x80` (reusing null slots where available).
- The inspected ScriptEngine consumers use this array for name lookup (`00470B50`), command lookup (`00470C50`), and touch/touching/untouch message dispatch (`00470840 → 004705B0 → 00470150`). These are lookup/event routes; none of the inspected paths invokes child vtable slot 0. The updater's own final actor animation call `00480D50` has a direct code reference from `0045BEA0` in this project's reference database.

## Limits and unresolved item

The preserved incoming-reference query for `0045BEA0` reports only the vtable data word `004E25B0`. This does not establish that the updater never runs: computed dispatch, runtime registration outside the inspected paths, or engine enumeration could call it. Static evidence now separates the known normal paths (owner callback `0045A070`, staged actor/spawn setup, ScriptEngine event registration) from the child updater, but still does not show who schedules `0045BEA0`, its cadence, or its order relative to Player updates and owner message drains. Those timing facts require a verified dynamic trace or a further positively identified dispatcher. The broader shared-actor search is in [`../scriptactor-common-scheduler-audit.md`](../scriptactor-common-scheduler-audit.md).

## Ghidra query scope

Used the existing `AddressXrefs.java`, `DecompileFunctions.java`, and `InstructionContext.java` helpers against a scratch copy because the preserved project may be locked. Queried updater/vtable/constructor addresses, the child registration and ScriptEngine lookup/event functions, victim root vtable callback words, staging service `0049C670`, and owner iterator instructions around `0049BD20`. Commands used the documented JDK 21 `JAVA_HOME`/`PATH` and writable `JAVA_TOOL_OPTIONS` profile. All commands used `-readOnly -noanalysis`.

This refines the open scheduling item in [`victim-state-updater.md`](victim-state-updater.md); it does not close it.
