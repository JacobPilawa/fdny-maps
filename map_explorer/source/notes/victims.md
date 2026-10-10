# Victim Spawn and Alternate Spot Notes

This document summarizes what can be inferred from the decoded map files, script files, resource definitions, and executable strings about `RCS_VictimRescue` and `RCS_VictimAltSpot`.

No Ghidra/decompiler pass was used for this document. The exact C++ selection code is still unknown.

## Short Version

- A normal survivor is an `RCS_VictimRescue` entity.
- An alternate spawn point is an `RCS_VictimAltSpot` entity.
- Each alt spot links to one survivor using `A_Victim`, which points at that survivor's internal `%name%`, not the human `A_Name`.
- In Training, there are two survivor entities: `victim1` and `victim2`.
- In Training, each survivor has two alternate spawn spots.
- I found no Training script or trigger that explicitly activates a particular survivor or causes an alternate spot to appear later.
- The best current interpretation is that victim spawn choice happens internally when the level/victim initializes: each `RCS_VictimRescue` gets a spawn candidate list consisting of its base `location` plus any linked `RCS_VictimAltSpot` records.
- The executable contains the strings `RCS_VictimAltSpot`, `Registering SpawnSpot`, `randomize`, and `forcespawnspot`, which strongly supports the idea that alt spots are registered as spawn candidates and then selected/randomized internally.
- Runtime testing or decompilation is required to prove the exact probability distribution and timing.

## Training Victims

Training contains these survivor-related entities:

| Map Label | Entity Index | Internal Name | Editor Name | Type | Base Location | Scripts |
|---|---:|---|---|---|---|---|
| `V1` | 191 | `RCS_VictimRescue1` | `victim1` | `man` | `1104 -60 -4976` | `pickup: ifnot gotvictim 1 (*Cmd3c)`; `pickup: set gotvictim 1` |
| `A1` | 192 | `RCS_VictimAltSpot1` | | | `1300 -120 -4952` | linked to `RCS_VictimRescue1` |
| `A2` | 193 | `RCS_VictimAltSpot2` | | | `1472 -120 -4972` | linked to `RCS_VictimRescue1` |
| `V2` | 194 | `RCS_VictimRescue2` | `victim2` | `man` | `1656 -120 -4968` | `pickup: if gotvictim 1 (*Cmd3b)`; `pickup: ifnot gotvictim 1 (*Cmd3c)`; `pickup: set gotvictim 1` |
| `A3` | 195 | `RCS_VictimAltSpot3` | | | `1820 -120 -4960` | linked to `RCS_VictimRescue2` |
| `A4` | 196 | `RCS_VictimAltSpot4` | | | `1972 -120 -4960` | linked to `RCS_VictimRescue2` |

### Candidate Spawn Sets

The likely candidate spawn sets are:

| Survivor | Candidate Spots |
|---|---|
| `V1` / `RCS_VictimRescue1` | base `1104 -60 -4976`; alt `1300 -120 -4952`; alt `1472 -120 -4972` |
| `V2` / `RCS_VictimRescue2` | base `1656 -120 -4968`; alt `1820 -120 -4960`; alt `1972 -120 -4960` |

If the engine includes the base location in the random pool, each Training victim has three candidate positions. If the engine only uses alt spots when at least one alt exists, each Training victim has two candidate positions plus a fallback base. The files alone do not prove which model is used.

## Training Trigger Flow Around Victims

Relevant Training final-section entities:

| Label | Entity | Location | Script Meaning |
|---|---:|---|---|
| `T14` | 188 | `1800 -232 -3768` | Shows final exercise hint: locate and rescue survivors. |
| `T15` | 189 | `1648 -220 -3188` | Shows transition hint and clears ash density. |
| `T16` | 190 | `2012 -104 -3136` | `makezone (Survivor Extraction Exercise)` |
| `T17` | 197 | `1272 -208 -4036` | After 3 seconds, shows goggles hint; once equips goggles. |
| `T18` | 204 | `2032 -232 -4128` | `touch: tell player escape` |
| `E1` | 238 | `1712 -48 -4028` | Escape arrow. |

None of those trigger scripts mentions `RCS_VictimRescue`, `RCS_VictimAltSpot`, `forcespawnspot`, `spawn`, or an explicit victim activation command.

Therefore, for Training, triggers appear to teach and gate the player, not spawn the victims. The victims are already map entities, and their final position is probably resolved by the victim/alt-spot system during level setup.

## Pickup Scripts In Training

Training has a `gotvictim` script variable, but it does not appear to control spawning.

`victim1`:

```text
pickup: ifnot gotvictim 1 (*Cmd3c)
pickup: set gotvictim 1
```

`victim2`:

```text
pickup: if gotvictim 1 (*Cmd3b)
pickup: ifnot gotvictim 1 (*Cmd3c)
pickup: set gotvictim 1
```

Training map script:

```text
set Cmd3b (hint 7 (Great job!  Take this person to the exit, and your training will be complete!))
set Cmd3c (hint 5 (You picked up the survivor!  Now head for the flashing exit.))
```

Interpretation:

