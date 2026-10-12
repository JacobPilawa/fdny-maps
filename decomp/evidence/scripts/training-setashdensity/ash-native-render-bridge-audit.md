# Ash native draw bridge audit

Snapshot: 2026-10-12. This is a bounded source audit of the bridge from the
recovered ash descriptor helpers to the existing native draw paths. Original
EXE and Genesis.dll hashes, query scope, and import details are recorded in
`resource-render-followup.md` and `ash-presentation-component.md`.

## What the evidence closes

- `0047F960` calls `0047D6D0`, `0047D420`, `0047D820`, and `0047D8A0` before
  the first `geWorld_AddPolyOnce` at `0047FCF9`; the optional second call at
  `0047FE58` uses the corresponding next-frame helpers. The supplied
  `player_ash_presentation.h` reproduces the descriptor-derived byte channels,
  RGB, and scaled alpha operands.
- The callback writes the chosen position/color data into the ash object's
  vertex storage at `+0xD8`, with a `0x24`-byte stride. The Genesis.dll wrapper
  copies nine 32-bit words per vertex into a `0xBC`-byte polygon record and
  links that record into the world list through world `+0xBC4` and poly `+0xB8`.
  Independent instruction review confirms each vertex record is XYZ at offsets
  `+0..+8`, UV at `+0xc..+0x10`, and RGBA at `+0x14..+0x20`. Constructor
  `0047F850` initializes the four UV pairs to `(0,0)`, `(.99,0)`, `(.99,.99)`,
  `(0,.99)`; the writes in `0047F5A0` and `0047F960` leave those UV slots intact
  before submission. Native packet type and builder now model all nine words.
- The native renderer currently draws fire as four-position OpenGL quads in
  `FireRenderer`, and actors as ACT triangles in `VictimRenderer`. Those are
  distinct simplified draw paths; neither represents Genesis polygon
  submission or its world-linked record. The asset names for resource IDs 200
  and 201 are known (`ash1.bmp` and `ash2.bmp`), while the actual objects
  returned by the runtime renderer table are not.

## Why an exact native bridge is not yet derivable

The full vertex payload can now be compared at the callback boundary. The
RRF-to-renderer registration route, the `+0x160` lookup subcontext choices, and
two conditional asset-entry layouts are source-mapped in
`ash-resource-registration-audit.md`; the receiver identity, loader receiver's
`+0x78` backend field, and runtime resource/world-list state remain unknown.
The oracle supplies an explicit fake context, resource table, and world
pointer, then intercepts the imported call; it does not establish live
Training table contents or render-list behavior.

## Callback payload comparison

`src/player_ash_polygon.h` assembles four XYZ/UV/RGBA vertices from the cached
particle positions and source-matched descriptor helpers. State 0 explicitly
uses `particle.spawnPosition` for vertex 0, matching the callback's copy from
`+0x5c`; vertices 1–3 retain fixture-seeded geometry storage. The hash-checked
`tools/ghidra/AshParticlePolyPcodeOracle.java` executes original `0047F850`
and `0047F960`, intercepts the `geWorld_AddPolyOnce` IAT target, and records
the 36 caller-buffer words plus all seven cdecl arguments. Its six samples
include the constructor-default state 0 callback with a zeroed fake object
buffer, a controlled state 0 callback with nonzero spawn coordinates and
zero-seeded remaining geometry storage, and four timed or resource-variant
state 1 cases (the source does not show Training changing `+0xB0` to state 1).
State 0 submits a one-vertex count;
the 36 captured memory words are still recorded, with the unused vertex
storage explicitly initialized to zero by the oracle. The state 1 variants
submit four vertices and exercise frame 0→1 at quarter time, 1→2 at half time
for both resource IDs 200 and 201, and 2→3 at three-quarter time.

The native checker is `tools/player_ash_polygon_pcode_check.cpp`; its focused
CTest entry is `player_ash_poly_pcode`. Regenerate the fixture with the
read-only Ghidra command used by the other ash P-code oracles and
`AshParticlePolyPcodeOracle.java output.json original.exe`. Six callback
captures currently match all 36 words and the controlled world, resource,
vertex pointer/count, and remaining submission arguments bit-for-bit. This is
callback payload parity for supplied particle/context state, not live runtime,
world-list, bitmap sampling, or visible parity.

## Best next addresses

1. Trace the actual caller of `0047F960` and runtime value supplied as
   `param_1`, including context `+0x160`, selected lookup subcontext, and its
   world pointer at `+0x78`.
2. Capture the `00495040` registration receiver and `0047D820` lookup receiver
   while IDs 200/201 load and draw. Establish whether they are the same
   subcontext, which loader receiver `+0x78` backend is active, and which
   bitmap handles/material fields Genesis receives. Preserve both registration
   layouts.
3. Compare one live Training submission and resulting world-list record
   against the Genesis wrapper's copy. This needs a bounded runtime capture;
   static callback fixtures do not expose active world/list state.

No code change or visual-parity claim is made by this audit.
