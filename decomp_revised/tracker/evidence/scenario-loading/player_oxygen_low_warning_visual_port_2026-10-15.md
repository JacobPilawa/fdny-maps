# Player low-oxygen warning visual port (2026-10-15)

## Retail trace

The pinned `inputs/original/binaries/FDNYFirefighter.exe` has SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Read-only Ghidra analysis of `004576B0` shows the oxygen widget constructs two
resource-12 gauge-face layers. The first is tinted RGB `(1, 1, 1)` and the
second `(1, 0.25, 0)`. `rescue.rrf` maps resource 12 to
`oxygauge.bmp`/`oxygauge_a.bmp`.

In status updater `00458400`, displayed oxygen at status `+0x28b0` controls the
warning object at `+0x29f0`. At or above `0.25`, the updater clears latch
`+0x29fc` and hides the effect. Below `0.25`, it sets the latch and writes this
value to the child controller at `+0x1c`:

```text
(1 - (0.25 - displayedOxygen) * 4) * 0.9 + 0.1
```

The scalar is `1.0` at the threshold, `0.55` at `0.125`, and `0.1` at zero.
The constants were read from the pinned image (`004E2180`, `004E0510`,
`004DFC24`, `004E0EB8`, and `004DF7B8`). This traces the orange layer and
controller input, but does not establish how the retail render path interprets
that controller field or blends the layer.

## Port and limits

The native viewer now draws the orange resource-12 face between the white face
and needle whenever displayed oxygen is below `0.25`. It maps the threshold to
zero alpha and empty oxygen to `0.1` alpha. This is a renderer approximation
based on oxygen level; it does not pass the retail controller scalar through
as alpha. Retail blend behavior and an in-game comparison remain open.

The packaged `--oxygen-hud-render-check` checks values at/above the threshold,
at `0.125`, and at zero, and verifies that the overlay changes the framebuffer.
On the current packaged build, the MBase render check reports three oxygen HUD
layers and 26,849 changed pixels. The packaged and CMake executables match at
SHA-256 `b958c5ab8439b14839e0e736ed8cedb659669fdc9d4cf91b77a7b110af99f90a`.
All 13 Training/rescue/stage packages pass `--rescue-check`, and
`--mission-catalog-check` loads all eight rescue entries. The checks demonstrate
host behavior only.

## Evidence

- Source excerpt: [`00458400_oxygen_warning_visual_fresh.c`](00458400_oxygen_warning_visual_fresh.c).
- Read-only Ghidra output: `.local/oxygen-status-init-query.log`,
  `.local/oxygen-status-warning-fresh.log`, and
  `.local/oxygen-warning-double.log`.
- Resource definition: `inputs/original/Data/ResDefs/rescue.rrf`, line 46.
- Related port state and renderer: `port/spikes/training-native-prototype/src/player_oxygen.h`
  and `port/spikes/training-native-prototype/src/oxygen_hud_renderer.h`.
