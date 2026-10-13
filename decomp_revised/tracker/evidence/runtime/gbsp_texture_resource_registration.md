# BSP texture and bitmap resource registration trace

## Provenance and scope

Read-only Ghidra trace against `FDNYFirefighter.exe`, x86:LE:32, image base
`0x00400000`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
This follows the renderer's bitmap resource registration around BSP world
creation. It is separate from the portable scene preview's SDL/OpenGL texture
upload and does not prove the preview's GL draw path is visually correct.

This pass reuses the existing hash-pinned decompilation files
[`00495210_bsp_world_loader.c`](../genesis-boundary/00495210_bsp_world_loader.c),
[`00494ad0_texture_bitmap_loader.c`](../genesis-boundary/00494ad0_texture_bitmap_loader.c),
and [`00493460_bitmap_file_loader.c`](../genesis-boundary/00493460_bitmap_file_loader.c).
It ties those already reviewed functions to the `.rrf` record parsers and the
post-world resource pass; those three function reviews were not duplicated in
the tracker.

## BSP load and resource handoff

`00495210` receives a BSP path, probes it through `004097F0` (the same
`FindFirstFileA` wrapper identified in the catalog trace), and logs
`BSP file not found: <path>` from `ROC\\Renderer\\ROCRender.cpp:0x52` on failure.
It opens the path through `geVFile_OpenNewSystem`, calls `geWorld_Create`,
closes the VFile, and logs `Level load failed: <path>` at source line `0x5b`
if world creation returns null. On success, it calls `geEngine_AddWorld` and
then `00494F00` with one of two renderer-resource subobjects selected by the
manager flag at `this+0x160`.

`00494F00` is a post-world resource pass. When its resource-state field is
nonzero, it walks one pending array and calls `00494D90`, clears that array,
then walks a second bitmap array and calls `00494AD0` for each item before
clearing that array. The array element layout and state-field meaning are not
fully established here.

## `.rrf` bitmap records

The `.rrf` dispatch trace in
[`lev_rrf_catalog_loader_trace_fresh.md`](../scenario-loading/lev_rrf_catalog_loader_trace_fresh.md)
identifies `ROC_BITMAP` records at `0049AEE0` and `ROC_ALPHA_TEXTURE` records at
`0049AFE0`.

- `0049AEE0` consumes the bitmap record's integer and quoted path, then calls
  `00493CC0` on the renderer resource manager. That wrapper chooses a resource
  subobject using `this+0x160` and calls `00493460`.
- `00493460` opens the supplied path through `geVFile_OpenNewSystem`, calls
  `geBitmap_CreateFromFile`, stores the bitmap with its integer resource key in
  the manager array at `this+0x60`, registers it through
  `geEngine_AddBitmap(engine, bitmap)`, and closes the VFile.
- `0049AFE0` reads its integer and one or two quoted paths, then calls
  `00495040`. Both branches of `00495040` call `00494AD0` with the supplied
  paths.
- `00494AD0` probes the base and optional alpha paths and emits
  `Texture bitmap not found: <path>` or
  `Texture alpha bitmap not found: <path>` from
  `ROC\\Renderer\\ROCRender.cpp` lines `0x23a`/`0x23c`. In its world-bitmap
  branch it opens the VFiles, calls `geBitmap_CreateFromFile`, registers the
  bitmap with `geWorld_AddBitmap`, calls `geBitmap_SetAlpha` when an alpha path
  is supplied, and closes the VFiles. The alternate branch stores a path-based
  resource object for later handling.

## Implications for the portable preview

The original pipeline distinguishes engine-wide `ROC_BITMAP` resources from
world-scoped bitmap/alpha resources and checks authored file paths before use.
The current preview already decodes the BSP-exported base BMPs and now reports
OpenGL upload failures per resolved asset. It does not yet apply alpha bitmap
pairs or recreate the engine's resource-key lookup. Those are follow-up parity
items after the visible walkable scene MVP is verified.

The source `.rrf` parser's field semantics, the two renderer subobjects' exact
roles, the deferred registration state, and the relationship between these
resources and every BSP surface's final material are still open. No claim is
made that this trace fully reconstructs the renderer or explains the earlier
onscreen texture report.
