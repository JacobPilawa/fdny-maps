# RescueBot Reverse Engineering Notes

Date: 2026-05-12  
Target: `FDNYFirefighter.exe`

## Summary

RescueBot is an automatic victim-rescue system. It can remove a victim from the stage without the player physically carrying that victim to an exit.

Confirmed entry points:

- Manual command path: `runrescuebot` -> `FUN_00475020` -> `FUN_00455230`
- Automatic update path: `FUN_00455580`
- Selection path: `FUN_00455230`
- Commit path: `FUN_00454750`

Current verified model:

1. RescueBot must be enabled and idle.
2. The stage/game state must allow RescueBot.
3. The auto-attempt timer must be open.
4. `FUN_00455230` scans runtime victim containers.
5. The first victim passing all runtime checks is selected.

Victim choice is not a direct random roll in `FUN_00455230`. Randomness appears in timers and radio/message selection, not in the candidate index.

## Runtime Timing Model

The global time source used in RescueBot code is `DAT_00502afc`. It is a game-time counter in milliseconds. It is not best understood as "time since this stage loaded"; RescueBot schedules deadlines by adding offsets to whatever the current game time is when a scheduler runs.

Timer source evidence:

```c
// FUN_0042bf20
DVar1 = timeGetTime();
*(DWORD *)(param_1 + 0xc) = DVar1;

// FUN_0042bf70
DVar3 = timeGetTime();
param_1[1] = DVar3 - param_1[2];
```

### Active Rescue Timer

When a victim is selected, `FUN_00454750` starts a short active timer:

```c
iVar7 = DAT_00502afc;
uVar3 = FUN_00404b30(4000);
*(uint *)this = uVar3 + 4000 + iVar7;
```

Meaning:

- Field `this+0x00` becomes `now + 4000 + random(0..3999)`.
- Units are milliseconds.
- Practical window is about 4 to 8 seconds.
- When it expires, `FUN_00455580` runs the drop-off transition and clears `currentVictim`.

Drop-off evidence:

```c
if ((param_1[1] != 0) && ((int *)*param_1 < DAT_00502afc)) {
  FUN_004504c0(pvVar6);
  ... "RESCUEBOT: Victim dropped off" ...
  param_1[1] = 0;
}
```

### Long Radio/Message Timer at `+0x10`

The `120000` value is real, but it is not the direct auto-attempt gate. It sets field `+0x10`.

```c
// FUN_00454100
*(int *)(param_1 + 0x10) = DAT_00502afc + 120000;
uVar1 = FUN_00404b30(120000);
*(uint *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + uVar1;
```

Meaning:

- Field `+0x10` becomes `now + 120000 + random(0..119999)`.
- Practical window is about 120 to 240 seconds.
- In `FUN_00455580`, this field is involved in a secondary radio/message path.

### Auto-Attempt Timer at `+0x14`

Field `+0x14` is the direct "next automatic RescueBot attempt time."

This is the timer that controls how often RescueBot is allowed to try selecting a victim automatically. It does not guarantee that a victim will be rescued; it only opens an attempt window. When the window opens, `FUN_00455580` calls `FUN_00455230`. If no victim passes the runtime checks, nothing is rescued, but the attempt timer is still rescheduled.

```c
// FUN_00455580
if ((int *)param_1[5] < local_84) {
  ...
  FUN_00455230((int)param_1);
  FUN_004541a0((int)param_1);
}
```

Because `param_1` is a `uint *`, `param_1[5]` is byte offset `+0x14`.

In plain terms:

- `DAT_00502afc` is current game time in milliseconds.
- `rescuebot+0x14` is the deadline for the next automatic attempt.
- If current game time is less than or equal to that deadline, RescueBot waits.
- If current game time is greater than the deadline, RescueBot tries once, then schedules a new deadline.

The call site proves the frequency behavior:

```asm
00455730 CALL 0x00455230  ; try to select/rescue a victim
0045578f CALL 0x004541a0  ; immediately schedule the next auto-attempt time
```

The scheduler is `FUN_004541a0`.

First it builds a base time from manager field `+0x343c`. The value is clamped between `180.0` and `7200.0`, then multiplied by `1000.0`, so this part is in milliseconds.

```asm
004541b4 FLD float ptr [ECX + 0x343c]
004541bb FCOM float ptr [0x004e1cbc] ; 180.0 upper/lower clamp bound path
004541d3 FCOM float ptr [0x004e1cb8] ; 7200.0 clamp bound path
0045423e FMUL float ptr [0x004e0ed4] ; 1000.0, seconds -> ms
```

Constants:

```text
0x004e1cbc = 180.0
0x004e1cb8 = 7200.0
0x004e0ed4 = 1000.0
```

Before the base time has been reached, the next attempt time is:

```c
rescuebot->nextAttemptTime = baseTime + random(0..29999);
```

Assembly proof:

```asm
00454253 CMP ECX,EDI       ; compare current time against base time
00454255 JNC 0x00454270    ; if current time >= base, use later branch
00454257 PUSH 0x7530       ; 30000
0045425c CALL 0x00404b30   ; random(30000)
00454264 ADD EAX,EDI       ; baseTime + random
00454266 MOV [EBX+0x14],EAX
```

After the base time has already passed, the next attempt time is based on current time plus a difficulty-scaled delay plus `random(0..59999)`.

```c
rescuebot->nextAttemptTime =
    now + scaledDelay + random(0..59999);
```

Assembly proof:

```asm
004542b7 TEST ECX,ECX
004542b9 FLD double ptr [EAX]       ; load scale factor
004542bb FMUL float ptr [0x004e1cb4]; 60000.0
004542c5 FILD dword ptr [ESP+0x10]  ; current time
004542d1 FADDP                      ; current time + scaled delay
004542d3 CALL 0x004b9400            ; round to integer ms
004542d8 PUSH 0xea60                ; 60000
004542e0 CALL 0x00404b30            ; random(60000)
004542eb ADD ECX,EAX
004542ef MOV [EBX+0x14],ECX
```

Constant:

```text
0x004e1cb4 = 60000.0
```

Practical interpretation:

- Yes, `+0x14` affects RescueBot frequency.
- It controls when the next automatic attempt is allowed.
- The early scheduling branch targets a base time plus up to 30 seconds of randomness.
- The normal recurring branch uses current time plus a scaled delay plus up to 60 seconds of randomness.
- An "attempt" is not the same as a guaranteed rescue. If no victim passes state, flag, blocked-LOS, and distance checks, the attempt finds nobody and the timer moves forward.

The scale factor is difficulty-related. The same field used by the RescueBot scheduler is written by the `difficulty` script command.

Difficulty command proof:

