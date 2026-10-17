# Held-item orientation audit

## Superseding Axe resource correction (2026-10-17)

This historical audit and its 2026-10-14 follow-up incorrectly state that
ordinary Axe selection uses primary model 6 (`axel.act`). Fresh Ghidra tracing
found that the root gate and each child variant flag are separate: ordinary
first equip leaves the root gate off and uses secondary model 1 (`axe.act`);
the duplicate-equip path enables model 6 as well. The shared port now matches
that path. The camera-space ActorInfo matrix and swing direction remain open;
see [the 2026-10-17 equipment follow-up](equipment_action_pose_followup_2026-10-17.md).

## Retail evidence

The audit uses the copied `inputs/original/binaries/FDNYFirefighter.exe`,
SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The reusable headless script is
[`AuditHeldItemFlow.java`](../../../tools/ghidra-scripts/AuditHeldItemFlow.java).
It decompiles the inventory constructors, follower helper, selected-item
methods, and records xrefs where Ghidra has a function boundary.

Ghidra confirms:

- Axe constructor `00460A50` calls shared inventory constructor `0045F7E0`,
  installs its axe vtable, and initializes child equipment components.
- Extinguisher constructor `00461690` follows the same common constructor and
  installs its own vtable. Neither constructor directly calls `0045F660`.
- `0045F660` stores its Player pointer at `this+0x54`, creates a follower at
  `this+0x15C`, parents that follower to `Player+0x34A4`, sets scale
  `(0.006, 0.009, 0.006)`, and stores the follower pointer at `this+0x98`.
- Extinguisher setup routine `00461830` calls `0045F660(..., 2)`. Other
  callers include goggles setup `00462A60`, an axe-child setup `00460250`, and
  hose setup `00463700`.
- Player view builder `0044F340` reads the selected item at Player
  `+0x357C`, adds its local view offset at item `+0x1E4` to the view position,
  finalizes the Player frame at `+0x3460`, updates the follower at `+0x34A4`,
  then calls each inventory item's post-view vtable slot `+0x24`. This closes
  the selected-item-to-Player-follower frame path, but not the model actor's
  final rendering matrix. The shared follower scale and local view offsets
  must not be mistaken for a proven visible ACT scale or draw root.

This proves the follower behavior for those setup objects. It does not close
the path from the axe/extinguisher inventory objects to the ACTs that retail
draws in the player's hands, or establish their final actor rotation, offsets,
scale consumer, or depth flags. The viewer's current ACT decoder is also
provisional inspection data and cannot independently establish the correct
retail-facing axis. A follow-up Ghidra headless pass has since closed the
matrix writer/consumer path; exact per-item matrix inputs remain open below.

## Current preview status (updated 2026-10-14)

The current shared viewer draws retail-selected Axe model 6 `axel.act` and the
extinguisher with a direct camera basis. The Axe's camera-relative offset is
`(35, 27, 140)`; the extinguisher offset is `(42, -5, 145)` with preview scale
`0.25`. These values are presentation guesses. The current port render shows
the Axe's broad head and the extinguisher's labeled side, but neither
establishes which side, angle, placement, or scale retail shows.

Earlier checks tested a camera-up half-turn. The newer source-backed
follow-up found that it makes the model-6 Axe edge-on and exposes the
extinguisher's reverse. Keep the direct roots as the current preview
candidate. Do not treat either candidate as a retail correction: the final
attached ActorInfo matrix and parent frame in camera coordinates remain
unknown. The extinguisher action/contact frame remains separate and still
drives suppression; the Axe swing path remains presentation-only in the
shared viewer.

Two independent audits confirm that the fire path cannot account for camera
rotation in the current source: the 0x7531 adapter uses yaw only to classify
the direction byte or place pain audio, while the retail `0044BB00` routine
sets only Player `+0x353C..+0x353F`. To make that boundary explicit in C++,
`PreviewPlayerFireOps` now stores yaw by value instead of retaining a mutable
pointer. SDL mouse motion is still the only ordinary runtime yaw writer.

The held-actor matrix chain is now traced in the pinned retail EXE. Equipment
setup `0045F660` configures its embedded transform through `00497E90` in mode
2. Component update `00497F70` builds a hierarchical matrix and calls
`00492110`, which writes ActorInfo `+0x10..+0x48` and dispatches `00492450`;
that adapter applies the pose through `geActor_SetPose`. This corrects the
earlier claim that `00497F70` only updates flags. The exact source frame and
per-item matrix values have not yet been connected to camera coordinates.

## Verification status

Static retail evidence says the fire-contact method `0044BB00` sets a transient
directional indicator byte and does not write view or movement yaw. The shared
viewer sector math matches its four angle ranges when yaw zero faces `-Z`; the
fire path no longer changes `yaw` or `yawInput`. Its cooling call order is
consistent with retail's consume-and-clear behavior, but frame scheduling and
the optional side-copy inputs remain provisional.

The 2026-10-14 follow-up rebuilt the Training app after selecting the retail
active Axe child and adding the camera diagnostic. `Info.plist` lint,
`--help`, `--tool-orientation-check`, and `--render-check` passed. The
orientation renders loaded all 121 groups and 24/24 textures. Direct and
half-turn candidate roots differ at 20,917 model-6 Axe pixels and 41,814
extinguisher pixels. These checks validate the port render path only; none
provides an original-game pixel comparison. See
[`held_item_and_fire_camera_followup_2026-10-14.md`](held_item_and_fire_camera_followup_2026-10-14.md)
for the latest build hash and user comparison steps.

## Offscreen first-person comparison

The shared half-turn was compared against the direct camera basis using the
actual exported ACTs and the same Training spawn camera. The opt-in
`--tool-orientation-check axe|extinguisher` mode renders and saves both roots:

- Axe: 110 triangles; the roots differ at 20,917 framebuffer pixels. The
  direct root shows the broad head while the half-turn makes it edge-on.
  Retail's live parent transform is not yet captured, so this remains
  provisional.
- Extinguisher: 313 triangles; the roots differ at 41,814 pixels. The direct
  camera root faces the extinguisher's labeled panel toward the camera; the
  half-turn exposes its other side. This is port ACT readability evidence,
  not a retail orientation finding.

This is evidence about the port's decoded ACT and OpenGL transform only. It is
not a retail-game pixel comparison and does not establish the final follower,
camera frame, hand position, scale, or depth behavior. The current source uses
the direct roots described above, pending a retail comparison.
