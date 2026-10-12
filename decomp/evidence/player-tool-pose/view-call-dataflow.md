# `0044F340` view-call inputs

Original instruction evidence: `movement-callers/assembly.txt` at `00453860` and `view-caller-assembly.txt` at `00451136`.

The ordinary movement caller `00452570` passes, in order:

1. Whether the movement vector exceeds `1e-7`.
2. Action-query enable (`1`).
3. Rotation interpolation fraction (`1`).
4. Target endpoint `(0,0,0)`.
5. Position interpolation fraction (`1`).
6. Position endpoint `(0,0,0)`.

`0044F340` first updates each item's virtual `+0x20` Think slot, then checks the selected item's start/hold/release action slots. It reads the selected item's `+0x1e4` view offset and `+0x1f0` aim offset while building the local view frame, updates the player follower at `+0x34a4`, and calls each item's `+0x24` update after the view transform.

The final view rotation calls `00496B50` with the player field at `+0x3430`. `00496B50` converts its input with the game's degrees-to-radians constant before rotating the frame. The field is written by `00452570` through the Player `+0x140` subobject: constructor `00453D50` installs vtable `004E1C40` there, and slot 0 resolves to `00452570`. At `00452BB7–00452BCB`, that update adds `STEER_PITCH` to Player `+0x3430`, wraps in 360-degree steps, and clamps to the active pitch limit.

The built-in input configuration maps `STEER_PITCH` to mouse axis 1 (`004395C0`); the mouse backend maps `MOUSE_Y` to axis 1 (`00437F30`). The control wrapper `00439310` negates the backend value and multiplies it by `0.1` (`004FF480`), so the source-side increment is `-raw MOUSE_Y * 0.1 degrees` per Player update, without a time multiplier or deadzone in this path. `PlayerManager` initializes fresh `TPlayerInfo+0x2C` to 1, and save/load identifies it as `DifficultyLevel`, supporting the source's ±40-degree default for a fresh profile. The native event adapter uses the source sign and sensitivity for SDL relative-Y units and that ±40-degree default. SDL relative-Y units have not been compared against the original backend's raw axis units; loaded profiles and the state-5/spectating cap exception are not represented in the native build.

A direct-offset scan missed the writer because it addresses the embedded subobject. The corrected alias scan, value words, and the relevant control-wrapper decompilation are preserved under `state-offsets/pitch-writer/`; the original Player update assembly remains in `movement-callers/assembly.txt`.

The independent timed caller `00451080` supplies its blend factor to both fractions, target endpoint `(-60,1000,0)`, and position endpoint `(0,-20,-30)`. Its 12-case fixture matches all 324 matrix/follower words exactly through `--view-update-check`.

## Inline direction precision

The first transition vector uses a distinct inline path and must not be modeled as `normalize(subtract(target, position))` over three stored floats. The original `0044F340` code first makes two scaled target vectors: `0042D050` adds `(0,height,1000)` to the tool aim offset and stores binary32 components, then `00447710` scales that result by the blend fraction; a second `00447710` scales the target endpoint by `1-fraction`. At `0044FA00..0044FA28`, the two scaled target vectors are added on the x87 stack. Z is stored to binary32 at `0044FA28`; X is subtracted from local position and stored at `0044FA31`; Y remains extended through its subtraction at `0044FA35`; Z is reloaded from its binary32 temporary and subtracted at `0044FA3C`. The squared length accumulates Z², Y², then X² (`0044FA3F..0044FA51`), followed by `FSQRT`, an extended reciprocal, and per-component float stores (`0044FA53..0044FA6F`). The normalized float vector then enters frame slot `+0x14`, which normalizes it again via `0045A230 → 00459B80`.

`src/view_update.h::normalizeDirectionDifference` mirrors those mixed-precision stores; `normalize` follows the shared `Z² + Y² + X²` order. This recovered the original 12-case timed-transition output exactly and also closed the 12-case `0045FEC0 → 0044F340` tool-offset/view chain to zero differences (420 words). The original x86 oracle runs under Unicorn; the offset-chain callbacks remain stubs, so the chain proves numeric producer/consumer composition rather than live scheduling. Raw captures are `evidence/view-update-oracle.json` and `evidence/view-item-offset-chain-oracle.json`; raw instructions are in `evidence/player-tool-pose/view-assembly.txt`.

`tools/view_update_oracle.py` now also executes original `0045FEC0` before `0044F340` for 12 offset-chain captures. During the view call, item Think callbacks remain return-stubbed; this establishes the offset writer/consumer data path without asserting item activation or live Training scheduling. Results are in `view-item-offset-chain-oracle.json` and can be checked with `--view-item-offset-chain-check`.

## Extinguisher callback order

The extinguisher vtable at `004e28c0` maps slot `+0x14` to start `004614E0`, `+0x18` to inherited base no-op `0043F660`, `+0x1c` to release `00461590`, `+0x20` to inherited base no-op, and `+0x24` to `00461F80`. `0044F340` invokes the selected item's `+0x24` callback after building the view frame and updating the player follower. The first operation in `00461F80` is `0045F600`, which advances the tool follower when its active byte is set; the frame record, active pose, contacts and pose interpolation follow that update.

Selection and ACTION have separate state. `0044C2A0` changes the selected inventory slot through item vtable `+0x3c`; for the extinguisher this resolves to `00461270`, which calls `0045F620` to store the equipped byte at `+0x158` and reattach its follower when enabled. Deselecting calls the extinguisher release slot. The post-view `0045F600` follower update is gated by `+0x158`; the spray pose/contact path in `00461F80` is separately gated by ACTION byte `+0x600`. The native loop now updates the extinguisher follower and its idle frame whenever selected, while ACTION alone gates active pose, contacts and effect draws. This is a source-derived live integration correction; the controlled view/follower fixtures do not compare full inventory scheduling.

These words are preserved in `evidence/item-pose/live-extinguisher/vtable-methods/vtable-pointer-words.tsv` and `vtable-think-and-post-slots.tsv`. In the latter, `004e28e0` is the table pointer `004e28c0 + 0x20`, so its slot 0/1 are the item's `+0x20`/`+0x24` entries.
