# Continue FDNY Firefighters: American Hero decompilation and port

Work in:

`/Users/jacobpilawa/Documents/ChatGPT/fdny_decomp`

Begin by inspecting the current checkout and reading `docs/WORK_QUEUE.md`, the
relevant evidence notes, and the current build/source state. Treat current files
and tool results as authoritative. Verify all inherited details before acting.
Use Ghidra for reverse engineering when it materially helps; preserve the
source binary hash, distinguish new analysis from reused exports, and keep
unverified timing or engine behavior labeled provisional.

## Goal

Reverse engineer and port the full retail game so its maps, rescue missions,
mechanics, scripts, and engine behavior work faithfully in a modern
cross-platform build. Prioritize a playable macOS build while keeping shared
gameplay code portable. Every rescue mission remains in scope; after that,
continue through the rest of the game catalog. This is a full gameplay port,
not a Training-only remake or a separate visual preview.

## Current state to verify

- The project now has a map-neutral scene host and a rescue mission selector.
  The package set loads all eight retail rescue choices and 13 rescue/stage
  packages. The current rescue-all packaged executable SHA-256 is
  `440b04d12211b499332ab6e7c4fb6d44fa929ff7a70abba623fb9710982476f5`.
  Global EVS variable names and `if` value comparisons now follow retail's
  case-insensitive behavior; the bundled 13-package rescue checks resolve the
  authored mixed-case bonus references.
  Packaged `--rescue-check` drives every authored victim on each of the eight
  primary maps through a mapped pickup message and an authored escape touch.
  These controlled transactions do not prove natural travel/contact,
  scheduling, full mission completion, or retail parity. Bank and Subway A
  also pass controlled door and trigger checks.
- The shared port has grounded BSP movement/collision, authored victim spawn
  and roaming behavior, contact pickup, rescue/exit transactions, fire state
  and suppression paths, equipment/goggles previews, script dispatch,
  BoundsTrigger/PropModel/BModel contact, and PATH-driven door state. Continue
  generalizing these into a complete rescue flow. Keep controlled or synthetic
  checks distinct from interactive retail comparisons.
- The shared runtime's clock and extended-precision paths now use portable
  128-bit arithmetic, with native and forced-fallback macOS builds. A native
  arithmetic parity harness passed 210,000 comparisons, and the fallback build
  passed controlled rescue checks on all 13 rescue/stage/Training packages.
  Linux and Windows builds and interactive behavior remain unverified. See
  `tracker/evidence/runtime/cmake_platform_boundary_2026-10-15.md`.
- The shared FireRescue host now resolves authored `moveto` PathPoints and
  advances the source actor using the mapped distance/progress rule. For the
  authored MBase and Gas R routes, it now applies the verified `H_Radius/2`
  visual-update gate for no-`C_Model` fires. Deferred route-trail FireRescue
  creation remains open: retail creates those children through a later generic
  spawn drain, but per-instance deactivation/reuse and recurring update
  ownership are unresolved. FireRescue child creation and its `0x7531` handler
  now run at the host's post-candidate spawn/message boundary. Factory evidence
  shows BModel, BoundsTrigger, and PropModel candidates precede FireRescue in
  retail, while the host still evaluates FireRescue first; exact callback and
  ScriptActor drain order remains open. See
  `tracker/evidence/scenario-loading/contact_manager_factory_order_audit_2026-10-15.md`.
  Trailing timing-key effects, exact transform/floor behavior, and callback
  cadence remain open.
  Named-fire spread now maintains retail's per-fire previous proximity and
  shared nearby-fire counter transitions during the ordered contact pass;
  already-counted terminal fires stay counted while resident. Teardown/reuse, full candidate
  membership, and natural spread timing remain open. See
  `tracker/evidence/fire-spread/proximity_count_port_2026-10-15.md`.
  Current-hash PlayerRescue getter
  disassembly confirms `Player::p` is the retail candidate origin; the fourth
  fire-exposure value and callback cadence remain open. See
  `tracker/evidence/rescue-moveto/README.md` and
  `tracker/evidence/fire-spread/player_rescue_candidate_getter.md`.
- The user reports Bank framerate now seems sorted. Treat that usability issue
  as resolved by report; retail-versus-port frame-time parity is still
  unmeasured. The Training red trigger-volume render gate has been corrected
  from retail model state and was previously offscreen-verified; fog is a
  separate missing renderer state.
