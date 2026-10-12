# Public Genesis3D driver source comparison

Checked 2026-10-12. This is a source-provenance and interface-comparison note, not a license opinion or proof of binary identity.

## Public source identity and inspection limit

Repository: [RealityFactory/Genesis3D](https://github.com/RealityFactory/Genesis3D), branch `master`, paths of interest `Engine/Drivers/D3DDrv/`, `Engine/Drivers/D3DDrv/` project/source files, and shared `Engine/Drivers/Dcommon.h`. The repository page reports 15 commits and the README describes the tree as based on Genesis3D 1.1 (released 1999-11-15) with community modifications. The [driver parent directory](https://github.com/RealityFactory/Genesis3D/tree/master/Engine/Drivers) confirms sibling `D3D7xDrv`, `D3DDrv`, `OpenGl`, and `WireFrame` directories and shared `Bmp.c`/`Dcommon.h`.

The inspected source revision is **not pinned to a commit SHA**: GitHub's repository page exposed branch `master`, but its commit identity was blank in the available rendering; the nested [`Engine/Drivers/D3DDrv`](https://github.com/RealityFactory/Genesis3D/tree/master/Engine/Drivers/D3DDrv) page failed to load, and raw-file fetches were unavailable. Shell access also could not resolve GitHub. I therefore could not inspect any leaf file or establish a revision-specific interface. Treat this comparison as incomplete and do not cite `master` as an immutable source pin. The exact source files to retrieve at a pinned commit are the `D3DDrv` entry/driver table implementation and its headers, plus `Dcommon.h` and the `D3DDrv` project file.

The public [Genesis3D driver documentation](https://genesis3d.com/old/docs/Drivers.htm) describes an original D3D renderer requiring DirectX 6 or better and DirectDraw surfaces for fullscreen mode. This is consistent with the general era and API family seen in the local DLL, but it is documentation for the Genesis3D SDK, not the inaccessible RealityFactory source files and not proof of shared implementation.

## Local pinned evidence

The local `D3DDrv.dll` SHA-256 is `7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7`. It exports only ordinal 1 `DriverHook` at RVA `0x6300`; raw instructions show one pointer argument, a write of `0x1001F040` through that pointer, several global writes, a log call, and return value `1`. The returned table has version values `101` and `1` at `+4` and `+8`; its observed function pointers include `+0x14 -> 0x10004C20`, `+0x18 -> 0x10004DA0`, and `+0x20 -> 0x10005DD0`. These slots are observed from pinned DLL data, not source field declarations. Genesis validates the version fields, writes callbacks at `+0xB0/+0xB4`, and invokes `+0x20` during selected-driver initialization. See [`d3ddrv_driverhook_boundary.md`](d3ddrv_driverhook_boundary.md) and [`selected_driver_initialization_trace.md`](selected_driver_initialization_trace.md).

## Match assessment

- **Lineage:** plausible. The project identifies itself as a Genesis3D 1.1-derived engine, and the official driver docs describe a DirectDraw-era D3D renderer. The FDNY DLL is named `D3DDrv.dll` and imports `DirectDrawCreate` and `DirectDrawEnumerateA`. These align at a broad API/era level.
- **Driver generation:** unresolved. The public tree has both `D3DDrv` and `D3D7xDrv`; the latter must not be substituted based on the similar name. The source for the former was not readable in this check.
- **`DriverHook` ABI/table:** unknown. No public prototype, calling convention, table type, field order, version constants, or slot semantics could be compared with the local one-argument export and the engine's observed table calls.
- **Source/binary identity and reuse:** not established. There is no matching build ID, symbol map, reproducible build, or distinctive code correspondence. Do not call the local DLL a build of this repository and do not copy or mechanically port public driver code on this evidence.

## Provenance and license notes

The repository README attributes the source to Genesis3D 1.1 plus later community modifications. The repository contains [`g3dlicense.txt`](https://github.com/RealityFactory/Genesis3D/blob/master/g3dlicense.txt), identified in the prior workspace review as Genesis3D Public License 1.01. The current [official Genesis3D site](https://www.genesis3d.com/) says Genesis3D 1.1 may be used without licensing obligations, while the community repository includes later contributions and a license file. Those statements do not resolve the provenance or terms of every file in the repository. Before any reuse in a distributed port, pin and inspect the exact source commit and per-file notices/license text, then review the intended use. No external source was copied into this workspace.

## Next safe query

Obtain the repository at a full commit SHA and inspect the `Engine/Drivers/D3DDrv` implementation/header/project plus `Engine/Drivers/Dcommon.h`. Record the exact `DriverHook` signature, calling convention, table declaration and version fields. Then compare those against local raw evidence: export argument at `[esp+4]`, out-pointer write, versions at table offsets `+4/+8`, engine callback writes `+0xB0/+0xB4`, and calls at `+0x14/+0x18/+0x20`. If a source copy is unavailable, retain the ABI result as unknown; do not infer equivalence from filenames or documentation. This is a source retrieval/static comparison only; no game launch, dashboard change, or Ghidra server change is needed.
