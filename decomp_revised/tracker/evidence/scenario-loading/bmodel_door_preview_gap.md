# Shared BModel doors: preview integration gap

## What is already established

- `FDNYFirefighter.exe` is pinned at SHA-256
  `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
  `0043DC60` is the BModel ScriptActor update method and `0043E680` handles
  BModel script commands. The update advances frame state 3/4 with global
  delta times speed, clamps to authored endpoints, samples each child model's
  motion path, and applies the resulting transform. The call frequency in the
  original executable is not established.
- Subway A's EVS binds `*subdoor` to `touch`/`untouch`: touch applies time scale
  4, plays `SubDoor`, and opens; untouch plays the cue and closes. The map
  statically joins 17 BModels to three animated children each. See
  [the Subway interaction trace](subway_a_interaction_path.md).
- The Training prototype's old `Door` remains Training-only. The map-neutral
  viewer now has its own per-BModel runtime state; see
  [`bmodel_door_runtime_port.md`](bmodel_door_runtime_port.md). Scene packages
  include retail-decoded `motions.json` tracks alongside unchanged chunk 24
  bytes.

## Decoder findings and implementation boundary

The pinned `Genesis.dll` (`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`)
is now decompiled through the retail `MOTN 0.F0` and `PATH 0.F2` loaders,
rotation and translation key readers, interpolation dispatch, and
`gePath_Sample`. The typed `motions.json` export was generated for all 17
copied BSPs while preserving the raw chunk. It contains 1,235 paths and 2,470
tracks. Every path uses key mode 1, which dispatches to direct quaternion
slerp at `10004E30` and cubic Hermite translation at `100157F0`; the retail
reader also applies quaternion sign continuity and precomputes translation
tangents. The three-slerp/control-quaternion rotation path is mode 2 at
`10004E60` and is not used by these maps. 124 channel lists use compressed
start-plus-step times. The source and coverage report are in
[`genesis_path_tracks_and_port_decoder.md`](../genesis-boundary/genesis_path_tracks_and_port_decoder.md).

The reusable map-neutral sampler in
[`gbsp_motion_path.h`](../../port/spikes/training-native-prototype/src/gbsp_motion_path.h)
now ports this observed nonlooping mode-1 subset: quaternion sign continuity
and slerp, key bracketing/clamping, translation tangent precompute, and cubic
translation sampling. All 1,235 copied PATH objects have a zero loop flag. The
viewer now resolves each BModel `C_AnimModelN` child name through the same
BSP's `%Model%` entity and numeric `Model` index, then joins that index to its
retail MOTN record. It prepares each PATH with the retail sampler and applies
sampled gameplay poses to both render and collision transforms. The interactive
viewer starts at each BModel's minimum path time. `M`/`N` sample a separate
inspection timeline; live BModel poses now take precedence in rendering so the
inspection timeline cannot hide the state used by collision. The gameplay route
dispatches BModel and BoundsTrigger touch events, runs `open`/`close` and
`settimescale` commands, and has synthetic interaction checks for Subway A and
Bank.
Subway A's broken-door trigger resolves its brush bounds and dispatches the
authored `Zapp3d` sound command. The retail constructor proves initial per-BModel
speed `1.0f`; see
[`bmodel_door_speed_semantics.md`](bmodel_door_speed_semantics.md). The original
callback cadence and delta units remain unknown, so host playback is not
claimed to match retail door timing. The old explorer remains a comparison
reference only; no motion decoder or gameplay implementation was copied from
it.

## Next source-backed implementation steps

1. Compare the host's unioned child-model contact bounds with the exact retail
   BModel bound setup, then verify player contact and audio in a macOS window.
   Check trigger `C_Model` AABBs against retail setup and verify their player
   contacts. Keep callback ordering and timing provisional until a retail
   capture.
2. Port the retail loop-time branch if a source asset uses it; the 17 copied
   BSPs contain no looping PATH objects. Keep x87 precision and frame cadence
   differences explicit.

The viewer now samples and renders authored moving BModel children, dispatches
BoundsTrigger events, and runs the authored Subway A and Bank door command
paths. The current build succeeds; synthetic interaction checks pass for both
maps, and `--inspect` loads them with zero texture failures and grounded
spawns. Interactive desktop contact, exact contact-box construction, callback
cadence, and retail timing parity remain open. The Bank synthetic interaction
check now targets nearest start-area door entity 541 rather than distant entity
365, but it teleports to bounds and does not verify walking contact or passage
through the live collision transform. `M`/`N` remain inspection-only controls.
