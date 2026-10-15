# Retail BModel contact-bound construction audit

Updated 2026-10-15. This audit maps the child-model extent reads used during
BModel initialization. It narrows the host comparison but does not establish
bit-for-bit parity for the final contact box.

## Pinned inputs

- `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- `inputs/original/binaries/Genesis.dll`, SHA-256
  `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
- The local GBSP package exporter at
  `tools/export_gbsp_scene.py`.

## Retail path

`InitializeBModelEntity` (`0043C670`) calls `0043B7A0` after it resolves the
child-model pointers stored at root `+0x1C0` and their count at `+0x1C4`.
`0043B7A0` initializes three-component min/max accumulators at root `+0x240`
and `+0x24C`, visits each child pointer, and unions that child's bounds into the
accumulators.

Each child extent is obtained through `004818C0`. The helper copies three
floats from the child model object at `+0x8C` and three from `+0x98`. It adds
the float stored at `0x004E2B88` to each minimum and subtracts it from each
maximum. In the pinned PE image that address contains the bytes `66 6F 72 20`
(`"for "`), which decode as `2.0535056e-19f`. At ordinary BSP coordinate
magnitudes this adjustment rounds away; it is not the 128-unit root-node inset
used separately by the `openaway` direction-bounds path.

After the union, `0043B7A0` calls `004819A0` with the min/max pair and copies a
52-byte bounds structure back to root `+0x240`. The helper computes a midpoint
and endpoint-relative vectors before copying that structure. The original
union min/max are also retained at root `+0x278` and `+0x284`; a separate
32-unit adjustment is applied to the structure at root `+0x260`. The contact
callback `0043B620` passes root `+0x240` to overlap helper `004817A0`.

The saved bounded disassembly excerpts are in
[`bmodel_contact_bound_construction_2026-10-15.asm`](bmodel_contact_bound_construction_2026-10-15.asm).
They include the initializer and its vector helpers, `004818C0`, `004819A0`,
`004817A0`, and the separate `0043B470` adjustment. `0043B620`'s fresh
pseudocode remains in
[`0043b620_bmodel_contact_candidate_fresh.c`](0043b620_bmodel_contact_candidate_fresh.c).

## Host comparison and limit

`tools/export_gbsp_scene.py::decode_models` reads each 0x50-byte GBSP `MODELS`
record and emits its raw `mins`, `maxs`, and `origin`; `decode_collision`
copies those fields into `collision.json`. The host loader
[`scene_viewer.cpp`](../../../port/spikes/training-native-prototype/src/scene_viewer.cpp)
resolves each BModel child through `%Model%`, then unions the joined collision
model's exported `mins` and `maxs` into `PreviewBModelMotion::boundsMin` and
`boundsMax`.

This matches the asset-bound inputs before the retail `004819A0` conversion,
assuming the Genesis runtime model extent fields retain the corresponding
GBSP `MODELS` values. The exporter and loader establish that data lineage; no
live Genesis model read has verified those runtime fields. The final
`004819A0` structure semantics also remain incomplete, so using the raw union
as the host contact AABB is still an approximation. No host code change is
justified from this trace alone. A follow-up should finish the structure-field
mapping and compare the retail runtime extents or a captured contact boundary
before changing the host box.
