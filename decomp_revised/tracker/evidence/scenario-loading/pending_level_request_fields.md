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
explicit stat-collection reinitialization. When called, `00424710` clears the
collection at the stat manager's `+0x30` and conditionally registers the rescue
definitions; see [the score-profile trace](mission_outcome_score_profile_fresh_2026-10-20.md).

This does not prove that cumulative live statistics survive the transition:
the `0042A070` flush invokes several helpers whose full side effects remain
unknown. The host also creates its `rescueStatistics` map inside each `run()`
invocation, so the current linked scene chain does not carry that map into the
next stage. Keep persistence unresolved until the remaining manager-pointer
lifecycle and opaque flush paths are traced; carrying values may be a port
choice, but is not a verified retail behavior.
