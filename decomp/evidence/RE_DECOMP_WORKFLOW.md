# FDNY reverse-engineering workflow

Reviewed 2026-10-12 against the current Training reconstruction and public decompilation tooling.

## What “decompilation progress” should mean here

`decomp.dev` is a progress website and GitHub bot. Its file/function map is a useful way to browse a project; it does not decompile a game. The Training treemap follows that presentation, but its tracked entries are implementation/evidence records, not a percentage of the original executable. The user referenced the Parasite Eve page at `decomp.dev/khasinski/parasite-eve-decomp`; the host blocks automated page access, so its layout is treated as a visual reference, not a source of technical guidance.

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

Use `mechanics.json` for subsystem summaries, `function-index.json` to find preserved decompiler exports, `original-function-inventory.json` for the current Ghidra-defined address list, and `function-inventory-reconciliation.json` to audit exact-address joins among those records. Regenerate the latter with `python3 tools/reconcile_function_inventory.py`. The export index does not preserve an executable identity for every C export, so review provenance before treating an unmatched address as a missing function boundary. The inventory and reconciliation help find unclassified or unmatched addresses; neither proves that every binary routine has a defined Ghidra function boundary or that any portion of the game is complete. Keep them beside the export index because they answer different questions. `CHECKLIST.md` tracks open work, `HANDOFF.md` records the continuation point, and `viewer-progress.json` supports file navigation.

## Tool fit for this executable

The retail target is a monolithic PE32 x86 executable. `objdiff` supports PC x86 and compares relocatable object files; its detailed progress reports power `decomp.dev`. It can become useful if the original compiler/toolchain and linkable target objects are recovered. It cannot turn the current executable alone into a meaningful compiler-match score. `decomp.me` accepts one extracted function plus context and likewise needs the right compiler preset and flags to prove an instruction match. For now, keep the native build's claims behavioral: use hash-pinned original/P-code fixtures, compare state words and shared-RNG ordinals, then add live event/clock/RNG transcripts when the game can be run. Keep the progress viewer's denominator scoped to the tracked ledger.

The public projects reinforce this split. The Black & White guide organizes a matching project around a binary config, address ranges (`splits.txt`) and named symbols, and promotes one source object only after `objdiff` says it matches. Zelda Twilight Princess likewise keeps functions marked nonmatching until the per-object diff reaches 100%. For FDNY, borrow the address-keyed inventory and explicit per-routine acceptance state, but defer that compiler/linker scaffolding until an original build recipe exists; the current small native components and original-execution fixtures fit behavioral recovery.

`decomp.dev`'s repo describes a dashboard and GitHub bot rather than a reverse-engineering engine. Its project tracking is optional. The existing local viewer already gives this project the useful file and routine navigation without implying source or function completion. Do not introduce a CI/toolchain integration until it can compare something meaningful for the original PE32 target.

## Assets and local material

Keep original retail files, extracted assets, raw Ghidra databases, and machine-specific build output local under the existing exclusions. Preserve reproducible extraction steps and hashes in authored notes; share native source, scripts, and numeric fixtures when their scope is clear. This matches the source/assets/tools/build separation used by mature decompilation projects while respecting FDNY's distinct executable and asset format.

## References

- [decomp.dev repository](https://github.com/encounter/decomp.dev) — progress website and GitHub bot.
- [decomp.dev Parasite Eve project](https://decomp.dev/khasinski/parasite-eve-decomp) — user-provided visual reference; the page could not be fetched by the automated browser.
- [decomp.me FAQ](https://www.decomp.me/faq) — what compiler matching means and why compiler settings matter.
- [objdiff](https://github.com/encounter/objdiff) — object/function comparison tooling, including x86/MSVC workflows.
- [Black & White decomp getting started](https://github.com/openblack/bw1-decomp/blob/main/docs/getting_started.md) — config, split ranges, symbol names, and a per-object matching loop.
- [Zelda Twilight Princess decompiling guide](https://github.com/zeldaret/tww/blob/main/docs/decompiling.md) — Ghidra/objdiff workflow and a clear nonmatching-to-matching gate.
- [sm64 project layout](https://github.com/n64decomp/sm64) — source, assets, tools, build artifacts, and local ROM extraction separation.
- [GoldenEye setup guide](https://github.com/n64decomp/007/blob/master/docs/SetupGuide.md) — original-data extraction and generated-resource build workflow.
