# How fire works in FDNY: a player guide

This guide explains the ordinary fires you meet in the rescue maps, using the
retail game code and the project’s saved map/script research. It is written for
players, so it avoids most engine terms. Where a behavior is only partly
traced, I say so instead of presenting a guess as a game rule.

## The short version

Most fires are placed by the level designer. They have two separate values:

- **Strength** controls how hot the fire is for damage and suppression.
- **Size** controls how large its visible and gameplay area is.

The level can change a fire’s size or send a particular fire along a set route.
Some named fires can also roll to create a small nearby fire. Those random
children do not normally spread again, because they are created without the
name required by the spread routine.

Spraying lowers strength. A weakened fire can regain strength when spraying
stops, so a dim flame is not always a permanently doused fire. The carried
rescue hose has a traced final-dousing step. The currently mapped handheld
extinguisher message lowers strength but does not request that final step; the
exact relationship between every extinguisher contact and the game’s final
mission state still needs a live-game check.

## What the game calls a fire

An ordinary fire is a world object with a position, a width, a depth, a height,
a strength, and settings that affect how it reacts to spray. Most are saved in
the map before you enter. The project’s map scan found hundreds of these
ordinary fire objects across the rescue maps.

The fire’s **strength and size are independent**. A script can enlarge a fire
without first making it hotter, or shrink it without using the spray system.
The `setsize` command changes the fire’s dimensions over time; it does not by
itself extinguish the fire or reset its strength. Scripts and level events can
also start a moving-fire sequence.

The flames are drawn as several animated layers with a ground glow. A named
fire also chooses one of three 3D fire-loop sounds. These visuals and sounds
are clues, not precise gauges: the visible size changes in steps, and the
flame can remain fairly opaque after its strength has fallen. Random children
are unnamed, so the traced setup gives them no named-fire loop sound.

## How a fire spreads

The code and map scripts show two different kinds of spread. They are easy to
mistake for one general “fire catches nearby things” rule, but they work quite
differently.

### 1. A level sends a fire along a route

A script can tell a named fire to move toward a named point in the level. As it
moves, the code builds and queues additional fire objects along its route. The
route is authored in the map; it is not chosen by the fire at random. The code
checks its progress as the game updates, so the exact drop spacing can vary a
little with update timing. The saved trace follows the queued-spawn request,
but the complete child registration and cleanup lifecycle is still being
mapped.

For the two routes found in the reviewed scripts, the moving fire checks for a
new deposit after advancing more than half its starting horizontal size, and
also deposits when it reaches the end of its route. Both route parents have a
horizontal size of 100 game units, giving a spacing threshold of about 50
units. The script can set how long the new fire’s size keys take, though the
precise visual timing of those trailing values has not been compared against a
live retail run.

Examples from the maps:

- In **Gas Station**, `Fire42` can travel through `PathPoint1` to
  `PathPoint4` after the rupture sequence, provided the gas-on condition is
  still true. The gas shutoff shrinks several other fires, but the reviewed
  script does not explicitly stop `Fire42` or erase a trail it already made.
- In **Military Base**, the startup script sends `mortarfire1` toward
  `mortar1`.

These sequences are level events. They do not mean that all nearby objects
are gradually catching fire.

### 2. A named fire rolls to make a random child

The random-spread code has several gates. The fire must still be active, have a
nonempty map name, have some horizontal size, and be ready for another try. In
addition, this check runs when the **player is within about 750 game units**
of the fire. The distance check uses the largest difference on the X, Y, or Z
axis. In plain terms, it is a box-like range around the player, not a normal
straight-line distance between two fires. A check exactly 750 units away can
update a shared nearby-fire count, but a spread roll needs to be closer than
750.

When the fire gets a spread roll, the chance is roughly its current strength:
a full-strength fire passes the chance roll, and a half-strength fire
passes about half the time. A failed chance roll waits at least half a second
before another roll. The roll is checked during the fire’s contact/update work,
so it is not an independent clock ticking continuously in the background.

After a successful roll, the game chooses a random direction and a point in a
ring around the parent fire. At full strength the point can be about half to
one full smaller fire-width away; at half strength the range is narrower. The
candidate starts above the ground. The game sweeps a small box downward about
200 units and accepts it only if it finds a surface close to the candidate.
The child is then aligned to that surface. If the sweep misses, no child is
made.

