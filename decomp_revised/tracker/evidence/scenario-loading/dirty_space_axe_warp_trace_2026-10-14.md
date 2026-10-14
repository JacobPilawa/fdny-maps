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
Axe attachment follow-up cross-checked saved address-matched exports. These
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

Both Player movement and Axe strikes call `00492CE0`, which forwards a swept
hull to Genesis `geWorld_Collision` with mask `0x43` and collision type `2`.
The Axe uses a separate post-hit candidate scan. `00460B90` first advances its
animation and requires the strict `+0x340 < 0.275 < +0x33C` crossing. It then
sweeps a `+/-6` box using Player frame data and receives the world hit point in
a stack local. On a miss, it returns before strike effects or candidate
processing. On a hit, it checks the impact point (offset by six units along
the Player frame axis) against type-4 objects' world AABBs expanded by eight
units. Only an eligible object proceeds to `0043E2A0`, which requires hit
points at UserData `+0x34`, a zero gate at `+0x38`, and an allowed strike-time
value at object `+0x1EC`; it sends `chop`, decrements hit points, and on zero
sends `break` and invokes `0043D660`.

The Axe hit point remains in the strike stack frame. The reviewed strike code
does not copy it into Player origin or use a survivor or fire as a direct Axe
parent. However, the parent behavior after a world hit needs a correction:
before scanning candidate objects, `00460B90` calls `00483A20` on the Axe
transform component at `+0x15C` with a null parent. `00483A20` writes null to
the component's parent field at `+0x70`; because the new parent is null, it
skips matrix and translation updates. The last cached transform is therefore
retained while detached.

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

**The Axe does contain multiple model instances and sampled transforms.** The
retail initializer creates two Axe child variants: model 6 `axel.act` and
model 1 `axe.act`. Axe setup stores its owning Player at `+0x54` and binds its
transform component at `+0x15C` to Player frame `+0x34A4`. Each animation update
`0045FB90` samples three ACT bone transforms into Axe fields `+0x35C`,
`+0x344`, and `+0x350`; `0045FEC0` copies the latter two translations into two
Genesis model-instance transforms. These are plausible places to look for a
stale or duplicated *visual* if both variants or a sampled frame are rendered
incorrectly. The current static evidence does not associate either model
instance with a victim or fire transform.

**A separate saved-position restore can also move the canonical transform.**
The constructor zeros Player vector `+0x3400..+0x3408`. In the normal branch of
`00452570`, Player fields `+0x4C..+0x54` are copied into this saved vector only
after the four directional burn flags clear, accumulated burn falls below
`0.1`, `+0x12C` is set, and `+0x3450 > 0.5`. `0044FF10` copies the saved vector
back into `+0x4C..+0x54`, which is the translation inside transform `+0x24`.
State-6 updater `00451080` calls this restore with parameter 0 when its timer
expires; the Player ScriptActor `reset` command also reaches `0044FF10`
directly with parameter 1. Fire exposure and oxygen depletion can enter state
6, so a delayed return to a saved point is a verified mechanism. The normal
updater snapshots before its later movement step; the oxygen gate stops
refreshing the snapshot once oxygen is `<=0.5`, while drain continues until
strictly below zero. Starting from full oxygen and using the mapped `0.05` per
3-second drain, the saved point can be roughly 30–36 seconds old when passout
starts; this is derived from the resource thresholds and cadence, while exact
frame timing and any match to the incident remain unobserved. State 6 runs its
one-second transition before the timeout branch calls the restore. The separate
integrator audit at `0049DBC0` found no copy of its prior motion values into the
origin cache.

## Boat episode static map pass

The user identified the Boat episode but does not yet know the landmark, action,
or destination. I inspected both local Boat exports. Boat A (`boat_a.bsp`) has
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
during the reported incident. The fire and oxygen routes both enter the Player
passout/state-6 path, whose timer invokes the saved-transform restore.
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

