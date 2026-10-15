# Player victim-pickup status HUD

Updated 2026-10-15 from a read-only Ghidra 12.0.4 headless trace of the
project's pinned retail executable. The copied binary SHA-256 is
4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. The
resource table is inputs/original/Data/ResDefs/rescue.rrf; no retail runtime
capture or rendered-frame comparison was available.

## Retail status path

- Player pickup handler 00450440 calls Player status action 1 at
  004571A0. The action enables the status sprite at Player status object
  +0x964, stores the current game-clock tick at +0xA88, samples its rectangle
  track at time zero, applies that sample, and posts status key RC_VictPick.
- Player status constructor 004576B0 creates the +0x964 sprite from resource
  55. The copied rescue.rrf maps resource 55 to rescuing.bmp and
  rescuing_a.bmp. Both copied BMPs are 64 by 64 pixels.
- Constructor 004576B0 adds four rectangle keys to the track at status +0xA8C:

  | Time (seconds) | Left | Right | Top | Bottom | Effect |
  |---:|---:|---:|---:|---:|---|
  | 0.0 | 320 | 320 | 240 | 240 | Zero-size start at screen center |
  | 0.1 | 256 | 384 | 176 | 304 | 128 by 128, centered |
  | 1.0 | 256 | 384 | 176 | 304 | Remains centered |
  | 1.1 | 502 | 566 | 5 | 69 | 64 by 64 near the upper-right corner |

- Status updater 00458400 calls sprite updater 00455C00 for the +0x964 child.
  The sprite updater subtracts its start tick and samples the four-coordinate
  track with seconds converted from milliseconds by binary64 constant
  004DFA28 = 0.001.
- The fresh 00484940 sampler trace shows linear interpolation between adjacent
  keys, truncation toward zero for each output coordinate, and the last key
  held after the track ends. The full decompilation is preserved in
  [00484940_vector_keyframe_sampler_fresh.c](00484940_vector_keyframe_sampler_fresh.c).
- Successful Player rescue helper 00451160 calls status action 2. That action
  explicitly hides the +0x964 sprite before updating the remaining-survivor
  status message.

## Plain-language explanation

Picking up a survivor starts a retail HUD animation: an image grows from the
center, stays there briefly, then shrinks and moves to the upper-right corner.
It remains on screen until the successful-rescue path hides it. This is a
screen-space carry-status cue; it does not establish a transform for drawing
the carried person on the Player.

## Port and limits

The shared viewer copies resource 55 verbatim, bundles it through
tools/export_victim_feedback_sprites.py, and draws the keyframed rectangle in
the traced 640-by-480 coordinates scaled to the current viewport. Status action
1 starts the animation and action 2 hides it. The packaged
--victim-pickup-hud-render-check checks the recorded keyframes, interpolation,
settled position, framebuffer change, and hide edge. These checks verify the
port's source-derived behavior and asset loading; they do not prove retail
blend, filtering, visual parity, or callback cadence.

The native draw samples elapsed SDL ticks at render time. Its relationship to
the retail Player/status update cadence remains open. The status-key consumer
behind RC_VictPick, other status teardown/passout edges, the carried-body pose,
and live original-game comparison also remain open. Color/alpha BMPs are loaded
using the existing HUD texture path, whose blend and filtering have not been
matched against the retail renderer.

## Packaged checkpoint

The rescue-all app was rebuilt with tools/build_scene_viewer_app.sh. The CMake
and packaged executables match at SHA-256
7ae969538feb49d51ff04421414767b84f9e2eac5a6c3beeae8413f93f3e5cc5. The
packaged --victim-pickup-hud-render-check and --rescue-check both passed on all
13 Training/rescue packages. On Training, the HUD check drew one icon layer
and changed 17,169 framebuffer pixels; its saved diagnostic frame is
.local/exports/training.victim-pickup-hud-render-check.bmp. The neighboring
--oxygen-hud-render-check still drew both layers, and --mission-catalog-check
loaded all eight rescue entries. These are host build, asset, render, and
controlled transaction checks, not original-game parity evidence.
