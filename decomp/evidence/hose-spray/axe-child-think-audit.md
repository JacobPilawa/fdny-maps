# Training Axe child Think callback audit

Recorded 2026-10-12 against the hash-pinned `FDNYFirefighter.exe` (`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). This is a bounded native model and original-P-code comparison for the child vtable `+0x20` target `004610E0`. It makes no claim about live Training callback cadence.

## Entry and state branches

A read-only `DumpVtablePointers.java` query of `.rdata` table `004E27F0` confirmed slot 8 (`+0x20`) is `004610E0`; slot 6 (`+0x18`) is `00460920`. The parent root wrapper `0045F890` tests Axe byte `+0x1FC`, conditionally calls child `Axe+0x200` at `+0x20`, and always calls child `Axe+0x648` at `+0x20`. The P-code fixture calls `004610E0` directly with a synthetic child object, so it validates the target body, not the wrapper's conditional dispatch or the callback schedule.

For an inactive child (`+0x158 == 0`), the callback returns without writes or services. For an active child it first clears Player `+0x345C` and sets the associated actor/userdata byte `+0x80`. Child state `+0x368` then dispatches:

- `0`: no state-specific call or store; common model-array work still runs.
- `1`: call strike handler `00460B90`.
- `2`: clear actor byte `+0x80`, call the Player vtable `+0x18` position getter, compare the child `+0x39C` origin with returned position `+0x28`, and call `0045FA60(child, 0.75f)` only when the 3D distance is strictly greater than float global `004E1604` (20.0 in the fixture).
- `3`: call `0045FEC0(child, 1.0f)`.
- Other values: no state-specific call; common model-array work still runs.

The model-array loop runs for every active state. Each model pointer from child `+0x324`/count `+0x328` is passed to `0047F960(model, DAT_00503A78->+4)`. If model `+0x168` is non-null and `abs(*(float*)(frame+0x1D8) - model+0x16C)` is strictly greater than global `004DFC78`, the callback clears model byte `+0x30`. Raw instruction context `00461222..0046123B` confirms `FLD float ptr [EAX+0x1D8]`, `FSUB float ptr [ESI+0x16C]`, `FABS`, then `FCOMP double ptr [004DFC78]`; the global operand is therefore read as a double. The fixture value decodes to 0.01. The native module preserves both arguments at the `0047F960` service boundary and models the later timer comparison/store.

## Original-P-code comparison

`tools/ghidra/HoseAxeChildThinkPcodeOracle.java` executes the original `004610E0` body in Ghidra EmulatorHelper. It intercepts only the strike `00460B90`, recovery `0045FA60`, scale helper `0045FEC0`, Player getter at vtable `+0x18`, and per-model `0047F960` calls. It compares seven cases: inactive/state 1, active/state 0, active/state 1 dispatch, state 2 at the exact distance threshold, state 2 beyond threshold, state 3, and an active state-0 model list with null-frame, stale-timer, and aligned-timer entries. The original body performs the state dispatch and all model timer reads/writes. The fixture is `axe-child-think-pcode-oracle.json`; `tools/axe_child_think_pcode_check.cpp` compares ordered service calls, Player/actor bytes, and every fixture model's frame presence, timer fields, and `+0x30` result against `src/axe_child_think.h`. The native model-update boundary receives each original model pointer and `DAT_00503A78+4` argument; the checker compares both values against the Ghidra record for every model update. The oracle was rerun read-only against the preserved project and pinned executable after capturing the argument explicitly.

The state-2 route materializes a binary32 vector difference in `0043B2D0`, then uses x87 multiply/add/square-root and compares to the float global. The native path stores each difference as `float`, computes the norm with extended precision where provided by the host, and compares against the binary32 threshold. The model timer path preserves the two float inputs and, consistent with the explicit `FCOMP double ptr` operand, compares their difference against the double global. The model fixture uses a far-above threshold sample, so it validates the clear/no-clear path and all model fields, not near-threshold rounding. The state-2 distance fixture includes exact 20.0 threshold equality.

## Boundaries and limits

This chunk does not emulate strike geometry, `00492CE0` world overlap, global candidate enumeration, `0043E2A0` BModel hit processing, Axe Hits stat/sound effects, animation internals, `0045FA60`/`0045FEC0` behavior, Player getter internals, or model presentation/update internals. No hit is claimed. The separate child `+0x18` handler `00460920` contains the state-0 Axe Hits initialization, state write to 1, and hide call at `child+0x3B8`; it is deliberately outside this `+0x20` module. Axe root press/held update `0045F8F0/0045FAD0` remains a separate module. No recurring dispatch cadence or integration into `main.cpp` is established here.

Focused comparison: `cmake --build build --target axe_child_think_pcode-check -j 4` and `ctest --test-dir build -R '^axe_child_think_pcode$' --output-on-failure` passed (7 cases, 8 ordered boundary calls, 3 model updates, 0 mismatches). The companion child held-action and Axe root callback checks also pass. The native model is currently a standalone module; no viewer or live-game path is connected.