The routine checks for a usable surface. It does **not** check whether that
surface is wood, fabric, or another fuel type. No fuel/material test appears
in this particular spread path, so its placement rule is geometric. This does
not rule out other scripted fire interactions elsewhere in the game.

The random child gets its own size animation: it takes about 0.75–3 seconds to
reach its peak, then another 1–6 seconds to collapse toward a thin remnant.
That size track is not proof that the actor is deleted at that moment; cleanup
and exact on-screen disappearance are still open questions.

### Can a child spread again? Is there a child limit?

Fresh random children are created with an empty name. The random-spread code
requires a nonempty name, so these children do not normally make a second
generation of random children. Route-deposited fires are also created without
the authored parent name. An unnamed child can still be an ordinary fire for
other interactions: the name gate is specifically part of the spread and
named-sound setup, not a general immunity flag.

No per-parent child limit or total-fire cap appears in the spread routine we
reviewed. The practical limits are the chance roll, the waiting schedule, the
player-proximity gate, and finding a valid surface. A shared count of nearby
**named** fires also affects the wait after a successful child: with fewer
than 12 such fires, the next wait is shortened in proportion to the count; at
12 or more, the full child growth/collapse interval is used. That count changes
timing; it is not a maximum number of children. The game may have other actor
or mission limits outside this routine, and the complete actor cleanup path
has not been mapped.

## How ordinary fire burns the player

Each active ordinary fire checks the player’s world position. There is a loose
range check out to 500 units along the largest coordinate difference. Passing
that check does **not** mean the player is taking damage: the point must also
fall inside the fire’s smaller damage volume.

That volume is roughly an oval around the fire. Its horizontal reach follows
the fire’s current width and depth, with extra padding for the player. Vertical
distance counts about twice as heavily as the same horizontal distance. This
means the damage area is flatter vertically than it is wide. Fire size, player
position, and fire strength all matter, so the visible flame edge is not an
exact safety boundary. The exact player-padding value is still unresolved in
the current retail trace: an older note recorded 45 units, while the preview
uses a provisional 30. Neither number should be treated as a confirmed retail
distance.

The game also checks visibility between the player and the fire. If that check
reports an obstruction, the ordinary-fire exposure is reduced to one tenth of
its unblocked value. The code therefore treats cover as strong protection,
but not as guaranteed complete protection.

Accepted exposure adds to a burn buildup. At maximum influence, the source
formula adds about 1.2 buildup points per second; passing out occurs when the
stored buildup goes above 1.0. With continuous maximum exposure and no cooling,
that is a little over 0.83 seconds. Real play can differ because the game’s
fire checks and player updates happen on their own schedules, several fires
can contribute, and burn recovery may run between hits.

When the four directional burn-warning flags are clear during the player
update, the burn buildup falls by 0.1 per second and is clamped at zero. That
recovery is much slower than taking sustained strong exposure. The HUD flags
show a direction from which fire exposure arrived; the traced handler changes
these indicators, not the player’s camera or movement heading. A separate
cooldown controls the `RC_Pain` sound and the `Serious Burns` statistic, so
that statistic is not a live readout of the burn buildup.

Once the player enters the passed-out state, this ordinary-fire damage handler
stops accepting more fire exposure. Running out of air is another route to
passing out, handled by a separate system.

## How spraying weakens and douses a fire

Each fire has a setting for how resistant it is to spray. The map can provide
that setting; when it is zero, the initializer derives a value between about
1 and 4 seconds from the fire’s saved radius. This is why there is no one
universal amount of time that works for every fire.

The shared spray routine lowers fire strength according to the elapsed time,
the fire’s spray setting, and the contact strength delivered by the tool. In
plain math, its response is `1 - (1 - contact strength)²`, and the strength
loss is that response multiplied by `elapsed time ÷ spray setting`. So it is
not a simple “one hit equals one fixed amount” rule. The contact point
matters: spray must intersect the fire’s influence area. The mapped
extinguisher path samples several points down its spray direction, while the
carried hose checks its current contact point. Exact extinguisher aim and
frame timing have not been confirmed in a live retail capture.

One odd code detail deserves a caveat: the response curve is strongest when
the value it receives is near 1, and it falls again for values above 1. An
older extinguisher-caller trace says the tool doubles its sampled value before
sending it, but that producer has not been fully rechecked in the current
audit. Treat that as a provisional code finding, not as advice to aim farther
away or avoid close contact.

