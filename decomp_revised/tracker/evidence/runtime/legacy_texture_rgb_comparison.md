# Legacy viewer and port texture-image comparison

Updated: 2026-10-13

## Result

Using the old map explorer strictly as an output reference, decoded every
legacy indexed PNG and the matching current port BMP for each texture used by
its 17 maps. All **1,005 image pairs** have the same width, height, and RGB
value at every texel. The compared maps use 22 to 118 textures each. The
per-image dimensions, names, SHA-256 values, and equality result are recorded
in [`legacy_texture_rgb_comparison.csv`](legacy_texture_rgb_comparison.csv).

Across the faces shown by the legacy viewer, a second comparison also found
exact agreement in model assignment, BSP texture index, and per-corner UVs:
**179,562 polygons / 507,849 fan triangles matched, with zero mismatches**.
The per-map counts and source-output hashes are in
[`legacy_face_material_uv_comparison.csv`](legacy_face_material_uv_comparison.csv).

The matching BSP byte hashes are recorded in
[`MAP_EXPLORER_REFERENCE_AUDIT.md`](../../../docs/MAP_EXPLORER_REFERENCE_AUDIT.md).
The legacy PNGs and viewer code were read as comparison inputs and were not
copied into the port or made runtime dependencies.

## Method

The reference PNGs are 8-bit indexed images with a PLTE palette and unfiltered
rows. The port exports 24-bit BMPs with bottom-up, BGR-ordered rows. Both files
were decoded to top-down RGB byte arrays, and dimensions and every RGB texel
were compared. Encoded file hashes naturally differ because the formats
differ; the RGB pixel arrays matched exactly.

## What this rules out, and what it does not

For the compared files and shared face set, it rules out differences between
the two exporters in palette lookup, source indexed pixel selection, 24-bit
image conversion, model/texture assignment, and per-corner UV values. This is
strong evidence that the reported visual differences occur after these static
asset/mesh records enter the viewer.

This comparison does **not** establish retail Genesis behavior or prove that
the GPU samples and presents the same texels. It also says nothing about
Genesis lightmaps, face flags/alpha, sRGB/gamma, filtering, surface
registration, or dynamic model transforms. Those remain separate render-path
questions in
[`surface_material_alpha_join.md`](surface_material_alpha_join.md),
[`genesis_render_asset_path.md`](genesis_render_asset_path.md), and
[`d3ddrv_render_path.md`](d3ddrv_render_path.md).

## Next visual investigation

Use a reported wrong surface and camera position to compare actual GPU
presentation, lightmap/alpha state, and model transform against the pinned
Genesis/D3D path. Static face-to-texture/UV disagreement is now lower priority
for surfaces included in the reference comparison. Do not adopt the legacy
viewer’s surface filters as a fix: its filter rules are documented as
inspection choices in
[`MAP_EXPLORER_REFERENCE_AUDIT.md`](../../../docs/MAP_EXPLORER_REFERENCE_AUDIT.md).
