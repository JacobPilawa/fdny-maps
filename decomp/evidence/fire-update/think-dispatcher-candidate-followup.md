# FireRescue Think dispatcher candidate follow-up

Read-only Ghidra 12.0.4 follow-up, 2026-10-11. The preserved executable analyzed in the project is `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. No live process was attached, and no gameplay code or Ghidra project was modified.

## Verified paths

- FireRescue's secondary table at `004E1774` contains `00449FE0` in slot 0. Ghidra references into that table are the constructor/destructor table installs at `0044A2B9` and `0044A370`; references out of it lead to `00449FE0`. The primary table is separately installed at `004E1780`. This query adds no code caller for Think.
- FireRescue setup calls common visual-subobject initializer `0046F930` with `this+0x24`. The helper installs visual-base table `004E2D08`, initializes its fields, and stores `00432A40` at subobject `+0x40`. `00432A40` releases an owned object and frees it; this field is cleanup, not an update callback. The helper makes no registration call. Xrefs show its other callers initialize visual subobjects for several other classes; they do not demonstrate a dispatcher.
- The specific app callback-list add route is `00499DE0 → 00498F90`: `00498F90` writes the callback pointer to app field `+0x40`, and `00499DE0` then calls the registered object's primary slot `+0x84`. The only code xref to `00498F90` in this program is from `00499DE0`. The two vtable words referencing `00499DE0` are `004DF9FC` and `004E3EE4`, not either FireRescue table. The companion app traversal `0049A310` walks its list, calls a registered object's primary slot `+0x74` when its active byte is set, removes entries through `+0x6C` when requested, then calls the app object's `+0x60`. Its incoming refs are the three data words `004DF9CC`, `004E3EB4`, and `004E3F64`. This confirms the previously documented app callback route's behavior, but supplies no FireRescue registration edge.
- The previously traced owner path remains distinct: `00429520` dispatches its four manager slots, staged entities, queued messages, sound, and staged-object queue; `0049C670` invokes primary slots `+0x20` and `+0x24` for newly admitted entities. Neither has an established call to the FireRescue secondary slot 0. See `training-fire-dispatch-order-audit.md` for the full ordering evidence.

## Bounded conclusion

This pass rules out the *inspected* common visual initializer and app callback-list add route as established registration paths for `00449FE0`. The recurring caller is still unknown. Static xrefs and these registration helpers do not rule out another engine-side interface, a generic dynamically populated list, or a runtime-computed indirect call. Do not infer Think cadence or ordering from this negative result.

## Ghidra provenance

Queries used the preserved project `/private/tmp/fdny-ghidra-ro-20261011/fdny`, program `/FDNYFirefighter.exe`, with `-noanalysis -readOnly`. Because the cached Java scripts were compiled for class-file version 69, commands used the installed Temurin 25 JDK. Scripts were `AddressXrefs.java`, `DataValue.java`, and `DecompileFunctions.java` from `decomp/tools/ghidra`.

Relevant query sets:

- `AddressXrefs.java`: `00449FE0 004E1774 004E1780 0046F930 00499DE0 0049A310 004E16AC 00448110`.
- `DecompileFunctions.java`: `0044A290 0044A330 0046F930 00499DE0 0049A310 0049BD20 00429490 0049C670 00429520 0042AEA0 00448110`.
- Registration follow-up: xrefs for `00498F90 00499DE0 0049A310 0049A2D0 004DF9FC 004E3EE4 004E1774 004E1780 004E2D08 00449FE0`; data at `004DF9F8 004DF9FC 004E3EE0 004E3EE4 004E1774 004E1780 004E2D08`; decompilation of `00498F90 00499DE0 0049A310 0049A2D0 0046F930 00432A40 0046F8C0`.

Full command logs are in `/private/tmp/firethink-dispatch-ghidra.log` and `/private/tmp/firethink-dispatch-registration-ghidra.log`. Prior broader slot-zero, register-transfer, and secondary-offset scans are documented in `../item-pose/live-extinguisher/message-components/slot-zero-callers/register-transfer-audit.md`; this follow-up does not repeat or expand those whole-program sweeps.
