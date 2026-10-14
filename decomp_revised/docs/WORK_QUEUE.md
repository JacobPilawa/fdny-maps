# Work queue and current handoff

Updated 2026-10-14. This file records the next decompilation and port tasks so
research can continue without redoing the old project review or recent audits.

## Active glitch investigation (2026-10-14)

The user's dirty-space report now has a source-mapped candidate: PlayerRescue's
`0043F810` collision callback can copy a downward floor-sweep hit into Player
origin. The Axe hit point stays local to the strike routine, and its hit scan
does not directly parent the Axe to a survivor or fire. Two Axe model variants
and their sampled transforms remain a possible visual-duplication seam. No
retail runtime repro has been captured; see
[`dirty_space_axe_warp_trace_2026-10-14.md`](../tracker/evidence/scenario-loading/dirty_space_axe_warp_trace_2026-10-14.md).
Next, compare one reproducible episode against the floor-probe and Axe model
paths before changing gameplay code.

## Delegated audit ledger

| Agent | Scope | Files | Status |
|---|---|---|---|
| `axe_collision_research` | Read-only Axe sweep, candidate list, and model-transform audit for dirty-space copies | `dirty_space_axe_warp_trace_2026-10-14.md`; pinned Ghidra exports | Complete; hit point is stack-local, direct Axe targets are type-4 objects, and the Axe stays parented to the Player on the reviewed path |
| `player_warp_research` | Read-only Player movement collision and stale-coordinate audit | `dirty_space_axe_warp_trace_2026-10-14.md`; pinned vtable bytes and Ghidra exports | Complete; vtable slot `+0x38` reaches `0043F810`, whose downward floor hit can snap Player origin; no stale-coordinate restore found |
| `axe_pose_sound_audit` | Retail Axe facing, swing direction, and sound playback trigger | Read-only saved Ghidra exports and copied-EXE disassembly | Complete; retail registers AxeSwing/AxeHitWood/AxeHitOther/WoodDoorBreak, but the playback trigger and final camera-space pose remain open |
| `ext_hud_damage_audit` | Training extinguisher pickup, retail hint window, and damage feedback | Read-only pinned executable audit and current viewer source | Complete; `ext1` is a touch PropModel whose event drain remains open; retail constructs a GUI hint widget; generic damage overlay remains unresolved |
| `retail_hint_gui_render_trace` | Trace the retail Player hint widget's render callback, text assets, layout, and styling | `tracker/evidence/scenario-loading/player_hint_widget_render_trace.md`; hash-pinned headless Ghidra | Complete; widget bounds and color markup are verified; font use and exact panel/draw path remain open |
| `retail_frame_pacing` | Trace retail delta-time consumers, frame pump, and pacing controls | `tracker/evidence/runtime/retail_frame_pacing_audit_2026-10-14.md`; read-only headless Ghidra audit | Complete; delta consumers and queue-empty update/render pump are mapped; delta writer and frame limiter remain unresolved |
| `propmodel_retail_schedule` | Resolve PropModel contact callback, pair drain, and owner cadence | `tracker/evidence/scenario-loading/retail_propmodel_contact_trace.md`; headless Ghidra follow-up | Complete; producer, visibility gate, pair delta drain, and `ext1` join are verified; external updater caller/cadence remain open |
| `gebm_truecolor_trace` | Verify embedded ACT GeBm true-color and color-key layouts | Read-only copied ACT inventory and Genesis.dll trace | Complete; referenced rescue actors include 24-bit BGR and color-key formats; decoder scope is recorded in the Genesis render note |
| `propmodel_port_review` | Review shared PropModel actor/contact integration and trace its VISDATA/PVS gate | Read-only review of exporter, `scene_viewer.cpp`, PropModel headers, and copied BSP data | Complete; lower-level cluster-PVS rejection is integrated; exact runtime area-matrix/current-view semantics remain open |
| `rescue_progression_audit` | Re-audit victim alternate spots, rescue completion, and exit progression for the next shared gameplay seam | Read-only hash-pinned retail exports and authored rescue packages; `rescue_completion_and_exit_path.md`, `victim_spawn_factory_and_order.md` | Complete; all 45 alternate-spot joins resolve and the `escape`/final-exit path is source-mapped; full contact-driven progression remains the next implementation target |
| `bank_live_hotpath_audit` | Independent Bank frame-loop/render/collision hotspot audit | Read-only current source and authored Bank export | Complete; identified 497 unconditional mesh submissions and per-fire render work; mesh frustum culling and cached fire frame durations are integrated, while live frame-time ranking remains open |
| `equipment_draw_trace` | Retail axe/extinguisher draw path, action and ACT motion mapping | Read-only Ghidra audit; `.local/ghidra/equipment-audit.log` | Complete; model draw roots, actor scale and exact axe motion link remain open |
| `fire_damage_live_path` | Why the shared viewer may not enter retail fire exposure | Read-only source audit against saved hash-pinned Ghidra outputs | Complete; ordinary frame loop calls the handler; contact values/cadence and live damage remain unverified |
| `equipment_draw_gap` | Current ACT camera placement and retail draw transform | Read-only source audit against saved Ghidra notes | Complete; found current roots behind/below the frustum; moved preview roots into camera-relative view; no live Ghidra instance was available |
| `trigger_volume_render` | Current red BoundsTrigger report, fog separation and render gating | `tracker/evidence/runtime/scene_preview_collision_texture_diagnostics.md` | Complete; current source hides zero-flag trigger models and the Training app was rebuilt; interactive relaunch confirmation remains |
| `axe_action_path` | Retail Axe input, child state and strike caller audit | `axe-strike-and-bmodel-hit-audit.md`, `axe-child-think-audit.md`, Axe port headers | Complete; shared sweep and hit leaf are integrated; child callback cadence and runtime list semantics remain open |
| `axe_strike_integration_followup` | Reassess safe shared-viewer Axe hit integration against saved strike/collision/BModel traces | `00460b90_FUN_00460b90.c`, `00492ce0_collision_wrapper.c`, `0043e2a0_FUN_0043e2a0.c`, Axe port headers | Complete; AABB, mask, hit gate, and event path are integrated; Player frame mapping, live candidate list order, and hit effects remain provisional/open |
| `axe_hit_integration_audit` | Check current Axe integration blockers and authored breakables | Read-only review of current viewer, strike/BModel Ghidra notes, Training data | Complete; first-pass sweep is viable; exact presentation pose, callback cadence, and runtime candidate order still need live comparison |
| `axe_failure_review` | Recheck reported backward pose and inert swing against current ACT and retail traces | Read-only current source, ACT export, saved Ghidra exports; no edits | Complete; corrected timer gate to prior `<0.275` and new `>0.275` crossing; no source-backed facing flip, retail matrix remains open |
| `rescue_shared_gap_audit` | Find a high-value shared rescue mechanic to continue after Axe; trace retail quake consumer | Read-only Ghidra audit and shared port review | Complete; CameraRescue's quake vector owner/updater is verified, but its connection to the rendered geCamera transform remains open |
| `finalexitloadmap_context_trace` | Determine whether the validated stage key reaches the pending loader unchanged | `00451870_player_script_command_handler.c`, `00450a70_player_finalexitloadmap_fresh.c`, `0042a260_set_pending_level_request_fresh.c`, `0047a1f0_script_command_dispatch.c` | Complete; viewer follows the authored key after the retail remaining-victim gate; checked key vs forwarded context equality and live transition timing remain open |
| `hud_goggles_trace` | Current hint/HUD and goggles visor/scan path | `scene_viewer.cpp`, `hint_overlay.h`, `goggles_renderer.h`, `goggles_scan.h`, `0044ee50_player_hint_route_fresh.c`, `004564c0_hint_window_constructor_fresh.c`, `00462a60_goggles_visor_setup_fresh.c` | Complete; render paths are in the packaged Training binary; live pixel visibility and target effect attachment remain unverified |
| `fire_damage_trace` | Whether Player fire exposure runs in ordinary viewer frames | `scene_viewer.cpp`, `player_fire_contact.h`, `player_fire_message.h`, `00451640_fire_exposure_trace.asm`, `docs/REVERSE_ENGINEERING_LOG.md` | Complete; handler is called per rendered frame; provisional spatial inputs and callback cadence leave user report unexplained |
| `player_fire_turn` | Retail `0044BB00` fire-facing side effects and HUD consumers | Read-only hash-pinned EXE instruction audit; `00452570`/`00456090` saved exports | Complete; retail sets four transient sector bytes and never changes camera yaw; exact sector map recovered |
| `held_item_axis` | Axe/ext ACT local axes and retail follower parent evidence | Read-only ACT/root audit; `actor_pose.h`, saved `0045F660` and item-view notes | Complete; direct roots and the half-turn candidate are both unverified against retail; exact retail draw root remains open |
| `held_item_draw_trace` | Inventory constructor/setup/follower chain for axe and extinguisher | Hash-pinned Ghidra headless caller/decompilation audit; `tracker/evidence/scenario-loading/held_item_orientation_audit_2026-10-13.md` | Complete; `0045F660` parent behavior is verified for setup callers, but its link to the visible inventory ACT draw root is open |
| `equipment_actor_render_path` | ActorInfo creation, special-context render callback, and pose root handoff | Hash-pinned local disassembly plus Ghidra headless follow-up; `tracker/evidence/scenario-loading/equipment_actor_render_path_2026-10-13.md` | Complete; `00497F70` → `00492110` matrix writer and `00492450` pose consumer are mapped; exact parent/source frame and per-item orientation remain open |
| `fire_contact_camera_audit` | Current yaw writes, directional-angle convention, and cooling order | Read-only source and hash-pinned retail audit | Complete; no fire path writes camera yaw; interactive response still needs the user's run |
| `retail_held_axes` | Retail view/follower frame and axe/ext ACT draw transform | Read-only saved Ghidra/ACT audit; `held_item_orientation_audit_2026-10-13.md` | Complete; selected-item view frame and follower parent are verified, visible ACT root and final axis remain open |
| `camera_fire_reaudit` | Recheck every camera/player heading writer around fire exposure | Read-only source and pinned retail audit; `current_feedback_audit_2026-10-13.md` | Complete; fire handler receives yaw by value and no fire path writes camera yaw; SDL mouse deltas remain the runtime check |
| `camera_fire_live_gap` | Recheck fire-contact autorotation against current input and movement writers | Read-only viewer and pinned retail audit | Complete; yaw/pitch only change from SDL relative-mouse events; retail state 6 intentionally blends view for 1 s after passout; live event/position comparison remains needed |
| `held_item_matrix_xref` | Continue retail ACT orientation trace through ActorInfo matrix writer | Read-only Ghidra headless on temporary project copy; `equipment_actor_render_path_2026-10-13.md` | Complete; mode-2 path writes ActorInfo matrix through `00497F70`/`00492110`; exact source frame and per-item matrix values remain open |
| `victim_ai_gap` | Check whether the roam sweep bounds are truly model-specific | Read-only pinned Ghidra evidence and startup sources | Complete; correction: retail bounds are fixed at ±24, matching the viewer; no change needed |
| `retail_held_item_orientation` | Recheck axe/ext first-person transforms against retail | `tracker/evidence/scenario-loading/held_item_and_fire_camera_followup_2026-10-14.md`; saved Ghidra decompilation; read-only | Complete; retail attached-pose writer/consumer is mapped, but exact camera-relative pose remains open; no source-backed root correction justified |
| `fire_camera_rotation` | Recheck yaw/pitch writers and first-contact fire diagnostic | Same follow-up note; viewer source and pinned retail fire exports; read-only | Complete; fire path does not rotate retail or viewer camera; audit now records mouse deltas on the exposure-entry frame |
| `held_variant_render_gate` | Retail Axe/axel resource selection and child render gates | Read-only Ghidra decompilation; `.local/ghidra/axe-pair-audit-runtime.log`; follow-up evidence note | Complete; retail active primary is model 6 axel and the selected port now draws it; secondary model visibility and final pose remain open |
| `fire_camera_input_reaudit` | Recheck camera writers and separate heading from translation/pain feedback | Read-only viewer and hash-pinned retail exports; follow-up evidence note | Complete; fire has no yaw/pitch writer; opt-in audit now samples Player motion and camera position as well as mouse events; live capture remains needed |
| `retail_held_pose_orientation` | Determine whether held ACT roots or retail matrices explain backward-facing tools | Read-only pinned Ghidra and ACT/root audit; `tracker/evidence/scenario-loading/held_item_and_fire_camera_followup_2026-10-14.md` | Complete; port camera roots are confirmed approximations; exact retail root remains open; selected Axe now draws source-mapped model 6 `axel.act` |
| `viewer_camera_rotation_cause` | Trace camera angle writes, SDL relative input, and fire/pain view effects | Read-only viewer and pinned retail audit; same follow-up evidence note | Complete; no fire yaw/pitch writer; next live run needs mouse deltas plus SDL focus/relative-mode state |
| `hand_orientation_audit` | Recheck current held-item facing against retail matrix evidence | Read-only source/ACT audit; `tracker/evidence/scenario-loading/held_item_and_fire_camera_followup_2026-10-14.md` | Complete; direct roots remain provisional and no axis change is source-supported; retail comparison needed |
| `fire_camera_audit` | Recheck current fire-contact camera/input path | Read-only viewer and pinned retail audit; same follow-up evidence note | Complete; only captured SDL motion writes yaw/pitch; no live input trace to explain the reported turn |
| `held_item_retail_transform` | Re-audit why Axe/ext models appear backward; determine whether current axis/root correction is retail-supported | Read-only hash-pinned Ghidra evidence and current viewer source | Complete; selected Axe asset is retail model 6, but live camera-space ActorInfo matrix is unresolved, so no axis flip or turn is source-justified |
| `fire_camera_writer_audit` | Re-audit all yaw/pitch writers and fire/pain effects in the viewer and retail trace | Read-only current source and saved hash-pinned Ghidra evidence | Complete; SDL relative motion is the only viewer yaw/pitch writer and fire indicators do not rotate retail camera; live input/framing trace still needed |
| `fire_exposure_crosswalk` | Compare viewer exposure candidate limit to retail FireRescue bookkeeping | Read-only saved hash-pinned `004483A0` decompilation and current source | Complete; retail limit belongs to each FireRescue instance; removed viewer-wide frame cap; single-fire damage inputs/cadence remain provisional |
| `rescue_catalog_path` | Identify broad rescue selection/transition seam without inventing stage keys | Read-only package inventory, loader, and finalexitloadmap exports | Complete; chooser and stage sibling packages are wired from catalog/authored keys; retail context equality remains unproven |
| `trigger_visuals` | Recheck red trigger brushes, fog, and Oil Rig B entity 78 | `trigger_brush_render_visibility.md`, `fog_helper_to_driver_path.md`, saved `00441040`/`00440CE0` exports | Complete; red brush gate is ported and offscreen-confirmed; fog still lacks renderer state; missing-model entity 78 cannot contact through the traced path |
| `held_tool_retail` | Recheck axe/extinguisher facing against retail matrix path | `held_item_and_fire_camera_followup_2026-10-14.md`, saved Ghidra/ACT notes | Complete; active Axe resource is verified, but camera-space matrix and hand placement are not recovered to support a pose correction |
| `held_matrix_deeper` | Recheck retail equipment parent scale, view offsets, and ActorInfo matrix writer | Read-only pinned Ghidra exports and copied-EXE disassembly | Complete; parent scale `(0.006, 0.009, 0.006)` and initial Axe/ext view offsets are mapped; matrix still needs live ActorInfo floats |
| `fire_camera_runtime` | Audit the reported fire-entry camera turn across SDL focus, relative mode, camera basis, and fire feedback | Read-only viewer and pinned retail evidence | Complete; no fire yaw/pitch write or camera-basis sign fault found; unfocused/stale SDL motion was a plausible input edge and now has a focus/relative-mode guard |
| `held_tool_pose_audit` | Recheck current visible Axe/ext facing against the retail mode-2 matrix path | `scene_viewer.cpp`, `AuditHeldToolTransform.java`, held-tool follow-up note | Complete; `axel.act` selection and live matrix writer are confirmed; current direct roots/offsets remain unverified, no source-backed axis flip found |
| `fire_camera_path_audit` | Recheck angle writers and fire-entry event order | `scene_viewer.cpp`, `player_fire_message.h`, pinned retail fire exports, held-tool follow-up note | Complete; SDL relative motion is the sole angle writer; opt-in audit now marks pain/passout; live stationary-input trace is still needed |
| `held_tool_orientation_delta` | Independently recheck backward-facing Axe/extinguisher pose against retail | Current viewer source, ACT assets, pinned Ghidra evidence; read-only | Complete; no source-backed axis/root correction, retail camera-space matrix and same-view capture remain open |
| `victim_movement_verify` | Verify victim roam step and turn helpers against retail instructions | Saved hash-pinned Ghidra output; read-only | Complete; strict target-arrival gate and exponential 0.3-second half-life turn are verified and ported; callback cadence remains open |
| `fire_hud_widget_trace` | Trace HUD child construction, resource IDs, and placement for the four fire sectors | Read-only Ghidra trace; `tracker/evidence/scenario-loading/fire_direction_hud_trace_2026-10-14.md` | Complete; resource 50 pair and exact rotated quads are mapped; first-pass port draws the live sector bytes; retail blend remains open |
| `gebm_truecolor_trace` follow-up | Cross-check natural victim and exit contact against retail source | `0045A070`, `00453A70`, `00450440` exports; `victim_transactions.h`, `scene_viewer.cpp` | Complete; confirmed the outer `+0x3438` dispatcher gate and inner carry gate; direct overlap bypass was identified and routed through the mapper; delivery cadence remains provisional |
| `propmodel_port_review` follow-up | Review BoundsTrigger gating and new victim-message integration | `scene_viewer.cpp`, `00440CE0`, `00440DC0`, `00470840` exports | Complete; confirmed `+0x164` visual gate and contact/render independence; caught and removed an unsupported Player-carry producer gate |
| `victim_message_pump` follow-up | Map the owner `+0x3BC` message drain and its order after contact callbacks | Preserved same-hash Ghidra exports for `00429520`, `0049B610`, and `004DF8CC` | Complete; same `00429520` invocation drains messages submitted during its earlier `+0xE0` contact pass; outer rendered-frame and Player update order remain open |
| `rescue_contact_gap` follow-up | Check final-exit enablement against BoundsTrigger contact semantics | `004504C0`, `00440F50`, `00440CE0`, `00470840`, authored exit inventory | Complete; exit availability is a model/render state, not a contact gate; persistent overlap remains `touching`, so no synthetic `touch` is source-supported |
| `carry_presentation_trace` | Trace what retail displays while Player carries a VictimRescue actor | Same-hash EXE pickup/status exports, Genesis actor-render gate, `scene_viewer.cpp` | Complete; pickup hides and freezes the world victim; Player status key semantics are unresolved and no Player-relative carried-body transform was found |
| `rescue_script_surface_followup` | Find broad missing authored Player/trigger behavior across campaign rescues | `00451870`, `00451560`, `00458400`, copied rescue actor inventory, shared host | Complete; all seven campaign rescues author oxygen toggles; shared oxygen state/commands and parameter-0 depletion passout are integrated; packaged catalog and synthetic rescue checks pass, while oxygen HUD rendering, cadence, opaque effects, and fog remain open |
| `victim_hazard_reactions_followup` | Recheck Player passout and carried-victim drop timing/integration | `00450FB0`, `00451080`, `0044FF10`, `victim_transactions.h`, `scene_viewer.cpp` | Complete; fire exposure uses passout argument 1, oxygen depletion uses argument 0, and the delayed drop is on state-6 completion; host services and Player `+0x3400` mapping are absent, so the transaction remains unwired |

