# Held-item facing and fire-camera follow-up

Reviewed 2026-10-14 against the current viewer source and saved, hash-pinned
retail EXE audits. No live Ghidra instance was running, so this pass produced
no fresh retail decompilation or screenshot.

## Superseding Axe resource correction (2026-10-17)

Several earlier sections below state that ordinary Axe selection activates
primary model 6 (`axel.act`). A later same-hash Ghidra follow-up found that
those sections conflated the Axe root gate at `+0x1FC` with each child's model
variant flag. The root gate starts off; ordinary first equip activates
secondary model 1 (`axe.act`). Only the duplicate `equip axe` branch sets the
root gate, after which primary model 6 can be activated too. The shared port
now follows this selection path. The final camera-space ActorInfo matrix and
swing direction remain open; see
[`equipment_action_pose_followup_2026-10-17.md`](equipment_action_pose_followup_2026-10-17.md).

## Fire-contact camera report

The viewer's ordinary yaw and pitch assignments are in the captured SDL
relative-mouse event path. The fire message adapter receives yaw by value;
the source-derived fire direction handler only updates directional burn
indicators, pain/passout state, and related feedback. It does not write
camera yaw, pitch, the Player view matrix, or mouse mode. Retail EXE evidence
for `00451640` → `0044BB00` likewise shows only a directional indicator byte
update, with no camera/view-heading write. The camera follows Player position,
so movement or collision displacement can still look like camera motion.

The opt-in `--fire-camera-audit` previously printed mouse deltas only after
exposure was already active, omitting the first contact frame. It now logs all
captured relative mouse events in audit mode and marks whether exposure was
active when each event was handled. This is diagnostic only and does not
change gameplay. It also samples heading, pitch, exposure/influence, Player
position/velocity/delta, grounded/crouched state, movement input, and derived
camera position every 250 ms. It now records SDL input-focus and relative-mode
state, actual sampled view-angle change, mouse-explained angle change and
event count per sample, and passout state. A live report is still needed to
distinguish a relative input delta from pain visuals or Player movement/collision.

## Held Axe and extinguisher

Retail setup evidence verifies equipment transform creation, the mode-2
matrix writer `00497F70` → `00492110`, pose application `00492450`, and
attached-actor render callback `004926E0`. `Player+0x34A4` is a verified
follower parent in the setup path; the final visible ACT matrix in camera
coordinates has not been established. Retail does not justify the viewer's
camera-up half-turn for the Axe, its direct basis for the extinguisher, or the
preview offsets/scale. These remain candidates rather than 1:1 corrections.

Fresh read-only Ghidra decompilation confirms `InitializePlayerAxeTool`
(`00460A50`) constructs two Axe child objects. The child setup at `00460250`
selects RRF model 6 (`axel.act`) for the flag-true child and model 1
(`axe.act`) for the flag-false child. Retail `0045F9F0` sets the parent
active byte when the Axe is selected, and `0045F8C0` gates the primary child's
post-view callback on that byte; the primary is the flag-true/model-6 child.
The viewer previously drew model 1 while Axe was selected. It now draws
`axel.act` in the active slot and uses the direct camera-root candidate,
which exposes the blade in the port render. Model 1 remains exported but is
not drawn as the selected first-person Axe. The retail camera-facing
ActorInfo matrix, hand placement, scale, and depth behavior are still open,
so this is a source-backed asset-selection correction with a provisional
render root rather than a 1:1 pose claim.

The Training app was rebuilt successfully. Its packaged executable loaded
121/121 render groups and 24/24 textures for the Axe orientation check, which
drew 110 triangles for the selected model-6 `axel.act`. Direct and half-turn
candidate roots differ at 20,917 axel pixels; the direct root shows its broad
head while the half-turn makes it edge-on. The extinguisher diagnostic drew
313 triangles and its root candidates differ at 41,814 pixels. These
measurements describe the decoded port ACTs and renderer; they do not
establish the final retail matrix.

