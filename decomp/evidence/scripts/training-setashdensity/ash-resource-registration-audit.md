# Ash renderer resource registration audit

Snapshot: 2026-10-12. Read-only Ghidra 12.0.4 queries against the preserved
`FDNYFirefighter.exe` (SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`), using
`-noanalysis -readOnly`. The query covered xrefs and decompilation for
`00493D00`, `00493650`, `0049B3B0`, `0049AFE0`, `00495040`, `00494AD0`,
`00494A00`, and `0043B6E0`, plus assembly at the renderer loader/lookup paths.
No Ghidra project changes were saved.

## Source-backed registration path

- `0047D820` asks `00493D00` to resolve the ash record's ID. `00493D00`
  selects a lookup receiver using context byte `+0x160`: context `+8` when
  clear, or context `+0xB4` when set.
- `00493650` reads an array pointer at receiver `+0x48` and a count at
  `+0x4C`. It linearly compares each entry's second dword with the requested
  resource ID, then returns the entry's first dword. A miss with the logging
  flag set emits `Undefined texture resource #` from
  `ROC\Renderer\ROCRender.cpp` line `0x28a`.
- The RRF parser `0049B3B0` dispatches `ROC_ALPHA_TEXTURE` records to
  `0049AFE0`. That loader calls the renderer wrapper `00495040` at two sites.
  `00495040` reads its receiver's `+0x160`; the set branch adjusts `ECX` by
  `+0xB4`, and both branches tail-enter `00494AD0` with the same stack
  arguments.
- `00494AD0` checks receiver `+0x78` to choose how to represent the asset. In
  the set branch, the code opens the primary image, calls
  `geBitmap_CreateFromFile`, appends a two-dword `(bitmap pointer, resource
  ID)` record to the selected receiver's `+0x48/+0x4C` array, and registers
  the bitmap with `geWorld_AddBitmap`. If an alpha image is present, it loads
  that image, registers it, and calls `geBitmap_SetAlpha` on the primary.
  This two-word entry matches the shape consumed by `00493650`.
- When `+0x78` is clear, the code instead appends a 0x10-byte record through a
  separate `+0x18/+0x1C` array. Constructor `00494A00` places the primary and
  alpha path string fields at `+0/+4`, the ID at `+8`, and a mode word at
  `+0xC`. This path does not populate the `+0x48/+0x4C` lookup array in the
  inspected function.

The ID lookup is shared. The bounded incoming-reference query also found
direct callers from `0047EAC0`, `0047E5B0`, `0047DF10`, `0047D860`,
`0048B7C0`, `0046D6B0`, `004696A0`, `0046A390`, `00427F00`, and `004275A0`,
besides the ash helper `0047D820`; it also found one callsite at `00445FB1`
outside a defined Ghidra function. This is evidence for a common renderer
lookup service, not enough to classify those callers' exact runtime assets.

The bitmap table record format and RRF-to-loader route are now source-mapped.
The static trace does **not** establish that the receiver used by loader
`00495040` is the same runtime object/subobject passed to ash lookup
`00493D00`; those pointers enter through different call paths, and the
`+0x160` offset adjustments differ in the clear case. Nor does it establish
the loader receiver's `+0x78` field, whether IDs 200/201 are registered in the active lookup
array, or the returned runtime bitmap handles. The existing callback oracle
continues to use an explicit fake resource table.

## Next evidence

Capture the renderer receiver addresses, byte `+0x160`, and loader receiver
field `+0x78` while the RRF loader registers ash IDs 200/201, then compare those exact entries with the
receiver and result observed at `0047D820`. This resolves context identity,
backend selection, bitmap object handles, and the complete asset-to-polygon
edge without treating the callback fixture as runtime evidence. Preserve the
two array layouts in any future native resource adapter; do not collapse them
until the active Training path is known. See
`resource-render-followup.md` for the original asset paths and
`ash-native-render-bridge-audit.md` for the controlled callback comparison.
