# Player locomotion yaw helper and local input mapping (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Functions `004968B0`, `00496360`, and `00495EB0` were freshly exported from a
headless Ghidra 12.0.4 analysis of that file. The unaligned caller block
`00452570` was checked with current-hash x86; its excerpts are recorded as a
manual instruction slice, not a Ghidra function. The exports and raw
instructions are in
[`player_locomotion_helpers_fresh_2026-10-20.c`](player_locomotion_helpers_fresh_2026-10-20.c)
and
[`player_locomotion_helpers_2026-10-20.asm`](player_locomotion_helpers_2026-10-20.asm).

## Ordinary movement basis

`00452570` resets the temporary basis at Player root `+0x24` with `0042D350`,
then calls `004968B0` with the yaw accumulator at root `+0x32F4`. Raw data at
`004E22BC` is binary32 `0.01745329238474369`, so this helper consumes degrees.
The helper's second rotation constant at `004DC50C` is zero. With the
identity basis installed by `0042D350`, the x87 sine/cosine calculations and
cross-product repair produce:

```text
column 0 = ( cos(yaw), 0, -sin(yaw) )
column 1 = ( 0,        1,  0        )
column 2 = ( sin(yaw), 0,  cos(yaw) )
```

`004968B0` does not update frame translation, scale, or parent. Its cross
product and normalization calls rebuild the third column and repair the
first; exact x87 intermediate rounding remains outside this algebraic
summary. The source's local transform sign is resolved for this ordinary
branch.

`00495EB0` calculates the result from the frame's three columns and scale:

```text
out = column0 * (scale.x * input.x)
    + column1 * (scale.y * input.y)
    + column2 * (scale.z * input.z)
```

The movement block initializes `(x,y,z)` to zero, adds `MOVE_LEFT`'s `0.75`
to X and subtracts `MOVE_RIGHT`'s `0.75` from X. It adds `MOVE_FORWARD`'s
`1.0` to Z and subtracts `MOVE_BACKWARD`'s `1.0` from Z. Y stays zero in
this input builder. After the vector cap, the ordinary branch therefore
applies the local vector `(lateral,0,forward)` through the yaw basis above.

State 5 and the separate `+0x380` debug latch instead materialize the stored
root `+0x3460` frame through `004977B0`, then use the same `00495EB0` multiply.
That frame's constructor and ordinary refresh are traced in
[`player_locomotion_frame_axes_2026-10-20.md`](player_locomotion_frame_axes_2026-10-20.md).

## Port status and limits

The host's normal movement currently computes a camera-relative vector in
`Player::step`; state-5 movement still substitutes camera pitch for the
stored `+0x3460` frame. The source's ordinary local yaw transform is now
known, but the host's OpenGL camera basis and the retail view/follower/
ActorInfo-to-Genesis camera composition do not yet have a numeric join. The
item-dependent stored-frame update also occurs later in the Player pass than
movement, and its recurring callback cadence is not established. No lateral
or forward sign change was made from local algebra alone. Map the renderer
coordinate conversion and the prior-update item frame before applying a host
world-space correction. No build, tests, runtime, or retail parity check was
performed for this static review.
