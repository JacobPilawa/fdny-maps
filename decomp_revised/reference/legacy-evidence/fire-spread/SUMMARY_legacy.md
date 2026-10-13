# How fire works in FDNY

Updated October 10, 2026. Based on the game files and code. Exact formulas and code references are in `MECHANICS.md`.

## This is how a fire works

Most fires are put in specific places by the level designer. Each fire has a size and a strength. Spraying it reduces its strength, which makes the flames fade and reduces how much it can burn you. **If it has not been fully extinguished, its strength can recover after you stop spraying.** That is the main behavior in the code that explains fires coming back.

- **Where it starts:** most fires already have a starting location in the map. Events such as an explosion can make particular fires larger or start a spreading sequence.
- **How it grows:** a fire can regain strength between spray hits. The level can also tell it to become physically larger or smaller. Strength and size are separate things.
- **How it spreads:** there are two systems. Some fires follow a route written into the level and leave fires behind. Others create small flames at random nearby positions. **Those new flames have no name, and the random-spreading routine requires a named fire, so newly created unnamed flames do not repeat that routine to spread endlessly outward.**
- **How you put it out:** the extinguisher and carried hose both reduce fire strength. A sufficiently successful carried-hose hit can also mark a fire as fully extinguished. The extinguisher contact routine reduces strength without making that final change, leaving room for recovery. Fire settings and where the spray lands affect how quickly it weakens.
- **How it hurts you:** ordinary fires burn you when you are close enough. Stronger fire and closer contact increase the burn amount. An obstruction between you and the fire reduces ordinary-fire damage to one tenth. Burns build up until you pass out. That buildup decreases when the game's burn indicators are clear. Running out of air is a separate cause of passing out.
- **Other dangerous flames:** some flame jets and explosions use a different damage system. The level can make these particularly strong, so they can burn you much faster.
- **Truck-hose stages are different:** their window fires are managed as groups of targets with a separate selection and dousing sequence.

## Where the fires come from

Across the 17 maps in this installation, we found **430 placed ordinary fires**. Spreading can create additional fires during play.

The map files specify a fire's position, horizontal size, height, name and resistance to spraying. They can also specify whether it produces smoke and whether the game should align it with the ground. Ground alignment adjusts the fire’s position to the surface.

| Stage | Placed ordinary fires |
|---|---:|
| Bank | 72 |
| Bar | 65 |
| Boat A | 47 |
| Boat B | 42 |
| Gas Station | 39 |
| Military Base | 11 |
| Oil Rig A | 15 |
| Oil Rig B | 14 |
| Oil Rig C | 24 |
| Oil Rig D | 16 |
| Subway A | 55 |
| Subway B | 23 |
| Training | 7 |
| **Total** | **430** |

City, Construction, Dock and the Oil Rig truck-hose stage have no placed ordinary fires of this kind. They create window fires through their own system, explained below.

All 430 placed ordinary fires use the same saved fire type and are saved as enabled. None has a saved sequence of size changes. Size changes are added later by level commands or when the game creates new fires.

## Growing, weakening and coming back

A fire's strength controls how strongly it burns. Spraying lowers that strength. Unless the fire has been marked fully extinguished, the game can raise its strength again after spraying stops, up to its normal full strength.

Its physical size is separate. The level can tell a fire to grow or shrink over a chosen amount of time. Spreading fires and the small random flames also get sequences of size changes. A fire can therefore change size because of a level event, not because it has gradually heated nearby material.

The visible flames do not show every tiny change in strength: part of their appearance changes in steps. Their transparency also changes as the fire weakens. Once both strength and transparency reach zero, the fire causes no burn exposure.

Stationary fires skip part of their update on some calls; moving fires use a different update pattern. Spraying also changes which updates run, affecting the pace of recovery.

## Putting fires out

Each fire has a setting that affects how quickly spraying weakens it. **A saved value of zero does not mean instant extinguishing.** The game replaces it with an automatically calculated setting between 1 and 4 seconds, based on the fire's saved horizontal size.

