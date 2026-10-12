# FDNY decomp handoff

**Snapshot:** 2026-10-11

**Target:** source-verified, 1-to-1 native C++ reproduction of the Training level, preserving reusable findings for a later full-game port. Training is not complete; the current build is a partial playable reconstruction.

## Repository state

- Working tree: `.`; project: `decomp/`.
- Branch `main`; the victim transaction milestone follows `bfdee2c` (prior lifecycle handoff). Remote: `https://github.com/JacobPilawa/fdny-maps.git`; the user approved pushes to it.
- The only untracked root item at handoff is `fire_spread/`; it is unrelated and must not be staged as decomp work.
- Read [`CHECKLIST.md`](CHECKLIST.md) for detailed progress, [`README.md`](README.md) for setup, and [`mechanics.json`](mechanics.json) for machine-readable status. Do not use `training-progress.html` as a source of truth; it has not been updated recently.

## Latest verified work

- Ghidra was used read-only against preserved game code. The bundled `FDNYFirefighter.exe` SHA-256 is `4b3b4dc1cc0b05bcb42c8e27ac6fca72fe5f7064ca8122fda7d41e7f6d2fc3e`; bundled `Genesis.dll` SHA-256 is `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`.
- Player message `0x7532` pickup is mapped as an isolated native transaction in `src/victim_rescue.h`. It stores the carried victim and state 5, clears the source actor-flags word from `0x400` to `0`, emits the synchronous pickup event, then invokes an ordered post-effects hook. `victim_pickup_effects.h` now supplies source-ordered storage and callback boundaries; engine/status/zone internals and runtime scheduling remain open. See [`training-pickup-flow.md`](evidence/victim-ai-audit/training-pickup-flow.md) and [`pickup-post-effects.md`](evidence/victim-ai-audit/pickup-post-effects.md).
- The Genesis normal-render gate checks `(flags & 0x1400) != 0`; Training victims start at `0x400`, so they pass, and pickup's zero flags suppress normal-pass submission. Native actor drawing now consumes per-entity actor flags through the source normal-pass gate; pickup production remains unconnected. Other render passes are not exhaustively mapped. See [`actor-render-flags.md`](evidence/victim-ai-audit/actor-render-flags.md).
- Ghidra maps Training escape release `00451160→0045C000` (state 6, Player-frame-derived transform, floor placement, actor flags restore, rescue event and timer writes) and a distinct passout/drop route `0044FF10→0045C1A0` (state 4, motion RNG, transform, floor placement, flags restore and passout event). `victim_release.h` now models escape/release ordering, flags and timers, with frame/engine operations delegated to callbacks. Passout/drop and continuous carried-victim positioning remain open. See [`post-pickup-lifecycle-static.md`](evidence/victim-ai-audit/post-pickup-lifecycle-static.md).
- Bounded Ghidra checks have not found a recurring dispatcher for FireRescue visual Think `00449FE0`. This is an unresolved path, not proof that runtime/computed dispatch is absent. See [`think-dispatcher-candidate-followup.md`](evidence/fire-update/think-dispatcher-candidate-followup.md) and the related audits linked from the checklist.
- The native victim pickup message check and 14-case pickup geometry check passed with zero mismatches. The six authored Training victim floor-placement cases match the Genesis.dll P-code oracle bit-for-bit. These are component checks, not a full gameplay comparison.
- Last verification on this work: `cmake --build decomp/build -j 2`, `decomp/build/fdny-training --victim-pickup-message-check`, `--victim-pickup-geometry-check`, `git diff --check`, and JSON parsing of `mechanics.json` passed. The build emitted a non-blocking Homebrew `json-c` deployment-target warning (library built for macOS 27.0, app target 13.3).

## Highest-priority gaps

1. Supply the source-derived frame/math and engine/statistics/status adapters for the isolated Training escape/release transaction `00451160→0045C000`, then connect it only after verifying the relevant schedule. Keep passout/drop `0044FF10→0045C1A0` separate because it has different RNG, motion and position steps.
2. Connect victim proximity, message production/delivery, the `0x7532` handler and ordered Player post-effects using a verified callback schedule. Share victim actor state with native rendering and implement the source-derived render gate; trace continuous carry positioning and exit completion against Ghidra.
3. Resolve Fire Think dispatch and frame order, then connect source-verified fire recovery/spread/douse and particle-pool behavior without inventing RNG draws.
4. Continue the rest of the Training script, BModel, hose, damage, tool, mission and shared-RNG paths. Track component parity separately from whole-level parity.

Major open items and their evidence are enumerated in [`CHECKLIST.md`](CHECKLIST.md). The pickup callback, Player scheduling and Fire Think cadence are all still open. The current implementation does not prove full Training frame order or whole-run RNG parity.

## Ghidra and original-runtime notes

- Use Ghidra for every reverse-engineered claim. Start with [`tools/ghidra/README.md`](tools/ghidra/README.md). Query the preserved project using `-noanalysis -readOnly`; use a scratch copy when the project lock is busy. On this host, preserved-project queries work with JDK 21; fresh analysis requires JDK 25.
- Keep exact binary hashes, function addresses, query scope and negative-search limits with evidence. A missing direct/static edge does not rule out computed dispatch.
- No live game capture was completed. Earlier WineDbg attempts froze or crashed; a bounded read-only path was not established. Live capture was set aside in favor of static Ghidra/source work. Do not leave the user waiting or ask them to enter Training unless a safe, time-bounded capture is ready.
- The user asked to preserve discoveries beyond Training, use one subagent for meaningful parallel work, and pause if usage reaches 1% remaining in either the five-hour or weekly window. Last observed usage: 7% of the five-hour allowance used and 50% of weekly allowance used. Recheck before extended work.

## Next-session starting point

Read this file, `CHECKLIST.md`, `README.md`, the victim evidence notes linked above, and `tools/ghidra/README.md`. Continue from the current pushed `main`; keep unrelated files out of commits. Verify source claims in Ghidra, record new findings before integrating them, run focused checks for any changed component, and push a decomp-only commit at the next substantial verified milestone.

## Current implementation milestone

Read `IMPLEMENTATION_PLAN.md` for the dependency map. Two GPT-6 Luna agents implemented the release boundary and render predicate; the lead added pickup post-effects, unified carry/state representations, corrected callback clock sampling and connected the normal draw gate. CMake/CTest owns three standalone checks. New assertions remain enabled in Release builds. These are static-source behavior checks, not original execution comparisons.

The release component still delegates all frame calculations and neighboring engine work. The post-effects component still delegates entity-mode internals, status and zone reconstruction. No recurring schedule is inferred. Next useful work: recover and implement the release frame transform using the existing exact arithmetic/frame components, then verify against original execution before runtime wiring.

Milestone validation: native build and all three CTest checks passed; existing pickup message/14-case geometry, actor animation (54 selections/432 updates), actor pose consistency (303 vertices/18 frames), and startup checks passed. A two-frame render check required macOS display access outside the sandbox and then drew 90 textured mesh groups with OpenGL error 0. Final Training parity is unverified.
