# Legacy map explorer: reference-only audit

Updated: 2026-10-13

## Decision

`/Users/jacobpilawa/Documents/ChatGPT/FDNY_trackdown/map_explorer/` is a useful
comparison viewer for geometry, materials, authored map objects, and individual
BSP motion tracks. It is reference material only. Its viewer code, texture
filters, motion controls, and collision overlays are not the implementation of
the port and are not evidence of retail runtime behavior.

The port remains grounded in hash-pinned FDNY executable, Genesis.dll,
D3DDrv.dll, and original map data analysis. Relevant current traces include
[`genesis_render_asset_path.md`](../tracker/evidence/runtime/genesis_render_asset_path.md),
[`d3ddrv_render_path.md`](../tracker/evidence/runtime/d3ddrv_render_path.md),
[`bmodel_update.md`](../tracker/evidence/scenario-loading/bmodel_update.md),
and [`subway_a_interaction_path.md`](../tracker/evidence/scenario-loading/subway_a_interaction_path.md).

## Data identity

SHA-256 comparison found all 17 explorer BSPs byte-identical to the copied
`inputs/original/Data/Maps/` BSPs in this worktree. This makes its render and
inspection output a useful same-input comparison. The hashes below also make
the comparison reproducible without relying on the explorer at runtime.

| BSP | SHA-256 |
|---|---|
| `City_h.bsp` | `9bc788dcaceb477fea52efd49cfbb2fd8f86824907d73c819280710b04fcb27a` |
| `bank.bsp` | `02a4b351279a9a4a1242050d3a7622f8d10149201646f890f310729a189d27b7` |
| `bar_r.bsp` | `1a796f4dfbf0d04bedf7e35ebdcf82e2e726940bf69b1df7ce2ea7428e018a98` |
| `boat_a.bsp` | `e1685b45ef29aa075c688985617b593079a6aad8a6b493bbae327356bd45ad44` |
| `boat_b.bsp` | `4ad7de96916b5c265a4cdd6c477d96a0b6f9cc6e8151c96ff2e1ef8a31313a50` |
| `const_h.bsp` | `f4dc99b6dd3b4465c1ba68cc29c09c075368f45f744a32a470b0663251862984` |
| `dock_h.bsp` | `17427ebd2964f31b47317e53c0881af41a05e78d57a7c95ef9783d10697a6d7c` |
| `gas_r.bsp` | `f6822f8d598d2888896165bbee8b554351230cd76ad6331abf0d735a03b54cbc` |
| `mbase.bsp` | `4ff3b18dd809c619a6734673a7f90bd2e98d9fc9518ee7bf94ece4b6ff90b266` |
| `oil_rig_A.bsp` | `c63ff7e6a7e41a6df15f572bc1c327cbc6ecbe934a17cf1c5a136c2a7d7e36c1` |
| `oil_rig_B.bsp` | `bfc16ba4e151a814ba2f80d84a3aadb096d9468c83262daab900c48e5043c701` |
| `oil_rig_C.bsp` | `786aeb2a56929f1ac8270cd3be16608aec5a6ae9677c477209dbd2cc0721f499` |
| `oil_rig_D.bsp` | `f74675df4da49c2723473b6c9d13faca698824d9bc104262f788394d7f86914d` |
| `oil_rig_h.bsp` | `6681a6ecb688f69b37bbbc25c6c0dea3f030f33880e7210c7cbed48a108bd4d0` |
| `subway_a.bsp` | `b33b67a01e0a2c144033f1da5c9c627b197d0a413b3b13190dd8531b63881f7e` |
| `subway_b.bsp` | `279ef316b08c0eae5d2ae33811402cf754d7e1aef95c2af199a83b87ff99c9e4` |
| `training.bsp` | `32986212fa875764ca3d5c1630e54ad6315abaea70983203601192cec603a963` |

## What the comparison can tell us

- Whether the port's parsed BSP vertex positions and static scene layout agree
  with an independent viewer on the same files.
- Whether a face is associated with the expected embedded bitmap and whether
  its UV placement looks consistent. Both texture builders use the Genesis
  dot-product / DrawScale / Shift / bitmap-size mapping with a flipped V axis;
  the legacy builder rounds its UV output to six decimal places.
- Whether authored entity locations, model bounds, trigger boxes, and decoded
  individual motion endpoints appear in the expected places for inspection.
- Which camera location and specific face or object show a visible mismatch,
  so the corresponding parser, engine path, or port code can be investigated.

## What it cannot establish

The old texture builder omits `sky_*`, `col_*`, and names containing
`trigger`, `clip`, `hint`, or `null`; it also omits faces belonging to models
joined from `RCS_BoundsTrigger` entities. These are choices made by that
inspection renderer. They do not prove that the retail renderer suppresses the
same faces, or that all such surfaces are non-rendering in every state.

The explorer previews authored BSP motion tracks as inspection clips. It does
not run the retail EVS command interpreter, ScriptActor contact/update
scheduler, conditional command flow, or the full moving-model collision path.
For example, the retail BModel path advances frame state and calls
`geWorld_SetModelXForm`; Subway A doors additionally depend on shared
`touch`/`untouch` event dispatch, time scale, sound, and three child motions.
Those must be ported from the pinned runtime traces, not inferred from the
explorer's playback controls.

Likewise, the explorer's collision overlay is a helpful map-inspection view,
not a replacement for the shipped Genesis collision routines or the port's
dynamic collision behavior. A visual match is a lead for investigation, not a
mechanics-parity result.

## Provenance and self-contained worktree

The explorer viewer, its controls/filters, and generated scene output are not
part of the native port. One inherited asset-inspection tool needs a more
careful provenance note: the active `tools/extract_act.py` is byte-identical
to the copy in the previous `FDNY_trackdown/decomp` archive and has 99.8%
line-level similarity to `map_explorer/source/tools/extract_act.py`; the
observed code difference is preserving the serialized `bitmapId` field. The
old `map_explorer/source/build_actor_assets.py` wrapper is not used here, but
the actor exporter imports this near-identical ACT decoder. Treat its decoded
actor files as provisional inspection data, not as an independently derived
port or proof of runtime behavior. The ACT layout and decoder must be
independently checked against the hash-pinned retail Genesis.dll and raw ACT
inputs before the actor asset path is considered source-backed. No new
map_explorer code or generated explorer scene data is being used by the port.

The current worktree contains the original BSP inputs and hash-pinned
parser/renderer evidence needed to compare scenes without depending on the
explorer at runtime. For provenance, the inspected legacy helper fingerprints
were:

| Reference file | SHA-256 |
|---|---|
| `source/build_textures.py` | `7ff5d90a0b3a51a380c2bb21cdb3f7363c0e6e9777fe4d833c7e46d0dfb58762` |
| `source/feature_data.py` | `9edde2d3957269dbbb621866dba8ed23aef1fd6992f2e9501408832d4d91e36f` |
| `textured-world.js` | `a3f33c03ee39d4e45eb7b7972cc09ab6e4f45e1c1ee6253b3238edf76b743163` |

## Port follow-through

1. Use the same-hash explorer view to localize visual discrepancies, recording
   the map, camera position, and affected surface or model.
2. Trace each discrepancy to the original BSP fields and, where relevant, the
   pinned Genesis/D3D renderer before changing the exporter or native renderer.
3. For moving models, port the decompiled command state, motion sampling,
   contact/update scheduling, render transform, and collision transform as
   separate behaviors with explicit evidence and remaining unknowns.
4. Keep explorer-only filters and guessed interaction behavior out of the
   retail compatibility path unless independent retail evidence supports them.
