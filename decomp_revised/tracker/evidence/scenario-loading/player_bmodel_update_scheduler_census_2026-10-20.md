# Player and BModel recurring update scheduler census

## Scope and provenance

Static analysis of `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The fresh caller query used Ghidra 12.0.4 headless against the saved
`.local/mission-lifecycle-ghidra/FDNYPort` project with `-noanalysis
-readOnly` and `FindFunctionCallers.java`. No retail runtime was captured.

## Local callbacks and direct references

The Player and BModel local update routes are known:

- Player root vtable slot `+0x14` calls wrapper `0044FE20`, which adjusts to
  Player's embedded ScriptActor at `+0x140` and calls `00470740`. Its
  slot-zero update is Player movement at `00452570`.
- BModel root slot `+0x14` uses thunk `0043AFE0` to reach `00470740`; its
  embedded ScriptActor slot-zero update is `0043DC60`.

The direct-reference census for `00470740` returned four call instructions
(`00443603`, `00446FE2`, `0044FE5F`, `0045999D`) and three unconditional
jumps (`0043AFE3`, `0043FA76`, `00442BB3`). The Player wrapper's call is
`0044FE5F`; the BModel thunk is `0043AFE3`. The other direct references are
local wrappers/thunks, not an identified owner manager iterating Player or
BModel roots. The query therefore confirms the local callback routes but does
not reveal their recurring caller.

## Contact registration is a separate route

The successful entity-spawn path registers roots through `00429490`. Its
contact-manager filter is `0x20`; the common entity registration mask is
`0xFFEB`. PlayerRescue's root type is 1, and BModel's is 4, so both are
eligible for their respective type lists in the contact manager. Registration
appends them to the manager's active-entity list; this establishes static
admission, not live overlap or runtime residence.

On the normal `0042AEA0` update path, `00429520` invokes owner managers at
`+0x08`, `+0x50`, `+0x98`, and `+0xE0` in that order. Ghidra's direct caller
query found `0042AEA0` as the only direct code reference to `00429520`. The
contact manager at owner `+0xE0` dispatches `0049BD20`; its manager vtable
reference is at `004E4124`. `0049BD20` walks the active entities and category
lists, invoking each matching root slot `+0x48`. The BModel's `+0x48` callback
is `0043B620`. This contact pass does not identify the root slot `+0x14`
recurring updater. Player's contact category mask is not established in the
evidence reviewed here.

`00429520` then calls the owner services at `+0x3C0`, `+0x3BC`, `+0x3B0`,
and `+0x3C4`. The first is the entity-spawn queue (`0049C670`), the second is
the mapped-message drain (`0049B610`), and `+0x3B0` calls the sound service
`004A0660`. The `+0x3C4` service is constructed by `0042B2C0`; its nested
vtable `004DF8E8` slot `+0x04` points to `0049C820`. That bounded code window
removes a queued node, then calls the queued item's slot `+0` when byte
`+0x1D` is zero or slot `+0x2C` otherwise. It does not directly dispatch an
entity root slot `+0x14`. The queued item classes and their producers remain
untraced, so this does not exclude an indirect effect. See the fresh
[service constructor](../runtime-loop/0042b2c0_owner_work_service_constructor_fresh_2026-10-20.c)
and [callback window](../runtime-loop/0049c820_service_callback_window_2026-10-20.asm).

The fresh direct-reference query found the `0049BD20` dispatcher in manager
vtable data at `004E4124`, and `0049BC90` registration in manager vtable data
at `004E411C`; it found no direct caller path from either function to the
Player or BModel root `+0x14` update wrappers. Existing same-hash fixed-offset
searches recorded 53 `[reg+0x14]` call/jump candidates but did not identify a
Player/BModel receiver. These negative static searches do not exclude a
computed dispatch path or an unexamined update loop.

## App callback passes are a separate route

The same-hash retained app-pass evidence also excludes the visible app callback
list as the Player/BModel root `+0x14` scheduler. `0049A310` traverses the list
at owner `+0x7C`, calling active children at vtable `+0x74`, pending-removal
children at `+0x6C`, and the owner at `+0x60`; `00499E60` traverses the same
list and calls child vtable `+0x70`. The owner `+0x60` route reaches the
separate game update. No available evidence shows PlayerRescue or BModel root
objects registered as children in this app list. These calls therefore do not
establish a recurring root `+0x14` route, while an unexamined or computed
dispatch elsewhere remains possible. See the retained [app prepass](../runtime-loop/0049a310_app_prepass.c),
[app postpass](../runtime-loop/00499e60_app_postpass.c), and
[idle-loop dispatch](../runtime-loop/00499410_app_idle_loop.c).

A follow-up attempt to reopen these functions with this turn's headless Ghidra
launcher failed before project load because LaunchSupport tried to access its
saved JDK path outside the writable workspace. This conclusion is a bounded
review of the existing hash-pinned outputs above, not a fresh Ghidra query or
runtime observation.

## Current boundary

The retail class admission and contact traversal are statically mapped, but
the manager/list that repeatedly invokes Player and BModel root slot `+0x14`,
their cadence, and their order relative to owner contact/Think passes remain
unresolved. Preserve the host's callback cadence/order as provisional; do not
infer a frame rate or retail parity from the known local callback routes.

Relevant evidence:

- [Player local update route](player_update_dispatch_2026-10-20.asm)
- [BModel local update and scheduler search](bmodel_update.md)
- [Entity contact-manager admission and dispatch](rescue_contact_manager_route.md)
- [Owner manager callback order](../runtime-loop/00429520_world_owner_pass.c)
- [Owner `+0x3C4` service construction and drain](../runtime-loop/0042b2c0_owner_work_service_constructor_fresh_2026-10-20.c)
- [App callback prepass](../runtime-loop/0049a310_app_prepass.c), [postpass](../runtime-loop/00499e60_app_postpass.c), and [idle-loop order](../runtime-loop/00499410_app_idle_loop.c)
- [WorldEntity slot `+0x14` scheduler boundary](worldentity_callback_dispatch_boundary.md)

No gameplay code, build, tests, runtime capture, or retail parity check changed
or ran for this census.
