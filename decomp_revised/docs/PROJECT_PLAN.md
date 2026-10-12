# Project plan

## Goal

Build a modern desktop port of FDNY Firefighter: American Hero that preserves the original engine behavior and gameplay as closely as practical. The user's success criterion is that people feel they are playing the same game: same systems, rules, timing, levels, and outcomes.

For this project, “decompile” means recovering enough of the game's code and engine behavior to make that port faithful. Rebuilding the exact Windows PE byte-for-byte is a separate, optional goal. It would reproduce the old executable; by itself it would not make the game run natively on modern systems.

## Acceptance target

The first native playable slice should run on macOS so the user can see tangible progress on their machine. Windows and Linux remain required targets for the complete port. Keep Genesis engine behavior and interfaces as the initial reference; map the boundary between `FDNYFirefighter.exe`, `Genesis.dll`, and `D3DDrv.dll`, then choose platform services and graphics/audio interfaces that can support all three systems.

The first slice must exercise a rescue campaign path. The earlier Training reconstruction is a separate gameplay reference and source of reusable mechanics; see the component audit before moving code. It does not set the game's product scope or acceptance target. Military Base (`mbase`), the first rescue entry after Training in `thegame.lev`, is the current candidate based on its authored entity mix. Confirm its texture/model resource dependencies and runtime load path before treating that choice as final.

## Milestones

| ID | Goal | Exit evidence | State |
|---|---|---|---|
| M0 | Pin inputs and target | Local copies, SHA-256 manifest, macOS-first playable sequencing, cross-platform final target, and explicit classification of original files versus compatibility overlays | Complete |
| M1 | Map code-bearing binaries | Fresh hash-pinned function, section, import/export, and prior-analysis crosswalk inventories for all four PE modules; FMOD’s UPX limitation is labeled | Complete |
| M2 | Map engine and runtime architecture | Startup/idle, engine boundary, level-to-EVS path, selected-level `data\\maps\\<key>.bsp` → World factory → WorldEntity/BSP load chain, parser-to-selector collection identity, selector initialization, BoundsTrigger contact-to-event route and manager eligibility, D3DDrv enumeration and selected-driver initialization/fog dispatch mapped; tell argument roles and gameplay callback cadence remain open | In progress |
| M3 | Establish a repeatable Ghidra workflow | Project-local analysis and MCP server; direct bridge handshake registered 180 Ghidra tools | In progress: project config and local service are present; reload/trust the workspace in Codex to expose the configured tool list in-chat |
| M4 | Make a game-wide Mac bootstrap | Load the level catalog and resource/map metadata through reusable code, resolve case-sensitive file paths, and start a non-Training rescue scene from a temporary developer entry point | In progress: the Training workbench contains an integrated gameplay reconstruction and reusable fire/victim/interaction code, but its entry point and data path are Training-specific. MBase's catalog and authored entities are inventoried; geometry/material export and the shared bootstrap are still absent |
| M5 | Reach a rescue campaign slice on Mac | Run one rescue mission with world geometry, camera, movement, collision, fire/victim state, and its defining interaction; compare behavior against original-code evidence or a controlled capture | Not started; MBase is the current candidate based on its first-rescue position and 228 authored BSP entities; texture/model loading and the scene path remain to map |
| M6 | Reconstruct the game flow and scenarios | Recreate menu, briefing, level selection, objectives, mission result, and authored rescue scenarios; validate reusable components against rescue missions first, then retain Training as an optional regression scenario | Not started |
| M7 | Cover the full campaign | Every selectable catalog entry and referenced BSP variant has a documented asset/script relationship, load result, and scenario status | Not started |
| M8 | Complete cross-platform desktop builds | Port the shared runtime to Windows and Linux and run the same scenario suite across macOS, Windows, and Linux | Not started |

## First three concrete goals

1. **Initial module map complete.** The four hash-pinned PE files have fresh Ghidra function/section inventories and import/export lists. The executable has 5,807 body-bearing functions; FMOD remains labeled packed because its supplied UPX image is not a useful body inventory.
2. **Use the refreshed crosswalk.** 763 of 871 prior indexed EXE addresses match fresh function entries. These are address-only joins because the old index does not bind every pseudocode export to a verified executable hash.
3. **Inventory the complete level set.** The game catalog and Training-to-BSP path are statically mapped. A checked-in inventory now reconciles all 12 catalog entries and 17 BSP files with same-key briefings and EVS files, including entity totals and rescue-object counts. MBase is the active first-slice candidate; next map its BSP geometry/material chunks and model/resource dependencies, then build the reusable rescue scene path.
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
- **Native port:** C++17/CMake and SDL2 are candidates because the old Training workbench uses them; neither the runtime architecture nor the renderer is locked. Audit platform-specific code before promoting any component into the game-wide port.
- **LLM assistance:** use it to propose names, call graphs, data structures, and candidate translations. Accept a result only after checking raw instructions, callers/data, and an original-code or runtime comparison appropriate to the claim.

## Decision gates

- After M1: decide which DLLs contain first-party game behavior and which are third-party middleware or wrappers.
- After M2: choose the Mac-first Genesis compatibility strategy and rendering/audio/input boundaries so the shared runtime can later support Windows and Linux.
- Before M4: set minimum OS versions and graphics/audio API requirements for the three desktop targets.
- Before importing old implementation code: confirm its binary hash, scope, build dependencies, evidence, and whether the implementation belongs in the clean port.
- Before promoting Training code into the main port: establish its exact evidence, data assumptions, and platform-specific dependencies, and verify that the component is reusable outside Training.
