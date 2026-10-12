# Ghidra helper scripts

`AddressXrefs.java` prints incoming and outgoing code/data references for explicit addresses. It is intended for the preserved, read-only project and does not modify the Ghidra database.

`FindStringReferences.java` finds defined string data containing one or more substrings and lists references to every byte in the string, including interior offsets. This helps trace command names and resource strings without exporting unrelated functions.

`DecompileFunctions.java` prints Ghidra C output for explicit function entry addresses.

`DefineAndDecompile.java` creates a function at a supplied entry when the program does not currently define one, then prints its decompilation. Function creation changes the analysis database: use this only in a temporary Ghidra project after verifying the entry from a vtable, direct branch or other source.

`ExportFunctionAssembly.java` prints the original instruction listing for explicit function entry addresses.

`CallGraphPath.java` searches direct call edges from a start function to a target function. It reports indirect calls and unresolved direct targets separately; a negative result does not cover computed dispatch.

`FindIndirectCallSlot.java` scans listed instructions for indirect `CALL`/`JMP` operands containing a literal displacement (for example `+ 0x14`). This is a text-based candidate finder: it does not resolve a receiver's type, detect a displacement assembled through registers, or prove that a call site is reachable.

`InstructionContext.java` prints a small address-labeled instruction window around an explicit address, including callsites whose containing function is not defined in Ghidra. The addresses make it easier to recover missing function boundaries and check the exact instruction against decompilation output.

`DataValue.java` prints Ghidra's defined data item and the raw 32-bit integer, float, and byte interpretations at explicit addresses. Use it to verify constants loaded by code rather than inferring their value from a symbol name.

`ExportFunctionInventory.java <output.json> [executable-sha256] [ghidra-version]` writes a sorted, read-only index of the functions currently defined in the selected Ghidra database, including entry address, provisional name, body size, thunk/external flags, and an `unclassified` status. It does not run analysis or modify the project. The FDNY snapshot `evidence/original-function-inventory.json` contains 5,807 defined entries for the hash-pinned executable; this is not proof that every routine in the binary has a Ghidra function boundary. Use it as an address denominator to classify, not as a completion percentage.

To regenerate the snapshot from the preserved project, run the query-only Ghidra command shown below with `-postScript ExportFunctionInventory.java decomp/evidence/original-function-inventory.json 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e 12.0.4` in place of the example `AddressXrefs.java` arguments. Keep `-noanalysis -readOnly` so this export does not mutate the reference database.

`FieldStoreAudit.java` lists direct `MOV` stores whose destination includes a structure-field offset. With an immediate-value argument, it filters to direct constant stores. It does not track values copied through registers, so treat it as a bounded search, not proof that no other write exists.

`VictimFloorCollisionPcodeOracle.java` runs the original Genesis.dll PE32 hull-collision path in Ghidra's software P-code emulator for the six authored Training victim candidates. It validates the entry and hull contact-commit instruction, uses fresh emulator state per sample, and writes a hash-provenanced JSON fixture. Pair it with `../victim_floor_pcode_inputs.py` to build the local BSP state and candidate list. The static contacts are compared with the native implementation in [`../../evidence/victim-startup/victim-floor-ghidra-pcode-oracle.md`](../../evidence/victim-startup/victim-floor-ghidra-pcode-oracle.md); this does not capture live scheduling or actor behavior.

Run it with the Ghidra installation on this machine:

```sh
/Applications/Ghidra/support/analyzeHeadless decomp/evidence/ghidra fdny \
  -process FDNYFirefighter.exe -noanalysis -readOnly \
  -scriptPath decomp/tools/ghidra -postScript AddressXrefs.java 0045C7F0 0045BEA0
```

