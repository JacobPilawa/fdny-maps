# Training door bounds loader audit

Read-only Ghidra 12.0.4 trace of the preserved `FDNYFirefighter.exe` and `Genesis.dll`, checked against the extracted Training BSP on 2026-10-11. The preserved databases were not modified; queries used scratch projects. No live gameplay capture was used.

## Verified data path

- Genesis loader `FUN_100319D0` distinguishes BSP chunk 1 (`MODELS`, element size `0x50`) from chunk 2 (`NODES`, element size `0x2c`). It stores the model-record array at GBSP base `+0x110` and node-record array at `+0x114`; counts are at `+0x16c` and `+0x170` respectively.
- `FUN_1003BBA0` iterates the serialized model records, reads each record's first dword as its root-node index, and selects the corresponding `0x2c` node record. It reads that node's six bounds floats at `+0x14..+0x28`. For each `0xd4` runtime WorldModel, it writes those six values to `+0x74..+0x88` and duplicates them at `+0x8c..+0xa0`. The runtime center at `+0xb0..+0xb8` is computed from the min/max pairs. Separately, it copies the serialized GFX_Model origin at `+0x20..+0x28` to runtime `+0xa4..+0xac`.
- EXE `0043B7A0` passes each BModel child WorldModel pointer to `004818C0`. The helper reads that model's `+0x8c..+0xa0` values, adds scalar `004E2928` to the three mins, and subtracts it from the three maxs. The scalar is `128.0f`. BModel then unions the contracted child bounds.
- The asset extractor `map_explorer/source/tools/extract_gbsp.py::parse_models` reads `collision.json` model mins/maxs from the serialized `0x50` GFX_Model record at `+0x08..+0x1c`; it reads the origin at `+0x20..+0x28`. Therefore `collision.json` mins/maxs are not the direct runtime operands to `004818C0`; those operands come from the root-node `0x2c` record.

## Training door comparison

For all 20 Training `*Door` models, the root-node bounds are the GFX_Model bounds padded by approximately 128 on each side. Applying the source's float32 `+128/-128` operations to the node bounds reproduces the extracted GFX_Model mins/maxs exactly for 14 doors. The other six (entity IDs 198–203; model IDs 71–66) differ only in X-min and Z-min by `-0.00048828125`, consistent with the serialized node-bound rounding. Their other four components match exactly.

This corrects the earlier assumption that the source contracts the already-extracted GFX_Model bounds: doing that would apply the 128-unit contraction twice and collapse the thin door bounds. For exact source inputs, retain each model's root-node bounds and apply the source float32 arithmetic. The native door helper now follows those verified operands, and `--door-asset-check` asserts the 14 exact matches plus the six known rounding cases.

## Ghidra queries

- Genesis: `DecompileGenesis.java 100319D0 10032400 1003B8C0 1003BBA0 1003C0A0`.
- EXE: `DecompileFunctions.java 0043B7A0 004818C0`; `AddressXrefs.java 004818C0`.
- BSP field mapping: `map_explorer/source/tools/extract_gbsp.py::parse_models`, compared with the preserved `decomp/evidence/original/training.bsp`. Float32 arithmetic comparison used the serialized model and root-node records, not a gameplay run.
