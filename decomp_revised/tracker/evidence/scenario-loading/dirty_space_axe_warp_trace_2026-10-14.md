# Dirty-space collision and Axe trace (2026-10-14)

## Scope and provenance

This is a source-level investigation of the reported “dirty space” correction
and apparent Axe copies at survivors or spreading fires on MBase, Boat, and Gas
R. The pinned input is `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra 12.0.4 headless ran against the existing `analysis/ghidra/FDNYPort`
project with `-readOnly -noanalysis`; analysis/script changes were discarded.
The addresses `0043F810` and `00452570` had no function boundaries in that
project, so the scratch script disassembled and created those boundaries only
in memory before decompiling them. Their vtable pointers were cross-checked
against the pinned executable. Later Player-writer follow-up queried the saved
Ghidra project read-only (`-noanalysis`) and inspected pinned disassembly; the
Axe attachment, visual-origin, and candidate-lifecycle follow-ups cross-checked
saved address-matched exports, raw disassembly, and local ACT data. These
follow-ups did not run a new full analysis. No runtime capture of the retail
game was available.

## Findings

**A downward collision callback can move the canonical Player transform to its
hit point.**
PlayerRescue's root vtable address point is `004E1C48`; slot `+0x38` at
`004E1C80` contains pointer `0043F810` (raw bytes `10 f8 43 00`). Player
update `00452570` calls collision loop `0043F6D0`. That loop asks the object's
vtable slot `+0x0C` for the current collision hull, calls `00492CE0` for a
world collision sweep, and invokes slot `+0x38` with the hit position and
normal, up to five times.

Raw call-site disassembly confirms `00452570` passes the Player root to
`0043F6D0`, which dispatches slot `+0x38` on that same root to `0043F810`.
Player transform `+0x24` is a `FrameTransform`; its translation at transform
`+0x28..+0x30` aliases Player root `+0x4C..+0x54`. The `+0x0C` getter thunk
`0049DB90` reaches `0049D790`, which computes world origin from that transform,
writes it into derived cache `+0x68..+0x70`, and returns the cache. In its
final downward-sweep hit branch, `0043F810` first writes the hit coordinates
directly to root `+0x4C..+0x54`, then mirrors them into the `+0x68` cache
through `00441E90`. This is a source-confirmed Player transform snap to that
collision hit. It occurs only inside the callback's nested sweep path and its
state/normal gates; it does not select an arbitrary nearest entity. The outer
collision loop can reuse the corrected cache for up to five iterations.
`0049E040`'s finalizer sets collision state and clears motion-delta fields.
Other callback paths call `0049E0A0` to project velocity and motion vectors
against the plane normal without changing the transform. `00452570` continues
after the sweep and integrates movement delta into the corrected translation,
so the final per-frame position can include that later integration.

The local movement path is `Player root slot +0x14` (`004E1C5C` → `0044FE20`)
to `00470740(Player+0x140)` and movement-subobject slot 0 (`00452570`). This
proves the order of collision handling and movement-delta integration inside
`00452570`, but not how often that path runs. The separate Player root slot
`+0x64` calls integrator wrapper `0043F680`; no static recurring dispatcher
or ordering between that wrapper, movement, and ScriptActor events is mapped.

Both Player movement and Axe strikes call `00492CE0`, which forwards a swept
hull to Genesis `geWorld_Collision` with mask `0x43` and collision type `2`.
The Axe uses a separate post-hit candidate scan. `00460B90` first advances its
animation and requires the strict `+0x340 < 0.275 < +0x33C` crossing. It then
sweeps a `+/-6` box using Player frame data and receives the world hit point in
a stack local. On a miss, it returns before strike effects or candidate
processing. On a hit, it checks the impact point (offset by six units along
the Player frame axis) against type-4 objects' world AABBs expanded by eight
units. Only an eligible object proceeds to `0043E2A0`, which requires nonzero
hit points at target UserData `+0x34`, `G_Liftable == 0` at UserData `+0x38`,
and an allowed strike-time value at object `+0x1EC`; it sends `chop`,
decrements hit points, and on zero sends `break` and invokes `0043D660`.

The Axe hit point remains in the strike stack frame. The reviewed strike code
does not copy it into Player origin or use a survivor or fire as a direct Axe
parent. However, the parent behavior after a world hit needs a correction:
before scanning candidate objects, `00460B90` calls `00483A20` on the Axe
transform component at `+0x15C` with a null parent. `00483A20` writes null to
the component's parent field at `+0x70`; because the new parent is null, it
skips matrix and translation updates. The last cached transform is therefore
retained while detached.

The Axe collision query is also positioned from the owning Player frame at
`Player+0x34A4`: both sweep endpoints and the strike orientation use that frame
before `00492CE0` runs. An already-invalid Player frame can therefore place the
Axe query in invalid space. The world hit returns through stack-local outputs,
though; the reviewed Axe sweep still has no source-confirmed assignment of that
hit point to Player translation. This leaves two separate coordinate paths to
test: Player frame data can affect where the Axe sweeps, while Player's own
downward collision callback can accept a hit and move Player.

The strike sets child state 2 and scans the type-4 candidate list. If no
eligible BModel accepts the hit, the no-candidate branch skips the immediate
reparent and skips the global result flag used by the later reparent branch.
The accepted result-bit-1 branch reparents the component to Player frame
`+0x34A4` and sets state 3. Reparenting can also happen conditionally: the
state-2 think callback reparents when the cached Axe position is more than 20
units from the Player, and the held-action callback reparents after its 300 ms
timer expires. The release callback is a no-op, and the animation-state reset
does not itself restore the parent pointer. This is a verified detached,
cached-transform seam and a stronger candidate for an apparent extra Axe than
direct attachment to a victim or fire. It still does not prove which model is
visible, where it appears, or that the Player warps there.

**Two Axe child models can be enabled at once, but only after a specific
duplicate-equip path; a no-candidate hit can leave the primary detached.**
`00460A50` initializes the root gate at `+0x1FC` to zero and creates a
flag-true child using model 6 `axel.act` and a flag-false child using model 1
`axe.act`. Each child has its own mode-2 ActorInfo render component.
`0045F9B0` activates the primary child only when the root gate is nonzero, then
activates the secondary child; ordinary first-time selection activates only
the secondary model-1 child. The only direct caller found for `0045F9F0` passes
1 from the existing-Axe/duplicate `equip axe` branch at `0044D152`. If the Axe
is already active, that path activates the primary immediately while the
secondary remains active; if inactive, the later selection callback sees the
set root gate and activates both. Each child activation sets its ActorInfo
visibility byte through `00497DE0`, and render callback `004926E0` checks that
byte before calling `geActor_Render`. Boat A and B startup scripts each contain
one authored `equip axe`, so this audit found no startup route to the duplicate
branch; later runtime equips remain unknown. The root gate and visibility
during the reported Boat incident are unknown.

On a successful world hit, `00460B90` detaches the primary child's transform
from Player frame `+0x34A4` but does not clear its render gate. With a null
parent, `00483A20` skips matrix/translation updates, and the follower updater
`00483720` does no work; the detached child can therefore retain its cached
transform while it remains visible. The secondary child can stay attached to
the Player. If no eligible type-4 BModel accepts the impact, the strike skips
the immediate reparent, leaving a source-supported path to two Axe visuals at
different transforms if the primary gate was enabled. The primary child is
later reattached only by conditional state-2 distance recovery, a timer path,
or item deactivation. This strengthens the duplicate-visual hypothesis, but
does not establish a user-visible frame, an Axe at a victim/fire, or a Player
warp. The model-6 stale-visual route also requires the duplicate-equip gate
path described above; normal first-time selection alone leaves that child
inactive.

The primary strike also samples ACT bones at `0045FB90` into Axe fields `+0x35C`,
`+0x344`, and `+0x350`; `0045FEC0` copies the latter two translations into
model-instance records in the Axe object's own `+0x200` actor list. The reviewed
candidate scan checks type-4 targets and BModel bounds, but writes neither the
impact point nor those model-instance transforms into a VictimRescue or
FireRescue origin. Victim setup uses its own ActorInfo frame parented to the
victim root, consistent with a separate transform chain.

The accepted candidate does not become the Axe frame's parent: the immediate
detach uses a null parent, and the result-bit reparent targets Player frame
`+0x34A4`. `0043D660` changes BModel command state or dispatches its `open`
route; it does not write Player translation. The Boat A/B exports contain no
nonempty `chop` or `break` handlers on the reviewed Axe-eligible BModels, so
this source pass found no authored Boat script path from an Axe hit to a
Player warp. Runtime candidate lists and dynamically attached script actions
remain unverified.

**Passout copies a safe-position snapshot, then resets to a separate checkpoint.**
The constructor zeros Player vector `+0x3400..+0x3408`. In the normal branch of
`00452570`, Player fields `+0x4C..+0x54` are copied into this snapshot only after
the four directional burn flags clear, accumulated burn falls below `0.1`,
`+0x12C` is set, and oxygen `+0x3450 > 0.5`. `0044FF10` first copies that
snapshot into canonical translation `+0x4C..+0x54`. If a victim is carried,
that temporary position is used by the drop path before its X correction.

The first trace overstated this write as the final Player destination. The
tail of `0044FF10` calls `0044CD10`, which overwrites Player translation from a
separate checkpoint at `+0x534..+0x53C`, copies that checkpoint into
`+0x3400..+0x3408`, resets movement/status fields, rebuilds zone membership,
and invokes `0049DA00` for floor placement. Player initialization seeds the
checkpoint from the starting frame; Player escape prepass `004513A0` can refresh
it from the current transform when byte `+0x12C` is nonzero and oxygen remains
strictly above `0.5`. Thus the delayed state-6 route (`00451080` → `0044FF10`)
and the Player ScriptActor `reset` route both end at this checkpoint/floor
placement path. The `+0x3400` snapshot remains relevant to a carried-victim
drop and as an intermediate transform write, but it is not established as the
final post-passout Player location. Whether this checkpoint path matches the
reported Boat incident is unknown.

State 6 runs its one-second view transition before calling `0044FF10`. The
separate integrator audit at `0049DBC0` found no copy of prior motion values
into the origin cache. Player byte `+0x12C` is now source-mapped as a transient
collision latch, not an Axe coordinate: `0049E0A0` sets it when the collision
record's `+0x10` field is at least `0.5`, and the physics integrator copies it
to `+0x12D` before clearing it. The checkpoint writer `004513A0` is reached by
the Player ScriptActor's `escape` command, not by a per-frame prepass. When
`+0x12C` is nonzero and oxygen exceeds `0.5`, it copies the current Player
translation into checkpoint `+0x534..+0x53C` before dispatching escape
completion. A later reset can therefore restore a position that came from a
collision correction, but only if the escape command runs while the latch is
set. This collision-to-checkpoint chain is source-backed at each step; its
timing and relationship to the reported Axe glitch remain open.

## Collision latch to checkpoint path

`0043F810` uses `0049E0A0` for accepted collision-response paths, while its
downward-sweep correction branch separately writes the selected hit point
straight to Player translation `+0x4C..+0x54`. The `0049E0A0` helper sets
Player byte `+0x12C` when the collision record's field at `+0x10` is at least
`0.5`, then projects velocity and movement vectors against the collision
plane. The direct downward-snap branch does not call `0049E0A0`, so the source
does not establish that this same hit sets the latch. `0049DBC0` copies the
byte to `+0x12D` and clears `+0x12C`. A
read-only Ghidra follow-up to the saved project found Player root vtable address
point `004E1C48` slot `+0x64` at `004E1CAC` points to wrapper `0043F680`; that
wrapper calls `0049DBC0` and then clears Player byte `+0x130`. The table entry
confirms the integrator wrapper is in the Player virtual-method surface, but
does not establish its scheduler order relative to movement collision or
script events.

`004513A0` is dispatched from the Player script handler `00451870` only for
the `escape` command. If the transient `+0x12C` latch is still set and oxygen
`+0x3450` is above `0.5`, it copies current translation `+0x4C..+0x54` and
heading `+0xD0D` into the checkpoint `+0x534..+0x53C`/`+0x530`; the later
`0044CD10` reset restores that checkpoint and applies floor placement. Thus a
collision-corrected out-of-bounds position could become a later reset target
through this route. This is not a direct Axe-hit-to-Player assignment: the Axe
strike result remains stack-local, and no reviewed code connects its impact
output to `+0x12C` or Player translation. Whether an escape command can observe
the latch before the physics integrator clears it depends on event/update
ordering that has not been established. The vtable relationship narrows the
missing question to runtime ordering of the Player collision/update callback
and the Player ScriptActor's `escape` event; no reviewed source establishes that
order.

## Boat episode static map pass

The user identified the Boat episode but does not yet know the landmark, action,
or destination, and suspects the issue concerns Axe/Player coordinates in
invalid or out-of-bounds locations rather than an ordinary gameplay mechanic.
I inspected both local Boat exports. Boat A (`boat_a.bsp`) has
SHA-256
`e1685b45ef29aa075c688985617b593079a6aad8a6b493bbae327356bd45ad44`. Its
`boat_a.evs` SHA-256 is
`d0e8d416a87bf24710835dac8c394dcbc5fec24021698f4ddf7677ba58f5b453`. Boat B's
BSP SHA-256 is `4ad7de96916b5c265a4cdd6c477d96a0b6f9cc6e8151c96ff2e1ef8a31313a50`
and its `boat_b.evs` SHA-256 is
`3d1de56a39ed7e92efef26d48c5127517fc5439dc20499f9f7d3eff323ce227c`. The
entity, script, collision, and geometry crosswalk below comes from the local
`.local/exports/boat_a/` and `.local/exports/boat_b/` bundles joined to these
hash-pinned BSP/EVS inputs; those generated exports are ignored local files.
Boat A's entity table contains 322 entries, including 47 `RCS_FireRescue`, 9
`RCS_VictimRescue`, 26 `RCS_BModel`, and 16 `RCS_BoundsTrigger` entities. The
player spawn is `(-1253, 16, 121)`. The world collision model spans
`(-1702, -1224, -283)` to `(2019, 401, 8856.59)` and has 104 models; all
entities with an exported `location` or `ZZ_location` point were within those
global bounds. This simple bounds check does not rule out bad local brush
geometry or a runtime collision seam.

The exported `boat_a` scripts contain no literal `warp`, `teleport`, or Player
position-set command. They do include ordinary rescue/escape callbacks and a
final-exit transition to `boat_b`. Nothing in this static map pass identifies a
scripted teleport matching the report, and the global bounds check does not
identify the specific collision surface involved. Boat B is the linked stage
loaded by Boat A's `finalexitloadmap boat_b`; its BSP SHA-256 is
`4ad7de96916b5c265a4cdd6c477d96a0b6f9cc6e8151c96ff2e1ef8a31313a50`. It has
164 entities, including 42 fires, 5 victims, and 6 BoundsTriggers. Its world
model spans `(-1788.95, -1098, 1201)` to `(1156.95, 1058, 8856.59)` and has 17
models. Boat B scripts also contain no literal `warp`, `teleport`, `reset`, or
`passout` command. Boat A entity 197 links model #17 `oxy2`, bounds
`[440.119,-205,6820.819]..[1444.12,195,8820.819]`; Boat B entity 62 links model
#6 `oxy2`, bounds `[-1142,-951.064,6802]..[-138,-523,8802]`. Both bind
`touch: tell player oxyon` and `untouch: tell player oxyoff`. These authored
triggers can start oxygen drain; they do not prove the Player reaches depletion
during the reported incident. Fire exposure and oxygen depletion can enter
state 6; its later reset path applies the separate Player checkpoint and floor
placement described above. No match to the reported incident is established.
Authored/global bounds checks do not rule out runtime BSP or model collision
seams.

Boat A's strongest Axe-target lead is the office/manager cluster: `officezone`
trigger e222's model bounds contain victims e194 `fatman` `(551,-280,1632)` and
e195 `fatman1` `(451,-52,1344)`, fires e124 `firetable` `(255,-84,1524)`, e137
`firetable1` `(531,-316,1804)`, and e138 `firetable2` `(365,-316,1831)`, plus
nearby BModel e289 `breakable_chair2` `(466,-262,1633)`. The chair's collision
AABB does not overlap the victim or fire points, so proximity alone does not
identify the Axe candidate or explain a warp.

Boat A's outlying `propAzone` e219 and `propBzone` e220 brushes have contents
0/4, not the Player collision mask `0x43`; their bounds extending outside the
world model therefore do not establish a solid collision seam. A separate
possible map-edge lead is Boat A solid `bboxclip1/2/3` models around X=1780–1925,
Z=3494–3868, near breakable BModels e297–299 and fire e133 `firefork7`; runtime
contact remains unverified.

Boat B has the strongest concrete geometry lead if the report occurs near its
opened hook/top deck. BModel e121 `button` references animated `%Model%` hook
#13 at `(-586.5,905,3922.5)`, with AABB `[-626.5,667.573,3881.5]..[-546.5,
1474,3961.5]`. This extends 416 units above world-model maximum Y=1058. The
pinned Genesis swept-hull path scans eligible BSP submodels, and 31 hook leaves
intersect Player mask `0x43`; the hook remains a candidate only when its runtime
flag and transform allow collision. Adjacent BSP model #14 `ladder8` also has
one leaf whose contents intersects `0x43`. Entity e120 is an `RCS_BModel` with
a touch action that calls Player `climb 1`, not a BoundsTrigger. These are
conditional Player-sweep candidates outside world-model-0 bounds, not proof of
a defect or of a match to the incident. See
[`genesis_collision_path.md`](../runtime/genesis_collision_path.md) and
[`genesis_collision_functions.c`](../genesis-boundary/genesis_collision_functions.c).
Boat B rescue3 trigger e61
has a large mismatch between its `ZZ_location` and contact brush, but the
source BoundsTrigger check uses the linked brush bounds; that anomaly alone
does not move the Player.

The BModel state/contact path does not establish that either collision model
is enabled. `0043E680` maps `open` to owner state 3; `0043DC60` samples and
updates the child transform, then calls `0043C1A0` at state/endpoint changes.
The helper calls Genesis `geWorld_OpenModel`, which changes open/portal state
at model `+0xC8`. Genesis swept collision separately checks model flags at
`+0xCC` and skips a model when bit `0x10` is set. The inspected open/update
path does not call `geWorld_ModelSetFlags` or prove the `+0xCC` value. e120's
climb touch candidate is handled through a separate Player overlap/contact
path and also does not toggle that collision flag. Thus #13/#14 remain
conditional until the runtime flag or an actual collision result is observed.
The flag lifecycle and contacts are cross-checkable in
[`0043e680_bmodel_dispatch_fresh.c`](0043e680_bmodel_dispatch_fresh.c),
[`0043dc60_update_bmodel_entity_fresh.c`](0043dc60_update_bmodel_entity_fresh.c),
[`0043c1a0_FUN_0043c1a0.c`](../../../reference/previous-work/decomp/evidence/item-pose/live-extinguisher/message-routing/0043c1a0_FUN_0043c1a0.c),
and [`0043b620_bmodel_contact_candidate_fresh.c`](0043b620_bmodel_contact_candidate_fresh.c).

Boat A's oxygen trigger is a separate possible passout route. If it reaches
state 6, the final reset destination is the checkpoint/floor path, not the
`+0x3400` snapshot. The user does not know whether oxygen or passout was
involved, so this remains secondary to the out-of-bounds collision hypothesis.

## Cross-map out-of-bounds collision comparison

Generated ignored exports under `.local/exports/{mbase,boat_b,gas_r}/` were
cross-checked against the map BSP hashes recorded in their `report.json`
metadata. Gas R's source BSP hash `f6822f8d598d2888896165bbee8b554351230cd76ad6331abf0d735a03b54cbc`
and Boat B's `4ad7de96916b5c265a4cdd6c477d96a0b6f9cc6e8151c96ff2e1ef8a31313a50`
were independently recomputed from local source files. MBase's report records
`4ff3b18dd809c619a6734673a7f90bd2e98d9fc9518ee7bf94ece4b6ff90b266`; that
source BSP was not available locally for independent recomputation. The
exporter labels `0x43` as the port's runtime mask choice; the retail wrapper's
`00492CE0` passing that mask and Genesis leaf filtering by `contents & mask`
are separately established by the pinned engine trace.

| Map | Candidate geometry beyond world-model-0 bounds | Player-mask result | Assessment |
|---|---|---|---|
| MBase | No named non-world BModel model extends beyond world-model-0. Outlying `escape3/5/6/7` and `underground2_fog` are trigger/fog models. | Their leaves have contents 36 or 4, so those named brushes miss `0x43`. Separately, 292 world-model-0 convex leaf volumes cross its reported bounds (282 contents 1, 10 contents 33). | Shared world-model edge-space solid is a conditional candidate; the trigger/fog brushes are not. The solid's intended role is unknown. |
| Boat A | No named non-world BModel extends world-model-0 bounds. Outlying propeller-zone brushes are not Player-mask solids. | 211 world-model-0 convex leaf volumes cross bounds (154 contents 1, 57 contents 33); the maximum protrusion is about 128 units. | Shared edge-space candidate; Boat A also has an in-bounds office Axe-target cluster and an outlying bboxclip area. |
| Boat B | Hook model #13 extends 416 units above world-model-0 Y max; ladder model #14 is adjacent. | 149 world-model-0 convex leaves cross bounds (95 contents 1, 53 contents 33, 1 contents 97). Hook has 31 mask-relevant leaves total but only one of six outside-bound leaves is relevant; ladder has one contents-104 leaf. | Shared world-model edge-space candidate plus conditional hook/ladder submodel candidates. Runtime flags and transforms decide whether those submodels are queried. |
| Gas R | No named non-world BModel model AABB extends beyond world-model-0; `Pump8` has an outside-bound leaf with contents 0. | 158 world-model-0 convex leaves cross bounds (150 contents 1, 4 contents 33, 4 contents 97); maximum protrusion is about 128 units. | Shared edge-space candidate; the outside Pump8 leaf itself cannot intersect `0x43`. |

The counts use each world leaf's oriented side planes to solve its convex
volume; they are not counts of oversized stored AABBs alone. In every map, the
listed mask-intersecting contents include solid bit `0x1`. An independent
read-only pass rehashed all four BSPs and found that the root-node bounds and
mask-solid convex leaves reach the nominal model-0 envelope expanded by 128
units on each face, within small floating-point serialization differences.
The resulting measured envelope of outside-crossing solid leaves is:

| BSP | Convex solid-leaf envelope beyond model-0 bounds |
|---|---|
| MBase | `(-896,-760,-7284)..(2680,584,1624)` |
| Boat A | `(-1830,-1352,-411)..(2147,529,8984.586914)` |
| Boat B | `(-1916.948853,-1226,1073)..(1284.945313,1186,8984.586914)` |
| Gas R | `(-2976,-752,-2860)..(4168,441,3264)` |

The matching-family public GBSPLib source offers a concrete builder
explanation for outside solid leaves. `CreateAllOutsidePortals` creates an
outside node with `BSP_CONTENTS_SOLID2`, expands its node bounds by 128 units
per axis, and creates six portals; later `CreateLeafSides` builds collision
sides for leaves whose contents intersect `BSP_CONTENTS_SOLID_CLIP`. The
definitions include `SOLID2` in that mask. This supports interpreting the
outside solid leaves as intentional compiler-generated exterior collision
geometry, rather than evidence by itself of retail BSP corruption. The same
source's header describes padded root bounds for frustum culling; the padded
root AABB alone does not cause Genesis retail collision. In the retail engine,
the model-bound broadphase still has to overlap the swept query, and the
traversed leaf planes determine a hit. The public builder is same-family source,
not proof that this exact compiler version generated these maps, so the leaf
geometry's role in the reported incident remains unverified.

Genesis' model-bound broadphase limits a stationary query to the overlap
region, while a swept AABB covers its start/end path. A sweep that overlaps the
model bounds can therefore traverse the outside leaf volume; the contact point
is adjusted by hull support planes before the engine interpolates the
crossing. This is a plausible shared edge-space collision path, and the
compiler evidence makes intentional exterior collision geometry a concrete
explanation. Neither source trace proves it was hit in the retail incident.

The world-model-0 pass above finds mask-solid outside leaf volumes in all four
scanned BSPs across the three reported episodes. Genesis' swept-hull route
rejects a model when the query AABB does not overlap that model's bounds, so a
local query far beyond the map cannot hit these leaves. Near the boundary,
however, the retail Player hull `(-30,-30,-30)..(30,85,30)` or Axe's six-unit
sweep can overlap model 0 while traversing an outside solid leaf. This can
produce an Axe world hit/detach and, separately, a Player collision callback
hit. The Player callback's nested downward sweep can then write its returned
contact point to canonical translation. This is a concrete shared
coordinate-path hypothesis, not evidence that the hit occurred in the reported
session. Same-family builder source indicates outside solid leaves are an
intentional construction; exact compiler provenance and runtime behavior
remain open.

## Working hypotheses

**User clarification (2026-10-14):** this occurs in the original retail game.
Prioritize coordinate handling for the Player and Axe in invalid/out-of-bounds
space. Treat an Axe swing as the condition exposing the fault, not as evidence
of an authored gameplay mechanic. The oxygen/passout/checkpoint path below is
secondary unless a reproduction shows the Player position changes during that
route.

- **Local port coordinate cross-check:** `Player::step` integrates the Player
  position without a lower-world-bound reset when the collision sweep finds no
  solid. The Axe query is then built from the post-step camera frame, whose
  translation is derived from `player.p`; the query hit is used for BModel
  candidate dispatch and is never assigned back to `player.p`. The rendered
  port Axe is also placed camera-relative each frame. Thus, in the current
  port, falling outside collision geometry can send the Axe query outside the
  world and produce misses, but this code path does not warp the Player or
  leave a world-space Axe at a survivor/fire. The port's movement and view
  frame remain approximations, so a discrepancy there is still possible.

- **Verified mechanism, uncertain match to the report:** a successful world
  hit detaches the primary Axe child's transform before the candidate scan.
  Root gate `+0x1FC` starts at zero, and the only direct activation caller found
  is the duplicate `equip axe` path; ordinary first-time selection enables only
  model 1 `axe.act`. If the duplicate path has enabled model 6 `axel.act`, a
  no-candidate hit can leave that child at its cached transform while model 1
  stays attached. Boat A/B startup scripts each contain one Axe equip, so this
  stale-primary visual requires a later runtime route not found in startup
  data. The incident's gate state and visual remain uncaptured.
- **Verified checkpoint reset, uncertain match:** state-6 timeout or Player
  `reset` first copies the `+0x3400` snapshot into canonical translation, then
  `0044CD10` overwrites it from checkpoint `+0x534` and applies floor
  placement. Separately, an `escape` command can refresh that checkpoint from
  current coordinates when collision latch `+0x12C` is set and oxygen is
  above `0.5`; no match to the reported out-of-bounds behavior is established.
- **Verified downward-sweep correction, shared candidate:** the
  `0043F810` callback can write a swept-hull hit from an eligible BSP model
  directly into transform translation `+0x4C..+0x54`, then refresh cache
  `+0x68..+0x70`. World-model-0 mask-relevant solid leaf volumes reach a
  nominal 128-unit envelope outside model bounds on MBase, Boat A/B, and Gas R.
  Same-family compiler source creates intentional outside solid leaves, so
  their presence alone does not imply a defect. Near the boundary, Player and
  Axe hulls may overlap the world-model broadphase and reach those leaves. The
  Boat B hook/ladder remain additional conditional submodel candidates. No
  runtime hit has been captured.
- **User's current lead: invalid Player/Axe coordinates.** Axe sweep endpoints
  are derived from Player frame `+0x34A4`, so an invalid Player frame can place
  the query in invalid space. Player collision response can alter canonical
  translation and set transient latch `+0x12C`; an `escape` command can then
  preserve current coordinates as the later reset checkpoint. No source
  currently proves that Axe collision sets this latch or that this chain
  matches the retail out-of-bounds report.
- **Unsupported direct Axe-to-Player warp:** the Axe's stack-local impact
  point is not assigned to the Player transform, the type-4 candidate is not
  used as the Axe's parent, and the reviewed Boat BModels have no authored
  `chop`/`break` route that changes Player position. The detached transform may
  explain a visual seam, but no source path yet joins it to the Player
  checkpoint reset.
- **Open:** whether root gate `+0x1FC` and both child visibility gates are set
  during the Boat incident; the exact actor/model type represented by any
  apparent duplicate, its lifetime and transform owner; any non-world contact
  or rescue callback that changes Player position; and the condition that
  makes the visual appear at a victim's feet or a spreading fire.

## Next evidence that will distinguish the paths

The user does not yet know a precise Boat repro. Static cross-map BSP scans can
continue; a runtime capture will still be needed to match one path. Near the
map edge, record whether the winning model is world model 0 or a submodel; the
outside solid leaves make this distinction important. In Boat B's hook/ladder
area, also read Genesis model flags through
`geWorld_ModelGetFlags` (`0x1003C170`) for models #13/#14 and record their
transforms. During movement/fall and Axe swings, at the `geWorld_Collision`
boundary record the hit point and normal, caller, model collision flags and
transform, and winning model pointer/index if exposed. If the result record
does not expose it, instrument the engine's accepted leaf/model. Identify
whether the hit came from world model 0 or a specific submodel. Sample Player
translation `+0x4C..+0x54`, derived cache `+0x68..+0x70`, collision latches
`+0x12C/+0x12D`, checkpoint `+0x534..+0x53C`, safe-position snapshot
`+0x3400..+0x3408`, state and state-6 timer. If a later reset follows, record
whether Player `escape` dispatched between the collision and reset, and
whether the checkpoint changed at that command. Across each Axe swing, sample
root gate `+0x1FC`, both child ActorInfo visibility bytes, active child state,
and each child's follower-frame parent pointer, cached transform, and ActorInfo
matrix. Record every Axe equip after map startup: primary model 6 is enabled
only through the duplicate-equip route found in this audit. Also record the
strike candidate type/result. If Player translation
changes to the selected collision point at `0043F810`, that identifies the
sweep correction. If it changes after state 6, check checkpoint `+0x534`; a
`+0x3400` value alone does not identify the final destination. If only the Axe
transform becomes detached while Player translation is stable, that
identifies the visual path.

## Evidence files

The Player transform and collision findings are cross-checkable in
[`0043f810_FUN_0043f810.c`](../../../reference/previous-work/decomp/evidence/fresh/0043f810_FUN_0043f810.c),
[`0043f6d0_FUN_0043f6d0.c`](../../../reference/previous-work/decomp/evidence/fresh/0043f6d0_FUN_0043f6d0.c),
[`0049db90_thunk_FUN_0049d790.c`](../../../reference/previous-work/decomp/evidence/collision-audit/0049db90_thunk_FUN_0049d790.c),
and [`00452570_FUN_00452570.c`](../../../reference/previous-work/decomp/evidence/gameplay/00452570_FUN_00452570.c).
The local Player movement dispatch and its scheduler limits are recorded in
[`player-update-scheduling-audit.md`](../../../reference/previous-work/decomp/evidence/player-update-scheduling-audit.md).
The collision latch and its physics-integrator clear are in
[`0049e0a0_FUN_0049e0a0.c`](../../../reference/previous-work/decomp/evidence/fresh/0049e0a0_FUN_0049e0a0.c)
and [`0049dbc0_FUN_0049dbc0.c`](../../../reference/previous-work/decomp/evidence/fresh/0049dbc0_FUN_0049dbc0.c),
with wrapper [`0043f680_FUN_0043f680.c`](../../../reference/previous-work/decomp/evidence/collision-audit/0043f680_FUN_0043f680.c);
the explicit Player `escape` dispatch is in
[`00451870_FUN_00451870.c`](../../../reference/previous-work/decomp/evidence/fresh/00451870_FUN_00451870.c),
and its checkpoint write is in
[`004513a0_player_escape_prepass.c`](004513a0_player_escape_prepass.c).
The temporary safe-position write and its timeout caller are in
[`0044ff10_FUN_0044ff10.c`](../../../reference/previous-work/decomp/evidence/fresh/0044ff10_FUN_0044ff10.c)
and [`00451080_FUN_00451080.c`](../../../reference/previous-work/decomp/evidence/startup-factories/00451080_FUN_00451080.c).
The final checkpoint reset is in
[`0044cd10_FUN_0044cd10.c`](../../../reference/previous-work/decomp/evidence/player-tool-pose/0044cd10_FUN_0044cd10.c);
Player initialization and escape-time checkpoint refresh are in
[`0044d550_FUN_0044d550.c`](../../../reference/previous-work/decomp/evidence/startup-factories/0044d550_FUN_0044d550.c)
and [`004513a0_player_escape_prepass.c`](004513a0_player_escape_prepass.c).
The Player `reset` handler is in
[`00451870_player_script_command_handler.c`](00451870_player_script_command_handler.c),
and oxygen cadence/threshold evidence is in
[`player_oxygen_resource_and_script_path_2026-10-14.md`](player_oxygen_resource_and_script_path_2026-10-14.md).
The Axe detach and collision-hit path are in
[`00460b90_FUN_00460b90.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/00460b90_FUN_00460b90.c)
and [`00483a20_FUN_00483a20.c`](../../../reference/previous-work/decomp/evidence/tool-attachment/00483a20_FUN_00483a20.c).
The hit-point and `G_Liftable` gates plus target command dispatch are in
[`0043e2a0_FUN_0043e2a0.c`](../../../reference/previous-work/decomp/evidence/mechanics-decompiled/0043e2a0_FUN_0043e2a0.c)
and [`0043d660_FUN_0043d660.c`](../../../reference/previous-work/decomp/evidence/gameplay/0043d660_FUN_0043d660.c).
The two child constructors and conditional activation are in
[`00460a50_FUN_00460a50.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/00460a50_FUN_00460a50.c),
[`0045f9b0_FUN_0045f9b0.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/0045f9b0_FUN_0045f9b0.c),
and [`0045f9f0_FUN_0045f9f0.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/0045f9f0_FUN_0045f9f0.c).
ActorInfo visibility is set by
[`00497de0_FUN_00497de0.c`](../../../reference/previous-work/decomp/evidence/player-tool-pose/model-instance/00497de0_FUN_00497de0.c);
the render callback gate and its ActorInfo owner are documented in
[`equipment_actor_render_path_2026-10-13.md`](equipment_actor_render_path_2026-10-13.md).
Axe bone sampling and model-instance transform writes are in
[`0045fb90_FUN_0045fb90.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/0045fb90_FUN_0045fb90.c)
and [`0045fec0_FUN_0045fec0.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/0045fec0_FUN_0045fec0.c);
the victim's separate root-parented ActorInfo frame is documented in
[`0045b690_initialize_victim_rescue_runtime_actor_fresh.c`](0045b690_initialize_victim_rescue_runtime_actor_fresh.c).
Boat B's BModel state, animation, and contact paths are in
[`0043e680_bmodel_dispatch_fresh.c`](0043e680_bmodel_dispatch_fresh.c),
[`0043dc60_update_bmodel_entity_fresh.c`](0043dc60_update_bmodel_entity_fresh.c),
and [`0043b620_bmodel_contact_candidate_fresh.c`](0043b620_bmodel_contact_candidate_fresh.c).
The model collision-flag filter and API context are in
[`genesis_collision_path.md`](../runtime/genesis_collision_path.md) and
[`genesis_collision_functions.c`](../genesis-boundary/genesis_collision_functions.c).
Same-family compiler evidence for outside solid leaves is in
[GBSPLib `PORTALS.CPP`](https://github.com/RealityFactory/GBSPLib/blob/83a4d532eb4fef9edcdf8cabe35f35dd043776e5/PORTALS.CPP#L153-L230),
[`GBSPPREP.CPP`](https://github.com/RealityFactory/GBSPLib/blob/83a4d532eb4fef9edcdf8cabe35f35dd043776e5/GBSPPREP.CPP#L1075-L1085),
[`Leaf.cpp`](https://github.com/RealityFactory/GBSPLib/blob/83a4d532eb4fef9edcdf8cabe35f35dd043776e5/Leaf.cpp#L218-L265),
and [`GBSPFILE.H`](https://github.com/RealityFactory/GBSPLib/blob/83a4d532eb4fef9edcdf8cabe35f35dd043776e5/GBSPFILE.H#L75-L118).
Boat geometry and script tables used in the crosswalk are generated local
exports under ignored `.local/exports/boat_a/` and `.local/exports/boat_b/`;
their source BSP/EVS hashes are recorded above, and the geometry observations
are transcribed here because the exports are not included in GitHub.
