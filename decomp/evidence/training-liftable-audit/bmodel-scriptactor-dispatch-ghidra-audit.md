# BModel ScriptActor updater dispatch audit

Read-only Ghidra 12.0.4 inspection of `FDNYFirefighter.exe` on 2026-10-11 (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). No live game or preserved-project edits were used. Missing function boundaries were added only in a copied scratch project under `/private/tmp/fdny-bmodel-dispatch-scratch-20261011`.

## Verified local call chain

- BModel constructor `0043F340` installs primary vtable `004E0DF8` at root `+0` and ScriptActor subobject vtable `004E0DF0` at root `+0x24`. The table word at `004E0E0C` (primary slot `+0x14`) is `0043AFE0`; the word at `004E0DF0` (subobject slot 0) is `0043DC60`.
- `0043AFE0` is a two-instruction thunk: `ADD ECX,0x24; JMP 00470740`. The shared updater `00470740` suppresses the slot-0 callback while subobject state `+0x4C` equals 1; otherwise it calls the subobject vtable slot 0. State 0 additionally enters the queued-script-command path. Thus a caller of BModel root slot `+0x14` can reach `0043DC60` through `00470740`, subject to that state gate.
- This proves the local virtual path. It does **not** prove who invokes root slot `+0x14`, when it runs, or its rendered-frame cadence.

## Regular owner path and Training registration

- The extracted Training records identify entities 109 and 111 as class `RCS_BModel`, each with `G_Liftable=1`; they appear in that order in `assets/training/entities.json`. Factory helper `0043F5A0` allocates a BModel and calls constructor `0043F340`. The constructor sets root type `+0x18` to 4 and category bit 1 at `+0x20`, then calls `0049D330(this,1)`. Registration `00429490` checks the object's virtual category against four owner-manager masks; `0049BC90` appends admitted objects to the selected manager list. Staging service `0049C670` processes its pending linked list from the head and calls `00429490`, so category-list order follows admission order. This audit did not establish that Training BSP record order is the runtime enqueue order for entities 109 and 111.
- In normal owner update `0042AEA0`, `00429520` runs four manager passes. The first three use `0049C0E0`, which checks member removal and performs list cleanup. The fourth uses `0049BD20`; for BModel category 1 it calls primary vtable slot `+0x48` (`0043B620`, contact candidate production), not slot `+0x14` (`0043AFE0`). The staged-object service initializes through slots `+0x20/+0x24`. Following renderer call `00494670` dispatches slot `+0x10` over its render list. None of these inspected regular owner paths establishes an invocation of the BModel updater thunk.
- The `+0x14` virtual call at `0042AEA0` is on the global engine object loaded from `DAT_00503A78`; it is separate from the BModel manager-list dispatch.

## Bounded conclusion

The local route `root+0x14 → 0043AFE0 → 00470740 → ScriptActor vtable[0] → 0043DC60` is verified, including the state gate. The regular owner/contact, staged-admission, and render paths inspected here do not schedule that route, so BModel update cadence and event timing remain open. A scan of all 1,773 computed `CALL` instructions in the read-only Ghidra listing found no direct `CALL` edge to `0043DC60`; its only address xref is the vtable word `004E0DF0`. The call inventory was used to locate dispatch candidates, not as proof that no other runtime path can reach the vtable.

Raw extracts and the computed-call inventory are retained locally (ignored by Git) in [`dispatch-ghidra/`](dispatch-ghidra/). Read-only queries included `AddressXrefs.java 0043AFE0 0043AFE3 0043DC60 004E0DF0 004E0E0C`, `DataValue.java 004E0E0C 004E0E40`, plus decompilation/listing of `0042AEA0`, `00429520`, `00429490`, `0049C0E0`, `0049C670`, `00470740`, `0043F340`, and the two vtables. Missing boundaries for `0043DC60`, `0049BD20`, and `0049BC90` were created only in the scratch copy.
