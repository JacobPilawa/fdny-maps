# Idle loop and owner-dispatch evidence

All fresh Ghidra function files and memory reads here came from read-only queries against project `FDNYPort`, program `FDNYFirefighter.exe`, Ghidra 12.0.4, on the exact executable hash:

`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`

The `.c` files are Ghidra pseudocode, not recovered source. JSON files are raw Ghidra MCP memory reads. `.objdump.txt` files are GNU objdump Intel-syntax disassemblies of the copied executable. Ghidra names/types remain provisional unless supported by vtable/data/caller evidence.

## Files

- `00499410_app_idle_loop.c`: Windows message pump and idle dispatch.
- `0049a310_app_prepass.c`, `00499e60_app_postpass.c`: registered-object pre/post callback traversals.
- `004df958_app_callback_vtable.json`: establishes app owner's `+0x60` entry as `0x0042BC60`.
- `0042bc60_manager_update_thunk.objdump.txt`: six-byte owner thunk to `FUN_0042AEA0` through subsystem pointer `+0xB4`.
- `0042aea0_game_update.c`, `00429520_world_owner_pass.c`: normal game update and owner-manager order.
- `0042d8c0_game_input_camera_update.c`, `callers_0042d8c0_game_input_camera_update.txt`: second frame-dispatch caller; reads mouse bindings, clamps cursor coordinates, updates the player/camera actor pose, then dispatches an input sink. Its sole caller is `FUN_0042AEA0`, under a guard-byte branch that returns before the ordinary manager/render path.
- `0042b4e0_subsystem_startup.c`, `0042b860_subsystem_constructor.c`: subsystem component construction and registrations.
- `0049bf10_category_manager_constructor.c`, `004e4124_category_dispatch_vtable.json`, `0049bd20_entity_category_dispatch.objdump.txt`: manager setup and manually bounded category dispatch code.
- `0044a330_firerescue_constructor.c`, `0049d330_category_bit_setter.c`, the two FireRescue vtable JSON files, and `0043afe0_*`/`00449fe0_*` files: local FireRescue callback routes and boundaries.
- `00470740_status_gated_update.c`: shared wait/state gate and visual subobject callback.
- `00499c70_singleton_initialize.c`: singleton pointer and selected fields initialized from `FUN_00499280`.
- `0043e360_time_command_append.c`, `0043e680_script_time_commands.c`, `0043f340_time_command_owner_constructor.c`, and matching `xrefs_to_*` files: fresh Ghidra output documenting a same-offset timed-script list whose object relationship to the renderer context is unproven and whose record layout differs from callback elements.
- `0043e360_time_command_list_false_match.md`: documents why the same-offset timed-script list is not evidence of the pre-frame callback producer.

`0x0042BC60`, `0x0043AFE0`, `0x00449FE0`, `0x004483A0`, and `0x0049BD20` are not fresh Ghidra function-manager entries. Their old decompilation exports, where available, remain under `reference/previous-work/decomp/evidence/`. They are excluded from `tracker/function_reviews.csv` and the dashboard's reviewed-function count.
