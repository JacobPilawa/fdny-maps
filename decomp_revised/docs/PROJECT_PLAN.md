# Project plan

## Goal

Build a modern desktop port of FDNY Firefighter: American Hero that preserves the original engine behavior and gameplay as closely as practical. The user's success criterion is that people feel they are playing the same game: same systems, rules, timing, levels, and outcomes.

For this project, “decompile” means recovering enough of the game's code and engine behavior to make that port faithful. Rebuilding the exact Windows PE byte-for-byte is a separate, optional goal. It would reproduce the old executable; by itself it would not make the game run natively on modern systems.

## Acceptance target

The first playable build must support modern Windows, macOS, and Linux desktop systems. Keep Genesis engine behavior and interfaces as the initial reference. After mapping the boundary between `FDNYFirefighter.exe` and `Genesis.dll`, decide how to preserve those interfaces across all three platforms while replacing platform-specific services as needed. Set minimum OS versions and graphics/audio API requirements after the engine and asset dependencies are mapped.

The first playable target should be Training because the earlier work already contains a partial native Training reconstruction and many original-code comparisons. The first controllable Training scene must boot on Windows, macOS, and Linux from the first playable milestone. Training completion must still be measured as a complete authored run on each target platform, not by adding up isolated function checks.

## Milestones

| ID | Goal | Exit evidence | State |
|---|---|---|---|
| M0 | Pin inputs and target | Local copies, SHA-256 manifest, cross-platform first-playable target, and explicit classification of original files versus compatibility overlays | Complete |
| M1 | Map code-bearing binaries | Fresh hash-pinned function, section, import/export, and prior-analysis crosswalk inventories for all four PE modules; FMOD’s UPX limitation is labeled | Complete |
| M2 | Map engine and runtime architecture | Startup/idle, engine boundary, level-to-EVS path, selected-level `data\\maps\\<key>.bsp` → World factory → WorldEntity/BSP load chain, parser-to-selector collection identity, selector initialization, BoundsTrigger contact-to-event route and manager eligibility, D3DDrv enumeration and selected-driver initialization mapped; unresolved `+0xA8` slot, tell-recipient dispatch, and gameplay callback cadence remain open | In progress |
| M3 | Establish a repeatable Ghidra workflow | Project-local analysis and MCP server; direct bridge handshake registered 180 Ghidra tools | In progress: project config and local service are present; reload/trust the workspace in Codex to expose the configured tool list in-chat |
| M4 | Recover a cross-platform runnable vertical slice | Boot to a controllable scene on Windows, macOS, and Linux with original Training geometry, player movement, camera, and collision, with each system compared against defined reference scenarios | Existing partial work; audit and adapt for all target platforms before import |
| M5 | Complete Training behavior | One authored Training run on each target platform covers doors, tools, fire, victims, rescue/failure, mission result, timing, events, and shared random draws with documented parity | Not started |
| M6 | Expand through the campaign | Each mission has an explicit scenario set and reusable engine/gameplay systems; no level is marked complete from asset loading alone | Not started |
| M7 | Package desktop ports | Clean distributable builds and repeatable scenario checks for Windows, macOS, and Linux; document platform-specific differences and remaining gaps | Not started |

## First three concrete goals

1. **Initial module map complete.** The four hash-pinned PE files have fresh Ghidra function/section inventories and import/export lists. The executable has 5,807 body-bearing functions; FMOD remains labeled packed because its supplied UPX image is not a useful body inventory.
2. **Use the refreshed crosswalk.** 763 of 871 prior indexed EXE addresses match fresh function entries. These are address-only joins because the old index does not bind every pseudocode export to a verified executable hash.
3. **Continue the runtime map.** The level-choice-to-BSP factory chain, catalog ownership, and startup-to-EVS path are mapped in [RUNTIME_MAP.md](RUNTIME_MAP.md). Next resolve Training's `tell player escape` recipient path, then identify BoundsTrigger and WorldEntity updater callers/order and continue the remaining owner and Genesis callback schedule.
## Progress measures

Use separate measures so they cannot be confused:

- **Binary map:** count of code-bearing modules with verified hashes and fresh Ghidra inventories.
- **Function understanding:** counts by evidence state in `tracker/functions.csv`, scoped to Ghidra-defined functions in a named binary snapshot.
- **Port status:** source-backed behavior components, comparisons, and integrated scenarios.
- **Whole-game parity:** completed authored scenarios and platform builds against pinned original conditions.

Do not present a single “percent decompiled” figure until the function denominator and completion meaning are both defensible. An objdiff/decomp.dev compiler-match percentage is not an acceptance metric for the native port.

## Tools

- **Ghidra 12.0.4:** static analysis and durable program database. The user already has Ghidra and a Ghidra MCP extension installed.
- **Ghidra MCP:** AI-assisted queries, renaming, comments, and bounded scripts against the active Ghidra program. Use the project bridge configured in `.codex/config.toml` after the extension server is started.
- **Headless Ghidra scripts:** repeatable inventories and bounded exports into `tracker/generated/`; the project uses a local Ghidra user home to avoid changing the user's global analysis settings.
- **Original-game runtime/debugger:** use only for carefully bounded traces after the launch environment and capture plan are stable. The prior notes report that one WineDbg attempt froze the PE32 game; this is a known constraint, not proof that live tracing is impossible.
- **Native port:** retain C++17/CMake as the provisional first implementation stack because the existing Training prototype uses it. Revisit after mapping the engine boundary and reviewing the other prior prototypes.
- **LLM assistance:** use it to propose names, call graphs, data structures, and candidate translations. Accept a result only after checking raw instructions, callers/data, and an original-code or runtime comparison appropriate to the claim.

## Decision gates

- After M1: decide which DLLs contain first-party game behavior and which are third-party middleware or wrappers.
- After M2: decide the Genesis compatibility strategy and rendering/audio/input backends that can support Windows, macOS, and Linux from the first playable build.
- Before M4: set minimum OS versions and graphics/audio API requirements for the three desktop targets.
- Before importing old implementation code: confirm its binary hash, scope, build dependencies, evidence, and whether the implementation belongs in the clean port.
- Before broadening beyond Training: close one end-to-end Training scenario and its event/time/random transcript.
