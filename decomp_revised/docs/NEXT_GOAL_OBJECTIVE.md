# FDNY Firefighters: American Hero — Porting Goal

Work in `/Users/jacobpilawa/Documents/ChatGPT/fdny_decomp`.

Continue the full, faithful 1:1 gameplay port of the retail game. Keep improving the playable build, cover every rescue mission, and prioritize core mechanics and mission progression. Build for macOS first while keeping shared code cross-platform. Do not turn this into a Training-only remake or a separate visual viewer.

Start by checking the current checkout, `docs/WORK_QUEUE.md`, relevant evidence and build notes, and recent Codex conversations. Reuse existing work and identify unfinished handoffs that should be cleaned up. Treat current files and verified tool results as authoritative; confirm old claims before relying on them. The work queue records current technical status and open questions.

Trace retail behavior and authored game data with Ghidra when useful, then implement findings in the shared port. Record binary hashes and evidence, and label uncertain timing or behavior as provisional. Make steady playable progress and distinguish controlled checks from retail parity.

After resolving any urgent loose ends, focus on these areas according to current evidence and impact:

1. Held equipment: verify Axe, extinguisher, and other hand models have the correct orientation, animations, sounds, selection, and working actions.
2. Rendering: bring lighting, textures, materials, effects, and HUD presentation in line with retail.
3. Audio: trace and add the retail background music and its level/menu playback behavior.
4. Front end: replace the startup scene-preview popup with a main menu and level-selection flow.
5. Mission lifecycle: implement and verify clear level-start, objective, completion, and level-end/transition behavior.

Use a few `gpt-6-luna` subagents for independent, bounded tasks when helpful. Avoid overlapping edits; keep a small ledger of each scope, files, and status, and integrate useful findings into the work queue and evidence notes. Ask no optional questions that would halt progress: make reasonable assumptions, record them, and continue. Ask only if a missing answer would materially change the work or make safe progress impossible.

Do not use computer-use or UI automation. Use project files, command-line tools, Ghidra, and other available non-UI interfaces.

Occasionally checkpoint reviewed project work to [JacobPilawa/fdny-maps](https://github.com/JacobPilawa/fdny-maps), under `decomp_revised/` on `main`, preserving the existing `decomp/`. Use the documented `python3 tools/sync_github.py` workflow to compare before publishing; keep retail binaries and local Ghidra/build data out of the mirror, and verify the resulting remote commit.

Update handoff notes and tracker CSVs at meaningful checkpoints. If either the 5-hour or weekly usage limit reaches 1% remaining or less, stop work, pause the goal, and leave a concise handoff.
