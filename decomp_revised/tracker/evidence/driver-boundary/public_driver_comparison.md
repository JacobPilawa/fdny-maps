# Public Genesis3D driver source comparison

Checked 2026-10-12. This is a source-provenance and reuse assessment, not a license opinion or proof of binary identity.

## Public source identity

The [RealityFactory/Genesis3D repository README](https://github.com/RealityFactory/Genesis3D) says its source is based on Genesis3D 1.1, released November 15, 1999, and includes numerous modifications and improvements from various authors. The repository currently exposes a `master` branch and its root page reports 15 commits. Its [Engine/Drivers tree](https://github.com/RealityFactory/Genesis3D/tree/master/Engine/Drivers) contains separate `D3D7xDrv`, `D3DDrv`, `OpenGl`, and `WireFrame` directories, plus shared `Bmp.c` and `Dcommon.h` files. This makes the tree a plausible implementation reference for legacy Genesis renderer concepts, but the README itself rules out treating it as an untouched canonical 1.1 snapshot.

The public `D3D7xDrv` directory exposes a project file and source files such as `D3d_main.cpp`, `D3ddrv7x.cpp`, `Render.cpp`, and `Scene.cpp` ([directory listing](https://github.com/RealityFactory/Genesis3D/tree/master/Engine/Drivers/D3D7xDrv)). That is useful evidence of a renderer architecture and platform-era dependencies. It is explicitly the D3D7x driver, so it must not be conflated with the game's `D3DDrv.dll` just because the names are similar. The `D3DDrv` nested GitHub page did not load in this inspection; therefore I have not verified its file list, public `DriverHook` declaration, or implementation.

## Comparison to the pinned FDNY DLL

The pinned game DLL is `D3DDrv.dll`, SHA-256 `7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7`. Its only export is ordinal 1 `DriverHook` at RVA `0x6300`. Raw instructions show the function takes a pointer argument, writes `0x1001F040` through it, sets several internal globals, logs `D3DDrv:  No error.`, and returns `1`; the pointer's structure/layout is not yet known. See [`d3ddrv_driverhook_boundary.md`](d3ddrv_driverhook_boundary.md).

The public tree and local DLL share a plausible engine/driver lineage by name and purpose, but the evidence currently stops there:

- No source-to-binary match has been shown (no matching hash, reproducible build, symbol map, or distinctive code/data match).
- No `DriverHook` prototype or returned-table definition has been compared to the FDNY output pointer. Since the public `D3DDrv` directory was not inspectable through the available web page, interface/layout equivalence is **unknown**, not a match.
- FDNY's DLL imports DirectDraw. The public D3D7x file names indicate a Direct3D 7-era implementation, but that is not enough to identify the target DLL's graphics API or build revision.
- Shared export naming, driver names, or similar source file names are weak lineage clues and do not prove compatible table layouts, calling conventions, renderer semantics, or source reuse suitability.

Conclusion: use the public tree to generate hypotheses and understand Genesis-style renderer responsibilities after independently inspecting its `D3DDrv` sources. Do not substitute it for the pinned driver or use it as a direct port component based on the evidence available now.

## License constraints to resolve before reuse

The repository includes [Genesis3D Public License 1.01](https://github.com/RealityFactory/Genesis3D/blob/master/g3dlicense.txt). Its text says modifications, excerpts, or use of modified engine source trigger source availability terms (preamble); section 3.3 requires source for modifications to be made available to executable recipients under that license, and section 3.7 sets notice requirements for executable distribution. Section 4.1 also specifies Genesis3D logo obligations. The license defines Covered Code broadly, including source that duplicates, enhances, modifies, or replaces covered functionality (see Exhibit B for its functional scope). These are material constraints for a public cross-platform port. This note is not legal advice; do not copy source or derive implementation from it for distribution until provenance and licensing have been reviewed for the intended project.

## Next comparison that would answer the open questions

1. Retrieve and pin the public repository commit SHA; inspect `Engine/Drivers/D3DDrv` (not only `D3D7xDrv`) and its shared headers/project files.
2. Find the public `DriverHook` declaration and call sites, record its calling convention and output-table type, then compare table size/order and function pointers against FDNY's `DriverHook` output and the engine's calls. The FDNY caller/table analysis is separate work and remains pending.
3. Compare distinctive FDNY strings, import set, global initialization pattern, and function-level code against that exact public commit. Only a strong set of correspondences would establish version lineage; exact source correspondence needs stronger evidence.
4. Keep algorithm/API study independent from code copying until a source-reuse and license decision is made.

No external Genesis3D source was copied into this workspace.
