# Fixed-slot candidate `0046269D` rejected as BoundsTrigger scheduler evidence

## Pinned input and observation

The instruction is in the pinned `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`). Raw x86 bytes at VA `0046269D` are `FF 50 14`, a virtual call through `[EAX+0x14]`. The saved Ghidra inventory has no function body beginning at `00462660`, so the call was previously a weak fixed-slot candidate for a recurring ScriptActor updater.

A read-only Ghidra 12.0.4 scratch-copy audit defined the instruction entry at `00462660` and inspected its incoming references and control flow. A table-like pointer run at `004E2960–004E2978` contains a reference to this code at `004E2974`. The routine takes an object pointer in `ECX` and a stack flag. When the flag is nonzero it calls that object's own vtable slot `+0x14`, then returns. When the flag is zero it walks an array at object offset `+0x5C0` (count at `+0x5D8`) and calls `0047DC90` on each child pointer adjusted by four; the `+0x14` call is outside that loop.

## Disposition

This is conditional self-vtable dispatch, not evidence of recurring list-based update scheduling. The inspected references do not identify a constructor, receiver type, or reachable runtime caller that connects this object to ScriptActor, BoundsTrigger, or shared updater `00470740`. Reject `0046269D` as support for the BoundsTrigger scheduling hypothesis. The static audit does not rule out a separate computed-slot dispatcher or another callback path; the recurring BoundsTrigger root-slot `+0x14` caller and its cadence remain unresolved.

The code-body and reference findings above are from the preserved read-only scratch audit; no live process or game runtime was used. The active contact-to-event trace is [`training_touch_dispatch_boundary.md`](training_touch_dispatch_boundary.md).