- Recent evidence identifies oxygen as a broad missing shared mechanic:
  authored `oxyon`/`oxyoff` commands occur across all seven campaign rescues
  and linked stages. Retail initializes the resource at 1.0, delays drain by
  700 ms, removes 0.05 every three seconds, refills at frame delta × 0.1, and
  routes depletion through Player passout argument 0. The shared viewer
  implements those commands, resource/timer state, warning gate, and the
  argument-0 passout route. It draws the gauge and needle from bundled retail
  sprite bitmaps, and packaged `--oxygen-hud-render-check` passes. Retail
  visual/blend parity remains open. The one-shot `RC_OxygenLow` warning now
  plays the source RRF's `RC_OxygenLow0`/`o2low.wav` through the shared audio
  path, bundled in all 13 rescue/stage packages. The separate `+0x2160` effect
  now uses the source-mapped resource-702 shield animation and
  `VisorUpDown`/`VisorLock` sounds; its timing and overlay were checked from a
  packaged scene. The low-oxygen orange gauge overlay is also source-mapped and
  ported; the host maps its controller range to a small alpha ramp as an
  approximation. The category-volume portions of selectors 4/5 now drive the
  shared mixer's groups 1 and 2; the manager flag, separate streamed music
  channel, retail audio/blend parity, warning blend behavior, and update cadence
  remain open. Player-owned `Breathe` and
  `OxyRelease` sound events are also ported and bundled in all 13 packages;
  mixer output parity remains open. See
  `tracker/evidence/scenario-loading/player_oxygen_resource_and_script_path_2026-10-14.md`
  and
  `tracker/evidence/scenario-loading/player_oxygen_low_warning_sound_port_2026-10-15.md`
  and
  `tracker/evidence/scenario-loading/player_oxygen_breathe_release_sound_port_2026-10-15.md`
  and
  `tracker/evidence/scenario-loading/player_oxygen_visor_effect_port_2026-10-15.md`.
  See `tracker/evidence/scenario-loading/player_oxygen_audio_category_mix_port_2026-10-15.md`.
  See `tracker/evidence/scenario-loading/player_oxygen_low_warning_visual_port_2026-10-15.md`.
- Victim pickup, rescue, and authored exits now have shared transaction paths.
  Carry presentation has no source-backed Player-relative body transform yet.
  The independent Player status icon now uses retail resource 55 and its
  source-traced center-to-corner animation; the packaged render and rescue
  checks pass across all 13 Training/rescue packages. Blend, callback cadence,
  and carried-body presentation remain open. See
  `tracker/evidence/scenario-loading/player_victim_pickup_status_hud_2026-10-15.md`.
  Retail `finalexit` success now opens the exit gate and refreshes the first
  owned item; incomplete `finalexit` and `finalexitloadmap` hints last seven
  seconds. Bank/Boat B and linked-stage checks verify these gates in controlled
  fixtures. Natural exit contact, transition context identity, and retail
  timing remain open; see
  `tracker/evidence/scenario-loading/rescue_completion_and_exit_path.md`.
  State-6 recovery now waits strictly beyond 1000 ms and runs the existing
  passout/drop transaction. The viewer maps the separate safe snapshot `+0x3400`
  and reset checkpoint `+0x534`, their oxygen-gated writers, carried-victim
  drop, floor placement, and status message. The one-second camera blend now
  runs through the source-shaped view helper; equipped-item offsets, callback
  cadence, and retail render parity remain unverified. Global mode is fixed at
  0, so mode-2
  HeroDown spawning is unsupported. The eight-map check exercises a controlled
  passout/drop/reset/re-pickup, not natural fire/oxygen recovery or retail
  parity. See
  `tracker/evidence/scenario-loading/player_passout_recovery_port_2026-10-14.md`.
  Controlled `finalexitloadmap` checks now exercise all six authored touch
  bindings, verify the remaining-survivor gate, and load each requested sibling
  package; all 13 bundled rescue/stage packages pass `--rescue-check`. Natural
  exit contact, trigger timing, pending context identity, and full staged
  mission completion remain open. See
  `tracker/evidence/scenario-loading/rescue_stage_transition_port_2026-10-14.md`.
  Held-tool pose/audio, other HUD details, damage feedback, fog, and engine timing
  also remain open or provisional. Use the current work queue for specific scope
  and avoid reopening items already marked resolved without new evidence.
