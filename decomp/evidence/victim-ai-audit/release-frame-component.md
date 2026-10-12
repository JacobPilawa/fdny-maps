# Victim release frame and signed-zero recovery

`src/victim_release_frame.h` ports the `0045C000` frame prefix through `0045C0EB`, stopping before model floor placement at `0045C0EE`. Read-only Ghidra 12.0.4 queries verified the prefix assembly and helpers `00446EE0`, `0049E770`, `0042D2B0`, `00446390`, and `0043B2D0`. The preserved frame-method audit maps the indirect up-axis operation to `00496210`.

The source copies **negated** Player-frame X and Z axes, rebuilds an up-axis basis with `(0,1,0)`, reads the resulting Z axis, multiplies it by 5 and then 24 with separate binary32 stores, and subtracts this vector from the directly returned Player frame position. It does not ask for a composed world transform. The initial victim scale/parent are retained. Floor placement and later event/flags/timer operations are outside this prefix.

`tools/ghidra/VictimReleaseFrameOracle.java` executes the original prefix under EmulatorHelper. It supplies a Player slot-`+0x18` frame getter, sets the original victim frame vtable `004DFC30`, and executes the original up-axis, normalization, cross-product, negation, multiply and subtraction instructions. Every sample uses fresh emulator/FPU state and must reach the verified floor-call boundary with exactly three getter calls. The oracle compares the provided binary hash with the Ghidra program hash; it records the hash, inputs, outputs and step counts in `evidence/victim-release-frame-pcode-oracle.json`. Raw query logs remain local under `release-frame-native/`.

Eight inputs cover four cardinal orientations, noncardinal axes, pitch removal, vertical-forward fallback, and a synthetic degenerate basis. Native `--victim-release-frame-check` compares all 15 stored axis/position/scale words per case: **120 comparisons, zero differences**. This is software P-code execution of a component, not a hardware x87 capture or full rescue trace. Prior EmulatorHelper condition-code limitations elsewhere remain relevant; no complete general-frame or nonfinite-input parity is asserted from these eight cases.

The first native comparison had 11 signed-zero differences and no nonzero differences. `Extended::pack` discarded zero signs; addition also returned an operand unchanged when both were zero, and zero division discarded the result sign. The arithmetic helper now preserves input/product/quotient zero signs, applies the round-to-nearest signed-zero rules for addition/subtraction, and emits +0 for exact nonzero cancellation. `tools/extended_zero_check.cpp` checks the sign combinations independently of the frame fixture. This fixes actual arithmetic semantics rather than masking zero bits in the comparison.

The source normalization assembly at `00459B80` checks magnitude against double values `+1e-28` at `004E0B68` and `-1e-28` at `004E0B60`; zero magnitude returns without division. The basis dot threshold is double `0.999` at `004E17E0`. The previous text suggesting a near-unit normalization bypass was inaccurate: this is a near-zero bypass. Raw values were extracted from the exact PE sections after Ghidra identified their instruction operands.

## Regression scope

Passed: six CTest checks (including release frame), original ordinary movement/view, Genesis transform (4,029 words), follower, tool-follow/core, clock, fire Think/size/suppression, collision, RNG, head motion, throw scalar, interaction-box (with its existing nine oracle-inconclusive exclusions), and item-view pose.

Two existing comparisons remain red: timed-transition `--view-update-check` (29 matrix and 24 follower-basis words; max error `5.96046e-8`) and `--view-item-offset-chain-check` (8 words; max error `1.16415e-10`). A separate `/private/tmp` build using the pre-change `Extended` header reproduces exactly the same counts and errors. They are existing parity gaps, not new regressions; neither is treated as passing.

The frame helper is currently an isolated component invoked by its check. Connecting it to the ordered release transaction, world collision and original escape/event delivery remains outstanding. Continuous carry, recurring scheduling, status/statistics adapters and exit completion are still open.
