# Retail equipment view-frame trace (2026-10-17)

The retail executable is
`inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra 12.0.4 headless analysis used a temporary project under
`/private/tmp/fdny_exe_pose_20261017`; the saved user Ghidra project was not
opened or modified. The pseudocode is preserved in
[`equipment_view_frame_trace_2026-10-17.c`](equipment_view_frame_trace_2026-10-17.c).

## Findings

- `00496360` takes a vector from its caller, normalizes it, and stores it in
  frame basis bytes `+0x1c..+0x24`. It repairs the other axes with cross
  products. `00496B50` rotates the frame around its up direction by the supplied
  angle, and `004964B0` orthogonalizes the basis. None of these functions adds
  an explicit sign inversion to the supplied vector.
- `0044F340` builds the Player view frame, applies the selected item's view and
  aim offsets, runs those frame helpers, updates the view follower, and then
  calls each inventory item's post-view callback. `0045F660` gives held-tool
  components an identity local transform parented to that view follower; it
  does not supply a fixed Axe hand rotation or translation.
- The held-tool parent is Player view follower `+0x34A4`. `0044F340` first
  computes the view frame at `+0x3460`, then calls `00483720` to refresh the
  follower; `00483720` resolves its parent frame and copies the three basis
  columns into follower offsets `+0x04`, `+0x10`, and `+0x1C`. The view-frame
  direction is formed toward a target point 1000 units along the view, and
  `00496360` stores its supplied vector without an explicit sign inversion.
  This identifies the held-tool parent as a view frame rather than the Player
  body basis. It still does not join the follower numerically to Genesis's
  camera world transform.
- The Axe has two child variants. `00460A50` initializes the root gate at
  Axe `+0x1FC` to zero, creates the primary child at `+0x200` with its child
  variant byte set, and creates the secondary child at `+0x648` with that byte
  clear. `00460250` maps those child flags to model 6 (`axel.act`) and model 1
  (`axe.act`), respectively. Ordinary first equip leaves the root gate off,
  so the secondary model 1 is the active Axe; the duplicate-equip branch at
  `0044D152` enables the primary child too. The two `+0x1FC` fields are
  different bytes and must not be conflated. Both ACTs contain a `swing`
  motion with positive early local-Z translation; the action callback samples
  the child ActorInfo clock but does not name a motion explicitly.
- The primary `axel.act` root bone `BONE06` has a non-identity attachment.
  The port's ACT evaluator follows the Genesis order documented in
  [`actor_camera_pose_trace_2026-10-17.c`](../genesis-boundary/actor_camera_pose_trace_2026-10-17.c):
  caller transform times the motion root transform, with sampled bone channels
  applied under that pose. A deeper Genesis trace confirms BONE06 takes the
  attachment-bearing flag-0 branch; the port's `attachment × sample` rotation
  and attachment-transformed sample translation match it. See
  [`actor_bone_attachment_branch_trace_2026-10-17.c`](../genesis-boundary/actor_bone_attachment_branch_trace_2026-10-17.c).
- Shipped Genesis camera projection treats negative camera-space Z as in front:
  both `geCamera_Project` and `geCamera_TransformAndProject` compute depth from
  `-cameraZ`. The port's OpenGL view also looks along local -Z. This alone does
  not establish that the retail Player follower's third basis axis needs a
  flip. The retail camera setup caller and the numeric matrix passed from the
  Player follower through `00497F70` / `00492110` into `geActor_SetPose` have not
  been joined, and no live retail ActorInfo frame was captured.

## Impact and next step

The viewer still renders the Axe with `rawToolRootFromCameraFrame` and a
camera-relative `(35, -8, 140)` origin. Both are provisional. Retail
component-frame-to-ActorInfo basis packing matches the port's conversion, but
the camera world-transform caller and a numeric relation between Genesis's
camera and Player view follower `+0x34A4` are not recovered. Genesis's runtime
parent-times-sampled quaternion order and BONE06 attachment integration are
traced, but the camera-to-follower relation remains open. A global camera
basis change would affect every map view, extinguisher contact orientation,
goggles placement, and all held ACTs. A per-tool correction would change the
tool's static facing and animated displacement. Neither is justified until
the camera/frame relation and final Axe ActorInfo pose are resolved. The
swing-direction report therefore remains open; this trace is diagnostic
progress, not a fix or retail-parity check.
