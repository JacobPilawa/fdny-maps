# First-person camera and Axe regression follow-up (2026-10-17)

The user reported that the Axe swings continuously, first-person view bob is
excessive, and vertical mouse look is inverted. The working macOS rescue-all
build is being corrected against the current shared port and the pinned retail
evidence; this note separates source-confirmed findings from unverified visual
parity.

## Findings and changes

- The Axe ACT swing clock had been replaced with `ItemViewPose::update`'s
  one-second item-pose clock. That clock resets after one second, while the
  retail Axe resources export `swing` tracks ending at approximately 3 seconds
  for `axe.act` and 2 seconds for `axel.act`. The render condition therefore
  kept replaying the ACT from time zero before it could reach its end. The port
  now advances `axeSwingElapsed` monotonically and keeps the one-second view-
  pose clock separate. The item view-pose clock and its source-derived
  offsets still wrap as `0045FEC0` does.
- Ordinary camera rendering now uses the `ViewUpdate` frame, whose +Z basis is
  backward and whose view ray is -Z. Positive mouse pitch means looking up;
  `ViewUpdate`'s local-X roll must receive the opposite sign to keep that view
  ray pointed upward. Both ordinary and passout transitions now pass negative
  input pitch to the frame roll.
- `00491460` initializes its three head-motion phase values to zero. The port
  had started them at `(8, 15, 4)`, which caused an arbitrary initial phase;
  these now start at `(0, 0, 0)`.
- Retail's movement head-motion records include 10-unit amplitudes in the
  authored movement modes. Those source values remain unchanged. Without a
  retail runtime comparison, reducing the amplitude would be an unsupported
  gameplay feel change. If bob remains excessive after the camera and startup
  fixes, the open question is whether host/render world-unit scale or the
  retail camera's downstream follower changes the visible displacement.

## Evidence and limits

Retail executable SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The swing track ends come from local ACT exports of retail `axe.act`
(`754eea0f17f3cca7ea33ec32e117212fd05fe34ebdac89d86f811f65184fef87`) and
`axel.act`
(`cfdc33f89ee4840dbbde05118b9e096c43538be214c0fb1ddc043f664de193c1`). The
camera sign follows the exported Player view-frame basis and the renderer's
camera-space -Z forward convention; it is not a speculative Axe-axis flip.

The macOS rescue-all bundle built successfully. The build-tree executable and
packaged app executable both have SHA-256
`669bd58dcf34e0f534954c92f4c49e5dd4822c16543ec3b26678eb3d174a45ce`.
`git diff --check` is clean. No interactive play session, retail runtime
capture, or visual parity comparison was run. The reported Axe direction
remains open: the exact camera-world-transform to Player-follower to
ActorInfo matrix is still not joined, and the preview retains its provisional
camera-relative origin.

## Follow-up source check (2026-10-17)

I rechecked the reported large bob and inverted vertical look against the same
retail executable. `00452570` builds a movement vector from the digital
forward/back/left/right actions, compares its magnitude with the binary64
global at `0x004E1C10`, and passes the resulting movement boolean plus active
byte `1` into `0044F340`. The exact global is `1e-7`; the adjacent strafe
weight at `0x004E1C18` is `0.75`. The viewer uses those same keyboard values,
strafe weight, threshold, and active flag, so the head-motion mode gate is not
a source-supported cause of continuous/full-strength bob.

The retail `00491140` records still apply the authored movement amplitudes
directly: standing movement reaches 8 units horizontally and +10/−5 vertically;
crouch movement reaches 10 horizontally and +10/−5 vertically. The port adds
the returned offset once. These are the retail-authored values, not an inferred
scale multiplier. Whether they look too large in the current viewer depends on
the unverified retail camera/follower rendering scale; changing them would
break source parity without that comparison.

