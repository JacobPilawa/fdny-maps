# Pending level request fields

## Request block

ProcessEverGameUpdate (0042AEA0) passes the address EverGame+0x660 to ProcessLevelLoadRequest (0042AB00). The code treats that address as a request block with reference-counted strings and scalar fields:

| Offset from EverGame+0x660 | Current interpretation | Evidence |
| --- | --- | --- |
| +0x00 | Map-name string. 00403E40 reads this first field and appends .bsp; the result is sent to the BSP loader. | 0042AB00, 00403E40 |
| +0x04 | Display/log label. 0042AB00 formats this into TEverGame: Loading level; setter copies the active record's +0x04 or seeds <Spawned Level>. | 0042AB00, 0042A260 |
| +0x08 | Resource-definition path. The Player final-exit helper supplies data\\resdefs\\rescue.rrf; the loader uses this field when initializing World resources. | 0042A260, 0042AB00 |
| +0x0C | Reference copied from active record +0x0C. In the level record this is the catalog `music:` string: 0042AEA0 passes active-record +0x0C to selector 2, which opens/plays that MP3. Linked map requests preserve the selected track through this copy. | 0042A260, 0042AEA0, 0042A730 |
| +0x10 | Scalar copied from active record +0x10; no-active-record fallback is the bit pattern for 1200.0f. | 0042A260 |
| +0x14 | Fallback-state byte. Setter sets it when no active record exists and clears it when copying active-record metadata. The loader consults it before the rescue resource-name check. | 0042A260, 0042AB00 |
| +0x18 | Scalar copied from active record +0x18; fallback is zero. | 0042A260 |
| +0x1C | Uses active record +0x1C when positive, otherwise active record +0x18. | 0042A260 |

These are behavioral labels, not recovered C++ member names or confirmed field types except where noted by the literal 1200.0f fallback and string operations.

## Map-key provenance

HandlePlayerFinalExitLoadMap (00450A70) builds and checks data\\maps\\<command-key>.bsp from its parsed command argument. On success it passes the original command-key reference to SetPendingLevelRequest (0042A260), which stores that value in the request block's +0x00 map-name field. ProcessLevelLoadRequest later appends .bsp to the stored field. Raw x86 stack offsets establishing the shared key are recorded in [the pending-key dataflow note](finalexitloadmap_pending_key_dataflow_2026-10-20.md).

The field consumed by the loader is therefore known, and the raw static trace proves it receives the same key used in the existence check. The static pre-load flush sequence is mapped in [level transition flush](level_transition_flush_2026-10-21.md); live stage completion, replacement presentation, callback timing, and cross-stage state retention remain follow-up work.

## Linked-stage stat-reset boundary

`SetPendingLevelRequest` (`0042A260`) copies the active level record's metadata
when EverGame `+0x688` is non-null, and clears request fallback byte `+0x14`.
`ProcessLevelLoadRequest` (`0042AB00`) only calls `00424710` when that fallback
byte is nonzero. The normal linked `finalexitloadmap` path therefore skips this
entire stat initializer call. When called, `00424710` first clears the
collection at the stat manager's `+0x30` through `004883D0`, then conditionally
registers the rescue definitions. Since the linked branch skips the call, it
also skips that clear; see [the linked-stage lifetime audit](linked_stage_stat_collection_lifetime_2026-10-21.md)
and [the score-profile trace](mission_outcome_score_profile_fresh_2026-10-20.md).

This does not prove that cumulative live statistics survive the transition:
the `0042A070` flush invokes helpers whose full side effects remain unknown.
Follow-up decompilation mapped direct world-resource and ScriptEngine cleanup
in `004944E0`, `00493EA0`, and `00470ED0`. ScriptEngine `+0x94` aliases
`0x00503044`, the same variable container used by `00470FC0`, so the flush
clears its old entries through `0043EE40`; `00471030` then stores fresh
`DIFFICULTY` and `LEVEL` values after the flush. `DIFFICULTY` comes from the
selected object at EverGame `+0x6E4`, field `+0x2C`, formatted with `%i`.
The profile-menu writer is statically resolved: slot events 0 through 5 call
`004239C0` on the manager embedded at `EverGame+0x6CC`, storing the selected
record at manager `+0x18` (`EverGame+0x6E4`) before opening level-selector child
2. See [the profile selection route](profile_selection_pointer_writer_2026-10-21.md).
The live selected slot and loaded value are not captured. `LEVEL` comes from
the active level record's first string field at `+0x688`. World-resource actor and container
callbacks still leave mission-stat/profile side effects open. The `004443E0`
callsite and formatter anchors are in
[`004443e0_difficulty_format_fresh_2026-10-21.asm`](004443e0_difficulty_format_fresh_2026-10-21.asm).
See the
[nested flush callee trace](level_transition_flush_2026-10-21.md#nested-world-resource-and-script-engine-effects).
The host passes one `rescueStatistics` map through the linked `run()` calls,
matching the static branch-level fact that linked loads skip the stat
collection clear. The host map is not the retail profile object, so runtime
values and full parity remain unverified. The score/profile manager constructor
initializes EverGame `+0x6E4` to null, but its later selected-record writer
remains unresolved. The flush drains manager lists and de-duplicates entity pointers
against the owner service's pending-work queue before enqueueing a
`SetEntityFactoryRequestPending` request. PlayerRescue is statically admitted
to the drained Think list, but the full callback effect and any restoration
into the fresh PlayerRescue remain unresolved. Every rescue/stage BSP creates
a PlayerRescue through its ordinary spawn path; that does not prove which
gameplay fields carry across the transition.
See [the flush and Player lifetime boundary](level_transition_flush_2026-10-21.md).
Keep statistics and Player-state persistence unresolved until those paths are
mapped or a controlled retail comparison becomes available.

## Score/profile pointer initialization

The retail EverGame constructor (`0042B860`) calls `00425090` with
`EverGame+0x6CC`. The fresh decompilation shows `00425090` clearing its
`param_1[6]` field, which is `EverGame+0x6E4`, to null before it loads or
creates the per-player `.rb` score records in the manager's record array.
`00430060` routes selected profile-slot events to `004239C0`, whose non-null
record branch writes the indexed record pointer into manager `+0x18`
(`EverGame+0x6E4`). The saved fresh body and source hash are in
[`00425090_profile_manager_initializer_fresh_2026-10-21.c`](../runtime-loop/00425090_profile_manager_initializer_fresh_2026-10-21.c).