## Current state

- **2026-10-14 Bank performance checkpoint:** optimized the retail-gated FireRescue
  visibility path, cached authored collision-model metadata for runtime traces,
  replaced per-vertex immediate rendering calls with client arrays, and added
  conservative view-frustum culling for static mesh groups. Moving/posed BModels
  bypass culling. Fire sprite duration tables are now allocated at fire
  initialization rather than once per drawn fire per frame (72 avoidable
  allocations in Bank). Bank render, interaction, and victim-release checks
  pass; the render-check framebuffer hash matches across the render-only
  optimizations. A sorted static-model AABB broadphase now narrows Player
  sweeps, while posed PATH models stay on a separate complete check path and
  candidate order is preserved. A one-second `--performance-audit` reports
  frame/update/world/overlay/swap timings and draw/cull counts. The user reports
  Bank framerate now seems sorted. Retail delta consumers are time-scaled, but
  the delta writer and presentation limit remain unresolved; the viewer's
  100 ms clamp and VSync remain provisional. Do not claim measured retail
  frame-pacing parity. The packaged rescue-all app was rebuilt, and all 13
  rescue/stage spawn and five-second grounded idle checks pass. See
  [`retail_frame_pacing_audit_2026-10-14.md`](../tracker/evidence/runtime/retail_frame_pacing_audit_2026-10-14.md)
  and [`bank_runtime_performance_followup_2026-10-14.md`](../tracker/evidence/runtime/bank_runtime_performance_followup_2026-10-14.md).
