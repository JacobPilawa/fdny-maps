# Training zone bounds: Ghidra trace

Read-only Ghidra 12.0.4 trace of the preserved `FDNYFirefighter.exe` project, cross-checked against the extracted Training BSP on 2026-10-11. The Ghidra database was not modified. No runtime capture was used.

## Source path

- `00441040` (BoundsTrigger initialization) reads a nonzero pointer from `this+0xFC`'s `+8` field, then calls `004818C0` with the trigger bounds at `this+0xE4` as destination and that pointer as source. The zero-model branch instead copies `this+0xC8/+0xCC/+0xD0` into both min and max bounds, producing zero volume.
- `004818C0` copies six floats from source offsets `+0x8C..+0xA0`, adds `004E2928` to the three minima and subtracts it from the three maxima. The preserved Ghidra data item at `004E2928` is `128.0f`.
- `004818A0` computes `(maxX-minX)*(maxY-minY)*(maxZ-minZ)`; the initializer uses it for the zero-volume diagnostic. `00481840` checks a point against all six bounds inclusively.

This establishes the transform from the runtime model pointer in the BoundsTrigger user-data record to its stored trigger box. The Genesis entity parser also establishes the authored `C_Model` → typed user-data model pointer conversion described below. What remains unproven is the runtime identity of each parsed Training entity instance (entity ID to specific `geEntity`/actor), because no live capture or per-instance trace was used.

## Authored model to user-data pointer

- The raw Training BSP's `RCS_BoundsTrigger` typedef has `C_Model` of type `model` at byte offset `+0x08` (68-byte user-data layout). This comes from `tools/extract_gbsp_typedefs.py` reading `evidence/original/training.bsp`, not from matching bounds.
- Genesis `FUN_100289B0` opens/parses an entity-set stream (`FUN_100286B0`), builds the type/class descriptors (`FUN_10027DD0`), then initializes per-entity user-data (`FUN_10028510`). The latter allocates and zeroes a buffer of the class descriptor's recorded size, stores it as the entity-node user-data buffer, and calls `FUN_10028280` for authored fields.
- In `FUN_10027DD0`, the built-in type descriptor initialized at `10027F5E..10027F71` uses the string at `10054868` (`model`) and type ID `5`. In `FUN_10028280`, case `5` looks up the authored model name with `FUN_10027C90`, reads its numeric `Model` property with `FUN_10027910`, and stores `world.ModelArray + index * 0xD4` at `userData + fieldOffset`. The class descriptor lookup supplies `C_Model`'s `+0x08` offset from the typedef, so this parser path writes the runtime model pointer at user-data `+8`.
- Genesis `geEntity_GetUserData` (`100278B0`) returns `geEntity + 8`; game code stores that result in the BoundsTrigger object at `+0xFC`, and `00441040` reads its `+8` model pointer. Thus the typed field conversion and the consumer's expected offset agree at the code level.

The code proves the generic loader conversion and consumer layout. It does not prove a particular Training BSP entity ID becomes a particular live `geEntity` instance: the parser's entity-set records are not keyed to an observed runtime actor in this audit.

## Training data cross-check

For the five extracted entities whose `S_Script01` is `makezone`, the BSP model's root-node bounds contracted by the Ghidra transform match both the entity JSON `bounds` and extracted `collision.json` model extents exactly (all values are integral binary32 values here):

| Entity | Model index | BSP root node | Contracted min | Contracted max |
|---:|---:|---:|---|---|
| 114 | 47 | 2669 | `(-476, -580, -1096)` | `(760, 348, 848)` |
| 135 | 51 | 2693 | `(-1140, -580, 868)` | `(96, 348, 2812)` |
| 158 | 58 | 2735 | `(264, -580, -3060)` | `(1500, 348, -1116)` |
| 190 | 73 | 2825 | `(904, -580, -5024)` | `(2140, 348, -3076)` |
| 234 | 82 | 2879 | `(-1652, -580, 2832)` | `(-416, 348, 4776)` |

The source root-node inputs for each row were read from the serialized Training BSP `NODES` records at `+0x14..+0x28`, using the root index in the corresponding 0x50-byte `MODELS` record. The extractor reads the comparison extents from the model record; as documented in [`training-door-bounds-loader-audit.md`](training-door-bounds-loader-audit.md), those serialized model extents are not the direct runtime inputs to `004818C0`.

## Queries and limits

- Preserved EXE project, query-only: `DecompileFunctions.java 00441040 004818C0 004818A0 00481840`; `AddressXrefs.java 00441040 004818C0 004818A0`; `DataValue.java 004E2928`.
- Preserved Genesis.dll project, queried read-only from a scratch copy: `DecompileFunctions.java 100289B0 10027DD0 10028510 10028280 10027C90 10027910 100278B0`; `AddressXrefs.java 10028510 10028280 10027C90 10027910`; instruction/string checks at `10027F5E..10027F71` / `10054868`.
- Extracted data: `decomp/assets/training/entities.json`, `decomp/assets/training/collision.json`; raw source: `decomp/evidence/original/training.bsp`; model and node field offsets independently described by `map_explorer/source/tools/extract_gbsp.py::parse_models` and [`training-door-bounds-loader-audit.md`](training-door-bounds-loader-audit.md).
- Exact data equality and the generic authored-field-to-runtime-model-pointer conversion are confirmed. Per-trigger runtime parity still requires identifying the loaded Training entity-set instances (or a bounded live capture) so the extracted entity IDs can be tied to the exact runtime `geEntity` records.
