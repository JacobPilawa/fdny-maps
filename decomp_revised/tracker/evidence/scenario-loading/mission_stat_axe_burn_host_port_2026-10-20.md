# Axe Hits and Serious Burns stat host integration (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
This checkpoint rechecked the hash-pinned saved Ghidra exports and source
instruction evidence. A live Ghidra bridge instance was unavailable during the
follow-up, so no new live analysis export was produced. The relevant saved
exports are
[`00460b90_FUN_00460b90.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/00460b90_FUN_00460b90.c),
[`004610e0_FUN_004610e0.c`](../../../reference/previous-work/decomp/evidence/tool-decompiled/004610e0_FUN_004610e0.c),
and [`00451640_fresh.c`](../fire-spread/00451640_fresh.c), with the existing
raw window in
[`00451640_fire_exposure_trace.asm`](../fire-spread/00451640_fire_exposure_trace.asm).
The 13-key registration and merge modes remain documented in
[`mission_outcome_score_profile_fresh_2026-10-20.md`](mission_outcome_score_profile_fresh_2026-10-20.md).

## Retail producers

### Axe Hits

Child Think `004610E0` dispatches the strike helper `00460B90` in child state 1.
The strike helper first advances its animation clock, then requires the old
clock at Axe `+0x340` to be below 0.275 seconds and the new clock at `+0x33C` to
be above 0.275. A miss from the swept world-collision query returns before the
stat update. After a positive world hit, `00460B90` increments `Axe Hits` by
one through `00486980`, before iterating type-4 BModel candidates or checking
whether any eligible object is chopped. The unit is one threshold-crossing
strike that hits world geometry, not one per target or damage point.

### Serious Burns

`00451640` returns immediately when Player state `+0x18C` is 6. Otherwise it
compares the global millisecond clock with Player `+0x390`; unsigned `now <=
deadline` skips the pain/stat block. When due, the handler requests `RC_Pain`,
draws a shared-RNG delay in the 1000–2500 range, stores current clock plus that
delay back to `+0x390`, and increments `Serious Burns` by one through
`00486980`. This counter is deadline-gated per qualifying handler invocation;
it is independent of the later continuous fire-exposure accumulator and its
passout threshold. Whether a sound actually plays does not gate the stat call.

## Shared host integration

`scene_viewer.cpp` now initializes both counters in its rescue-stat map. The
Axe map increments in the positive world-sweep branch before BModel eligibility
is checked. The existing Player fire-message transaction invokes a new
injected callback after the host pain deadline is updated; that callback adds
one to the rescue `Serious Burns` record while preserving the separate
temporary counter used by the current feedback hint.

Together with the existing producer integrations, the shared mission host now
has in-memory producer coverage for all 13 registered mission-stat keys.
Profile serialization and the Records page remain unported. This checkpoint
records source-to-host wiring and compile/package status only; no gameplay
test, interactive play, or retail-parity comparison was performed. Host
collision results, Player message cadence, and surrounding game-loop timing
remain adapters, so the counter contributions are source-shaped rather than
proven retail runtime parity. The rescue-all macOS app compiled and packaged
with matching standalone/app executable SHA-256
`19f3f359420047b4ae54ec701931f264ce2d0ac6c14876d4b8a344a623a76456`; `Info.plist`
validation passed. The linker warned that installed json-c targets macOS
27.0 while this build targets 13.3. No gameplay tests or interactive run were
performed.
