# Player `+0x343C` deadline input audit

## Finding

The deadline input read by `004541A0` is initialized by Player lifecycle routine `0044D550`. Its raw source is the current level object's 32-bit value at `+0x10`, or `600.0f` when no level object is installed. The routine then multiplies that value by `1.5` for game modes 0 and 1, otherwise by `1.0`, and stores the result back as a float. The code does not establish the semantic name or authoring format of the level object's `+0x10` member; this audit therefore does not label it a rescue-time setting.

The value is not initialized by constructor `00453D50`. That constructor is called by `00454080`; `0044D550` is installed in the Player vtable at `004E1C68`. The preserved database has no direct code xref to the virtual method, so the exact dispatch time is not established here. Its body is the identified writer and lifecycle initialization path.

## Vtable base and staged initialization follow-up

Fresh read-only inspection confirms the Player primary vptr assignment at `00453D98` is `*(this+0)=0x004E1C48`. Therefore `004E1C68` is byte offset `+0x20` from the primary table base, not a separate vtable. The table words verified with `DataValue.java` are:

| Primary-table byte offset | Entry address | Target |
|---:|---:|---:|
| `+0x08` | `004E1C50` | `00453A70` |
| `+0x14` | `004E1C5C` | `0044FE20` |
| `+0x20` | `004E1C68` | `0044D550` |
| `+0x24` | `004E1C6C` | `0043F660` |

Staging routine `0049C670` drains its pending list, obtains a callback receiver for each staged object by invoking that object's `+0x0C` subobject vtable entry `+0x08`, and stores the returned pointer in a temporary array. It then walks that array in order, calls primary-vtable byte offset `+0x20` for every pointer, and in a second full pass calls `+0x24`. Thus, if the pointer returned for the Player is its primary `this` pointer and the Player is present in this staging batch, `0049C670` invokes `0044D550` in the first callback pass, followed later by `0043F660` in the second. This is concrete staged-callback evidence, but the queried code does not prove that Player is submitted through this queue or that this is its only invocation path; it does not establish unconditional Player initialization timing relative to level startup.

This corrects the ambiguous wording “slot `+8`” in `evidence/victim-ai-audit/player-pickup-bounds.md`: if it means byte displacement, it is wrong for `0044D550`; the target is at byte displacement `+0x20` (vtable entry index 8). Other notes that explicitly say slot index 8 (`+0x20`) or byte offset `+0x20` agree with the verified table.

## Writer and consumer order

At `0044DE07`–`0044DE22`, `0044D550` reads the current-level pointer from the application object at `+0x688`. A non-null pointer supplies the dword at `level+0x10`; a null pointer selects immediate `0x44160000` (IEEE-754 `600.0f`). After reading game mode from application `+0x6E4`, the routine selects a double multiplier (`1.5` for modes 0/1; `1.0` for the other branch), executes `FLD [Player+0x343C]`, `FMUL multiplier`, and `FSTP [Player+0x343C]` at `0044DE82`–`0044DE90`. Thus the field is mutated in place during this initializer, with the multiplication stored at float precision.

The level-loading routine `0042AB00` installs its level argument into application `+0x688` before its flush call, which temporarily clears that pointer, then installs the same argument again before parser/startup work. The Player initializer reads whichever current-level pointer is present when `0044D550` runs. This establishes how a level object can feed the field, but not the schedule that calls the virtual initializer relative to level parsing or player spawning.

`004541A0` reads the field at `004541B4` and uses it as an input to the already-recovered deadline calculation. It does not write `Player+0x343C`; its output writes are to the caller-provided target. Four other direct reads at `004533DC`, `00453441`, `004534E3`, and `00453511` occur in bytes not represented as functions in the preserved Ghidra database, so their enclosing routines and roles remain unknown.

## Authoring and script route

The bundle's Training startup scripts (`training.evs`, `rescue.evs`, and their included `tools.evs` / `doors.evs`) contain no evident command that writes this field or the level object's `+0x10` value. The Training `setrescuebot` script route resolves to `00451870`, which writes Player `+0x32E4`, not `+0x343C`. A bounded string-reference search for `rescue_time`, `rescuetime`, `rescue timer`, and `setrescue` found no alternative direct script setter. The scripts and current-level loader do not identify the authored source file or schema for `level+0x10`; no authored scalar value is claimed.

## Evidence and limits

Raw Ghidra query logs are retained in the ignored directory `evidence/victim-ai-audit/player-deadline-input-native/`. The offset access scan reports two direct MOV stores, one x87 FSTP store, two reads in `0044D550`, four additional reads in undefined-function bytes, and the `004541A0` read. The scan examined defined instruction operands for literal `0x343C`; it cannot rule out writes through computed pointers, bulk copies, or indirect aliases. `FieldStoreAudit.java` independently reported the two MOV stores. The assembly context log shows the x87 store.

The query used the preserved Ghidra program read-only with `-noanalysis` on the bundled `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). Raw outputs include the field scan, xrefs, constructor and initializer assembly, deadline-helper assembly, level-loader excerpts, and script/string searches. The direct-xref search does not exclude computed virtual dispatch, and this audit makes no frame-frequency or startup-order claim.
