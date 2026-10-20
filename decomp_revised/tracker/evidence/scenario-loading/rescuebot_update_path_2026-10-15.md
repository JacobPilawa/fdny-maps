# RescueBot Player update path (2026-10-15)

Status: **the shared port now models the recurring RescueBot lifecycle behind
an explicit `setrescuebot 1`; retail constructor defaults and live cadence
remain unresolved.** Evidence is static analysis of the pinned
`FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`,
including a fresh Ghidra 12.0.4 headless decompilation and instruction-level
checks. No retail runtime capture was made.

## Enable command reaches the recurring callback

Player command handler `00451870` recognizes `setrescuebot`, parses its first
argument with the integer helper (`sscanf` `%i`), and stores the nonzero test
at Player `+0x32E4`. Player update `00452570` calls
`00455580((uint*)(Player + 0x32CC))` in its normal-state branch, after the
item update. Its Player-state-6 branch calls `00451080` and bypasses this
normal-state work.

The callback's gate `(char)param_1[6] != 0` reads callback offset `+0x18`.
Because its argument is `Player+0x32CC`, that byte is exactly
`Player+0x32E4`. This closes the static writer-to-reader path: `setrescuebot`
controls the recurring callback. The outer Player update's scheduler and
cadence are not recovered, so this does not establish a wall-clock interval
for `00455580`.

When enabled and outside Player state 6, `00455580` checks its active-victim
deadline and attempt deadline. It only attempts selection in Player states 3,
4, or 5. Its maximum is the x87 expression `int(totalVictims * factor + 0.5)`,
where the mode factors are `0.4`, `0.25`, or `0.0`; the `0.5` constant is at
`0x004DE6F8`. `004B9400` converts toward zero. If Player `+0x3530` is below
that maximum, the callback calls selector `00455230`.

The selector scans registered VictimRescue candidates in manager iteration
order. It accepts states 4 and 7, skips the embedded `norescuebot` byte at
victim `+0x1FD`, requires an obstructed Player-to-victim world segment, and
accepts distance greater than or equal to 600 units. It takes the first
eligible candidate.

## Selection and state effects

`00454750` randomly chooses one of eight authored response strings, increments
Player `+0x3530`, and calls `0045BF70`. It stores that victim as the active
target and sets its next deadline to current game time plus 4,000 ms plus one
bounded draw in `[0,4000)`. `0045BF70` enters victim state 6, dispatches
`botrescue`, writes `UINT32_MAX` to victim `+0x1F0`, resets the actor animation,
and clears normal-render eligibility through `00481140(..., false)`. When the
active deadline expires, `00455580` calls `004504C0` to refresh final-exit
availability, clears the active target, extends a future attempt deadline by
10,000 ms, and schedules another RescueBot message deadline.

The same callback also chooses among five generic RescueBot prompts when its
message deadline expires. `00454100` schedules that deadline at current game
time plus 120,000 ms plus a bounded draw in `[0,120000)`. `00454130` adds
10,000 ms to each of the two stored deadlines only when that deadline is still
in the future. The initial `00454BA0` routine builds both message lists and
calls `004541A0`, but it is passed Player `+0x340C`; its exact relationship to
the recurring callback object at `+0x32CC` remains unresolved.

Developer command `runrescuebot` is a separate entry: shared dispatcher
`0047A1F0` calls `00475020`, which directly invokes selector `00455230` on
Player `+0x340C`. That one-shot call does not pass through the recurring
callback's `Player+0x32E4` enable gate or its attempt-limit check. The selector
still rejects a second active victim in the context passed to it.

## One-shot command entry rechecked (2026-10-20)

The pinned EXE SHA-256 remains
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. A
bounded x86 disassembly of `00475020` confirms its complete entry sequence:

```text
00475020  mov eax, [00503A78]
00475025  mov ecx, [eax+30]
00475028  add ecx, 340C
0047502E  call 00455230
00475033  mov al, 1
00475035  ret
```

The dispatcher call site is in the saved `0047A1F0` decompilation at
[`0047a1f0_script_command_dispatch.c`](0047a1f0_script_command_dispatch.c).
This verifies the one-shot selector entry and its context offset. The recurring
callback receives Player `+0x32CC`; the connection between that object and the
one-shot/initializer context at `+0x340C` is still unresolved. The shared
viewer has no equivalent global `runrescuebot` command dispatch, so this path
remains unported; do not merge it into the recurring `RescueBotState` until the
object relationship is traced. This is static evidence, not a live command or
rescue behavior check.

## Authored coverage and port boundary

The hash-pinned inventory contains no authored `runrescuebot`. Training's
Player entity 115 explicitly sets `setrescuebot 0`; the other preserved map
scripts contain no `setrescuebot` command. The reviewed constructor exports do
not establish the field's default before script dispatch. Therefore the
ordinary retail path is now verified as connected, but which campaign maps
enable it by default is unresolved.

## Shared port checkpoint (2026-10-20)

The shared viewer now carries the retail maximum-count formula, candidate
filters and first-match ordering, `norescuebot`, the `botrescue` state-6/hide
transition, the 4–8 second active deadline, message prompts, and the final-exit
refresh on drop-off. It remains gated on an explicit authored
`setrescuebot 1`; Training sets it to 0, and no other preserved rescue map
script sets it. The retail default enable byte is still unresolved.

The viewer initializes its local schedule after an explicit enable using the
source `00454BA0` message/attempt schedule pattern. The connection between that
initializer's `Player+0x340C` object and recurring `Player+0x32CC` object is
not proven. Host callback cadence follows the viewer frame loop, Player-state
mapping uses the host crouch/climb states, and no natural RescueBot run or
retail parity check was made. Treat those timing and state adapters as
provisional. Implementation is in [`rescue_bot.h`](../../../port/spikes/training-native-prototype/src/rescue_bot.h)
and the shared [`scene_viewer.cpp`](../../../port/spikes/training-native-prototype/src/scene_viewer.cpp).

Fresh pseudocode exports for the eight lifecycle functions are in
[`rescuebot_lifecycle_fresh_2026-10-19.c`](rescuebot_lifecycle_fresh_2026-10-19.c).
The local macOS CMake target compiled successfully, and the rescue-all app
bundle was packaged at `.local/FDNY Firefighters.app` with 13 rescue map stages
and eight campaign choices. The packaged executable SHA-256 is
`a91c67e52ccbf31881881e0ea3daca69b76664f98be3abd89c5a556be69f6ae8`, matching
the CMake target. The bundle metadata passes `plutil -lint`. These are build
and packaging checks, not RescueBot behavior checks.

## Plain-language explanation

The bot is part of ordinary Player updating, but its on/off setting is a script
field. When enabled and due, it looks for a far-away victim hidden from the
Player by the world, removes that victim from the normal render pass, and
increments a Player counter. Training explicitly turns it off. Other maps do
not set it in their preserved scripts, and the retail default is not known.
