# RescueBot Simple Handoff

Date: 2026-05-12

This document explains only RescueBot: what it is, when it tries to save someone, which victims it checks, and what happens after an attempt.

## The Short Version

RescueBot is an automatic rescue helper. It can save a victim without the player carrying that victim to the safe zone.

The basic flow is:

1. Stage starts and the RescueBot timer begins.
2. After a stage/difficulty-specific delay, RescueBot gets one attempt.
3. It scans victims in a fixed order.
4. It saves the first victim who passes the checks.
5. If nobody passes, nobody is saved.
6. After that, RescueBot schedules the next attempt based mostly on difficulty.

A RescueBot attempt is not a guaranteed rescue. It is only the game saying, “try to find someone valid now.”

## Full Flow

### 1. Stage Starts

When the stage loads, the game initializes RescueBot.

Training is special: Training disables RescueBot with `setrescuebot 0`, so normal automatic RescueBot saves should not happen there.

### 2. First Timer Starts

The first RescueBot attempt time depends on the stage and difficulty.

Each time is a range because the game adds a random delay of up to about 30 seconds, then RescueBot actually runs on the next gameplay update after that timer expires.

| Difficulty | Training First RescueBot Attempt | Military Base First RescueBot Attempt | Downtown Bar First RescueBot Attempt | Gas Station First RescueBot Attempt | Bank Robbery First RescueBot Attempt | Capsized Boat A First RescueBot Attempt | Subway Accident A First RescueBot Attempt | Oil Rig Rescue A First RescueBot Attempt |
|---|---:|---:|---:|---:|---:|---:|---:|---:|
| Easy | disabled | `4:30..4:59.999` | `9:00..9:29.999` | `7:12..7:41.999` | `13:30..13:59.999` | `13:30..13:59.999` | `10:48..11:17.999` | `9:00..9:29.999` |
| Medium/Normal | disabled | `5:37.500..6:07.499` | `11:15..11:44.999` | `9:00..9:29.999` | `16:52.500..17:22.499` | `16:52.500..17:22.499` | `13:30..13:59.999` | `11:15..11:44.999` |
| Hard | disabled | `5:00..5:29.999` | `10:00..10:29.999` | `8:00..8:29.999` | `15:00..15:29.999` | `15:00..15:29.999` | `12:00..12:29.999` | `10:00..10:29.999` |

That means RescueBot first tries somewhere in that time window. It does not mean somebody will definitely be saved.

### 3. Timer Expires

When the timer expires, RescueBot checks whether it is allowed to run.

It will not run if:

- RescueBot is disabled, like in Training.
- RescueBot is already busy saving someone.
- The game is in a state where rescue logic is paused or suppressed.
- The difficulty limit for automatic saves has already been reached.

### 4. RescueBot Scans Victims In Order

If RescueBot is allowed to run, it scans the victims in a fixed order for that stage.

It does not randomly pick a victim during this scan.

The first victim who passes all checks gets selected. Later victims are ignored for that attempt.

### 5. Each Victim Has To Pass These Checks

A victim can be saved by RescueBot only if all of these are true:

| Check | Simple meaning |
|---|---|
| Victim is in a rescuable state | They are still an available victim in the world. |
| Victim is not marked `norescuebot` | The game has not explicitly excluded them from RescueBot. |
| Victim is at least `600` units from the player | RescueBot does not save victims who are too close to you. |
| Player does not have a clear collision/line trace to the victim | Practically: RescueBot prefers victims you cannot directly “see/reach” through clear space. |

Important: “clear trace” failing is the weird part. If the player has a clear trace to the victim, RescueBot does not pick them. If the trace is blocked, they can pass this check.

### 6. If A Victim Passes

If a victim passes all checks:

1. RescueBot selects that victim.
2. RescueBot becomes busy.
3. About `4..7.999` seconds later, the victim is dropped off / counted as saved.
4. RescueBot then schedules a future attempt.

After a successful save, the next attempt is usually:

| Difficulty | Next attempt after a successful save |
|---|---:|
| Easy | `70..129.999` seconds later |
| Medium/Normal | `130..189.999` seconds later |
| Hard | About `16.7` hours later, effectively no normal repeat |

### 7. If No Victim Passes

If no victim passes all checks:

1. Nobody is saved.
2. RescueBot still schedules the next attempt.
3. The next attempt is usually sooner than after a successful save, because there is no drop-off delay.

If the attempt fails, the next attempt is:

| Difficulty | Next attempt after a failed/no-save attempt |
|---|---:|
| Easy | `60..119.999` seconds later |
| Medium/Normal | `120..179.999` seconds later |
| Hard | About `16.7` hours later, effectively no normal repeat |

## First Attempt Times By Stage

These are the first automatic RescueBot attempt windows.