Bundled and CMake executables match at SHA-256
`9982548e2ce2885398095f436e9e0f084886075663d52ec6987ca331b0e5e9ab`.
`tools/build_scene_viewer_app.sh training` completed, including `Info.plist`
lint. The macOS 13.3 link emitted only the existing warning that the Homebrew
json-c dylib was built for macOS 27.

The packaged `--tool-orientation-check axel` and `--render-check` both
completed. The scene check uploaded all 24 textures, submitted all 121 render
groups, and reported no GL or texture errors.

## User comparison needed

Open the rebuilt `.local/FDNY Scene Preview.app` and compare the model-6 Axe
and extinguisher facing/hand placement with the same retail view. The saved
`training.axel-camera-root.png` and `training.axel-half-turn.png` files show
port-side candidates, not retail captures. Then stand still at a fire with
the mouse physically still and note whether the camera turns, or whether only
pain/indicator feedback or Player framing changes. For a trace, run this
from Terminal, leave movement keys released, and keep the mouse physically
still through contact:

```sh
"decomp_revised/.local/FDNY Scene Preview.app/Contents/MacOS/fdny-scene-viewer" \
  "decomp_revised/.local/FDNY Scene Preview.app/Contents/Resources/scene" \
  --fire-camera-audit
```

Share the `mouse`, `exposure`, and `frame` lines, especially `viewDelta`,
`mouseAngleDelta`, `mouseEvents`, `relative`, and `focus`. Fire-contact yaw
matches the reviewed retail code, but the live input/framing symptom and exact
held-tool matrix still need comparison before this can be marked 1:1.

## 2026-10-14 recheck after the user's report

The user reports that the current hand-held models still look backward and the
camera appears to autorotate on fire contact. Two independent read-only audits
rechecked the current source against the saved retail exports. The hand audit
confirmed that retail's final camera-space matrix is still unknown and that
the current direct roots are preview guesses; no model-axis or 180-degree
change is justified by available evidence. The fire audit confirmed that only
captured SDL relative-mouse events write viewer yaw/pitch. Fire contact does
not change capture mode, angles, or the camera basis. The existing audit mode
logs SDL event deltas and distinguishes angle changes from player/camera
translation, but no live log was available to explain the report.

Rebuilt the Training app with `tools/build_scene_viewer_app.sh training`.
`Info.plist` lint passed, and the bundled and CMake executables match at
SHA-256 `9982548e2ce2885398095f436e9e0f084886075663d52ec6987ca331b0e5e9ab`.
`--help`, `--render-check`, and the Axe (`axel`) and extinguisher orientation
checks completed. The render check loaded 121/121 groups and 24/24 textures;
the orientation checks drew 110 Axe triangles and 313 extinguisher triangles.
These are offscreen port checks only. There is no retail screenshot or live
retail input trace in the project, so neither issue is checked off as 1:1.

Next required evidence: compare the same selected tools in the port and retail
game, including facing and hand placement; then run `--fire-camera-audit` with
mouse and movement still through first exposure and share its `mouse`,
`exposure`, and `frame` lines. Keep the current roots provisional until that
comparison identifies a source-backed correction.

## Latest packaged recheck

The Training app was rebuilt from the current CMake source. `Info.plist` lint
passed, and the bundled executable matches the build executable at SHA-256
`e3fd9d56979bd2d62fa0fc16a07ddb0ff3bdfedef07257d4c97f2ef3f1a52b15`.
`--render-check` loaded all 121 render groups and 24 textures with zero texture
failures. `--tool-orientation-check axel` rendered the 110-triangle Axe ACT;
the direct and half-turn roots differed at 20,917 pixels. The extinguisher
check rendered 313 triangles; its roots differed at 41,814 pixels. Those
results validate the packaged port render only and still provide no retail
comparison. Fire yaw remains statically source-aligned, while the reported
live camera symptom needs the `--fire-camera-audit` trace.

