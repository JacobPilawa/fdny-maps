# `RCS_PlayerHose` stream actor: read-only Ghidra audit

This note records a bounded static trace of the player-hose actor and stream-effect helper relevant to Training. Statements below distinguish disassembly/decompilation facts from unresolved runtime behavior. The C listings are Ghidra output with provisional names and types, not reconstructed source.

## Reproducibility and inputs

- Executable: `evidence/original/FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Training BSP: `evidence/original/training.bsp`, SHA-256 `32986212fa875764ca3d5c1630e54ad6315abaea70983203601192cec603a963`.
- Extracted entity table inspected: `assets/training/entities.json`, SHA-256 `f4bb9740af5811d3ec71472c6ecc92bf5b207e7540e5f068520d4ad81ecb6d4e`.
- Analysis used Ghidra 12.0.4 and a copied preserved Ghidra project in a temporary directory. The project was opened with `-noanalysis -readOnly`; the existing `AddressXrefs.java`, `DecompileFunctions.java`, `ExportFunctionAssembly.java`, `InstructionContext.java`, and `FieldStoreAudit.java` scripts were read-only queries. The exports are in `player-hose-stream-audit/` next to this note.

The query pattern for reproducing the function and xref exports is documented in [`../../tools/ghidra/README.md`](../../tools/ghidra/README.md). Key extracts: [`00468230_FUN_00468230.c`](player-hose-stream-audit/00468230_FUN_00468230.c), [`00468050_FUN_00468050.c`](player-hose-stream-audit/00468050_FUN_00468050.c), [`00467e90_FUN_00467e90.c`](player-hose-stream-audit/00467e90_FUN_00467e90.c), [`00466780_FUN_00466780.c`](player-hose-stream-audit/00466780_FUN_00466780.c), [`0046d1b0_FUN_0046d1b0.c`](player-hose-stream-audit/0046d1b0_FUN_0046d1b0.c), and [`0046c5a0_FUN_0046c5a0.c`](player-hose-stream-audit/0046c5a0_FUN_0046c5a0.c). `assembly.txt` preserves instruction listings and `references.txt` preserves Ghidra's xref results. The project `.gitignore` excludes raw `.c`/`.txt` evidence exports; the Markdown summary is trackable.

## Verified object and dispatch facts

1. Factory `00468230` calls `operator_new(0x1A88)` and, when allocation succeeds, calls constructor `00468050(entity)`.
2. Constructor `00468050` invokes base constructor `0043F610`, writes primary vtable `004E2D10`, and initializes a stream-effect subobject at actor `+0x1480` through `0046DC80`. It also calls `0049D160(entity, actor+0x194, 0x14)`. That validator obtains the entity UserData pointer, asks the registered entity definitions for this class's UserData size, and checks equality with `0x14` (20 bytes; its diagnostic says “EntDefs are out of sync”). The constructor then copies three dwords from UserData `+4`, `+8`, `+0xC` to actor `+0x4C`, `+0x50`, `+0x54`.
3. Training's extracted table has 262 entries. Its only `RCS_PlayerHose` and `RCS_HosePath` entries are `%typedef%` definitions at indices 251 and 252; a scan found no concrete entity rows with either class. The `RCS_PlayerHose` typedef orders `spawn_message: int`, `location: point`, and `FirstPathPoint: RCS_HosePath`. The 20-byte validated UserData size agrees with a 4-byte integer, three 4-byte location components read by the constructor, and a remaining 4-byte reference-sized field. The constructor does not visibly read that final field. This verifies a 4-byte final field for this UserData record, but does not establish how/when a path node is connected to the actor.
4. The actor's primary vtable word at `004E2D24` (vtable `004E2D10 + 0x14`) points to `00467E90`. That updater switches on actor state at `+0x18C`: 3 calls `004677D0`, 4 calls `004674A0`, 5 calls `00465FB0`, and 6 calls `00466780`; it then performs common follower/transform work. This establishes the callback implementation and state routing, not when the engine schedules a live instance.
5. State 6 (`00466780`) reads `MOUSE_LB`, `STEER_PITCH`, and `STEER_YAW`, computes a direction, obtains an origin through `00465EF0`, and calls `0046D1B0(actor+0x1480, origin, direction, mouse-held)`. `0046D1B0` writes direction into its stream object, advances pressure/width interpolation using frame delta `DAT_00502B04`, appends stream samples through `0046C000`, optionally appends a second stream when byte `+0x2F0` is set, then calls `0046C5A0`.

## Path-node data: observed offsets versus schema names

Training's typedef lists these fields in order: `ZZ_spawn_message`, `ZZ_location`, `A_NextPoint`, `B_LookAtPoint`, `C_IsAHotSpot`, `D_NextHotSpot`, `E_NextHotSpotPathDir`, `M_IsFirstHotSpot`, `M_SpotTime`, `M_MaxPressure`, `N_PitchSpread`, `Q_NumCycles`, then nine fire-model/spray-time pairs. No concrete node values are present in the extracted Training entity rows.

The state-3 path routine `004677D0` calls spline helper `00490BB0`. On the helper's nonzero transition output, it reads an item at output `+0x10` and searches the actor's list with `00466010`; that search compares the first dword of each list element. The list pointer/count at actor `+0x1A5C/+0x1A60` are initialized to zero by the constructor. The code then reads node-relative `+0x24`, `+0x1C`, `+0x30`, `+0x2C`, and `+0x28` for hotspot checks, next-node lookup, actor state/configuration, and timing. If the path references are 4-byte fields, the typedef order makes those offsets consistent respectively with `M_IsFirstHotSpot`, `D_NextHotSpot`, `N_PitchSpread`, `M_MaxPressure`, and `M_SpotTime`. This is a schema-to-offset mapping conditional on the reference-field width; the BSP extraction does not independently provide complete node byte offsets or concrete records. `00490BB0`'s internal spline samples must not be treated as proof that its returned sample is an `RCS_HosePath` entity.

At the first hotspot transition, `004677D0` submits the string `RC_HoseLvlStart` through `004897C0`. This proves that event name is emitted on this code path; it does not establish whether Training has a live actor or how the event is consumed. Other path state transitions and outcomes are left to the extracts rather than inferred from this trace.

## Collision, particles, and RNG boundary

`0046C5A0` processes buffered stream segments and calls `geWorld_Collision`. In the branch where collision returns nonzero, it creates splash/particle data. The source-visible random calls on that branch are one `FUN_00498B30(0.65, 1.35)`, one `FUN_00498B30(5.0, 10.0)` followed by float-to-integer helper `004B9400`, then six random calls for each resulting loop iteration: three `(-1.75, 1.75)`, one `(1.0, 3.0)`, and two `(0.1, 0.5)`. Thus the visible call count for a collision branch is `2 + 6N`, where `N` is the converted loop count; the count is conditional on collision and this helper's invocation. No random call appears directly in `0046D1B0`.

These two helpers show stream history, world collision, and presentation effects. They contain no direct FireRescue iteration or fire-suppression call. Do not equate this actor's collision/splash path with the separate Hose Spray item's fire-contact path.

## Unresolved items

- The Ghidra xref query shows a data reference at `004E2B40` to factory `00468230`, but no direct code caller. Indirect factory selection/registration and whether Training constructs this actor are not established.
- Training has no extracted concrete `RCS_PlayerHose` actor or `RCS_HosePath` nodes. The actor constructor's final UserData field, path-list population, and linkage into the spline path remain unresolved.
- The vtable establishes which update method is installed; actual callback cadence/order relative to player, camera, fire, and item updates is unknown without runtime evidence.
- Rendering/audio behavior is not exhaustively named here; stream subobject and callback fields are described only by their observed data flow.
- Static read-only analysis cannot prove that an indirect/runtime path is absent. No live-game capture was performed for this audit.
