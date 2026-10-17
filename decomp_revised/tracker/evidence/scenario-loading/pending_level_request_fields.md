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

HandlePlayerFinalExitLoadMap (00450A70) builds and checks data\\maps\\<command-key>.bsp from its parsed command argument. On success it passes a separate context string to SetPendingLevelRequest (0042A260), which stores that value in the request block's +0x00 map-name field. ProcessLevelLoadRequest later appends .bsp to the stored field.

The field consumed by the loader is therefore known. The current static trace does not prove the context string equals the parsed finalexitloadmap argument: the helper checks the command key but does not compare it with the string it queues. The context producer and runtime equality check remain follow-up work.
