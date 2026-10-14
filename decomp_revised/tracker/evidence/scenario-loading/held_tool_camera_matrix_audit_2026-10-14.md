# Held-tool transform and fire-camera follow-up

Reviewed 2026-10-14 against the copied retail EXE
`inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra 12.0.4 ran in a new project under `.local/`; the saved user Ghidra
project was not opened or changed. Fresh pseudocode for six Ghidra-defined
helpers is in [`held-tool-transform-ghidra/`](held-tool-transform-ghidra/).

## Held tools

The current viewer draws the selected Axe model 6 (`axel.act`) and the
extinguisher from camera roots with guessed hand offsets; the extinguisher's
visible scale is also guessed. The retail matrix path is more than an ACT-axis
choice:

- `0045F660` attaches the equipment follower under `Player+0x34A4`, initializes
  scale `(0.006, 0.009, 0.006)`, and zeroes item view/aim vectors at `+0x1E4`
  and `+0x1F0`. This scale belongs to that follower; it does not establish the
  final visible ACT scale.
- `00497F70` recursively resolves the component frame, then assembles a full
  ActorInfo matrix from three frame basis columns and position/scale values.
- `00497300` composes child transforms through their parent frames;
  `00495D20` applies a scaled point through one frame;
  `00496020` repeats that point transform through the parent chain.
  `00446EE0`, `00447830`, and `0049E770` copy three distinct basis columns.
- `00492110` stores the matrix in ActorInfo and `00492450` applies it to the
  Genesis actor. The view frame and equipment-local frame values that feed
  this path are not captured in the current viewer or in the saved fixtures.

This confirms the preview bypasses a live, hierarchical retail transform, but
does not identify a correct 180-degree turn, other axis change, exact hand
position, or visible scale. I kept the rendered roots unchanged. The next
source-backed step is to recover the equipment-local component writer and
associate its runtime parent/local values with the ACT ActorInfo; a same-view
retail capture would also close the visual comparison.

## Fire camera

The retail fire direction handler `0044BB00` sets directional burn-indicator
state and does not write yaw or pitch. A separate retail state-6 update,
`00451080`, does intentionally transition the view after passout:

- elapsed time is unsigned `DAT_00502AFC - Player+0x342C`, with uint32 rollover
  handled by adding `2^32`;
- the blend is `f = 1 - elapsed_ms * 0.001`;
- `0044F340` is called while `f >= 0`, including at exactly 1000 ms; the
  recovery call `0044FF10(player,0)` happens only when `f < 0`;
- the target endpoint is `(-60, 1000, 0)` and the position endpoint is
  `(0, -20, -30)`; player height is 10 in state 4 and 70 otherwise.

Thus a camera move at passout is intentional retail behavior, rather than
fire-contact yaw steering. It is a plausible explanation only if the reported
turn happens around passout, not proof of what the user observed. The shared
viewer currently has a scalar yaw/pitch renderer and lacks source-derived
head-motion, selected-item release state, and full camera-frame rendering for
this call. I did not add guessed passout rotations. `--fire-camera-audit`
remains necessary to correlate a live symptom with mouse input and the
incapacitated timestamp.

## Packaged check

`tools/build_scene_viewer_app.sh training` succeeded. The bundled executable
matches the CMake build at SHA-256
`7725ebe6fe879d2989cbf456b0d9a1a88be5bd1b350f04e94169d103303f0684`, and
`Info.plist` lint passed. Packaged `--render-check` loaded 121/121 groups and
24/24 textures with no texture failures. The Axe and extinguisher orientation
diagnostics drew 110 and 313 triangles, respectively, in both the direct and
half-turn candidates. `--hint-render-check` changed 22,418 framebuffer pixels.
Training trigger entity 106/model 3 changed 1,024,000 pixels between the
forced-visible and `ModelSetFlags(0)` check. These validate packaged port
paths only. None establishes retail held-tool pose, live camera behavior, or
visual parity.

## Delegation ledger

| Agent | Scope | Files reviewed | Status |
| --- | --- | --- | --- |
| `hand_pose_audit` | Trace retail equipment matrix and compare viewer root | `scene_viewer.cpp`, `view_update.h`, `head_motion.h`, `0045F660`/`00497F70` evidence | Complete, read-only; no supported facing flip; camera-parent reuse alone cannot recover the held ActorInfo pose |
| `camera_audit2` | Resolve state-6 view blend timing and required live inputs | `00451080` decompilation, view-call fixture/dataflow, viewer fire path | Complete, read-only; verified the one-second blend; current adapter lacks live inputs |
| `passout_view_adapter` | Assess safe integration into shared renderer | `scene_viewer.cpp`, `ViewUpdate`, native workbench | Complete, read-only; full-frame renderer and item-release/view state are missing |

## State for the next iteration

**Verified:** selected Axe resource; retail component-to-ActorInfo matrix
chain; no fire-contact yaw writer; exact passout blend duration/endpoints;
current Training build and offscreen port diagnostics.

**Provisional:** both held roots, tool offsets/extinguisher scale, user-reported
fire-turn cause, and live contact/exposure cadence.

**Open:** close the equipment-local frame producer or capture retail's final
ActorInfo matrix, and correlate the user's fire turn with first exposure vs.
passout. Do not claim either visual report is 1:1 until the retail comparison.

## 2026-10-14 passout input/release iteration

Two additional read-only subagent reviews checked the frame parent chain and
the camera event path. Fresh Ghidra output confirms `0042D2D0` stores its parent
argument at frame `+0x40`; Axe child initialization separately stores the
animation object at child `+0x0C`, while its local frame starts with no parent.
These fields belong to distinct chains. Neither the Axe nor extinguisher
constructor supplies the runtime component parent/local values or final
ActorInfo matrix, so no facing flip can be justified from this trace.

The camera review confirms the sole normal viewer yaw/pitch writer is accepted
SDL relative mouse motion. Retail fire contact does not steer those angles.
Retail state 6 instead takes the timed `00451080` view path. The shared viewer
had continued accepting mouse-look during passout and had not released an
active extinguisher action. I updated the passout adapter to release that
selected action (retail `00461590`, vtable `+0x1C`) and ignore mouse-look after
entering state 6. It still lacks the selected-item offsets and full camera
frame needed to reproduce the one-second blend, so this is a source-backed
state fix, not a 1:1 camera port.

The rebuilt Training app and CMake executable match at SHA-256
`ff7e2e0730a55d2a028a6267e5f98e9078034ca774945c8273fcdbbab67799b8`. Packaged
checks passed: 121/121 render groups, 24/24 textures, Axe `axel.act` 110
triangles, extinguisher 313 triangles, hint overlay 22,418 changed pixels,
and Training trigger 106 visibility comparison. The orientation check only
measures the two port candidates; it does not compare them with retail. No
live test was run because the camera symptom requires mouse-still input during
first fire contact and through passout.

**Verified:** passout invokes selected-item ACTION release; the viewer now
stops an active extinguisher spray and ignores SDL look input while
incapacitated; retail state-6 blend constants and timing; latest app hash and
packaged checks.
**Still provisional:** held-tool facing/position/scale and the user's camera
turn cause.
**Still open:** retail same-view tool comparison and a live `--fire-camera-audit`
trace through first exposure and passout. The next user check should preserve
the `mouse`, `exposure`, `feedback`, and `frame` lines around that interval.

## 2026-10-14 targeted recheck and packaged baseline

Two additional read-only agents rechecked the remaining pose and camera gaps.
The Axe `axel.act` root attachment matrix and the extinguisher `BONE01`
attachment matrix are both rotated, and are already composed by the ACT pose
code. The missing values are still the live component parent/local frame and
the final ActorInfo camera-space pose. No half-turn or alternate axis is
source-justified. Retail `00451080` runs the passout view blend for elapsed
times 0 through 1000 ms (`f = 1 - elapsed_ms / 1000`), then recovers only once
the blend factor becomes negative; `00450FB0` and the fire handler do not write
camera yaw/pitch. The viewer still cannot reproduce this frame transition
without its live head, selected-item offsets, and camera-frame inputs.

Rebuilt the Training app from the current source. `Info.plist` lint passed, and
the bundled executable matches the CMake executable at SHA-256
`49ec83d9d632a5b20c4775dd1a61049f6753b4748325d6d2e4eb88d30e5d6722`.
Packaged `--render-check` loaded 121/121 render groups and all 24 textures with
zero failures. The packaged `axel` and extinguisher orientation checks drew 110
and 313 triangles; direct-root and half-turn candidates differed at 20,917 and
41,814 pixels. These diagnostics exercise the port candidates only. They do
not establish which pose matches retail, and the camera report still needs a
stationary-input `--fire-camera-audit` run. Both issues remain open for a
same-view retail comparison.

## 2026-10-14 camera-default and packaged iteration

The shared viewer's ordinary camera now clamps pitch to ±40° and places the
crouched eye at 10 units (standing remains 70). The pitch evidence in
`reference/previous-work/decomp/evidence/player-tool-pose/state-offsets/pitch-writer/README.md`
shows that `00452570` reads DifficultyLevel at `TPlayerInfo+0x2C`; a fresh
profile initializes this to 1, selecting ±40°, while other loaded profile
values and the state-5/spectating exception can select a different effective
cap. State 4's 10-unit height is used by the checked-in native workbench and
the traced retail `0044F340` camera call. Both values also feed the viewer's
fire-camera audit and main render camera. These edits correct default pitch
and crouch framing; they cannot cause or explain automatic yaw rotation.

Two read-only reviews found no retail-backed Axe/ext facing correction. The
selected item's mode-2 component is composed through `00497F70` and applied to
ActorInfo by `00492450`, but the viewer still bypasses that chain with guessed
camera roots, and neither the live matrix nor the ACT native forward axis was
captured. I left the tool roots and offsets unchanged.

Rebuilt the packaged Training app. `Info.plist` lint passed, and packaged and
CMake executables match at SHA-256
`743593407d624559a0ad1c9d4beb6568f045e72c41e8bae72d409c32e71c80d3`.
Packaged diagnostics loaded 121/121 groups and 24/24 textures with zero texture
failures; the Axe ACT drew 110 triangles, extinguisher 313, hint rendering
changed 22,418 pixels, and Training trigger 106's forced-visible/flags-zero
frames differed. These are port checks only. The app has not been compared
live with retail, and the fire-camera audit needs a stationary-input run to
distinguish SDL motion from the intentional state-6 passout camera transition.

**Updated status:** camera default pitch/crouch height are source-aligned for
the fresh-profile path. Held-tool facing and live fire/passout camera behavior
remain unverified against retail. Do not check either 1:1 issue off yet.
