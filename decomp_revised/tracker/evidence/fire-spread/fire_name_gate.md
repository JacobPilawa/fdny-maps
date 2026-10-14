# FireRescue spread name gate

## Result

The word tested by `00448320` and `00447A90` is the length field of a
reference-counted string stored at FireRescue root `+0x48`. FireRescue setup
copies UserData `A_Name` into that string when `A_Name` is present. The gate is
therefore true for a nonempty FireRescue name and false for an empty name.

The embedded subobject constructor `0046F930` initializes root `+0x48` to the
shared empty string `&DAT_00501908`. The shared string length at `+4`
(`DAT_0050190C`) starts at zero. `00448D60` checks the UserData pointer at
root `+0x78`, offset `+4`; when non-null, it constructs a string from that
value and passes it to `0046E320`, which replaces the subobject string at
root `+0x48`. String assignment routine `00401D80` stores the string length at
implementation `+4` and its bytes starting at `+12`.

This resolves the earlier ambiguity: root `+0x48` does not alias the UserData
block, but it holds a string made from UserData `A_Name`.

## Evidence and quality

The pinned input is `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. This
matches the hash on the current FireRescue spread exports. No Ghidra instance
was running during this follow-up, so the chain below was checked against the
local retail binary with `objdump` and cross-referenced with saved constructor
and initializer decompilations.

- `0044A330` constructs the embedded subobject at root `+0x24`:
  [FireRescue constructor](../runtime-loop/0044a330_firerescue_constructor.c).
- `0046F930` assigns `&DAT_00501908` to subobject `+0x24`, which is root
  `+0x48`; it also initializes the shared string length to zero:
  [embedded subobject constructor](../../../reference/previous-work/decomp/evidence/fire-update/training-route-attachment/constructor-state/0046f930_FUN_0046f930.c).
- Retail bytes at `00448D60` test UserData `+4`, construct a string from it,
  and call `0046E320` with the local string and origin buffer:
  [retail initializer disassembly](00448D60_name_setup.asm). The saved
  decompilation is
  [00448D60](../../../reference/previous-work/decomp/evidence/rng-startup/00448d60_FUN_00448d60.c).
- `00401D80` writes the nonempty source-string length at implementation `+4`;
  `00402790` initializes and assigns the local string. `0046E320` replaces the
  subobject's string pointer:
  [current setter export](../rescue-moveto/0046E320_fresh.c).
- Both spread gates read the length word from root `+0x48`:
  [spread routine](00447A90_fresh.c) and
  [proximity callback](00448320_fresh.c).
- `00447A90` obtains a generated `RCS_FireRescue` definition record and zeros
  its `+4` field, then passes that record through `0049D130` in the pending
  spawn request. The matching `RCS_FireRescue` typedef maps `+4` to `A_Name`;
  the class initializer skips string assignment for a null UserData `+4`,
  leaving the child on the empty-string gate. This child conclusion is a
  high-confidence inference across the factory request and UserData layout.

## Port behavior

The shared preview enables spread for authored FireRescue entities whose
`A_Name` is nonempty. Unnamed authored fires remain visible and suppressible,
but do not attempt random spread. Randomly generated FireRescue children have
no authored name and do not spread again. The exported inventory contains 430
FireRescue records, of which 60 have a null or empty `A_Name`.

The authored-fire eligibility mapping is verified by retail code and the
typedef. The dynamic-child name conclusion is a high-confidence inference from
the factory record, spawn request, and matching typedef. Callback cadence, the
host PlayerRescue-origin adapter, full engine RNG order, and dynamic child
visual/lifetime behavior remain provisional. The viewer's shared proximity
count only includes named active fires, matching the same gate used by the
native proximity bookkeeping.
