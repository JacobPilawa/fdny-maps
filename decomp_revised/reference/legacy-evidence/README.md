# Curated legacy evidence

These are selected research artifacts copied from the earlier local
`FDNY_trackdown` work so the revised findings and their supporting audit trail
remain readable in a clean clone. They are historical analysis, not recovered
original source. The repository's root `decomp/` directory remains the full
archival decompilation snapshot.

- `fire-update/scheduler-registration-audit.md` and
  `fire-update/training-fire-dispatch-order-audit.md` record bounded earlier
  searches for the recurring FireRescue Think callback. They justify reusing
  the negative result instead of repeating broad scans.
- `rng/004b9400_FUN_004b9400.c` is the earlier same-address Ghidra pseudocode
  for the float-to-integer rounding helper used by the new timing-key trace.

- `rescue-moveto/` contains one selected old export for each useful function
  address in the movement trace. The new hash-pinned work keeps separate fresh
  decompilations and assembly listings so a legacy export cannot be mistaken
  for original source or for the current Ghidra database output.
