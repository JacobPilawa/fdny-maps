# Review of the previous FDNY work

## What was useful

The previous-work tree remains preserved in the repository's root-level [`decomp/`](../../decomp/) archive. It contains function inventories, Ghidra exports, Training script and startup audits, a partial native Training prototype, and focused mechanics notes. A local working copy was used to review it; the revised project keeps only curated evidence and links back to the archive.

The most useful material for this phase is the address-level Ghidra evidence in:

- ../../decomp/evidence/training-script-command-audit.md
- ../../decomp/evidence/training-script-queue-audit.md
- ../../decomp/evidence/training-startup-integration.md
- ../../decomp/evidence/startup-world/
- ../../decomp/evidence/world-script/
- ../../decomp/evidence/entity-scheduler/
- ../../decomp/evidence/entity-load/

The main script and startup notes identify relevant functions and authored Training commands. The Ghidra outputs are valuable leads because they are tied to the same EXE SHA-256 now pinned in inputs/original. This turn refreshed the selected level-to-EVS functions in the active evidence directory.

## How the old work is being used

- Keep the previous project as a reference archive; do not bulk-copy its prototype or all duplicate reports into the active port.
- Promote a small number of hash-matched function decompilations into tracker/evidence when they answer an active mapping question.
- Treat native tests and startup gates as tests of the native reconstruction. They are not original-process captures and do not prove integrated original timing.
- Treat repeated decompilation copies in nested audit folders as one source of evidence, not independent corroboration.
- Keep static command branches separate from claims that a command or event actually executes in Training.
- Keep the archived Genesis3D source reference separate from the port until binary matching and license review support a reuse decision.

## Carry-over decision

For the current level-to-EVS mapping batch, the active project contains curated, fresh Ghidra outputs for the level path helper, level selection/start, EVS loader, mode wrappers, parser, command dispatcher, and delayed callback. The rest of the old notes remain searchable in the root `decomp/` archive. The partial native implementation can be audited later as a candidate source of component boundaries after the original function map and cross-platform service choices are clearer.