```c
// FUN_004792c0
puVar6 = (undefined4 *)(*(int *)(DAT_00502aac + 0x6e4) + 0x2c);

if (__stricmp(arg, "easy") == 0) {
  *puVar6 = 0;
}
else if (__stricmp(arg, "med") == 0) {
  *puVar6 = 1;
}
else if (__stricmp(arg, "hard") == 0) {
  *puVar6 = 2;
}
```

Difficulty display proof:

```c
// FUN_00423f20
if (difficulty == 0) return "easy";
if (difficulty == 1) return "medium";
if (difficulty == 2) return "hard";
```

RescueBot reads the same value:

```asm
004541e8 MOV EDX,dword ptr [0x00502aac]
004541ee MOV EAX,dword ptr [EDX + 0x6e4]
004541f4 MOV ESI,dword ptr [EAX + 0x2c] ; difficulty: 0 easy, 1 medium, 2 hard
```

Before the base time is reached, `FUN_004541a0` scales the base time:

| Difficulty | Selector | Scale | Effective first-attempt base |
|---|---:|---:|---|
| Easy | `0` | `0.6` | `baseSeconds * 0.6 * 1000 + random(0..29999)` |
| Medium/Normal | `1` | `0.75` | `baseSeconds * 0.75 * 1000 + random(0..29999)` |
| Hard | `2` | `1.0` | `baseSeconds * 1.0 * 1000 + random(0..29999)` |

After the base time has passed, `FUN_004541a0` scales the recurring interval:

| Difficulty | Selector | Recurring scaled delay | Practical recurring attempt spacing |
|---|---:|---:|---|
| Easy | `0` | `1.0 * 60000` ms | `60..119.999` seconds after the previous scheduling point |
| Medium/Normal | `1` | `2.0 * 60000` ms | `120..179.999` seconds after the previous scheduling point |
| Hard | `2` | `1000.0 * 60000` ms | `60000..60059.999` seconds, about `16.7` hours, effectively disabling normal repeated auto-rescues |

The hard-mode recurring value looks extreme, but the assembly is direct: hard selects the double constant `1000.0`, multiplies it by `60000.0`, then adds `random(60000)`.

Hard recurring proof:

```asm
00454270 TEST ESI,ESI
00454272 ... [ESP+0x10] = 1000.0
00454282 ... [ESP+0x18] = 2.0
00454292 ... [ESP+0x20] = 1.0
004542a4 LEA EAX,[ESP + 0x20] ; easy -> 1.0
004542aa CMP ESI,0x1
004542ad LEA EAX,[ESP + 0x18] ; medium -> 2.0
004542b3 LEA EAX,[ESP + 0x10] ; hard/other -> 1000.0
004542b9 FLD double ptr [EAX]
004542bb FMUL float ptr [0x004e1cb4] ; 60000.0
004542c5 FILD dword ptr [ESP+0x10]   ; current time
004542d8 PUSH 0xea60                 ; random(60000)
004542ef MOV [EBX+0x14],ECX          ; next auto-attempt deadline
```

Plain-language takeaway:

- Yes, difficulty affects RescueBot frequency.
- Easy gets the earliest first attempt and the shortest recurring interval.
- Medium/Normal waits longer.
- Hard can still get an initial attempt, but normal repeated RescueBot attempts are pushed so far into the future that they are probably irrelevant during normal play.
- The timer at `+0x14` is the important RescueBot frequency timer. The `120000` timer at `+0x10` is a separate radio/message timer and should not be used to predict automatic victim saves.

## Complete Timer Lifecycle

### Field Layout

The RescueBot object is treated as a small state block by the timing code:

| Offset | Meaning | Notes |
|---:|---|---|
| `+0x00` | active rescue drop-off deadline | Set only after a victim is selected. |
| `+0x04` | current victim pointer | Nonzero means RescueBot is already carrying/rescuing someone. |
| `+0x08` | victim-found radio line list | Used when a victim is selected. |
| `+0x0c` | generic radio line list | Used by the separate message timer. |
| `+0x10` | radio/message deadline | `now + 120000 + random(0..119999)`. Not the direct rescue-attempt timer. |
| `+0x14` | next automatic RescueBot attempt deadline | This is the frequency timer for auto-rescue attempts. |
| `+0x18` | enabled flag | Stage scripts can turn this off, e.g. Training. |

### Construction Does Not Start the Real Timer

`FUN_00451480` constructs/initializes the RescueBot block at world offset `+0x340c`.

Evidence:

```asm
00453f67 LEA ECX,[ESI + 0x340c]
00453f84 CALL 0x00451480
```

Constructor behavior:

```c
// FUN_00451480
malloc(0x18) ... zero fields ...
*(undefined1 *)(param_1 + 0x18) = 1;
```

Meaning:

- This creates support lists and sets `enabled = 1`.
- It does not schedule a meaningful `+0x14` auto-attempt deadline by itself.
- The real scheduling happens later in the level/player initialization path.

### Level/Player Start Schedules the First Timers

`FUN_00454ba0` is the important startup/reset routine for RescueBot timing. It is called from player/level setup:

```asm
0044df43 LEA ECX,[ESI + 0x340c]
0044df49 CALL 0x00454ba0
```

`FUN_00454ba0` does three important timer things:

1. Clears the current victim pointer.
2. Schedules the radio/message timer at `+0x10`.
3. Calls `FUN_004541a0`, which schedules the first automatic attempt deadline at `+0x14`.

Evidence:

```asm
004551e6 MOV EDX,dword ptr [0x00502afc]
004551f0 ADD EDX,0x1d4c0       ; 120000 ms
004551f6 PUSH 0x1d4c0          ; random(120000)
004551fb MOV [ESI+0x10],EDX
004551fe CALL 0x00404b30
00455203 ADD [ESI+0x10],EAX
0045520b CALL 0x004541a0       ; schedule +0x14
```

Plain-language sequence:

- When this setup routine runs, the game creates a future radio/message time between `120` and `240` seconds from the current game clock.
- It also creates the first RescueBot auto-attempt deadline through the difficulty-scaled scheduler.
- This is the closest currently verified point to "the RescueBot timer starts."

### What `now` Means

`now` is `DAT_00502afc`, a millisecond game-time counter.

It is not stored as "seconds since this stage started" inside RescueBot. RescueBot stores absolute deadlines against the current engine clock:

```c
deadline = DAT_00502afc + delay;
```

So if the setup routine runs when `DAT_00502afc == 50000`, a `120000` ms delay becomes deadline `170000`, not "120 seconds" as a standalone value.

### How Often RescueBot Checks the Timer

`FUN_00455580` is the RescueBot update function. It is called from the larger gameplay/world update:

```asm
00453870 LEA ECX,[ESI + 0x32cc]
00453876 CALL 0x00455580
```

This means RescueBot is polled by the game update loop. It is not a separate OS timer and not a background thread. The exact frames-per-second rate is not proven by this function alone, but the practical behavior is:

- Every gameplay update, the game checks `DAT_00502afc` against RescueBot's stored deadlines.
- If a deadline has not passed, nothing happens.
- If a deadline has passed, the relevant branch runs on that update tick.
- Therefore the real trigger moment is "the first gameplay update after `now > deadline`."

Important comparison detail:

```asm
004556ab MOV ECX,[ESP + 0x10] ; current DAT_00502afc captured earlier
004556af CMP ECX,[ESI + 0x14]
004556b7 JBE 0x00455794       ; if now <= deadline, skip auto-attempt
```

So the auto-attempt branch opens only when:

```c
now > rescuebot->nextAttemptDeadline;
```

### Update Gates Before Any Auto Attempt

Before `+0x14` can matter, `FUN_00455580` applies broad gates:

```c
if ((char)param_1[6] != '\0') {              // enabled flag at +0x18
  if (*(int *)((int)pvVar6 + 0x18c) != 6) {  // state 6 suppressed
    if ((param_1[1] == 0) &&
        (state == 3 || state == 4 || state == 5)) {
      ...
    }
  }
}
```

Plain-language meaning:

- RescueBot must be enabled.
- The mission/world state must not be state `6`.
- RescueBot must not already have a current victim.
- The world state must be one of `3`, `4`, or `5`.
- Only after those gates does it check whether `now > +0x14`.

### First Auto-Attempt Deadline

The scheduler `FUN_004541a0` can be read as `ScheduleNextAutoAttempt`. It first computes a difficulty-scaled base time:

```asm
004541b4 FLD float ptr [ECX + 0x343c]
004541bb FCOM float ptr [0x004e1cbc] ; 180.0
004541d3 FCOM float ptr [0x004e1cb8] ; 7200.0
0045423e FMUL float ptr [0x004e0ed4] ; 1000.0
00454244 FMUL double ptr [EAX]       ; difficulty scale
```

The source base value is `world+0x343c`. That field is initialized during player/world setup before `ScheduleNextAutoAttempt` uses it.

Default base-time setup evidence:

```asm
0044de17 MOV EAX,dword ptr [EAX + 0x10]       ; optional override source
0044de1a MOV dword ptr [ESI + 0x343c],EAX
0044de22 MOV dword ptr [ESI + 0x343c],0x44160000 ; default 600.0 seconds
0044de82 FLD float ptr [ESI + 0x343c]
0044de8e FMUL double ptr [EAX]                ; setup difficulty multiplier
0044de90 FSTP float ptr [ESI + 0x343c]
```

Meaning:

- The raw default is `600.0` seconds.
- If `DAT_00502aac+0x688` points to an override object, the raw value comes from that object's `+0x10` instead.
- During setup, the raw base is multiplied by a first difficulty multiplier.
- Later, `ScheduleNextAutoAttempt` clamps the resulting value to `180..7200` seconds, converts it to milliseconds, and applies a second scheduler difficulty multiplier.

Default setup base seconds:

| Difficulty | Raw default | Setup multiplier | `world+0x343c` after setup |
|---|---:|---:|---:|
| Easy | `600s` | `1.5` | `900s` |
| Medium/Normal | `600s` | `1.5` | `900s` |
| Hard | `600s` | `1.0` | `600s` |

`ScheduleNextAutoAttempt` then applies this first-attempt scale:

| Difficulty | Selector | First-deadline scale |
|---|---:|---:|
| Easy | `0` | `0.6` |
| Medium/Normal | `1` | `0.75` |
| Hard | `2` | `1.0` |

Default first auto-attempt window:

| Difficulty | Calculation | First auto-attempt deadline |
|---|---|---:|
| Easy | `900s * 0.6 + random(0..29.999s)` | `540..569.999s` on the game clock, `9:00..9:29.999` |
| Medium/Normal | `900s * 0.75 + random(0..29.999s)` | `675..704.999s` on the game clock, `11:15..11:44.999` |
| Hard | `600s * 1.0 + random(0..29.999s)` | `600..629.999s` on the game clock, `10:00..10:29.999` |

These are default-path values. If the optional override object changes the raw base from `600s`, use the same formulas with the override value.

Campaign rescue-stage first auto-attempt windows:

The campaign level file `Data/ResDefs/thegame.lev` gives each rescue stage a `time:` value in minutes. The parser multiplies that number by `60.0` and stores seconds at the level definition's `+0x10`; player/world setup copies that value into the RescueBot base-time path.

Parser proof:

```asm
00423645 PUSH 0x4df228              ; "time:"
0042368e LEA ESI,[EBX + 0x10]       ; current level definition +0x10
00423696 CALL 0x0040cc50            ; parse float
0042369d FMUL float ptr [0x004df224]; 60.0, minutes -> seconds
004236a3 FSTP float ptr [ESI]
```

First-attempt formula from the configured stage time:

| Difficulty | Formula from `time:` minutes |
|---|---|
| Easy | `timeMinutes * 54 + random(0..29.999s)` |
| Medium/Normal | `timeMinutes * 67.5 + random(0..29.999s)` |
| Hard | `timeMinutes * 60 + random(0..29.999s)` |

