# Shared PropModel actor and contact port

Date: 2026-10-14

## Verified retail behavior

The retail source trace is pinned to
[`FDNYFirefighter.exe`](../../../inputs/reference/raw-copy/FDNYFirefighter.exe),
SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The shared contact path is detailed in
[`retail_propmodel_contact_trace.md`](retail_propmodel_contact_trace.md):

- PropModel setup `00443860` loads the authored `D_Model`, parents the ACT
  frame under the PropModel frame, applies authored heading/pitch, and begins
  with unit scale. `setscale` replaces the three stored scale values.
- Contact producer `00443700` checks the ACT model's enable byte, obtains the
  candidate actor's world point through root slot `+0x0C`, compares squared
  distance to the authored PropModel radius, then gates the candidate with
  `geWorld_GetLeaf` / `geWorld_MightSeeLeaf` at the PropModel center.
- PropModel root slot `+0x14` enters `00470740`, drains contact events through
  `00444050` and `00470840`, then copies and clears the pair list. `00470840`
  dispatches all new `touch` pairs, then continuing `touching` pairs, then
  removed `untouch` pairs.
- `show` and `hide` call `00497DE0`, which writes the same model enable byte
  used by the contact producer. Setup initializes that byte to 1.
- Training entity 161 `ext1` maps to ACT resource 2, `ext.act`, at
  `(780,-164,-1748)`, touch radius 70, and `touch:*RefillExt`. Its macro
  equips/refills the extinguisher and plays `Steam4`; the entity has no `E_Use`.

The PropModel contact producer and drain are now identified. The external
caller of root slot `+0x14`, its ordering against the contact manager, and its
wall-clock cadence remain unresolved.

## Shared port changes

The ACT exporter resolves every PropModel `D_Model` referenced by the 13
rescue/stage scene packages. The current catalog contains 15 distinct ACT
resources and 22 embedded bitmap entries, including `ext.act`, the coiled hose,
the valve, and rescue-map collectibles. The general ACT exporter writes a
shared `props_manifest.json`, bind-pose OBJ/JSON, inspection PNG/BMP, and RGBA
texture payloads. All generated assets remain under `.local/actor_exports`.

The ACT frame uses the source-derived heading expression
`-heading * 30 - 90` degrees and pitch `-pitch * 90` degrees, applies the
authored location, and uses replacement scale. The renderer uses cached
bind-pose vertex arrays for world props. It does not claim retail lighting,
pose animation, exact x87 trigonometric residue, or texture blend/depth parity.

The contact path compares the viewer Player point with the authored PropModel
center/radius, retains per-entity contact state, and dispatches event groups in
retail's `touch`, `touching`, `untouch` category order. `show`/`hide` updates
both render visibility and contact eligibility. Training's `ext1` therefore
reaches the authored refill macro when the player enters its radius, subject to
the current viewer event cadence.

The current Player point is `Player::p`, not a live capture of the frame point
returned by retail's Player slot `+0x0C`. The port now resolves the Player and
PropModel-center leaves through the world BSP and applies the lower-level
cluster-PVS bit rejection from `geWorld_LeafMightSeeLeaf`. This is a
source-backed rejection when the PVS bit is clear. A positive result remains
permissive because the wrapper's current-view semantics and runtime area
visibility matrix are unresolved. Invalid leaves, unclustered leaves, missing
chunks, and invalid offsets fail open to preserve contacts. The port also scans
and advances PropModel script actors on the viewer frame loop because the
retail root `+0x14` updater's external cadence is not known. These differences
keep contact timing and exact eligibility provisional.

The raw GBSP `CLUSTERS` and `VISDATA` payloads are retained in each exported
scene at `chunks/05_clusters.bin` and `chunks/21_visdata.bin`. For every
packaged rescue map, cluster records contain the VISDATA byte offsets and the
visibility payload sizes match the authored cluster count and padded bitset
stride. `PropModelPvs` reads these chunks directly and classifies points using
the world collision BSP's leaf cluster IDs; it does not need a separate,
lossy visibility export.

The hash-pinned `inputs/reference/raw-copy/Genesis.dll`
(`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`) trace
maps `geWorld_GetLeaf` at `0x1003C420`, `geWorld_MightSeeLeaf` at
`0x1003C450`, and the pairwise helper `geWorld_LeafMightSeeLeaf` at
`0x1003C470`. Leaf records are 60 bytes with cluster at `+0x2C` and area at
`+0x30`. The pairwise helper returns visible when either cluster is `-1` or
the source cluster's VISDATA offset is `-1`; otherwise it checks the target
cluster bit. After a positive cluster result, it also checks the world area
matrix at `world+0x1E4 + areaA*0x100 + areaB`.

The current port implements the source-backed cluster-bit rejection and
returns visible for unclustered leaves, missing chunks, or invalid offsets.
This does not synthesize the unresolved runtime area matrix. The actual
`geWorld_MightSeeLeaf` wrapper consumes current world visibility state; using
the Player movement point as the source leaf approximates that state until the
retail view-leaf update and the Player slot `+0x0C` point are captured. A clear
cluster PVS bit can reject contact, while a set bit is not sufficient to claim
the retail contact would be admitted. To keep the contact pass inexpensive,
the viewer refreshes the Player leaf once per frame and caches each static
PropModel center leaf at scene load.

## GeBm decode coverage

The copied ACT inputs contain indexed format 1 with palette flag `0x09`, and
24-bit BGR format 10 with image flags `0x01`, `0x03`, and `0x22`. The latter
include a color-key field for flags with bit 1 set. Genesis.dll's
hash-pinned Ghidra trace of `geBitmap_CreateFromFile` / `0x10017870` shows that
the helper reads the key using `gePixelFormat_BytesPerPel(format)` and
`gePixelFormat_GetPixel`; per-level data follows a zero marker and the retail
base-level terminator is `0x0f`. The exporter now decodes these referenced
formats and passes alpha through its `.rgba` payload to OpenGL. The detailed
engine trace and retained instruction excerpt are in
[`genesis_render_asset_path.md`](../runtime/genesis_render_asset_path.md).

Alpha sidecar bitmaps, compressed palettes, Paeth-filtered levels, and mip
chains above level 0 are not yet supported. Retail handling of keyed texture
depth/blend behavior remains unverified.

## Verification and handoff

- `tools/build_scene_viewer_app.sh rescue-all` regenerated all actor assets,
  built the C++ viewer, and packaged all 13 rescue/stage scene packages.
- The Training `--render-check` uploaded 24/24 BSP textures without failures.
- The rescue catalog check loaded all 8 retail rescue choices and their 13
  scene/stage packages after the PVS integration.
- Training `--tool-orientation-check extinguisher` drew the exported ACT's 313
  triangles and generated the camera-root/half-turn comparison captures.
- These checks verify asset export, renderer initialization, and the packaged
  C++ path. They do not verify entering `ext1` in a live game session or
  same-view retail parity.

The user reports Bank framerate now seems sorted. The current source still has
no measured same-route retail FPS or frame-pacing comparison; the delta writer
and presentation limit remain open in
[`retail_frame_pacing_audit_2026-10-14.md`](../runtime/retail_frame_pacing_audit_2026-10-14.md).
