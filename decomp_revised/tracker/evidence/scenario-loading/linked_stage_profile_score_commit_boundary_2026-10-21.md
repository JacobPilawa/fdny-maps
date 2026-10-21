# Linked-stage profile score commit boundary

## Provenance

Static read-only Ghidra evidence for `inputs/original/binaries/FDNYFirefighter.exe`,
SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The executable hash was checked against the input file. The relevant pseudocode
and instruction exports are
[`0042aea0_evergame_pending_load_update_fresh.c`](0042aea0_evergame_pending_load_update_fresh.c),
[`0042aea0_evergame_pending_load_update_fresh.asm`](0042aea0_evergame_pending_load_update_fresh.asm),
[`0042ab00_process_level_load_request_fresh.c`](0042ab00_process_level_load_request_fresh.c),
and
[`0042ab00_process_level_load_request_fresh.asm`](0042ab00_process_level_load_request_fresh.asm).
No retail runtime was captured.

## Direct control-flow boundary

The Player `finalexitloadmap` handler (`00450A70`) queues a pending level
request through `0042A260`. On the next `ProcessEverGameUpdate` (`0042AEA0`),
the pending byte at EverGame `+0x68C` is consumed before result handling; this
calls `ProcessLevelLoadRequest` (`0042AB00`) with the request block at `+0x660`.
The active-record request has fallback byte zero, so the loader skips the
conditional stat reinitialization described in
[`linked_stage_stat_collection_lifetime_2026-10-21.md`](linked_stage_stat_collection_lifetime_2026-10-21.md).

`0042AB00` performs the pre-load flush through `0042A070`, queues the next
World, and clears result state at EverGame `+0x6EC` near its return (`0042AE2A`).
Only after that call returns does `0042AEA0` read `+0x6EC` (`0042AF48`). The
value-1 branch calls `00424650` (`0042AF67`), which copies live stats into the
selected level's profile record and saves it. The value-2 branch skips that
score commit. Therefore an ordinary mapped linked-stage request runs the
flush and clears the result state before the score-commit condition; that
direct path does not commit the accumulated collection to the profile.

The mapped win-result producer is `004295B0`, reached when the Player status
widget completes through `00458400`; it writes result state 1 or 2. That is a
separate result path from the direct stage request. See
[`mission_outcome_status_dispatch_2026-10-19.md`](mission_outcome_status_dispatch_2026-10-19.md)
and
[`mission_outcome_score_profile_fresh_2026-10-20.md`](mission_outcome_score_profile_fresh_2026-10-20.md).

## Evidence limits and port status

Confidence is high for the direct call ordering, `+0x6EC` clear, and the
value-1 score-commit branch. Exclusion of the normal linked-stage path is
moderate-high because class-specific or indirect flush callbacks are not fully
traced. A separate result event or runtime ordering could not be observed.
This is static control-flow evidence, not retail runtime parity.

The host carries one `RescueStatisticsMap` across linked `run()` contexts but
does not persist that map to a profile. Keeping it in memory mirrors the
loader's no-reset branch at a high level; it does not reproduce save timing or
profile serialization. The profile-backed Records/menu work remains queued
behind gameplay priorities, and actual save-file values remain unknown.