- The active dirty-space glitch subgoal concerns the original retail game's
  Player/Axe coordinate flow in invalid or out-of-bounds areas on MBase, Boat,
  and Gas Station, following the user's report of an Axe visual near victims
  or spreading fire and an apparent Player warp. Source evidence maps a Player
  downward-sweep hit writing to canonical translation, an Axe sweep derived
  from Player frame `+0x34A4`, and Axe-transform detachment after a successful
  world hit. Convex world-model leaf volumes with contents intersecting mask
  `0x43` reach a nominal 128-unit envelope past reported bounds on all four map
  groups, within small floating-point differences. Same-family GBSPLib source
  deliberately builds outside solid leaves; its padded root bounds serve
  frustum culling and do not alone cause collision. The retail model-bound
  broadphase and leaf planes still need to be involved, and the incident match
  remains unknown. A collision latch
  `+0x12C` can gate an explicit Player `escape` checkpoint capture, but no
  direct Axe-hit-to-Player write or matching retail runtime trace is
  established. Axe also has two model children: root gate `+0x1FC` starts
  false, and ordinary first-time selection activates only the model-1
  secondary. The only direct primary-gate caller found is the duplicate
  `equip axe` path; Boat A/B startup scripts each author one Axe equip. If that
  later path enables model 6, a world hit with no accepted candidate can leave
  the detached primary visible at its cached transform while model 1 stays
  attached. This is conditional source behavior, with runtime gate state
  unknown. The Axe candidate scan does not use a hit
  BModel as the Axe parent or write Player coordinates; the reviewed Boat
  Axe-eligible BModels have no authored `chop`/`break` handlers. Runtime script
  attachments remain open. Boat B's hook/ladder remain additional conditional
  candidates.
  Do not implement a behavioral workaround until the retail coordinate write
  that matches the report is observed. See
  `tracker/evidence/scenario-loading/dirty_space_axe_warp_trace_2026-10-14.md`
  and the active glitch section in `docs/WORK_QUEUE.md`.
- The public GitHub mirror is
  `https://github.com/JacobPilawa/fdny-maps`, with the active project under
  `decomp_revised/` and the older `decomp/` preserved. A read-only check on
  2026-10-14 found remote `main` at `367b8c762adf639f13b1afec4c7ae3b7fa3a9d19`;
  this checkout has no remote and its local snapshot `a9e5c49` records the
  earlier `d830a4c` baseline. The expanded local trace and handoff edits are
  not on GitHub. A prior publish was rejected by automatic review, and the
  restriction is not confirmed resolved, so do not retry or work around it.
  When resolved, use `python3 tools/sync_github.py` to compare and publish
  through the correct subdirectory mapping. Retail inputs and Ghidra project
  containers remain local and ignored.

## How to continue

- Use `docs/WORK_QUEUE.md` and the evidence ledger to choose the next
  high-value, source-backed gap. Favor shared mechanics used across multiple
  rescue missions and meaningful playable progress. Work on actual behavior:
  victims, fire, tools, hazards, triggers, doors, mission transitions, rescue
  objectives, and renderer/engine behavior needed by those systems.
- Trace retail code and authored data before implementing. Keep evidence
  provenance explicit: verified, inferred, provisional, or open. Do not copy
  the `FDNY_trackdown/map_explorer/` viewer, controls, filters, collision
  overlays, or assumptions into the port.
- Keep new work self-contained in this project and prefer copied inputs under
  `inputs/original/`. Reuse old workbench code only after checking its scope
  and provenance.
- Use one or two `gpt-6-luna` subagents for independent tasks when useful.
  Avoid overlapping edits, keep a small ledger of scopes/files/status, stop
  completed agents, and integrate findings into the main queue.
- Keep handoff notes current. The dashboard already reads tracker CSVs; update
  them at meaningful checkpoints and avoid routine dashboard HTML edits.
- Use existing targeted checks to support requested verification; do not add
  or run unrelated tests. Avoid computer-use/UI automation unless essential.
  The user can launch the original or provide captures if a live retail
  comparison becomes necessary.
- Ask questions only when missing information would materially change the
  implementation. Continue independent work while optional clarification is
  pending.
- Check the account's 5-hour and weekly usage during extended work. If either
  has 0% remaining, stop immediately, pause the goal, and leave a concise
  handoff.

## Completion target

Deliver a faithful, selectable, playable rescue campaign across all rescue
missions, then continue toward full-catalog and cross-platform parity. Report
what changed, what evidence supports it, how it was checked, and what remains
unverified. Keep the goal active until the requested objective is achieved or
the user explicitly ends it.
