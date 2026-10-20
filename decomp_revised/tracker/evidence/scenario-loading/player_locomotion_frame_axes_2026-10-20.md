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

The constructor initially makes this frame identity, zero-positioned, and
unparented. Entity initialization later copies a transform into it, so the
constructor alone does not establish runtime axes. The callback does not
establish the full world orientation because the indirect second-setter vector
still depends on equipment view data. The normal Player callback is visible at
`00453860`; the recurring manager and cadence remain unknown.

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

This resolves the setter input recipe, but not the input movement sign/scale
or its world-direction interpretation. The frame's third axis is equipment
dependent, and the movement block only uses `+0x3460` for its special movement
transform. Do not substitute camera pitch/yaw for this equipment vector.

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
transform; the host movement-frame orientation and callback cadence remain
provisional. This trace did not change movement transforms. Evidence logs from
the read-only review were temporary; this note records the durable
address-level result.
