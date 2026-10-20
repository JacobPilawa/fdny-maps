# Jumps and Objects Thrown producer follow-up (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Read-only Ghidra 12.0.4 follow-up used the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project with
`-process FDNYFirefighter.exe -readOnly -noanalysis`. The Player offset scan
covered `+0x18C` and `+0x12C`; producer and helper bodies were checked against
x86 instructions from the same executable.

## Jumps

`0044E990` is called by the Player update at `00453784` and by the script
handler at `004518D4`. The update compares Player `+0x388 + 500 ms` against
clock `00502AFC`. If the interval has elapsed:

- Player `+0x18C == 5` is changed to state 3 and vertical velocity `+0x100` is
  assigned the binary32 value 400 (`0044E9C2..0044E9D5`).
- Otherwise, byte `+0x12C` must be nonzero; then 400 is added to `+0x100`
  (`0044E9E1..0044E9FB`).
- Both successful branches add one to `Jumps` through `00486980`, request the
  Player's `Jump` sound, and store the current clock to `+0x388`
  (`0044EA01..0044EA76`).

The fields are not a generic jump counter and a grounded Boolean. Reset helper
`0044CD10` initializes `+0x18C` to 3 and `+0x12C/+0x12D` to 1. The parsed
`jump` ScriptActor token in handler `00451870` calls `0044E990` directly at
`004518D4`. A separate handler branch compares the token with `climb` at
`00451B60`, then calls `0044C0B0` at `00451B92`; the helper sets state 5 only
when the current state is not 5 and Player `+0x3440` is zero. The adjacent
`unclimb` token at `004E1BDC` restores Player state 3 only from state 5. Thus
state 5 is part of the climb command path, not a second `jump` binding. The
raw comparison and state-reset instructions are preserved in
[`player_climb_state_command_2026-10-20.asm`](player_climb_state_command_2026-10-20.asm).
Other writers use states 3, 4, and 7 for distinct Player interaction states.
Collision callback `0049E0A0` sets `+0x12C` for an
upward-facing contact; integrator `0049DBC0` copies it to `+0x12D` and clears
`+0x12C`. This establishes a transient contact latch, while its equivalence to
the host's grounded state or keyboard jump path remains unproven.

The Player action constructor registers `JUMP` at `004395C0` using control ID
`0x105`, which the input-name constructor registers as `MOUSE_RB` at
`00437F30`. The movement update has a separate `JUMP` action query at
`0045370E..00453739` through `00438550`. It is reached only when Player root
state `+0x18C` is 4; a true result calls `0044BCE0`, whose geometry query either
plays the sound at root `+0x508` and leaves state 4, or writes root `+0x88` and
returns the state to 3. This branch does not call the `0044E990` velocity/stat
routine. When state is not 4, the update calls `0044E990` separately at
`00453784`; that routine uses its 500 ms timer and `+0x12C` latch, independent
of the visible `JUMP` query. The camera/input pass at `0042DB06` also queries
MOUSE_RB but discards that call's result. The exact state-4 label, the geometry
query's full meaning, outer update cadence, and mapping to the host's general
jump controls remain unresolved. See
[`player_jump_action_state4_2026-10-20.asm`](player_jump_action_state4_2026-10-20.asm).
No Space binding was found in the retail Player action constructor.

The shared host accepts jumps from grounded or climbing states, applies the
same 400-unit vertical impulse, and has a 500 ms cooldown. It does not model the
retail state-5 command gate, the consumed `+0x12C` callback latch, or the
source's combined impulse/stat/sound transaction. Do not count every host jump
as a retail-equivalent `Jumps` value until the input route and these state gates
are joined.

## Objects Thrown

The literal key at `004DF490` has two current-program code references: stat
registration at `00424AEC` and producer key construction at `00453993`. The
producer is inside the Player update body beginning at `00452570`; Ghidra has
no function boundary for the block. This update receives the Player movement
subobject in `ESI`, which is root `+0x140`.