If the preserved project is open in Ghidra or another headless process, a query can fail to acquire its lock even with `-readOnly`. Copy `fdny.gpr` and `fdny.rep/` to a scratch directory under `/private/tmp` and point `analyzeHeadless` at that copy. On this host, headless startup also needs the installed JDK on `PATH` and a writable Java profile, for example `JAVA_HOME=/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home`, `PATH=/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home/bin:/usr/bin:/bin`, and `JAVA_TOOL_OPTIONS=-Duser.home=/private/tmp/fdny-ghidra-user-home`. Keep `-noanalysis -readOnly` for query-only scripts; run `DefineAndDecompile.java` only against a separate analysis scratch copy.

Observed on the current Ghidra 12.0.4 installation: preserved-project queries work with JDK 21, while importing and analyzing a fresh executable or DLL project needs JDK 25 because one analyzer dependency uses Java class-file version 69. If fresh analysis reports `UnsupportedClassVersionError`, switch only that scratch analysis to the installed JDK 25; keep the preserved database query-only. This avoids spending time retrying a JDK mismatch and keeps the reference project unchanged.

For example, direct stores of the value 7 to field offset `0x120` can be listed with `-postScript FieldStoreAudit.java 0x120 7`.

Use `-postScript DecompileFunctions.java 0045BF70` to inspect a function not already exported into the evidence tree.

If the preserved database does not define a verified function entry, import the matching executable into a temporary Ghidra project, run analysis, then use `-postScript DefineAndDecompile.java 00448D60`. Do not use that script with the preserved read-only project; it is intentionally separate from the non-mutating query helpers.

Use `-postScript ExportFunctionAssembly.java 0045BF70` to preserve its instruction listing. The script does not change the Ghidra database.

Use `-postScript CallGraphPath.java 0045BF70 00498B30` to look for a direct call path from one function to another.

Use `-postScript FindIndirectCallSlot.java "+ 0x14"` to list fixed-offset indirect CALL/JMP candidates. This is a candidate scan only; inspect the receiver type and call path before assigning behavior.

Use `-postScript InstructionContext.java 0043AFE3 12 12` to inspect an exact callsite and surrounding instructions without exporting the whole function.

Use `-postScript DataValue.java 004E0ED4` to inspect a referenced constant's data and raw value.

Use `-postScript FindStringReferences.java moveto stopmove setsize` to locate matching string data and its references.

Use this to check a particular reference edge; a missing static code reference does not rule out indirect dispatch through a vtable or runtime registration.

`VictimReleaseFrameOracle.java <output.json> <original.exe> [drop]` executes the original release prefix (`0045C000` to `0045C0EE`) or passout prefix (`0045C1A0` to `0045C295`). Drop mode seeds and executes original bounded RNG, records animation arguments, and stops before floor placement. Only frame getter and animation adapters are supplied; no game process is attached. Numeric fixtures and limits are recorded in `evidence/victim-ai-audit/release-frame-component.md` and `passout-frame-component.md`.

`PlayerRescueDeadlineOracle.java <output.json> <original.exe>` executes original `004541A0` after original seeding, with supplied singleton/Player/app state. It verifies program/input hashes and records deadline plus word-generator draw count for 48 clamp/mode/clock cases. See `evidence/victim-ai-audit/rescue-deadline-exit-component.md` for scope.

`PlayerPainDeadlinePcodeOracle.java <output.json> <original.exe>` executes the bounded original `00451640` suffix from `00451742`, after its vector-facing turn helper. It records equal/deadline-due behavior, the original seeded delay helper/conversion, exposure store, and strict passout branch with string, RC_Pain, and statistic services stubbed. Its three samples are in `evidence/fire-update/player-exposure-timer-followup/original-player-pain-suffix-pcode.json`; its no-draw pain-service pairing and limits are in the adjacent native deadline audit.

`PlayerPassoutPcodeOracle.java <output.json> <original.exe>` executes Player `00450FB0` with controlled animation, string/chatter, list, HeroDown, and selected-item method stubs. Five samples cover parameter 0/1 animation selection, state/timestamp stores, the optional selected-item ACTION-release callback at `+0x1C`, and parameter-2 early HeroDown delegation. The hash-provenanced fixture and explicit limits are in `evidence/fire-update/player-exposure-timer-followup/player-passout-transaction.md`.
