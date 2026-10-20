# FDNY Firefighters: American Hero — Porting Goal

Work in `decomp_revised`.

Continue working toward a full, faithful 1:1 reproduction of the retail game, including its missions, gameplay, presentation, and engine behavior. Keep the playable build progressing, cover every rescue mission, and build for macOS first while keeping shared code cross-platform. Do not turn this into a Training-only remake or a separate visual viewer.

Start by checking the current checkout, `docs/WORK_QUEUE.md`, relevant evidence and build notes, and recent Codex conversations. Reuse existing work and identify unfinished handoffs that should be cleaned up. Treat current files and verified tool results as authoritative; confirm old claims before relying on them. The work queue records current technical status and open questions.

Use the work queue and evidence notes to choose the next useful step toward the full reproduction. Resolve urgent loose ends where practical, then make steady progress on the highest-value gaps supported by current evidence. Trace retail behavior and authored game data with Ghidra when useful, then implement findings in the shared port. Record binary hashes and evidence, label uncertain timing or behavior as provisional, and distinguish controlled checks from retail parity.

Use a few `gpt-6-luna` subagents for independent, bounded tasks when helpful. Avoid overlapping edits; keep a small ledger of each scope, files, and status, and integrate useful findings into the work queue and evidence notes. Ask no optional questions that would halt progress: make reasonable assumptions, record them, and continue. Ask only if a missing answer would materially change the work or make safe progress impossible.

Do not use computer-use or UI automation. Use project files, command-line tools, Ghidra, and other available non-UI interfaces.

Occasionally checkpoint reviewed project work to [JacobPilawa/fdny-maps](https://github.com/JacobPilawa/fdny-maps), under `decomp_revised/` on `main`, preserving the existing `decomp/`. Use the documented `python3 tools/sync_github.py` workflow to compare before publishing; keep retail binaries and local Ghidra/build data out of the mirror, and verify the resulting remote commit.

Update handoff notes and tracker CSVs at meaningful checkpoints. If either the 5-hour or weekly usage limit reaches 1% remaining or less, stop work, pause the goal, and leave a concise handoff.