- `gotvictim` is a tutorial/hint state variable.
- Picking up either victim can set `gotvictim` to `1`.
- `victim2` has an extra branch that shows the "Great job!" message if `gotvictim` is already set.
- This does not prove that only one survivor is active.
- The executable contains survivor-count strings such as `All of the survivors have been rescued!`, `There are %i survivors left!`, and `Good job, but there are still %i more survivors!`, so the safer assumption is that every valid `RCS_VictimRescue` entity counts unless the engine suppresses it internally.

## What Causes Alternate Spots To Appear?

Current evidence says: nothing in the map script causes them to appear.

The alt spots are normal decoded BSP entities:

```text
classname: RCS_VictimAltSpot
A_Victim: RCS_VictimRescue1
ZZ_location: 1300 -120 -4952
```

The executable contains these relevant strings:

```text
RCS_VictimAltSpot
rescue\VictimAltSpotEnt.cpp
Registering SpawnSpot
TE_VicimtAltSpot: Not pointing to a Victim!
randomize
forcespawnspot
```

That strongly suggests the flow is:

1. The level loads entities.
2. Each `RCS_VictimAltSpot` looks up its linked `RCS_VictimRescue` by `A_Victim`.
3. The alt spot registers itself as a spawn spot for that victim.
4. The victim system chooses a final spawn spot internally.
5. `forcespawnspot` likely exists as a debug/script command to override the random choice, but no current map script uses it.

The exact timing is not proven. It is probably during entity spawn/initialization, not when the player enters a trigger.

## Are Victims Activated By Zones?

Probably not in the sense of "spawned by zones."

The executable has zone/count strings:

```text
%i survivors in this area
1 survivor in this area
TMapZoneManager: **ERROR** Victim ...
```

Training has:

```text
makezone (Survivor Extraction Exercise)
```

This implies zones know which victims are inside them and can report survivor counts by area. It does not imply that entering a zone spawns or activates those victims.

In Training, the survivor zone trigger is near the exercise area, but the victims and alt spots are independent entities after it in the entity list. No decoded script links the zone trigger to victim creation.

## Cross-Map Pattern

Across maps, the same structure repeats:

- Maps contain multiple `RCS_VictimRescue` survivors.
- Some survivors have one or more `RCS_VictimAltSpot` rows.
- Alt spots link to a victim by internal `%name%`, e.g. `RCS_VictimRescue2`.
- Many victims have no alt spots and should use only their base `location`.
- Many victims use behavior scripts like `roam 1000`, which likely affects movement after spawning, not spawn selection.

Examples:

| Map | Pattern |
|---|---|
| `training` | 2 victims, each with 2 alt spots. |
| `subway_a` | 6 victims; only `RCS_VictimRescue2` has 2 alt spots. |
| `subway_b` | 10 victims; 4 victims have 1 alt spot each. |
| `gas_r` | 7 victims; no decoded alt spots. |
| `mbase` | 5 victims; no decoded alt spots. |

This supports the idea that alternate spots are per-victim optional spawn candidates, not a global pool.

## Current Confidence

High confidence:

- Training contains two `RCS_VictimRescue` survivor entities.
- Training contains four `RCS_VictimAltSpot` entities.
- `A1` and `A2` belong to `V1`; `A3` and `A4` belong to `V2`.
- Training disables rescuebot with `setrescuebot 0`.
- Training scripts do not explicitly call `forcespawnspot`, `randomize`, or any victim activation command.
- Alt spots are linked by internal victim id, not by map label or editor `A_Name`.

Medium confidence:

- Each victim chooses among its base location and linked alt spots during level initialization.
- Trigger zones are used for hints/area names/survivor counts, not for spawning victims.
- `roam N` affects post-spawn movement, not spawn selection.

Low confidence / unknown:

- Exact RNG distribution.
- Whether the base `RCS_VictimRescue.location` is always part of the random choice when alt spots exist.
- Whether the random choice is per victim independently or influenced by a level-wide seed/order.
- Whether difficulty affects victim spawning; I found no decoded Training script evidence for this.
- Whether `forcespawnspot` is exposed to console, script, or only internal debug builds.

## Practical Routing Implication

For route planning, treat every `RCS_VictimRescue` as an active survivor and treat linked `RCS_VictimAltSpot` records as possible locations for that same survivor.

For Training specifically:

- Plan for `V1` at one of `V1`, `A1`, or `A2`.
- Plan for `V2` at one of `V2`, `A3`, or `A4`.
- Do not assume a trigger makes a survivor appear later; the final positions are likely decided before you reach the survivor exercise.

## How To Prove The Remaining Details

The next useful tests are runtime tests:

1. Load Training many times and record where `victim1` and `victim2` appear.
2. Check whether each victim ever appears at its base `V#` location when alt spots exist.
3. Check whether the choices are independent, e.g. whether all `V1`/`V2` combinations can occur.
4. Try console/debug commands if available: `debugvictims`, `forcespawnspot`, and `randomize`.
5. If runtime testing is inconclusive, decompile `VictimRescueEnt.cpp` / `VictimAltSpotEnt.cpp` references in `FDNYFirefighter.exe`, focusing on these strings:

```text
RCS_VictimAltSpot
Registering SpawnSpot
forcespawnspot
randomize
TE_VictimRescue
Victim will not be created
```

