# Player normal-state jump input bridge (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
This review joins the saved `0044E990` decompilation, current-hash Player
movement disassembly, and the shared host's `Player::step` input wiring.

## Current mapping

`0044E990` contains a 500 ms clock gate and the state-5/contact-latch impulse
transaction; its saved body has no action query. The Player movement block
calls it in the state-not-4 path before the integrator and collision pass.
The movement block's named `JUMP` poll is state-4-only and routes to
`0044BCE0` for crouch clearance. Separately, `004395C0` registers `JUMP` with
control ID `0x105` (`MOUSE_RB`), and ScriptActor token `jump` directly calls
`0044E990`.

The shared `Player::step` currently uses `jumpEdge` only to request state-4
uncrouch. In ordinary movement it applies the 500 ms/latch impulse without
consulting `jump` or `jumpEdge`. This follows the mapped helper transaction,
but the retail path from the `JUMP` action to the normal-state `jump`
ScriptActor event has not been established. Do not change the physics gate or
claim normal-state input parity until that dispatcher route is traced. The
retained `input.rrf` does not supply a `JUMP` default binding. No host code,
build, test, runtime, or parity change was made for this review.

## Input-map follow-up

Fresh read-only Ghidra output from the same saved project confirms the action
layer shape. The Player constructor first stores the named `JUMP` action through
`00438D70` at `00439F52`; a later constructor block passes `JUMP` and control
ID `0x105` through `00437410` to `00438BF0` (`0043AD57..0043AD80`). The
decompilation exports are retained as
[`00438d70_input_action_registration_fresh_2026-10-20.c`](00438d70_input_action_registration_fresh_2026-10-20.c),
[`00437410_input_binding_add_wrapper_fresh_2026-10-20.c`](00437410_input_binding_add_wrapper_fresh_2026-10-20.c),
and
[`00438bf0_input_binding_insert_fresh_2026-10-20.c`](00438bf0_input_binding_insert_fresh_2026-10-20.c).

`00438550` resolves the named action entry, iterates its bound input objects,
and calls their virtual predicate at vtable offset `+4`. The first nonzero
predicate returns an active low byte; no match leaves that byte clear. Its
body does not invoke the ScriptActor router. The call in Player movement at
`00453739` consumes this query only on the state-4 crouch-clearance path. See
[`00438550_input_action_query_fresh_2026-10-20.c`](00438550_input_action_query_fresh_2026-10-20.c).

This narrows the unresolved edge to any producer outside the named-action
query and its constructor. The separate Player `jump` ScriptActor token still
reaches `0044E990`, but no static route from the `JUMP` predicate to that token
has been found. The normal-state impulse remains ungated in the host pending a
source-backed bridge or a source-backed confirmation that no such bridge is
used.
