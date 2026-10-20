# Player movement byte gates (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The movement block has no function object in the saved Ghidra project. The
following instructions were read from the hash-pinned executable with
headless Ghidra 12.0.4 in read-only mode.

## Player `+0x380`: debug/action latch

The Player initializer `0044D550` clears root byte `+0x380` (`0044DDF2`); the
movement block has `ESI = Player+0x140`, so `[ESI+0x240]` is the same byte. The
`DBG_TOGGLE_SPECT` action query uses string `004E0AF0` at `004525C7`. On its
accepted path, the block stores 1 at `0045262E` and sets state `+0x18C` to 3.
The later `JUMP` query uses string `004E0A38` at `00453685`; when that query
succeeds and the byte is set, `004536FA` clears it and again sets state
`+0x18C` to 3.

This evidence identifies `+0x380` as an initialized debug/action latch in the
examined path. It also gates the 800 movement-vector cap, frame transform,
gravity suppression, 0.2 smoothing base, and collision-loop skip in the
movement block. Do not use it as another name for normal state-5 climbing.
The host currently models state 5 through `climbing` and does not expose this
debug action path.

```text
0044DDF2  MOV byte ptr [ESI + 0x380],BL  ; initializer has BL=0
004525C7  PUSH 0x4e0af0                 ; "DBG_TOGGLE_SPECT"
0045262E  MOV byte ptr [ESI + 0x240],0x1 ; root +0x380
00452635  MOV dword ptr [ESI + 0x4c],0x3 ; root state +0x18C
00453685  PUSH 0x4e0a38                 ; "JUMP"
004536FA  MOV byte ptr [ESI + 0x240],0x0 ; root +0x380
00453701  MOV dword ptr [ESI + 0x4c],0x3 ; root state +0x18C
```

## Player `+0x198`: Total Steps gate, writer resolved; initial value unresolved

In the movement block, `[ESI+0x58]` aliases Player root byte `+0x198`. At
`00453306` it is read after the state-not-5 and `+0x380`-clear checks; a
nonzero value permits the `0044EA90` Total Steps/footstep helper call at
`0045331A` when the movement vector also passes its magnitude threshold.
Player construction calls `00491460(root+0x198)`. The inspected initializer
and constructor chain do not explicitly assign byte zero of that subobject.
The byte is later written by `00491140`, called from `0044F340`: it is cleared,
then set when the middle of three successive head-motion Y samples is strictly
lower than both neighbors. Thus the writer and sample predicate are known;
the gameplay interpretation as a step/trough signal is based on its use by the
`Total Steps` gate. The first/default value remains unresolved because the
constructor omits the byte write and no allocator guarantee has been shown.

The movement pass reads the existing byte at `00453306..00453312`, calls
`0044EA90` at `0045331A`, and only later calls `0044F340` at `00453860` to
refresh it. This makes the gate a prior-callback sample state, subject to the
still-unknown recurring callback schedule and object reuse behavior. The host
uses a deterministic false initial byte and updates the sample predicate after
its Player step.

```text
00453306  MOV AL,byte ptr [ESI + 0x58] ; root +0x198
00453309  TEST AL,AL
00453312  JZ 0x0045331f
0045331A  CALL 0x0044ea90             ; Total Steps / footstep producer
```

The byte is only proven to gate the `Total Steps` callsite and to be set from
the sample pattern above. The sample pattern is exact; the semantic label is
provisional. The host does not currently render the HeadMotion output into the
camera, following the earlier camera-bob removal direction.

