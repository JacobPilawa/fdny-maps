# Reverse-engineering and port workflow

## Function record states

Every body-bearing function in `tracker/functions.csv` gets one current state and may keep links to earlier evidence. External import stubs use `external` and are tracked separately from code coverage:

1. `inventory_only` — Ghidra has a function boundary; no behavior claim yet.
2. `triaged` — likely subsystem and callers/callees recorded; names remain provisional where appropriate.
3. `source_mapped` — control flow, key data, side effects, and important call order have supporting instruction or cross-reference evidence.
4. `component_compared` — a bounded native component was compared with original code or a documented original-process capture. Record inputs, outputs, number of cases, and stubs.
5. `port_integrated` — the component participates in a real port path with its caller/order established.
6. `scenario_parity` — an end-to-end authored scenario matches the original on defined state, event, timing, and RNG observations.

These states are not interchangeable. A Ghidra decompile export is not recovered source. A component comparison is not whole-level parity. Exact compiler matching, if ever pursued, gets its own status field and requires an original-compatible build chain.

The dashboard's time series counts **reviewed Ghidra-defined EXE functions** from `tracker/function_reviews.csv`: each entry must have a fresh decompilation saved from the hash-pinned project and an address-specific behavior note. Those functions are shown as `triaged` in the function atlas. `triaged` records a useful subsystem/behavior pass; advance to `source_mapped` only when control flow, important data, side effects, and call order have enough supporting instruction or reference evidence. This is an inspection count, not a percent-complete claim and not proof of native-port parity. Manually bounded code slices that are not function objects in the fresh Ghidra inventory are tracked in evidence notes but excluded from this count. Append a dated snapshot to `tracker/progress_history.csv` only after a meaningful batch of function reviews.

## Evidence note template

For each important routine or system, record:

- binary path, SHA-256, Ghidra version, address/RVA, and function range;
- likely role, callers/callees, owning object, and relevant globals/vtables;
- fields read/written, state transitions, events, clock and RNG effects;
- evidence used: raw instructions, decompiler output, data references, P-code, original-runtime trace, asset/script record, or port comparison;
- fixture inputs, outputs, case count, helper calls retained, and any stubbed services;
- what remains unknown, especially indirect calls, scheduling, callback cadence, and integration.

## Review loop

1. Start with one binary-hash-pinned question.
2. Use Ghidra to inspect the function, callers, data, and related vtables. Use raw x86 instructions to resolve ordering and floating-point details.
3. Write a short evidence note before translating a large region.
4. Build the smallest component boundary that preserves the discovered side effects and call order.
5. Compare it against original P-code or a controlled runtime capture. State exactly what the comparison proves.
6. Integrate it only when the original caller and schedule are understood. Compare the first divergent state/event/RNG point.
7. Update the function ledger and milestone as evidence accumulates. The live dashboard page reads the CSVs and refreshes once a minute when opened through `Open FDNY Dashboard.command`, which serves it locally. Opening `dashboard/index.html` as a `file://` page shows its static snapshot because browsers block those CSV reads. Do not regenerate its HTML for routine reviews; refresh the static fallback only when the inventory or page structure changes materially.

## LLM use

Use LLMs for candidate explanations and repetitive queries. Keep the original bytes and evidence as the oracle. Do not accept plausible C-like pseudocode on appearance alone; check its machine code path, semantics, and behavior on controlled cases. Record uncertainty and negative-search limits rather than turning them into claims that a path does not exist.
