# Bank start-view opaque surface audit

## User report

The user reports that Bank should start open, but the interactive preview shows
an opaque enclosure that looks like a rendered trigger volume. The user suspects
transparency/material handling and says the view feels like being trapped in a
box. The latest follow-up confirms the entrance is expected to be open. Keep
this report open until the exact interactive surface and launched package are
identified; do not assume the visible box is either BoundsTrigger model solely
from its appearance.

## Renderer update (2026-10-13)

The previous source filtered every BoundsTrigger model from drawing, including
Bank models 300 and 301. That class/model visibility override has been removed
from both the map-neutral viewer and the older Training host. Trigger meshes
now pass through the ordinary scene renderer; they remain excluded from
player-solid collision while contact handling uses their authored bounds. The
source has not been rebuilt or visually checked after this edit, so confirm the
result with a fresh launch. Face-level TexInfo alpha/flags and lightmap
references are still not honored by the renderer; correct visual material
behavior remains open.

## Package and trigger joins

The current Bank export is `bank.bsp`, SHA-256
`02a4b351279a9a4a1242050d3a7622f8d10149201646f890f310729a189d27b7`. Its
PlayerRescue entity 334 starts at `(-697, 160, 1196)` with authored heading 0.
The nearby green trigger brushes are:

| Entity | Authored trigger | BSP model | Texture | Exported TexInfo alpha |
| --- | --- | ---: | --- | ---: |
| 466 | `lbbyzone` | 300 | `obj_suvgreen` | 255 |
| 467 | `bankzone` | 301 | `obj_suvgreen` | 255 |

Both `C_Model` joins resolve through the `%Model%` records. The original audit
found that the viewer suppressed both models at render time; that behavior has
since been removed as described above. They remain available as authored AABB
contacts. Confirm whether the opaque enclosure is one of these green
`obj_suvgreen` meshes or a different rendered group, using the exact fresh
launch and package path before changing material handling.

## Candidate ordinary world surface

A static ray audit from the start eye position `(-697, 230, 1196)`, assuming the
preview's heading-0 view points along `-Z`, hits ordinary world model 0 before
either trigger brush. The first hit is 405 units away: group `wal_Stco_wall`,
face 4947, TexInfo 2107. The copied `geometry.json` records TexInfo flags
`32800` (`0x8020`), alpha `255`, and face `light_ofs = -1`. The next reported
hit is model 0 `asr_marbl05`, face 5927, at 428 units; its TexInfo alpha is also
255. I verified these face and material records in the copied Bank export.

This ray result is a diagnostic inference from the exported BSP triangles, not
a pixel-to-face match against the user's window. It means a wall-looking opaque
enclosure is not explained by that face having transparency enabled. A
source audit confirms the preview converts `C_Heading` to radians and applies
inverse yaw to the view, so heading 0 points along `-Z`, matching the ray
assumption. The retail meaning of `C_Heading` remains unverified.

At the time of the original audit, the viewer dropped face-level TexInfo
alpha/flags and lightmap references from its renderer batches. The metadata
transport checkpoint below closes that loss in the export/loader path, while
the draw loop still uses opaque RGB base textures. Material parity remains
open, but the present wall candidate has alpha 255, so an alpha fix alone
would not make it transparent.

## Face metadata transport checkpoint (2026-10-13)

`tools/export_gbsp_scene.py` now emits an ordered `face_spans` array on every
`world_mesh.json` group. Each span maps the source face ID to the first emitted
fan triangle and triangle count, and retains the source TEXINFO ID, flags,
alpha, face-light, reflective scale, mip bias, and FACES light offset,
dimensions, and four light-type bytes. Export-time validation checks each
span against `geometry.json`, verifies every model face is covered once, and
checks triangle/UV counts. The C++ `SceneMesh` loader retains the same values
and rejects malformed, noncontiguous, incomplete spans. The draw loop still
uses only positions, UVs, and the base texture, so this is a data-path change;
it does not claim or alter alpha, blend, depth, or lightmap behavior.

