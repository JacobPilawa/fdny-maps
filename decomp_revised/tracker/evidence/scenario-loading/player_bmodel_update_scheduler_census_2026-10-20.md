# Player and BModel recurring update scheduler census

## Scope and provenance

Static analysis of `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The earlier read-only direct-call and fixed-offset scans did not identify the
owner manager because dispatch is through a function pointer in manager
vtable data. A fresh saved-project follow-up traced the owner constructor,
manager vtable, registration mask, and member adapter. No retail runtime was
captured. See the consolidated [root `+0x14` owner scheduler trace](root_plus14_owner_scheduler_trace_2026-10-20.md).

## Local callbacks and manager route

The local update routes are:

- Player root slot `+0x14` → `0044FE20` → embedded ScriptActor at `+0x140`
  → `00470740` → Player movement update `00452570`.
- BModel root slot `+0x14` → thunk `0043AFE0` → `00470740` → embedded
  ScriptActor update `0043DC60`.

Owner constructor `0042B860` constructs the manager at owner `+0x50` with
`0049C4E0`. Its filter value is `2`, its primary vtable is `004E4164`, and its
slots `+0`/`+4` point to `0049C0E0`/`0049C3D0`. The manager list pump calls its
slot `+4` for each member; adapter `0049C3D0` then dispatches the member root
slot `+0x14`.

Registration routine `00429490` compares the entity's root `+0x1C` mask with
the four manager filters and appends eligible roots through each manager's
registration interface. Common root initialization `0049D420` sets the
eligibility mask to `0xFFEB`, which includes filter value `0x2`. Both
PlayerRescue and BModel constructors reach this initializer, and the successful
generic spawn path registers them through `00429490`. Their static admission
to the `+0x50` Think list is supported. Actual runtime residence and the order
of individual members within that list remain unobserved.

The normal `0042AEA0 → 00429520` owner pass calls `+0x50` before the `+0xE0`
contact manager. Its early `+0x64C` branch can bypass the manager pass; a
pending level load at `+0x68C` is handled before the pass and falls through.
Player and BModel root `+0x48` contact callbacks are separate from their
`+0x14` Think updates.

## Reconciliation with earlier negative searches

The direct-reference census for `00470740` returned local wrappers/thunks,
not the recurring manager. A whole-program decoded `[reg+0x14]` scan found an
apparent main-loop hit at `0042AED4`, but that dispatches the singleton input
object constructed at `004395C0`, not PlayerRescue or BModel. Neither search
followed the owner manager's data-bound `004E4164+4 → 0049C3D0` adapter, so
those negative results were incomplete rather than evidence against the route.

The same-hash app callback list remains a distinct path: its pre/post passes
use child slots `+0x74`, `+0x70`, and `+0x6C`, then call the owner update at
`+0x60`. No evidence ties PlayerRescue or BModel roots to that list. The owner
`+0x3C4` tail service likewise drains queued item callbacks through item slots
`+0` or `+0x2C`; it is not the direct root `+0x14` Think scheduler.

## Current boundary

The static owner scheduler and coarse manager order are now identified. Exact
wall-clock cadence, behavior across the early-return branch, runtime manager
membership/order, and Windows execution remain unverified. Preserve the host's
paced-loop interval as provisional. The source-backed contact-pair phase delay
is covered in the [owner scheduler trace](root_plus14_owner_scheduler_trace_2026-10-20.md).

Relevant evidence includes [Player local update route](player_update_dispatch_2026-10-20.asm),
[BModel update route](bmodel_update.md), [owner manager order](../runtime-loop/00429520_world_owner_pass.c),
[owner constructor](../runtime-loop/0042b860_subsystem_constructor.c), and the
[owner `+0x3C4` service trace](../runtime-loop/0042b2c0_owner_work_service_constructor_fresh_2026-10-20.c).

No retail runtime capture or parity check was performed in this follow-up.
