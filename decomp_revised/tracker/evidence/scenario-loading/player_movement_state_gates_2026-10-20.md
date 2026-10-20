# Player movement state gates and motion fields (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The evidence is a read-only review of the hash-pinned Ghidra decompilation
`reference/previous-work/decomp/evidence/gameplay/00452570_FUN_00452570.c`,
the fresh `0049DBC0` export, and the raw x86 update window. The movement block
at `00452570` has no function object in the active saved Ghidra project; it is
retained as a manual code review rather than a function review.

## Retail state and movement gates

In the Player movement block, ESI is `Player+0x140`. The movement state at
`ESI+0x4C` is therefore root `+0x18C`. State 4 is crouch; its named `JUMP`
action query is at `00453708..00453779` and routes to the same clearance helper
`0044BCE0` used by ScriptActor `uncrouch`. The state-not-4 path calls
`0044E990` at `00453784`, then the root vtable `+0x64` integrator wrapper, then
collision loop `0043F6D0` when byte `ESI+0x240` is clear. This establishes
local ordering, not the outer callback cadence.

A fresh current-hash instruction check narrows the state-4 exit: velocity
half-life/blending at `004532B3..004532EF` runs before the `00453708` state
branch. In state 4, the named JUMP query at `00453739` reaches the epilogue
whether it returns false or calls `0044BCE0`; this branch bypasses
`0044E990`, the root `+0x64` integrator, and the `0043F6D0` collision pass.
State 4 therefore updates the capped/smoothed velocity but does not integrate
Player position in that pass. The exact instruction excerpt is
[`player_crouch_velocity_return_2026-10-20.asm`](player_crouch_velocity_return_2026-10-20.asm).

The source selects a pitch cap from DifficultyLevel at the mission stat store:
20, 40, or 70 degrees. State 5 or byte `ESI+0x240` raises that cap to 45
degrees when the base cap is at most the float at `004E1C1C` (45); otherwise
the higher base cap remains. This clamps `ESI+0x32F0`, which maps to Player
root `+0x3430`, the view-pitch accumulator. `ESI+0x32F4` maps to root
`+0x3434`, the view-yaw accumulator. The separate movement-vector cap is 400
normally, 260 in state 4, and 800 when byte `ESI+0x240` is nonzero.

Fresh current-hash disassembly of `0044BCE0` pins the state-4 clearance call:
it passes Player root `+0x4C` as the query origin, root `+0x78` as the minimum
vector, and a stack maximum `(root+0x84, DAT_004E181C, root+0x8C)` to
`00492DF0`. The wrapper uses `geWorld_GetContents` mode 2 and treats contents
mask `0x43` as blocked. A blocked query leaves state 4 and calls `004A11A0`
through root `+0x508`; a clear query writes `DAT_004E181C` to root `+0x88` and
restores state 3. The constant is 85.0 in the reviewed function export. This
maps the retail argument sources and state writes; the Genesis query transform
and equivalence of the host collision/content path remain open.
Evidence: [`player_uncrouch_clearance_2026-10-20.asm`](player_uncrouch_clearance_2026-10-20.asm)
and [`player_crouch_clearance_query_fresh_2026-10-20.c`](player_crouch_clearance_query_fresh_2026-10-20.c).

When state 5 or byte `ESI+0x240` is active, the movement vector is transformed
through the frame at `ESI+0x3320` using `004977B0`; gravity is set to zero.
The smoothing exponent base is 0.2 for state 5, byte `ESI+0x240`, or a
nonzero contact byte at `ESI-0x14` (root `+0x12C`); otherwise it is 0.95. The
exponent multiplier is 27. Byte `ESI+0x240` also skips the later collision
loop. State 6 follows a separate `00451080` path.

The transform at `ESI+0x3320` (Player root `+0x3460`) is initialized with
identity axes, zero translation, unit scale, and a null parent by the Player
constructor. Entity initialization calls `0042D350(root+0x3460, stackCopy)`;
the stack pointer is an output copy, not an input frame. `0042D350` resets the
receiver's axes/scale, preserves translation/parent, and calls `0042D0A0` to
copy the receiver into that stack buffer. `0044CD10` does not write the frame,
so the inspected constructor/entity-init chain leaves the parent null.
`004977B0` composes a nonnull parent before the state-5 movement transform;
with this frame's observed null parent it copies the frame directly. The
ordinary path separately resets the basis/scale on Player root `+0x24`, rotates
it by the yaw accumulator through `004968B0`, and applies the movement vector
through `00495EB0`. State 5/debug movement materializes root `+0x3460` through
`004977B0`, then applies the same basis-column multiplication in `00495EB0`.
The recurring `0044F340` callback writes world `+Y` as the up axis and derives
a third axis from the selected item's view/aim offsets; the ordinary caller's
vector is `normalize((aimOffset + (0,height,1000)) - viewOffset)`. A fresh
helper trace resolves the ordinary movement branch separately: `004968B0`
uses degree yaw and constructs columns `(cos,0,-sin)`, `(0,1,0)`, and
`(sin,0,cos)` from identity, while `00495EB0` multiplies local X/Y/Z by those
columns. The action vector is `(lateral,0,forward)`, with left `+0.75`, right
`-0.75`, forward `+1`, and backward `-1`. The state-5/debug frame remains
equipment-dependent. Its relation to the Genesis camera and host OpenGL
camera basis is unresolved, so this closes local signs only and is not world
movement parity. See
[`player_locomotion_yaw_helper_2026-10-20.md`](player_locomotion_yaw_helper_2026-10-20.md).
The smoothing call at `004532B3` passes the 0.95/0.2 base and
27 to `00482AD0`; its result is a half-life used by `00482FC0` as
`pow(0.5, frameDelta/halfLife)`. For those fixed bases this is algebraically
equivalent to the host's `pow(base, 27*dt)`. The host's ground/climbing base
selection, double-precision libm, x87 rounding, and callback cadence remain
unverified. See
[`player_movement_smoothing_formula_2026-10-20.md`](player_movement_smoothing_formula_2026-10-20.md).

