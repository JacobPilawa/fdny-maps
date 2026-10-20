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