- **2026-10-14 PropModel actor/contact checkpoint:** the shared exporter now
  includes all 15 actor resources referenced by PropModels in the 13 rescue
  stage packages. The ACT decoder handles the verified paletted and 24-bit BGR
  GeBm layouts, and the renderer uploads RGBA so color keys can be preserved.
  World PropModels draw from their authored location, heading, pitch, and live
  `setscale`; `show`/`hide` also change the retail-derived model/contact gate.
  Player-point/radius contacts dispatch in retail's `touch`, `touching`, then
  `untouch` category order. The contact path now resolves the Player and
  PropModel-center BSP leaves and applies the source-backed cluster VISDATA
  rejection. The runtime area-visibility matrix, Player frame-point alignment,
  and updater's external caller/cadence remain open; positive PVS results are
  permissive rather than full retail visibility parity. Build and Training
  render checks pass.
  See [`propmodel_world_actor_port_2026-10-14.md`](../tracker/evidence/scenario-loading/propmodel_world_actor_port_2026-10-14.md).
- **2026-10-14 rescue contact and trigger checkpoint:** natural victim overlap
  now submits `0x7532` through a generation-checked Player route, then applies
  the retail message gate (`+0x3438`) and carry-pointer gate (`+0x3534`) in
  order. The producer still queues overlaps while carrying so the Player
  handler owns that rejection. BoundsTrigger contact dispatch is grouped into
  retail's `touch`, `touching`, and `untouch` passes; its visual pulse now
  requires the authored `tell player escape` gate. Contact and render state
  stay independent. The same-hash owner vtable trace now verifies that a
  message submitted during the `+0xE0` contact pass is drained by `+0x3BC`
  later in the same `00429520` invocation. The host uses the same local order;
  rendered-frame cadence and Player update ordering remain open. Final-exit
  availability changes render/model state only; a Player who remains inside
  gets persistent `touching`, so retail requires a new overlap edge for another
  `touch`. See
  [`victim_contact_message_port_2026-10-14.md`](../tracker/evidence/scenario-loading/victim_contact_message_port_2026-10-14.md).
  Carry presentation is also source-checked: pickup sets the Genesis actor
  flags to zero, so the normal world render pass hides the victim. State 5
  skips its child animation updater. No source-backed Player-relative victim
  pose was found; keep the world actor flags authoritative and leave the
  `RC_VictPick` status visual meaning open. See
  [`actor-render-flags.md`](../reference/previous-work/decomp/evidence/victim-ai-audit/actor-render-flags.md)
  and [`victim_pickup_message_path.md`](../tracker/evidence/scenario-loading/victim_pickup_message_path.md).
  The packaged `--rescue-check` now passes on Training and all seven campaign
  rescue maps through the mapped pickup queue and authored exit touch. The
  mission-catalog check loads all eight retail rescue choices; Training render,
  trigger-visibility, and extinguisher-orientation checks pass. The packaged
  rescue-all app was rebuilt after oxygen integration; current executable
  SHA-256: `5bbb5190739afcd26c219e961bd75bd128d463fed91d445d9a68fca083d76dff`.
- **2026-10-14 Player oxygen port checkpoint:** the shared host now routes
  authored `oxyon`/`oxyoff` through the Player command handler and updates the
  source-mapped oxygen scalar/timers in `player_oxygen.h`; a cross-map audit found 12 BoundsTrigger entities across the seven
  campaign rescue maps, with additional authored bindings in linked stages. Retail initializes the oxygen
  value to 1, starts with timed drain disabled, drains 0.05 every three seconds
  after `oxyon`'s 700 ms delay, and refills by frame delta × 0.1 after
  `oxyoff`. Depletion calls the parameter-0 passout route; the status updater
  uses the same value and dispatches `RC_OxygenLow` below 0.25. Depletion now
  reaches shared passout argument 0, while fire retains argument 1. The scene
  viewer compiles and the rescue-all app was rebuilt (packaged executable SHA-256
  `5bbb5190739afcd26c219e961bd75bd128d463fed91d445d9a68fca083d76dff`). The
  packaged catalog check loaded all eight rescue entries; synthetic rescue
  checks passed on the eight primary maps. Those checks do not exercise oxygen.
  Player cadence/order, oxygen HUD rendering, subordinate oxygen effects,
  state-6 recovery, and the delayed carried-victim drop remain open. The current
  passout transaction cannot be wired faithfully until the viewer maps Player
  `+0x3400`, host service callbacks, and cleanup/status behavior. Evidence:
  [`player_oxygen_resource_and_script_path_2026-10-14.md`](../tracker/evidence/scenario-loading/player_oxygen_resource_and_script_path_2026-10-14.md).
- **GitHub handoff (2026-10-14):** all 607 files from the remote
  `decomp_revised/` at `e785aec` are present locally. The October 14 sync
  includes subsequent local source, evidence, tracker, and documentation work.
  Use `python3 tools/sync_github.py` to compare, or add `--push` to publish.
  The helper maps this folder into `decomp_revised/` and preserves the rest of
  `JacobPilawa/fdny-maps`. Local Ghidra containers and retail inputs are ignored.
- **2026-10-14 Axe/rescue checkpoint:** left-click now waits through the
  source-mapped +100 ms Axe action deadline, starts the exported `axel.act`
  swing, and samples the strict animation-time crossing `prior < 0.275 < new`
  before running the `AxeStrikePose`/`AxeWorldCollision` sweep. Candidate checks
  use the retail inclusive 8-unit expansion around each loaded world AABB,
  followed by the `G_Liftable`, hit-point, and root `+0x1EC` gates. Accepted
  targets receive authored `chop`/`break` dispatch; the retail `0043D660`
  boundary starts their BModel `open` PATH. Onscreen Axe feedback reports hit
  and miss outcomes. SDL `dt`, left-click/held mapping, camera-frame conversion,
  candidate list identity/order, ACT-to-retail timer alignment, and the retail
  attached hand matrix remain provisional; no 1:1 Axe claim is made. The
  provisional camera-up root was lowered from 27 to -8 units after the 0.30 s
  exported ACT sample clipped above the view. The staged diagnostic now shows
  the full pose in frame and 53,610 pixels different from bind pose. This checks
  the port animation render, not an interactive strike. CMake and staged app
  executables match at SHA-256
  `3b109e7dacf4d1dc3cb63b7671022cb11cb4ea44270efe8b91adc5694385fade`.
  Synthetic pickup/release checks passed on Training and all seven campaign
  rescue maps. Bank's door approach check dispatches entity 541's authored touch
  without M, plays its sound, and reaches PATH 1/1; the straight approach stops
  24.011 units off center at static world model 0, while lateral alignment then
  crosses. This is a controlled preview fixture, not an interactive retail
  comparison. Retail `0044C360`'s first-populated-item refresh remains wired
  after rescue delivery. `finalexitloadmap` now gates on
  remaining survivors and chains Boat A→B, Subway A→B, and Oil Rig A→B→C→D by
  authored key into sibling packages. The viewer adapter does not prove
  retail's separate pending-context string equals that key; live transition
  contact/timing remain open. See
  [`axe_swing_runtime_port_2026-10-14.md`](../tracker/evidence/scenario-loading/axe_swing_runtime_port_2026-10-14.md)
  and [`rescue_completion_and_exit_path.md`](../tracker/evidence/scenario-loading/rescue_completion_and_exit_path.md).

  **User check:** open `.local/FDNY Scene Preview Axe Update.app`, choose
  Training, press `1`, approach a chair or box with the authored Axe hint, then
  left-click once. Report whether the tool still faces backward and which
  onscreen result appears (`no world hit`, `no eligible breakable`, or
  `chopped`). This is the interactive check needed before treating the visible
  swing or hit path as working.

- The latest Training app rebuild includes the audit-only pain/passout feedback
  markers, per-FireRescue exposure-candidate
  correction, no-`C_Model` BoundsTrigger handling, and the SDL focus/relative-mode
  guard for camera input. Fire passout now releases an active extinguisher
  ACTION and ignores mouse-look deltas during Player state 6, matching the
  reviewed release route while preventing more SDL-driven turn during
  incapacitation. The viewer now uses the retail fresh-profile DifficultyLevel
  1 pitch cap (±40°) and state-4 crouch eye height (10; standing 70) instead of
  ±60° and 30. Loaded-profile pitch limits and the state-5/spectating exception
  remain unported.
  The earlier Axe iteration was also packaged in `.local/FDNY Scene Preview
  Axe Update.app`. The standard rescue-all app has since been rebuilt with the
  PropModel cluster-PVS gate; its current executable SHA-256 is
  `b44840eced38f76a3639271ea593b77cbc856fe0614ed37b5b9341a441da8529`.
  The packaged `--victim-roam-math-check` passes the strict arrival and
  exponential heading cases; packaged `--render-check` loaded 121/121 groups
  and 24/24 textures without GL errors. `--fire-direction-render-check` drew
  all four retail sector quads in all 13 Training/rescue packages, changing
  about 130,518 framebuffer pixels per package with no OpenGL errors.
  Alpha/blend and live indicator cadence still need retail comparison.
  Packaged render and held-model orientation diagnostics pass; they do not
  establish live 1:1 visual/gameplay parity. The tools' camera-space roots and
  extinguisher scale remain provisional. Retail's one-second state-6 camera
  blend is still not rendered by the shared viewer, and a live fire-entry
  trace is still needed to separate passout view movement from SDL input. The
  pitch/crouch corrections cannot explain an automatic yaw turn. See
  `tracker/evidence/scenario-loading/held_item_and_fire_camera_followup_2026-10-14.md`.

- Fire exposure has no source-confirmed write to the viewer's mouse-look yaw. A fresh
  hash-pinned retail audit shows `0044BB00` only sets one of four Player bytes
  (`+0x353C..+0x353F`): front `[-45°,45°]`, rear `[-180°,-135°]` or
  `[135°,180°]`, positive-angle side `(45°,135°)`, and
  negative-angle side `(-135°,-45°)`. `00452570` tests these bytes to gate burn
  cooling, then clears them; `00456090` sends them to four HUD child effects.
  The shared viewer now records the source-mapped sectors and calls its
  source-derived cooling component. The port now draws the retail `fireprox`
  color/alpha pair in the source-mapped four viewport locations, rotated by
  each sector's recovered angle. The matching Player bytes drive visibility.
  Exact retail alpha sampling/blend and interactive parity remain open. An opt-in
  `--fire-camera-audit` logs exposure transitions and all captured relative
  mouse deltas, including the first exposure frame, and per-250-ms samples of
  heading, pitch, exposure/influence, Player position/velocity/delta,
  grounded/crouched state, movement input, camera position, SDL focus/relative
  mode, sampled view-angle changes, and mouse-explained angle changes per
  sample. Audit mode also tags pain and passout callbacks. A live run is needed
  to distinguish input-driven yaw from movement or pain feedback. The refreshed
  Training executable hash is
  `b44840eced38f76a3639271ea593b77cbc856fe0614ed37b5b9341a441da8529`.

