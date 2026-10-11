# Ghidra helper scripts

`AddressXrefs.java` prints incoming and outgoing code/data references for explicit addresses. It is intended for the preserved, read-only project and does not modify the Ghidra database.

`FindStringReferences.java` finds defined string data containing one or more substrings and lists references to every byte in the string, including interior offsets. This helps trace command names and resource strings without exporting unrelated functions.

`DecompileFunctions.java` prints Ghidra C output for explicit function entry addresses.

`FieldStoreAudit.java` lists direct `MOV` stores whose destination includes a structure-field offset. With an immediate-value argument, it filters to direct constant stores. It does not track values copied through registers, so treat it as a bounded search, not proof that no other write exists.

Run it with the Ghidra installation on this machine:

```sh
/Applications/Ghidra/support/analyzeHeadless decomp/evidence/ghidra fdny \
  -process FDNYFirefighter.exe -noanalysis -readOnly \
  -scriptPath decomp/tools/ghidra -postScript AddressXrefs.java 0045C7F0 0045BEA0
```

For example, direct stores of the value 7 to field offset `0x120` can be listed with `-postScript FieldStoreAudit.java 0x120 7`.

Use `-postScript DecompileFunctions.java 0045BF70` to inspect a function not already exported into the evidence tree.

Use `-postScript FindStringReferences.java moveto stopmove setsize` to locate matching string data and its references.

Use this to check a particular reference edge; a missing static code reference does not rule out indirect dispatch through a vtable or runtime registration.