| Stage | Easy | Medium/Normal | Hard | Notes |
|---|---:|---:|---:|---|
| Training | disabled | disabled | disabled | Training uses `setrescuebot 0`. |
| Military Base | `4:30..4:59.999` | `5:37.500..6:07.499` | `5:00..5:29.999` | Enabled. |
| Downtown Bar | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` | Enabled. |
| Gas Station | `7:12..7:41.999` | `9:00..9:29.999` | `8:00..8:29.999` | Enabled. |
| Bank Robbery | `13:30..13:59.999` | `16:52.500..17:22.499` | `15:00..15:29.999` | Enabled. |
| Capsized Boat A | `13:30..13:59.999` | `16:52.500..17:22.499` | `15:00..15:29.999` | Enabled. |
| Subway Accident A | `10:48..11:17.999` | `13:30..13:59.999` | `12:00..12:29.999` | Enabled. |
| Oil Rig Rescue A | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` | Enabled. |

Some later map chunks do not have their own campaign timing entry in the files inspected so far. If loaded by themselves without an override, they appear to use the 10-minute default timing:

| Map chunk | Easy | Medium/Normal | Hard |
|---|---:|---:|---:|
| Boat B | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` |
| Subway B | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` |
| Oil Rig B | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` |
| Oil Rig C | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` |
| Oil Rig D | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` |

## Victim Order By Stage

This is the order RescueBot is expected to check victims. It checks from top to bottom and picks the first valid victim.

### Military Base

1. `victim1`, man
2. `victim3`, man
3. `victim4`, man, roaming
4. `victim5`, man
5. `victim10`, man, roaming

### Downtown Bar

1. `Victim01`, fatman, roaming
2. `Victim02`, woman, roaming
3. `Victim03`, man, roaming
4. `Victim04`, man, roaming
5. `Victim05`, fatman, roaming
6. `Victim06`, woman, roaming

### Gas Station

1. `Victim1`, man, roaming
2. `Victim2`, woman, roaming
3. `Victim3`, man, roaming
4. `Victim4`, man, roaming
5. `Victim6`, woman, roaming
6. `Victim7`, woman, roaming
7. `Victim5`, man

### Bank Robbery

1. `betty`, girl
2. unnamed woman, roaming
3. unnamed girl
4. unnamed boy
5. unnamed man
6. unnamed fatman, roaming

### Capsized Boat A

1. `victimfork`, boy
2. `girl`, girl
3. `victim3`, girl
4. `boy2`, boy, roaming
5. `fatty`, fatman
6. `fatman`, fatman
7. `fatman1`, fatman
8. `fat`, fatman
9. `girl`, girl, roaming

### Boat B

1. `victim3`, girl
2. `boy2`, boy, roaming
3. `fatty`, fatman
4. unnamed man
5. `erg`, woman, roaming

### Subway Accident A

1. `Victim1`, fatman
2. `Victim2`, man
3. `Victim3`, woman, roaming
4. `Victim4`, woman, roaming
5. `Victim5`, man, roaming
6. `Victim7`, man, roaming

### Subway B

1. `Victim1`, fatman
2. `Victim2`, fatman
3. `Victim4`, dog
4. `Victim5`, boy
5. `Victim6`, woman
6. `Victim7`, woman, roaming
7. `Victim9`, woman, roaming
8. `Victim10`, man, roaming
9. `Victim11`, woman, roaming
10. `Victim12`, man, roaming

### Oil Rig A

1. unnamed woman
2. unnamed girl
3. unnamed man

### Oil Rig B

1. unnamed man
2. unnamed dog
3. `woman`, woman, roaming

### Oil Rig C

1. unnamed fatman, roaming
2. unnamed man, roaming
3. `fatman behind rubble`, fatman, roaming
4. `man on ledge1`, man, roaming
5. `woman at console`, woman, roaming
6. `other man in A1`, man, roaming

### Oil Rig D

1. `man`, man, roaming
2. `man1`, man, roaming
3. `woman`, woman, roaming

### Training

Training has two victims, but automatic RescueBot is disabled.

1. `victim1`, man
2. `victim2`, man

## Plain Examples

### Gas Station on Medium/Normal

1. Stage starts.
2. First RescueBot attempt happens at `9:00..9:29.999`.
3. RescueBot checks Gas Station victims in this order: Victim1, Victim2, Victim3, Victim4, Victim6, Victim7, Victim5.
4. For each victim, it checks distance, blocked trace, and victim state.
5. First victim who passes gets saved.
6. If someone is saved, the next attempt is usually `130..189.999` seconds later.
7. If nobody is saved, the next attempt is `120..179.999` seconds later.

### Oil Rig A on Easy

1. Stage starts.
2. First RescueBot attempt happens at `9:00..9:29.999`.
3. RescueBot checks woman, then girl, then man.
4. First valid one gets saved.
5. If none are valid, nobody gets saved and RescueBot tries again `60..119.999` seconds later.

## Simple Takeaways

- RescueBot is timer-based.
- The first timer depends on stage and difficulty.
- Later timers mostly depend only on difficulty.
- RescueBot scans victims in a fixed order.
- RescueBot saves the first valid victim, not a random victim.
- Victims close to the player do not qualify.
- Victims with a clear player-to-victim trace do not qualify.
- Training disables automatic RescueBot.
- Hard difficulty appears to heavily suppress repeated automatic saves.
