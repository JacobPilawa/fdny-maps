# Training ladder contact fixture audit (2026-10-15)

## Plain-language explanation

The retail Player collision path has no ladder-specific bypass in the traced
route. Training's ladder model is solid, while its script accepts Player
contact within five units of the model bounds. The old fixture sampled only
20-unit steps, so it could jump over a position where the Player hull clears
the thin ladder and the authored contact bounds still overlap. The fixture now
also samples the exact hull-clearance boundaries around BModel bounds.

## Pinned data and source evidence

- `FDNYFirefighter.exe` SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Training `training.bsp` SHA-256: `32986212fa875764ca3d5c1630e54ad6315abaea70983203601192cec603a963`.
- Training `training.evs` SHA-256: `0c01a1f8d88ef7a994be382d223fd5302bd29301496362613fe963332b336061`.
- Genesis.dll SHA-256: `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
- The Player's standing hull is `(-30,-30,-30)..(30,85,30)`. Its swept query reaches `00492CE0` with mask `0x43` and collision type `2`; the wrapper supplies null optional filters. Genesis skips a model when flag `0x10` is set, otherwise applies a callback only when one was supplied. See [`genesis_collision_path.md`](../runtime/genesis_collision_path.md) and [`dirty_space_axe_warp_trace_2026-10-14.md`](dirty_space_axe_warp_trace_2026-10-14.md).
- Training entity 108 authors `touch: tell player climb` and `untouch: tell player unclimb`, with `E_Reach=5` and zero hit points. The Player contact handler therefore keeps the authored five-unit reach. Its model 5 bounds are X `[-252,-244]`, Y `[-504,-264]`, and Z `[-76,-12]`; its solid leaf has contents `9`, which intersects `0x43`.
- The live model collision flag was not established. The conclusion is conditional on model 5 being enabled, and the trace gives no evidence for a ladder-specific collision exemption.

## Fixture geometry and change

At ladder center Y/Z `(-384,-44)`, placing the Player center at X `-284` leaves a two-unit gap between the Player's max-X `-254` and ladder min-X `-252`. The Player hull still overlaps the ladder bounds expanded by five units: its max-X `-254` is beyond the expanded min-X `-257`. This is a geometry derivation from the pinned bounds, not a live collision or contact capture.

The `--interaction-check` candidate search in `scene_viewer.cpp` retains its
20-unit samples and adds positions just beyond each horizontal ladder face by
the hull extent plus `min(2, contactReach/2)` units. It continues to require
both the authored reach overlap and a clear host collision query; it does not
disable or filter the ladder model. The search chooses the nearest clear
candidate and then exercises the existing authored touch/climb/untouch fixture.

## Validation

The macOS `fdny-scene-viewer` target rebuilt successfully with
`cmake --build port/spikes/training-native-prototype/.local/build-scene-viewer --target fdny-scene-viewer -j 4`.
The packaged Training data passed
`fdny-scene-viewer .local/exports/training --interaction-check`: entity 108
selected clear position `(-212,-384,-44)`, enabled Player climbing, moved
upward `3.15762` units for one `Player::step`, and disabled climbing on
untouch. That selected point has a two-unit gap beyond the ladder's max-X face
and still overlaps the expanded contact bounds. The symmetric `(-284,-384,-44)`
example is geometrically inside contact reach, but did not win the host search;
nearby collision can reject a derived candidate. This is a synthetic fixture
check and does not establish retail callback scheduling or visual parity.

## Limits

This fixture directly dispatches authored touch and untouch commands after
selecting a position. It does not establish retail event delivery latency,
live model flags, recurring Player update cadence, ladder animation/camera
behavior, or full in-game contact scheduling. The point `(-284,-384,-44)` is a
source-derived candidate example; adjacent world geometry can still reject it.
