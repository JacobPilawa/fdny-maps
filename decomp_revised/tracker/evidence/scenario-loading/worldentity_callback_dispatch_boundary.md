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

WorldEntity constructor `0049E240` reaches common base initialization
(`root +0x14 = 0xFFEB`) and then ORs value `0x14`, producing mask `0xFFFF`.
Its root vtable slot `+0x14` is `0049E2C0`. The selected-level World factory
path creates it through `0049E300`; the generic spawn queue `0049C670` calls
`00429490` on every returned root. That registration compares `0xFFFF` with
owner `+0x50` filter value 2 and appends the WorldEntity to the active Think
list. The previous scheduler note's unresolved WorldEntity registration edge
is closed by the [World factory trace](world_factory_resolution.md) and
[spawn-queue export](0049c670_entity_spawn_queue_fresh.c).

The normal owner pass `00429520` invokes managers, including `+0x50`, before
its `+0x3C0` entity spawn queue. A WorldEntity created by that queue therefore
first receives root slot `+0x14` on a later normal owner invocation. Each
`0049E2C0` invocation decrements the initialized counter from 4; the fourth
Think callback after registration loads the EVS through `00471310` and sets
application state 3. This establishes four normal Think callbacks, not four
rendered frames or any fixed elapsed duration. The `0042AEA0` early `+0x64C`
branch returns before the manager pass, so skipped invocations do not advance
the counter.

The whole-program fixed-offset scan found 53 indirect `CALL`/`JMP
[reg+0x14]` candidates. Its apparent main-frame hit at `0042AED4` dispatches
through `*DAT_00503A78`; constructor `004395C0` installs vtable `004E0B44`,
whose slot `+0x14` is input handler `00439330`, not a WorldEntity. The two
world-load candidates (`0049EE0D` and `0049EECE`) still lack a verified
WorldEntity receiver. The cadence limiter at `0049C630` likewise has no
registration edge to WorldEntity. See the archived
[`fixed-offset-call-candidates.md`](../../../../decomp/evidence/scriptactor-common-scheduler-audit/fixed-offset-call-candidates.md).

The active-list membership and four normal Think callbacks are statically
established for a WorldEntity created by the selected-level factory path. The
interval between those callbacks and runtime success of the EVS load remain
unverified. Do not convert the count to a frame duration.

## Provenance and next work

The constructor and callback pseudocode are saved in [`0049e240_world_startup_constructor.c`](0049e240_world_startup_constructor.c) and [`0049e2c0_delayed_script_load.c`](0049e2c0_delayed_script_load.c). The queue pump and candidate wrappers are same-hash archived Ghidra exports at [0049C0E0](../../../../decomp/evidence/startup-world/0049c0e0_FUN_0049c0e0.c), [0049C3D0](../../../../decomp/evidence/startup-world/0049c3d0_FUN_0049c3d0.c), and [0049C630](../../../../decomp/evidence/startup-world/0049c630_FUN_0049c630.c). The WorldEntity vtable/factory chain is in [world_factory_resolution.md](world_factory_resolution.md).

Next, audit any remaining root classes whose factory paths are not yet joined
to `00429490`, and keep the interval between World Think callbacks open until
runtime scheduling can be captured.
