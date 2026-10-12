# Selected renderer load and table initialization trace

## Inputs and method

- `Genesis.dll`: SHA-256 `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
- `D3DDrv.dll`: SHA-256 `7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7`.
- Both files are under `inputs/original/binaries/`. The VA ranges below were checked against raw x86 disassembly and PE data bytes. No Ghidra server was launched and no game/runtime test was performed.
- The saved Genesis function inventory has `FUN_10025E00` at `0x10025E00`, size 724 bytes. It follows `geEngine_ShutdownDriver` at `0x10025D80`. `FUN_10025E00` is a provisional label; the inventory gives no recovered prototype.

## Genesis selected-driver load

The public Genesis export `geEngine_SetDriverAndMode` is a wrapper around this selected-driver path. At export VA `0x100263E0`, it calls `FUN_10025E00` at `0x100263F0`, forwarding its third stack argument through `EDI`; the helper's selected-DLL load, `DriverHook` resolution, table validation, `+0x20` initialization, and retained handle are described below. The EXE's corresponding IAT entry is VA `0x004DC1E4` (Genesis FirstThunk RVA `0x000DC028`, import index 111), and its only direct caller found is `FUN_004998B0` at call instruction `0x00499BE2`, reached during startup/window initialization from `FUN_0042C800`. The caller supplies values derived from `[ESI+0x74]`, `[[ESI+8]+0x18]`, and `[[ESI+8]+4]`; their meanings are not established.

After the helper returns successfully, the export wrapper conditionally calls an engine-table slot at `+0xA8` (`0x1002644E`) when `[ESI+0x33678]` is non-null. It sets `ECX` from `[engine+0x38A68]` and passes five values from engine offsets `+0x38A6C` through `+0x38A7C`. The slot's purpose and these fields remain unnamed. Later wrapper calls at `0x10023570` and `0x10026D70` are visible but not interpreted by this trace.

At entry to `FUN_10025E00`, the code calls `geEngine_ShutdownDriver` (`0x10025D80`) and aborts if it fails. It then checks `[ESI+0x33668]`; if nonzero, it logs an error and returns failure. The following selected-driver path is visible in raw instructions:

1. At `0x10025EAF–0x10025EC9`, it reads a pointer from `[ESI+0x38904]`, forms another pointer from `[EBX+0x204]`, and calls `FUN_10024880`. On success, the returned handle is stored at `[ESI+0x3366C]`. `FUN_10024880` is also used by enumeration and calls the `LoadLibraryA` import after constructing a DLL path. The exact field names for `[ESI+0x38904]` and `[EBX+0x204]` are not established here.
2. At `0x10025F16`, it pushes the string pointer `0x10053D5C` (`"DriverHook"`) and the loaded handle, then calls the `GetProcAddress` IAT slot at `0x100510A0`. A null result logs failure.
3. At `0x10025F61–0x10025F69`, it passes the address of `[ESI+0x33678]` to the resolved hook. The returned table pointer is read from that out-parameter.
4. At `0x10025F86–0x10025F94`, it requires `[table+4] == 101` and `[table+8] == 1`, the same values checked during temporary enumeration.
5. On accepted version fields, the engine writes Genesis callback pointers `0x10033D60` and `0x103881B0` into table offsets `+0xB0` and `+0xB4` (`0x10025FA4–0x10025FAE`). These slots are modified after `DriverHook` returns; their purpose is not named by this trace.
6. The function copies several caller/engine structures into a stack context and calls the function pointer `[table+0x20]` at `0x10026035`, with `ECX = &stack_context` (`LEA ECX,[ESP+8]` at `0x10026031`). The `+0x20` function's return value gates success. A zero return logs failure; a nonzero return sets `[ESI+0x33668] = 1` and `[ESI+0x389FC] = 1`, calls a print helper, and returns success.

The loader does not call `FreeLibrary` on this successful selected-driver path. The handle remains stored in `[ESI+0x3366C]` for later engine use/shutdown. This contrasts with `FUN_10023950`, which temporarily loads candidates, enumerates metadata, and frees each candidate handle. Thus, the enumerated subdriver/mode records are later used to select a candidate that the engine loads again and keeps while initializing it.

## D3DDrv table returned by `DriverHook`

At `D3DDrv` VA `0x1001F040`, the table bytes begin with the same version values consumed by Genesis, followed by function pointers. The addresses below are read directly from the pinned `.data` bytes; they are table offsets, not recovered source field names.

| Table offset | Stored value | Evidence-level observation |
|---:|---:|---|
| `+0x00` | `0x100185B8` | Pointer-valued first field; not interpreted here. |
| `+0x04` | `101` | Genesis version check. |
| `+0x08` | `1` | Genesis version check. |
| `+0x14` | `0x10004C20` | Called by the enumeration path with callback/context arguments. |
| `+0x18` | `0x10004DA0` | Called from the subdriver callback with a subdriver name and mode callback. |
| `+0x20` | `0x10005DD0` | Called by selected initialization with `ECX` pointing to a stack context. |
| `+0xB0` | zero in static initializer | Overwritten by Genesis with `0x10033D60` on selected load. |
| `+0xB4` | zero in static initializer | Overwritten by Genesis with `0x103881B0` on selected load. |

The table initializer also contains additional code pointers at `+0x1C`, `+0x24`, and later slots; their meanings are outside this trace. `DriverHook` writes the table address `0x1001F040` through its output pointer and returns 1, as documented in [`d3ddrv_driverhook_boundary.md`](d3ddrv_driverhook_boundary.md).

## Observable behavior at selected table slot `+0x20`

The raw body at D3DDrv VA `0x10005DD0` starts by reading three dwords from the context at `ECX + 0x608`, `+0x60C`, and `+0x610`, and calls `0x10005070` with those values and a pointer into the context (`ECX+0x209`). If that helper succeeds and the first two context values equal `-1`, it calls `GetClientRect` through IAT VA `0x10015138`, computes the rectangle's width and height, and writes those results back at context offsets `+0x608` and `+0x60C`. It then copies those two values and the third context dword to globals `0x100422E0`, `0x100422E4`, and `0x100422D8`, and returns 1. The helper-failure path returns 0.

This is sufficient to prove that selected initialization invokes the D3DDrv `+0x20` slot and that this implementation consumes and updates fields in a context before reporting success. The exact field meanings and semantics of helper `0x10005070` are not asserted here.

## Limits and follow-up

- Raw disassembly establishes the EXE callsite, export wrapper, load, `GetProcAddress`, out-pointer, version checks, callback-slot writes, `+0x20` dispatch, success flags, retained module handle, and a later `+0xA8` table call. It does not establish the full source-level signature or names for `FUN_10025E00`, its arguments, structure fields, or the `+0xA8` slot.
- The Ghidra-generated D3DDrv function inventory does not list `0x10004C20`, `0x10004DA0`, or `0x10005DD0` as entries in this saved export; the slot targets and their instructions were verified directly in the binary. Use the hash-matched Ghidra program to confirm boundaries and recover prototypes when its tools are available.
- Next useful static query: trace reads/writes of `[ESI+0x3366C]`, `[ESI+0x33678]`, `[ESI+0x38904]`, `[ESI+0x33668]`, `[ESI+0x389FC]`, then identify the table methods and imported function through xrefs. Do not infer runtime mode selection or renderer parity from this static path alone.
