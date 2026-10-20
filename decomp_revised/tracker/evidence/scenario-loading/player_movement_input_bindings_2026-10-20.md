# Player movement input bindings and host aliases (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Static evidence is in `reference/previous-work/decomp/evidence/input-action/004395c0_FUN_004395c0.c`,
`00437410_FUN_00437410.c`, and
`reference/previous-work/decomp/evidence/gameplay/00452570_FUN_00452570.c`.
The gameplay block at `00452570` is an evidence-only code slice, not a
Ghidra function object. The live Ghidra bridge had no running instance during
this check; no fresh decompilation is claimed.

## Retail action values and defaults

`004395C0` registers `MOVE_FORWARD`, `MOVE_BACKWARD`, `MOVE_LEFT`, and
`MOVE_RIGHT` as digital actions, then calls `00437410` (a wrapper for
`00438BF0`) twice per action to add two keyboard bindings:

| Action | First key code | Second key code | `00452570` local contribution |
| --- | --- | --- | --- |
| `MOVE_FORWARD` | `0xC8` (up arrow) | `0x11` (W) | `+1.0` |
| `MOVE_BACKWARD` | `0xD0` (down arrow) | `0x1F` (S) | `-1.0` |
| `MOVE_LEFT` | `0xCB` (left arrow) | `0x1E` (A) | `+0.75` lateral |
| `MOVE_RIGHT` | `0xCD` (right arrow) | `0x20` (D) | `-0.75` lateral |

The `1.0` and `0.75` values are backed by the immediate data used in the
movement block (`_DAT_004dfc24` and `_DAT_004e1c18`, respectively). The
preserved `inputs/original/Data/ResDefs/input.rrf` also contains the arrow
codes for the corresponding movement/strafe defaults. Fresh current-hash
tracing resolves the ordinary local movement vector as `(lateral,0,forward)`:
left adds `0.75` to X, right subtracts `0.75` from X, forward adds `1.0` to Z,
and backward subtracts `1.0` from Z. The normal path sends this vector through
the degree-based yaw basis in `004968B0` and the column multiply in
`00495EB0`; exact formulas are recorded in
[`player_locomotion_yaw_helper_2026-10-20.md`](player_locomotion_yaw_helper_2026-10-20.md).
The host's `side` scalar remains positive for right. Its camera basis uses an
OpenGL view convention, while the retail view/follower-to-Genesis camera
composition has no numeric join yet, so this local sign result alone does not
justify a host world-space correction.

## Shared host change

The rescue scene host in `port/spikes/training-native-prototype/src/scene_viewer.cpp`
now accepts both arrow keys and WASD for each movement action. It retains the
source-backed `0.75` lateral weight. This adds the missing default aliases to
the shared playable path; it does not establish retail key rebinding/config
behavior, action polling cadence, the final movement-frame orientation, or
retail gameplay parity. The macOS rescue-all app compiled with matching
standalone/app executable SHA-256
`2835a7ee082c8c0a9a164a69c4c6bd98d3ddcc41883d3d2c38f75bde3219aea9`; its
`Info.plist` validation passed. Linking warned that installed `json-c` targets
macOS 27.0 while the build links for 13.3. This is compile-only evidence; no
tests, interactive gameplay, or retail parity check were run.