At `0045387B`, the update requires `[ESI+0x3300]` to be non-null. With the
movement-subobject base above, this is Player root `+0x3440`, the held BModel
pointer. The mode-1 BModel use transition at `0044BC40` assigns that field to
the used BModel root; reset and initialization clear it. The hash-pinned
`G_Liftable`/MOTN/PATH scan covers all 17 preserved BSPs and finds 18 authored
liftable BModels. Seventeen pass the static `0043BE00` first-child path checks;
Boat A entity 192 (`chair1`) has no child model. Bank entities 432/482 and Boat
A 269/270 contain the `throwbox`-named children; other static candidates
include Bar R woodboxes, Boat A's `pushbox`, MBase throwboxes, Oil Rig B
barrels, Oil Rig C rubble, and Training metalboxes. These are candidates for
the held-object branch, but authored data does not prove which instances are
selected or reach it. Full details and source hashes are in
[`mission_stat_bmodel_liftable_pose_2026-10-20.md`](mission_stat_bmodel_liftable_pose_2026-10-20.md).

Fresh read-only Ghidra instruction windows at `0045387B`, `00453A02`, and
`00453A38` are recorded in
[`00452570_held_bmodel_throw_gate_fresh_2026-10-20.asm`](00452570_held_bmodel_throw_gate_fresh_2026-10-20.asm).
Fresh read-only decompilations of the called vector helpers are in
[`bmodel_throw_gate_vectors_fresh_2026-10-20.c`](bmodel_throw_gate_vectors_fresh_2026-10-20.c).
`0044BED0` selects BModel endpoint `+0x1FC..+0x204` when selector byte `+0x220`
is zero, or endpoint `+0x1F0..+0x1F8` when nonzero. `0043B2D0` subtracts the
Player movement vector at `+0x338C` from that selected endpoint. `0049E770`
copies movement-subobject `+0x3364`, which is Player root `+0x34A4`, the Player
view follower's vector at `+0x1C..+0x24`. The held-target writer uses a distinct
Player-root `+0x24` frame returned by the vtable slot at root `+0x18`; it does
not identify the throw-gate vector. The Player update calls `0044F340` at
`00453860` before its held-pointer check at `0045387B`; `0044F340` refreshes the
view follower with `00483720`. This ties the gate to the follower refreshed by
that Player update pass, but not to the visible Genesis camera transform or an
outer Player callback schedule. The raw instruction window is recorded in
[`player_throw_view_follower_refresh_2026-10-20.asm`](player_throw_view_follower_refresh_2026-10-20.asm).
`00497AA0` takes the normalized dot-product angle between the follower vector
and the endpoint displacement. It compares the
result with `004E169C`, verified as binary32 `10.0`. Angles strictly below 10
degrees take the throw path; equality, larger angles, and unordered comparisons
go to a side-selection branch. There is no intervening button or action-edge
check, so the angle gate runs on every Player update that reaches this
held-pointer branch. The host still substitutes its camera basis; the follower
to camera mapping and Player callback cadence remain unresolved.

The throw path calls `0043B580`, which stores the current clock on the BModel,
sets trajectory acceleration to -384, computes duration from endpoint distance
with the binary32 `1/960` scale, computes initial vertical velocity, and sets
BModel state `+0x25C` to 2. The current-hash setup decompilation is in
[`0043b580_bmodel_throw_setup_fresh_2026-10-20.c`](0043b580_bmodel_throw_setup_fresh_2026-10-20.c).
It then calls Player action
status 4 through `004571A0`, clears Player root `+0x3440`, restores Player
state `+0x18C` to 3, and adds exactly one to `Objects Thrown` through
`00486980` at `0045399C..004539CF`. This identifies the counted operation as a
held-BModel throw transaction rather than arbitrary contact or generic ACTION.

The Player action callback is joined to the Axe subobject. Status 3 calls
`00455BC0`, which enables the ActorInfo and resets its motion state. Status 4
calls `0047DC90` on Player `+0xA94` with false; this disables the ActorInfo and
its child. The Player loadout evidence identifies this subobject as the Axe
actor. The shared renderer now mirrors these two states. Existing-Axe equip
sets root `+0x1FC`; normal slot activation calls the Axe `+0x3C` callback and
then Player status 5 for the inventory HUD. It does not call status 3 or touch
Player `+0xA94`, so the retail link that makes this separate actor visible
again after status 4 remains unresolved. See
[`axe_reactivation_followup_2026-10-20.md`](axe_reactivation_followup_2026-10-20.md).
The Player constructor registers
`ThrowObject` on the Axe subobject, but the release-side sound dispatch was not
found; the BModel separately registers `ThrowObjectEnd` on its root `+0x124`
sound instance. The fresh status-helper excerpts are in
[`player_bmodel_action_status_fresh_2026-10-20.c`](player_bmodel_action_status_fresh_2026-10-20.c).

