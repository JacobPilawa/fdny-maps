# Shared victim frame operations

`src/victim_frame_ops.h` supplies only the frame and floor callbacks used by
the Training release and passout components. It holds a reference to the
external victim `FrameTransform` and a `std::function<const FrameTransform&()>`
Player-frame getter. The X-axis copy, Z-axis copy, and position copy each call
that getter independently, matching the three Player-frame reads in each
source helper. Both axis copies negate the source axis. Release applies
`ViewUpdate::setUp` with the helper's `(0,1,0)`, then keeps the ×5 and ×24
binary32 operations separate before subtracting the result from Player-frame
position. Passout copies Player-frame position directly and rebuilds the
basis from world-up. Neither path replaces the frame's parent or scale.

Floor placement delegates to `placeModelOnFloor` with the Training victim
hull `mins=(-24,-24,-24)`, `maxs=(24,24,24)` and caller-supplied trace and
failure-report callbacks. Its resulting position remains in the referenced
victim frame; `lastFloorTraceSucceeded` records the wrapper result.

## Passout frame method evidence

Read-only Ghidra 12.0.4 `ExportFunctionAssembly.java 0045C1A0` on a scratch
copy of the preserved project showed pushes at `0045c260` (`0`), `0045c26b`
(`0x3f800000`), and `0045c270` (`0`) immediately before the indirect frame
method call at `0045c272` through vtable slot `+8`. On x86 right-to-left
argument passing these are `(0,1,0)`. The passout adapter therefore uses
`ViewUpdate::setUp(victimFrame,{0,1,0})`; this value is instruction-derived.
`release-frame-method.md` independently maps that frame operation to
`0042BD50` → `00496210`, which sets the up axis and rebuilds the perpendicular
basis with its fallback behavior.

The same Ghidra listing shows separate Player frame getter calls before
`00446EE0`/`0042D2B0`, `0049E770`/`0042D2B0`, and the final direct position
copy. The existing release frame source notes map the conversions to
negated X/Z axes. Training victim hull bounds are recorded in
`../victim-startup/native-runtime-integration.md`; the six authored floor
cases match Genesis.dll P-code as documented in
`../victim-startup/victim-floor-ghidra-pcode-oracle.md`.

## Acceptance and caller boundary (2026-10-12)

`ctest --test-dir decomp/build -R '^(victim_frame_ops|victim_transactions)$' --output-on-failure` passes 2/2. The adapter check covers 46 original frame/floor samples and 618 compared words: 8 release-frame, 32 passout-frame, and 6 Training Genesis floor cases. It also checks three separate Player-frame reads, failed-floor fallback, and the correction arithmetic. `VictimTransactions::PassoutOps` owns the live receiver lookup because the original path rereads Player+0x3534 after callbacks; the composed transaction check changes that carried entity during reset and confirms the redirected actor frame receives the 30-unit X correction. This verifies the tracked frame/floor component and caller boundary only; it does not establish recurring victim scheduling or full Training behavior.
