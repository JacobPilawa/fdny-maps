# Selected-level World factory resolution

## Result

The Training selected-level request is statically linked through the registered `World` factory to the `WorldEntity` BSP loader. This closes the factory/vtable gap recorded in the earlier selected-level trace for the successful static load path.

The result is pinned to `FDNYFirefighter.exe` SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Dispatch chain

1. Startup function `0042B4E0` constructs a descriptor with `0049E360` and registers it through `0049F160` in the factory registry at `DAT_00503A78 + 0x14`. Preserved Ghidra output for `0049E360` assigns descriptor vtable `004E44B0` and names the descriptor `World`.
2. The selected-level loader `0042AB00` calls `00429C00` with the name `World`, stores its return value at spawn request `+0x0C`, writes command `0x2711` at `+0x08`, and stores the selected BSP path in the request's first field. For Training that path is `data\\maps\\training.bsp`.
3. The pinned executable's `.rdata` table at `004E44B0` has these first four words:

   | Table offset | Value |
   |---|---|
   | `+0x00` | `0043F2F0` |
   | `+0x04` | `0043F670` |
   | `+0x08` | `0049E300` |
   | `+0x0C` | `0043F660` |

   The bytes at file offset `0xE44B0` are `F0 F2 43 00 70 F6 43 00 00 E3 49 00 60 F6 43 00`. This independently confirms that descriptor virtual slot `+0x08` points to `0049E300` in the hash-pinned copy.
4. Queue drain `0049C670` takes the factory pointer from request `+0x0C` and calls its virtual slot `+0x08`, passing the request. It registers the returned object and runs the ordered initialization passes.
5. Factory `0049E300` allocates a `0x30`-byte object and constructs it with `0049E240`. That constructor installs primary vtable `004E4458` and copies the request path into object field `+0x24`.
6. WorldEntity's primary vtable entry at `004E4478` (slot `+0x20` from `004E4458`) is `0049E4D0`. The queue's first initialization pass invokes slot `+0x20`; `0049E4D0` reads `this+0x24` and passes the path to BSP loader `00495210`.

Thus the selected `World` request creates a `WorldEntity`, and the staged object's initialization loads the selected BSP. `0049E4D0` then enumerates BSP entities and submits their factory requests through the same spawn queue.

## Evidence provenance and limits

Fresh, hash-pinned outputs in this workspace cover `0042B4E0`, `0042AB00`, `00429C00`, `0049D130`, `0049E240`, and `0049E4D0`. The preserved same-binary Ghidra exports for descriptor construction, queue drain, and the World factory are under `reference/previous-work/decomp/evidence/entity-dispatch/`, `entity-bootstrap/`, `entity-queues/`, and `startup-world/`. The current EXE bytes at the factory vtable were read directly and match the pinned hash. The WorldEntity vtable bytes are saved in `memory_004e4460_world_entity_vtable.json`.

This is a static successful-path reconstruction, not a runtime trace. It does not establish runtime timing, failure-path behavior, or full Training parity.
