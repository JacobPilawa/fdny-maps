# Carry entity mode component

`src/carry_entity_mode.h` models the entity loop in `0044FCE0(mode)` used by pickup (`mode=1`) and release/reset (`mode=0`). It walks the supplied entity vector in source global-list order and examines entity type at `+0x18`. Type 3 entities always take `00446EC0`: store the mode byte at entity `+0x12D`, then apply `00497DE0` to the model object at `+0xD0`, writing `+0x44=1` and `+0x45=mode`. Type 8 entities take `00440F50` only when entity `+0x164` is nonzero; that helper reports absent model metadata, stores mode at entity `+0x165`, and calls `geWorld_ModelSetFlags(model,0)` only when `mode==0`.

The raw fields are modeled locally. `reportMissingWorldModel(entity)` represents the source diagnostic boundary; it runs immediately when the gated type-8 helper finds no model, before the `+0x165` store. The only engine API callback is `setWorldModelFlags(optionalModel,0)`, because the Genesis model setter is not ported. The caller has no early return after the missing-model diagnostic: it proceeds to store `+0x165` and, on false mode, calls the setter with the null model pointer. The standalone component preserves that attempted null argument through an empty optional. This proves the callsite argument and ordering; it does not establish whether `geWorld_ModelSetFlags` tolerates null or returns normally. No flag behavior is assigned to other entity types.

## Ghidra evidence

Fresh Ghidra 12.0.4 decompilation against the preserved `FDNYFirefighter.exe` project was run read-only with `-noanalysis -readOnly` for `0044FCE0`, `00446EC0`, `00440F50`, and `00497DE0`. The project lock required the scratch project `/private/tmp/fdny-victim-rescue-audit-ghidra`; the raw output is `/private/tmp/carry-entity-mode-query.txt`. `0044FCE0` initializes the global-entity iterator, advances one list node at a time, and invokes `00446EC0` for type 3 and `00440F50` for type 8, in that order of traversal. `00446EC0` writes `+0x12D` before calling `00497DE0`. `00497DE0` writes model object `+0x45=mode` and then `+0x44=1`. `00440F50` first checks `+0x164`; within that gate it diagnoses a missing model, writes `+0x165`, and conditionally invokes `geWorld_ModelSetFlags(model,0)` only for false mode. There is no branch returning after the diagnostic; on the false path the callsite loads and passes the model pointer even when it is null. Setter null handling is outside this query. The executable hash is recorded in [`../../HANDOFF.md`](../../HANDOFF.md).

Pickup-specific caller order is recorded in [`pickup-post-effects.md`](pickup-post-effects.md): `0044FCE0(1)` runs after the synchronous pickup event and before later Player effects. Rescue and passout call `0044FCE0(0)` at their separately documented points. This component does not infer a recurring schedule.

## Validation and integration

From `decomp/`, run:

```sh
c++ -std=c++17 -Wall -Wextra -pedantic tools/carry_entity_mode_check.cpp -o /tmp/carry-entity-mode-check
/tmp/carry-entity-mode-check
```

The standalone check covers enabled and disabled modes, type filters, type-8 gate and missing-model branches, diagnostic-before-field-write ordering, list-order callbacks, type-3 model-object writes, and the false-only world-model API call. This is component validation, not original-binary execution. Integrate by adapting current global entity records into `CarryEntityModeEntity` in list order and copy the resulting raw fields back; supply the diagnostic and world-model adapters at their exact callsites. Keep call timing tied to existing verified pickup/release transactions rather than a new frame schedule.
