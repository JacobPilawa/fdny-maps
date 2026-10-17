# Axe view-ray sign recheck (2026-10-17)

This recheck corrects a sign interpretation in the recent Axe camera notes.
Retail EXE SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The two extracted retail ACTs are pinned in
[`equipment_action_pose_followup_2026-10-17.md`](../scenario-loading/equipment_action_pose_followup_2026-10-17.md).

## Coordinate meaning

`cameraFrame.axis[2]` is the backward camera basis. The visible ray is
`viewRay = -cameraFrame.axis[2]`, which starts at the camera and points into
the scene. Therefore:

- Positive displacement dot `viewRay` moves away from the camera and deeper
  into the scene.
- Negative displacement dot `viewRay` moves toward the camera.

The port's `actorRootFromFrame` maps ACT local `-Z` onto this visible ray.
The root-attachment-composed first swing translations are approximately
`(0.66, 10.89, -11.62)` for `axe.act` and `(-0.60, 10.89, -11.62)` for
`axel.act`. At neutral yaw their root translation projects to `+11.62` along
the view ray and `-11.62` toward the camera. The root translation thus moves
away from the camera; it does not support the prior conclusion that the swing
root moves toward the player.

That calculation isolates root translation only. Sampled root rotation and
child bone motion can move some Axe vertices toward the camera and alter the
visible silhouette. The updated `--fire-camera-audit` reports both
`axeVertexDeltaAlongViewRay` and `axeVertexDeltaTowardCamera` as min/max/mean,
plus separately signed root-translation projections. Positive values in the
`TowardCamera` fields mean actual camera-ward motion. A live capture is needed
to determine whether that mesh motion matches the user's observation.

## Related unresolved evidence

The retail ActorInfo consumer and Player view follower are traced, but their
same-frame numeric relation remains unknown; the viewer's outer origin
`(35, -8, 140)` is also provisional. These gaps affect the Axe's placement and
camera-space appearance. The source-derived local attachment, ActorInfo basis
packing, and Genesis pose composition do not justify a global or per-tool axis
flip. See
[`retail_camera_actorinfo_context_trace_2026-10-17.md`](retail_camera_actorinfo_context_trace_2026-10-17.md)
and
[`actor_camera_pose_trace_2026-10-17.md`](../genesis-boundary/actor_camera_pose_trace_2026-10-17.md).
