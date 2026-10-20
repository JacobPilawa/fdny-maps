# Player locomotion frame axes (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The trace is a read-only Ghidra 12.0.4 review of Player `0044F340`, its frame
vtable, wrapper `0042BD50`, setters `00496210`/`00496360`, and constructor
`00453F8A`.

## Confirmed callback writes

The locomotion frame is Player root `+0x3460`, distinct from view follower
`+0x34A4` and held-target frame `+0x24`. `0044F340` calls frame vtable slot
`+8` at `0044FA75` with vector `(0,1,0)`. The wrapper dispatches to slot `+0xC`
(`00496210`), which normalizes and installs basis column `+0x10`: the up axis
is world `+Y`. A later slot `+0x14` call at `0044FA9C` reaches `00496360` and
sets basis column `+0x1C` from a vector computed using selected-item view/aim
data. The setters repair the remaining axes by normalization and cross
products. This refresh leaves frame translation `+0x28..+0x30` and parent
`+0x40` unchanged.

The constructor calls `0042D2D0(root+0x3460, 0)`, initializing identity axes,
zero translation, unit scale, and a null parent. Entity initialization calls
`0042D350(root+0x3460, auStack_5c)` at `0044DF30`. The second argument is an
output copy: `0042D350` resets the receiver's axes and scale, preserves its
translation and parent, then calls `0042D0A0` with the stack frame as the copy
destination and `root+0x3460` as the source. The caller ignores this returned
copy and passes `root+0x3460` directly to `00483A20` to initialize the
follower. `0044CD10` does not write this frame,
so the inspected constructor and entity-init path preserve its zero
translation and null parent. The frame refresh setters also preserve
translation and parent. The normal Player callback is visible at `00453860`;
the owner `+0x50` manager reaches its root `+0x14` callback, while wall-clock
cadence and member order remain unknown. See
[`root_plus14_owner_scheduler_trace_2026-10-20.md`](root_plus14_owner_scheduler_trace_2026-10-20.md).
The earlier interpretation
of the stack area as an input frame was incorrect; the copy direction is
confirmed by current-hash x86 for `0042D350`/`0042D0A0` and saved Ghidra
decompilation. See
[`player_locomotion_frame_parent_setup_2026-10-20.asm`](player_locomotion_frame_parent_setup_2026-10-20.asm).

## Ordinary Player callback vector

The second setter's vector is resolved for the ordinary caller in the
`00452570` Player update block. `0044F340` selects the active item through the
Player inventory array `+0x3564` and selected index `+0x357C`. It reads the
selected item's view offset at `+0x1E4` and aim offset at `+0x1F0`; the ordinary
call passes view and aim fractions of 1 and zero endpoint vectors. It adds the
aim offset to `(0, height, 1000)`, subtracts the view offset, then supplies the
normalized result to setter `00496360`. The height is selected from two source
constants according to Player state 4. The raw x87/binary32 store order is in
[`view-call-dataflow.md`](../../../reference/previous-work/decomp/evidence/player-tool-pose/view-call-dataflow.md).

This resolves the setter input recipe. A follow-up trace of ordinary yaw
helper `004968B0`, basis multiply `00495EB0`, and the `00452570` input builder
now resolves the ordinary path's local movement signs and basis formula; see
[`player_locomotion_yaw_helper_2026-10-20.md`](player_locomotion_yaw_helper_2026-10-20.md).
The frame's third axis remains equipment-dependent, and state-5/debug movement
uses `+0x3460` instead of the ordinary yaw-reset frame. Its relationship to
the rendered Genesis camera and the host OpenGL basis remains open. Do not
substitute camera pitch/yaw for this equipment vector.

The equipment source inputs are present for the four modeled tools. Common
setup `0045F660` initializes item `+0x1E4` view and `+0x1F0` aim offsets to
zero; the HoseSpray, extinguisher, goggles, and Axe setup paths call that helper.
The Axe's later `0045FEC0` callback writes dynamic offsets from its animated
sine phase, and the host already models that arithmetic in
`src/item_view_pose.h`. Thus the values can be supplied without a zero-vector
placeholder. The remaining Axe input gap is the live callback activation and
frame ordering; the current offset/view oracle verifies 420 words but stubs
item Think callbacks. These facts narrow the data gap without resolving final
frame composition, world-direction mapping, or recurring cadence.

## Host status

`Player::step` still uses a camera-pitch substitute for the retail `+0x3320`
transform; the host does not yet pass a stored locomotion frame into the climb
movement path. The ordinary movement path resets the basis/scale on Player
root `+0x24` with `0042D350`, rotates it by the yaw accumulator through
`004968B0`, then applies movement input through `00495EB0`. The state-5/debug
path asks `004977B0` to materialize root `+0x3460`; that helper follows a
nonnull parent recursively, but the reviewed constructor/entity-init path
keeps this frame unparented, so the helper copies it directly. Both branches
use `00495EB0` for the final basis-column transform. The current function's
stack census finds no writes to the output copy before `0042D350`, as expected;
in the state-5 branch, `004977B0` then overwrites that same buffer with the
parent-resolved stored frame. The host's frame-column convention matches that
helper, but the final equipment-derived direction, world mapping, callback
cadence, and live Axe offset activation remain open. The stack area in the
constructor and movement calls is a helper output copy, not an unknown input
frame. The accompanying assembly excerpt records the copy direction and
current limits.
