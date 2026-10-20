# WorldEntity delayed callback dispatch boundary

## Instance state and callback behavior

Pinned input: `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

WorldEntity constructor `0049E240` installs root vtable `004E4458` (raw store at `0049E276`), keeps the selected path at `this+0x24`, initializes `this+0x28` to 0, and sets `this+0x2C` to 4. Vtable slot `+0x14` points to `0049E2C0`; slot `+0x20` points to BSP/world initialization method `0049E4D0`.

`0049E2C0` returns if `this+0x28` is already set. Otherwise it decrements `this+0x2C`; when the value reaches zero it calls `00471310(this+0x24, 0)` to load the full EVS script, sets `this+0x28` to 1, and sets application state at `DAT_00502AAC+0x6F0` to 3. This proves a one-shot action after four invocations on the constructed instance. It does not define the interval between invocations.

## Scheduler search and false candidates

`0049C0E0` is the active-list pump for the owner `+0x50` Think manager. Its
manager slot `+4` points to adapter `0049C3D0`, which calls each member's root
slot `+0x14`; the vtable entry is data-bound at `004E4164+4`. The earlier xref
query found no direct code caller because the manager reaches the adapter
through vtable data. See the [root scheduler trace](root_plus14_owner_scheduler_trace_2026-10-20.md).

WorldEntity constructor `0049E240` reaches common base initialization and then
ORs value `0x14` into root `+0x14`; that mask still includes the Think
manager's filter value 2. This makes the instance eligible if it is passed
through `00429490`, but the WorldEntity factory/load path has not yet been
joined to that registration call. Therefore the new manager trace does not
prove that a WorldEntity instance enters the active `+0x50` list, or that its
four-invocation counter is advanced there.

The whole-program fixed-offset scan found 53 indirect `CALL`/`JMP
[reg+0x14]` candidates. Its apparent main-frame hit at `0042AED4` dispatches
through `*DAT_00503A78`; constructor `004395C0` installs vtable `004E0B44`,
whose slot `+0x14` is input handler `00439330`, not a WorldEntity. The two
world-load candidates (`0049EE0D` and `0049EECE`) still lack a verified
WorldEntity receiver. The cadence limiter at `0049C630` likewise has no
registration edge to WorldEntity. See the archived
[`fixed-offset-call-candidates.md`](../../../../decomp/evidence/scriptactor-common-scheduler-audit/fixed-offset-call-candidates.md).

Only the four-invocation threshold and one-shot effect are established for a
constructed WorldEntity. Do not convert the threshold to a frame count or
duration until its active-list membership and runtime cadence are verified.

## Provenance and next work

The constructor and callback pseudocode are saved in [`0049e240_world_startup_constructor.c`](0049e240_world_startup_constructor.c) and [`0049e2c0_delayed_script_load.c`](0049e2c0_delayed_script_load.c). The queue pump and candidate wrappers are same-hash archived Ghidra exports at [0049C0E0](../../../../decomp/evidence/startup-world/0049c0e0_FUN_0049c0e0.c), [0049C3D0](../../../../decomp/evidence/startup-world/0049c3d0_FUN_0049c3d0.c), and [0049C630](../../../../decomp/evidence/startup-world/0049c630_FUN_0049c630.c). The WorldEntity vtable/factory chain is in [world_factory_resolution.md](world_factory_resolution.md).

Next, join WorldEntity factory/load registration to `00429490` or establish
its active-list membership in a bounded runtime trace. Then compare its four
invocations with the normal and early-return owner paths; the call target alone
does not resolve the counter interval.
