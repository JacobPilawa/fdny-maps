# FDNY decomp handoff

Snapshot: 2026-10-11. Target: complete source-verified 1-to-1 Training reproduction, then reuse recovered systems for a full C++ port. Training remains a partial playable reconstruction; whole-run RNG and frame order are unverified.

## Workspace and navigation

- Workspace: `/Users/jacobpilawa/Documents/ChatGPT/FDNY_trackdown`; source: `decomp/`; branch: `main`.
- Remote: `https://github.com/JacobPilawa/fdny-maps.git`; the user authorized milestone commits/pushes. Current shared-transaction milestone follows pushed `40093a2`.
- Leave unrelated untracked `fire_spread/` untouched. Stage only decomp work. Retail assets/binaries, raw Ghidra exports and build/runtime products stay local under existing exclusions.
- Read `CHECKLIST.md`, `IMPLEMENTATION_PLAN.md`, `mechanics.json` and `README.md`. `training-progress.html` is stale and is not authoritative.

## Current verified victim work

- `victim_rescue.h`: delivered message `0x7532` gates, carried pointer, state 5, raw flags `0x400→0`, synchronous pickup event, then ordered post-effects hook.
- `victim_pickup_effects.h`: source storage and ordered entity/status/zone adapters; final clock sample. Engine adapter internals remain unfinished.
- `victim_release.h`: ordered escape/release transaction, state 6, flags restore, rescue event, then sampled clock/timer writes and Player cleanup boundaries.
- `victim_release_frame.h`: negated Player X/Z, up-axis rebuild, separately stored ×5 then ×24 offset, Player-position subtraction. Eight original P-code release cases match 120 words. Passout copies Player position directly after the same basis work.
- `victim_passout.h`: caller counter/position preamble, conditional bounded motion draw/speed, passout event, separate Player-X +30 and victim-frame-X −30 corrections. Old Y notes were corrected against assembly. The independent herodown caller tail is still unported.
- `victim_frame_ops.h`: concrete frame/floor adapters use one external frame, three distinct Player getter calls and ±24 hull placement. Dropped-frame correction mutates the external frame; there is no mirrored coordinate in passout state.
- Passout prefix oracle executes original RNG and frame instructions: 32 cases / 576 values match. Adapter checks compare 40 frame cases and six native Training BSP floor cases: 618 words match. Failed-placement fallback is tested.
- `victim_rescue_statistics.h`: counter and ordered live pickup-timestamp/clock reads; record update core uses configured mode and signed int32 comparisons; addition wraps at 32 bits. Name lookup/loading/configuration remain adapter work.
- Native normal victim drawing reads per-entity flags through `(flags & 0x1400)!=0`. Pickup production and complete rescue gameplay are still unconnected. Continuous carry positioning is unresolved.

Evidence: also see `evidence/victim-ai-audit/rescue-deadline-exit-component.md`. `evidence/victim-ai-audit/{pickup-effects-component,training-release-component,release-frame-component,passout-component,passout-frame-component,victim-frame-ops,rescue-statistics-component}.md`. Keep static-source checks, original component execution, native integration and whole-game parity distinct.

- `player_rescue_deadline.h`: recovered the previously unported escape post-effect `00454300→004541A0`; 48 complete original P-code calls match deadline/draw count. Source draws one bounded 30000/60000 value after status action 2, before carry clear. Player+343C producer/meaning remain open.
- `zone_membership.h` and `victim_exit.h`: concrete membership rebuilding and exit bookkeeping now exist; binding/bounds and engine callbacks remain inputs. Training has no authored finalexit candidates. Empty-zone gate/item refresh is source behavior, not a proven complete outro.

- `victim_transactions.h` now composes delivered pickup/escape with one shared carried pointer, counters, timestamps, actor/frame/animation and original deadline draw. `carry_entity_mode.h` and `victim_status.h` supply source engine/UI boundaries. Required services are explicit; actual scene producer scheduling is still absent.
- The scene renderer now consumes `VictimRuntimeActor` frame/animation/flags. `--victim-transactions-check` runs both actual Training pickup scripts and native BSP release, status/membership/counter/exit cores with controlled UI services. See `evidence/victim-ai-audit/composed-transactions.md`.

## Validation and remaining gaps

Fifteen CTest checks pass. Existing pickup message/14-case geometry checks pass. The prior arithmetic milestone passed fourteen broader selected checks; timed-transition `--view-update-check` and `--view-item-offset-chain-check` remain red with identical old/new arithmetic results. They are existing parity gaps, not passing checks. Details are in `release-frame-component.md`. The prior two-frame native render check drew 90 textured mesh groups with OpenGL error 0 after display-service access.

Next work:

1. Bind the composed delivered transactions to actual engine/status/statistic-name/item services and source producer/route delivery and remaining passout tail; do not redo completed frame recovery.
2. Resolve Player/victim/BModel root callback scheduling and continuous carry. Source local call chains do not establish recurring cadence. Pickup proximity is before owner message drain, but its order relative to movement remains unresolved.
3. Resolve recurring Fire Think `00449FE0` dispatch and relative frame order, then integrate recovery/spread/douse/particles and their RNG consumers. Bounded static audits have not identified its dispatcher.
4. Complete scripts, BModel carry/throw/axe, hose, damage, goggles, sounds, mission completion and full Training replay parity. Never insert guessed RNG draws to compensate for missing consumers.

## Reverse-engineering and user constraints

Use `tools/ghidra/README.md`. Preserved-project queries are `-noanalysis -readOnly` with JDK 21; copy to scratch if locked. Fresh analysis needs JDK 25. Preserve binary hashes, function addresses, query scope and negative-search limits. Missing direct edges do not exclude computed dispatch.

Exact bundled EXE SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`; DLL: `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`. The current EXE hash corrects an older transcription that omitted a character; the oracle verifies actual program/input hashes.

No live game capture succeeded. Earlier WineDbg attaches froze/crashed; do not attach or ask the user to enter Training without a safe bounded capture/recovery method. Continue static/oracle work meanwhile.

The user requested GPT-6 Luna implementation delegation, lead ownership of complex source/integration decisions, preserved discoveries beyond Training, occasional milestone pushes, and pausing at 1% remaining in either usage window. Recheck usage during extended work. Latest observation: 61% five-hour used, 59% weekly used.