- Held ACT rendering is not confirmed 1:1. Retail sets the Axe primary-active
  byte when the item is selected; its primary child is configured as model 6
  `axel.act`, and the parent gates that child's post-view update on the same
  active byte. The viewer previously drew model 1 `axe.act` while selected;
  it now draws `axel.act`, matching the active primary child's authored
  resource. The camera-root candidate exposes its blade in the port ACT render.
  Extinguisher still uses its direct camera-root candidate because that shows
  its label. Retail's exact camera-space ActorInfo matrix is not recovered:
  the mode-2 writer is mapped through `00497F70` → `00492110` → `00492450`,
  but the live source frame, hand origin, scale, and depth behavior remain
  open. The packaged Training check loads 121/121 render groups and 24/24
  textures; it does not prove retail-facing parity. The current Training app
  hash is `4d14e55c52c68e5dca1e88a7ec9e45a903d85bb286e4832dd07eb2533a5e020d`.

- The normal Training render check's spawn view has no trigger brush, so it
  cannot confirm the red-volume report by itself. A focused model-flag render
  check covers entity 106/model 3; its current packaged result is recorded
  below. Static joins resolve 25/25 Training BoundsTriggers to models using
  `col_red`; `scene_viewer.cpp` initializes their retail-derived model flags
  to zero and gates drawing on the inferred normal-render bit. Enabled exit
  pulses use approximate SDL frame sampling. See
  `tracker/evidence/runtime/trigger_brush_render_visibility.md`.

- Repeated checks against the packaged Training app found entity 106/model 3
  solid red when forced visible and absent at flags zero; all 1,024,000 frame
  pixels change. The normal render check uploaded all 24 textures, submitted
  121/121 groups, and drew 28 fire sprite quads. This confirms the current app
  package contains the trigger model-flag gate, not retail renderer parity.
  Oil Rig B entity 78 (`zone control room`) has no `C_Model`; retail
  `00441040` copies root fields `+0xC8/+0xCC/+0xD0` into equal min/max fields
  on its null-model path, and `00440CE0` requires a model pointer before
  contact. The port now skips this record without fabricating bounds. Its
  fog/oxygen scripts do not fire through the traced BoundsTrigger contact
  route. The mapping of those root fields to exported `ZZ_location` is open.

- The map-neutral Mac scene preview uses authored spawn data, recovered movement, and BSP collision. All seven rescue maps pass collision-backed spawn and five-second grounded-movement checks. A fresh Bank build and offscreen render check exercised the current draw path: all 497/497 world mesh groups were submitted and all 105/105 textures decoded and uploaded. Per-face source identity, TexInfo values, and lightmap references now pass through the exporter and C++ mesh loader with correspondence checks. This confirms metadata transport, not that materials match the original; renderer use and retail lightmap/material semantics remain open. Trigger contact/collision handling and rendering are separate.

- Same-key EVS include chains and referenced WAVs are now bundled for all 13
  rescue mission and stage packages: Training, the seven catalog rescues, Boat
  B, Subway B, and Oil Rig B-D. The current C++ loader inspects all 13 with
  scripts loaded and no texture failures. BoundsTrigger C_Model bounds resolve
  across all 17 BSP packages; the 13 rescue packages build their authored
  `makezone` groups. The synthetic `--rescue-check` also completes one
  authored escape/release transaction on all 13 packages. That does not
  verify live contact scheduling, linked-stage handoff, complete mission
  objectives, effects, or retail parity. Boat B still logs two unimplemented
  ambient-sound `addtimecmd` commands.

- The map-neutral preview now builds typed victim actors from the BSP's
  victim/alternate-spot records and hash-pinned ACT resource crosswalk. It
  selects initial motions/material variants, preserves the two victim spawn
  phases, registers alternate spots, and dispatches the five native spawn
  script fields. Authored `roam` commands now use the traced origin radius,
  FireRescue obstacle spheres, victim-box sweep, floor-height probes,
  player-relative target gates, random fallback, and walk selection. The fresh
  hash-pinned retail trace verifies the state-7 step helper's strict arrival
  test and normalized movement and the turn helper's exponential
  `0.5^(dt/0.3)` old-heading weight; the shared viewer uses those formulas.
  A focused arithmetic diagnostic passes, but callback cadence still uses the
  viewer's rendered-frame delta. Static `H_Radius` seeds fire obstacle radii
  while its retail UserData binding is still unproven; victim callback cadence
  and all-class RNG ordering remain open. The viewer now constructs authored
  FireRescue actors in entity order, applies source-shaped floor placement,
  animates their retail sprite frames, and runs the recovered FireUpdate
  transition in the rendered frame loop. The 38 retail flame/glow BMPs are
  hash-recorded in the local asset bundle. Holding left mouse with the
  extinguisher selected now uses the recovered four-point pose sampler,
  padded fire falloff, 32-record message 30000 queue, and nonterminal
  suppression transition. The viewer camera supplies the extinguisher parent
  frame, and action cadence remains a rendered-frame approximation; those
  host inputs are provisional. Random proximity spread now follows the mapped
  004483A0 → 00448320 → 00447A90 contact path, including its 750-unit attempt
  gate, candidate sweep, child shape/lifetime, and deferred creation. Its
  once-per-rendered-frame cadence, host Player::p to retail candidate-origin
  alignment, and dynamic-child RNG/lifecycle remain provisional. The gate at
  root +0x48 maps to nonempty FireRescue A_Name; unnamed authored fires and
  generated children are gated out. PlayerRescue slot +0x0C is
  now mapped to thunk 0049DB90; the viewer adapts its movement origin while
  the getter body remains supported by archived decompilation. Spread follows
  the retail nonempty A_Name gate; generated unnamed children do not become
  spread parents. Player message `0x7531` now reaches the traced pain, burn
  accumulation, and passout path; retail pain/passout WAVs are bundled. Fire
  contact scheduling, the retail Player getter's fourth value, emitter scale
  and the minimum influence constant remain provisional. The viewer now uses
  its 30-unit player hull radius as provisional candidate padding, so exposure
  can reach the player body around the authored fire volume. This corrects an
  overly restrictive zero-padding adapter, but live damage feel still needs a
  desktop capture. A read-only audit confirmed this path is called in the
  ordinary rendered frame loop; plausible non-contact causes remain the
  approximate volume/padding/scale/visibility inputs, an inactive fire state,
  or a player already in failed state. No interactive observation identified
  which applies. Spray particles and original item
  charge/refill parity remain open. Earlier MiscEmitter and other class RNG
  draws are still missing.
  The preview now bundles the map's
  EVS include chain and referenced WAVs, dispatches BModel and BoundsTrigger
  `touch`/`touching`/`untouch` events, and applies `open`/`close` plus per-owner
  time scale to retail PATHs. Bank's `VaultConsole` can issue its named
  `tell vault open` route; E/Space is the host action. Subway A's broken-door
  trigger dispatches its authored `Zapp3d` command. Synthetic interaction
  checks pass on Subway A and Bank; all 43 Subway A and 14 Bank trigger bounds
  resolve from `C_Model`; `makezone` builds six Subway A and ten Bank named
  zones. Callback cadence and wall-clock timing remain provisional, and exact
  retail contact bounds are still open. Authored `tell player escape` now
  reaches the shared victim-release and exit-gate transaction; synthetic
  headless checks pass on Training and all seven campaign rescue BSPs, each
  releasing one carried victim. MBase trigger 159 counts 1/5 rescued. These do
  not verify live trigger scheduling, carried-body
  presentation, or mission completion. Player exposure is now wired through
  the source-shaped message handler; persistent stats, passout animation/item
  release and carried-body presentation remain open. Terminal suppression now preserves
  the fire render state because retail only establishes mode 5 and a cleared,
  semantically unknown byte; its actual visual lifecycle remains open. `M`/`N` remain a
  global PATH inspection control.
  The MBase macOS app bundle had previously passed the headless rescue check;
  the Bank app bundle includes the fire assets and its packaged binary passed
  the earlier render and teleport-based interaction checks. The source
  `--interaction-check` now approaches entity 541 with `Player::step`, triggers
  its touch/OpenDoor route without M, waits for the PATH endpoint, centers at
  the aperture, and crosses using the live collision pose. This controlled
  route does not confirm ordinary approach timing or reproduce the user's
  impassable-door report. The app bundle was rebuilt for Training on
  2026-10-13 after the HUD, camera-relative per-item held model roots, goggles,
  and provisional player-hull exposure-padding changes; the
  launcher also rebuilds the selected map before opening. Source/evidence are in
  `scene_viewer.cpp`,
  `tools/export_victim_actors.py`, `tools/export_equipment_actors.py`,
  `tools/export_goggles_sprites.py`, `tools/export_fire_sprites.py`,
  `tools/export_evs_bundle.py`,
  `tracker/evidence/scenario-loading/fire_entity_viewer_runtime.md`, and
  `tracker/evidence/scenario-loading/bmodel_door_runtime_port.md` plus
  `tracker/evidence/scenario-loading/extinguisher_scene_viewer_runtime.md`.
  The latest Training rebuild includes per-item Axe/extinguisher roots; its
  packaged and CMake executables match SHA-256
  `4c9564098bda8b8395752a2983904eb6c0a93e815e3c7df45c5995242369208d`.
  `--inspect` loaded all 121 render groups, `--render-check` uploaded 24/24
  textures without GL errors, and `--tool-orientation-check` drew both held
  ACTs under both candidate roots. These do not establish retail visual parity.

