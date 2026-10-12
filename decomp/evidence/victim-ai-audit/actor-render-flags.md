# Victim actor render flags

Read-only Ghidra 12.0.4 analysis of the exact bundled `evidence/original/Genesis.dll`, SHA-256 `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`. The DLL was imported into a fresh scratch project under `/private/tmp/fdny-actorflags-exact-import/ActorFlags`; no repository Ghidra database or live process was changed.

## Setter and world actor table

`geWorld_SetActorFlags` at `0x1003C340` searches the world actor array (`world+0x3B8` count, `world+0x3BC` records, 12-byte stride) by actor pointer and writes its third argument verbatim to record word 1. `geWorld_GetActorFlags` at `0x1003C3B0` reads the same word. This replaces the stored flags value; it is not a mask operation. `geWorld_AddActor` at `0x1003C1A0` appends the actor and supplied flags, while `geWorld_RemoveActor` at `0x1003C280` removes the record. The flag setter does not add or remove actors.

The game wrapper `00481140` calls this API with `0x400` for true and `0` for false. Pickup helper `0045BEF0` passes false, so the world actor flags word becomes zero. Rescue release helper `0045C000` passes true, so it becomes exactly `0x400`; other flag bits are not preserved by those calls.

## Render and query effects

The normal world render path reaches `0x1003D660` through `0x1003EAD0`; `geEngine_RenderWorld` is at `0x10024950`. At `0x1003D770–0x1003D776`, the render loop tests `flags & 0x1400` and skips actor submission when neither bit is set. Thus flags `0x400` pass this normal-pass submission gate, and flags `0` fail it. This supports describing `0x400` as a **normal-world-render eligibility bit** for this renderer path; no official enum name was found.

The reflection branch tests `flags & 0x1800`, so exactly `0x400` does not pass that separate gate. The examined collision/intersection helpers `0x100360F0`, `0x10037C10`, and `0x10037E90` filter on `0x2000`, not `0x400`. Actor list membership is independent of this flag.

## Native implication and limits

When pickup and rescue are wired into native runtime state, the actor should remain registered while normal render submission is disabled during carry and re-enabled at rescue, matching the two exact stored flag values. The current native transaction records and forwards the raw value `0`; the renderer does not yet consume it. Initial Training actor flags at insertion and every other Genesis render pass have not been exhaustively traced.

## Query provenance

- Fresh import and analysis used Ghidra 12.0.4 / JDK 25 on the exact bundled DLL.
- Queried/decompiled `geWorld_SetActorFlags`, `geWorld_GetActorFlags`, `geWorld_AddActor`, `geWorld_RemoveActor`, `0x1003D660`, and `geEngine_RenderWorld`; retained exports and instruction windows in the scratch project.
- Inspected actor table field references and render gate instructions, plus bounded scans of actor-table and flag-bit consumers. Absence claims are limited to the functions/scans listed above.
- The game-side wrapper and victim callsites were separately rechecked against the preserved `FDNYFirefighter.exe` Ghidra project (`00481140`, `0045BEF0`, `0045C000`).
