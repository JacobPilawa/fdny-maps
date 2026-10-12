# Genesis3D source and reuse assessment

Checked 2026-10-12. This is an engine-boundary research note, not an implementation or a license conclusion.

## What is available

The public [RealityFactory/Genesis3D repository](https://github.com/RealityFactory/Genesis3D) contains substantial C engine code grouped into Actor, Engine, Entities, Math, Physics, VFile, and World areas. Its README says it is based on Genesis3D 1.1, released in 1999, with community changes. The source tree also includes `Engine/Drivers` directories for `D3D7xDrv`, `D3DDrv`, `OpenGl`, and `WireFrame`. These are useful comparison sources for API behavior, layouts, algorithms, data formats, and legacy renderer interfaces.

A focused comparison note at [`public_driver_comparison.md`](../tracker/evidence/driver-boundary/public_driver_comparison.md) records what the public tree does and does not establish. The `D3DDrv` nested source listing was inaccessible during that check, so its `DriverHook` declaration and output-table type remain unverified. An additional ecosystem search found Styx3D, whose README identifies it as a Jet3D rework and leaves SDL3/software-driver work incomplete; Jet3D is the separate Genesis3D 2.x C++ line, so Styx3D is not evidence of a ready Genesis3D 1.1 driver port. No source was copied.

The pinned FDNY EXE imports 116 symbols from `Genesis.dll`; that DLL exports 536 names. The names include `geWorld_Collision`, actor operations, transforms, sound, and `geVFile` operations. The local import/export inventories are the authority for which APIs this game actually uses. The checked-in `D3DDrv.dll` exports only `DriverHook` and imports DirectDraw. Raw Genesis instructions now show `FUN_10023950` loading candidate DLLs, resolving `DriverHook`, checking table fields at `+4` and `+8` for values 101 and 1, then invoking table method `+0x14`. An engine callback copies subdriver records and invokes method `+0x18`, which copies mode records. This is consistent with temporary driver enumeration; the later selected-driver load and lifetime of the saved table pointer remain open. The focused traces are [`d3ddrv_driverhook_boundary.md`](../tracker/evidence/driver-boundary/d3ddrv_driverhook_boundary.md) and [`driverhook_caller_table_trace.md`](../tracker/evidence/driver-boundary/driverhook_caller_table_trace.md).

## What is not established

- No exact Genesis3D 1.1 build identity or source match has been proved for FDNY's `Genesis.dll`.
- The public driver directory names do not establish that any checked-in driver matches FDNY's `D3DDrv.dll`; no binary/source identity or drop-in compatibility has been verified.
- The public `D3DDrv` source listing and its `DriverHook` declaration have not yet been inspected; the interface comparison remains open.
- The community source includes modifications; matching function names do not prove matching implementation or binary layout.
- The source tree is not a drop-in cross-platform runtime. FDNY-specific modifications, dependencies, and its graphics/audio/platform services still need comparison.
- The focused search found no FDNY-specific recovered Genesis or driver source.

The current [official Genesis3D site](https://genesis3d.com/) states that Genesis3D 1.1 may be used without licensing obligations. The RealityFactory source snapshot still includes the historical Genesis3D Public License and contains later community contributions. Record the exact source commit, file provenance, and applicable terms for any code brought into a distributable port; do not infer that the separate Jet3D/Styx3D line is interchangeable. No public code has been copied here. The historical license link and current official statement are in [REFERENCES.md](REFERENCES.md).

## Next comparison

Inspect and pin the public `Engine/Drivers/D3DDrv` source, find its `DriverHook` declaration and returned table, then compare its calling convention, version fields, and method order against the now-mapped FDNY call path. In parallel, compare the game's imported `geWorld_Collision`, `geActor`, `geVFile`, `geCamera`, and `geEngine` callsites against public headers and implementation. Record exact source revision, signatures, structure fields, binary-version differences, and direct callsite behavior. Use public source to form hypotheses; confirm them against FDNY's pinned modules. Keep renderer migration separate from gameplay semantics.

## First callsite comparison

The fresh EXE crosswalk is in [`tracker/genesis_api_crosswalk.csv`](../tracker/genesis_api_crosswalk.csv), with query outputs in [`tracker/evidence/genesis-boundary/README.md`](../tracker/evidence/genesis-boundary/README.md).

- **VFile:** the public header declares `geVFile_OpenNewSystem(FS, FileSystemType, Name, Context, OpenModeFlags)`. The BSP loader passes DOS type `1`, a path, null context, and read-only flag `1`, then passes the open handle to `geWorld_Create` (confirmed from x86 argument setup). It closes the VFile after world creation. Bitmap loaders use the same virtual-file family and hand decoded bitmaps to the engine or world.
- **Collision:** six callers pass segments and, in some cases, extents to `geWorld_Collision`. The EXE-side wrappers reveal several different gameplay uses, including FireRescue ground placement. The public source can help clarify parameter meanings, but the game's flags and results must be checked against the supplied DLL and geometry.
- **Actor posing:** three callers pass motion, time, and transform data to `geActor_SetPose`; one also sets scale. The public actor header describes this interface, but it cannot establish when the game schedules these calls.
- **Rendering:** the game has a scene render path that brackets object callbacks with `geEngine_BeginFrame`/`geEngine_EndFrame` and calls `geEngine_RenderWorld` separately through `FUN_00492C40`. The render object's vtable calls and current scene ordering are saved in the evidence bundle.

This is useful for scoping port interfaces: file access, collision, actor poses, and rendering all cross the `Genesis.dll` boundary. It does not show that Genesis3D's public source is an exact match or that the engine layer is the only platform-specific part.

No external engine files have been copied into the workspace or incorporated into the port.