The spray’s position, strength and number of contacts affect how quickly it weakens the fire, alongside recovery between hits.

| Stage | Fires using the automatic setting | Setting range, in seconds |
|---|---:|---:|
| Bank | 72 | about 1.30–4 |
| Bar | 0 | 1.4–5.5, explicitly saved |
| Boat A | 47 | about 1.86–4 |
| Boat B | 42 | about 1.86–4 |
| Gas Station | 39 | about 1.24–4 |
| Military Base | 11 | about 1.86–4 |
| Oil Rig A | 15 | about 1.86–4 |
| Oil Rig B | 14 | about 1.86–4 |
| Oil Rig C | 24 | about 1.86–4 |
| Oil Rig D | 16 | 3 |
| Subway A | 55 | about 1.86–4 |
| Subway B | 23 | about 2.77–4 |
| Training | 3 | about 2.43–4 |

Training's three smaller fires use an automatic setting of about 2.43; its four larger fires explicitly use 4. In total, 361 of the 430 fires use the automatic calculation.

There is also a distinction between **weakening a fire** and **marking it fully extinguished**:

- The extinguisher contact routine weakens it without marking it fully extinguished.
- The carried rescue hose can perform the final step once strength falls below one tenth of full strength. That stops the fire's normal update, stops its looping sound and increases the “Fires Doused” statistic.

The extinguisher's calculation has another surprising detail: stronger sampled contact does not always produce faster weakening. The game doubles the contact value before applying a curved response, and that curve falls again at its high end.

## How spreading works

The game creates additional fire through fixed routes and random nearby placement.

### 1. Fires following a fixed route

The level can give a particular fire a list of destinations. The fire moves along straight segments between them and leaves additional fire objects behind. Their direction follows the segment. The route itself is not randomly chosen.

The movement code deposits another fire when it has advanced more than half the parent's base horizontal size since the previous deposit, or when it reaches/passes its destination. Both identified route fires have a base horizontal size of 100, giving a 50-unit spacing threshold. The game checks this spacing during updates.

We searched all 21 external script files and every embedded entity-script field in the 17 maps. **Two ordinary-fire movement sequences were found:**

**Gas Station: Fire42 travels through four points.**

| Order | Destination | Saved position (X, Y, Z) |
|---|---|---|
| Start | Fire42 | (295, -125, 1341) |
| 1 | PathPoint1 | (729, -126, 1279) |
| 2 | PathPoint2 | (815, -125, 1439) |
| 3 | PathPoint3 | (1190, -125, 1398) |
| 4 | PathPoint4 | (1171, -125, 1270) |

Each leg is assigned one second of travel.

The spreading sequence is part of the gas-main rupture event. It runs when the player touches the trigger named `unfog2`, provided the gas is still on, and the event is allowed to happen only once. The same event enlarges Fire39, Fire40 and Fire41, opens an exploding-car model, plays an explosion and later shows the rupture hint.

The spread command gives the deposited fires very long size-change timings: 99,999 seconds for each of its two timing arguments—roughly 27.8 hours each. These timings make the trail’s scheduled size changes extremely slow.

**Military Base: mortarfire1 travels to one point.**

| Order | Destination | Saved position (X, Y, Z) |
|---|---|---|
| Start | mortarfire1 | (588, -144, -2900) |
| 1 | mortar1 | (1904, -232, -3712) |

This movement command appears directly in the stage's startup script. It requests one second of travel and gives deposited fires size-change timing arguments of 10 and 120 seconds. There is no random destination selection in this command.

These are the only ordinary-fire movement sequences in the 17 maps and 21 external scripts. All five route points belong to these two stages.

### 2. Small flames appearing at random nearby positions

A named ordinary fire can try to create a smaller nearby fire. The attempt runs when the game checks nearby things against the parent fire.

The sequence is:

