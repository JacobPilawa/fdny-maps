# Victim script command audit

Read-only Ghidra 12.0.4 audit of the preserved `FDNYFirefighter.exe` project on 2026-10-11. The executable SHA-256 is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. The analysis database was opened with `-noanalysis -readOnly`; no function boundaries or database state were changed.

## Custom victim commands

The victim child vtable installed at entity `+0xd4` points to `0045CDB0` for script events and `0045BEA0` for its updater. `0045CDB0` first gives the event to base handler `0047A1F0`; only when that handler declines does it inspect victim-specific commands. Ghidra decompilation and direct call xrefs support these command paths:

| Command | Ghidra path | Verified effect |
| --- | --- | --- |
| `playanim` | `0045CDB0 → 0045B3B0` (call at `0045CF27`) | Parses an integer argument, then calls `0045B3B0`. That helper only logs `"PlayAnim" IS UNIMPLEMENTED` (`004E233B`, referenced at `0045B3F8`) and returns handled; it does not select or set an actor pose. |
| `forcespawnspot` | `0045CDB0 → 0045A4C0` (call at `0045D03C`) | Parses an integer index. `0045A4C0` accepts only `0 <= index < entity[+0x204]`, copies the selected saved spot into entity position `+0x4c..+0x54`, and sets frame heading through `00496C30`. It does not write the victim state field `+0x120`. |
| `roam` | `0045CDB0 → 0045C7F0` (call at `0045D116`) | Parses a floating-point argument with `0040CC50` (`sscanf` `%f`, then `%g`) and enters the state-7 movement route. `0045C7F0` stores the parsed bits at `+0x224`, writes state 7 to `+0x120`, initializes its goal from current position, sets speed to `36.0`, and probes the four `(±5,0,±5)` offsets before continuing or falling back to state 4 if all are blocked. The argument's purpose is not established here. |
| `norescuebot` | Inline branch in `0045CDB0` (string reference `004E2538 → 0045D13E`) | When the command node has at least one argument, writes byte 1 to victim `+0x129`. The loop does not parse or inspect the argument's value. |

These are generic script-handler capabilities, not proof that Training uses them. The authored Training victim records 191 and 194 in `assets/training/entities.json` contain `pickup:` commands only. They do not author `playanim`, `forcespawnspot`, `roam`, or `norescuebot`. Training's player `setrescuebot 0` is a separate player-script command path (`00451870`), so it should not be conflated with the victim's `norescuebot` command.

## Scheduling result

The child is registered with the script engine, which narrows—but does not resolve—the missing scheduler path. Victim initializer `0045B690` calls `0046E320` with the child at victim `+0xd4`; `0046E320` calls `00470DD0` with global object `00502FB0` and that child pointer. `00470DD0` logs `- added actor to ScriptEngine: ` from source path `script\\scriptengine.cpp` and appends the pointer to an array at engine `+0x7c` with count `+0x80`.

The script-engine array APIs inspected here use that list for lookup and event routing: `00470B50` resolves an actor by comparing its child name at `+0x28`; its direct callers are in `00470840`, which dispatches `touch`, `touching`, and `untouch` events through `004705B0`. `00470C50` resolves an actor by a named child command. `00470A70` removes an actor pointer and compacts the list. These paths establish ScriptEngine registration and command dispatch, but not recurring invocation of the child vtable slot 0.

The Ghidra xref query still shows `0045BEA0` with only the child-vtable data reference at `004E25B0`; `0045DDD0` installs that vtable at victim `+0xd4`. The updater's cadence remains unresolved, as recorded in `victim-state-updater.md`; indirect dispatch elsewhere is not ruled out.

The command parser distinction was checked by decompiling `0040CC50` and `0040CD50`: the former scans `%f` and then `%g`, while the latter scans integer syntax (`%i` or `%x`). Commands were inspected with `DecompileFunctions.java` and `AddressXrefs.java`; literal references were cross-checked with `FindStringReferences.java`. The Training command absence was checked against the extracted entity records. This audit changes no native runtime code.
