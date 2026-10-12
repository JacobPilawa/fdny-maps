# D3DDrv `DriverHook` boundary

## Pinned binary and export

Input: [`D3DDrv.dll`](../../../inputs/original/binaries/D3DDrv.dll), SHA-256
`7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7`.

The file is a 32-bit x86 PE DLL with image base `0x10000000` and image size
`0x00532000`. Its export table contains one named export: ordinal 1,
`DriverHook`, RVA `0x6300` (VA `0x10006300`). Its import table includes
`DirectDrawCreate` and `DirectDrawEnumerateA` from `DDRAW.dll`; it also imports
Windows APIs. Inventories are preserved in [`exports.csv`](../../exports.csv)
and [`imports.csv`](../../imports.csv).

## Raw entry-point behavior

Raw x86 disassembly shows `DriverHook` receives a pointer at `[esp+4]` and:

1. stores `3` at `0x100422C8` and `0` at `0x100422CC`;
2. stores the address `0x100422C8` at `0x1001F0EC`;
3. writes `0x1001F040` through the supplied pointer;
4. stores `0x100421B0` at `0x1001F050`;
5. calls `0x10006020` with a null first argument and a pointer to the string
   `D3DDrv:  No error.`; and
6. returns integer `1`.

At `0x10006020`, the second argument is copied to the fixed buffer at
`0x100421B0`. The helper then tail-calls `0x10002040` with format string
`\n!! ERROR: %s\n` and that buffer. The strings and instructions establish a
formatted logging path. The entry's pointer writes establish an initialization
boundary, but do not identify the pointed-to structure fields or their
semantics.

Relevant raw instructions:

```text
10006300: 8b 44 24 04                   movl 4(%esp), %eax
10006304: c7 05 c8 22 04 10 03 00 00 00 movl $3, 0x100422c8
1000630e: c7 05 cc 22 04 10 00 00 00 00 movl $0, 0x100422cc
10006318: c7 05 ec f0 01 10 c8 22 04 10 movl $0x100422c8, 0x1001f0ec
10006322: 68 50 86 01 10                pushl $0x10018650
10006327: c7 00 40 f0 01 10             movl $0x1001f040, (%eax)
1000632d: 6a 00                         pushl $0
1000632f: c7 05 50 f0 01 10 b0 21 04 10 movl $0x100421b0, 0x1001f050
10006339: e8 e2 fc ff ff                calll 0x10006020
1000633e: 83 c4 08                      addl $8, %esp
10006341: b8 01 00 00 00                movl $1, %eax
10006346: c3                            retl
```

## Limits and next step

This is a raw-disassembly trace, not a fresh Ghidra function review. It does
not establish that the DLL matches any public Genesis3D driver source. The
public source tree is a comparison lead only; see
[`ENGINE_SOURCE_REUSE.md`](../../../docs/ENGINE_SOURCE_REUSE.md).

Next, identify the caller that invokes `DriverHook`, determine the layout
consumed through its output pointer, and trace which driver-table entries the
engine calls. Keep names provisional until those callsites or matching
structure definitions confirm them.
