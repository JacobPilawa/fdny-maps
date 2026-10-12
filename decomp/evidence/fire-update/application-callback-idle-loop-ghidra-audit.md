# Application callback idle loop and actor-list boundary: Ghidra audit

Snapshot: 2026-10-11. Read-only static trace from WinMain through the registered game-system object to its recurring application callback list. The list also serves pointer input; this audit does not map Training Player, victim, or FireRescue wrapper callbacks onto its entries.

## Owner and recurring call path

1. WinMain `0042C800` creates the application/window object with `00498E90`. That constructor stores it in `DAT_00503A70` and initializes field `+0x40` to null.
2. WinMain creates the game-system object with `0042BCB0`. It calls `00499DC0`, which initializes its callback-list base via `0049A460`, then installs vtable `004DF958`.
3. Before entering the message loop, WinMain calls virtual slot `+0xA4` on that object. In `004DF958`, `+0xA4` is `00499DE0`. This sets its active byte at `+0x4C`, stores the object in application field `DAT_00503A70+0x40` through `00498F90`, then invokes the object's virtual `+0x84` method.
4. WinMain enters `00499410(application)`. On each idle iteration with no Windows message pending, it calls the object stored at application `+0x40` through vtable slots `+0x74` and `+0xA0`. Vtable `004DF958` maps these slots to `0049A310` and `00499E60` respectively.

`0049A310` walks a linked list whose head is object field `+0x7C` (`param_1[0x1F]`). For each non-null object pointer in the list node, it checks the object's active byte at `+0x4C`; when active, it calls that object's virtual slot `+0x74`. If the object's removal byte at `+0x4D` is set, it calls virtual slot `+0x6C`, unlinks/frees the node, and decrements the list count at `+0x84`. After traversal it calls the manager's virtual slot `+0x60`.

The second recurring method, `00499E60`, resets its cursor to the same head at `+0x7C` and makes another pass over the remaining linked-list entries, calling each contained object's virtual slot `+0x70`.

## What this establishes and what remains open

- Established: an idle-frame path exists, registered through application field `+0x40`; it walks a manager-owned application callback list, dispatching an active-object virtual callback at slot `+0x74`, deferred removal at `+0x6C`, then a second pass through slot `+0x70`. The same list supports pointer-input callbacks.
- Not established: whether the list entries are Player, victim, BModel, FireRescue, ScriptActor subobjects, or adapters; which registration method appends them; how often the two idle callbacks run under real message traffic; or the relative order of this dispatch against owner category passes, message drains, input, render, and wall-clock timers.
- Consequently this is not evidence that Player root `+0x14`, FireRescue root `+0x14`, or victim root `+0x14` is invoked by this manager. Those local wrappers still need their list-entry receiver/vptr and callback target traced. Do not wire native actor schedules from this finding alone.

## Callback-list type check and actor-registration boundary

The list owner is the object created by `0042BCB0`: it delegates base setup to `00499DC0 → 0049A460`, then installs `004DF958`. In that vtable, `+0x70` (`004DF9C8`) is `0043F660` (empty), `+0x74` (`004DF9CC`) is the recurring list pass `0049A310`, and `+0x84` (`004DF9DC`) is startup callback `0042BE70`. `+0xA0` and `+0xA4` are `00499E60` and `00499DE0` as above.

`0049A040` and `0049A1C0` also traverse the same list rooted at `+0x7C` for pointer input: they test entries with virtual `+0x78`, then dispatch mouse callbacks through `+0x20` or `+0x18`. This establishes that this owner/list also serves an input-dispatch role; it does not establish that the list is UI-only. It is a useful discriminator for recovering the entry interface.

The known Player (`004E1C48`), victim (`004E25B8` and secondary `004DFC30`), and FireRescue (`004E1780` and secondary `004E1774`) vtables do not expose code pointers at `+0x74`: the corresponding words are float constants or the `MOUSE_LB` string. Their constructors (`00453D50`, `0045DDD0`, `0044A330`) do not call the game-system registration function `00499DE0` / application setter `00498F90`. This rules out those known root and secondary subobjects as direct list entries under this list's `+0x74` dispatch. It does not rule out an adapter, another subobject, or later registration in code outside those constructor bodies. BModel remains unresolved: its primary `+0x74` word points at `00407BD0` and its secondary's at `004B9323`; neither is currently a defined function boundary, and neither by itself proves registration.

Actor handle assignment is a separate mechanism: `0049D450` reads singleton `DAT_00503A78+0x10` and calls `0049B810`; `0049B810` allocates/updates a handle record. Matching destruction `0049D510 → 0049B780` removes it from that manager. These paths do not call `00499DE0` or append to the callback list. The reviewed evidence therefore does not connect actor handles to the app callback list.

Limits: the vtable word checks and four constructor bodies above are a bounded type check, not a whole-program proof that no actor adapter is registered elsewhere. No callback-list append routine, actor-manager-to-app-list bridge, or recurring FireRescue/Player/Victim callback was identified in this pass. No native schedule should be inferred from it.

## Reproducible queries and scope

Against the preserved Ghidra project, Ghidra 12.0.4, executable SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, use `-noanalysis -readOnly`:

```sh
/Applications/ghidra/support/analyzeHeadless decomp/evidence/ghidra fdny \
  -process FDNYFirefighter.exe -noanalysis -readOnly \
  -scriptPath decomp/tools/ghidra \
  -postScript DecompileFunctions.java 0042C800 00498E90 0042BCB0 00499DC0 0049A460 00499DE0 00498F90 00499410 0049A310 00499E60
```

The follow-up read-only queries were `DecompileFunctions.java 0042BCB0 00499DC0 0049A460 0049A310 00499E60 0049A040 0049A1C0 0049B810 0049D450 0049D510 0049B780 0044A330 0045DDD0 0043F340 00453D50`, `DataValue.java 004DF9C8 004DF9CC 004DF9DC 004DF9F8 004DF9FC 004E1CBC 004E262C 004E17F4 004DFCA4 004E0E6C 004E0E64`, and `AddressXrefs.java 00499DE0 00498F90 00499E60 0049A310 0049D450 0049B810 0049B780`. The earlier vtable/reference pass also used `AddressXrefs.java 00499410 0049A310 00499E60 00499DE0 00498F90`. On this host, headless needs Temurin 21 on `PATH` and a pre-created writable `JAVA_TOOL_OPTIONS=-Duser.home=/private/tmp/fdny-rootupdate-home`. All Ghidra queries used `-noanalysis -readOnly`; no analysis or database edits were performed.