- The shared viewer now runs ordered `PlayerRescue` script fields through the
  Player command host. Authored axe/extinguisher/goggles slots populate through
  the retail-mapped `equip` path, and `select` updates shared slot state;
  viewer keys 1/2/3 choose an equipped tool. Equipment ACT assets now export
  from `rescue.rrf`; the Axe left-click records the source-derived root ACTION
  deadline (global clock +100 ms), starts the named `swing` track after that
  gate, and runs its sweep on the source-mapped strict timer crossing
  `prior < 0.275 < new`. The extinguisher's ACT
  is drawn in a camera-relative preview position. The
  current axe bone hierarchy put the model below the view and the extinguisher
  contact-frame origin behind the OpenGL camera; their presentation roots now
  place both models in the first-person view and clear world depth. Current
  source draws the selected model-6 Axe and extinguisher with direct camera
  roots. A later ACT comparison found the half-turn makes the Axe edge-on and
  the extinguisher's reverse face the camera; neither render candidate is
  confirmed against retail. Exact held-item matrices and user comparison
  remain open.
  The extinguisher action/contact pose still drives spray. These roots and
  held-item depth behavior are visual approximations; the visible retail draw
  root, flags, exact local rotation, and scale remain open. Axe damage now
  reaches the shared BModel hit leaf, with on-screen hit/miss feedback; exact
  callback timing, runtime candidate ordering, and hit audio remain open. Spray effects/audio remain
  open. Ghidra confirms goggles resources 300/301
  are full-width 640x32 screen rectangles; the viewer draws their copied alpha
  masks while goggles are selected. Left-click populates the source-filtered
  kind-2/kind-6 scan list within 2,000 units, then draws resource 703 at
  approximate host target positions for the source-derived 8.5-second window.
  Visor tint, marker size/placement and child animation remain provisional.
  Authored hints now draw in an onscreen panel using the text and duration sent
  to the retail GUI hint widget; font, styling and transitions are
  approximations. The viewer camera parent and rendered-frame cadence remain
  provisional. Training's
  `setashdensity` and `setrescuebot` script fields also reach their existing
  player state. Per-item deactivation callbacks and delayed full mode-0 EVS
  ordering remain open. See
  `tracker/evidence/scenario-loading/player_loadout_command_path.md`,
  `tracker/evidence/scenario-loading/extinguisher_scene_viewer_runtime.md`,
  and `tracker/evidence/scenario-loading/goggles_scan_viewer_runtime.md`.

- Read-only follow-up audits verified that the packaged Training binary
  includes the onscreen hint panel, camera-relative equipment ACT draw calls,
  goggles visor masks and heatpulse target markers, and the per-frame Player
  fire-exposure handler. These audits confirm integration, not that those
  pixels or contact events were observed interactively. Goggles only have a
  source-backed target scan effect; general night vision is unsupported by
  current retail evidence. The Axe's exported `swing` track, source-shaped
  strict timer crossing, collision sweep, and BModel hit transaction now run
  in the shared viewer. The primary animation's ACT mapping, camera-space
  matrix, Genesis runtime hit point, candidate-list population/order,
  strike-service effects, and callback cadence remain open. See
  `tracker/evidence/scenario-loading/current_feedback_audit_2026-10-13.md`.

- 250 EXE functions have address-specific behavior notes; function activity is
  tracked separately from source recovery and port parity.
- Forty-four hash-pinned Genesis.dll functions are now decompiled and
  recorded in the separate `tracker/genesis_function_reviews.csv` engine
  ledger, including the MOTN/PATH reader, key-channel samplers, and quaternion
  interpolation entry point.
  They are not included in the EXE-only 250-function dashboard count.
- The new `.lev`/`.rrf` loader pass adds thirteen address-specific function
  reviews across catalog parsing, resource tokenization, bitmap/model records,
  menu initialization, and briefing/resource requests. Its progress history
  records 228 reviewed functions for the CSV-driven dashboard after the
  hint/goggles GUI functions and the fire-direction indicator path were
  reviewed.
- The entity-factory queue order is now mapped across a selected-level load:
  mode-1 EVS runs before World creation, WorldEntity appends BSP requests after
  the current pump's drain phase, and PlayerRescue requests are handled by a
  later pump invocation. Early shared `rescue.evs` tells to Player are dropped
  before Player registration. The delayed full EVS pass still needs ordering
  against that later pump to settle the initial selected tool. See
  `tracker/evidence/scenario-loading/entity_factory_queue_update_order.md`.
- The four code-bearing PE inputs have fresh hash-pinned inventories. The
  level-loading and renderer boundaries are mapped through the shipped
  Genesis.dll and D3DDrv.dll: GBSP v15 loading, bitmap decode/registration,
  actor material registration, Direct3D texture-handle creation, mip-level
  lock/unlock, textured polygon submission, and frame callbacks. The raw
  rescue.rrf audit separates sprite/effect/HUD alpha pairs from the embedded
  BSP face-to-TexInfo-to-TEXTURES base-image path. The exact runtime per-face
  flags/lightmaps and every dynamic surface call remain open. See
  `tracker/evidence/runtime/d3ddrv_render_path.md` and
  `tracker/evidence/runtime/genesis_render_asset_path.md`.
  FireRescue random spread, suppression, both hose/message contact routes,
  proximity gating, exposure dispatch, and the player burn/passout receiver
  now have fresh address-specific evidence. The shared owner-update to
  contact-manager traversal is also mapped through `0049BD20`, including the
  FireRescue category-bit callback path; the viewer still lacks equivalent
  typed contact lists, and rendered-frame cadence, Think/message ordering,
  child registration, and cross-system RNG order remain open.
- The ScriptActor name registry is shared across classes. `tell`'s first value
  is the case-insensitive actor name and the second is its command. Bank's
  `tell vault open` reaches the BModel ScriptActor handler; the handler's
  `open` branch changes BModel command state or queues an open action. The per-frame update maps state 3/4 progression, child-model motion, and one-shot timed command dispatch. A new map ledger joins 10 named `open` targets to their BSP actors across Bank, Boat A, Bar R, and Gas R. Retail PlayerRescue equip/select commands are statically traced from BSP/EVS through inventory state and now update the shared viewer's authored loadout; item callbacks, final selection after delayed EVS, live cadence, and runtime parity remain open. See tracker/evidence/scenario-loading/player_loadout_command_path.md.
- Subway A now has a static entity-to-handler trace for 17 automatic door BModels and 25 broken-door sound BoundsTriggers; all 51 animation children and all trigger collision models resolve. Sound-name lookup, deferred playback, sample/stream FMOD calls, and the 3D-attribute API are traced, and `SubDoor`/`Zapp3d` join to hash-pinned 3D-buffer definitions and WAVs. The resource-gate path and min/max-distance setter are statically mapped; runtime audibility/falloff and the remaining Subway rescue mechanics remain open. See `tracker/evidence/scenario-loading/subway_a_interaction_path.md`.
- The shared viewer now loads map EVS bundles, runs BModel `touch`/`untouch` bindings, applies authored time scale and named `open`/`close` commands, samples per-owner PATH state into render/collision, and plays bundled WAV resources with an approximate stereo/falloff path. Subway A's synthetic check passes. The earlier Bank teleport fixture passed `*Door` and `VaultConsole -> tell vault open -> vault` checks. The revised approach harness triggers `touch` and PATH motion without M, then deliberately waits for the full-open endpoint and centers with a side-step before it crosses; it does not test ordinary live approach timing or the user's reported route. Live contact, exact retail bound construction, callback ordering/cadence, audio parity, and `RCS_BoundsTrigger` scheduling are still open. See `tracker/evidence/scenario-loading/bmodel_door_runtime_port.md`.
- The retail Genesis.dll path pipeline is decompiled through `gePath_Sample`; the exporter decodes `MOTN 0.F0` / `PATH 0.F2` tracks for every copied BSP and preserves chunk 24 unchanged. Coverage is 1,235 paths / 2,470 tracks across 17 maps; all rotation tracks select slot 4 and all translation tracks slot 1, with 124 compressed-time lists per channel and no looping paths. A reusable nonloop mode-1 sampler ports quaternion sign continuity/slerp and translation tangent/cubic sampling from the retail DLL. The map-neutral viewer joins BModel `C_AnimModelN` children to those tracks and applies gameplay transforms to rendering and collision. `M`/`N` sample a separate inspection timeline; live BModel poses take precedence in the renderer, and touch/named commands drive gameplay state. Timing parity remains open. See `tracker/evidence/genesis-boundary/genesis_path_tracks_and_port_decoder.md`, `tracker/evidence/scenario-loading/bmodel_door_preview_gap.md`, and `tracker/evidence/scenario-loading/bmodel_door_runtime_port.md`.
- The old C++ Training workbench remains useful as the source-backed gameplay
  implementation base. It is still a Training host, not a game-wide runtime;
  Training remains a required catalog target and regression case alongside all seven campaign rescues.
- The legacy map explorer was audited as a reference viewer: all 17 BSP inputs
  are byte-identical to the current copied maps. Its geometry/material/motion
  views can help localize mismatches, but its rendering filters, static motion
  previews, and collision overlays are not retail behavior and are not port
  code. The audit and hashes are in
  [`MAP_EXPLORER_REFERENCE_AUDIT.md`](MAP_EXPLORER_REFERENCE_AUDIT.md).
- Provenance follow-up: the actor exporter currently imports an inherited ACT
  parser that is near-identical to `map_explorer/source/tools/extract_act.py`.
  Its decoded files are provisional inspection data, not independently
  derived port evidence. Verify or rederive the nested VFS, BODY, motion, and
  GeBm layouts from hash-pinned retail Genesis.dll behavior and raw ACT data
  before treating actor asset loading as source-backed. Do not use the legacy
  viewer/parser as port implementation.
- Its 1,005 used texture images across those maps were decoded and compared to
  the port BMP exports; all dimensions and RGB texels match exactly. This rules
  out image-palette conversion as the source of a texel-content mismatch. For
  the shared visible set, 179,562 polygons / 507,849 triangles also match in
  model, texture index, and per-corner UV. This shifts the visual investigation
  to lighting/lightmaps, alpha/material flags, GPU sampling, and dynamic model
  state. See `tracker/evidence/runtime/legacy_texture_rgb_comparison.md`.
- The GBSP exporter produces all 17 local BSP packages. `SceneData` preserves
  every exported JSON document, validates package identity/schema, and resolves
  package asset paths without case assumptions.
- The standalone viewer loaded all 17 packages through the C++ loader with
  zero unresolved mesh texture paths. Fresh floor sweeps and idle movement
  checks pass on all seven rescue maps; BMP decode succeeds for every unique
  texture file. Offscreen CGL render checks now confirm OpenGL texture upload
  and varied framebuffer output on all seven. See
  `tracker/evidence/runtime/scene_preview_collision_texture_diagnostics.md`.
- The user requires every rescue mission and map/mechanic to remain in scope,
  with a cross-platform desktop port and macOS as the first playable target.