Fresh Ghidra 12.0.4 headless review also covered `00483B00`, `00483A20`,
`00483720`, and `00483680`. The follower constructor starts with a null parent;
`00483A20` assigns/resolves a parent, and `00483720` copies its basis while
applying its configured flags, thresholds, and rates. The constructor does
not establish that the retail camera world transform comes from the Player
view follower or provide a runtime parent matrix. A separate camera trace
found `00491C60` calls `geCamera_SetWorldSpaceXForm` from its own context
hierarchy; the numeric link from that hierarchy to Player `+0x34A4` remains
unproven. The port's per-frame camera follower setup is therefore still a
parity gap and a plausible source of visible bob differences, not a confirmed
bug. The full camera-to-ActorInfo call chain is recorded in
[`retail_camera_actorinfo_context_trace_2026-10-17.md`](retail_camera_actorinfo_context_trace_2026-10-17.md).

For mouse pitch, `pitch` grows when SDL reports upward mouse movement. With the
retail/Genesis camera convention used by the renderer, local `+Z` is backward
and the visible ray is `-Z`; passing negative pitch to `ViewUpdate`'s local-X
roll makes positive pitch point that ray upward. The current code matches that
static frame relation. No interactive capture was run, so the user's reported
inversion is not marked fixed pending confirmation on the rebuilt app.

The extinguisher static audit found its action edge, sample generation,
eligibility, queue delivery, and suppression path present. It did not find a
deterministic input/contact/suppression drop. The camera-to-tool transform can
still place all four samples away from a fire; `--fire-camera-audit` capture or
the missing numeric matrix join is required to distinguish that from the
source-backed near-zero center-contact response. No tool-orientation or bob
amplitude guess was applied.

The rescue-all app was rebuilt from the current checkout. The build-tree and
packaged macOS executables both remain SHA-256
`669bd58dcf34e0f534954c92f4c49e5dd4822c16543ec3b26678eb3d174a45ce`;
`git diff --check` is clean. This is a packaged build check only, not an
interactive or retail-parity check. The retail EXE SHA-256 remains
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## 2026-10-17 stationary camera loop follow-up

The user reports a large repeating camera bob while stationary. The port had
left `axeActionStarted` set after the finite exported Axe `swing` ACT ended.
`ItemViewPose::update` wraps its clock at one second and produces hand offsets
up to `(12.5, -7.5, 10)`; because the action flag remained set, those offsets
continued cycling through `ViewUpdate` after the visible Axe animation had
stopped. The renderer already clears `axeSwingActive` when the exported swing
track ends. The viewer now notices that completed state on the next frame,
clears the stale action and strike flags, and zeroes the item pose before
composing the next `ViewUpdate` camera frame. This removes a port-side periodic
camera input tied to the Axe action.

The mapping from retail ACTION press/hold/release callbacks to the viewer's
mouse input remains incomplete, so the lifecycle correction is not yet a full
retail action-parity claim. Retail's ordinary idle head-motion mode still has
its source-derived 2-unit vertical breathing motion, unchanged here. The
authored Player spawn also is not floor-snapped and can cause an initial drop;
that does not explain sustained motion after landing. No interactive capture
was used to measure the user's camera after this fix.

The macOS rescue-all app rebuilt successfully. Build-tree and packaged
executables match at SHA-256
`761b4b044d47ca46c083e37fa2a3461a11b3ce0b2111c0e8ed402358435b5d43`;
`git diff --check` passes. This is a compile/package check, not a gameplay or
retail-parity check. The retail EXE remains
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## 2026-10-17 camera-context and held-tool follow-up

A second read-only Ghidra pass traced the retail camera-node constructors and
their callers against the same retail EXE hash. Vtable `004E3B38` points to
`00491D70`, whose constructor stores transform parent `param_1` at `+0x58`,
context `param_3` at `+0x60`, and clears camera pointer `+0x5C`. Its direct
callers are `00493AF0` and `004928E0`.

`00493AF0` is called only from `004956C0`. It creates a camera node whose
transform parent is the caller's scene/root pointer and whose context is the
new scene-camera object. That context creation is reached from menu and
scene/app paths; both pass the global scene/root object as the transform
parent. The second camera-bearing class is created through
`00498AC0 -> 00493BD0 -> 00491EA0`; its context comes from a descriptor field,
not a Player-view write. These constructor paths establish scene/context
ownership, but do not assign Player follower `+0x34A4` as the camera parent.

