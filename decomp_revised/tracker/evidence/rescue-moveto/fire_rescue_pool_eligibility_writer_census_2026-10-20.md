# FireRescue pool eligibility byte writer census

## Scope and binary

Read-only analysis of `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, with
Ghidra 12.0.4 headless in a disposable project under `/private/tmp`. The broad
`FindOffsetWrites.java` register-plus-displacement scan reported 1,295
candidate stores containing displacement `0x1C`; stack locals and unrelated
class fields are common false positives. The inspected FireRescue owner paths
are recorded below. This was not a live retail trace.

## Root base byte and class helpers

`0049D420` is called by FireRescue constructor `0044A330`. Its direct store at
`0049D444` writes byte zero to `this+0x1C`, alongside the generic base fields
at `+0x10`, `+0x1D`, `+0x18`, and `+0x20`. Reuse method `004475F0` calls
`0049D310`; that direct store at `0049D31F` also clears byte `this+0x1C`.
Factory `0044A470` scans its retained FireRescue roots and branches to reuse
when the byte at `root+0x1C` is nonzero.

`0049D530` is the one obvious generic helper setting its receiver's byte
`+0x1C` to 1. Its only direct code caller is `00448550`. FireRescue destructor
`0044A290` calls `00448550` with `ECX=root+0x24`; the helper's store therefore
targets `root+0x40`, inside that embedded subobject, rather than the factory's
`root+0x1C` eligibility byte. The destructor's separate `0049D510` call
releases root identity and does not set root `+0x1C`.

## FireRescue fields at the same numeric offset

The UserData layout at
[`training-userdata-layout-audit.md`](../../../reference/previous-work/decomp/evidence/fire-update/training-userdata-layout-audit.md)
defines FireRescue UserData `+0x1C` as `L_FloorMe`. Two other direct stores
must not be counted as writes to the root eligibility byte:

- Route builder `00449340` writes generated-child UserData `+0x1C` from the
  source FireRescue UserData's `L_FloorMe` field.
- Spread builder `00447A90` writes 1 to generated-child UserData `+0x1C`, also
  the floor flag.

FireRescue visual initialization `00448D60` allocates a separate `0x24`-byte
object and calls `004A13A0` on it before storing the pointer at root `+0x114`.
The helper's `+0x1C` clear is inside that allocated subobject, not the root.

## Current boundary

No direct register-plus-`0x1C` write that sets the FireRescue root's eligibility
byte nonzero was identified in this bounded scan. This does not prove the field
is never changed: aliased or computed addresses and indirect calls can evade the
candidate filter. The exact byte meaning, any other writers, whether the reuse
branch is reached during ordinary gameplay, and child retirement remain open.
Do not add FireRescue pooling to the host based on this census alone.