`ESI+0x3310` aliases Player root `+0x3450`, the oxygen resource. The
hash-pinned `00451560` trace in
[`player_oxygen_resource_and_script_path_2026-10-14.md`](player_oxygen_resource_and_script_path_2026-10-14.md)
establishes its use: `0044CD10` initializes it to 1.0; with oxygen drain
enabled, `00451560` subtracts 0.05 at the `+0x3454` three-second deadline and
calls passout below zero, while disabled drain refills by frame delta times
0.1 up to 1.0. The neighboring `+0x3314/+0x3318` fields alias root
`+0x3454/+0x3458`, the next-drain and drain-enable deadlines. This field is
not movement speed; its timer/delta writes are the oxygen update path.

The code converts movement input to a vector, applies the selected vector cap,
smooths toward it, and calls `0044EA90` only when state is not 5, byte
`ESI+0x240` (root `+0x380`) is clear, byte `ESI+0x58` (root `+0x198`) is
nonzero, and the movement magnitude exceeds the small epsilon at `004E1C10`.
The helper uses a 100 ms gate and increments `Total Steps` after selecting an
index different from Player `+0x558`. The `FootStep1`–`FootStep4` sound list
is appended during Player initialization. `+0x558` is initialized to zero,
but a static instruction scan found no later writer; its role as a
prior-selection field is unproven. Selection uses the shared RNG through
`00498B30`/`00404AC0`, so retry count affects the global random stream. Byte
`+0x380` is initialized clear, set by the `DBG_TOGGLE_SPECT` action path, and
cleared by the `JUMP` action path.

The host now advances the source-shaped head-motion Y history after its Player
step and reads the previous sample-pattern flag before the next step. Its
`PlayerFootsteps` scheduler applies the 100 ms clock gate, four-sound
random-range/retry selector, `Total Steps` increment, and `FootStep1`–`4`
request. The source's first `+0x198` value, any computed writer to `+0x558`,
shared RNG interleaving, +0x380 debug action path, and live callback cadence
remain unresolved. The host begins with the +0x198-equivalent false and
`+0x558` rejection index zero. See
[`player_total_steps_and_headmotion_2026-10-20.asm`](player_total_steps_and_headmotion_2026-10-20.asm).

The follow-up Player frame trace confirms that `0044F340` refreshes the
locomotion frame's up axis as world `+Y`; a later indirect setter derives the
third basis column from selected-item view/aim data. Frame position and parent
are untouched during this refresh. The stack frame passed at `0044DF30` is an
output copy of the initialized `+0x3460` receiver; `0042D350` resets its axes
and unit scale while preserving its zero translation/null parent. The state-5
path materializes this stored frame before applying its movement vector. The
host's current camera-pitch substitute therefore remains provisional. Runtime
equipment vector, final axes, and outer update cadence remain open. See
[`player_locomotion_frame_axes_2026-10-20.md`](player_locomotion_frame_axes_2026-10-20.md)
and [`player_locomotion_frame_parent_setup_2026-10-20.asm`](player_locomotion_frame_parent_setup_2026-10-20.asm).

## Shared host comparison and limits

The shared scene host now raises its default DifficultyLevel-1 pitch clamp
from 40 to 45 degrees while the `climbing` state adapter is active, matching
the retail state-5 threshold branch. The separate retail `+0x380` action
remains unrepresented. The host `Player::step` in
`port/spikes/training-native-prototype/src/core.h` still uses synthetic
`ground`, `crouch`, and `climbing` flags; fixed 400/260 movement-vector caps; a
camera-pitch substitute for the `+0x3320` movement transform; and a local
collision/integration loop. Difficulty selection beyond the fresh-profile
default, byte `+0x380`, and state 6 are not represented. Do not infer retail
motion parity from the current `Player::step`.

The Player `CROUCH` ScriptActor command is now reflected in the host adapter:
it enters state 4 only when the previous accepted-contact latch is set, and
repeating it while crouched does not uncrouch. The state-4 JUMP path owns the
standing-clearance query. The host key assignment is still provisional. See
[`player_crouch_command_gate_2026-10-20.md`](player_crouch_command_gate_2026-10-20.md).

Next, resolve the equipment-varying final axes and camera/ActorInfo relation
of the locomotion frame, and how retail local movement signs map through that
frame before changing host vectors. The input key aliases and magnitudes are
mapped; callback/update cadence is not. The oxygen alias and initialized
resource default are documented above; its outer update cadence remains
provisional in the oxygen evidence.
The pitch/yaw fields
`+0x32F0/+0x32F4`, core frame/vector helpers, and smoothing exponent
relationship are mapped. Continue tracing initialization/writers for the
`+0x198` Total Steps gate, then separate source movement from host
collision/callback cadence.

## Build checkpoint

The macOS `fdny-scene-viewer` target compiled after the state-5 pitch-limit
change. The executable SHA-256 is
`0c910dfd7eac2a71921948414d02b6e50fcf6e2b51e7ea18a609e2c0da71b1f0`.
Linking warned that installed `json-c` targets macOS 27.0 while the target is
13.3. This confirms compilation only; no tests, gameplay run, or retail
comparison was performed.
