# Level catalog collection owner resolution

## Provenance

Read-only raw x86 inspection of the pinned `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Ghidra MCP pseudocode for the same hash is preserved in the adjacent evidence files. No game runtime was used.

## Population edge

The catalog parser does populate the collection read by the level selector:

1. In `0042B4E0`, startup passes `ESI+0x6AC` in ECX to parser `00423160` at callsite `0042B761` (`lea ecx,[esi+0x6ac]`). The subsystem constructor `0042B860` initializes that embedded collection at `+0x6AC` and stores the subsystem base in `DAT_00502AAC`.
2. Parser prologue instruction `0042317E` saves its incoming ECX at `[ESP+0x2C]`. Instruction `004231D1` then pushes EBP, shifting the stack by four bytes. At the append site, `004233D8` reloads `[ESP+0x30]`, which is the saved original ECX; `004233DD` adds four, and `004233E2` calls `0048D9D0`.
3. The append receiver is therefore `subsystem+0x6B0`: the array-pointer field at offset `+4` inside the collection whose base is `subsystem+0x6AC`. This is the same field layout initialized by `004230E0` and read by selector lookup `004225A0`.
4. Selection handler `0042F500` calls `004225A0` with `DAT_00502AAC+0x6AC`. The lookup reads array pointer `this+4` and count `this+8`, comparing each record's ordinal at `+0x18`. It passes the selected record directly to level startup `0042AB00`.

This resolves the parser-to-selector collection identity from the actual x86 stack path, not from a record-layout resemblance. Confidence: high.

## Selector ordinal path

Parser `00423160` starts its level-record ordinal at zero and increments it after each `level:` record. The selector's `+0x2C` field is used as the lookup key by `0042F500`. In the adjacent selector update routine, raw instructions at `0042F2A1`–`0042F2D4` pass an input ordinal to `004225A0`, load the matched record's `+0x18` ordinal, and write it back to the selector object's `+0x2C`. The `0042F500` branches also adjust that field for navigation before refreshing the selection. This supports a direct ordinal-indexed menu model in catalog order.

The constructor or menu-opening path that seeds the initial `+0x2C` value has not been identified. Exact meanings of event codes and menu presentation timing also remain open.

## Evidence

- Parser and subsystem startup: `../runtime-loop/0042b4e0_subsystem_startup.c`, `../runtime-loop/0042b860_subsystem_constructor.c`, and `00423160_level_manifest_parser.c`.
- Collection constructor and indexed lookup: `004230e0_level_collection_constructor.c`, `004225a0_level_record_lookup.c`.
- Selection handler: `0042f500_level_selection_load.c`.
- Generic append helper: `0048d9d0_array_slot_append.c`.
- Earlier xrefs and incomplete P-code dataflow output (superseded by this raw-instruction stack proof): `startup_xref_summary.txt`.
