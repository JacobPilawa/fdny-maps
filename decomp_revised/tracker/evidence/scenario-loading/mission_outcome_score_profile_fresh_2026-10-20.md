# Mission stat catalog and saved score profile trace

## Scope and provenance

This is a static, read-only follow-up to the Records page trace. The input
`inputs/original/binaries/FDNYFirefighter.exe` has SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The exports below were produced with Ghidra 12.0.4 from
`.local/mission-lifecycle-ghidra/FDNYPort` using
`DecompileTargetAndRefs.java`; the project was opened read-only. The function
bodies and current-hash identity are saved in
[`mission_outcome_score_profile_fresh_2026-10-20.c`](mission_outcome_score_profile_fresh_2026-10-20.c).

## What the static trace establishes

`0042AC0A` calls `00424710` during level-load setup. With its flag set,
`00424710` builds the stat-definition collection at object `+0x30` and
registers 13 named entries through `00488660`:

| Order | Key | Raw `(type, mode, initial, aux +0x18, aux +0x28)` arguments |
| ---: | --- | --- |
| 0 | Completion Time | `(1, 0, 0, 0, 0)` |
| 1 | Bonus Points | `(0, 0, 0, 1, 0)` |
| 2 | Rescues | `(0, 0, 0, 1, 0)` |
| 3 | Shortest Rescue Time | `(1, 1, 999999999, 0, 0)` |
| 4 | Longest Rescue Time | `(1, 2, 0, 0, 0)` |
| 5 | Axe Hits | `(0, 0, 0, 1, 0)` |
| 6 | Gallons Sprayed | `(2, 0, 0, 1, 0)` |
| 7 | Objects Thrown | `(0, 0, 0, 1, 0)` |
| 8 | Air Time | `(1, 0, 0, 1, 0)` |
| 9 | Fires Doused | `(0, 0, 0, 1, 0)` |
| 10 | Serious Burns | `(0, 0, 0, 1, 0)` |
| 11 | Jumps | `(0, 0, 0, 1, 0)` |
| 12 | Total Steps | `(0, 0, 0, 0, 0)` |

The tuple is kept in raw constructor-argument order after the key. `00488660`
stores the name at descriptor `+0x1C`, type at `+0x10`, mode at `+0x14`,
initial/current value at `+0x24`, auxiliary arguments at `+0x18` and `+0x28`,
and registration order at `+0x20`. These offsets and raw codes do not by
themselves establish presentation labels, units, or which fields are visible
on the Records page.

The save path confirms a named `Score` structure. `00424DC0` writes one
score-record object's `Name`, `LevelIndex`, and `DifficultyLevel` fields, then
iterates its `Score` entries. `00425990` reads the `Score` root and recognizes
`LevelName` and `stats` children. `00425D50` reads the `Player` root's `Name`,
`LevelIndex`, and `DifficultyLevel` fields and dispatches its `Score` child.
This identifies the relevant save-tree nodes, but there is no sample save in
the preserved input set to verify its concrete values or complete nested
layout.

Combined with `004332E0`, the page builder looks up the selected level key in
the manager's saved score collection, iterates stored string entries, groups
them by descriptor order, and emits colored headings plus up to two formatted
child values per group. Its typed value formatter is `00486EC0`. The current
exports do not yet prove that the 13 registered mission-stat keys map one for
one to the strings rendered by the Records page, or which records/categories
are populated for each mission and difficulty.

The shared macOS port currently has an in-memory rescue-stat map but no
profile-backed Records page model or menu route. Therefore this trace supports
the catalog/save-tree model as the next implementation target; it does not
support inventing rows or connecting the unresolved win/loss transition. The
page's text grouping is static source evidence, not a presentation or retail
parity result. Result timing, state-2 producer, message-gate lifecycle, and
natural mission completion remain separate open issues.

## Next evidence step

Correlate the stat descriptor keys from `00424710` with values written into
the serialized `stats` child, including score history ordering and grouping.
If no concrete saved profile is available, preserve the known raw schema and
leave the unresolved join explicit. Add a shared profile-backed Records page
only after the row/value source and frontend entry route are established; keep
the result transition gated on separate runtime evidence.