After routing the Axe click edge to the source-derived root ACTION press
deadline, the app was rebuilt and the checks were rerun. The bundled and CMake
executables match at SHA-256
`b096509f33a889d069a95a6cecb375c38fc768cfb74d8cfccb86fbba1cf35e53`.
The same render and orientation checks passed; these still do not exercise a
live click or establish retail camera-space parity.

## Per-fire exposure correction and latest packaged build

Saved retail `004483A0` evidence checks and increments the four-record field
at `FireRescue+0xB8`. The shared viewer had mistakenly kept the count outside
the fire loop, so four earlier fires could suppress all later fire candidates
in that frame. The viewer now evaluates its single Player candidate once per
FireRescue and does not impose a global four-fire cap. Exact retail reset
timing, candidate padding/scale, and single-fire damage cadence remain open;
this change only removes the proven cross-fire gate mismatch.

The Training app rebuilt successfully with that change. Its packaged and CMake
executables match at SHA-256
`c7978bb329a3f76356975f7b8fa0715da0bb43bc20042771730e42e74a54b09e`, and
`Info.plist` lint passed. The packaged `--render-check` loaded all 121 render
groups and 24 textures, emitted 28 fire sprite quads, and reported no texture
failures. The `--tool-orientation-check axel` and `--tool-orientation-check
extinguisher` runs drew the selected Axe child (110 triangles) and extinguisher
(313 triangles); both direct-root and half-turn candidates were rendered for
comparison. The packaged `--hint-render-check` also passed with 22,418 changed
framebuffer pixels. These checks verify the packaged port paths, not retail
pose or camera parity.

The latest read-only camera review again found that fire contact does not
write yaw or pitch in either saved retail code or the current viewer. SDL
relative mouse events remain the only viewer angle writer. No live mouse/input
trace or retail pixel capture is available, so the user's autorotation report
and held-tool facing are still open. Run the app from the launcher or compare
the app at `.local/FDNY Scene Preview.app`; for a fire trace, use:

```sh
"decomp_revised/.local/FDNY Scene Preview.app/Contents/MacOS/fdny-scene-viewer" \
  "decomp_revised/.local/FDNY Scene Preview.app/Contents/Resources/scene" \
  --fire-camera-audit
```

Keep mouse and movement input still at first fire contact, then share the
`mouse`, `exposure`, and `frame` log lines. Those will show whether angle
changes track relative input or whether only player/camera position and pain
feedback change.

## Rebuilt user-check package

Two focused read-only reviews rechecked the held tools and the red trigger/fog
report. Retail selects the Axe primary resource `axel.act`; the port now uses
that resource. The held mesh still uses a guessed camera root and offsets,
because the complete camera-space matrix and hand origin remain unresolved.
Neither review found source evidence that supports a 180-degree rotation or a
different root. Retail's reviewed fire handler sets a directional burn
indicator and does not write yaw or pitch; the viewer's only yaw/pitch writer
is captured SDL relative motion. There is still no live input trace or retail
image to identify what caused the reported turn or reverse-facing appearance.

The trigger review confirms Training's 25 red brush models are authored
`col_red` BSP groups. Retail initializes resolved trigger models with
`geWorld_ModelSetFlags(model, 0)` at `00441040`; the current viewer's focused
flag check again drew the red model when forced on and omitted it at flags zero.
The normal-render meaning of that bit remains inferred from upstream
Genesis3D. Retail fog is a separate command/render-driver state path and the
viewer still has no fog rendering; no fog code was added from guessed parameter
semantics.

One linked-stage edge case is now explicit in the viewer. Oil Rig B entity 78
has no `C_Model`. Retail's null-model initialization at `00441040` copies root
fields `+0xC8/+0xCC/+0xD0` into both bound endpoints, and contact check
`00440CE0` requires the model pointer at UserData `+8`. The port skips this
record instead of inventing a volume. The copied fields' exact relationship to
the exported `ZZ_location` remains open. This makes its authored fog/oxygen
touch scripts inactive through the traced BoundsTrigger contact path.

