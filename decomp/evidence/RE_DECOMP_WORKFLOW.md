# FDNY reverse-engineering workflow

Reviewed 2026-10-11 against the current Training reconstruction and public decompilation tooling.

## What “decompilation progress” should mean here

`decomp.dev` is a progress website and GitHub bot. Its file/function map is a useful way to browse a project; it does not decompile a game. The Training treemap follows that presentation, but its tracked entries are implementation/evidence records, not a percentage of the original executable.

There are two different exactness goals:

- **Compiler-matching decompilation:** source rebuilds to the target's same instructions, requiring a compatible compiler, ABI, flags, and target. `decomp.me` is designed for isolated compiler-matching scratches; `objdiff` compares rebuilt objects/functions.
- **Behaviorally exact native port:** a new implementation is compared with the original program on the same inputs and state. This is the current FDNY route: native C++ components plus hash-pinned original-executable oracles, explicit RNG counts, and separate integration checks.

The current Mac-hosted C++17 app is not evidence of instruction-matching the retail x86 executable. Do not report an objdiff/compiler-match percentage unless an original-compatible compiler and build recipe exist.

## Recommended loop for each routine

1. Record executable/DLL hash, routine address, Ghidra project/version, and query scope.
2. Trace callers, vtables/data, and field reads/writes. Preserve assembly for ordering/rounding questions; treat decompiler variable names as provisional.
3. State the component boundary and inputs/services. Preserve intermediate float stores, integer wrap, callback order, and shared RNG draws.
4. Capture controlled original outputs or P-code results with input provenance. Compare numeric words and draw counts; state exactly how many cases/outputs were checked and what was stubbed.
5. Keep three claims separate: **source-mapped**, **component-compared**, and **integrated runtime parity**. Negative Ghidra searches are bounded observations, never proof that dynamic dispatch is absent.
6. Integrate only after the caller/order is known. Then compare event and RNG transcripts from equal state; stop at the first unexplained divergence.

Use `mechanics.json` for subsystem summaries, `function-index.json` to find preserved decompiler exports, `original-function-inventory.json` for the current Ghidra-defined address list, `CHECKLIST.md` for open work, `HANDOFF.md` for the continuation point, and `viewer-progress.json` for file navigation. The function inventory gives a useful classification denominator, but it is not proof that every binary routine has a defined Ghidra function boundary or that any portion of the game is complete. Keep it beside the export index because they answer different questions.

## Assets and local material

Keep original retail files, extracted assets, raw Ghidra databases, and machine-specific build output local under the existing exclusions. Preserve reproducible extraction steps and hashes in authored notes; share native source, scripts, and numeric fixtures when their scope is clear. This matches the source/assets/tools/build separation used by mature decompilation projects while respecting FDNY's distinct executable and asset format.

## References

- [decomp.dev repository](https://github.com/encounter/decomp.dev) — progress website and GitHub bot.
- [decomp.me FAQ](https://www.decomp.me/faq) — what compiler matching means and why compiler settings matter.
- [objdiff](https://github.com/encounter/objdiff) — object/function comparison tooling, including x86/MSVC workflows.
- [sm64 project layout](https://github.com/n64decomp/sm64) — source, assets, tools, build artifacts, and local ROM extraction separation.
- [GoldenEye setup guide](https://github.com/n64decomp/007/blob/master/docs/SetupGuide.md) — original-data extraction and generated-resource build workflow.
