# Gameplay Distance Checks in FDNY Firefighters: American Hero

## Short answer

The 750-unit value is **not a general game radius**. It belongs to one
FireRescue proximity check that can call the random child-fire routine. Other
systems use different numbers, different shapes, and sometimes the opposite
kind of rule. RescueBot, for example, looks for victims at least 600 units
away; survivor roaming uses a map-authored roam distance; goggles scan within
2,000 units; and victim pickup checks box overlap rather than a circle.

All values below are **game-world units**. The evidence does not establish a
reliable conversion to feet or meters.

## Why the shape of the check matters

A number such as “750” is incomplete by itself. The game can compare positions
in several ways:

- **Straight-line distance (a 3D sphere):** `sqrt(dx² + dy² + dz²)`. Moving
  diagonally uses up range faster than moving along one axis.
- **Largest-axis distance (a cube):** `max(|dx|, |dy|, |dz|)`. Each coordinate
  can be close to the limit at the same time, so diagonal points can be much
  farther away in a straight line.
- **Expanded box overlap:** the game checks whether two boxes intersect after
  growing one by a margin. This has no single circular radius.
- **Object bounds or a fire-shaped volume:** the limit follows the target's
  authored size or current dimensions rather than one fixed number.

## Fire checks

### Random spread: 750, measured as the largest coordinate difference

The FireRescue callback compares the player's reported world position with
the fire's origin. It takes the largest absolute difference on X, Y, or Z,
then truncates that nonnegative value to an integer:

```text
D = max(|player.x - fire.x|,
        |player.y - fire.y|,
        |player.z - fire.z|)
spread attempt if integer(D) < 750
```

So every axis must be under 750. This makes a cube with 750-unit half-widths,
not a sphere with a 750-unit radius. An offset of `(700, 700, 700)` passes the
spread range check even though its straight-line distance is about 1,212. An
offset of `(750, 0, 0)` does not call the spread routine. The separate shared
nearby-fire count includes the exact 750 boundary (`<= 750`); that bookkeeping
threshold is not the same as permission to attempt a spread.

Passing this proximity check only lets the callback try. The parent must also
be active, named, sized, and ready for another attempt; its strength chance
must pass; and the downward surface sweep must find a collision. The check
runs as part of a contact/update callback, not as an independent background
clock.

This 750 value measures player-to-parent proximity. It does **not** place the
child 750 units away. Child placement has a separate size-based formula:

```text
R = min(parent width, parent depth) / 2
r = R * (1 + U * fire strength)
child candidate = (fire.x + r*cos(angle), fire.y + 20,
                   fire.z + r*sin(angle))
```

Here `U` is a fresh random value in `[0,1)` and the angle is random around the
horizontal circle. The chosen radius is based on the parent's current size
and strength, then collision decides whether the point is usable. See the
[plain-language fire guide](FIRE_MECHANICS_PLAYER_GUIDE.md) for the floor
sweep and worked example.

**Evidence:** [current fire trace](../tracker/evidence/fire-spread/README.md),
[fresh callback pseudocode](../tracker/evidence/fire-spread/004483A0_fresh.c),
[proximity-count and strict spread gate](../tracker/evidence/fire-spread/00448320_fresh.c),
[fresh child-spread pseudocode](../tracker/evidence/fire-spread/00447A90_fresh.c).

### Fire exposure: 500 broad check, then a size-based oval

The same FireRescue callback has a separate, nearer broad check for player
exposure. It uses the stored max-axis distance and enters the detailed fire
volume calculation at 500 or less. That is only a cheap outer filter: it does
not mean the fire can damage the player from 500 units away.

The detailed shape depends on the fire's current dimensions and a player
padding value. Its traced distance formula is:

```text
R = (fire width + fire depth) / 2 + player padding
q = sqrt(dx² + (2*dy)² + dz²)
inside the damage volume when q <= R
```

