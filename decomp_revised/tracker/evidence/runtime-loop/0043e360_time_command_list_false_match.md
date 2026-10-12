# `0043E360` is not established as the `FUN_004943E0` pre-frame callback producer

**Input:** pinned `FDNYFirefighter.exe`, SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`; Ghidra 12.0.4; read-only MCP queries.

`FUN_004943E0` iterates entries in the selected context's `+0x94/+0x98` array and calls each entry's vtable slot `+0x08`. `FUN_00494670` selects the context at outer `+8` or `+0xB4`, so those outer array fields appear at `+0x9C/+0xA0` or `+0x148/+0x14C`.

`FUN_0043E360` also writes an array at its `this+0x148` and count at `this+0x14C`, making it a tempting numeric-offset match. It allocates a 0x10-byte record, stores an active byte at `+0`, the supplied time value at `+4`, a reference-counted command string at `+8`, and `this+0x9C` at `+0xC`. The call site is in `FUN_0043E680`'s `addtimecmd` script-command branch, which registers a timed command on the owning object. Those records cannot be the same elements that `FUN_004943E0` dereferences as vtable-bearing callback objects. No evidence connects this `this` pointer to the selected renderer context, so the matching numeric offset alone does not identify a producer; a later timed-script handler could still have an indirect relationship to rendering.

The constructor `FUN_0043F340` initializes the `+0x148/+0x14C` list in this object's layout. Treat it as a separate timed-script record list, not as direct evidence of the pre-frame callback producer; that producer remains unresolved.

Fresh pseudocode and xrefs for these routines are saved alongside this note. The conclusion is a negative identification of this candidate only; it does not identify the actual pre-frame producer.
