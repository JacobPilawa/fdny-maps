# Root `+0x14` owner Think scheduler trace

## Scope and provenance

Static analysis of the copied `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Fresh read-only Ghidra headless output was checked against the saved
`FDNYPort` project and the pinned executable. No retail runtime was captured.

Supporting exports:

- [`0042B860` owner manager construction](../runtime-loop/0042b860_subsystem_constructor.c)
- [`00429520` normal owner manager order](../runtime-loop/00429520_world_owner_pass.c)
- [manager registration, list pump, and base mask](../runtime-loop/00429490_root_manager_registration_fresh_2026-10-20.c)
- [owner manager constructors](../runtime-loop/0049c4xx_owner_manager_constructors_fresh_2026-10-20.c)
- [`0049C3D0` member adapter](../runtime-loop/0049c3d0_owner_think_member_adapter_2026-10-20.asm)
- [Player/BModel scheduler census](player_bmodel_update_scheduler_census_2026-10-20.md)

## Owner `+0x50` is the root Think manager

Owner constructor `0042B860` constructs the manager at owner `+0x50` with
`0049C4E0`. That constructor installs primary vtable `004E4164` and stores
filter value `2` at manager `+0x44`. The primary table has slot `+0` at
`0049C0E0` and slot `+4` at `0049C3D0`.

`0049C0E0` walks its active-member list. For each member it calls manager slot
`+4`, then calls member slot `+4` to check whether the member should be
removed. The manager adapter `0049C3D0` dereferences the member's primary
vtable and calls root slot `+0x14`. Although a direct code-xref query found no
caller for `0049C3D0`, the instruction is reached through the function
pointer stored in vtable data at `004E4164+4`.

Registration is supplied by `00429490`. It reads each entity's eligibility
mask through root slot `+0x1C`, compares it with the four owner manager filters,
and calls the matching manager's registration interface slot `+4`. For
owner `+0x50`, that interface is the secondary table at manager `+4`; its
slot `+4` points to `0042B1A0`, which appends a non-null member to the active
list.

The common root initializer `0049D420` sets root `+0x14` to `0xFFEB`, which
includes the manager's filter value `0x2`. The PlayerRescue constructor chain
`00453D50 → 0043F610 → 0049DF30 → 0049D8C0 → 0049D420` and BModel constructor
`0043F340 → 0049D420` both use that base initialization. Their successful
generic spawn path registers them through `00429490`. This supports static
admission to the owner `+0x50` Think list; it does not establish live
residence, list order, or callback cadence.

Their local callbacks are:

- PlayerRescue root slot `+0x14` → `0044FE20` → embedded ScriptActor at
  `+0x140` → `00470740` → Player movement update `00452570`.
- BModel root slot `+0x14` → thunk `0043AFE0` → `00470740` → embedded
  ScriptActor slot zero `0043DC60`.

BoundsTrigger and PropModel use the same base mask and generic registration
path. Their local slot `+0x14` routes reach the pair drains
`00440DC0` and `00444050`, respectively. Exact manager-list residence remains
a static constructor/registration conclusion, not a runtime observation.

The selected-level `World` factory uses the same spawn queue: `0049C670`
creates `WorldEntity` through `0049E300` and calls `00429490` before batch
initialization. Its constructor changes the common mask to
`0xFFEB | 0x14 = 0xFFFF`; vtable slot `+0x14` points to delayed EVS callback
`0049E2C0`. Thus the selected WorldEntity is statically admitted to the `+0x50`
Think list. Its counter starts at 4, and each callback decrements it; the
fourth normal Think callback after registration loads the EVS. Because
`00429520` invokes manager `+0x50` before its `+0x3C0` spawn-queue service,
construction at the end of one pass cannot consume the counter in that same
pass. The early `0042AEA0` branch at `+0x64C` skips the manager pass.

## Think/contact phase boundary

On the normal game-update path, `0042AEA0` calls `00429520`. The latter
invokes owner managers in this order: `+0x08`, `+0x50`, `+0x98`, then contact
manager `+0xE0`. A separate early `+0x64C` branch in `0042AEA0` returns before
the manager pass. A pending level load at `+0x68C` is handled before the pass
and falls through to it.

The `+0x50` root callbacks drain contact-pair lists through `00470840`:

- BModel drains root `+0x160`; contact production at root slot `+0x48`
  (`0043B620`) appends to that list.
- BoundsTrigger drains root `+0x100` in `00440DC0`; its `+0x48` producer
  `00440CE0` appends the pair deltas.
- PropModel drains root `+0x158` in `00444050`; its `+0x48` producer
  `00443700` appends the pair deltas.

Because the normal `+0x50` Think manager runs before the `+0xE0` producers,
pairs added during one owner invocation cannot be consumed by those Think
callbacks until a later normal owner invocation. This establishes a
next-owner-pass phase delay for these pair lists. It does not establish a
rendered frame count, elapsed time, or retail runtime cadence. The WorldEntity
startup count is similarly four normal owner Think callbacks, not four frames.

## Shared-port change and limits

The shared scene loop now drains pending BModel, BoundsTrigger, and PropModel
ScriptActor pair events at the start of its contact-processing phase, then
samples and stores new deltas for the following host update iteration. The
host uses one paced simulation iteration as its owner-pass approximation.
Event-type ordering for BoundsTrigger and PropModel remains grouped as
`touch`, `touching`, then `untouch`. FireRescue contact/message delivery and
VictimRescue mapped-message delivery remain on their separate paths.

This is a source-backed phase-order correction, not a retail timing or parity
claim. The retail early-return branch, per-class list order, dynamic list
changes, and actual Windows runtime schedule remain unresolved.
