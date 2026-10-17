# Genesis actor pose and camera convention trace (2026-10-17)

Binary: `inputs/original/binaries/Genesis.dll`, SHA-256
`5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
Ghidra 12.0.4 headless analysis ran in a temporary project under
`/private/tmp/fdny_genesis_pose_20261017`; the saved user project was not
opened or modified. The pseudocode excerpts are preserved in
[`actor_camera_pose_trace_2026-10-17.c`](actor_camera_pose_trace_2026-10-17.c)
and the attachment-branch excerpt in
[`actor_bone_attachment_branch_trace_2026-10-17.c`](actor_bone_attachment_branch_trace_2026-10-17.c).

## Findings

- `geCamera_SetWorldSpaceXForm` stores the supplied camera world transform,
  transposes its basis for the camera-space transform, and copies its position.
  `geCamera_Project` and `geCamera_TransformAndProject` use `-cameraZ` as
  projection depth. Genesis therefore treats camera-space -Z as the visible
  forward half-space.
- Exported `geActor_SetPose` delegates to `FUN_10010650`. That implementation
  obtains the motion-root transform with `geMotion_GetTransform`; when the
  caller supplies an outer transform, it composes the caller transform with
  that motion-root transform. It then samples each bone's named motion
  channels and scales sampled translation channels by the actor scale fields.
- `geXForm3d_Multiply` and `geXForm3d_Transform` confirm the engine's 3x4
  basis-plus-translation composition and point-transform arithmetic. This
  matches the composition order used by the port's `ActorPose` evaluator at
  the engine boundary.

## Bone-pose evaluator follow-up (2026-10-17)

The same hash-pinned Genesis binary was traced beyond `geActor_SetPose` into
the dirty-bone update path. `FUN_1000FFB0` (`1000FFB0`) selects either the
actor root or the indexed parent bone and calls `FUN_1000FD40` (`1000FD40`)
for dirty records. In its animated branch, `1000FD40` multiplies the parent
quaternion by the sampled quaternion stored at child `+0x70`, then writes the
child's orientation matrix. It adds sampled translation at child `+0x64` to
the base offset at `+0x58`, transforms that point through the parent frame,
and `1000FFB0` later applies the actor-root matrix to the resulting bones.
`FUN_10010490` is the indexed bone-transform accessor.

`FUN_1000FB10` initializes each runtime bone from the ACT attachment. It stores
the attachment quaternion at runtime `+0x24`, copies the attachment 3x4 matrix
to `+0x34`, preserves its base translation at `+0x58..+0x60`, zeros sampled
translation/quaternion fields, and calls `FUN_1000FA60`.

`FUN_1000FA60` sets runtime flag `+0x84` only when all three attachment
quaternion vector components at `+0x28,+0x2c,+0x30` are strictly between the
double constants at `0x10051D48` and `0x10051D50`. Reading those pinned image
bytes as little-endian doubles gives `-0.0001` and `+0.0001`. The two update
branches in `FUN_1000FD40` therefore mean:

- Flag 1: compose parent quaternion × sampled quaternion, then transform
  base-plus-sampled translation through the parent. This is the near-identity
  attachment case.
- Flag 0: compose parent × attachment × sampled quaternion, transform sampled
  translation through the stored attachment matrix, add its base translation,
  then transform through the parent.

For `axel.act` (SHA-256
`cfdc33f89ee4840dbbde05118b9e096c43538be214c0fb1ddc043f664de193c1`), root
bone `BONE06` attachment converts to quaternion approximately
`(w,x,y,z)=(0.8280,-0.5255,0.1626,-0.1086)`. Its flag is 0, so the port's
`attachment * sampledKey`, followed by `parent * local` in `actor_pose.h`,
matches the selected Genesis rotation and translation path. The earlier
attachment-integration uncertainty does not explain the Axe swing direction.

The remaining minimum evidence is the final retail ActorInfo root matrix and
Genesis camera/view basis at the same known swing time. The retail caller that
sets the camera world transform and its numeric relation to Player view
follower `+0x34A4` remain open. Do not change the Axe axes based on this static
trace alone.

These functions establish Genesis's camera and actor-pose conventions, but
they do not show the exact numbers written into the retail held-item
ActorInfo matrix during an Axe swing. The EXE camera world-transform caller
and a numeric relation between the Player view follower and Genesis camera
remain open. Do not apply a global sign flip based on this trace alone.