Vertical separation counts twice as much as the same horizontal separation,
so the volume is flattened vertically. Influence fades toward its edge, is
weighted by fire strength, and is reduced to one tenth when the visibility
query reports an obstruction. The precise player-padding value is unresolved;
older notes and the viewer differ, so neither should be treated as a confirmed
retail distance.

**Evidence:** [callback and 500-unit gate](../tracker/evidence/fire-spread/004483A0_fresh.c),
[fire-volume falloff formula](../tracker/evidence/fire-spread/0044A7A0_fresh.c),
[fire evidence overview](../tracker/evidence/fire-spread/README.md).

## Survivor and RescueBot checks

### RescueBot: at least 600 straight-line units away

RescueBot's selector accepts an eligible victim only when the Player-to-victim
3D straight-line distance is **600 units or more**. It also requires a blocked
world segment between them and a victim in one of the selector's accepted
states. A `norescuebot` flag excludes a victim. The selector takes the first
eligible victim in its manager's list.

This is a **minimum separation rule** for choosing a RescueBot target. It is
not a 600-unit radius in which the bot can rescue any victim, and it is almost
the opposite of FireRescue's “player is nearby enough to consider spreading”
rule. The saved selector pseudocode calculates `sqrt(dx² + dy² + dz²)` before
the comparison. The focused update note independently records the 600 value
and selection gates; callback timing and live behavior have not been verified.

**Evidence:** [RescueBot update and selector report](../tracker/evidence/scenario-loading/rescuebot_update_path_2026-10-15.md),
[victim roam and RescueBot report](../tracker/evidence/scenario-loading/victim_roam_behavior.md),
[saved selector pseudocode](../reference/previous-work/decomp/evidence/item-pose/live-extinguisher/message-routing/00455230_FUN_00455230.c).

### Survivor roaming: an authored 3D boundary for victims told to roam

A script command such as `roam 1000` stores a roam distance for that victim.
When the game tests a new destination, it measures the 3D straight-line
distance from the victim's original authored location and rejects a candidate
outside that radius:

```text
(candidate.x - center.x)² + (candidate.y - center.y)²
+ (candidate.z - center.z)² <= roam radius²
```

That is a sphere around the roam center, not a 750-unit global neighborhood.
The map data supplies different roam values for different victims. Examples
in the preserved inventory include 80, 500, 1,000, 2,400, and 5,000 units.
Being inside the roam sphere still does not guarantee a valid destination: the
game also sweeps the victim's body, avoids recorded fire obstacles, and checks
nearby floor heights.

Roam activation is a separate step. In the traced entry path, an authored
`roam <number>` script command calls `StartVictimRoam`, which sets numeric
state 7 and immediately tests four nearby start offsets. If all four fail, it
logs that the victim is stuck and returns it to state 4. No player-distance
test appears in that command handler or startup routine, and the audit has not
found a nearby-player trigger that starts roaming. Some map victims have a
`roam` command; others do not. The preserved Training victim scripts use
pickup events and contain no `roam` command. This is the confirmed path, not
proof that every possible indirect state change in the full game has been
traced.

The roaming AI has other distance tests with different jobs:

- At 432 units, it checks the Player against the victim's **current roam
  target**, not against the victim. Passing that check lets the AI consider
  player-relative choices, subject to facing and visibility tests.
- It picks a new roam target when it gets within about 72 units of the current
  target.
- Candidate generation also has short movement lengths and other branches;
those values are step choices, not the victim's full roam radius.

#### What the 432-unit check means for the victim's behavior

Think of roaming as a series of short waypoints, not as the victim constantly
chasing or fleeing the Player. A victim only enters this roaming state when a
script tells it to with a command such as `roam 1000`. It moves toward its
current waypoint; when it gets close enough (about 72 units), it chooses the
next one.

When choosing that next waypoint, the AI measures the straight-line 3D distance
from the Player to the **current waypoint**. In formula form, the 432-unit gate
is:

```text
distance_squared = (player.x - target.x)²
                 + (player.y - target.y)²
                 + (player.z - target.z)²
compare distance_squared with 432² (= 186,624)
```

