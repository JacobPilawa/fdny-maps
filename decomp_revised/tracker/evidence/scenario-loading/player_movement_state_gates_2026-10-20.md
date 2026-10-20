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

The source selects a pitch cap from DifficultyLevel at the mission stat store:
20, 40, or 70 degrees. State 5 or byte `ESI+0x240` raises that cap to 45
degrees when the base cap is at most the float at `004E1C1C` (45); otherwise
the higher base cap remains. This clamps `ESI+0x32F0`, which maps to Player
root `+0x3430`, the view-pitch accumulator. `ESI+0x32F4` maps to root
`+0x3434`, the view-yaw accumulator. The separate movement-vector cap is 400
normally, 260 in state 4, and 800 when byte `ESI+0x240` is nonzero.

When state 5 or byte `ESI+0x240` is active, the movement vector is transformed
through the frame at `ESI+0x3320` using `004977B0`; gravity is set to zero.
The smoothing exponent base is 0.2 for state 5, byte `ESI+0x240`, or a
nonzero contact byte at `ESI-0x14` (root `+0x12C`); otherwise it is 0.95. The
exponent multiplier is 27. Byte `ESI+0x240` also skips the later collision
loop. State 6 follows a separate `00451080` path.

The transform at `ESI+0x3320` (Player root `+0x3460`) is initialized as an
identity frame with a null parent by the Player constructor; `004977B0`
composes parent frames before the state-5 movement vector transform. The
movement update builds a temporary basis from the pitch/yaw accumulators using
`0042D350`/`004968B0`, then applies it through `00495EB0`. Runtime writes to
the `+0x3460` frame, its final world axes, and the input scale/sign convention
are not mapped. The smoothing call to `00482AD0` receives the 0.95/0.2 base and
27, but its exact exponent formula and boundary handling remain unverified;
the host's `pow(base, 27*dt)` is therefore still an approximation.

`ESI+0x3310` aliases Player root `+0x3450`. `00451560` updates this field on
timer branches, including frame-delta-scaled accumulation and clamp/reset at
0/1; it also changes neighboring animation/condition fields. Its semantic
name and constructor default have not been established, so it should not be
treated as movement speed.

The code converts movement input to a vector, applies the selected vector cap,
smooths toward it, calls `0044EA90` only when state is not 5, byte
`ESI+0x240` is clear, byte `ESI+0x58` (root `+0x198`) is nonzero, and the
movement magnitude exceeds the small epsilon at `004E1C10`. The helper uses a
100 ms gate and increments `Total Steps` after selecting an index different
from Player `+0x558`. The `FootStep1`–`FootStep4` sound list is appended during
Player initialization. `+0x558` is initialized to zero, but a static
instruction scan found no later writer; its role as a prior-selection field is
unproven. Selection uses the shared RNG through `00498B30`/`00404AC0`, so
retry count affects the global random stream. Meanings of the `+0x198` and
`+0x380` gate bytes and live callback cadence remain unresolved. See the
accompanying Total Steps callsite audit in
`mission_stat_producers_elapsed_hose_2026-10-20.md`.

## Shared host comparison and limits

The shared scene host now raises its default DifficultyLevel-1 pitch clamp
from 40 to 45 degrees while the `climbing` state adapter is active, matching
the retail state-5 threshold branch. The separate retail `+0x380` flag remains
unrepresented. The host `Player::step` in
`port/spikes/training-native-prototype/src/core.h` still uses synthetic
`ground`, `crouch`, and `climbing` flags; fixed 400/260 movement-vector caps; a
camera-pitch substitute for the `+0x3320` movement transform; and a local
collision/integration loop. Difficulty selection beyond the fresh-profile
default, byte `+0x380`, and state 6 are not represented. Do not infer retail
motion parity from the current `Player::step`.

Next, resolve the meaning/default of `+0x3310`, runtime updates and world axes
of the `+0x3320` frame, and the exact weight formula in `00482AD0`; map the
source movement input scale before changing host movement vectors. The
pitch/yaw fields `+0x32F0/+0x32F4` and core frame/vector helpers are mapped.
Also resolve initial values and writers for movement bytes `+0x198` and
`+0x380`, then separate source movement from host collision/callback cadence.

## Build checkpoint

The macOS `fdny-scene-viewer` target compiled after the state-5 pitch-limit
change. The executable SHA-256 is
`0c910dfd7eac2a71921948414d02b6e50fcf6e2b51e7ea18a609e2c0da71b1f0`.
Linking warned that installed `json-c` targets macOS 27.0 while the target is
13.3. This confirms compilation only; no tests, gameplay run, or retail
comparison was performed.
