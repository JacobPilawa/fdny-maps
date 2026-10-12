# Genesis dynamic driver discovery and `DriverHook` call

## Pinned binaries and method

- `Genesis.dll`: SHA-256 `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
- `D3DDrv.dll`: SHA-256 `7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7`.
- Genesis function inventory labels the 247-byte function at `0x10023950` as `FUN_10023950`; this pass used raw x86 instructions, PE import mapping, and data bytes, not a new Ghidra decompilation or function review.

The EXE and Genesis import tables do not statically import `D3DDrv.dll`. Genesis instead imports `LoadLibraryA` at IAT VA `0x1005109C`, `GetProcAddress` at `0x100510A0`, and `FreeLibrary` at `0x100510A8`. The `FUN_10024880` helper constructs a path from the engine path and candidate DLL name, then calls the `LoadLibraryA` IAT slot.

## Candidate enumeration and hook call

Genesis' candidate-name list begins at `0x1005F0C0` and advances in `0xC8`-byte records. The first five names in that list are `D3DDrv.dll`, `GlideDrv.dll`, `SoftDrv.dll`, `SoftDrv2.dll`, and `OglDrv.dll`.

In `FUN_10023950`, the loop:

1. loads a candidate DLL through `FUN_10024880`;
2. calls `GetProcAddress(handle, "DriverHook")`;
3. calls the resolved function with the address of a stack pointer-output slot;
4. if the hook succeeds, reads the returned table pointer and requires dword `[table+4] == 101` and `[table+8] == 1`;
5. stores the table pointer at `[EBX+0x33678]` and invokes its method at offset `+0x14`, with `ECX=0x100238A0` and `EDX=EBX`;
6. calls `FreeLibrary(handle)` and advances to the next candidate.

On the version-mismatch path, the function calls `geErrorLog_AddExplicit` with the string `EnumSubDrivers: found driver of different version, ignoring: `, source string `Engine\\System.cpp`, and line 493. The string reference and control flow support interpreting `FUN_10023950` as an enumerator, but its Ghidra name remains provisional.

The callback body at `0x100238A0` explains the `+0x14` use at the dataflow level. It bounds the subdriver-name copy to less than `0x200` bytes, uses `[EBX+0x33660]` as an index capped at 12, and copies each accepted subdriver record into engine state with stride `0x4488`. It also records the current DLL name from `[EBX+0x33664]` and the active table pointer from `[EBX+0x33678]`. It then calls that table's `+0x18` method, passing the subdriver name in `ECX`, callback `0x10023770`, and the engine pointer.

The callback at `0x10023770` bounds a mode-name copy to less than `0x200` bytes, caps a per-subdriver mode count at 32, and stores each mode record with stride `0x20C` beginning at subdriver-record offset `0x304`. These string and record copies happen before `FreeLibrary`, so the collected enumeration metadata is engine-owned. The code supports provisional behavior labels “enumerate subdrivers” for table `+0x14` and “enumerate modes” for table `+0x18`; no source prototype has confirmed those names. The later selected-driver load and retained handle/table use are traced separately in [selected_driver_initialization_trace.md](selected_driver_initialization_trace.md): Genesis loads the selected DLL again, validates its `DriverHook` table, calls table `+0x20`, and keeps the module handle on the success path.

## Raw xref basis

The strings are at Genesis VAs `0x10053D1C` (`EnumSubDrivers...`) and `0x10053D5C` (`DriverHook`). Searching the pinned DLL for their little-endian absolute pointers finds `.text` file offsets `0x239EC` and `0x23995`, respectively; a second `DriverHook` reference occurs at file offset `0x25F17`. The first reference maps to VA `0x10023995`, in `FUN_10023950`, and is followed by the `GetProcAddress` and indirect `DriverHook` calls. Raw disassembly also shows the version field reads, callback chain, engine-owned enumeration records, and `FreeLibrary` path. The callback bodies at `0x10023770` and `0x100238A0` lie in code that the saved Ghidra function inventory did not identify as function entries; their bounds are manually inferred from instruction flow and call references.

## Next query

Use the same hash-matched Genesis Ghidra program to confirm the function boundary and callers for `FUN_10023950`, then follow `[EBX+0x33678]` and the `0x100238A0` callback. Continue resolving the selected context fields and table methods against additional callsites, and recover the missing function boundaries for callback targets. Keep table fields unnamed until their definitions or callsites prove semantics.