The Training app was rebuilt after the loader change. `Info.plist` lint passed,
and the packaged and CMake executables match at SHA-256
`225be011e692098a21f780557efe836f849d5b63855e5d23e20d453773a77878`.
Packaged checks report 121/121 render groups, 24/24 uploaded textures, and 28
FireRescue sprite quads. The selected Axe diagnostic drew 110 triangles and
the extinguisher diagnostic drew 313; candidate roots differed at 20,917 and
41,814 pixels respectively. The focused trigger check changed all 1,024,000
pixels between forced-visible and flags-zero frames. Rescue transaction
checks pass on Training and the five linked stages Boat B, Subway B, and Oil
Rig B-D; Boat B still reports two unimplemented ambient-sound `addtimecmd`
commands. These are port diagnostics and synthetic transaction checks, not
retail parity tests or live play.

## User comparison to close

The rebuilt app is ready at `.local/FDNY Scene Preview.app`. In Training, select
the Axe and extinguisher and compare their facing and hand placement with the
same retail view. Also check whether the trigger brushes still appear. At a
fire, keep the mouse and movement keys still through first contact and note
whether the horizon actually turns, the camera only shifts with Player motion,
or the pain/burn indicators change. If convenient, run the app executable with
`--fire-camera-audit` and share its `mouse`, `exposure`, and `frame` lines.
This live comparison is required before marking either reported issue as
resolved or 1:1.

## SDL focus guard and deeper held-matrix trace

The user's follow-up still reports backward-facing tools and a fire-entry
camera turn. A fresh viewer audit confirmed that `SDL_MOUSEMOTION` was applied
whenever the local capture preference was true, even if the window had lost
input focus or SDL relative mode was no longer active. The ordinary focused
fire handler and the retail direction handler still contain no yaw/pitch
write; the camera basis is consistent with the renderer's -Z view. I added a
narrow input guard in `scene_viewer.cpp`: release SDL relative mode on focus
loss, restore it on focus gain if the user had capture enabled, and ignore
motion unless capture, relative mode, and window focus are all active. Audit
mode logs ignored nonzero motion and focus transitions. This is a robust input
fix for stale/unfocused events, but has not yet reproduced the user's fire
symptom; it should not be claimed as the confirmed cause.

The deeper read-only equipment trace adds verified setup details. Retail
`0045F660` creates the selected-item follower under `Player+0x34A4` with scale
`(0.006, 0.009, 0.006)` and zero-initializes the item view vectors at `+0x1E4`
and `+0x1F0`. Axe/ext component scale begins at unit scale in `00497E90`.
`00497F70` builds the mode-2 matrix from the runtime component frame and helper
outputs; `00492110` writes the 3x4 pose plus separate scale fields to
ActorInfo, and `00492450` applies both. These functions are statically mapped,
but the composed numeric camera-space pose depends on live Player frame and
equipment callback state. The final Axe/ext matrix and hand position therefore
remain unverified. There is no running Ghidra instance, and a headless Ghidra
attempt was blocked before analysis by sandbox access to the user's Ghidra
configuration; copied-EXE disassembly was used for the additional raw-call
details.

The Training app was rebuilt with the input guard. Packaged and CMake binary
SHA-256 is
`4d14e55c52c68e5dca1e88a7ec9e45a903d85bb286e4832dd07eb2533a5e020d`;
`Info.plist` lint passed. Using the explicitly bundled Training directory,
`--render-check` passed with 121/121 render groups, 24/24 textures, and 28
FireRescue quads. Axe and extinguisher orientation diagnostics drew 110 and
313 triangles and saved direct/half-turn candidates. These checks confirm the
port builds and renders. `--hint-render-check` changed 22,418 framebuffer
pixels; the focused trigger check reproduced entity 106/model 3 when forced
visible and omitted it with flags zero; the synthetic rescue check released
one victim from Training's authored exit path. These checks do not establish
retail pose parity or explain the reported camera turn. A live
`--fire-camera-audit` run and same-view retail comparison are still required
to close both items.

## 2026-10-14 transform and fire-path recheck

