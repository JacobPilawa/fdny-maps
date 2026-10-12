# Reconstruction approach

Snapshot: 2026-10-12.

## What this project is measuring

The current deliverable is a native C++ reconstruction that reproduces Training behavior. It is not a matching decompilation whose rebuilt executable must be byte-identical to the original Windows binary. Keep those goals separate: isolated instruction/P-code comparisons and live-game traces are strong evidence for behavior, but they do not establish a matching build. If byte matching becomes a requirement, first identify and pin the original compiler, linker, flags, runtime libraries and binary layout, then add a separate object/binary diff workflow.

## Recommended loop

1. Pin the original executable and relevant data with hashes. Keep the preserved Ghidra project read-only; run repeatable headless scripts for bounded queries and export findings into `evidence/`.
2. For each routine, record its address, callers/owner, state inputs and outputs, writes, RNG/clock/event effects, unresolved indirect calls, and the exact evidence/fixture that supports each claim.
3. Compare small native components against original P-code or the original process, preserving real internal helpers and call order. Label emulator substitutions, engine-service stubs and fixture limits explicitly.
4. Track component equivalence separately from runtime integration. Connect a component only after its owner schedule, cadence and service inputs are established; then compare live state, events, time and shared RNG order.
5. Report progress as verified routines/components and open integrations. Do not turn an incomplete function inventory or a high unit-test pass rate into a percentage of whole-game completion.

This is close to the project's existing practice: address-keyed Ghidra evidence, hashed original-code fixtures, native C++ comparisons, and separate open items for Player/owner cadence, rendering and live behavior. The highest payoff now is resolving those integration boundaries and capturing Training runs, not adding more isolated cases to already well-covered routines.

## Tools and reference projects

- Ghidra's official headless analyzer supports repeatable script-driven analysis; its read-only option discards project changes when the run exits. Continue using it for bulk, bounded exports and preserve the interactive project as analysis state: [HeadlessAnalyzer API](https://ghidra.re/ghidra_docs/api/ghidra/app/util/headless/HeadlessAnalyzer.html), [HeadlessOptions API](https://ghidra.re/ghidra_docs/api/ghidra/app/util/headless/HeadlessOptions.html).
- The requested decomp.dev page blocks automated access here. Its linked [Parasite Eve project](https://github.com/khasinski/parasite-eve-decomp) is a primary-source example of a different target: source-only, compiler/toolchain-configured matching decompilation, with `objdiff` reports generated from fresh retail disassemblies. Its inventory, reproducible build inputs and audited progress report are useful patterns, but its byte-match metric should not be copied into this behavior-focused native port.
