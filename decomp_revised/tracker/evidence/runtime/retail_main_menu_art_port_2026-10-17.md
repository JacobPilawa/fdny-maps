# Retail main-menu artwork port — 2026-10-17

## Confirmed retail evidence

Static evidence is tied to `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The hash-pinned `0042DEC0` menu constructor loads
`data\\resdefs\\menu.rrf` and initializes a 640x480 menu surface. The RRF
registers bitmap resource ID 10 as `data\\menu\\main.bmp`.

- `inputs/original/Data/ResDefs/menu.rrf` — SHA-256
  `ae89458ef0aa2017351c1fa92fa22286eb4d4052cd55bbee4c1b917a1b9104b0`
- `inputs/original/Data/menu/main.bmp` — 640x480 bitmap, SHA-256
  `38a0b043e9af7759dac0f593ba3105e3c0debaef8a649270eeb67b9858061d59`
- Existing constructor evidence:
  [`0042dec0_menu_resource_load_fresh.md`](../scenario-loading/0042dec0_menu_resource_load_fresh.md)
- Existing menu/selector path evidence:
  [`lev_rrf_catalog_loader_trace_fresh.md`](../scenario-loading/lev_rrf_catalog_loader_trace_fresh.md),
  [`0042f500_level_selection_load.c`](../scenario-loading/0042f500_level_selection_load.c),
  [`selector_initial_ordinal_resolution.md`](../scenario-loading/selector_initial_ordinal_resolution.md)

## Port change

The rescue-all macOS bundle copies the original `main.bmp` to
`Contents/Resources/menu/main.bmp`. The native title renderer decodes and draws
that bitmap at its authored 4:3 aspect ratio. This replaces the synthetic
gradient and duplicate title text with the actual retail title artwork.

The rescue-all bundle also copies the paired `mcontrols.bmp` and
`mcontrols_a.bmp` from `menu.rrf` resource ID 100. The renderer merges the
grayscale mask into texture alpha and draws the four labels from authored
quarter-height rows. Fresh trace of `00430F00` resolves the button origins to
`(50,190,346,514)`, the shared top-origin y range to `440..472`, and the hit
widths to `(100,116,128,70)`. Button actions 0..3 map through callback
`00430DF0` to menu children 1, 4, 3, and 8. The port shows GAME/SCORES/OPTIONS/
EXIT in that order; GAME opens the current rescue chooser and EXIT quits.
SCORES and OPTIONS pages are not ported, and GAME bypasses the retail career
page before opening the port-created eight-row selector.

Selected-label intensity is an approximation. Retail button code advances an
alpha layer using global frame delta, but exact highlight appearance and
timing have not been captured. Mouse hit rectangles use traced widths;
arrow-key navigation remains a port binding because retail input production is
unresolved.

The initial artwork-only build was hash-matched and its offscreen render check
confirmed the title art and port-created mission rows. That check predates the
authentic control texture integration and does not verify its visible result.
The current rescue-all app build packages both control bitmaps; its executable
and resource hashes are in the latest progress-history row. No interactive
launch or retail runtime comparison was performed for this checkpoint.

The renderer source is
[`mission_selector.h`](../../../port/spikes/training-native-prototype/src/mission_selector.h),
and the package copy is in
[`build_scene_viewer_app.sh`](../../../tools/build_scene_viewer_app.sh).
The same-hash root widget and selector evidence is in
[`retail_menu_widget_trace_2026-10-17.md`](retail_menu_widget_trace_2026-10-17.md).

## Unresolved parity and impact

Static audit confirms that retail constructs ten menu children and that the
root uses authentic labels and authored control positions. The native rescue
list still uses a port-created layout. Career, scores, and full options page
transitions, selector art/text/coordinates, back-state behavior, exact button
highlight timing, and menu sound triggers remain open. Runtime comparison is
needed to confirm actual appearance and timing.

Static observations are not a retail runtime capture. The front end now uses
authentic retail title and root-control art/layout with provisional highlight
and partial action flow. It is not a retail-matched full menu yet.
