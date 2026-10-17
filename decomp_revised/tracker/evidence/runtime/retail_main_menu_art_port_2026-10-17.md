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
gradient and duplicate title text with the actual retail title artwork. The
two port actions and their placement remain provisional UI; no retail evidence
currently supports their exact labels or coordinates.

The packaged executable is SHA-256
`b48789b0b0fdd5122eb7336b6579cdb1e2ad16d1e3a6d594f50691420693437e`, matching
the CMake-built executable. Its bundled `menu/main.bmp` hash matches the
source bitmap. `--mission-menu-render-check` rendered the retail title art and
all eight mission rows; title selection changed 42,824 pixels and all seven
mission-row transitions changed the frame. This controlled render check does
not verify interactive behavior or retail layout parity.

The renderer source is
[`mission_selector.h`](../../../port/spikes/training-native-prototype/src/mission_selector.h),
and the package copy is in
[`build_scene_viewer_app.sh`](../../../tools/build_scene_viewer_app.sh).

## Unresolved parity and impact

Static audit confirms that retail constructs ten menu children, including the
level-selector child, and that the selector updater resolves authored level
title/bitmap data and selection-box coordinates. The current native rescue
list still uses a port-created layout and names. Exact main-menu widgets,
screen order, selector art/text/coordinates, navigation and click semantics,
back-state transitions, and menu sound triggers are not mapped. A Ghidra
instance was unavailable during this checkpoint, so these items remain open
until the selector activation/vtable, menu dispatch/input producer, and state
transition handlers are decompiled from the same-hash executable. Runtime
comparison is also needed to confirm actual behavior and timing.

Static observations are not a retail runtime capture. The current front end
should be described as using authentic retail title artwork with provisional
menu interaction, not as a retail-matched menu.