Boat B has the strongest concrete geometry seam if the report occurs near its
opened hook/top deck. BModel e121 `button` references animated `%Model%` hook
#13 at `(-586.5,905,3922.5)`, with AABB `[-626.5,667.573,3881.5]..[-546.5,
1474,3961.5]`. This extends 416 units above world-model maximum Y=1058, and its
31 BSP leaves carry Player mask `0x43` (contents 1/33), unlike Boat A's outlying
propeller-room trigger brushes. Ladder trigger e120 references model #14
`ladder8`, whose bounds overlap the hook; its touch action is Player `climb 1`.
This makes the hook a genuine Player-collidable out-of-world0 geometry lead,
not proof of a defect or of a match to the incident. Boat B rescue3 trigger e61
has a large mismatch between its `ZZ_location` and contact brush, but the
source BoundsTrigger check uses the linked brush bounds; that anomaly alone
does not move the Player.

## Working hypotheses

- **Verified mechanism, uncertain match to the report:** any successful Axe
  world hit detaches its transform before the candidate scan. A no-candidate
  hit can leave the cached transform detached until a conditional recovery. An
  apparent extra Axe is plausible; its visible model, target location, and
  lifetime are not captured.
- **Verified Player restore, uncertain match:** state-6 timeout or Player
  `reset` writes saved coordinates directly into transform `+0x24`'s
  translation. Boat oxygen triggers can start depletion/passout, but the
  incident has not been matched to those zones or timers.
- **Verified floor-hit snap, uncertain match:** the `0043F810` downward-sweep
  hit branch writes its world collision hit directly into transform translation
  `+0x4C..+0x54`, then refreshes derived cache `+0x68..+0x70`. Whether the Boat
  glitch enters this nested collision branch is unknown.
- **Unsupported direct Axe-to-Player warp:** the Axe's stack-local impact
  point is not assigned to the Player transform, and a type-4 BModel scan does
  not itself set Player coordinates. The detached transform may explain a
  visual seam, but no source path yet joins it to saved-position restore.
- **Open:** the exact actor/model type represented by any apparent duplicate,
  its lifetime and transform owner, any non-world contact or rescue callback
  that changes Player position, and the condition that makes the visual appear
  at a victim's feet or a spreading fire.

## Next evidence that will distinguish the paths

The user does not yet know a precise Boat repro. Static Boat A/B crosswalk work
can continue without it; an in-game capture will still be needed to match a
specific route. A useful runtime trace should sample canonical transform
translation `+0x4C..+0x54`, derived cache `+0x68`, saved vector
`+0x3400..+0x3408`, Player state and state-6 timer, oxygen value/deadlines,
floor-sweep hit point/normal, and (across the swing) Axe child state, parent pointer
`+0x15C/+0x70`, cached transform, both model-instance positions, and strike
candidate type/result. This separates the direct floor-hit snap, delayed
saved-coordinate restore, and detached Axe visual without assuming they share a
trigger.

## Evidence files

The Player transform and collision findings are cross-checkable in
[`0043f810_FUN_0043f810.c`](../../../reference/previous-work/decomp/evidence/fresh/0043f810_FUN_0043f810.c),
[`0043f6d0_FUN_0043f6d0.c`](../../../reference/previous-work/decomp/evidence/fresh/0043f6d0_FUN_0043f6d0.c),
[`0049db90_thunk_FUN_0049d790.c`](../../../reference/previous-work/decomp/evidence/collision-audit/0049db90_thunk_FUN_0049d790.c),
and [`00452570_FUN_00452570.c`](../../../reference/previous-work/decomp/evidence/gameplay/00452570_FUN_00452570.c).
The restore and its timeout caller are in
[`0044ff10_FUN_0044ff10.c`](../../../reference/previous-work/decomp/evidence/fresh/0044ff10_FUN_0044ff10.c)
and [`00451080_FUN_00451080.c`](../../../reference/previous-work/decomp/evidence/startup-factories/00451080_FUN_00451080.c);
the Player `reset` handler is in
[`00451870_player_script_command_handler.c`](00451870_player_script_command_handler.c),
and oxygen cadence/threshold evidence is in
[`player_oxygen_resource_and_script_path_2026-10-14.md`](player_oxygen_resource_and_script_path_2026-10-14.md).
The Axe detach and collision-hit path are in
[`00460b90_FUN_00460b90.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/00460b90_FUN_00460b90.c)
and [`00483a20_FUN_00483a20.c`](../../../reference/previous-work/decomp/evidence/tool-attachment/00483a20_FUN_00483a20.c).
Boat geometry and script tables used in the crosswalk are generated local
exports under ignored `.local/exports/boat_a/` and `.local/exports/boat_b/`;
their source BSP/EVS hashes are recorded above, and the geometry observations
are transcribed here because the exports are not included in GitHub.
