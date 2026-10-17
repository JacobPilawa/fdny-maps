# Rescue front-end entry flow — 2026-10-17

## Change

The default macOS app now opens a native title screen with **Start Rescue** and
**Quit**, then a source-ordered selector for the eight rescue entries in the
retail level catalog. Escape from the mission list returns to the title screen;
closing a mission window returns there after any authored linked-stage chain
ends. The ordinary project launcher no longer displays the AppleScript raw-BSP
picker. Passing a map key remains a development path that opens one package
directly.

The shared app bundle is now named `FDNY Firefighters.app`, and the ordinary
build-script default is the rescue-all bundle. It contains Training, all seven
primary rescue packages, and the five linked-stage packages. The source-ordered
selector exposes the eight rescue catalog entries; linked stages continue
through their authored `finalexitloadmap` routes.

The title-screen copy and layout are new port UI. They do not claim to reproduce
retail artwork, menu commands, input semantics, transition timing, or audio.

## Retail evidence and limits

Static traces use `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The existing [`level_to_evs_static_trace.md`](../scenario-loading/level_to_evs_static_trace.md)
maps the catalog ordinal to retail level-selection handler `0042F500`, which
resolves the selected record and passes it to level startup `0042AB00`. The
port's selector uses the source-ordered catalog ordinal, key, and title. The
[`menu resource-load note`](../scenario-loading/0042dec0_menu_resource_load_fresh.md)
confirms the retail menu constructor loads `data\\resdefs\\menu.rrf` through
`0042DEC0`; it does not yet describe the retail menu artwork or interaction
semantics. No new Ghidra function was reviewed for this port checkpoint.

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
- Packaged `--mission-menu-render-check` rendered the title screen and all
  eight mission rows; both selectable title rows changed the frame, as did all
  seven mission-row transitions.
- Packaged `--rescue-check` passed on all 13 included Training, primary rescue,
  and linked-stage packages. These are controlled pickup/release/passout/exit
  transactions, not natural travel, mission completion, interactive launch, or
  retail parity.
- Shell syntax checks passed for both launchers and the build script. The old
  launcher name remains as a compatibility alias; neither launcher invokes
  AppleScript.

## Next work

Trace the retail menu widgets and menu/level stream callsites, then use the
level-specific time field only after its runtime semantics and expiry path are
mapped. Implement visible mission-start/objective/completion/end behavior from
that evidence. Keep natural rescue flow and retail parity separate from the
current controlled transaction results.
