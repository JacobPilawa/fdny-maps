# BSP surface material and alpha-resource join

## Scope and pinned target

This trace joins the exported GBSP face/texinfo/texture records to the original
renderer-facing resource registration boundary. The executable analyzed in
Ghidra is `FDNYFirefighter.exe`, x86:LE:32 at `0x00400000`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` (verified
from `inputs/original/binaries/FDNYFirefighter.exe`). Ghidra reports the same
current program and executable path. Renderer internals beyond imported
Genesis3D APIs are not part of this executable, so public Genesis3D 1.1-derived
source below is comparative reference, not proof of the shipped `Genesis.dll`
implementation.

## Surface-to-bitmap join

The exported MBase GBSP v15 package is pinned by
`.local/exports/mbase/report.json`: source `mbase.bsp`, SHA-256
`4ff3b18dd809c619a6734673a7f90bd2e98d9fc9518ee7bf94ece4b6ff90b266`. The
exporter decodes each face's `TexInfo` index, follows that texinfo's `Texture`
index into the `TEXTURES` chunk, and groups triangles by `(model_id,
texture_id)`. Each emitted group carries the resulting `texture_index`, texture
name, and deterministic `texture_file` BMP path (`tools/export_gbsp_scene.py`,
`world_mesh`).

That relationship matches the public Genesis3D GBSP layout: `GFX_Face.TexInfo`
selects `GFX_TexInfo`, whose `Texture` selects `GFX_Texture` (the on-disk
texture record carries name, flags, dimensions, TEXDATA offset, and palette
index) ([GBSPFile.h, GFX_Face/GFX_Texture/GFX_TexInfo](https://github.com/RealityFactory/Genesis3D/blob/master/World/Gbspfile.h#L220-L266)).
The renderer's public `Surface.c` follows precisely that chain in `GetSurfInfo`
and marks the surface `SURFINFO_TRANS` when `TexInfo.Flags & TEXINFO_TRANS` is
set ([Surface.c, GetSurfInfo](https://github.com/RealityFactory/Genesis3D/blob/master/World/Surface.c#L235-L283)).
Its `GetTexVerts` projects UVs as dot products of each position and the two
texinfo vectors; `GetSurfInfo` then applies inverse DrawScale, Shift, and
texture dimensions to compute wrapping shifts ([Surface.c, UV and shift setup](https://github.com/RealityFactory/Genesis3D/blob/master/World/Surface.c#L193-L229), [Surface.c, shift computation](https://github.com/RealityFactory/Genesis3D/blob/master/World/Surface.c#L336-L367)).
The current exporter implements those base UV inputs and normalization, with
the expected vertical flip for its BMP/OpenGL convention.

The MBase check found 10,023 faces, 71 texture records, and 95 mesh groups.
Every face's texinfo-to-texture index is in range; the groups collectively use
63 unique texture indices, and every group has a `texture_file`. The 63 used
base BMPs decode in preview inspect mode. This establishes a complete base RGB
material path for the static scene geometry; it does not establish complete
lighting parity, since current exports preserve but do not apply lightmaps.

## What alpha means here, and what remains unjoined

There are two alpha-related data paths, and they should not be conflated:

1. BSP `GFX_TexInfo` has a numeric `Alpha` field as well as `Flags`. Public
   `Surface.c` explicitly promotes the `TEXINFO_TRANS` flag to a transient
   surface flag. In the MBase package 9,987 face uses have texinfo alpha 255 and
   36 use 128; several flag values occur. This is evidence for a small set of
   potentially translucent/partially alpha surfaces, not an absence of their
   base RGB bitmap. The exact shipped-driver interpretation of `Alpha` and
   `TEXINFO_TRANS` still needs renderer-side confirmation.
2. The EXE's `ROC_ALPHA_TEXTURE` parser (`0049AFE0`) accepts a resource key,
   base bitmap path, and optional alpha bitmap path, then calls
   `RegisterWorldBitmapAndAlpha` (`00494AD0`). A fresh analysis of `00494AD0`
   against the pinned program confirms that it loads the base bitmap, loads the
   optional second bitmap, calls `geBitmap_SetAlpha(base, alpha)`, and registers
   the base via `geWorld_AddBitmap`. This is the authored external bitmap
   resource path described in
   [`gbsp_texture_resource_registration.md`](gbsp_texture_resource_registration.md).
   The EXE-side evidence does not establish that these RRF resources replace or
   provide alpha for a BSP `GFX_Texture` by name/index; that join remains
   unproven. The BSP exporter currently emits its embedded palette-indexed
   TEXDATA as 24-bit RGB BMPs, so it also discards any alpha channel during
   conversion (the original indexed RGB texels are retained).

Neither alpha path explained the reported flat scene: the face-to-BSP-texture
join was complete, and alpha controls coverage/transparency rather than
selecting the base RGB image. The renderer trace found the actual failures
upstream of texture interpretation: missing `RCS_BoundsTrigger.model` values
were converted to model index 0, disabling the world BSP for collision and
rendering, and the shared viewer drew `col_*` helper surfaces over the scene.
Both are fixed. The macOS CGL framebuffer check now confirms that all textures
assigned to visible groups upload and produce varied pixels for all seven
rescue maps; see
[`scene_preview_collision_texture_diagnostics.md`](scene_preview_collision_texture_diagnostics.md).

This establishes base RGB texture sampling and drawing, not exact Genesis
presentation parity. The shipped engine's BMP decoder, world bitmap-list
registration, GBSP chunk checks, and puppet material registration now have
hash-pinned traces; see
[`genesis_render_asset_path.md`](genesis_render_asset_path.md). The offscreen
frame does not establish window/display behavior, and the exporter still omits
lightmap shading and the face-level `texinfo.flags` / `texinfo.alpha` rendering
rules.

## Concrete port step

Keep the existing face → texinfo → texture-index → `texture_file` join as the
albedo input. Next, extend the exported material/group record to retain the
source `texinfo.flags` and `texinfo.alpha` per face (or split groups when those
vary), then reproduce the confirmed surface alpha/transparency behavior. In a
separate resource pass, parse the map's `.rrf` records and compare their base
bitmap paths/keys against BSP texture names before applying any optional alpha
bitmap; do not infer a match by order.

## Evidence limits

The EXE trace is hash-pinned and uses its imported API call boundary. Traces
now analyze shipped `Genesis.dll` collision and bitmap/resource paths, but not
its surface drawing implementation; see
[`genesis_collision_path.md`](genesis_collision_path.md) and
[`genesis_render_asset_path.md`](genesis_render_asset_path.md). The open RealityFactory
tree identifies itself as based on Genesis3D 1.1 with community changes, so its
surface implementation is reference material and not automatically reusable
code ([repository provenance](https://github.com/RealityFactory/Genesis3D)).
No alpha-resource-to-BSP-texture key join or visual parity claim is made here.