While the fire is being sprayed, a “recently sprayed” flag changes its update
path. When spraying stops, a fire that has not entered its terminal doused
state can rebuild strength toward full. Most stationary fires also skip some
update work, so the exact recovery rate depends on the game’s callback timing.

There are two distinct outcomes in the traced code:

1. **Weakened:** strength goes down, so the fire’s damage source and appearance
   diminish. The actor remains active and can recover after spraying stops.
2. **Terminally doused:** a spray contact requests finalization and leaves the
   stored strength strictly below 10% of its normal value. The game switches
   the fire to a terminal mode, increments `Fires Doused`, stops its named
   loop sound, and disables later fire-visual updates.

The carried rescue hose’s traced contact path requests this final step. The
shared message receiver used by the traced extinguisher contact path passes a
“do not finalize” flag, so that route only proves weakening. Some extinguisher
producer details and the final mission-level accounting have not all been
rechecked against live play. For a player, the practical lesson is that a
flame becoming small or dim is not by itself proof that the objective counted
it as doused.

The terminal code stops future visual updates and polygon submissions; it
does not show an explicit fade animation or prove the exact frame when an
already-drawn flame disappears. Treat that visual timing as unresolved.

## Fire and the rest of the environment

- **Ground and surfaces:** placed fires can be aligned to a nearby surface.
  Random children must pass a downward surface sweep. The source path does not
  identify fuel materials before placing a child.
- **Walls and cover:** a failed world-visibility check greatly reduces player
  exposure, but the traced rule leaves a small residual amount.
- **Level events:** scripts can change fire size, start or stop a moving route,
  trigger an explosion sequence, or alter other fires. Those scripted effects
  are separate from natural strength recovery and random-child spread.
- **Smoke and air:** smoke can be a visual effect attached to a fire. The
  reviewed burn path does not use smoke density to compute fire damage. The
  air-supply system has a separate update, so smoke appearance alone does not
  tell us how quickly air is being lost.
- **Victims:** the checked ordinary-fire burn message is routed to the player.
  An older trace also found a no-op victim message handler. That is not a full
  proof that every victim is immune to every fire-related mission event, so
  victim behavior remains partly unresolved.
- **Flame jets and explosions:** some hazards that look like fire use a
  separate line-shaped damage system, with their own active periods and
  strength. They should not be assumed to follow ordinary-fire size,
  suppression, or damage rules; the detailed trace for those actors is older
  and needs a fresh retail review.
- **Truck-hose missions:** the window fires in the truck-hose stages are
  managed as groups of targets with their own ramp-up, selection, and dousing
  sequence. The ordinary FireRescue rules in this guide do not fully describe
  those stages.

## What is confirmed and what remains open

The strongest current evidence covers random spread’s name, size, chance,
proximity, placement, and child-shape gates; the player exposure and
obstruction paths; the shared spray calculation; and the carried hose’s
terminal-dousing flag. The retail executable used for those fresh traces is
hash-pinned in the linked evidence notes.

The main open items are the exact live-game update cadence, the player hitbox
padding used by retail, the full producer path for every extinguisher sample,
when temporary child actors are eventually removed, the precise visual
disappearance after terminal dousing, and untraced fire interactions with
materials or victims. The preview/port has not had a live side-by-side retail
test for these behaviors, so its successful builds and controlled checks are
not proof of full gameplay parity.

## Research notes behind this guide

- [Current fire exposure, random spread, and suppression trace](../tracker/evidence/fire-spread/README.md)
- [Why random children do not recursively spread](../tracker/evidence/fire-spread/fire_name_gate.md)
- [How the nearby-fire count changes retry timing](../tracker/evidence/fire-spread/proximity_count_port_2026-10-15.md)
- [When spread children join the world update](../tracker/evidence/fire-spread/fire_contact_pass_order_port_2026-10-15.md)
- [What terminal dousing does to fire visuals](../tracker/evidence/fire-spread/visual-terminal-state.md)
- [Fire direction indicators on the HUD](../tracker/evidence/scenario-loading/fire_direction_hud_trace_2026-10-14.md)
- [Fire `moveto` and authored route notes](../tracker/evidence/rescue-moveto/README.md)
- [Scripted fire size changes](../tracker/evidence/scenario-loading/firerescue_setsize_script_port.md)
- [Older plain-language fire summary](../reference/legacy-evidence/fire-spread/SUMMARY_legacy.md)
- [Older detailed fire mechanics audit and its evidence limits](../reference/legacy-evidence/fire-spread/MECHANICS_legacy.md)
