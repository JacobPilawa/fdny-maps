# Training ash resource and render operands

Read-only Ghidra 12.0.4 follow-up against preserved `FDNYFirefighter.exe`
(SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`),
opened `-noanalysis -readOnly`. Queried decompilation for `0047D420`,
`0047D560`, `0047D6D0`, `0047D820`, `0047D8A0`, `0047D860`, `0047D960`,
`0047D3E0`, `0047D400`, `00493D00`, and `00493650`; read constants and the
existing `0047F960` instruction export. No changes were made to the Ghidra
project. The five descriptor words and flags referenced below are the prior
source-mapped findings in `descriptor-lifetime-followup.md`.

## Descriptor to presentation data

The renderer path consumes more of each descriptor than its animation timing:

- `0047D820` takes the current record's first dword (the authored ID 200 or
  201) and calls `00493D00(..., id, true)`. That function selects the active
  renderer resource table (`renderContext + 8` or `+0xb4`, according to
  context byte `+0x160`) and calls `00493650`.
- `00493650` linearly searches that table for an object whose `+4` field equals
  the requested ID and returns the object's first dword. Thus 200/201 are
  renderer resource keys, and `0047D820` supplies the resulting resource
  pointer to the polygon submission. The actual bitmap/material names and the
  table's runtime objects are not statically identified by this path.
- With Training flags `0x3c`, bit `0x20` is set. `0047D420` then interpolates
  the three floats at descriptor `+0x10..+0x18` between the current and next
  records using elapsed-frame fraction, caches the resulting triplet at
  particle `+0x24..+0x2c`, and returns it. `0047F960` multiplies those three
  channels by 255 and writes them into the per-vertex color records at
  `+0xec/+0xf0/+0xf4` and corresponding repeated vertex slots. The descriptor
  trailing triplets therefore feed interpolated RGB/color data, not vertex
  positions. Literal endpoint triplets are already tabulated in the prior
  note.
- Bit `0x10` is also set. `0047D8A0` interpolates descriptor `+0x0c` across
  current/next records (or returns the current value when this flag is clear);
  `0047F960` multiplies this result by particle `+0xa8` for the polygon's final
  alpha operand. Training's authored `+0x0c` values are all `0.2`.
- `0047D6D0` reads the current and next records' `+8` values, converts them
  through the 255-scale/round helper to bytes, then—because bit `0x4` is set—
  interpolates the two byte channels by elapsed-frame fraction and multiplies
  by particle `+0x20` before its final byte conversion. Training's `+8` words
  are `1,1,1,1,0` as float bit patterns; this path does not consult shared RNG.

The submission path has a dynamically dispatched call at `0047FCF9` and the
same call at `0047FE58` for the optional second frame. Ghidra names the target
`geWorld_AddPolyOnce`, but the instruction is `CALL dword ptr [0x004DC0D0]`;
that cell contains `0x000FD69E` in the static image, which is not a mapped
function address, so a concrete runtime target is not established here. The
recovered first-call operands in decompilation are the selected world/render
context, particle vertex buffer (`this + 0xd8`), state from `+0xb0`,
frame-derived value, descriptor-resource lookup result, particle field
`+0xa4`, and the alpha product above. The second call has the analogous
operands, using the next-frame helpers `0047D560` and `0047D960`.

This closes a concrete descriptor-to-render-data path: record ID selects a
renderer resource object, its trailing triplet supplies interpolated vertex
color, and `+0x0c` supplies the final alpha factor. It does not identify the
resource object's runtime table state, resolve the indirect call target, or
establish live Training vertex positions/visibility. The preserved
`evidence/original/rescue.rrf` resolves the authored names: ID 200 is
`data\sprites\ash1.bmp` (line 60), and ID 201 is `data\sprites\ash2.bmp`
(line 61). Both original 16×16, 8-bit BMPs were found in the installed game's
`Data/sprites/` directory and copied byte-for-byte to the ignored local
`assets/training/fire-sprites/` directory. Their SHA-256 hashes are recorded in
`evidence/asset-manifest.json`; the proprietary bitmaps themselves are not
staged for the public source mirror. This resolves asset availability, but no
pixel match or exact visual reproduction is claimed.
