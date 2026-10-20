# Retail BModel contact-bound construction audit

Updated 2026-10-15; root-offset and update-path corrections added 2026-10-20.
This audit maps the child-model extent reads used during BModel initialization
and movement. It narrows the host comparison but does not establish bit-for-bit
runtime parity.

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

After the union, `0043B7A0` calls `004819A0` with the min/max pair and copies
the returned 52-byte structure to root `+0xF0`. The helper computes a midpoint
and endpoint-relative vectors. The raw union remains at root `+0x240` and
`+0x24C`. The union is also copied into a secondary six-float box at
`+0x278..+0x28C`; `0043B470`, called with root `+0x260`, subtracts 32 from its
three minima and adds 32 to its three maxima. The contact callback `0043B620`
passes root `+0x240` directly to overlap helper `004817A0`; it reads the six
raw min/max floats there, so neither the `004819A0` conversion nor the separate
32-unit-expanded copy forms this callback's contact bounds.

The correction comes from rechecking the AT&T displacements in the pinned
assembly: they are decimal, so `lea 240(%ebp)` is root `+0xF0`, while
`lea 576(%ebp)` and `lea 588(%ebp)` are root `+0x240` and `+0x24C`. The
initializer also runs through `0043DC60`'s embedded ScriptActor path, which
normalizes its `root+0x24` pointer back to the BModel root before the call.

The current-hash `0043DC60` update samples each child motion path and writes
its model transform before checking parsed BModel UserData `+0x38`
(`G_Liftable`). When that value is nonzero, callsite `0043E188` invokes
`0043B7A0` with `this-0x24`, refreshing the raw child-bound union before the
timed-command loop. `0043BAA0` also writes each child's requested translation
as target center minus that child's rotational center, optionally stores the
new owner center, then calls `0043B7A0`; its callers include the trajectory
update `0043D330` and center correction `0043BD90`. Thus the retail liftable
path recomputes bounds after child pose changes. A fresh direct-reference and
offset-use audit did not identify a BModel-root consumer for the converted
`+0xF0..+0x120` structure or the 32-unit-expanded `+0x278..+0x28C` copy;
matching numeric offsets in Player and other functions use different bases.

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

This matches the asset-bound inputs for the contact callback, assuming the
Genesis runtime model extent fields retain the corresponding GBSP `MODELS`
values. The exporter and loader establish that data lineage; no live Genesis
model read has verified those runtime fields. The shared host contact pass now
recomputes bounds from current child poses for nonzero `G_Liftable` entities,
reusing its eight-corner world-bound adapter; static bounds remain for other
BModels. This follows the retail refresh gate but approximates Genesis runtime
extents, whose exact update behavior is not live-verified. The host still omits
the tiny per-child adjustment from `004818C0` (approximately `2.0535056e-19`),
whose effect can differ at zero-valued bounds. The converted
`+0xF0..+0x120` structure and expanded `+0x278..+0x28C` copy have no verified
consumer yet. Treat the lifted contact-box port as source-shaped, with runtime
extent parity and callback cadence provisional.
