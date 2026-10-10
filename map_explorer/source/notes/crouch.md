# Crouch Deep Dive (FDNYFirefighter.exe)

Date: 2026-05-12
Scope: crouch mechanics, crouch-related state changes, and "crouch bug" (stand state with crouch squat retained).

## TL;DR

- Crouch is a real player state (`state = 4`) with lower movement cap and lower vertical body offset.
- Normal uncrouch tries to stand only if there is room overhead.
- Ladder scripts (`climb` / `unclimb`) can move the player through non-crouch states without restoring the crouch offset.
- This creates the crouch bug: you are no longer in crouch state, but still keep crouch squat properties.

## 1) Input And Commands

## Action registration

`FUN_004395c0` registers gameplay actions including `JUMP` and `CROUCH`.

It also binds `CROUCH` to multiple key codes:

- `0x2e`
- `0x1d`
- `0x9d`

This matches Training map hint text:

- `"Press 'C' or 'CTRL' to crouch."`

Evidence in extracted data:

- `extracted/training/entities.txt` line with hint text about `C` or `CTRL`.

## Script/command parser

`FUN_00451870` handles player commands including:

- `jump`
- `crouch`
- `uncrouch`
- `climb`
- `unclimb`

This matters because ladders in map scripts directly call `climb` and `unclimb`.

## 2) Player State Values Used By Crouch/Ladder

From the crouch/climb code paths:

- `state = 3`: normal standing/movement state
- `state = 4`: crouch state
- `state = 5`: climb state

Related additional state seen nearby:

- `state = 7` from `FUN_0044bc40` (special transition/interaction state, used by another trigger path)

Primary state field is at player offset:

- `player+0x18c`

## 3) What Crouch Changes

## 3.1 Crouch state + body offset

Crouch entry logic (state 3 -> 4) sets:

- `player+0x18c = 4`
- `player+0x88 = DAT_004e1820`

`DAT_004e1820 = 25.0` (float).

Uncrouch success sets:

- `player+0x18c = 3`
- `player+0x88 = DAT_004e181c`

`DAT_004e181c = 85.0` (float).

Interpretation:

- `+0x88` acts like vertical body/height anchor used by stand/crouch collision placement.
- Crouch lowers this value from `85` to `25`.

## 3.2 Movement speed cap

In the movement update block around `0x00453163`:

- default cap is `400.0`
- if `state == 4` (crouch), cap becomes `260.0`
- if special flag at `+0x240` is set, cap becomes `800.0`

Evidence:

- `0x43c80000 = 400.0`
- `0x43820000 = 260.0`
- `0x44480000 = 800.0`

So crouch clearly reduces movement cap.

## 4) Normal Crouch/Uncrouch Logic

## Crouch

Crouch is applied when the crouch action is processed and standing is allowed.

Two implementation paths exist:

- direct logic in `FUN_00451870` (`crouch` command)
- helper `FUN_0044bcb0` called by movement update path

Both set state 4 and the crouch offset.

## Uncrouch

Uncrouch path uses `FUN_0044bce0`:

- only runs if current state is 4
- builds a test position using current `x,z` and standing `y=85`
- calls collision test (`FUN_00492df0`)
- if blocked overhead: stay crouched
- if clear: set state 3 and restore `+0x88 = 85`

This is why normal uncrouch can fail in tight spaces.

## 5) Ladder/Climb Integration

Map scripts frequently do this on ladder triggers:

- touch: `action Player (climb 1)`
- untouch: `action Player unclimb`

Verified across many maps in `extracted/*/entities.txt` (subway, boat, oil rig, mbase, training, etc.).

## Climb command behavior

`climb` in `FUN_00451870` calls `FUN_0044c0b0`.

`FUN_0044c0b0`:

- if not already climbing and some guard passes:
- sets `state = 5`
- zeroes movement vector at `+0xfc`

Important: it does NOT restore standing offset (`+0x88`).

## Unclimb command behavior

