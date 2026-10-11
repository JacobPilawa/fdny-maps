# Owner deferred services

Generated from the preserved read-only `FDNYFirefighter.exe` Ghidra project. Raw vtable words are in `service-vtable-32-words.tsv`; constructor and update code are in `0042b1f0_FUN_0042b1f0.c`, `0042b2c0_FUN_0042b2c0.c`, `0042b350_FUN_0042b350.c`, and `app-callback-registration/00429520_FUN_00429520.c`.

| Owner field | Secondary vptr at service `+0x40` | Callback used by `00429520` | Confirmed behavior |
|---|---|---|---|
| `+0x3C0` | `004DF8F8` | `0049C670` | Drains staged entities, registers each with the entity manager, then calls entity vtable `+0x20` and `+0x24` initialization hooks. |
| `+0x3BC` | `004DF8CC` | `0049B610` | Drains mapped messages, resolves route/generation, calls the receiver handler, and releases each mapper record. |
| `+0x3C4` | `004DF8E8` | `0049C820` | Drains staged object callbacks; dispatch depends on the object's byte at `+0x1D` (vtable slot 0 with argument 1, or slot `+0x2C`). |

The order in `00429520` is: four state-manager calls at `+0x08`, `+0x50`, `+0x98`, and `+0xE0`; the `+0x3C0` staged-entity service; the `+0x3BC` message drain; sound service `004A0660`; then the `+0x3C4` staged-object service. The `+0xE0` manager is `0049BD20`; after its removal pass, it traverses nine category lists and dispatches member vtable `+0x48` when category bits match.

FireRescue primary vtable `+0x48` is `004483A0`, which can enqueue message `0x7531`. When that callback is reached from the `+0xE0` manager, the message is enqueued before the `+0x3BC` drain in the same `00429520` call. This establishes that local fire-contact-to-message delivery path. It does not locate Fire Think `00449FE0`, nor does it place the player movement / extinguisher callback relative to this owner update. The controlled extinguisher oracle's explicit queue drain remains fixture ordering, not a Training-frame claim.

The `+0x3C4` slot-0 callback queue was checked as a possible Fire Think scheduler. Its generic dispatcher `0049C820` drains objects through primary vtable slot 0 or `+0x2C`, but the current xref and owner-field scans do not show FireRescue visual subobject `+0x24` being registered there. See `slot-zero-callers/staged-callback-queue-audit.md`; queue membership and Fire Think's live caller remain unresolved.

The constructor follow-up also ruled out two neighboring hooks as Think dispatch: common visual-base `+0x40` stores release callback `00432A40`, while staged-entity registration `0049C670` invokes FireRescue primary slots `+0x20/+0x24` as `00448D60` initialization and `0043F660` no-op. Fire Think remains secondary visual slot 0 at `004E1774`, with no recurring caller established. Raw xrefs, helper exports and assembly are in `slot-zero-callers/fireresque-constructor-xrefs.tsv`, `fireresque-helper-exports/`, and `fireresque-helper-assembly.txt`.
