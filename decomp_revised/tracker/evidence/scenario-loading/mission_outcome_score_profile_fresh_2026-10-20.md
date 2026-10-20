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
`00423FA0` creates a per-level score record with its own stat collection, and
`004243F0` serializes the `root`/`Player` tree to the current profile path. The
preserved inputs contain no sample save, so concrete on-disk values and all
nested serialization details remain unverified.

On the win-only path, `00424650` looks up or creates the selected level's
score record, copies the live stat collection from manager `+0x30` into that
record through `00488710`, marks the record active, then calls `004243F0` to
save. `00488710` walks the stat entries by key, creates missing saved entries,
and merges existing values using the selector stored at `+0x18`: selector 0
keeps the smaller value, selector 1 keeps the larger value, and other selectors
overwrite. This joins the 13 registered stat keys to the values in the saved
record.

`004332E0` retrieves that same saved stat collection for the selected level,
groups entries by registration ordinal, and emits each stored name with up to
two formatted child values. Its typed value formatter is `00486EC0`. Thus the
registered keys are the Records-page row source, with the raw type selectors
and best-value merge behavior above. The saved inputs do not include a sample
profile, so concrete stored values, full nested serialization details, and
final pixel placement remain unverified.

The shared macOS port currently has an in-memory rescue-stat map but no
profile-backed Records page model or menu route. The title-menu action still
reports the Scores/Options page as unported. Keep menu implementation paused
per the existing work-queue priority; continue tracing and integrating the
remaining gameplay statistic producers so all 13 saved values can be populated.
The port now adds `Fires Doused` only from the terminal `finalize=1` HoseSpray
transition into FireRescue mode 5. The separate extinguisher message route
passes `finalize=0`, and PlayerHose's WindowFire timer path does not invoke
shared FireRescue suppression, so neither increments that rescue statistic.
The value is still in-memory only; profile serialization and the Records page
remain unported. This static data-flow trace is not a visual or retail-parity
result. Result timing, status replay cadence, profile serialization, and
natural mission completion remain separate open issues.

A subsequent hash-pinned producer pass also integrated `Completion Time` using
the retail `frameDelta × 1000` contribution and `Gallons Sprayed` using the same
contribution only during the selected slot-4 HoseSpray action. A focused
follow-up traced the Player `+0x343C` remaining-seconds countdown, its timeout
write of state 2 to Player `+0x3438`, and the Air Time gate: nonnegative
remaining time with Player `+0x12C` clear. The shared host now carries the
countdown, timeout gate, and in-memory Air Time contribution. Arithmetic and
field gates are source-backed; host callback cadence, movement-latch timing,
and the presentation unit implied by “Gallons” remain provisional. The
remaining unported stat producers are `Jumps`, `Total Steps`, and
`Objects Thrown`. Their retail producer sites are now identified, while the
user-input bridge and state gates for `Jumps`, the sound-selection cadence for
`Total Steps`, and integration of the held-BModel trajectory/drop lifecycle
for `Objects Thrown` still block a source-matched host transaction. See
[`mission_stat_producers_elapsed_hose_2026-10-20.md`](mission_stat_producers_elapsed_hose_2026-10-20.md).

## Shared port checkpoint

`scene_viewer.cpp` initializes the in-memory `Fires Doused` counter and adds one
when `FireSuppression::spray` takes its strict below-0.1 terminal branch from
`finalize=1`. The source helper returns immediately after mode 5, so a later
update cannot count the same FireRescue twice. This mirrors the stat update in
`00448970`; it does not add profile persistence, and the source update is gated
by the HoseSpray contact reaching influence at least 0.01. No test or runtime
parity check was performed for this checkpoint.

The rescue-all macOS app rebuilt successfully with matching standalone and
packaged executable SHA-256
`90158fb8df2b516afa70a0a508f5bfd60c62a8fb062c9ae9ebe7dd21af666481`.
`Info.plist` validation passed and the source-ordered rescue catalog contains
eight choices. This is a compile/package check only; no gameplay test,
interactive run, or retail comparison was performed.

## Next evidence step

Finish the three remaining stat integrations against the shared host: map the
JUMP action's runtime poll route and the state-5 command token, establish the
host cadence for the Total Steps sound-selection gate, and promote the
source-shaped held-BModel throw transaction from the Training spike into the
shared mission scene with the callback-owned trajectory substep, drop-event,
sound, and timer services. The direct `0043DC60` → `0043D330` route is mapped;
the outer update cadence and data-driven event effects remain open. Separately
map the exact serialized scalar/container encoding before adding profile
persistence. Keep the result transition gated on separate runtime evidence,
and return to the Records UI after core gameplay coverage improves.