- Gas R BSP entities 265, 280, and 309 are joined to the explosion1, explosion2, and gasoff EVS definitions. ScriptActor queue pop/order and wait deadlines are mapped, alongside BModel button1 use handling, type-1 contact candidate checks, and shared BoundsTrigger overlap-pair → touch/touching/untouch dispatch. The named-model motion join resolves button1's effective reach to 20 units. Live contact/selection, updater cadence, and original-game effect captures remain open. See the Gas R evidence linked in the scenario-loading README. The same retail Genesis.dll MOTN/PATH extent decode is now joined across Training and all seven rescue maps: 71 of 76 authored-gate BModels receive a static 48-unit reach; five motionless ladder/button models retain authored values. Per-map/entity results are in `tracker/evidence/scenario-loading/bmodel_reach_cross_map.md`, its JSON inventory, and the rescue coverage CSV. This closes only the static reach calculation; live contact and callback cadence remain open.
- The shared Script host now routes the retail FireRescue `setsize x y z duration` form to the targeted actor's timed `FireSize` track, including the authored key reset/current-size anchor and the `clock + duration * 1000` target. This enables Gas R `button1 -> gasoff`'s six size commands without a map-specific branch. The port uses SDL ticks and host frame updates; live button use, retail visual timing, and original-game effect parity remain open. Boat A's separate exploder `setsize 3` path is not covered. See `tracker/evidence/scenario-loading/firerescue_setsize_script_port.md`.
- `tracker/rescue_mission_coverage.csv` explicitly tracks Training plus all
  seven campaign rescue missions. All eight load as geometry packages in the
  inspection viewer; none is yet a playable rescue port. `level_map_inventory.csv`
  and `scene_load_coverage.csv` cover the other catalog entries and all 17 BSPs.
- `tracker/rescue_actor_inventory.csv` maps 564 authored player, fire, victim,
  alternate-victim-spot, and PathPoint entities across all 17 BSPs. Fire/victim
  counts reconcile to the level inventory. Raw epairs and positions are
  retained for map-neutral spawn conversion; current-hash code confirms the
  VictimRescue and VictimAltSpot factory path and registration-before-spawn order.
- Victim `roam <number>` traces from the class-specific ScriptActor fallback
  through obstacle collection, initial-position checks, animation selection,
  target selection, and state-7 movement. Obstacle kind 6 is tied to
  `RCS_FireRescue`, and its virtual radius getter reads root `+0x88`; the static
  host uses authored `H_Radius` pending proof of the retail UserData binding.
  The viewer now uses the source-derived candidate/retry/player gates and fire
  clearances. `forcespawnspot` selects from the victim's registered spawn list.
  The actor inventory summarizes the exact
  non-empty victim script values across every BSP; the alternate-spot name
  resolver joins all 45 authored spots to a victim on the same BSP. Numeric-only
  fields and update cadence remain unresolved. The current-hash factory trace confirms alternate-spot registration before victim spawn selection across the authored rescue BSPs. See
  `tracker/evidence/scenario-loading/victim_roam_behavior.md` and `tracker/evidence/scenario-loading/victim_spawn_factory_and_order.md`.
- The shared victim pickup path now maps contact overlap through Message Mapper
  message `0x7532`, the Player's empty-carry gate, and the VictimRescue `pickup`
  event. The shared escape/release path updates rescue statistics and victim
  state, then applies final-exit gates. A generated 17-BSP inventory records 40
  authored exit triggers, joins all 40 `C_Model` values to named BSP models, and
  records Boat, Subway, and Oil Rig stage destinations. `tell player` now traces through the static PlayerRescue factory and handler path;
  trigger/update cadence and live mission parity remain open. See
  `tracker/evidence/scenario-loading/victim_pickup_message_path.md`,
  `tracker/evidence/scenario-loading/rescue_completion_and_exit_path.md`, and
  `tracker/rescue_exit_trigger_inventory.md`.

## Delegated audit ledger (2026-10-13)

- **Fire viewer integration audit — complete, read-only.** Reviewed
  `scene_viewer.cpp`, the FireEntity/FireRenderer components, and the app
  bundler. Its recommended entity-order initialization, explicit sprite root,
  and draw integration are implemented and documented in
  `tracker/evidence/scenario-loading/fire_entity_viewer_runtime.md`.
- **Material alpha and Bank collision correlation — complete, read-only.**
  Reviewed the scene exporter, world mesh batching, texture loading, trigger
  mesh join, pinned Genesis/D3D render evidence, and the new model-0 approach
  trace. Confirmed the collision hit is not the `wal_Stco_wall` start-view face
  candidate; retail world sweeps and animated BModel transforms are separate,
  but the exact Bank BSP leaf/aperture remains unknown. No agent edits were
  made.
- **Trigger visibility audits — complete, read-only.** `trigger_visual_audit`
  joined all 25 Training BoundsTriggers to their rendered BSP models and
  confirmed each includes authored `col_red`; `trigger_retail_flags` traced
  retail `ModelSetFlags(0)` setup and the mode-gated update pulse. Integrated
  in `scene_viewer.cpp` and documented in
  `tracker/evidence/runtime/trigger_brush_render_visibility.md`. The normal
  render bit and host pulse cadence remain inferred/provisional.
- **Fog renderer audit — complete, read-only.** `fog_renderer_trace` followed
  `fogenable`/`fogdisable` through `004789D0`/`00478AC0`, the state setters,
  `0049CF10` interpolation and camera far-clip update, and the Genesis
  `geEngine_SetFogEnable` call. The helper-to-driver path is now recorded in
  `tracker/evidence/runtime/fog_helper_to_driver_path.md`. Authored parameter
  names, visual driver semantics, cadence, and a shared-renderer fog port remain
  open; no agent edits were made.
- **Ladder runtime audit — complete, read-only.** `ladder_runtime_audit`
  reviewed the Training fixture, player command handler, reach, and collision
  evidence. It found that the ladder's own solid model may invalidate every
  candidate in the current fixture; retail collision filtering is unverified.
  No files changed.
- **Bank collision/material follow-up — complete, read-only.**
  `bank_contact_review` checked the live-loop ordering and found no M-to-
  collision bypass; the ordinary user route remains untested.
  `bank_retail_collision_trace` confirmed retail Player bounds at
  `0044D550` as mins `(-30,-30,-30)` and maxs `(30,85,30)`; the port matches
  them. `mesh_metadata_audit` reviewed source face/span correspondence, and
  `retail_material_trace` found no live Ghidra session or source-backed retail
  blend/depth/lightmap semantics. Their scopes were read-only; findings are
  recorded in the Bank evidence notes.

## Next tasks, in order

### User-reported desktop blockers (2026-10-14)

- **Held tools and fire-camera turn (current user report):** the latest Training
  package includes an SDL focus/relative-mode guard: the viewer releases mouse
  capture on focus loss, restores it on focus gain when capture was requested,
  and ignores relative motion unless capture, relative mode, and input focus
  are all active. Its pitch cap now matches a fresh DifficultyLevel 1 profile
  (±40°), and crouch view height is 10 as in state 4. Packaged Training render
  and Axe/ext orientation diagnostics pass. Fresh Ghidra helper decompilation
  confirms retail's hierarchical equipment component/parent-frame matrix
  writer, but no live ActorInfo camera-space
  values are available to justify a facing/root correction. Retail fire
  contact does not write yaw/pitch; retail state 6 intentionally blends the
  view for 1 second after passout. The shared viewer lacks the state-6 inputs
  and full-frame renderer to implement that call faithfully. The user-reported
  live turn remains open until it is correlated with passout vs. relative mouse
  input. The latest package was rebuilt from source; `--render-check` loaded
  121/121 groups and 24/24 textures, and Axe/ext orientation checks drew 110
  and 313 triangles. These and the hint/trigger checks are port-only checks.
  Same-view retail tool comparison and a still-input
  `--fire-camera-audit` remain required before marking parity. Latest
  packaged/CMake binary SHA-256:
  `b44840eced38f76a3639271ea593b77cbc856fe0614ed37b5b9341a441da8529`.
- **Door motion/collision:** Bank's authored door route now applies its source-derived openaway direction and PATH motion from contact; M/N only control the inspection timeline. The fresh continuous route to entity 541 dispatches touch, plays `OpenDoor`, and reaches PATH 1 without M, then stalls on static world model 0 at X offset 24.011. The endpoint/alignment control crosses at X=-719.419. Retail exports confirm swept collision checks static and transformed models and BModel PATH updates model transforms; the exact BSP aperture and live ordinary route remain open. Reproduce from a new app launch and compare touch dispatch, PATH/render/collision pose, player position, and world hits. Callback timing remains provisional.
- **Bank start/render appearance:** the user expects an open start but sees an opaque enclosure. The fresh offscreen Bank check submitted all 497 world mesh groups and decoded/uploaded 105 textures. It showed a dark obstruction but did not match a surface to the user's interactive view. All groups use the ordinary render path. The exporter and C++ loader now retain per-face identity, TexInfo values, and lightmap references; trace retail rendering and implement material behavior generically. Keep the visual diagnosis distinct from player collision. See the Bank start-view surface audit.
- **Bank performance:** the Bank hot path now skips FireRescue LOS traces outside the retail 500-unit gate/falloff, caches collision-model bounds, draws mesh buffers through OpenGL client arrays, culls static groups outside the camera frustum, and caches fire sprite durations at initialization instead of allocating once per fire draw per frame. Moving/posed BModels bypass culling. Bank render, door/vault interaction, and victim-release checks pass; the render-check image is unchanged. The user now reports Bank framerate seems sorted. This does not yet measure same-route retail FPS or frame pacing; the delta writer and presentation limit remain unresolved. Keep physics time-scaled while those retail timing facts remain open. See [`bank_runtime_performance_followup_2026-10-14.md`](../tracker/evidence/runtime/bank_runtime_performance_followup_2026-10-14.md).
- **Extinguisher, hint windows, and damage feedback:** Training entity 161 `ext1` is authored as PropModel `D_Model=2` (`ext.act`) at `(780,-164,-1748)`, with 70-unit touch radius and `touch:*RefillExt`; it has no `E_Use`. That macro equips/refills the extinguisher and plays `Steam4`. The viewer now exports and draws world PropModels and routes player-point/radius contact through authored script events, including `ext1`. Its contact path applies the lower-level cluster-PVS rejection, while the runtime area matrix/current-view semantics and update cadence remain open. Training build and render checks pass, but no live contact or retail capture has confirmed the pickup yet. Retail hint routing `0044EE50→004564C0` constructs a GUI text widget at Player `+0x6D8`; matching renderer/font details remain open. No generic full-screen damage overlay was found in the bounded audit. See [`propmodel_world_actor_port_2026-10-14.md`](../tracker/evidence/scenario-loading/propmodel_world_actor_port_2026-10-14.md) and the delegated ledger above.
- **Fog and remaining red surfaces:** Training's solid red blocks are now
  joined to 25/25 authored BoundsTrigger models using `col_red`; retail setup
  sets those model flags to zero, and the shared viewer honors that model
  state. A repeated packaged check reproduces the forced-visible red brush and
  its removal at flags zero. Oil Rig B entity 78 has no `C_Model`; retail's
  contact check requires the model pointer, so the port skips it without
  fabricating bounds. The flag-to-normal-render bit remains inferred. Keep
  investigating Bank/Gas R alpha-128 red faces and implement face alpha/blend/depth
  generically. Fog is separate: Gas R binds `fogenable 5 320 60 40 40` to
  `inside` and `fogdisable` to `outside`; Bank/Boat also author default fog
  tuples. The dispatcher passes five script values to `004789D0` and
  `fogdisable` calls `00478AC0`; the current tuple interpolates into
  `geEngine_SetFogEnable`, alongside generated channel values. Semantic names,
  driver conventions, cadence, and the shared port remain open. See
  `tracker/evidence/runtime/trigger_brush_render_visibility.md` and
  `tracker/evidence/runtime/fog_helper_to_driver_path.md`.