`0043B580` receives the held BModel root and starts its trajectory. BModel
update `0043D330` converts the wrapped clock delta to seconds and advances
state 2 from the stored duration, endpoints, and vertical-trajectory fields.
It interpolates X/Z linearly and Y with the stored acceleration and initial
velocity, then writes the model transform. When elapsed time exceeds duration
it resets state to 0, applies the selected endpoint, flips the endpoint byte,
dispatches the ScriptActor `drop` event, and calls `004A11A0` on BModel root
`+0x124`. The BModel constructor initializes that field as a sound instance via
`004A13A0`; `004A11A0` requests playback. This is a sound request, not timed-list
cleanup. The exact timer or timed-list cleanup associated with the event remains
unresolved. Player and
BModel constructors register `ThrowObject` and `ThrowObjectEnd` sounds,
consistent with the sequence but not needed to identify the stat gate. The
source-shaped sequence also exists in
`port/spikes/training-native-prototype/src/training_liftable_sequence.h` as a
reference fixture. The shared mission host now connects the common scalar
setup/trajectory to carry selection, the held-target gate, the `Objects
Thrown` statistic, `drop` dispatch, and `ThrowObjectEnd` sound request. Its
Player frame and callback cadence adapters remain provisional. The direct call to
`0043D330` comes from `0043DC60`'s slot-zero BModel ScriptActor update after its
model-count guard, when root `+0x258` is nonzero. The callback also processes
timed-command records later in the same update, but there is no direct
trajectory-to-timed-list cleanup call. The recurring external route into root
slot `+0x14`, callback cadence, and ordering against contact/rendering remain
unknown. A current-hash scan found no literal `drop` or `removealltimecmds`
binding in the preserved EVS sources or any `S_Script*` value from the 17 retail
map exports; see [the bounded route/data audit](bmodel_throw_update_route_2026-10-20.md).
The Player-side held-target
writer is now source-mapped: it composes movement-subobject `+0x338c` with the
embedded frame's third column times 90 and second column times -60, then stores
the result at held BModel `+0x224` before correction. Its exact evidence and
remaining frame/cadence uncertainties are in
[`bmodel_player_target_writer_fresh_2026-10-20.c`](bmodel_player_target_writer_fresh_2026-10-20.c).
The shared equipment renderer mirrors status 3 enabling/resetting and status 4
disabling the Axe subobject. Existing-Axe equip sets item root `+0x1FC`, and
ordinary slot activation calls the Axe `+0x3C` callback followed by Player
status 5 for the inventory HUD. It does not call status 3 or touch Player
`+0xA94`, so the retail link that makes this separate actor visible again after
status 4 remains unresolved. See
[`axe_reactivation_followup_2026-10-20.md`](axe_reactivation_followup_2026-10-20.md).
Player frame/callback cadence adapters and data-dependent drop/timer effects
remain port work. Static authored candidate coverage is now
accounted for across every preserved BSP; it does not establish runtime
reachability or which objects participate in rescue objectives.

## Port decision and next evidence

`Objects Thrown` now increments at the shared throw transaction, and the
source-shaped trajectory, completion event, and end-sound request are wired to
the shared BModel scene update. The source block evaluates its angle gate on
each held Player update without a separate input edge. The host substitutes
its camera basis for the throw gate's Player view follower at root `+0x34A4`
and for the separate held-target frame at root `+0x24`; it samples the callback
once per host frame. Both adapters and the host cadence remain provisional.
`Jumps` still needs the retail input, state, and contact-latch route mapped to
the host movement transaction.
The local retail Player route is root slot `+0x14` → wrapper `0044FE20` →
`00470740(Player+0x140)` → movement slot zero → `00452570`; the recurring outer
manager and cadence remain unknown. See
[`player_update_dispatch_2026-10-20.asm`](player_update_dispatch_2026-10-20.asm).
`Total Steps` remains coupled to the 100 ms footstep-sound variant scheduler documented in
[`mission_stat_producers_elapsed_hose_2026-10-20.md`](mission_stat_producers_elapsed_hose_2026-10-20.md).
This is static decompilation evidence only: no gameplay check, build, or retail
parity result is claimed.