Two read-only Luna audits reviewed the current report. The held-tool audit
confirmed that the selected Axe is retail model 6 `axel.act`, which the viewer
already draws. The current direct camera basis is consistent with the
renderer looking down local -Z, but its hand offsets and the extinguisher's
0.25 scale remain guesses. Neither a half-turn nor another axis change is
supported by the current retail evidence.

A new headless Ghidra pass used a temporary copy of the saved project and the
copied retail EXE; it did not modify the saved Ghidra database. The repeatable
script is `tools/ghidra-scripts/AuditHeldToolTransform.java`. It recovered the
previously unbounded entries `00461830` and `00497F70`: the extinguisher setup
calls the common Player follower setup, then builds equipment-specific
transform tracks and attached actor nodes; the mode-2 update composes the live
component frame and sends its matrix to `00492110` for ActorInfo pose update.
This confirms that the viewer's direct camera root bypasses retail's
equipment-specific transform chain. The final matrix values and actor-node to
ACT mapping are still not joined to camera coordinates, so the port cannot
claim the visible facing or hand placement is 1:1.

The fire-path audit again found no yaw or pitch write in retail fire handling
or the viewer's fire-message adapter. `0044BB00` stores a directional burn
indicator; the port passes yaw by value. The only viewer angle writer remains
captured SDL relative mouse motion, processed before that frame's fire exposure
check. A mouse event delivered on the first exposure frame can therefore
coincide with fire contact; camera translation and pain/burn feedback are other
possible perceived movement. The SDL focus/relative-mode guard is present, but
no user-input trace has isolated the reported live turn. The next check is to
compare exposure-entry timestamps with `mouse` and `frame` lines from
`--fire-camera-audit` while input is still.

**Verified:** Axe resource selection; no retail/port fire yaw writer;
equipment-specific matrix update chain.
**Inferred:** a same-frame SDL delta or Player framing/feedback can make camera
motion seem linked to fire contact.
**Provisional:** current hand offsets, extinguisher scale, and visible
camera-space tool orientation.
**Open:** retail comparison of both held tools and one still-input fire
contact trace. Do not check either visual issue off as 1:1 until those are
observed.

## 2026-10-14 focused iteration

Two additional read-only audits reviewed the reported backward-facing tools
and fire-entry camera motion. No retail evidence supports changing the ACT
axis or applying a half-turn: the selected Axe resource is already the
retail-primary model 6 `axel.act`, but the viewer still substitutes a direct
camera root for the retail equipment follower's composed ActorInfo matrix.
For the extinguisher, the viewer's `.25` scale and `(42,-5,145)` camera offset
also remain guesses. Both need same-view retail captures before a pose change
can be called a correction.

The fire camera review again found no yaw/pitch writer in the fire-message
path. The opt-in audit already compared sampled `viewDelta` against accumulated
`mouseAngleDelta` and reported movement, camera position, focus, and relative
mode. It now also emits `feedback=pain` and `feedback=passout` rows with the
same timestamp, yaw, pitch, and Player position. This is audit-only and does
not alter gameplay. A live stationary-input trace can now correlate pain and
passout feedback with the view-angle and Player-position samples.

The Training app rebuilt with the diagnostic markers and passed the packaged
`--render-check` (121/121 groups, 24/24 textures, zero texture failures), Axe
model-6 orientation render (110 triangles), and extinguisher orientation render
(313 triangles). The direct and half-turn candidates still differ, but those
port-only renders do not establish which orientation matches retail. Packaged
and CMake executables match at SHA-256
`84d51bfb996ab2ae167cc1e809ea232c08bd091e18b67b7a799c1cff25927386`.

**Verified:** retail primary Axe resource selection; retail fire handler does
not steer the camera; audit-only pain/passout log points build successfully.
**Provisional:** both held-item roots, offsets, and extinguisher scale.
**Open:** same-view Axe/extinguisher retail comparison and live fire-entry log.
Neither report is checked off as 1:1.

## 2026-10-14 retail matrix and state-6 follow-up