- **Ladder interaction:** the viewer routes authored climb/unclimb commands,
  but its fixture's ladder 108 model 5 is thin and remains in player collision.
  The agent audit suspects the fixture rejects all candidates because the
  ladder itself collides with the player hull; retail filtering is not proven.
  Trace Player collision/model filtering before changing the solid check, then
  validate contact, alignment, movement, animation/camera, and live scheduling.
1. **Confirm shared doors and triggers against an interactive run.** The current
   packaged rescue-all app passes `--interaction-check` for Bank and Subway A.
   Bank entity 541 receives touch, plays `OpenDoor`, and reaches PATH 1; the
   straight route stops on static world model 0 at a 24.011-unit lateral
   offset, while the centered control crosses. Subway A entity 274 opens at
   speed 4, closes on untouch, and trigger 515 dispatches `Zapp3d`. These are
   controlled headless checks. Exact BSP aperture, ordinary user route,
   callback cadence, contact timing, live exit scheduling, and full mission
   completion remain open. Do not change the Player hull or disable the wall
   without new source evidence.
2. **Trace oxygen status and passout completion.** The shared oxygen resource
   and authored commands now run, but the viewer does not draw the gauge or low
   warning. Retail `00458400` smooths the resource into Player status widget
   `+0x28d0`; trace its construction and draw path before adding presentation.
   Retail state 6 calls `0044FF10(player, 0)` only when unsigned elapsed time is
   strictly greater than 1000 ms. The existing carried-drop transaction still
   lacks a viewer mapping for Player `+0x3400`, required passout host services,
   and cleanup/status callbacks; `readGlobalMode` is also fixed at 0, so its
   mode-2 HeroDown branch cannot run. Map those inputs and callbacks before
   wiring the one-shot completion. See the oxygen and held-tool/camera evidence.
3. **Material and render parity.** The current Bank render check submits every
   exported mesh group and decodes/uploads all referenced textures, but its dark
   obstruction is not matched to the user's start view. Face identity, TexInfo
   alpha/flags, and lightmap references now survive the export and loader path;
   trace the retail render path before implementing general material behavior.
   Keep player collision separate from appearance and do not add guessed
   visibility rules.
   The RGB texture and shared face/UV comparison is recorded in the legacy
   texture comparison evidence; use the old explorer only to localize a
   mismatch.