Player update `0044F340` builds frame `+0x3460`, updates follower `+0x34A4`,
then scans a linked list for a type-5 record and calls `00495450` on that
record's context. `00495450` follows the context frame's `+0x40` parent chain,
builds its matrix, and sends it to ActorInfo via `00492110`; the class-specific
consumer then updates either the Genesis camera or a held actor. The same-
routine ordering does not prove that the context chain points to
`+0x34A4`. Static analysis still lacks the runtime pointer identity or a
resolved descriptor/parent assignment that would supply that link. No global
camera flip or per-tool sign change is supported by this trace.

The camera report remains split into a source-confirmed port regression and an
unconfirmed live remainder. The old stale-Axe-action path could feed the
wrapping item-pose offsets into the camera indefinitely; the current code
clears that pose after the finite ACT ends. Retail normal standing idle motion
still has a two-unit vertical breathing amplitude. If the user's stationary
camera continues to move much more than that after the cleanup, the remaining
cause is not established by the static movement gate or idle data. The port's
camera-context/follower matrix and visible displacement scale still need a
live comparison.

The extinguisher trace also has a source-derived edge case that can look like
no effect. A near-center point can produce raw influence near `1`; the port
and retail both double it before the fire computes `1 - (1 - influence)^2`.
At delivered influence near `2`, that response is near zero. This explains a
weak center-only hit if the other three samples miss, but it does not establish
that this is the user's case. Input, route delivery, and the suppression
callback remain present in the source-shaped path. The unresolved point is
whether the camera-parented sample points hit the fire and, if so, what
influences the live frame delivers. No interactive capture was run.

## 2026-10-17 ACT translation composition check

The user's default first-equip Axe is model 1 `axe.act`, not model 6
`axel.act`. Its root is BONE01 with an authored non-identity attachment
matrix. Applying that matrix to the first non-idle swing translation from the
export (`2.677789, 15.150226, 4.153375`) yields approximately
`(0.659103, 10.888506, -11.617076)` in the actor root frame. The same
attachment-times-sample calculation for `axel.act` BONE06 yields approximately
`(-0.601105, 10.888505, -11.617077)`. Retail resources are pinned by SHA-256:
`axe.act` `754eea0f17f3cca7ea33ec32e117212fd05fe34ebdac89d86f811f65184fef87`
and `axel.act`
`cfdc33f89ee4840dbbde05118b9e096c43538be214c0fb1ddc043f664de193c1`.

These are translation-only deltas at neutral yaw. The viewer's current raw
camera-frame root maps actor-local `-Z` onto the view ray, so these composed
negative-Z root translations move toward the viewer in that preview. This is a
plausible explanation for the reported direction and identifies the guessed
camera-to-ActorInfo root as the next thing to resolve. The preview root is not
yet joined to the retail Player follower and Genesis camera matrices, and
translation alone does not capture the blade's sampled rotation, so it does
not establish the retail swing direction.

## 2026-10-17 camera, Axe and extinguisher trace continuation

The default first-equip model is `axe.act`, root bone BONE01. Its attachment
quaternion is not in Genesis's near-identity range, so the flag-0 path applies
attachment × sampled rotation and attachment-transformed sample translation.
The shared `ActorPose` evaluator uses that same composition. This rules out
the earlier possibility that the default model bypasses its authored root
attachment; it does not validate the viewer's outer actor root.

A new constructor/caller pass traced the type-5 objects reached by Player
`0044F340`. Factories `00446D10`/`00464A20` construct records through
`00446C40`/`00464660`; their shared base initializer `0049CBE0` creates a
helper at record `+0x24` through `004956C0`. That helper creates a camera node
through `00493AF0` under the global scene owner. Player `00495450(record+0x24)`
updates the helper's node through `00492110`. The node's static parent is the
global scene owner's parent; this trace found no pointer assignment connecting
that chain to Player follower `+0x34A4`. The scene owner's ancestry and runtime
identity remain unresolved, so this narrows the retail camera path without
proving a complete separation.

