# EXE-to-Genesis boundary evidence

This bundle records a selected crosswalk from `FDNYFirefighter.exe` into `Genesis.dll`. Ghidra MCP calls were read-only, targeted `FDNYFirefighter.exe` in the clean `FDNYPort` project, and were checked against the copied executable SHA-256:

`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`

The decompiler outputs are pseudocode, not recovered source. Function names are provisional Ghidra labels. The `callers_*.txt` files are direct caller-list responses. `00495210_bsp_world_loader.disassembly.txt` preserves the x86 stack/register setup needed to see the VFile handle passed to `geWorld_Create`; the decompiler's imported prototype did not show that argument.

## Contents

- `004928b0_vfile_open_wrapper.c`, `00493460_bitmap_file_loader.c`, `00494ad0_texture_bitmap_loader.c`, `00495210_bsp_world_loader.c`: four `geVFile_OpenNewSystem` callers.
- `00492950_empty_world_create.c`: a separate empty-world creation path.
- `00448630_world_collision_caller.c`, `0046c5a0_world_collision_caller.c`, `0048fea0_world_collision_caller.c`, `0048ff30_world_collision_caller.c`, `00490000_world_collision_caller.c`, `00492ce0_collision_wrapper.c`: all six `geWorld_Collision` callers.
- `00480d50_actor_pose_adapter.c`, `00492410_actor_pose_adapter.c`, `00492450_actor_scale_pose.c`: all three `geActor_SetPose` callers.
- `00492c40_world_render_callsite.c`, `00494670_frame_dispatcher.c`, `004943e0_preframe_callbacks.c`, `0042d1b0_frame_bitmap_display.c`: selected renderer and frame calls, plus a pending callback list that is a candidate path for the unresolved visual update schedule.
- `callers_*.txt`: Ghidra MCP's direct incoming-caller responses for selected imported APIs and the pre-frame helper/dispatcher.

## Limits and next question

The public `RealityFactory/Genesis3D` tree identifies itself as based on Genesis3D 1.1, with later community modifications. Its [VFile header](https://github.com/RealityFactory/Genesis3D/blob/master/VFile/vfile.h) provides the reference function declaration and constants; its [Actor header](https://github.com/RealityFactory/Genesis3D/blob/master/Actor/actor.h) describes `geActor_SetPose`. These sources clarify an API shape, not the exact shipped DLL implementation.

The `FUN_004943e0` list at the selected world subobject's `+0x94` is drained before the render-object callbacks. Its element vtable `+0x08` method has not been identified, and no evidence yet ties this list to FireRescue. Find the code that populates this list and verify the stored object/vtable before calling it a Think schedule.
