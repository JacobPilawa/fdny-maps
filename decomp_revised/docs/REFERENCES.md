# Research and references

Checked 2026-10-12. The requested `decomp.dev/projects` page blocked automated retrieval through the browser, so the project layout is being used as a visual reference and technical guidance below comes from accessible project documentation and source repositories.

## Decompilation and progress tracking

- [decomp.dev projects](https://decomp.dev/projects) — the requested project list and visual reference.
- [Decompedia: matching decompilation](https://github.com/decompals/decompedia/blob/main/home.md) — defines matching decompilation as source compiling to the exact original binary and notes the compiler/toolchain constraint.
- [Black & White decompilation getting started](https://github.com/openblack/bw1-decomp/blob/main/docs/getting_started.md) — a practical matching workflow using binary configs, split ranges, symbol names, object diffs, and an explicit per-function match gate.
- [Tenchu decompilation notes for decomp.dev](https://github.com/Fuuzetsu/tenchu-decomp/blob/master/docs/decomp-dev.md) — explains function-level report units, status categories, and how a dashboard consumes generated progress reports. It also shows why that dashboard's binary-match values should not be reused as port-parity percentages.
- [decomp.dev source repository](https://github.com/encounter/decomp.dev) — implementation of the progress site and GitHub integration.

## Ghidra and the MCP bridge

- [Ghidra MCP by bethington](https://github.com/bethington/ghidra-mcp) — requested bridge; its current setup uses an installed Ghidra extension and an `uv` stdio bridge for an AI client.
- [Ghidra](https://github.com/NationalSecurityAgency/ghidra) — official reverse-engineering suite and source.
- [OpenAI Codex configuration reference](https://learn.chatgpt.com/docs/config-file/config-reference) — Codex supports project-scoped `.codex/config.toml`; the project must be trusted for its configuration to load. The config can allowlist MCP tools and set approval behavior.

## Genesis3D source and engine boundary

- [RealityFactory/Genesis3D source repository](https://github.com/RealityFactory/Genesis3D) — its README identifies this tree as based on Genesis3D 1.1 from November 1999, with community modifications. It includes Actor, Engine, Entities, Math, Physics, VFile, and World code. Use it as a candidate API/algorithm reference, not as proof that FDNY's `Genesis.dll` is the same revision.
- [Genesis3D Engine/Drivers source tree](https://github.com/RealityFactory/Genesis3D/tree/master/Engine/Drivers) — the repository contains `D3D7xDrv`, `D3DDrv`, `OpenGl`, and `WireFrame` driver directories. This is a useful implementation reference for the legacy graphics boundary; no exact source match to FDNY's `D3DDrv.dll` has been established.
- [Genesis3D public driver table](https://github.com/RealityFactory/Genesis3D/blob/master/Engine/Drivers/Dcommon.h) and [official driver documentation](https://genesis3d.com/old/docs/Drivers.htm) — reference for the exported `DriverHook` table and its legacy Direct3D/software driver boundary. The public table version differs from FDNY's checked version.
- [Current Genesis3D site](https://genesis3d.com/) — distinguishes Genesis3D 1.x (C) from Jet3D 2.x (C++) and currently states that Genesis3D 1.1 may be developed with free licensing obligations. The RealityFactory repository still includes an older license text and community changes, so record the exact source tree and applicable statement before redistribution.
- [Jet3D history](https://www.jet3d.com/jet3d_history.html) — describes Jet3D as a release of work descended from the in-progress Genesis3D 2.0 code, distinct from the 1.1 source.
- [Styx3D](https://github.com/8bitprodigy/Styx3D) — an active small modernization project whose README says it reworks Jet3D and lists SDL3/software-driver and OpenGL 1.1 work as incomplete/planned. Useful ecosystem lead, but not a verified Genesis3D 1.1 or FDNY driver replacement.
- [Genesis3D VFile header](https://github.com/RealityFactory/Genesis3D/blob/master/VFile/vfile.h) — declares `geVFile_OpenNewSystem` and the DOS/read-only constants used to interpret the BSP loader callsite.
- [Genesis3D Actor header](https://github.com/RealityFactory/Genesis3D/blob/master/Actor/actor.h) — describes `geActor_SetPose` and distinguishes actor definitions from actor instances.
- [Genesis3D GBSP v15 structures](https://github.com/RealityFactory/Genesis3D/blob/f85b288ff54873e93879791ee9eb1367a311909f/World/Gbspfile.h) — defines the chunk IDs and serialized BSP record layouts used to build the typed MBase reader; this is a strong schema match, not an exact FDNY binary match.
- [Genesis3D BSP surface projection](https://github.com/RealityFactory/Genesis3D/blob/f85b288ff54873e93879791ee9eb1367a311909f/World/Surface.c) — computes BSP UV inputs from vertices, texture vectors, draw scales, shifts, and image dimensions. Its behavior matched the prototype's Training mesh export.
- [Historical Genesis3D Public License 1.01 in that repository](https://github.com/RealityFactory/Genesis3D/blob/master/g3dlicense.txt) — this source snapshot still contains the old license text. Read it alongside the current official Genesis3D site's 1.1 statement and preserve exact provenance for any community modifications; this project has not copied external source.
- [Genesis3D API overview](https://genesis3d.com/old/docs/EngineAPIs.htm) and [Engine API reference](https://genesis3d.com/old/docs/APIReference/Engine.htm) — candidate API references for comparing FDNY imports and callsites.
- [Genesis3D feature overview](https://genesis3d.com/old/docs/Features.htm) — documents the older engine's driver/platform scope; it is context for the graphics port, not a substitute for inspecting FDNY's `D3DDrv.dll`.

The fresh FDNY executable imports 116 Genesis symbols. `Genesis.dll` exports 536 names; `D3DDrv.dll` exports `DriverHook` and imports DirectDraw. Raw callsite analysis now maps Genesis' five-candidate dynamic driver scan, interface checks, and first table-method invocation. It does not establish a source-to-binary match or that an existing modernized engine is a drop-in for FDNY.

## Retail asset containers

- [Unshield](https://github.com/twogood/unshield) — upstream MIT-licensed tool/library used locally to list and extract the InstallShield cabinets on the user-provided CD image. Its source commit and version are recorded in the reverse-engineering log; the utility was built under `/private/tmp`, not installed globally or copied into this repository.

## LLM-assisted decompilation

- [Teaching an LLM to Decompile the Nintendo 64](https://huggingface.co/blog/MatthewReingold/n64-decomp-dev-blog) — a recent project write-up that sets an evaluation target first, filters on exact-match training examples, and stresses original compiler/flags in recompilation. It is useful for workflow lessons; it is not evidence that an LLM can automatically reconstruct this game's behavior.
- [When LLM Decompilers Recompile More and Preserve Less](https://arxiv.org/abs/2609.05370) — recent research showing that successful recompilation and semantic agreement can diverge. This supports keeping source plausibility, component comparisons, and runtime parity as separate measures.

## How these references apply here

Matching-decomp projects provide useful inventory, symbols, object boundaries, and visual progress patterns. This FDNY project is targeting a modern behavior-preserving port, so its primary completion evidence is function understanding plus original-comparison and whole-scenario parity. The local dashboard follows the visual idea of a function atlas while keeping those measures separate.
