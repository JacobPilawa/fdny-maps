# Shipped Genesis collision path

Updated: 2026-10-13

## Pinned input and method

The analyzed input is the supplied `Genesis.dll`, PE32 x86, SHA-256
`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
Ghidra analysis was run against that exact binary. The decompilation excerpts
for the eight reviewed functions are retained in
[`genesis_collision_functions.c`](../genesis-boundary/genesis_collision_functions.c).
The Ghidra database could not be saved because this program did not have a
configured save location; this report and the pseudocode export are the durable
record of the current analysis.

## Public call boundary into Genesis

The EXE wrapper at `0x00492CE0` passes its current world to
`geWorld_Collision` at `0x10001E30`, with caller-supplied mins/maxs, start/end,
contents mask `0x43`, model-collision flag `2`, and zeroed optional arguments.
It copies the collision result's hit position and normal to its output pointers.
The wrapper and saved decompilation are in
[`00492ce0_collision_wrapper.c`](../genesis-boundary/00492ce0_collision_wrapper.c).

`geWorld_Collision` initializes the output record and dispatches based on the
extent pointers: null mins/maxs select the point/ray route
`FUN_10038470`; non-null extents select the swept-hull route
`FUN_100372A0`. Thus the general player wrapper uses the hull route, while
point traces and movement extents share a top-level engine API.

## Extent/swept-hull route

`FUN_100372A0` prepares the swept query bounds and scans world models. It skips
models with the observed disabled flag, applies the optional model callback,
rejects models whose world bounds do not overlap the query, transforms the
query into model-local coordinates where needed, and traverses that model's
BSP. It keeps the nearest accepted hit and converts the contact point/normal
back through the model transform before returning the result.

`FUN_10035470` builds the swept AABB from hull mins/maxs and segment endpoints.
`FUN_10035210` classifies that AABB against a BSP split plane and returns side
bits used to decide which child nodes may contain a contact.
`FUN_100371F0` recursively follows those child bits. Negative node indices
identify leaves; it intersects the leaf contents with the requested mask and,
when the leaf has collision planes, invokes `FUN_10036EA0` to test the convex
leaf against the swept hull. The traversal returns the closest relevant
contact through shared query state.

## Point/ray route

`FUN_10038470` scans eligible models and performs broadphase and optional
model-space transforms before entering `FUN_100363F0`. That routine recursively
classifies the segment endpoints against BSP split planes; negative node
indices identify leaves, which are accepted only when their contents intersect
the requested mask. This route supplies point/ray collision without a swept
box. It shares the same model list, contents filtering, transform handling,
nearest-hit selection, and result conversion as the hull route.

## Port evidence and current limits

The reused C++ Training collision implementation already has direct oracle
evidence against unmodified 32-bit Genesis.dll for static Training world BSP
queries. Its 1,000 swept-hull samples report zero mismatches and maximum point
error `7.45058e-09`; its 207 point traces report zero mismatches and zero point
error. The corresponding old-work oracle JSON and provenance are copied under
`port/spikes/training-native-prototype/evidence/` in the local work folder.
Those fixtures cover static BSP world models with identity transforms and do
not establish dynamic actor, moving-submodel, or complete game-runtime parity.

The current rescue preview failure was above this collision algorithm: absent
`RCS_BoundsTrigger.model` fields were interpreted as model 0, which disabled the
entire world BSP for rendering and collision. That conversion is fixed in both
the map-neutral viewer and Training workbench. All seven rescue-map spawns now
remain on the world floor during the five-second movement check. The full
per-map OpenGL and collision results are in
[`scene_preview_collision_texture_diagnostics.md`](scene_preview_collision_texture_diagnostics.md).

The generated oracle JSON and provenance copied from the earlier work folder
are ignored by Git and remain local; reproducing those numerical checks from a
fresh clone requires supplying those same fixtures. The hash-pinned Ghidra
pseudocode and all summary results are tracked in this repository.
