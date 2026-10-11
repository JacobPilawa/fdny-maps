# Original player action input evidence

- Training config `evidence/original/input.rrf` defines `fire` on keyboard code 29, `selectaxe` on 02, `selectext` on 03, and keypad aliases on 79/80.
- The input-device constructor `00437F30` names `MOUSE_LB` as code `0x103`, `MOUSE_MB` as `0x104`, and `MOUSE_RB` as `0x105`. Its decompilation and raw instructions are preserved in `evidence/input-action/device-backend/`.
- Player-controls constructor `004395C0` binds `ACTION` to `0x103` and `JUMP` to `0x105`. The control queries in `0044F340` therefore map to left mouse for action and right mouse for jump. This resolves the earlier ambiguity between named `ACTION` and the separate configured `fire` control.
- `0044F340` dispatches `ACTION` pressed/held/released to item vtable slots `+0x14`, `+0x18`, and `+0x1c`. Native left-button press/release now reaches the extinguisher's recovered `004614E0`/`00461590` state callbacks. While the extinguisher is active, the native loop advances its follower and active pose using the captured `00461F80` RNG/pose prefix.
- The extinguisher hold slot is inherited base no-op `0043F660`. Sound playback, frame-record animation, fire contacts, obstruction, queued messages, and fire updates remain unintegrated. The native active-pose update consumes the one draw captured in the prefix; the complete global frame draw order is not established.

The left-button tutorial text for goggles in `evidence/original/training.evs` is consistent with this mapping. Native 1/2 tool selection is also connected. Keep `fire` code 29 distinct from `ACTION` code `0x103`.
