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
against the pinned executable. The other bodies described below were freshly
decompiled from the existing project. No runtime capture of the retail game
was available.

## Findings

**A collision-driven Player position snap exists.** PlayerRescue's root vtable
address point is `004E1C48`; slot `+0x38` at `004E1C80` contains pointer
`0043F810` (raw bytes `10 f8 43 00`). Player update `00452570` calls the
collision loop `0043F6D0`. That loop asks the object's vtable slot `+0x0C` for
the current collision hull, calls `00492CE0` for a world collision sweep, and
invokes vtable slot `+0x38` with the resulting hit position and normal, up to
five times.

The Player callback `0043F810` uses Player origin `+0x68` and hull bounds
`+0x78/+0x84`. If its downward sweep hits, it writes the hit point to Player
scratch `+0x4C..+0x54` and calls `00441E90` to copy those three coordinates
into origin `+0x68`. This is a real source-backed snap to collision geometry.
The other callback paths call `0049E0A0`, which projects velocity and motion
vectors against the reported plane normal; it does not write Player origin.
The snap is reached through the downward floor probe. The current trace does
not show it selecting the nearest arbitrary entity or using Axe output.

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
does not copy it into Player origin or use the candidate object as an Axe
parent. On the accepted-break path, it binds the Axe transform component back
to Player frame `+0x34A4`. The independent VictimRescue/FireRescue touch route
uses the contact manager; no path in this Axe trace selects a survivor or
FireRescue as the strike target. This makes a direct “Axe hit point teleports
the Player to the survivor/fire” explanation unsupported by the current
source.

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

**Stale Player coordinates remain unproven.** Integrator `0049DBC0` saves
previous internal motion values and advances the state using frame delta and
delta-squared acceleration. The reviewed collision and Axe hit paths do not
copy those prior fields into Player origin. A broader failure or update-order
path has not been ruled out, but there is no source-backed evidence yet for the
proposed stale-coordinate rollback.

## Boat episode static map pass

The user identified the Boat episode for the next trace, but has not yet given
the nearest landmark, the action immediately before the warp, or its destination.
I inspected the local `boat_a` export (`boat_a.bsp`, SHA-256
`e1685b45ef29aa075c688985617b593079a6aad8a6b493bbae327356bd45ad44`). Its
entity table contains 322 entries, including 47 `RCS_FireRescue`, 9
`RCS_VictimRescue`, 26 `RCS_BModel`, and 16 `RCS_BoundsTrigger` entities. The
player spawn is `(-1253, 16, 121)`. The world collision model spans
`(-1702, -1224, -283)` to `(2019, 401, 8856.59)` and has 104 models; all
entities with an exported `location` or `ZZ_location` point were within those
global bounds. This simple bounds check does not rule out bad local brush
geometry or a runtime collision seam.

The exported `boat_a` scripts contain no literal `warp`, `teleport`, or player
position-set command. They do include ordinary rescue/escape callbacks and a
final-exit transition to `boat_b`. Nothing in this static map pass identifies a
scripted teleport matching the report, and the global bounds check does not
identify the specific collision surface involved.

## Working hypotheses

- **Verified mechanism, uncertain match to the report:** the Player's downward
  collision probe can snap origin to its hit point. A dirty-space repro that
  falls or crosses a floor boundary could enter this path; static code does
  not establish that it explains the MBase, Boat, or Gas R episodes.
- **Possible visual artifact:** the Axe has two child model variants and
  multiple bone-derived model translations. A stale attachment or visibility
  gate could make an Axe head appear elsewhere; no survivor/fire attachment is
  mapped yet.
- **Unsupported direct warp theory:** the Axe's stack-local impact point is
  not assigned to Player origin, and its direct target scan is restricted to
  type-4 candidates. Axe miss, hit, and break-state effects need runtime
  observation before attributing the reported warp to a swing.
- **Open:** the exact actor/model type represented by any apparent duplicate,
  its lifetime and transform owner, any non-world contact or rescue callback
  that changes Player position, and the condition that makes the visual appear
  at a victim's feet or a spreading fire.

## Next evidence that will distinguish the paths

For one reproducible retail episode, record the map and a nearby named landmark,
whether the Player is falling or moving when the snap occurs, whether it happens
without swinging, and the last action before the Axe copy appears. A runtime
trace should sample Player origin `+0x68`, floor-sweep hit point and normal,
Player frame `+0x34A4`, Axe owner `+0x54`, transform-parent pointer `+0x15C/+0x70`,
both Axe model-instance positions, and the strike result/candidate type each
frame across the swing. That can determine whether the snap is the known floor
probe or another setter and whether the apparent copy is a second variant,
bone/model transform, or actor attached to a collision target.
