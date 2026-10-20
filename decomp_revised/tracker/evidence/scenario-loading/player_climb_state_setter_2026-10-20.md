# Player climb state setter (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Read-only Ghidra 12.0.4 decompilation and x86 listing came from the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project with `-noanalysis`.

## Setter transaction

The Player ScriptActor `climb` token reaches `0044C0B0` from `00451B92`; its
token and caller are preserved in
[`player_climb_state_command_2026-10-20.asm`](player_climb_state_command_2026-10-20.asm).
Fresh decompilation of `0044C0B0`:

```c
if (Player->state != 5 && Player->heldBModel == NULL) {
    Player->state = 5;
    FUN_0042D280(Player + 0xFC, localVector, 0, 0, 0);
    return 1;
}
return 0;
```

The guard is exact: an already-state-5 Player or a non-null held-object
pointer at root `+0x3440` leaves the state unchanged. On acceptance the helper
stores zero to the three-word field at `+0xFC..+0x104` and copies that vector to
the local output. This is the same tuple whose Y component at `+0x100` is the
jump velocity written by `0044E990`; the host's `Player::v = {}` represents
this reset. No additional state or velocity write appears in the setter
body. The helper's local output is not used by `0044C0B0`.

The adjacent `unclimb` handler at `00451BDA` restores state 3 only if the
current state is 5. It does not call the setter or clear the held pointer.

## Shared host change

`Player` now carries a `holdingBModel` state bit derived from the shared
held-BModel pointer. The `climb` command enters the host climb adapter and
clears velocity only when the Player is not already climbing and no BModel is
held. The host clears the bit at its throw-release transition. The `unclimb`
handler already maps to the state-5-only host Boolean transition.

This closes the source-backed entry gate and vector reset. Climb movement,
Player state interactions while crouched/incapacitated, and callback cadence
remain host adapters; this change does not establish retail parity.
