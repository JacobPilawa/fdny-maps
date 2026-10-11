# Training BModel throw runtime: Ghidra audit

This audit extends [`throw-path-ghidra-audit.md`](throw-path-ghidra-audit.md) and [`throw-scalar-oracle.md`](throw-scalar-oracle.md). It traces the runtime method containing the direct `0043DDC6 → 0043D330` call, the model mutation performed by `0043BAA0`, and the static limits on establishing the update cadence. It is static executable evidence plus Ghidra decompilation; it is not a live gameplay trace.

## Verified caller and object offsets

- BModel construction at `0043F340` installs the table beginning at `004E0DF0` at BModel root `+0x24`. The first word at `004E0DF0` is `0043DC60`. `0043DC60` derives the BModel root as its receiver minus `0x24` and passes that root to `0043D330` at `0043DDC6`. The constructor separately installs the table beginning at `004E0DE4` at root `+0xA0`; these are two distinct subobject vtables.
- The preserved Ghidra project has no function boundary at `0043DC60`; full auto-analysis on an isolated project copy also did not create one. The prologue at `0043DC60` and its BModel-subobject adjustment were verified in Ghidra, then that entry was defined and decompiled only in the analysis scratch copy. The callsite is therefore verified as part of the `0043DC60` vtable method, while its dynamic dispatcher is not identified.
- The vtable receiver is the subobject at root `+0x24`; consequently, the caller's `[receiver+0x234] != 0` gate addresses BModel root `+0x258` (`useMode`). If `useMode` is zero, `0043DC60` does not invoke `0043D330`. `0043D330` itself performs throw interpolation only when root `+0x25C == 2`. These are separate gates: the caller requires a nonzero use mode, and the updater requires throw mode 2.
- Before this gate, `0043DC60` returns if its receiver `+0x1A0` is less than one; that is the BModel root's model count at `+0x1C4`. This ties the update path to a BModel with at least one model. The model-pointer array is root `+0x1C0`, as used by both `0043D330` and `0043BAA0`.

## `0043BAA0` model-transform mutation

The `this` pointer is the BModel root. The routine iterates `root+0x1C0[0..root+0x1C4)`. For each model ID, it obtains the render-world/model context through `DAT_00503A78` and its selected `+0x78` field; the context selection branches on the byte at the subsystem object `+0x160` (`+0x8` versus `+0xB4` before loading `+0x78`).

For each model, the routine calls the imported functions Ghidra labels `geWorld_GetModelRotationalCenter` and `geWorld_GetModelXForm`. It then subtracts the returned three-coordinate rotational-center value from the three coordinates supplied by the caller. Those three differences replace the final three float fields of the 48-byte xform buffer; the preceding 36 bytes fetched into that buffer are not changed before `geWorld_SetModelXForm`. The stores are three x87 float subtractions rounded at their `FSTP float` instructions. This describes the code's data flow; this audit does not assign an extra local/world-space interpretation to the imported rotational-center result.

When its third argument is nonzero, `0043BAA0` also copies the supplied coordinates into BModel root `+0x224`, `+0x228`, and `+0x22C`. It then calls `0043B7A0`, which recomputes the bounds at `+0x240..+0x257`, the derived values at `+0xF0..+0x120` and `+0x278..+0x28C`, and the axis-box interface at `+0x260`. The throw updater passes nonzero for both its interpolated point (`0043D522`) and selected endpoint (`0043D46D`). A separate caller, `0043BD90`, invokes this mutator with a zero third argument when correcting the object's position.

The shared vertex/model context is selected from global `DAT_00503A78` in `0043BAA0`; this function does not read or write Player fields. The held-object pointer and Player state that lead into throw setup are documented in the linked path audit: `0044BC40` stores the carried BModel at Player root `+0x3440`, sets Player root `+0x18C` to 7, and sets BModel root `+0x25C` to 1. Release invokes `0043B580`; the Player updater then clears `+0x3440` and changes `+0x18C` to 3.

## Throw completion writes and event

The scalar oracle and the `0043D330` listing show these completion operations, in order: set root `+0x25C` to zero; call `0043BAA0(root, selectedEndpoint, true)`; flip the byte at root `+0x220`; construct the script event name `drop`; dispatch it through the ScriptActor subobject at root `+0x24`; then call `004A11A0(root+0x124, null)` to clear the timer/list state. The selected endpoint is root `+0x1FC` when the pre-flip `+0x220` byte is zero, otherwise root `+0x1F0`. The event construction and dispatch are visible at `0043D47D..0043D497`; the endpoint selection and mutator call are at `0043D457..0043D46D`.

