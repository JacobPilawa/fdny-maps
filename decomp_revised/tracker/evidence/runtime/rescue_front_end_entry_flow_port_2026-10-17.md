# Rescue front-end entry flow — 2026-10-17

## Change

The default macOS app opens a native title screen with the retail root labels
**GAME**, **SCORES**, **OPTIONS**, and **EXIT**, then a source-ordered selector
for the eight rescue entries when GAME is chosen. GAME currently bypasses the
retail career child page; SCORES and OPTIONS remain visible but their child
pages are not ported. EXIT and Escape leave the app. Escape from the mission
list returns to the title screen; closing a mission window returns there after
any authored linked-stage chain ends. The ordinary project launcher no longer
displays the AppleScript raw-BSP picker. Passing a map key remains a development
path that opens one package directly.

The shared app bundle is now named `FDNY Firefighters.app`, and the ordinary
build-script default is the rescue-all bundle. It contains Training, all seven
primary rescue packages, and the five linked-stage packages. The source-ordered
selector exposes the eight rescue catalog entries; linked stages continue
through their authored `finalexitloadmap` routes.

The title uses retail `menu.rrf` bitmap ID 10 (`main.bmp`) and alpha texture ID
100 (`mcontrols.bmp` / `mcontrols_a.bmp`). Fresh same-hash Ghidra trace maps the
four root button origins, hit widths, y range, action codes, and child indices;
the selected-label fade remains approximate. The eight-row rescue selector is
still port UI; it does not show the retail single-record briefing/map/marker
page. Exact keyboard bindings, menu audio timing, and child-page transitions
remain open.

## Retail evidence and limits

Static traces use `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The existing [`level_to_evs_static_trace.md`](../scenario-loading/level_to_evs_static_trace.md)
maps the catalog ordinal to retail level-selection handler `0042F500`, which
resolves the selected record and passes it to level startup `0042AB00`. The
port's selector uses the source-ordered catalog ordinal, key, and title. The
[`menu resource-load note`](../scenario-loading/0042dec0_menu_resource_load_fresh.md)
confirms the retail menu constructor loads `data\\resdefs\\menu.rrf` through
`0042DEC0`; the fresh widget/child trace is recorded in
[`retail_menu_widget_trace_2026-10-17.md`](retail_menu_widget_trace_2026-10-17.md).

The authored level catalog also contains per-level `time:` values and distinct
`menumusic:` / `music:` fields. Existing static evidence does not yet show how
the time limit reaches a HUD, expires, or affects mission completion, nor where
menu and level music streams begin or switch. Mission objective/result/outro
semantics remain open; specifically, the no-final-exit `0044C360` path is not
verified as a mission outro. See
[`rescue_completion_and_exit_path.md`](../scenario-loading/rescue_completion_and_exit_path.md)
and [`the catalog-loader trace`](../scenario-loading/lev_rrf_catalog_loader_trace_fresh.md).

## Controlled verification

- `sh tools/build_scene_viewer_app.sh rescue-all` completed and linted the app
  `Info.plist`. CMake emitted the existing json-c deployment-target warning
  (dylib built for macOS 27.0, link target 13.3).
- The CMake and app-bundled executable hashes match:
  `bcbd5e3803e036a2b9b2240ad81e8f162c04773f1104c9617182e6876fd29121`.
- Packaged `--mission-catalog-check` loaded all eight rescue entries through
  `SceneData`.
- The earlier `--mission-menu-render-check` rendered the title artwork and
  eight mission rows, but predates the new four-label root control integration.
  No new offscreen render check was run for this checkpoint.
- Packaged `--rescue-check` passed on all 13 included Training, primary rescue,
  and linked-stage packages. These are controlled pickup/release/passout/exit
  transactions, not natural travel, mission completion, interactive launch, or
  retail parity.
- Shell syntax checks passed for both launchers and the build script. The old
  launcher name remains as a compatibility alias; neither launcher invokes
  AppleScript.

## Next work

Port the retail career and scores/options child pages, then replace the
eight-row list with the single-record selector using authored briefing images,
map coordinates, marker, and button actions. Trace menu/level stream callsites,
then use the level-specific time field only after its expiry/result path is
mapped. Resolve the early mission-start equipment `tell` handoff described in
[`entity_factory_queue_update_order.md`](../scenario-loading/entity_factory_queue_update_order.md): the first shared `rescue.evs` loadout tells run before Player ScriptActor registration and are dropped; the later full EVS load is delayed by four World callbacks, but its order against the BSP entity pump is still unresolved. The copied MP3s and `menumusic`/per-level catalog choices are present, but playback start/switch/stop/loop triggers remain untraced. Keep natural rescue flow and retail parity separate from controlled transaction results.