- **Check whether the parent qualifies.** It must have a name, still have some horizontal size, be ready for another attempt, and be reached by a nearby check. The distance test uses the largest separation along any of the three coordinate directions, rather than a circular distance, and requires it to be under 750 game units after conversion to an integer.
- **Roll against the parent's strength.** At full strength the roll passes. At half strength it passes approximately half the time. A failed roll makes the parent wait another half-second before trying again.
- **Choose a direction and distance.** Direction is random around a full circle. Distance depends on the parent's current horizontal size and strength. At full strength, candidates range from half the smaller horizontal dimension out to the whole smaller dimension. A weakened fire chooses from a narrower band closer to the parent.
- **Check for a suitable surface.** The game starts the candidate 20 units above the parent's fire position and checks downward for up to 200 units with a small collision box. It rejects a miss or a hit that shifts sideways too far. This code does not select wood, fuel or another material to ignite.
- **Choose the new flame's shape and timing.** More random draws affect its dimensions. It starts with no horizontal size or height, reaches its larger size after about 0.75–3 seconds, then reaches its final collapsed shape after another 1–6 seconds. The final shape has zero width and height, with some length remaining.
- **Schedule the parent's next attempt.** After successfully creating a flame, the parent waits according to those timings. A bookkeeping count of nearby named fires lets it try again sooner when fewer than 12 are counted. At 12 or more it uses the normal wait. This changes how often flames are created.

If the surface check fails, this routine does not set the same half-second delay used for a failed strength roll. It can try again on a later qualifying check.

**A newly created, unnamed flame cannot repeat this random-spreading process.** The game creates it without an actor name, and the routine refuses unnamed parents. The deposited fires from the fixed routes are also created without a name. The original named fire can keep making additional nearby flames, but the normal creation path does not produce an endlessly branching chain of new parents.

The new flames use ordinary-fire damage handling and can burn the player.

### Why the random flames differ between runs

The game starts its random-number sequence using the computer's millisecond timer when the program launches. Fire does not have its own separate random-number sequence: other systems draw from the same one, including fire sound/appearance choices and other gameplay effects.

That means restarting the game can give different nearby flames. Even with the same starting seed, a different order or number of random calls can change which values fire receives. The identified seeding call runs at program startup.

A route such as Gas Station's four-point path stays fixed. The surrounding small flames can vary.

## What the gas shutoff actually does

Turning off the gas before touching the rupture trigger prevents the conditional rupture/spread sequence described above.

The shutoff also tells Fire1, Fire2, Fire9, Fire8 and Fire27 to shrink toward size (10, 10, 10) over two seconds, then records that the gas is off. Fire1 appears twice in the command.

The shutoff command contains no instruction to stop Fire42 or remove its existing trail.

There is a separate first explosion event. Touching `Explotion1_trigger` runs it once: it opens explosion/car models and enlarges Fire1, Fire2 and Fire3 over 0.6 seconds. Those size changes are separate from the Fire42 movement route.

## Burns, walls and passing out

The player has a buildup of burns, separate from air supply. Each accepted fire hit adds to that buildup. Once it crosses the game's limit, the player passes out from fire. The game reduces the buildup when its directional burn indicators are all clear; repeated exposure can prevent that recovery.

For ordinary fires:

- Damage depends on closeness and fire strength. It fades toward the edge of the fire's influence area.
- The distance calculation weights vertical separation more heavily than horizontal separation.
- The player's starting exposure allowance adds 45 game units to the calculation.
- If the game's line trace finds an obstruction, the calculated burn amount becomes one tenth of its previous value.
- Several accepted hits can each add burns. The burn routine has no overall per-update damage cap. A player who has passed out stops accepting further burns through it.

The “Serious Burns” statistic is counted on a cooldown tied to pain events. It is not a direct readout of the player's current burn buildup.

Fire checks and extinguisher hits can be queued and handled later. Flame-jet damage and carried-hose weakening act directly, so the order of spraying and damage matters.

## Flame jets and explosions

Some map effects are not ordinary fires. Two saved effect types have their own burn handling: one produces continuous torch-like flames; the other is an explosion whose burn handling is active only during particular animation frames.

