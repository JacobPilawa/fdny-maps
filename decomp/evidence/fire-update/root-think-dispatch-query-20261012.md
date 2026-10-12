# FireRescue root Think route: bounded Ghidra recheck

Read-only Ghidra 12.0.4 query against the preserved `FDNYFirefighter.exe` database on 2026-10-12. The project remained `-noanalysis -readOnly`; no live process or runtime trace was used.

## Confirmed shared updater behavior

`0043AFE0` is a two-instruction thunk: `ADD ECX,0x24` at `0043AFE0`, then `JMP 00470740` at `0043AFE3`. The FireRescue primary vtable word `004E1794` points to this thunk. Other incoming data references to the thunk are `004E0E0C` and `004E10A4`; Ghidra reports no direct code references to the thunk itself, consistent with virtual dispatch.

The updater `00470740` is defined in the preserved database. Its decompilation checks the receiver's word `+0x4C` (index `0x13`): when equal to 1, it compares word `+0x48` (index `0x12`) with global `DAT_00502AFC` and clears `+0x4C` when due. Otherwise it invokes the adjusted receiver's primary virtual slot 0, copies `+0x4C` to `+0x50`, and, if `+0x4C` is zero, calls `004704A0`. The thunk-adjusted receiver is the FireRescue visual subobject at root `+0x24`, so this virtual slot-0 operation is the already-known edge to secondary slot 0 / `00449FE0`.

Incoming code references to `00470740` are at `00443603`, `00446FE2`, `0044FE5F`, `0043AFE3`, `0043FA76`, `00442BB3`, and `0045999D`. They show the shared updater is called by several root wrappers, but do not identify who invokes the FireRescue root wrapper through primary slot `+0x14`.

## Remaining dead end

This query confirms the local wrapper and state gate only. It supplies no FireRescue caller, registration path, recurring cadence, or relation to the owner/category pass. Since the thunk has only vtable data references and indirect invocation is unresolved, the preserved static call graph stops at the primary vtable word `004E1794`. The actual receiver/list that issues that virtual call remains the next lead.

## Reproduction

```sh
JAVA_HOME=/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home \
PATH=/Library/Java/JavaVirtualMachines/temurin-21.jdk/Contents/Home/bin:/usr/bin:/bin \
JAVA_TOOL_OPTIONS=-Duser.home=/private/tmp/fdny-ghidra-user-home \
/Applications/ghidra/support/analyzeHeadless decomp/evidence/ghidra fdny \
  -process FDNYFirefighter.exe -noanalysis -readOnly \
  -scriptPath decomp/tools/ghidra \
  -postScript AddressXrefs.java 0043AFE0 00470740 004E1794 004E1780 \
  -postScript DecompileFunctions.java 00470740 \
  -postScript InstructionContext.java 0043AFE0 15 15
```
