# Review of the previous FDNY work

The revised workspace was informed by a separate local research archive. That archive contained Ghidra exports, Training script/startup audits, a partial native Training prototype, and mechanics notes. It is not duplicated into this public subtree. The repository's pre-existing [`../decomp/`](../../decomp/) folder remains unchanged as the public historical archive.

## Carry-over approach

- Promote address-specific findings into this revised workspace only when they answer an active mapping question.
- Treat Ghidra names and pseudocode as hypotheses; preserve addresses, raw instructions, callers, and data references.
- Treat native prototype tests as tests of that reconstruction, not captures from the original process.
- Keep static command branches separate from claims that a command or event executes in the original game.
- Track function recovery, component comparison, and integrated behavior as separate milestones.

`tracker/legacy_crosswalk.csv` retains path strings from the local legacy export archive for provenance. Those old exports are not mirrored here; the fresh address-specific evidence is under `tracker/evidence/`.