| Stage | Continuous flame sources | Explosion sources |
|---|---:|---:|
| Subway B | 16 | 0 |
| Oil Rig B | 1 | 2 |
| Boat A | 0 | 4 |
| Boat B | 0 | 1 |

These are the only sources of these two types in the extracted maps. All other effect objects are still preserved in the catalog.

Subway B's 16 flame sources are arranged in four groups of four, named FireDoorA_1–8 and FireDoorB_1–8. Oil Rig B's `flame1` is given strength 10 and size 1.4 by its saved commands. Boat A's three named exploders are turned off/on and resized by commands; it also contains another explosion source.

Their damage depends on distance from a line segment representing the effect and on its configured strength. The effect must be within 2000 units of the camera and pass the engine’s visibility check. Its damage calculation then uses distance and strength, without a separate wall trace.

Commands, animation frames and size/orientation changes control when these effects are active and where they reach.

## The occasional apparent instant deaths

The cause of the sudden deaths you described is still unresolved.

Possible causes of rapid burns include unusually strong flame effects, several overlapping hits, or entering a fire while burns are already built up. At maximum contact and without burn recovery, Oil Rig B’s strength-10 flame adds enough burns to cross the threshold in a little over a tenth of a second.

The frame-time routine limits each update to a tenth of a second. Running out of air can also cause the player to pass out.

## Smoke, air and victims

Smoke, ash and fog have their own visual systems and level commands.

The air routine removes one twentieth of the full air supply every three seconds, with a separate refill branch. This routine does not use smoke density.

The ordinary-fire and flame-effect burn routines do not apply player burn buildup to victims. Victims ignore ordinary-fire damage messages, and flame/explosion damage selects the player.

## The truck-hose window fires

These stages use path nodes that define groups of window fires. We now have all 83 saved path nodes in the catalog, including 28 marked as hotspots:

| Stage | Path nodes | Hotspots |
|---|---:|---:|
| City | 19 | 9 |
| Construction | 34 | 8 |
| Dock | 17 | 5 |
| Oil Rig hose stage | 13 | 6 |

The game chooses a window fire within a group and manages a sequence for it. When that sequence begins, a progress value starts at zero. A ramp phase raises it toward full size/appearance. A separate dousing step lowers it. Under one shared setting, both can run during the same update; dousing can also be requested through a spray message.

The group waits for the selected fire's sequence before moving to the next fire, and tracks rounds through the group. It can also randomly trigger a flare. Deactivating a group cancels its active sequences.

## Still to investigate

The saved-data inventory covers all 17 maps and 21 external scripts: 430 ordinary fires, 230 effect objects, 83 hose-path nodes and the embedded entity scripts.

The remaining mechanics questions are:

- **Spray contact:** how aiming chooses contact positions, how many hits a spray produces, and when queued hits are applied.
- **Fire lifecycle:** exact recovery timing, cleanup and statistics for generated flames, and what later level commands do to extinguished fires.
- **Reused fire objects:** whether an old named fire can be reused without losing its name, allowing a generated flame to spread again.
- **Damage and walls:** later changes to the player's exposure size, including crouching, and the engine's wall-collision rules.
- **Smoke and victims:** whether other routines connect smoke to air consumption or give victims separate fire reactions.
- **Truck-hose stages:** aiming, pressure, cycles and difficulty rules.
- **Difficulty and time scale:** how these affect the wider fire and spray systems.
- **Gameplay checks:** recovery after spraying, overlapping fires, random flames, walls and shutting off gas during the rupture sequence.
- **Sudden deaths:** identify the cause if a reproducible example becomes available; this remains lower priority.

## Files in this folder

The folder stays self-contained with five files:

- `SUMMARY.md`: this plain-language explanation.
- `MECHANICS.md`: exact formulas, code references, detailed steps and unresolved technical questions.
- `catalog.json`: the saved fire, effect and path records, plus script references and spreading routes.
- `evidence.zip`: copied executable, raw map entity data, scripts, previous research, fresh code/assembly exports and file hashes. The coverage and spread audits are inside this archive too.
- `analyze.py`: rebuilds the catalog and verifies archived evidence using standard Python.
