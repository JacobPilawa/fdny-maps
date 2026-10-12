# WorldEntity delayed callback dispatch boundary

## Instance state and callback behavior

Pinned input: `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

WorldEntity constructor `0049E240` installs root vtable `004E4458` (raw store at `0049E276`), keeps the selected path at `this+0x24`, initializes `this+0x28` to 0, and sets `this+0x2C` to 4. Vtable slot `+0x14` points to `0049E2C0`; slot `+0x20` points to BSP/world initialization method `0049E4D0`.

`0049E2C0` returns if `this+0x28` is already set. Otherwise it decrements `this+0x2C`; when the value reaches zero it calls `00471310(this+0x24, 0)` to load the full EVS script, sets `this+0x28` to 1, and sets application state at `DAT_00502AAC+0x6F0` to 3. This proves a one-shot action after four invocations on the constructed instance. It does not define the interval between invocations.

## Scheduler search and false candidates

The general linked-list queue pump `0049C0E0` calls queue-member slot `+4`, then member slot `+4` again to check removal. It does not call WorldEntity slot `+0x14`, so this function's list-pump shape does not establish the WorldEntity callback scheduler.

Two nearby routines are only candidates without a type/registration edge:

- `0049C3D0` is a thin virtual wrapper that calls receiver slot `+0x14`. No evidence ties its receiver or caller to a WorldEntity.
- `0049C630` increments a counter at `+8`; when it exceeds a limit at `+4`, it calls receiver slot `+4` and resets the counter. No evidence ties this cadence limiter to the WorldEntity or its slot `+0x14`.

The mapped app-owner and entity-manager paths do not close the gap: ordinary manager category dispatch is through member slot `+0x48`; staged entity admission invokes slots `+0x20` and `+0x24`; the relevant WorldEntity update slot is `+0x14`. No positive caller path or timing relation has been found. Therefore, only the four-invocation threshold and one-shot effect are established. Do not call it four frames, 120 updates, or any fixed duration without a verified scheduler/runtime trace.

A follow-up audit of the saved whole-program fixed-offset scan found 53 indirect `CALL`/`JMP [reg+0x14]` candidates. The apparent main-frame hit at `0042AED4` dispatches through `*DAT_00503A78`; constructor `004395C0` installs vtable `004E0B44` there, whose slot `+0x14` is input handler `00439330`, not WorldEntity. The generic wrapper `0049C3D0` has no incoming references in its saved xref query. Two hits near world loading (`0049EE0D` and `0049EECE`) have generic indirect-call context but no verified WorldEntity receiver or recurring schedule. This search narrows the static candidates without proving a dynamic caller. See the archived [`fixed-offset-call-candidates.md`](../../../../decomp/evidence/scriptactor-common-scheduler-audit/fixed-offset-call-candidates.md).

## Provenance and next work

The constructor and callback pseudocode are saved in [`0049e240_world_startup_constructor.c`](0049e240_world_startup_constructor.c) and [`0049e2c0_delayed_script_load.c`](0049e2c0_delayed_script_load.c). The queue pump and candidate wrappers are same-hash archived Ghidra exports at [0049C0E0](../../../../decomp/evidence/startup-world/0049c0e0_FUN_0049c0e0.c), [0049C3D0](../../../../decomp/evidence/startup-world/0049c3d0_FUN_0049c3d0.c), and [0049C630](../../../../decomp/evidence/startup-world/0049c630_FUN_0049c630.c). The WorldEntity vtable/factory chain is in [world_factory_resolution.md](world_factory_resolution.md).

Next, identify the caller that dispatches WorldEntity root slot `+0x14`, and compare its receiver registration and ordering with the owner manager, staged-entity service, and parser load. If static xrefs remain indirect, use a bounded runtime trace only after the Wine launch and capture setup are verified.