So this is a true spherical distance check: it uses all three coordinates. It
does not measure 432 units from the victim's body, and it is not three separate
“within 432 on X, Y, and Z” checks. Being inside the sphere merely lets the AI
consider a Player-relative direction. Two more tests must pass:

1. The victim must be facing the Player within a 45-degree angle, measured from
   one of the victim's orientation vectors. The code proves that it checks an
   orientation cone; which model axis is visibly “forward” still needs a
   runtime check.
2. A world collision query must find a clear segment between the current roam
   waypoint and the Player. A wall or other blocking geometry can make this
   test fail even when the Player is close.

If the Player is outside 432 units, the victim is not in the right facing cone,
or the segment is blocked, the AI falls back to a direction based on its last
roam step. It does not turn toward the Player merely because the Player is
nearby. When the gates do pass, the first candidate is a 12-unit step aimed
from the current waypoint toward the Player. Later branch checks can keep that
step aimed toward the Player or reverse it so it points away. In the traced
branch, carrying-state and the Player's own facing angle affect that choice; an
additional internal float comparison can also reverse it, and its exact
meaning has not been resolved. The evidence therefore supports both toward and
away steps, but not a simple rule that victims always chase or always flee.

The chosen step is still only a candidate. The game checks that it stays
inside the victim's authored roam boundary, does not collide with the world,
clears nearby fire obstacles, and has suitable floor under the destination.
Rejected candidates are adjusted and retried; if normal choices fail, the AI
can switch to its short random-step mode. The reverse-engineered trace is
static analysis of the pinned executable; exact callback timing and the
victim's visible forward axis have not been confirmed in a live game capture.

**Evidence:** [victim roam behavior](../tracker/evidence/scenario-loading/victim_roam_behavior.md),
[fresh script dispatcher](../tracker/evidence/scenario-loading/0045cdb0_dispatch_victim_rescue_script_fresh.c),
[fresh roam startup](../tracker/evidence/scenario-loading/0045c7f0_start_victim_roam_fresh.c),
[fresh candidate validator](../tracker/evidence/scenario-loading/0045ab40_validate_victim_roam_candidate_fresh.c),
[fresh target updater](../tracker/evidence/scenario-loading/0045ae50_update_victim_roam_target_fresh.c),
[authored victim script inventory](../tracker/evidence/scenario-loading/rescue_actor_inventory.md),
[Training victim roam boundary](../reference/previous-work/decomp/evidence/victim-ai-audit/training-pickup-flow.md).

### Roaming near fires: a separate 3D clearance sphere

When a victim's roam setup builds its obstacle list, it reads a FireRescue
radius and clamps it to 24–1,200 units if it falls outside that range. The
builder collects a fire when its center is within `fire radius + victim roam
radius` of the victim's roam center. For candidate rejection, it stores a
clearance sphere of `fire radius + 48` units around that fire and rejects a
candidate strictly inside the sphere.

The code uses a runtime field at FireRescue `+0x88`. The precise connection
from the authored map key `H_Radius` to that runtime field has not been proven,
so the viewer's use of `H_Radius` is a documented proxy rather than a confirmed
retail field binding.

**Evidence:** [victim roam behavior and mapping caveat](../tracker/evidence/scenario-loading/victim_roam_behavior.md),
[FireRescue radius getter](../tracker/evidence/scenario-loading/0044a310_firerescue_obstacle_radius_getter.md),
[fresh obstacle builder](../tracker/evidence/scenario-loading/0045c4d0_build_victim_roam_obstacles_fresh.c),
[fresh candidate validator](../tracker/evidence/scenario-loading/0045ab40_validate_victim_roam_candidate_fresh.c).

### Victim pickup: expanded boxes, not a 60-unit sphere

Pickup checks whether the Player's candidate bounds overlap a box centered on
the victim. The box extends by the configured proximity on each axis; when
that setting is invalid or below 1, the code uses 60 units. Comparisons are
inclusive on X, Y, and Z.

