# Continue FDNY Firefighters: American Hero decompilation and port

Work in:

`decomp_revised`

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
  The last recorded package loaded all eight retail rescue choices and 13
  rescue/stage packages. Controlled checks covered Training and the seven
  campaign rescue maps; they do not prove ordinary retail scheduling or full
  mission parity. The current rescue-all packaged executable SHA-256 is
  `5bbb5190739afcd26c219e961bd75bd128d463fed91d445d9a68fca083d76dff`.
  The packaged catalog check loads all eight rescue choices, and the existing
  synthetic rescue check passes on each primary rescue package. Packaged
  interaction checks for Bank and Subway A also pass their controlled door and
  trigger routes. These checks do not exercise oxygen or ordinary live contact
  scheduling.
- The shared port has grounded BSP movement/collision, authored victim spawn
  and roaming behavior, contact pickup, rescue/exit transactions, fire state
  and suppression paths, equipment/goggles previews, script dispatch,
  BoundsTrigger/PropModel/BModel contact, and PATH-driven door state. Continue
  generalizing these into a complete rescue flow. Keep controlled or synthetic
  checks distinct from interactive retail comparisons.
- The user reports Bank framerate now seems sorted. Treat that usability issue
  as resolved by report; retail-versus-port frame-time parity is still
  unmeasured. The Training red trigger-volume render gate has been corrected
  from retail model state and was previously offscreen-verified; fog is a
  separate missing renderer state.
- Recent evidence identifies oxygen as a broad missing shared mechanic:
  authored `oxyon`/`oxyoff` commands occur across all seven campaign rescues
  and linked stages. Retail initializes the resource at 1.0, delays drain by
  700 ms, removes 0.05 every three seconds, refills at frame delta × 0.1, and
  routes depletion through Player passout argument 0. The shared viewer now
  implements those commands, resource/timer state, warning gate, and the
  argument-0 passout route. Retail update cadence, oxygen gauge/status
  rendering, and subordinate oxygen effects remain open. See
  `tracker/evidence/scenario-loading/player_oxygen_resource_and_script_path_2026-10-14.md`.
- Victim pickup, rescue, and authored exits now have shared transaction paths.
  Carry presentation has no source-backed Player-relative body transform yet.
  The existing passout/drop helper is not wired into the viewer; its mapped
  carried-victim drop is on the timed Player state-6 completion route. The
  viewer lacks Player position `+0x3400` mapping and required passout host
  callbacks, including cleanup and status services. Held-tool pose/audio, HUD
  details, damage feedback, fog, and engine timing also remain open or
  provisional. Use the current work queue for specific scope and avoid
  reopening items already marked resolved without new evidence.
- The public GitHub mirror is
  `https://github.com/JacobPilawa/fdny-maps`, with the active project under
  `decomp_revised/` and the older `decomp/` preserved. The October 14 sync
  reconciles the local work after `e785aec`; all 607 files from that remote
  snapshot were present locally. Use `python3 tools/sync_github.py` to compare
  and add `--push` to publish through the correct subdirectory mapping.
  Retail inputs and Ghidra project containers remain local and ignored.

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