On the active path, `0043D330` calls `0043BAA0(root, interpolatedPoint, true)` at `0043D522`, then samples the path at root `+0x21C` and performs a further model-transform update. The scalar point calculation, tick rollover, duration comparison, and rounding boundary are recorded in the scalar oracle. The event meaning beyond the literal event name and its dispatch target is not inferred here.

## Scheduling boundary

The read-only xref scan finds one direct code caller of `0043D330`: `0043DDC6` inside `0043DC60`. A follow-up [dispatcher audit](bmodel-scriptactor-dispatch-ghidra-audit.md) verifies the local virtual route: BModel root slot `+0x14` calls thunk `0043AFE0`, which adjusts to the ScriptActor subobject and tail-jumps to `00470740`; that shared updater can invoke slot 0, resolving to `0043DC60`, subject to its state gate. The regular owner/category pass, staging service, and render traversal inspected in that audit do not invoke this updater thunk. The executable contains a data reference from vtable word `004E0DF0` to `0043DC60`, and the BModel constructor installs that table at root `+0x24`. These findings verify the local dispatch route but do not identify its regular runtime caller, place it relative to the contact manager, or establish cadence.

## Reproduction and retained Ghidra extracts

Input executable: `evidence/original/FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Ghidra version: 12.0.4 PUBLIC. The preserved project was copied to `/private/tmp/fdny-throw-runtime-ghidra-20261011/fdny`; its `fdny.rep/project.prp` SHA-256 after the read-only queries was `478fe337b1cd28c845c6a33005993dde9f66353b971303d22a263722c720d487` (the empty `.gpr` file hash is the standard empty-file hash). All queries against that copy used `-noanalysis -readOnly` and the existing scripts under `decomp/tools/ghidra`.

To recover the missing method boundary, a second isolated copy at `/private/tmp/fdny-throw-runtime-analysis-20261011/fdny` was auto-analyzed, then `DefineAndDecompile.java 0043DC60` was run only on that copy. The preserved database was not modified. Relevant extracts, including raw Ghidra output, are in [`throw-runtime-ghidra/`](throw-runtime-ghidra/): `ghidra-first-pass.txt`, `ghidra-mutator-followup.txt`, `ghidra-analysis-recovery.txt`, `caller-address-listing.txt`, `caller-function-analysis.txt`, `throw-field-vtable-audit.txt`, and `constructor-vtable-context.txt`.

Reproduction commands, from the repository root (with the same scratch copies and Ghidra installation):

```sh
JAVA_HOME=/Library/Java/JavaVirtualMachines/temurin-25.jdk/Contents/Home \
JAVA_TOOL_OPTIONS='-Duser.home=/private/tmp/fdny-throw-runtime-user-20261011' \
/Applications/Ghidra/support/analyzeHeadless \
  /private/tmp/fdny-throw-runtime-ghidra-20261011 fdny \
  -process FDNYFirefighter.exe -noanalysis -readOnly \
  -scriptPath decomp/tools/ghidra \
  -postScript AddressXrefs.java 0043BAA0 0043DDC6 0043D330 0043B580 \
  -postScript InstructionContext.java 0043DDC6 24 60 \
  -postScript DecompileFunctions.java 0043BAA0 0043D330 \
  -postScript ExportFunctionAssembly.java 0043BAA0 0043D330

JAVA_HOME=/Library/Java/JavaVirtualMachines/temurin-25.jdk/Contents/Home \
JAVA_TOOL_OPTIONS='-Duser.home=/private/tmp/fdny-throw-runtime-user-20261011' \
/Applications/Ghidra/support/analyzeHeadless \
  /private/tmp/fdny-throw-runtime-analysis-20261011 fdny \
  -process FDNYFirefighter.exe -noanalysis \
  -scriptPath decomp/tools/ghidra \
  -postScript DefineAndDecompile.java 0043DC60 \
  -postScript AddressXrefs.java 0043DC60 \
  -postScript ExportFunctionAssembly.java 0043DC60
```

The caller instruction-address extract was produced by a read-only listing script stored in `/private/tmp/ThrowRuntimeAddressListing.java`, using the first project copy and range `0043DB20..0043DF10`. Auto-analysis recovery was run only on the second scratch copy. No game was launched and no native gameplay code was changed.

The remaining focused commands used `FieldStoreAudit.java 0x234`, `FieldStoreAudit.java 0x218`, `DataValue.java 004E0DE4 004E0DE8 004E0DEC 004E0DF0 004E0DF4 004E0DF8 004E0E0C 004DC07C 004DC080 004DC084`, and `AddressXrefs.java 004E0DE4 004E0DE8 004E0DEC 004E0DF0`, again on the first scratch project with `-noanalysis -readOnly`. The caller listing command was `-postScript ThrowRuntimeAddressListing.java 0043DB20 0043DF10` with `-scriptPath /private/tmp`.
