# FireRescue route UserData allocation and zero-spray initialization

## Source and method

The pinned retail binary is `inputs/original/binaries/FDNYFirefighter.exe`,
SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
This follow-up used Ghidra 12.0.4 headless decompilation in a disposable copy
of the local Ghidra project. The saved project and scratch output remain under
`/private/tmp/fdny-route-ghidra-audit`; no live retail process or UI was used.

## The route builder gets a fully cleared data block

FireRescue descriptor initialization `00448070` installs vtable
`004E16A0`. Its slot `+0` is `00448210`; slot `+4` is `004479B0`. In the
route builder `00449340`, after descriptor lookup, an indirect call through
slot `+0` returns the `RCS_FireRescue` UserData pointer before the builder
writes its generated-child fields.

`00448210` stores pointers to `0x8C`-byte records in the descriptor array at
`+0x0C`, with its record count at `+0x10`. The first byte of each record is an
availability flag and the `0x88` bytes after it are FireRescue UserData. A
record is selected when that flag is nonzero. Both that cached-record branch
and the `operator_new(0x8C)` branch converge on the same code:

```c
*record = 0;
for (i = 0x22; i != 0; --i) {
    *userdata_dword++ = 0;
}
return record + 4;
```

Thus every UserData allocation through this class descriptor clears the
availability byte and zeroes all `0x88` data bytes, whether the backing record
is new or recycled. `004479B0` finds the cached record whose `+4` pointer
matches its argument and sets the leading availability byte to one. This is
the allocator's reuse protocol; it does not establish when the route-child
lifecycle returns a record to that pool.

The factory's root reuse is a separate pool. Reused root method `004475F0`
calls `0049D310` to refresh root request state, then calls
`0049D160(request, root+0x78, 0x88)`. `0049D160` attaches the request's
UserData pointer and checks the registered class size; it does not itself
clear the data. The constructor takes the same pointer-validation path. The
zeroing guarantee for generated route requests comes from descriptor slot
`+0`, not from `0049D160`.

## Route-child fields that remain zero

The generated request in `00449340` is allocated through `00448210` and then
filled explicitly. For the verified no-`C_Model` route path, the builder
leaves `ZZ_spawn_message`, `K_SprayTime`, and `N_IsAlive` untouched, so these
fields remain zero in the request. `F_FireType`, `C_Model`, and `A_Name` are
explicitly written as zero. The route builder writes its active key count,
size records, and timestamps; generated child key-time bases do not inherit
stale contents from a prior cached request.

This resolves the host's zero proxy for the generated route child's untouched
fields. It does not infer placed BSP defaults for unrelated map entities or
other classes. The non-null model route path, root-pool eligibility, owner
retirement, and live callback cadence remain open.

## Retail zero `K_SprayTime` behavior

After setting up the FireRescue root, `00448D60` reads UserData
`K_SprayTime` at `+0x18`. When it is strictly between `-1.0e-28` and
`+1.0e-28`, it replaces the value with
`1 + 3 * FUN_00447770(H_Radius, 24, 288)`. Current-hash decompilation shows
`00447770` returns zero below 24, the fraction `(radius - 24) / 264` from 24
through 288, and one above 288 for ordinary finite radii. The fallback
`004DC50C` is binary32 zero, `004DFC24` is binary32 1.0, and `004E16C4` is
binary32 3.0. The threshold operands are binary64 `-1.0e-28` and `+1.0e-28`.

The host already passes zero for this generated field and applies the same
radius-based range. That behavior is now source-backed for route children.
The host uses double intermediates rather than the original x87 extended
intermediates, so exact edge rounding and callback timing are not parity
claims.

## Evidence limits

The trace establishes the descriptor allocator's clear operation and the
route request's call through that slot. It does not establish a runtime
sequence showing when a route child becomes inactive or which request blocks
are reused during a mission. No retail playthrough, visual capture, or
gameplay parity comparison was performed.