`unclimb` in `FUN_00451870` does:

- if `state == 5`, set `state = 3`

Important: it does NOT restore standing offset (`+0x88`).

That is the core bug vector.

## 6) Crouch Bug (Ladder Version) - Reconstructed Sequence

Starting conditions:

1. You crouch normally.
2. State becomes `4`.
3. Crouch offset becomes `+0x88 = 25`.

Then ladder interaction:

4. Ladder touch trigger runs `climb 1`.
5. Climb sets `state = 5`.
6. Offset `+0x88` is unchanged (still `25`).

Then ladder exit:

7. Ladder untouch trigger runs `unclimb`.
8. Unclimb sets `state = 3`.
9. Offset `+0x88` is still unchanged (still `25`).

Result:

- You are no longer in crouch state (`state 3`), but still have crouch-like squat offset.
- This matches observed "come out of crouch but keep squat" behavior.

## Why this happens

Normal uncrouch (`FUN_0044bce0`) is the path that restores `+0x88` to standing value (`85`).

Ladder exit (`unclimb`) bypasses that restoration and only flips state.

## 7) Other Potential Crouch-Bug Inducers

Besides touch-ladder -> unclimb, anything that:

- leaves crouch-related offset low (`+0x88 = 25`), and
- changes state away from crouch without calling normal uncrouch restoration

can produce similar behavior.

Confirmed candidate:

- Jump while climbing path (`FUN_0044e990`):
  - if `state == 5`, it sets `state = 3`
  - does not restore `+0x88`

So if you entered climb while crouched, a climb-exit via this path can also preserve squat.

Likely candidates to test (state-changing systems):

- Any scripted `climb`/`unclimb` transitions (very common in map triggers).
- Any other system that forces `state` from 4/5 to 3 without going through `FUN_0044bce0`.

## 8) Practical Testing Matrix

For runtime verification in gameplay:

1. Baseline:
- Stand -> crouch -> uncrouch in open area.
- Expect full restore (no bug).

2. Ladder bug core:
- Crouch.
- Touch ladder trigger (`climb`).
- Leave ladder (`unclimb`).
- Check if stand state with retained squat appears.

3. Climb jump exit:
- Crouch.
- Enter climb.
- Exit climb via jump-like transition.
- Check retained squat.

4. Tight-space uncrouch control:
- Crouch under low ceiling.
- Attempt uncrouch.
- Verify normal uncrouch fails (expected, not bug), then compare against ladder-induced state transition.

## 9) Key Proof Snippets

Crouch state/offset set (helper path):

```asm
0044bcc9 MOV EDX,[0x004e1820]    ; 25.0
0044bccf MOV [ECX+0x88],EDX
0044bcd5 MOV [ECX+0x18c],0x4
```

Uncrouch success restore:

```asm
0044bd40 MOV EAX,[0x004e181c]    ; 85.0
0044bd45 MOV [ESI+0x88],EAX
0044bd4c MOV [ESI+0x18c],0x3
```

Climb state set:

```asm
FUN_0044c0b0:
... MOV [ECX+0x18c],0x5
```

Unclimb state clear only:

```c
if (*(int *)((int)this + 0x4c) == 5) {
  *(undefined4 *)((int)this + 0x4c) = 3;
}
```

Crouch speed cap in movement update:

```asm
if state==4 -> cap = 260.0
else         cap = 400.0
```

## 10) Current Confidence

High confidence:

- Crouch state id (`4`) and climb state id (`5`).
- Crouch modifies vertical offset (`+0x88`) to `25`.
- Normal uncrouch restores it to `85` only on successful uncrouch path.
- Ladder scripts call `climb` and `unclimb` widely.
- `unclimb` changes state without restoring the standing offset.
- This directly explains crouch bug behavior.

Medium confidence:

- Exact gameplay meaning of every nearby state (`6`, `7`) outside crouch/climb core.
- Whether every observed crouch bug variant maps to this same offset field (very likely, but not yet instrumented live in memory).