The opt-in `--fire-camera-audit` now samples the actual post-`ViewUpdate`
camera every 250 ms: base eye, prior follower position, final position,
head-motion mode/phase/offset, basis vectors, and view ray. When Axe is
selected, it also records the same ACT root-bone pose used by the renderer and
computes each ACT vertex's motion along that candidate view ray after the
current preview root transform. The ray points from the camera into the scene:
positive `axeVertexDeltaAlongViewRay` means farther from the camera, while
positive `axeVertexDeltaTowardCamera` means closer to it. Root-translation
fields report the same two signs. These are port runtime measurements, not a
retail capture. The audit keeps the retail camera-to-ActorInfo relation open
instead of encoding a sign correction from the current preview.

The extinguisher trace remains split between contact and response. Its active
frame now logs the full tool basis, held state, and frame delta. Each sample
logs raw and doubled influence plus the exact retail quadratic response
coefficient and expected per-frame strength change; delivery logs still show
actual before/after strength and alpha. A center hit at raw influence near 1
can deliver near 2 and therefore have near-zero response under the recovered
retail equation. A missed contact, an input/selection issue, or that weak
center response remain distinguishable only in an interactive audit capture.
No non-retail clamp or altered suppression curve was added.

## Audit build and capture path (2026-10-17)

The macOS rescue-all app rebuilt with the expanded audit. The build-tree and
packaged executables match at SHA-256
`ea5f1bd5d5fb298ca2256be4ec21480edbf07e033480b555c732dda570de9349`. The
`json-c` linker warning remains: Homebrew's dylib targets macOS 27 while this
app targets macOS 13.3. `git diff --check` passed. No interactive gameplay or
retail parity capture was run.

### Signed view-ray audit rebuild

The view-ray sign was corrected after rechecking the camera convention:
positive ray projection moves away from the camera. The audit now prints
`axeRootDeltaAlongViewRay`, `axeRootDeltaTowardCamera`,
`axeVertexDeltaAlongViewRay`, and `axeVertexDeltaTowardCamera`; per-vertex
fields are min/max/mean. The macOS rescue-all bundle was rebuilt and its build
and packaged executables match at SHA-256
`a5d3d4b169aa4389d2f300ce162b115645604485729f492eb86ef4f4a7093ac9`. `strings`
confirms the new field names are in the packaged executable. CMake and the
bundle's Info.plist lint passed; no interactive gameplay capture or retail
parity test was run. The source-derived root translation goes away from the
camera, while vertex motion from sampled rotations still needs the live audit.
See [`axe_view_ray_sign_recheck_2026-10-17.md`](axe_view_ray_sign_recheck_2026-10-17.md).

Run the packaged Training scene from a terminal to collect both the camera/Axe
and extinguisher traces:

```sh
".local/FDNY Firefighters.app/Contents/MacOS/fdny-scene-viewer" \
  ".local/FDNY Firefighters.app/Contents/Resources/scenes/training" \
  --fire-camera-audit 2>&1 | tee /private/tmp/fdny-camera-equipment-audit.log
```

Let the Player settle, keep the Axe selected, and stand still for several
seconds. `camera-pose` reports the head offset and actual final camera position;
`axeVertexDeltaAlongViewRay` and `axeVertexDeltaTowardCamera` report minimum,
maximum, and mean per-vertex swing displacement with explicit signs. The view
ray points from the camera into the scene, so positive along-ray values mean
motion away from the camera; positive toward-camera values mean motion toward
the camera. Root-translation projections are logged separately.
Then select the extinguisher, aim at a fire, and hold the action. For each
sample, compare `eligible`, `deliveredInfluence`, and
`suppressionResponse`; the corresponding delivery line shows whether fire
strength and alpha changed. The built-in trace narrows the remaining issue to
camera motion, the guessed Axe root, an extinguisher contact miss, or the
source-derived weak center response. It does not replace a retail runtime
comparison.