So “60” is a per-axis box expansion used with the Player's bounds. It is not a
3D radius and does not mean the Player's center must be within 60 units of the
victim. The outer Player message handler and carry-state checks must also
accept the pickup.

**Evidence:** [shared victim pickup path](../tracker/evidence/scenario-loading/victim_pickup_message_path.md),
[fresh pickup overlap code](../tracker/evidence/scenario-loading/0045a070_check_victim_pickup_overlap_fresh.c).

## Player detection and interaction examples

### Goggles: a 2,000-unit 3D scan sphere

The goggles scan accepts actor kinds 2 and 6 when their squared 3D distance
from the Player is at most 4,000,000, which is a 2,000-unit sphere. Some
kind-2 states are excluded. This is a target scan list; its distance should
not be confused with the fire-spread gate or with a physical fire-damage
radius.

**Evidence:** [goggles scan trace](../tracker/evidence/scenario-loading/goggles_scan_viewer_runtime.md).

### Generic use selection: a 200-unit query limit plus target geometry

One Player use-selection pass rejects a geometric query hit beyond a squared
200-unit distance limit. A candidate also has to provide a usable query shape
and be in the interaction list. A box-shaped door, a prop sphere, or another
query object can therefore behave differently even under this outer limit.
It is not proof that every object can be used from anywhere inside a 200-unit
sphere.

**Evidence:** [Player selection and geometric query trace](../reference/previous-work/decomp/evidence/hose-spray/README.md),
[shared BModel/BoundsTrigger contact route](../tracker/evidence/scenario-loading/rescue_contact_manager_route.md).

### PropModel touch example: an authored 70-unit sphere

A Training extinguisher refill prop (`ext1`) has an authored touch radius of
70 units. The PropModel contact check compares the squared 3D distance from
the Player's returned world point to the prop's center against that authored
radius squared, then applies a separate world-visibility gate. Other props
can author a different radius; 70 is an example, not a game-wide setting.

**Evidence:** [PropModel contact trace](../tracker/evidence/scenario-loading/retail_propmodel_contact_trace.md).

### BModel reach and BoundsTriggers: boxes and authored bounds

Door and liftable BModels usually use an AABB/bounds overlap test with a reach
margin. Across eight inspected maps, 71 of 76 BModels that met the code's
eligibility conditions use an effective 48-unit bounds-test reach. Five keep
their authored 5-, 20-, or 24-unit value because their linked motion has no
nonzero duration. A 48-unit reach margin is therefore not the same as a
48-unit sphere.

BoundsTriggers test their authored model bounds against candidate bounds; the
code evidence does not define one generic trigger radius. The exact overlap
extent depends on the trigger's level geometry.

**Evidence:** [cross-map BModel reach audit](../tracker/evidence/scenario-loading/bmodel_reach_cross_map.md),
[contact manager route](../tracker/evidence/scenario-loading/rescue_contact_manager_route.md),
[BoundsTrigger trace](../tracker/evidence/scenario-loading/00440ce0_bounds_trigger_contact_check_fresh.c).

## What to take away

- **750** is a FireRescue player-proximity gate measured by the largest axis
  difference. It is not the child's placement distance and not a shared game
  radius.
- **500** is a nearer broad gate for ordinary fire exposure. A separate,
  fire-size-based flattened volume computes actual influence.
- **600** is RescueBot's minimum Player-to-victim separation, with a blocked
  world segment and other eligibility gates.
- **`roam N`** is an authored 3D sphere around a victim's roam center; values
  differ by victim and map.
- **2,000** is a goggles target-scan sphere.
- **60, 70, 48, and 200** belong to different box, prop-contact, BModel-bound,
  or use-query checks. They do not share one geometry or purpose.

These are representative checks from the inspected evidence, not every
comparison or collision radius in the executable. The detailed traces are
mostly static analyses of the pinned retail build
`FDNYFirefighter.exe` (SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). They
do not by themselves verify callback timing, live hit behavior, or parity of
the preview. Each unresolved field mapping or runtime caveat is called out in
the linked evidence notes.