A fresh Ghidra 12.0.4 analysis was run in a new project under `.local`,
without opening or modifying the saved user database. It decompiled and saved
the real Ghidra functions `00497300`, `00495D20`, `00496020`, `0049E770`,
`00446EE0`, and `00447830` to
[`held-tool-transform-ghidra/`](held-tool-transform-ghidra/). These confirm
that retail recursively composes equipment transforms through parent frames
and writes a full ActorInfo matrix. They do not provide the runtime frame
values needed to choose a facing correction. The diagnostic roots therefore
remain unchanged and provisional.

The independent camera review recovered the state-6 passout view blend:
`f = 1 - elapsed_ms * .001`, with target endpoint `(-60,1000,0)` and position
endpoint `(0,-20,-30)`. Retail calls `0044F340` at exactly zero and only
recovers when the factor is below zero. Fire contact itself still does not
write yaw/pitch. The shared viewer lacks the live head/item/follower inputs and
full-frame camera renderer needed to add the transition faithfully.

The packaged Training build completed. Its binary SHA-256 is
`7725ebe6fe879d2989cbf456b0d9a1a88be5bd1b350f04e94169d103303f0684` and
matches the CMake target. Packaged render, Axe/ext orientation, hint overlay,
and trigger-visibility checks passed. They validate port rendering only;
neither tool-facing parity nor the live fire-camera symptom has been closed.
Full evidence, subagent ledger, and next steps are in
[`held_tool_camera_matrix_audit_2026-10-14.md`](held_tool_camera_matrix_audit_2026-10-14.md).

## 2026-10-14 rebuilt comparison package

The held-tool audit also reviewed the copied ACT bind attachments and local
geometry. Those values explain each model's authored origin and bone basis,
but do not establish which side faces the Player after retail's live mode-2
ActorInfo matrix is applied. No half-turn, axis flip, or offset change is
source-justified, so the gameplay roots remain unchanged and provisional.

Rebuilt the Training package from the current source with
`tools/build_scene_viewer_app.sh training`. The packaged and CMake executables
match at SHA-256
`743593407d624559a0ad1c9d4beb6568f045e72c41e8bae72d409c32e71c80d3`.
Packaged `--render-check` loaded 121/121 groups and 24/24 textures with no
texture failures. `--tool-orientation-check axel` drew 110 triangles and
`--tool-orientation-check extinguisher` drew 313; each compares port camera-root
and half-turn candidates only. No code correction is claimed from these
diagnostics.

**Verified:** current package builds and diagnostics pass; retail fire contact
does not steer yaw/pitch; retail state 6 has a timed view transition.
**Provisional:** held-tool facing/placement and cause of the user's reported
turn.
**Open:** same-view retail comparison of both selected tools and a still-input
fire-camera trace from exposure entry through pain/passout. Do not mark either
report as 1:1 until those are compared.

## 2026-10-14 Axe swing visibility refinement

After inspecting the staged port render at the source-mapped 0.275-second hit
window, I lowered the provisional Axe camera root's up offset from 27 to -8
world units. The 0.30-second `axel.act` sample now stays fully within the
viewport; it changes 53,610 pixels from bind pose. The half-turn candidate
remains edge-on in the port render, so the direct basis remains the clearest
visual candidate, but neither root is matched to a retail capture. The current
staged binary is `.local/FDNY Scene Preview Axe Update.app` with SHA-256
`3b109e7dacf4d1dc3cb63b7671022cb11cb4ea44270efe8b91adc5694385fade`.

The packaged rescue catalog loads all eight primary entries. Synthetic pickup
and release checks pass on Training and the seven campaign rescue maps; the
five linked stage packages pass scene inspection. Bank's headless approach
check triggers its nearest `*Door` without M and reaches PATH 1/1, then stops
on static world model 0 at a 24.011-unit lateral offset. It crosses after
lateral alignment. This is preview behavior under a controlled fixture, not
an interactive or retail comparison. The user should compare the staged Axe
swing against an authored breakable and report the onscreen hit/miss result;
backward-facing pose, live hit effects, and Bank's ordinary approach remain
open.
