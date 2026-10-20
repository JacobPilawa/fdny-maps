# Player jump and contact-latch trace (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Read-only Ghidra 12.0.4 headless follow-up used the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project with
`-process FDNYFirefighter.exe -readOnly -noanalysis`.

## Jump transaction and reset

Player movement calls `0044E990` at `00453784`; the Player ScriptActor
`jump` command calls it at `004518D4`. The helper waits until Player
`+0x388 + 500 ms <= clock 00502AFC`. State `+0x18C == 5` exits to state 3 and
sets vertical velocity `+0x100` to `400.0f`. Other states require byte
`+0x12C != 0` and add `400.0f` to that velocity. Either accepted branch
increments `Jumps`, requests the Player `Jump` sound, and stores the current
clock at `+0x388`.

The reset helper `0044CD10` sets state `+0x18C` to 3, zeros timer fields
`+0x384`, `+0x388`, `+0x38C`, and `+0x390`, then initializes contact bytes
`+0x12C/+0x12D` to 1. The raw helper excerpt from `0044CF10..0044CF7C` is:

```asm
0044cf10  MOV dword ptr [ESI + 0x388],EBX
0044cf16  MOV dword ptr [ESI + 0x38c],EBX
0044cf1c  MOV dword ptr [ESI + 0x384],EBX
0044cf22  MOV dword ptr [ESI + 0x390],EBX
0044cf75  MOV byte ptr [ESI + 0x12c],0x1
0044cf7c  MOV byte ptr [ESI + 0x12d],0x1
```

This reset initializes the transient latch bytes. The Player's `JUMP` action
is queried in state 4 and reaches the crouch-clearance/uncrouch helper. The
state-not-4 movement branch invokes `0044E990` without a button/action-edge
check in the traced block; the separate ScriptActor `jump` command also calls
the helper.

## Collision writer and integrator consumer

Accepted Player collision responses in the code region beginning `0043F810`
call `0049E0A0` at `0043F92E` and `0043F9F8`. That helper sets Player byte
`+0x12C` when collision-record float `+0x10` is at least the shared `0.5`
threshold, then projects velocity and displacement against the contact plane.
The separate downward-sweep snap path writes translation directly and does
not call this helper, so not every floor correction is proven to set the
latch. The collision branches and threshold are detailed in
[`dirty_space_axe_warp_trace_2026-10-14.md`](dirty_space_axe_warp_trace_2026-10-14.md).

`0049DBC0` integrates the motion fields using global timestep `00502B04`,
then copies `+0x12C` to `+0x12D` and clears `+0x12C`. Player virtual table
address point `004E1C48` has slot `+0x64` at `004E1CAC` pointing to wrapper
`0043F680`; that wrapper calls `0049DBC0` and clears Player byte `+0x130`.
No static direct `CALL 0043F680` reference was found because Player dispatches
the wrapper through its vtable. The raw movement window below identifies that
indirect call site and its order relative to jump evaluation and collision.

The raw Player movement window resolves the local order. In the state-not-4
branch, block `00452570` calls `0044E990` at `00453784`, then dispatches root
vtable slot `+0x64` at `004537CC`, and then calls collision loop `0043F6D0` at
`004537DB` when movement-subobject byte `+0x240` is clear. Accepted collision
responses can set root `+0x12C` after that pass's integrator consumed it, so
the next normal-state pass can read the contact latch before consuming it.
This establishes the intra-update handoff; the outer manager's recurring
cadence and exact relation to host frames remain unresolved. See
[`player_update_jump_contact_order_2026-10-20.asm`](player_update_jump_contact_order_2026-10-20.asm).

## Port boundary

The shared host now evaluates the normal-state jump transaction from the
previous step's collision latch before clearing it, adds the source-shaped 400
vertical impulse with the 500 ms gate, increments `Jumps`, and requests the
registered `Jump` sound. A collision in the current host step supplies the
next step's latch. The host's collision acceptance, single-step cadence, and
climb-state mapping remain adapters, so this is not a retail parity claim.
The right-mouse `JUMP` action remains confined to the crouch clearance path;
Space is retained as an extra host alias there. This trace is static
decompilation evidence only; no gameplay run or retail parity check was made.

