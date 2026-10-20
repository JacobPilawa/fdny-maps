# Player crouch command gate (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The Player ScriptActor handler is the saved, hash-pinned `00451870` export;
its command loop and relevant callers were checked against current-hash x86 in
`player_jump_action_state4_2026-10-20.asm` and
`player_uncrouch_clearance_2026-10-20.asm`.

## Retail state change

`00451870` dispatches the literal `CROUCH` token (`004E1C00`) to a loop over
Player command recipients. For each recipient, the handler sets crouch only if
state `+0x18C` is not already 4 and the previous accepted-contact latch at
`+0x12C` is nonzero. It writes 25.0 to Player `+0x88` and then sets state 4.
Thus the command is idempotent while crouched; it does not itself toggle back
to standing. The separate `UNCROUCH` token (`004E1BF4`) and the state-4 `JUMP`
action both call `0044BCE0`, which returns to state 3 only when the standing
clearance query is clear. The helper details and query argument sources are
recorded in
[`player_uncrouch_clearance_2026-10-20.asm`](player_uncrouch_clearance_2026-10-20.asm).

## Shared host update and limits

`Player::step` now enters crouch on the existing host crouch edge only when
`priorContactLatch` is set, and the edge does nothing while already crouched.
The state-4 JUMP path remains responsible for the standing-clearance check.
The SDL assignment of C to this host command and Space/right mouse to JUMP is
still provisional: the retained `input.rrf` does not include CROUCH or JUMP
defaults. Host collision checks also use the port's local collision geometry
instead of retail `geWorld_GetContents`; matching content/query behavior and
callback cadence remain open. No app build, tests, runtime check, or retail
parity check was performed for this change.
