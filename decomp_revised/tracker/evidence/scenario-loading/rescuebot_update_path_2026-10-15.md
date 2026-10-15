# RescueBot Player update path (2026-10-15)

Status: **the retail selector and recurring Player-update call are statically
connected; timing, initialization defaults, and victim lifecycle remain open.**
This review reuses Ghidra exports for the pinned `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. It is
not a live retail capture, and no native RescueBot behavior was added.

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

When enabled and outside Player state 6, `00455580` also checks callback-local
timer/limit fields, then compares Player `+0x3530` with the current result of
`004B9400`. If the count is below that result, it calls selector `00455230`;
otherwise it logs the maximum. The selector scans registered VictimRescue
candidates in state 4 or 7, skips the embedded exclusion byte read at victim
`+0x1FD`, requires an obstructed Player-to-victim world segment, and accepts
distance greater than or equal to 600 units. It takes the first eligible
candidate in the manager's iteration order.

## Selection and state effects

`00454750` reports the chosen victim, increments Player `+0x3530`, and calls
`0045BF70`. It stores that victim as the RescueBot's active victim and sets its
next action deadline to current game time plus 4,000 ms plus one bounded draw
in `[0,4000)`. `0045BF70` enters victim state 6, dispatches `botrescue`, writes
`UINT32_MAX` to victim `+0x1F0`, resets the actor animation, and clears the
normal-render eligibility flag through `00481140(..., false)`. Thus the target
is hidden from the normal actor pass as the bot handles it; this alone does not
show whether or how `botrescue` content, rescue statistics, or cleanup later
change.

Developer/script command `runrescuebot` is a separate entry: shared dispatcher
`0047A1F0` calls `00475020`, which directly invokes selector `00455230` on
Player `+0x340C`. That one-shot call does not pass through the recurring
callback's `Player+0x32E4` enable gate. The selector still rejects a second
active bot victim.

## Authored coverage and port boundary

The hash-pinned inventory contains no authored `runrescuebot`. Training's
Player entity 115 explicitly sets `setrescuebot 0`; the other preserved map
scripts contain no `setrescuebot` command. The reviewed constructor exports do
not establish the field's default before script dispatch. Therefore the
ordinary retail path is now verified as connected, but which campaign maps
enable it by default is unresolved.

Do not claim RescueBot parity from the existing host field alone: the viewer
stores the authored `setrescuebot` value but does not run `00455580`'s
recurring behavior. A safe port still needs the constructor/default path, the
callback-local schedule/limit initialization, candidate order and frame
cadence, `botrescue` event effects, and the victim state-6 lifecycle. Until
those are established, keep this feature documented and dormant in native
gameplay.

## Plain-language explanation

The bot is part of ordinary Player updating, but its on/off setting is a script
field. When enabled and due, it looks for a far-away victim hidden from the
Player by the world, removes that victim from the normal render pass, and
increments a Player counter. Training explicitly turns it off. Other maps do
not set it in their preserved scripts, and the retail default is not known.