## Focused report recheck (2026-10-17)

The no-input camera path still reconstructs its base from Player position and
eye height, then adds `HeadMotion` mode 0 through `ViewUpdate`. Retail's idle
record has a two-unit vertical amplitude at two radians per second; this gives
a smooth, roughly three-second breathing cycle. The port does not show a
second no-input oscillation in that path after the stale Axe action and
wrapping item-pose loop are cleared. Player gravity can move the initial spawn
until floor contact, but source review does not show sustained vertical motion
once grounded. A live post-landing `--fire-camera-audit` sample is still
needed to explain a displacement larger than the source-authored idle motion.

I rechecked the retail camera consumer in a fresh scratch Ghidra project using
the pinned EXE hash above. Its vtable entry and pseudocode are recorded in
[`retail_camera_actorinfo_context_trace_2026-10-17.md`](retail_camera_actorinfo_context_trace_2026-10-17.md)
and
[`retail_camera_context_trace_2026-10-17.c`](retail_camera_context_trace_2026-10-17.c).
The trace still does not provide the runtime camera-parent or ActorInfo
matrices needed to correct the Axe root. The preview's `(35, -8, 140)` origin
and rendered swing direction remain provisional; no guessed global frame or
per-tool rotation was applied.

No audit capture was available at the documented
`/private/tmp/fdny-camera-equipment-audit.log` path during this recheck. The
user's reported jostle and swing direction therefore remain unconfirmed in the
current packaged build.

## Walking camera shake regression follow-up (2026-10-18)

The ordinary camera regression was found by comparing the current viewer diff
with its committed version. The earlier ordinary path left the camera at the
Player eye position; only passout used `ViewUpdate` and its stateful follower.
The newer path applied `HeadMotion` to ordinary movement too, including retail
movement modes with 8–10 unit amplitudes, and routed that position through the
thresholded follower. The retail amplitudes themselves were not changed.

The ordinary path now supplies zero head-motion offset and uses
`ViewUpdate`'s direct world-frame position instead of the follower result. It
keeps the existing camera pitch and held-item offsets. This disables normal
idle and walking bob and prevents the unverified follower from affecting the
walking camera. Passout still uses its separate head-motion and follower path.
The rebuilt `fdny-scene-viewer` and the existing app-bundle executable match at
SHA-256 `717cb02f64a27f59d7f47be6a024e53ece03bc5f77128cc72d722a85255947a7`;
`git diff --check` passes. No interactive gameplay capture was run, so the
user-visible result remains for the next launch to confirm.

## Complete bob removal requested by the user (2026-10-18)

The prior change still composed item view/aim animation offsets into the normal
camera, and the independent Training executable still applied `HeadMotion`.
Both playable hosts now construct the normal camera with `ViewUpdate::directEye`
from Player position, fixed eye height (70 standing, 10 crouched), yaw and
pitch. No movement phase, breathing, item pose, or camera follower enters this
frame. Passout retains its timed transition but receives zero head-motion
offset. The recovered retail math and diagnostic fixtures remain available;
the playable camera intentionally follows the user's no-bob preference.

Both `fdny-scene-viewer` and `fdny-training` compile. A temporary native harness
used actual `Player::step`, exported `custom_editor_check/collision.json`, and
`directEye` for eight seconds of alternating left/right walking at 30, 60 and
120 Hz, standing and crouched. All six cases produced exactly zero vertical
camera range, with horizontal travel above 100 units. This verifies flat-floor
walking, not a visual retail comparison. The game app and both older local
preview app executables were updated atomically and share SHA-256
`9a046b691d6e19765fb21159918d96b48c0d07da7cb0003f79bd6892c1b0ff16`.
The Training executable hash is
`bd49ad1c1c8c48cc6d6bb3457e3acc3aae68177f0f72cf4626c0f947f2c9aa00`.
An existing running game must be restarted to load the new executable.
