# Retail Genesis MOTN/PATH decoder

Updated: 2026-10-13

## Provenance and scope

The decoder is based on the copied retail `Genesis.dll` (SHA-256
`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`) in the
workspace Ghidra instance. The fresh pseudocode export is
[`genesis_path_sampling_fresh.txt`](genesis_path_sampling_fresh.txt).
The map explorer was not used to derive or implement this format. The raw
GBSP chunk 24 remains in each package beside `motions.json`.

## Retail reader and sampler findings

- `geMotion_CreateFromFile` at `1000D570` reads `MOTN 0.F0` and dispatches
  each `PathArray` item to `gePath_CreateFromFile` at `1000F580`.
- `gePath_CreateFromFile` accepts text PATH versions `0.F1` and `0.F2`, and
  routes the binary `0x1001` form to a separate reader. This first port decoder
  deliberately accepts the actual BSP maps' `PATH 0.F2` records only.
- Rotation keys are read at `10005A00`; translation keys at `10015FB0`. Their
  `Keys` header gives the count, key mode, flags, and a retained trailing
  field. Flag bit 1 (`2`) selects start-plus-step key times. Rotation flag bit
  0 selects quaternion versus axis-angle values. The retail `sscanf` calls
  accept numeric prefixes, so authoring comments after a numeric header are
  ignored for decoding and preserved in the exported raw line.
- Key mode `1` dispatches rotation through sampler slot `4` and translation
  through slot `1` (`1000DB80`, `1000DB40`). The slot-4 routine at `10004E30`
  directly slerps the two endpoint quaternions with `geQuaternion_Slerp`
  (`1002BB60`); on equal key pointers it copies the quaternion. Before that,
  `10005150` calls `10004F90` across adjacent keys to flip quaternion signs
  when the retail continuity check finds a negative dot product. It does not
  build the control quaternions for mode 1.
- The three-slerp routine is slot `5`, entered at `10004E60` for rotation key
  mode `2`. It slerps endpoint quaternions and their precomputed controls at
  key offset `+0x14`, then slerps those results with weight `2t(1-t)`.
  `10005050` and `10004EF0` build those mode-2 controls. This distinction
  matters because the copied maps use mode `1`, not mode `2`.
- The slot-1 translation routine at `100157F0` evaluates cubic Hermite basis
  terms against the outgoing tangent at key offset `+0x1C` and incoming
  tangent at the next key's offset `+0x10`. Retail constants are `1.0` at
  `1005118C` and `3.0` at `10051D9C`, giving basis weights `1-3t²+2t³`,
  `3t²-2t³`, `t-2t²+t³`, and `t³-t²`. `100158D0` builds key tangents from
  neighboring times and values, with separate endpoint handling for looping
  tracks. These are the retail curve paths; they are not copied from the map
  explorer.
- `gePath_Sample` at `1000E380` samples rotation and translation separately,
  builds the rotation matrix, and writes translation into the resulting
  transform. Its helper selects surrounding key times and dispatches by the
  channel's sampler slot. The callback time and BModel update cadence are
  still a separate game-runtime question.

## Coverage of the copied maps

The retail-backed exporter decoded all 17 BSP MOTIONS chunks:

- 1,235 MOTN model records, each containing one PATH object.
- 2,470 present key tracks: one rotation and one translation track per path.
- All 1,235 rotation and 1,235 translation key lists use key mode 1. The PATH
  channel headers report sampler slots 4 and 1 respectively.
- Each channel has 1,111 explicit-time lists and 124 start-plus-step lists.
  Key lists contain 2–6 records in these maps.
- All 1,235 rotation tracks use quaternion WXYZ input; all 1,235 track-extra
  loop flags are zero, so all 1,235 PATH objects are nonlooping. The current
  reusable sampler is deliberately limited to this observed retail mode-1,
  nonlooping subset and rejects other variants instead of guessing.
- BSP SHA-256 values in `motions.json` match the package reports. For chunks
  present in the map, the stored chunk SHA-256 matches the byte-for-byte
  `chunks/24_motions.bin` copy.

`tools/decode_gbsp_motions.py` writes typed records to `motions.json`, including
the raw key lines and compression header. `SceneData` exposes the document,
and [`gbsp_motion_path.h`](../../../port/spikes/training-native-prototype/src/gbsp_motion_path.h)
now ports the retail direct WXYZ slerp, sign-continuity pass, nonloop key
bracketing/clamping, and cubic translation tangent/basis calculations for the
observed mode-1 subset. The decoder retains source identity and does not
replace the raw payload. Three catalog BSPs have empty motion records.

## Remaining work

This is data decoding and sampler code, not door behavior. The preview does not
yet animate rescue BModels from these paths. Next, map each `C_AnimModelN`
child to its `%Model%` record, and then connect the sampled transform to the
source-mapped BModel state machine. Shared touch/untouch dispatch and external
update cadence stay explicit follow-ups.

Binary/`0.F1` PATH variants, complete MOTN event/name-array use, runtime
transform composition, and bit-exact x87 behavior remain open. No rescue-game
code has been copied from the explorer or applied from its motion preview.