| Stage | `thegame.lev` time | Easy first attempt | Medium/Normal first attempt | Hard first attempt | Notes |
|---|---:|---:|---:|---:|---|
| Training | `500 min` | disabled | disabled | disabled | Training has `setrescuebot 0`; computed timer is not normally usable. |
| Military Base (`mbase`) | `5 min` | `4:30..4:59.999` | `5:37.500..6:07.499` | `5:00..5:29.999` | Enabled campaign rescue stage. |
| Downtown Bar (`bar_r`) | `10 min` | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` | Enabled campaign rescue stage. |
| Gas Station (`gas_r`) | `8 min` | `7:12..7:41.999` | `9:00..9:29.999` | `8:00..8:29.999` | Enabled campaign rescue stage. |
| Bank Robbery (`bank`) | `15 min` | `13:30..13:59.999` | `16:52.500..17:22.499` | `15:00..15:29.999` | Enabled campaign rescue stage. |
| Capsized Boat (`boat_a`) | `15 min` | `13:30..13:59.999` | `16:52.500..17:22.499` | `15:00..15:29.999` | Enabled campaign rescue stage. |
| Subway Accident (`subway_a`) | `12 min` | `10:48..11:17.999` | `13:30..13:59.999` | `12:00..12:29.999` | Enabled campaign rescue stage. |
| Oil Rig Rescue (`oil_rig_a`) | `10 min` | `9:00..9:29.999` | `11:15..11:44.999` | `10:00..10:29.999` | Enabled campaign rescue stage. |

Extracted rescue-capable map variants without direct `thegame.lev` entries:

| Map | First-attempt timing status |
|---|---|
| `boat_b` | No direct `thegame.lev` entry found. If loaded without a level-definition override, the fallback raw base is `600s`, giving Easy `9:00..9:29.999`, Medium/Normal `11:15..11:44.999`, Hard `10:00..10:29.999`. |
| `subway_b` | No direct `thegame.lev` entry found. Same fallback/default note as `boat_b`. |
| `oil_rig_B` | No direct `thegame.lev` entry found. Same fallback/default note as `boat_b`. |
| `oil_rig_C` | No direct `thegame.lev` entry found. Same fallback/default note as `boat_b`. |
| `oil_rig_D` | No direct `thegame.lev` entry found. Same fallback/default note as `boat_b`. |

Why this is still described as a window, not a single exact wall-clock moment:

- The random term is real: `FUN_00404b30(30000)` adds `0..29999` ms.
- RescueBot stores an absolute game-clock deadline, not a stopwatch that starts from a visible "stage loaded" event.
- The setup function normally runs near level/player setup, but this analysis has not proven the exact user-visible frame that should be called "stage start."
- The branch runs on the first gameplay update where `gameTimeNow > nextAutoAttemptTime`, so the actual visible event can be delayed by up to one update tick after the deadline.
- If `DAT_00502aac+0x688` supplies an override base value, the default `600s` calculation does not apply.

Then it compares the current game clock against that computed base deadline:

```asm
0045424b MOV ECX,dword ptr [0x00502afc] ; now
00454253 CMP ECX,EDI                    ; compare now to base deadline
00454255 JNC 0x00454270                 ; if now >= base, use recurring branch
```

If the base deadline is still in the future:

```c
nextAttempt = baseDeadline + random(0..29999);
```

If the base deadline has already passed:

```c
nextAttempt = now + recurringDifficultyDelay + random(0..59999);
```

This distinction matters. The first call during level setup schedules against an absolute difficulty-scaled base point, not simply `now + N`, unless that base point has already passed. In plain terms, the first attempt is based on "the stage's RescueBot base time," while later attempts are "N seconds after the previous attempt window."

### Recurring Auto-Attempt Frequency

After an auto-attempt window opens, `FUN_00455580` always calls `FUN_004541a0` again:

```asm
00455730 CALL 0x00455230  ; only if victim-cap check passes
0045578f CALL 0x004541a0  ; always reschedule after this auto-attempt window
```

Recurring delay table:

| Difficulty | Selector | Formula | Practical interval |
|---|---:|---|---|
| Easy | `0` | `now + 60000 + random(0..59999)` | `60..119.999s` |
| Medium/Normal | `1` | `now + 120000 + random(0..59999)` | `120..179.999s` |
| Hard | `2` | `now + 60000000 + random(0..59999)` | about `16.7h`, effectively no normal repeat |

Recurring attempts are not stage-specific in the code found so far. Once the first attempt window has happened, stage time no longer appears in the recurring formula. The per-stage differences come from gates and victim availability, not from the recurring timer formula.

Per-stage recurring timer table:

| Stage | Easy recurring auto-attempt window | Medium/Normal recurring auto-attempt window | Hard recurring auto-attempt window | Notes |
|---|---:|---:|---:|---|
| Training | disabled | disabled | disabled | `setrescuebot 0`. |
| Military Base (`mbase`) | `60..119.999s` after scheduling | `120..179.999s` after scheduling | `60000..60059.999s` after scheduling | Timer may open; rescue still depends on gates and victim checks. |
| Downtown Bar (`bar_r`) | `60..119.999s` | `120..179.999s` | `60000..60059.999s` | Same recurring formula. |
| Gas Station (`gas_r`) | `60..119.999s` | `120..179.999s` | `60000..60059.999s` | Same recurring formula. |
| Bank Robbery (`bank`) | `60..119.999s` | `120..179.999s` | `60000..60059.999s` | Same recurring formula. |
| Capsized Boat (`boat_a`) | `60..119.999s` | `120..179.999s` | `60000..60059.999s` | Same recurring formula. |
| Subway Accident (`subway_a`) | `60..119.999s` | `120..179.999s` | `60000..60059.999s` | Same recurring formula. |
| Oil Rig Rescue (`oil_rig_a`) | `60..119.999s` | `120..179.999s` | `60000..60059.999s` | Same recurring formula. |

Recurring-timer caveats:

- The values above are the base recurring schedule immediately after an automatic attempt window.
- If a victim is successfully selected, `CommitRescueBotVictim` sets a `4..7.999s` active drop-off timer. When that drop-off completes, `FUN_00454130` adds `10s` to any still-future `nextAutoAttemptTime`.
- Therefore, after a successful rescue, the next visible attempt window is usually the recurring window plus another `10s`: Easy `70..129.999s`, Medium/Normal `130..189.999s`, Hard `60010..60069.999s`.
- The separate radio/message timer can also call `FUN_00454130`; if it fires before the next auto attempt, it can add another `10s` delay.
- If no victim is selected during an attempt window, there is no drop-off push, so the plain recurring table applies unless the radio/message timer intervenes.

### Expired Timer Does Not Guarantee a Scan

When `now > +0x14`, the game does not immediately scan victims unconditionally. It first computes a difficulty-scaled maximum number of RescueBot victims allowed.

Evidence:

```asm
004556bd MOV EDX,dword ptr [0x00502aac]
004556c3 MOV EAX,dword ptr [EDX + 0x6e4]
004556c9 MOV EAX,dword ptr [EAX + 0x2c] ; difficulty
...
00455713 FILD dword ptr [EBP + 0x3528]  ; stage/world victim-count value
00455719 FMUL double ptr [EAX]          ; difficulty cap factor
0045571b FADD double ptr [0x004de6f8]   ; +0.5 before rounding
00455721 CALL 0x004b9400                ; rounded allowed maximum
00455728 CMP dword ptr [EBP + 0x3530],EDI
0045572e JGE 0x00455739                 ; if already at/over cap, skip selector
00455730 CALL 0x00455230                ; scan victims
```

The cap factors are:

| Difficulty | Selector | Cap factor applied to `[world+0x3528]` |
|---|---:|---:|
| Easy | `0` | `0.4` |
| Medium/Normal | `1` | `0.25` |
| Hard | `2` | `0.0` |

Plain-language meaning:

- The expired `+0x14` timer opens an attempt window.
- The victim selector only runs if the stage has not already reached its difficulty-scaled RescueBot victim cap.
- On hard, this cap factor is `0.0`, so the allowed automatic victim count likely rounds to zero unless the base value or rounding behavior creates an exception.
- Whether the selector runs or is skipped, the code still reschedules `+0x14` afterward.

### Why an Attempt Window May Not Rescue Anyone

The stage/difficulty tables above show when the game first *tries to open* a RescueBot attempt window. They do not mean RescueBot is guaranteed to rescue someone at that time.

Practical interpretation:

- The listed first-attempt time is the earliest scheduled automatic RescueBot opportunity.
- On that update, `UpdateRescueBot` may call `TrySelectVictimForRescueBot`.
- `TrySelectVictimForRescueBot` can still find no valid victim.
- If no victim is found, the code logs `"RESCUEBOT: No eligible victims"` and schedules the next attempt.

Failure reasons before the victim scan:

| Gate | Code evidence | Practical meaning |
|---|---|---|
| RescueBot disabled | enabled flag at `rescuebot+0x18` | Training disables RescueBot with `setrescuebot 0`; other scripts could do the same. |
| Suppressed mission state | `world/state +0x18c == 6` suppresses update | Some mission/result/cutscene state prevents RescueBot work. |
| RescueBot already busy | `currentVictim` at `rescuebot+0x04` is nonzero | RescueBot cannot start another rescue while carrying/rescuing a victim. |
| Wrong gameplay state | update only proceeds for state `3`, `4`, or `5` | Timer may expire while the game is not in a normal rescue gameplay state. |
| Difficulty victim cap reached | compare `[world+0x3530]` against rounded cap | If the stage has already hit its allowed RescueBot-rescued count, the selector is skipped. |

Failure reasons inside `TrySelectVictimForRescueBot`:

| Per-victim check | Code evidence | Practical meaning |
|---|---|---|
| Runtime state must be `4` or `7` | `victim+0x120 == 4 || victim+0x120 == 7` | The victim must be in one of two live states that RescueBot considers rescuable: ordinary waiting/idle (`4`) or roam-positioning/roam-capable (`7`). |
| Internal exclusion flag must be clear | byte at `victim+0x1fd == 0` | The victim must not have run the `norescuebot` victim script command. |
| Player-to-victim trace must be blocked | `FUN_00490130(...) == 0` | RescueBot prefers victims the player does not have a clear collision trace to. If the trace is clear, the victim fails this gate. |
| Distance must be at least `600.0` units | distance >= `_DAT_004e1f50` | Victims too close to the player are not auto-rescued. |
| First valid victim wins | `FUN_00454750(...)` then immediate return | The scan stops at the first runtime-eligible victim; later victims are not considered during that attempt. |

Timer outcome matrix:

| What happens at the scheduled time? | Does someone get rescued? | What happens next? |
|---|---|---|
| Timer expires, but broad gate fails | No | Auto-attempt branch does not run yet; behavior depends on which gate failed. |
| Timer expires, cap already reached | No | Selector is skipped and `ScheduleNextAutoAttempt` reschedules the next window. |
| Timer expires, selector runs, no victim passes | No | Logs `"No eligible victims"` and `ScheduleNextAutoAttempt` reschedules. |
| Timer expires, selector finds a victim | Yes, after delay | `CommitRescueBotVictim` starts a `4..7.999s` drop-off timer. |

So for Oil Rig Rescue, the table does **not** mean "someone will always be rescued by about 10 minutes." It means:

- On Easy, Oil Rig's first automatic attempt window opens at `9:00..9:29.999`.
- On Medium/Normal, it opens at `11:15..11:44.999`.
- On Hard, it opens at `10:00..10:29.999`.
- A victim is rescued only if all broad gates and per-victim checks pass at that moment.
- If the scan fails, the next attempt is scheduled using the recurring difficulty interval.

### If a Victim Is Selected

`FUN_00455230` scans victim candidates. If one passes, it calls `FUN_00454750`.

`FUN_00454750` commits the rescue:

```c
*(int **)((int)this + 4) = param_1;       // current victim pointer
iVar7 = DAT_00502afc;
uVar3 = FUN_00404b30(4000);
*(uint *)this = uVar3 + 4000 + iVar7;     // active drop-off deadline
```

Meaning:

- RescueBot becomes "busy" because `+0x04` is nonzero.
- The selected victim is not dropped off instantly.
- Drop-off is scheduled for `4..7.999` seconds later.

During this busy period, the auto-attempt branch is blocked because `param_1[1] != 0`.

### Drop-Off and Timer Pushes

When RescueBot is busy and the active drop-off timer expires:

```c
if ((param_1[1] != 0) && ((int *)*param_1 < DAT_00502afc)) {
  FUN_004504c0(pvVar6);
  param_1[1] = 0;
  FUN_00454130((int)param_1);
  FUN_00454100(extraout_ECX);
}
```

`FUN_00454130` pushes future deadlines forward by `10000` ms if they are still in the future:

```c
if (DAT_00502afc < *(uint *)(param_1 + 0x10)) {
  *(uint *)(param_1 + 0x10) += 10000;
}
if (DAT_00502afc < *(uint *)(param_1 + 0x14)) {
  *(uint *)(param_1 + 0x14) += 10000;
}
```

Then `FUN_00454100` resets the radio/message deadline:

```c
messageDeadline = now + 120000 + random(0..119999);
```

Plain-language meaning:

- After a successful rescue finishes, the message timer is reset.
- Any still-future message/attempt deadlines are nudged 10 seconds later.
- If `+0x14` was already in the past, `FUN_00454130` does not move it. The later idle branch can then notice it and reschedule through the normal auto-attempt path.

### Separate Radio/Message Timer

The `+0x10` timer is checked in the same update function, after the auto-attempt branch:

```asm
00455798 CMP ECX,dword ptr [ESI + 0x10]
0045579b JBE 0x004558d0       ; if now <= message deadline, skip message
...
004558a1 CALL 0x00454130
004558a6 CALL 0x00454100      ; reset +0x10
```

This branch chooses a random generic radio line and displays it. It is not the victim-selection trigger.

### Debug and Manual Paths

Debug activation:

```asm
00455650 MOV AL,[0x00502a78]
...
004556a3 MOV EAX,[ESP + 0x10] ; now
004556a7 INC EAX
004556a8 MOV [ESI + 0x14],EAX ; next attempt deadline = now + 1
```

Meaning:

- If the debug activation condition `DBG_ACTIVATE_RESCUEBOT` is true, the game sets `+0x14` to `now + 1`.
- That makes the auto-attempt branch eligible on a following update once `now` advances past that value.

Manual command:

- `runrescuebot` calls into `FUN_00455230` through `FUN_00475020`.
- This bypasses waiting for `+0x14`, but the same victim selector/runtime checks still matter.

### Practical End-to-End Timeline

Descriptive names used below:

| Real function/field | Descriptive name | Meaning |
|---|---|---|
| `FUN_00454ba0` | `InitializeRescueBotTimers` | Startup/reset function that fills radio lines and starts the timer schedule. |
| `FUN_004541a0` | `ScheduleNextAutoAttempt` | Writes `nextAutoAttemptTime` at `rescuebot+0x14`. |
| `FUN_00455580` | `UpdateRescueBot` | Per-gameplay-update polling function. |
| `FUN_00455230` | `TrySelectVictimForRescueBot` | Scans victims and tries to find an eligible candidate. |
| `FUN_00454750` | `CommitRescueBotVictim` | Marks a victim as selected and starts the short drop-off timer. |
| `rescuebot+0x10` | `nextRadioMessageTime` | Separate radio chatter timer. |
| `rescuebot+0x14` | `nextAutoAttemptTime` | Main automatic RescueBot attempt timer. |
| `rescuebot+0x00` | `activeDropoffTime` | Time when the currently selected victim gets dropped off. |
| `rescuebot+0x04` | `currentVictim` | Nonzero while RescueBot is busy with a victim. |

Practical timeline, assuming RescueBot is enabled and the level is loaded through `thegame.lev`:

1. `InitializeRescueBotTimers` (`FUN_00454ba0`) runs during player/world setup.
2. It sets `currentVictim` to `0`, meaning RescueBot starts idle.
3. It schedules `nextRadioMessageTime` (`rescuebot+0x10`) to `now + 120 to 239.999 seconds`. This is only radio chatter timing, not the rescue trigger.
4. It calls `ScheduleNextAutoAttempt` (`FUN_004541a0`) to write `nextAutoAttemptTime` (`rescuebot+0x14`).
5. `ScheduleNextAutoAttempt` uses the stage/world base time at `world+0x343c`. For campaign rescue stages, that base comes from the stage's `time:` entry in `thegame.lev`; see the stage table above.
6. `ScheduleNextAutoAttempt` applies RescueBot's first-attempt difficulty scaling and randomness. For example, Gas Station (`time: 8`) gets Easy `7:12..7:41.999`, Medium/Normal `9:00..9:29.999`, and Hard `8:00..8:29.999`.
7. Every gameplay update calls `UpdateRescueBot` (`FUN_00455580`). This is a polling check. There is no separate RescueBot thread or OS timer.
8. Until `gameTimeNow > nextAutoAttemptTime`, `UpdateRescueBot` does not open an automatic rescue attempt.
9. On the first gameplay update after `gameTimeNow > nextAutoAttemptTime`, `UpdateRescueBot` checks the broad gates: RescueBot enabled, mission state allowed, RescueBot idle, world state `3/4/5`, and difficulty-scaled victim cap not exceeded.
10. If those gates pass, `UpdateRescueBot` calls `TrySelectVictimForRescueBot` (`FUN_00455230`).
11. `TrySelectVictimForRescueBot` scans victims in runtime order. A victim still needs to pass state checks, flag checks, blocked player-to-victim trace, and minimum `600` unit distance.
12. If no victim passes, no one is rescued. `UpdateRescueBot` still calls `ScheduleNextAutoAttempt`, so the next attempt window moves to the recurring schedule.
13. If a victim passes, `CommitRescueBotVictim` (`FUN_00454750`) stores that victim in `currentVictim` and sets `activeDropoffTime` to `now + 4 to 7.999 seconds`.
14. While `currentVictim` is nonzero, RescueBot is busy. The automatic attempt branch is blocked during this time.
15. On the first gameplay update after `gameTimeNow > activeDropoffTime`, `UpdateRescueBot` drops the victim off, clears `currentVictim`, pushes still-future timers by `10` seconds, and resets `nextRadioMessageTime`.
16. Future automatic attempt windows use the recurring difficulty schedule, which is not stage-specific: Easy `60..119.999s` after scheduling, Medium/Normal `120..179.999s`, and Hard `60000..60059.999s`.
17. If the attempt successfully selected a victim, the later drop-off usually adds `10s` to that already-scheduled next attempt, making the practical post-success windows Easy `70..129.999s`, Medium/Normal `130..189.999s`, and Hard `60010..60069.999s`.

Uncertainty note:

- The constants and formulas above are exact for the documented campaign path.
- The visible timing is a window because the code adds random milliseconds, stores absolute game-clock deadlines, and only checks them during gameplay updates.
- First-attempt values depend on the configured stage time. If a map is loaded without a `thegame.lev` level definition, the observed code has a fallback raw base of `600s`.

## Victim Selection Checks

The selector is `FUN_00455230`.

```c
if (((piVar2[0x48] == 4) || (piVar2[0x48] == 7)) &&
    (*(char *)((int)piVar2 + 0x1fd) == '\0')) {
  cVar3 = FUN_00490130(victim_pos, player_pos, 0);
  if (cVar3 == '\0') {
    if ((float)_DAT_004e1f50 <= distance) {
      FUN_00454750(local_e4, piVar2, piStack_e8);
      return;
    }
  }
}
```

Important decompiler notation correction:

- `piVar2[0x48]` means "read the 32-bit value at `victim + 0x48 * 4`."
- The real byte offset is `victim+0x120`.
- The assembly confirms this:

```asm
00455399 MOV EAX,dword ptr [ESI + 0x120]
0045539f CMP EAX,0x4
004553a4 CMP EAX,0x7
```

Runtime checks:

- `victim+0x120` must be state value `4` or `7`.
- `victim+0x1fd` byte must be `0`.
- Player-to-victim trace must be blocked.
- Player-to-victim distance must be at least `600.0`.

The distance constant at `0x004e1f50` is `600.0`:

```text
bytes: 00 00 00 00 00 c0 82 40
double: 600.0
```

## Runtime Eligibility vs Static Map Data

The BSP files identify static victim entities (`RCS_VictimRescue`) and their designer fields. They do not contain the live runtime values checked by `FUN_00455230`.

Static BSP data can show:

- Victim entity order
- Victim name
- Victim position
- Victim `D_Type`, such as `man`, `woman`, `dog`
- Victim `J_Radius`
- Linked alt-spots

Static BSP data cannot prove:

- `victim+0x120 == 4`
- `victim+0x120 == 7`
- `victim+0x1fd == 0`
- Current runtime LOS/collision result
- Current runtime container order after initialization

Therefore the plots mark **static RescueBot candidates**, not guaranteed eligible victims. Training is the exception: it has `setrescuebot 0`, so normal auto RescueBot behavior should be disabled there.

## What State 4 and State 7 Mean

Short version:

- State `4` is the normal live victim state after a victim has been created and is waiting/idle in the world.
- State `7` is set by the victim `roam` script command while the victim is being prepared for roaming/repositioning.
- Both states are intentionally accepted by RescueBot.
- Neither state is a person type. They do not mean `man`, `woman`, `dog`, etc.

### State 4: Normal Spawned / Waiting Victim

Victim creation sets state `4` after the model/entity has been created.

`FUN_0045b690` (`VictimRescueEnt.cpp`, victim creation path):

```asm
0045bc7b MOV EBP,0x4
0045bc80 MOV dword ptr [ESI + 0x120],EBP
```

The same function later clears the RescueBot exclusion flag:

```asm
0045bd4e MOV byte ptr [ESI + 0x1fd],BL
```

In this function, `BL` is `0`, so new valid victims start with `victim+0x1fd = 0`.

The animation/idle helper also returns normal non-special victims to state `4`.

`FUN_00459a50`:

```asm
00459a91 MOV dword ptr [ESI + 0x120],0x4
```

Plain-language meaning: after creation or after choosing a normal idle/beckon animation, the victim is in the ordinary "available in the world" state. RescueBot accepts this state.

### State 7: Roam Setup / Roaming Victim

State `7` is set by `FUN_0045c7f0`.

```asm
0045c815 MOV dword ptr [ESI + 0x224],EAX
0045c820 MOV dword ptr [ESI + 0x120],0x7
0045c82a MOV dword ptr [ESI + 0x1c8],EDX
```

That function:

- saves the roam radius/limit argument at `victim+0x224`
- sets `victim+0x120 = 7`
- saves the current victim position at `victim+0x1c8..0x1d0`
- tries nearby `+/-5` unit offsets with `FUN_0045ab40`
- logs `"Victim (...) is stuck when spawned!"` and falls back to state `4` if all tested offsets fail

The caller is the victim command parser `FUN_0045cdb0`. It compares a script command against the literal string `"roam"` at `0x004e2544`, parses the numeric argument, and calls `FUN_0045c7f0`.

```asm
0045d067 PUSH 0x4e2544        ; "roam"
0045d06d CALL 0x004ca140      ; string compare
...
0045d10b MOV EDX,dword ptr [ESP + 0x18]  ; parsed roam argument
0045d116 CALL 0x0045c7f0
```

This is visible in the extracted map data. Gas Station has multiple victim scripts like:

```text
extracted/gas_r/entities.txt
S_Script01: roam 250
S_Script01: roam 80
S_Script01: roam 400
S_Script01: roam 350
```

Stage-level visibility from `extracted/rescuebot_entity_inventory.csv`:

| Stage | Victims | With `roam N` | Without `roam` | `norescuebot` scripts | D_Type values |
|---|---:|---:|---:|---:|---|
| `bank` | 6 | 2 | 4 | 0 | boy, fatman, girl, man, woman |
| `bar_r` | 6 | 6 | 0 | 0 | fatman, man, woman |
| `boat_a` | 9 | 2 | 7 | 0 | boy, fatman, girl |
| `boat_b` | 5 | 2 | 3 | 0 | boy, fatman, girl, man, woman |
| `gas_r` | 7 | 6 | 1 | 0 | man, woman |
| `mbase` | 5 | 2 | 3 | 0 | man |
| `oil_rig_A` | 3 | 0 | 3 | 0 | girl, man, woman |
| `oil_rig_B` | 3 | 1 | 2 | 0 | dog, man, woman |
| `oil_rig_C` | 6 | 6 | 0 | 0 | fatman, man, woman |
| `oil_rig_D` | 3 | 3 | 0 | 0 | man, woman |
| `subway_a` | 6 | 4 | 2 | 0 | fatman, man, woman |
| `subway_b` | 10 | 5 | 5 | 0 | boy1, dog, fatman, man, woman |
| `training` | 2 | 0 | 2 | 0 | man |

Plain-language meaning: `roam N` is a designer script telling that victim they can roam within a radius/limit of `N` map units. The code uses state `7` while setting up that roaming behavior. RescueBot accepts state `7`, so roaming-capable victims are not excluded just because they are in that roam setup/roam state.

Important nuance:

- For normal victim types, `FUN_0045c7f0` eventually calls `FUN_00459a50(this, 2)`, and `FUN_00459a50` usually writes state `4` again.
- For the special `herodown` victim profile, `FUN_00459a50` takes a different branch and does not visibly write state `4` in that path.
- No extracted campaign rescue map currently shows `D_Type: herodown`; the visible campaign victim types are things like `man`, `woman`, `fatman`, `girl`, `boy`, and `dog`.
- The safe conclusion is that state `7` is tied to the `roam` handling path, but live runtime sampling would be needed to prove exactly how long normal roaming victims remain in state `7` after initialization.

### `norescuebot` Flag

The per-victim exclusion flag checked by RescueBot is visible in the same victim command parser.

`FUN_0045cdb0` compares against the literal string `"norescuebot"` at `0x004e2538`:

```asm
0045d13e PUSH 0x4e2538        ; "norescuebot"
0045d144 CALL 0x004ca140      ; string compare
0045d160 MOV byte ptr [EBP + 0x129],BL
```

In this parser, `EBP` is pointing at `victim+0xd4`, so `EBP+0x129` is the real victim offset:

```text
victim + 0xd4 + 0x129 = victim + 0x1fd
```

The current extracted campaign/map data does not show any `norescuebot` victim scripts:

```text
rg -n "norescuebot" extracted "FDNY Firefighter 1.0/FireFighter American Hero/Data"
```

Only documentation notes currently contain that word. So the command exists in the executable, but the extracted shipped rescue maps do not appear to use it.

### State 5 and State 6 Context

Other nearby state writes help confirm that `+0x120` is a runtime state field:

| State | Code evidence | Meaning inferred from command/animation |
|---|---|---|
| `4` | `FUN_0045b690`, `FUN_00459a50` write `victim+0x120 = 4` | Normal spawned/idle/available victim. |
| `5` | `FUN_0045bef0` writes `victim+0x120 = 5` and emits `"pickup"` | Being picked up by the player. |
| `6` | `FUN_0045bf70` writes `victim+0x120 = 6` and emits `"botrescue"` | Being handled by RescueBot. |
| `6` | `FUN_0045c000` writes `victim+0x120 = 6` and emits `"rescue"` | Being rescued/handled through the normal rescue flow. |
| `7` | `FUN_0045c7f0` writes `victim+0x120 = 7`; called by `"roam"` script handling | Roam setup / roam-capable state. |

`MapZone.cpp` logic also treats states `5` and `6` as special: it skips victims in those states when assigning victims to zones.

```asm
0048438a MOV EAX,dword ptr [ESI + 0x120]
00484390 CMP EAX,0x5
00484393 JZ ...
00484395 CMP EAX,0x6
00484398 JZ ...
```

That supports the interpretation that states `5` and `6` are "currently being carried/rescued/handled" states, while `4` and `7` are still valid in-world candidates.

## Victim Type Field

The runtime field `victim+0x120` is not the same as the map/data victim type.

Data-file victim types:

```text
Data/ResDefs/victims.srf
Victim: boy
Victim: woman
Victim: man
Victim: fatman
Victim: girl
Victim: dog
Victim: herodown
```

Map entities expose:

```text
RCS_VictimRescue
D_Type: man / woman / dog / etc.
J_Radius: 40 or 60 in observed maps
```

Code reads:

```c
piVar2[0x48] == 4 || piVar2[0x48] == 7
```

Conclusion:

- `D_Type` is an asset/behavior profile from data files.
- `victim+0x120` is a live runtime state gate.
- `piVar2[0x48]` is only Ghidra's pointer-array notation for the same `+0x120` field.
- There is no evidence that `4` or `7` means dog, woman, man, etc.

## LOS / Collision Polarity

The helper chain is:

```text
FUN_00455230 -> FUN_00490130 -> FUN_0048fea0 -> geWorld_Collision
```

`FUN_0048fea0`:

```c
iVar1 = geWorld_Collision(...);
return (bool)('\x01' - (iVar1 != 0));
```

Then `FUN_00455230`:

```c
cVar3 = FUN_00490130(...);
if (cVar3 == '\0') {
  // continue to distance check and possible rescue
}
```

Interpretation:

- `geWorld_Collision != 0` means the trace hit something.
- `FUN_0048fea0` returns `0` when the trace hit something.
- `FUN_00455230` continues only when the helper returns `0`.
- RescueBot wants a blocked trace.
- Victims with a clear direct trace from the player fail this gate.

The LOS plots are approximations. They use simplified 2D wall segments from the extracted BSP. The game uses a 3D GenesisD collision query, so the plots are useful for intuition but are not proof of final runtime LOS.

## Training Stage

Training is explicitly special:

- Extracted `training` map data has `setrescuebot 0` on `RCS_PlayerRescue`.
- `FUN_00455580` also suppresses updates when mission state is `6`.

Normal conclusion:

- Training victims may exist in static map data.
- Training should not auto-rescue victims through normal RescueBot flow.

## Gas Station Candidate Order

Source: `extracted/gas_r/annotated_entities.json`, sorted by `RCS_VictimRescue` entity index.

| Order | Label | Name | entity_index | X | Z |
|---|---|---|---:|---:|---:|
| 1 | V1 | Victim1 | 278 | 1404 | 24 |
| 2 | V2 | Victim2 | 282 | 1576 | 512 |
| 3 | V3 | Victim3 | 283 | 756 | 648 |
| 4 | V4 | Victim4 | 284 | 1269 | 1404 |
| 5 | V5 | Victim6 | 285 | 1988 | 480 |
| 6 | V6 | Victim7 | 286 | 1972 | -204 |
| 7 | V7 | Victim5 | 401 | -404 | 1164 |

This is a static order model. Runtime scan order is likely related but can still differ if container initialization reorders entities.

## Map Visualizations

Generated per-stage plots:

- `rescuebot_explainer_order.png`: static victim candidate order
- `rescuebot_explainer_checks.png`: static distance from a sample player point plus runtime-only check list
- `rescuebot_explainer_los.png`: approximate 2D blocked-trace visualization

Map projection:

- Projection uses engine `X/Z`.
- `Y` is height.
- PNG orientation is matched to the displayed orientation of `*_topdown_annotated_with_table.pdf`.
- Higher horizontal surfaces are drawn after lower surfaces so the plot reads as top-down rather than bottom-up.
- LOS lines are simplified and should not be treated as exact Genesis3D collision results.

Generated data:

- `extracted/rescuebot_stage_victim_order.csv`
- `extracted/rescuebot_stage_victim_order.json`

## Player Collision Notes

Generated diagram:

- `extracted/player_collision_extents.png`

Two boxes are currently documented:

### Actor Render Hint Box

`FUN_00492340` defines an axis-aligned box with min corner `(-24, -24, -24)` and max corner `(24, 24, 24)`.

Decompiled proof:

```c
// FUN_00492340
local_18 = 0xc1c00000; // -24.0
local_14 = 0xc1c00000; // -24.0
local_10 = 0xc1c00000; // -24.0
local_c  = 0x41c00000; // +24.0
local_8  = 0x41c00000; // +24.0
local_4  = 0x41c00000; // +24.0
geActor_SetRenderHintExtBox(*(undefined4 *)(param_1 + 0x60), &local_18, 0);
```

Assembly proof:

```asm
0049234c c7 44 24 04 00 00 c0 c1  ; -24.0
00492354 c7 44 24 08 00 00 c0 c1  ; -24.0
0049235c c7 44 24 0c 00 00 c0 c1  ; -24.0
00492364 c7 44 24 10 00 00 c0 41  ; +24.0
0049236c c7 44 24 14 00 00 c0 41  ; +24.0
00492374 c7 44 24 18 00 00 c0 41  ; +24.0
0049237c ff 15 6c c1 4d 00        ; geActor_SetRenderHintExtBox
```

Shape and size:

- Shape: axis-aligned cube/box.
- Width: `48` units.
- Height: `48` units.
- Depth: `48` units.

This is render/culling metadata, not direct proof of physical movement collision.

### Swept Collision Box

The wrapper at `0x00482740` passes `15.0` into `FUN_00490000`.

Assembly proof:

```asm
00482769 68 00 00 70 41  ; push 0x41700000 = 15.0
00482774 e8 87 d8 00 00  ; call FUN_00490000
```

Inside `FUN_00490000`, the radius-like value is multiplied by `0.5` before min/max extents are stored.

Assembly proof:

```asm
00490017 d9 44 24 0c       ; load radius argument
00490020 dc 0d f8 e6 4d 00 ; multiply by [0x004de6f8]
```

Constant proof:

```text
0x004de6f8 bytes: 00 00 00 00 00 00 e0 3f
double value: 0.5
```

`FUN_00490000` then stores negative and positive copies:

```asm
0049002d d9 e0             ; change sign
0049002f d9 15 4c 32 50 00 ; store negative extent
00490041 d9 15 40 32 50 00 ; store positive extent
004900d8 ff 15 bc c0 4d 00 ; geWorld_Collision
```

Shape and size:

- Shape: swept axis-aligned cube/box.
- Input size: `15.0`.
- Half extent after `0.5` multiplier: `7.5`.
- Min/max extents: approximately `(-7.5, -7.5, -7.5)` to `(7.5, 7.5, 7.5)`.
- Full width/height/depth: `15` units.

This is stronger evidence for gameplay collision because it is passed into `geWorld_Collision`.

Practical difference:

- Render hint box is renderer bookkeeping. You do not directly interact with it in gameplay.
- Swept collision box is a movement/collision test shape and is more relevant to fitting, clipping, and bumping.

## Reproduce

```bash
python tools/render_rescuebot_diagrams.py
python tools/render_player_hitbox_diagram.py
```