The retail material follow-up found no live Ghidra instance. Pinned project
evidence confirms the Bank ray candidate face 4947 is model 0, TexInfo 2107,
flags `0x8020`, alpha 255, and no lightmap (`light_ofs = -1`); that remains a
ray-based candidate rather than a verified pixel match. Public Genesis source
is comparative evidence only and does not settle the shipped renderer's
alpha, blending, depth, or lightmap semantics. The all-map metadata audit is
complete: all 17 BSP reports match their copied input SHA-256s; 211,090 faces
map to 211,090 face spans across 2,889 render groups and 578,269 triangles.
The C++ `--inspect` loader reported one span per face for all 17 maps and zero
texture decode failures. Bank face 4947 retained TexInfo 2107, flags `0x8020`,
alpha 255, and `light_ofs = -1`. A fresh Bank `--render-check` produced a
byte-identical BMP to the pre-change frame, so the metadata path did not alter
rendering. Remaining work is retail render-path tracing and matching a fresh
Bank view to an exact group/face.

## Current render and package audit

The viewer preserves each mesh's BSP model ID through `world_mesh` loading and
batching. The original audit found a `renderScene()` filter that skipped
BoundsTrigger models 300 and 301 based on their `C_Model` joins. That visual
filter has since been removed, along with the separate `col_*` texture-prefix
draw filter. A source audit confirms both now use the ordinary mesh render
path. Trigger brush collision exclusion remains separate. The next fresh render
should show whether either mesh accounts for the user's reported enclosure.

The existing offscreen `--render-check` frame
(`.local/exports/bank.render-check.bmp`) shows a large dark polygonal
obstruction below an overhanging wall, not a clearly visible green SUV trigger
brush. It is not the user's interactive screenshot and has not been matched to
a particular model/face, so it does not identify the obstruction.

The app-bundled `entities.json`, `geometry.json`, and `world_mesh.json` match
the local Bank export byte-for-byte. Their SHA-256 values are respectively
`502ed7d8...5aab59`, `db4910c0...39444`, and `a86cd5d7...5b24a6`. The current
build executable is newer than the app-bundled executable: build
`fdny-scene-viewer` SHA-256
`9c58160a3977a344a4a03f8f70b8f5dcd999127821c66c8bf95b597ebb15f3d1` has
mtime 2026-10-13 13:59:12, while the app-bundled
`fdny-scene-viewer` SHA-256
`8515b3b123ce1e8c71c671cb13c9538380a83f4ae952e27c1c375453a04e6842` has
mtime 2026-10-13 13:52:29. `Open FDNY Scene Preview.command` invokes the app
builder, which rebuilds and copies the executable before launching. A window
already left open or a direct launch of the bundle could still be running the
older executable; the exact launch path is not yet known.

The new approach/collision diagnostic initially hit world model 0 near the
doorway, but that trace reports the swept player origin after hull expansion,
not the wall's exact surface point. A read-only correlation audit confirms it
is not face 4947: the rendered `wal_Stco_wall` candidate is at z=791 and
y=192..249, while the reported player-origin hit is near z=809.77 and
y=-33.90. The centered headless route passes the door, so the off-center
collision hit does not identify the opaque start surface or an alpha defect.
The relevant BSP leaf-side collision plane remains to be identified if the
interactive route reproduces a blocker.

## Next checks

1. Rebuild and launch a fresh viewer. Capture the exact interactive window and
   startup stdout, including the absolute package path and BSP SHA-256. Note
   whether the user launched via `Open FDNY Scene Preview.command` or opened an
   existing app process; the latter may predate the current build.
2. Match the visible enclosure to the current offscreen dark polygon, green
   trigger brushes, or a world model group. Use temporary per-draw
   group/model/texture labels or wireframe diagnostics to identify it; do not
   suppress a group to make the scene look open.
3. Compare the preview's heading conversion against retail behavior and the
   authored Bank layout; the source preview's heading-0 view already matches
   the ray audit's `-Z` assumption.
4. If the surface is model 300 or 301, compare its normally rendered pixels
   against the authored material and retail visibility behavior. If it is model
   0, identify the world face and expected visibility before changing material
   alpha.
5. Join the world-model-0 collision hit at the nearby Bank doorway to the
   candidate rendered wall face; both reports may involve the same opening,
   but the current coordinates and camera ray are not yet a verified match.

Package provenance is logged by the app's `--render-check`; see
[`bmodel_door_runtime_port.md`](bmodel_door_runtime_port.md) for the separate
live door-contact report.
