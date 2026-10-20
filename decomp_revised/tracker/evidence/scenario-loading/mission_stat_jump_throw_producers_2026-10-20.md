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
`004518D4`. A separate handler branch at `00451B92` calls `0044C0B0`; that
helper sets state 5 only when the current state is not 5 and Player `+0x3440`
is zero. The token for this state-5 branch is unresolved and must not be
identified as `jump`. Other writers use states 3, 4, and 7 for distinct Player
interaction states. Collision callback `0049E0A0` sets `+0x12C` for an
upward-facing contact; integrator `0049DBC0` copies it to `+0x12D` and clears
`+0x12C`. This establishes a transient contact latch, while its equivalence to
the host's grounded state or keyboard jump path remains unproven.

The Player action constructor registers `JUMP` at `004395C0` using control ID
`0x105`, which the input-name constructor registers as `MOUSE_RB` at
`00437F30`. This confirms the default action binding but not the call path from
right-click input to the update or state-5 branch: no saved poll site joins the
action to either call. No Space binding was found in that constructor.

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

The Player update copies a selected BModel vector through `0044BED0`, transforms
it through `0043B2D0`, and calls `ComputeVectorAngleDegrees` (`00497AA0`). It
compares the resulting angle with `004E169C`, verified as binary32 `10.0`.
Angles strictly below 10 degrees take the throw path; equality, larger angles,
and unordered comparisons go to a side-selection branch. The semantic labels
for the two vectors remain unresolved.

The throw path calls `0043B580`, which stores the current clock on the BModel,
sets trajectory acceleration to -384, computes duration and initial vertical
velocity, and sets BModel state `+0x25C` to 2. It then calls Player action
status 4 through `004571A0`, clears Player root `+0x3440`, restores Player
state `+0x18C` to 3, and adds exactly one to `Objects Thrown` through
`00486980` at `0045399C..004539CF`. This identifies the counted operation as a
held-BModel throw transaction rather than arbitrary contact or generic ACTION.

The status-4 callback is joined to the Player Axe subobject: `004571A0` calls
`0047DC90` on Player `+0xA94` with a false value. The Player loadout evidence
identifies this subobject as the Axe actor; the callback hides that subobject.
The higher-level name for status 4 and a matching shared-host visual state
remain unresolved. The Player constructor registers `ThrowObject` on the Axe
subobject, but the release-side sound dispatch was not found; the BModel
separately registers `ThrowObjectEnd` on its root `+0x124` sound instance.

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
source-shaped sequence already exists in
`port/spikes/training-native-prototype/src/training_liftable_sequence.h`, but
it is not connected to the shared mission scene. The Player-side held-target
writer is now source-mapped: it composes movement-subobject `+0x338c` with the
embedded frame's third column times 90 and second column times -60, then stores
the result at held BModel `+0x224` before correction. Its exact evidence and
remaining frame/cadence uncertainties are in
[`bmodel_player_target_writer_fresh_2026-10-20.c`](bmodel_player_target_writer_fresh_2026-10-20.c).
Shared frame/model adapters, the throw trajectory scheduler, and the drop/timer
lifecycle remain integration work. Static authored candidate coverage is now
accounted for across every preserved BSP; it does not establish runtime
reachability or which objects participate in rescue objectives.

## Port decision and next evidence

No shared counter was wired. `Jumps` needs the retail jump input/state/latch
route mapped to the host movement transaction; `Objects Thrown` needs the
held-target frame relation and correction cadence resolved, then the
source-shaped held-BModel transaction promoted from the Training spike into the
shared mission scene with its frame, model, trajectory, drop, and timer
services.
`Total Steps` remains coupled to the 100 ms footstep-sound variant scheduler documented in
[`mission_stat_producers_elapsed_hose_2026-10-20.md`](mission_stat_producers_elapsed_hose_2026-10-20.md).
This is static decompilation evidence only: no gameplay check, build, or retail
parity result is claimed.
