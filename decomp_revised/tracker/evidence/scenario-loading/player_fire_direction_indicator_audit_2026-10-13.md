# Player fire direction indicators and camera-facing correction

Audited 2026-10-13 against the copied retail executable:

- Path: `inputs/original/binaries/FDNYFirefighter.exe`
- SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`

## Retail behavior

`00451640` derives a horizontal fire-relative angle and calls `0044BB00(Player,
angle)`. `0044BB00` does not write camera yaw, the Player view frame, or movement
heading. It sets one transient byte in Player `+0x353C..+0x353F` according to
these sectors:

| Player byte | Inclusive/exclusive angle range | Setter address |
|---|---|---|
| `+0x353C` | `[-π/4, +π/4]` | `0044BB44` |
| `+0x353D` | `[-π, -3π/4] ∪ [+3π/4, +π]` | `0044BB92` |
| `+0x353E` | `(+π/4, +3π/4)` | `0044BBBE` |
| `+0x353F` | `(-3π/4, -π/4)` | `0044BBEA` |

The side labels are angle-sign-relative; the visual compass labels are not yet
verified. `00452570` reads all four bytes, skips its burn-cooling subtraction
when any is set, then clears the bytes. `00456090` maps them to four HUD child
effects at offsets `+0x3DC`, `+0x624`, `+0x500`, and `+0x748`, respectively.
The child animations, screen placement, and labels remain unrecovered.

## Port correction

The viewer's former `PreviewPlayerFireOps::turnTowardFire` applied a guessed
`3 rad/s` yaw step and copied it into `yawInput`. Because the fire-message
handler can run for several fires in one host frame, these steps stacked and
also overrode mouse look. That behavior had no retail match.

`scene_viewer.cpp` now calculates the source-relative angle with viewer yaw
zero facing `-Z`, accumulates the four source-mapped indicator bytes for the
frame, and runs `updatePlayerBurnCooling` before processing current-frame fire
messages. Fire exposure no longer changes either `yaw` or `yawInput`. The
reader/cooling behavior uses the saved source-derived helper; side-copy values
are still injected as false because their runtime inputs are not wired.

The viewer now loads retail resource 50 (`fireprox.bmp` plus
`fireprox_a.bmp`) and draws one 128x128 paired-texture widget for each active
sector byte. Its four child rotations, viewport positions, and the corrected
rear/side child mapping follow `fire_direction_hud_trace_2026-10-14.md`.
A packaged offscreen check draws all four widgets in all 13 Training/rescue
packages, changing about 130,518 framebuffer pixels per package with no
OpenGL error. The host uses first-channel grayscale alpha and
standard source-alpha blending as a renderer approximation; exact retail mask
sampling, blend state, and live cadence still need comparison.

## Verification performed

The macOS viewer and Training app bundle built successfully. `--inspect` loaded
the Training package, found the authored fire/trigger entities, and completed
its grounded movement check. `--render-check` uploaded all 24/24 Training
textures and rendered a varied framebuffer without OpenGL errors. After the
held-item iteration, the bundled app and CMake binary both have SHA-256
`49ec83d9d632a5b20c4775dd1a61049f6753b4748325d6d2e4eb88d30e5d6722`.

These checks establish compilation and that the port can draw all four source-
mapped HUD widgets. They do not establish interactive camera response or retail
pixel parity. The retail path is static source evidence; exact live scheduling
and GUI blend behavior remain provisional.
