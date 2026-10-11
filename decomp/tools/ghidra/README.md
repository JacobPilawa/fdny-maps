# Ghidra helper scripts

`AddressXrefs.java` prints incoming and outgoing code/data references for explicit addresses. It is intended for the preserved, read-only project and does not modify the Ghidra database.

`FindStringReferences.java` finds defined string data containing one or more substrings and lists references to every byte in the string, including interior offsets. This helps trace command names and resource strings without exporting unrelated functions.

`DecompileFunctions.java` prints Ghidra C output for explicit function entry addresses.

`DefineAndDecompile.java` creates a function at a supplied entry when the program does not currently define one, then prints its decompilation. Function creation changes the analysis database: use this only in a temporary Ghidra project after verifying the entry from a vtable, direct branch or other source.

`ExportFunctionAssembly.java` prints the original instruction listing for explicit function entry addresses.

`CallGraphPath.java` searches direct call edges from a start function to a target function. It reports indirect calls and unresolved direct targets separately; a negative result does not cover computed dispatch.

`InstructionContext.java` prints a small address-labeled instruction window around an explicit address, including callsites whose containing function is not defined in Ghidra. The addresses make it easier to recover missing function boundaries and check the exact instruction against decompilation output.

`DataValue.java` prints Ghidra's defined data item and the raw 32-bit integer, float, and byte interpretations at explicit addresses. Use it to verify constants loaded by code rather than inferring their value from a symbol name.

`FieldStoreAudit.java` lists direct `MOV` stores whose destination includes a structure-field offset. With an immediate-value argument, it filters to direct constant stores. It does not track values copied through registers, so treat it as a bounded search, not proof that no other write exists.

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

Use `-postScript InstructionContext.java 0043AFE3 12 12` to inspect an exact callsite and surrounding instructions without exporting the whole function.

Use `-postScript DataValue.java 004E0ED4` to inspect a referenced constant's data and raw value.

Use `-postScript FindStringReferences.java moveto stopmove setsize` to locate matching string data and its references.

Use this to check a particular reference edge; a missing static code reference does not rule out indirect dispatch through a vtable or runtime registration.