4. **Desktop actor check.** The rebuilt MBase bundle includes the map and
   decoded victim ACT assets. Confirm victim placement, motion, contact pickup,
   and authored escape trigger behavior in the interactive macOS window. Try
   walking into a victim, then through `escape1` (bounds center near
   `1906, -124, -2788` in the viewer's displayed coordinates). The title should
   show carry and rescue state; carried-body presentation remains missing.
   Record the map/camera when a base
   texture binding still appears wrong; the current audit found no proven
   face-to-texture index mismatch. Lighting, lightmaps, alpha, and window
   presentation parity remain visual follow-ups.
5. **Runtime catalog/resource resolver.** The `.lev`/`.rrf` loader trace is
   recorded in `tracker/evidence/scenario-loading/lev_rrf_catalog_loader_trace_fresh.md`.
   Implement catalog-selected `SceneData` creation with explicit case aliases for
   BSP, briefing, EVS, actors, motions, sounds, and textures. Retain the original
   resource spelling and source hash on each resolved row.
6. **Promote the viewer into the shared host.** The viewer now loads actors,
   starts the traced victim roam update, and applies proximity pickup. Extract
   reusable startup, movement, collision, renderer, and asset loading from the
   Training `main.cpp`; keep the current Training executable as a regression
   fixture. Reconcile the ported roam behavior with exact native turn and move
   helpers; no map name or Training assumption belongs in the shared loader.
7. **Wire rescue entities and script startup.** Derive common typed player,
   fire, victim, bounds-trigger, BModel, prop, and exit records from the raw
   ordered epairs. Preserve unknown classes and properties. Trace MBase's
   EVS/shared-EVS commands and original callback cadence before dispatching.
   `moveto` now traces through FireRescue command parsing, shared ScriptActor
   name lookup, state-4 update, timed-key generation, and visual vector interpolation. Bank's `vault open` command now maps from map EVS through the shared registry to the BModel-specific handler. The PathPoint initializer
   passes its authored name into the ScriptActor field searched by that lookup.
   MBase's `setashdensity 0` route now maps to the clamped Player scalar at
   `+0x5b0`; the MBase `tell` is delayed until mode 0, but its order relative to
   Player registration remains open. The previous ash-pool analysis and native
   clamp implementation are reused.
   Determine the remaining PathPoint field meanings, the exact authoring/runtime
   intent of the two trailing `moveto` timing values, the external
   scheduler/cadence for `UpdateFireRescueActor`, and the ten `S_Script`
   event-command semantics. The VictimRescue `roam` path is statically mapped;
   continue with runtime capture of Bar R's unhandled `setskin 2` fields,
   numeric-only victim script values and victim update cadence; the current-hash
   factory path and registration-before-selection order are now mapped. Capture live command timing and the final selected tool per scenario; the static
   Player identity/recipient route is now mapped, and every rescue mission and
   stage remains in scope. Ordered PlayerRescue equipment commands now update
   shared viewer inventory state; finish delayed EVS handling, item actions,
   and goggles visibility.
8. **Build one complete rescue path, then cover every rescue mission.** MBase
   is first for loader integration; the source-derived release transaction is
   now reachable from its authored `escape` binding, but survivor carry
   presentation, live contact scheduling, and full mission completion remain
   open. Complete those and shared doors, then extend the same path to the
   required `bank`, `bar_r`, `gas_r`, `boat_a`, `subway_a`, and `oil_rig_a`
   scenarios. Include Training as a required catalog scenario. Reach visible actors, fire state,
   tool interaction, victim rescue, and mission completion for each while
   checking behavior against pinned EXE evidence.
9. **Cover the complete campaign and platforms.** Track each rescue catalog
   scenario, then all hose/other catalog entries and the remaining BSP
   variants. Add Windows/Linux builds against the same scenario contracts.

## Parallel review results already incorporated

### Current held-tool and fire-camera audit (2026-10-14)

| Agent | Scope | Files reviewed | Status |
| --- | --- | --- | --- |
| `hand_pose_audit` | Trace retail equipment matrix and compare viewer root | `scene_viewer.cpp`, `view_update.h`, `head_motion.h`, retail transform evidence | Complete, read-only; no supported facing correction; live ActorInfo values remain missing |
| `camera_audit2` | Resolve state-6 blend timing and required view inputs | `00451080` decompilation, view-call fixture/dataflow, fire adapter | Complete, read-only; verified one-second passout blend; current adapter lacks live inputs |
| `passout_view_adapter` | Assess safe state-6 view integration | `scene_viewer.cpp`, `ViewUpdate`, native workbench | Complete, read-only; full-frame camera renderer and selected-item release/view state are missing |
| `tool_pose_matrix_trace` | Resolve retail frame parentage for the backward Axe/ext models | Fresh headless Ghidra pass; `.local/held-transform-audit2.log`; equipment actor path note | Complete, read-only; child animation parent and component frame parent are separate; live component/ActorInfo values remain missing |
| `fire_camera_runtime_trace` | Recheck the runtime yaw/pitch path and state-6 behavior | `scene_viewer.cpp`, `00451080`, passout and view-call notes | Complete, read-only; fire has no yaw writer; passout has a one-second retail view transition; stationary live trace remains needed |
| `camera_crouch_review` | Verify current pitch cap and crouch camera-height edits against retail evidence | `scene_viewer.cpp`, pitch-writer README, held-tool camera note | Complete, read-only; ±40° is the fresh-profile default and height 10 matches state 4; neither causes yaw rotation |
| `hand_pose_trace` | Recheck the retail Axe/ext transform chain for a supported orientation correction | Ghidra project copy/log, held-tool camera note, `scene_viewer.cpp` | Complete, read-only; mode-2 component reaches ActorInfo but live matrix/native forward axis remain unknown; no flip/offset edit justified |
| `held_pose_fresh_trace` | Recheck ACT local axes against the retail equipment matrix chain | Current ACT exports, pinned Ghidra notes, `scene_viewer.cpp` | Complete, read-only; both ACTs have rotated root attachments and final camera-space pose is still missing; no facing correction justified |
| `camera_passout_trace` | Recheck retail state-6 blend endpoints/timing and viewer angle writers | `00451080`, `00450FB0`, `00451640`, `scene_viewer.cpp`, `view_update.h` | Complete, read-only; one-second passout view transition verified, but full live frame inputs are missing; fire/pain do not write viewer yaw/pitch |

The latest source-backed iteration follows the retail passout transaction and
camera defaults:
entering state 6 releases the selected item's `+0x1C` callback. The shared
viewer now ends an active extinguisher action and rejects mouse-look while
incapacitated. It still does not render retail's one-second `00451080` view
blend, because the selected-item offsets and full live camera frame are not
available. The held tool roots remain provisional; Ghidra's `0042D2D0` parent
pointer at frame `+0x40` and the Axe child's animation parent at `+0x0C` belong
to distinct chains. The current Training binary is hash-matched, and the
packaged render, Axe/ext orientation, hint, and trigger checks pass. The
fresh-profile pitch cap and state-4 crouch height follow source-backed
defaults. These checks do not establish retail visual parity; held roots,
loaded-profile pitch state, state-6 frame rendering, and the live camera
symptom remain open.

### Current execution ledger (2026-10-13)

| Agent | Scope | Files reviewed | Status |
| --- | --- | --- | --- |
| `door_evidence_audit` | Audit retail BModel command/contact behavior and Subway A/Bank fixtures | `bmodel_update.md`, `bmodel_door_speed_semantics.md`, `subway_a_interaction_path.md`, `bank_vault_open_dispatch.md`, `rescue_contact_manager_route.md`, fresh Ghidra exports | Complete; evidence integrated into the runtime port note |
| `door_host_architecture` | Identify the shared viewer integration seam and build/launch route | `scene_viewer.cpp`, `scene_data.h`, `gbsp_motion_path.h`, `bmodel_contact.h`, Training workbench notes | Complete; architecture integrated, no overlapping edits |
| `rescue_crash_audit` | Read-only audit of the headless `--rescue-check` crash | `scene_viewer.cpp`, `victim_transactions.h`, release/frame helpers | Complete; static review only because its environment lacked exports; root reproduced and fixed the no-GL headless actor-loading fault, then recorded the passing MBase check |
| `carry_pose_audit` | Read-only audit of retail carry presentation and actor render behavior | `victim_pickup_message_path.md`, `post-pickup lifecycle evidence`, `scene_viewer.cpp`, actor render flags | Complete; pickup hides the world actor as source-backed; no retail carry anchor or continuous pose path is established, so do not invent transform math |
| `door_m_runtime_audit` | Audit reported door that sounds on approach but moves only after `M` | `scene_viewer.cpp`, BModel runtime notes, Bank/Training authored EVS | Complete; `openaway` host gap is implemented; follow-up found the synthetic fixture selected distant entity 365, so the check now selects nearest start door 541; live approach and collision remain unresolved |
| `bank_trigger_render_audit` | Identify Bank start enclosure and assess whether alpha is responsible | `bank/entities.json`, `world_mesh.json`, trigger model resolver, bounds flags evidence | Complete; the fresh normal render submits all 497 world mesh groups. Red exit-trigger faces with alpha 128 are a candidate, not a match to the user's view; retail material semantics remain open |
| `door_live_path_audit` | Explain the M-only door report against current live and headless paths | `scene_viewer.cpp`, `core.h`, `scripts.h` | Complete; M/N affect inspection only, while the harness waits for full-open and aligns before passage; exact live route remains open |
| `bank_start_render_audit` | Recheck Bank camera, render obstruction, and bundle provenance | `scene_viewer.cpp`, Bank exports, app bundle, launch scripts | Complete; a fresh offscreen render shows an unmatched dark polygon with all 497 mesh groups submitted. Exact user-view match and material behavior remain open |
| `fire_integration_audit` | Read-only review of fire simulation/contact integration seam | Fire entity, suppression, exposure helpers, scene viewer, fresh EXE evidence | Complete; no edits; terminal visual shutdown is unsupported and contact scheduling remains open |
| `victim_equipment_audit` | Read-only review of victim carry/rescue and equipment/goggles integration seam | Victim transaction helpers, Player loadout trace, scene viewer, fresh EXE evidence | Complete; no edits; authored PlayerRescue loadout integration implemented by root; item behavior remains open |
| `extinguisher_port_seam` | Read-only review of generalizing Training extinguisher contacts into the shared viewer | `main.cpp`, `scene_viewer.cpp`, extinguisher/fire helpers, fresh suppression evidence | Complete; no edits; identified the camera-parent/cadence boundary; shared message path integrated |
| `goggles_visibility_audit` | Read-only trace of goggles inventory callbacks and visibility effects | Player loadout evidence, goggles initializer/vtable, scene viewer | Complete; no edits; slot 3 is verified, visibility semantics remain unknown |
| `fire_spread_seam` | Read-only audit of random spread integration and callback dispatch | `scene_viewer.cpp`, FireEntity components, fire-spread evidence, shared contact manager | Complete; no edits; dispatcher traversal is source-mapped and spread is now wired provisionally; candidate lifecycle, RNG ordering, and owner cadence still block faithful parity |
| `goggles_trace` | Read-only trace of goggles vtable, selection callbacks, and scan visibility behavior | Pinned EXE vtable/data, goggles exports, Player selection path | Complete; no edits; verified target filter/range and callbacks; resource 703 and the 8.5-second effect window now drive the shared heatpulse renderer; child animation and broad visibility remain open |
| `goggles_effect_review` | Read-only review of preview marker fidelity against the retail target update | Goggles renderer, scan runtime note, pinned EXE disassembly | Complete; no edits; corrected host billboard description; retail actor-local transform and child attachment remain open |
| `fire_exposure_integration` | Read-only review of Player fire exposure/passout delivery seam | `player_fire_message.h`, exposure/passout helpers, scene viewer, fresh FireRescue/Player evidence | Complete; no edits; documented missing cadence, LOS, player-effect/audio/stat bindings, and passout host services; do not wire guessed automatic scheduling |
| `spread_trace_audit` | Read-only audit of current-hash random spread semantics and deferred creation seam | `00447A90_fresh.c`, `00448320_fresh.c`, contact-manager route, scene viewer | Complete; no edits; confirmed shape/queue details and the pointer-at-root-+0x48 / pointed-to-+4 gate; root follow-up mapped it to string length |
| `player_fire_candidate_trace` | Earlier read-only trace of PlayerRescue candidate position for spread and fire exposure | Player vtable/getter exports, FireRescue callback evidence, scene viewer | Superseded by `player_position_getter`; the earlier claim that PlayerRescue's slot mapping was unproven is corrected by current-hash vtable bytes |
| `fire_48_subobject` | Trace FireRescue root +0x48 construction and default state | `0044a330_firerescue_constructor.c`, `0046f930_FUN_0046f930.c`, startup initialization | Complete; read-only; found the shared empty-string pointer; initializer setter chain was completed with retail disassembly |
| `fire_gate_writers` | Trace writes to the spread gate and shared state | `00448320_fresh.c`, `00447A90_fresh.c`, `0046e320_FUN_0046e320.c`, `00448d60_FUN_00448d60.c` | Complete; read-only; initial shared-state interpretation was superseded after resolving 00448D60's string setter |
| `fire_name_gate` | Map the spread gate to authored FireRescue A_Name | `00447A90_fresh.c`, `00448320_fresh.c`, FireRescue UserData audit, retail 00448D60 disassembly | Complete; root follow-up proved the embedded string receives A_Name and the gate reads its length; unnamed generated children retain an empty string |
| `player_position_getter` | Read-only trace of PlayerRescue candidate vtable slot +0x0C | `memory_004e1c40_player_rescue_vtables.json`, `00453d50_player_rescue_object_constructor_fresh.c`, archived 0049DB90/0049D790 exports | Complete; no edits; current-hash slot maps to 0049DB90; old body export supports transformed XYZ, while getter bytes and fourth float remain unrefreshed |
| `bank_door_audit` | Audit the reported Bank M-only door symptom against live loop and controlled harness | `scene_viewer.cpp`, `bmodel_door_runtime_port.md`, Bank interaction evidence | Complete; no edits; M/N only affect inspection state; continuous aimed route opens PATH but hits static world model 0 at a 24-unit lateral offset; aligned control crosses; live route remains open |
| `bank_collision_retail` | Trace retail swept collision and BModel transform path against Bank door result | Pinned EXE/Genesis exports, Bank entity/model inventory, `bmodel_door_runtime_port.md` | Complete; read-only; swept hull checks static and transformed models, BModel PATH updates Genesis model transforms; exact leaf and live route remain open |
| `bank_contact_review` | Audit Bank live-loop contact/PATH ordering and reported M workaround | `scene_viewer.cpp`, Bank door route and interaction fixture | Complete; read-only; no M-to-collision path; live ordinary route still needs capture |
| `bank_retail_collision_trace` | Resolve retail Player hull and swept collision path for Bank obstruction | Pinned EXE/Genesis exports, `core.h`, Bank door evidence | Complete; read-only; retail mins `(-30,-30,-30)`/maxs `(30,85,30)` match the port; exact leaf and live route remain open |
| `mesh_metadata_audit` | Independently audit world-mesh face-span source correspondence | Exporter, geometry/world-mesh JSON, C++ loader | Complete; read-only; validated field mapping and recommended all-map metadata checks; no renderer change |
| `retail_material_trace` | Search pinned retail evidence for TexInfo alpha and lightmap render semantics | Ghidra exports, material/driver evidence, Bank and Training exports | Complete; read-only; no live Ghidra instance and no source-backed blend/depth/lightmap interpretation found |
| `next_gameplay_slice` | Select a reusable authored campaign mechanic and trace its script route | Gas R BSP/EVS, `gas_r_button1_use_path.md`, `gas_r_script_variable_queue_path.md`, shared viewer host | Complete; no edits; recommended generic FireRescue `setsize` support for Gas R button1's six authored commands |
| `fire_setsize_audit` | Independently audit retail timed-size semantics and the shared host seam | `00449A80_fresh.c`, `00447920_fresh.c`, `0044B620_fresh.c`, `0044AE80_fresh.c`, `scene_viewer.cpp`, `fire_size.h` | Complete; read-only; confirmed key reset/insert/interpolation and timestamp ordering; highlighted existing past-time key insertion bug fixed in this checkpoint |

- **Genesis driver reuse audit:** public Genesis3D's shared `Dcommon.h`
  exposes a `geBoolean` out-pointer hook (`DRV_Driver **`), `_fastcall`
  callbacks, and table version 100/3. FDNY checks 101/1 and uses observed
  table slots at `+0x14/+0x18/+0x20/+0xA8`; public-source field names support
  the initial enumeration/init roles but later offsets and source identity
  remain unresolved. Public source is an API/algorithm reference only; no code
  was copied. See
  [`public_source_reuse_audit.md`](../tracker/evidence/driver-boundary/public_source_reuse_audit.md),
  [`ENGINE_SOURCE_REUSE.md`](ENGINE_SOURCE_REUSE.md) and
  [`public_driver_comparison.md`](../tracker/evidence/driver-boundary/public_driver_comparison.md).
- **Rescue integration audit:** the highest-value shared seam is the
  level-neutral loader/catalog resolver. The old renderer and collision
  consumers can use the exported schemas; the Training host's startup,
  scripts, actor catalog, and Apple UI remain specific to that workbench. The
  new `SceneData` package loader and viewer implement the first half of this
  recommendation. The shared escape transaction now runs through MBase's
  authored EVS route in a synthetic check; complete rescue gameplay remains
  open.

## Data ledgers

- `tracker/scene_load_coverage.csv`: generated by building
  `fdny-scene-viewer` and running `python3 tools/build_scene_load_coverage.py`.
  It records what the C++ package loader actually opened for each local map.
- `tracker/level_map_inventory.csv`: catalog and BSP relationship, entity
  counts, file hashes, and exporter coverage.
- `tracker/function_reviews.csv` and `tracker/progress_history.csv`: function
  review activity. `dashboard/index.html` reads these CSVs when opened; update
  the CSV data to show progress without regenerating the HTML.
- `tracker/genesis_function_reviews.csv`: hash-pinned Genesis.dll engine
  decompilation ledger; separate from the EXE-only dashboard activity count.
