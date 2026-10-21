# FireRescue shared timed-vector track and route position boundary

## Pinned source

`inputs/original/binaries/FDNYFirefighter.exe` SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

This follow-up used Ghidra 12.0.4 headless decompilation in a disposable
project copy plus the retained same-hash instruction evidence. It did not use
the Ghidra UI or a live retail session.

## The shared vector is size/bounds data

FireRescue root `+0x12C` is the visual component. Constructor helper
`0044B790`, called from root construction at `0044A330`, initializes component
vector `+0x44..+0x4C`, scales `+0x90..+0x98`, and dirty byte `+0x50` to zero.
Its disassembly is retained in
[`fire_sprite_alpha_init_chain_2026-10-20.asm`](../fire-spread/fire_sprite_alpha_init_chain_2026-10-20.asm).
Visual setup `0044B270` resets the timed key list through `0044AD70`, clears
head/next pointers at `+0x88/+0x8C`, and sets the three component scales at
`+0x90..+0x98` to 1.

`00447850` copies component `+0x44/+0x48/+0x4C` to an output vector.
`00447920` (`setsize`) resets the same key list, seeds it from this current
vector, then adds the requested dimensions through `0044B620`. The state-4
route helper `00449340` addresses the same component at `root+0x12C`, reads the
same vector, resets the same list, and adds keys through `0044B620`.
`0044AE80` interpolates the active keys against `DAT_00502AFC`, multiplies
each axis by `+0x90/+0x94/+0x98`, and writes the result to `+0x44..+0x4C` when
the squared delta passes its stored threshold. It sets dirty byte `+0x50` on
that write.

The consumers establish the vector's role. `0044B060` forms bounds from half
of `+0x44..+0x4C`, combines them with component base coordinates at
`+0x28..+0x30`, transforms the corners, and updates child/bounds effects.
`0044A840` also uses the vector while building transformed fire visual bounds.
Thus `setsize` and `moveto` share one size/bounds key track; the vector is not
world translation. Exact per-axis scale meaning and the detailed
`00458DA0` linked-object effect remain unresolved.

## Parent route-generated size keys

The route emission block at `004498FC..00449999` resets this same track after
queuing the child request. `0044AD70` clears its active key list. The first
`0044B620` insertion is `(0,0,0)` at `DAT_00502AFC - 10`; the second points to
the source parent's `root+0x108..+0x110` vector at
`DAT_00502AFC + truncTowardZero(delay)`. The source vector is the no-model
initializer extent `(H_Radius,H_Height,H_Radius)`. Consequently, a route
emission also replaces any keys left by an earlier `setsize` command.

Same-hash x86 around the delay computation:

```text
0044992F: FLD   dword ptr [EDI+0xFC]
00449935: FCOM  dword ptr [0x004E0ED4]  ; 1000.0
0044993B: FNSTSW AX
0044993D: TEST  AH,0x41
00449940: JNE   0044994C
00449942: FSTP  ST(0)
00449944: FLD   dword ptr [0x004E0ED4]  ; upper clamp = 1000.0
0044994A: JMP   00449961
0044994C: FCOM  dword ptr [0x004E169C]  ; 10.0
00449952: FNSTSW AX
00449954: TEST  AH,0x05
00449957: JP    00449961
00449959: FSTP  ST(0)
0044995B: FLD   dword ptr [0x004E169C]  ; lower clamp = 10.0
00449961: FSUB  dword ptr [0x004E169C]  ; rate - 10
00449967: FMUL  dword ptr [0x004E175C]  ; 0.001000000047...
0044996D: FSUBR dword ptr [0x004DFC24]  ; 1 - normalized rate
00449973: FMUL  dword ptr [0x004E1754]  ; 250
00449979: FADD  dword ptr [0x004E1758]  ; 1750
0044997F: CALL  004B9400               ; integer conversion toward zero
00449984: ADD   EAX,dword ptr [0x00502AFC]
0044998A: PUSH  EAX
0044998B: LEA   EAX,[EDI+0x108]
00449991: PUSH  EAX
00449992: MOV   ECX,EBP
00449994: CALL  0044B620               ; insert target vector at clock + EAX
```

The two compare/test sequences clamp ordinary finite route-rate values to
`[10,1000]`. The resulting delay is
`((1 - ((rate - 10) * 0.001)) * 250) + 1750` before integer conversion: 2000
at rate 10 and 1752 at rate 1000. The `0.001` operand is the stored binary32
constant, and the expression executes on x87; host arithmetic is an
approximation at integer-boundary cases. The route rate's author-facing units,
the reason for this delay, and its live effect are not established. This is
static binary evidence, not a live comparison.

For no-`C_Model` FireRescue entities, initializer `00448D60` writes
`root+0x108..+0x110 = (H_Radius, H_Height, H_Radius)`. The route updater uses
the first component times one half as the threshold for emitting its
deferred-child request. Its generated request includes route and timed-vector
data. The threshold is not a source-fire world-position update interval.

## Route translation boundary

The state-4 actor callback runs `00449340`, then calls `0044B060` on the visual
component. `00449340` changes route progress at root `+0xE8/+0xEC`, builds the
deferred FireRescue request, and changes the shared size/bounds key track. On
completion it resets state `+0x70` and calls `00442BF0` on the object at root
`+0x100`. `00442BF0` forwards the pair to a container at that object's
`+0xE8`, where `004901E0` adds a two-DWORD pair when it is not already present.
This completion helper is route bookkeeping; it is not a transform setter.

`FUN_0044A6A0` is a seven-byte helper whose current-hash body is `mov eax,ecx`,
`mov byte ptr [eax+0x50],1`, `ret`. The route caller passes the visual
subobject at root `+0x12C`, so this helper returns that same subobject and marks
its visual/bounds state dirty. At each `00449340` emission gate, the builder
copies visual `+0x28..+0x30` to the generated request's `ZZ_location` at
`+0x7C..+0x84` before updating anything. Later in the same helper it writes
the current route candidate (`local_50/local_4c/local_48`) back to visual
`+0x28..+0x30`.

This changes the embedded visual's placement, not the FireRescue root world
translation at `+0x4C..+0x54`. The renderer effect is direct: `0044B060` uses
the visual position for bounds/culling, and `0044A840` adds its components to
transformed vertices written into the fourth child sprite's vertex array.
`0047F960` consumes those child vertices for polygon submission. The existing
same-hash visual evidence documents that consumer; the current-hash x86
instruction excerpt for the accessor and vertex writes is retained in
[`00449340_visual_position.asm`](00449340_visual_position.asm). The exact full
placement chain for each of the other three flame quads remains less direct.

## Generated route-child request and current port

At each `00449340` emission gate, retail creates another `RCS_FireRescue`
UserData request and copies the pre-update visual base into `ZZ_location`.
For the authored no-`C_Model` route case, current-hash instructions confirm
these request fields:

The pinned request/factory path is `00449340` → `0049B760`/`0049B690` class
lookup → `00429C00` descriptor lookup → `0049D130`/`0049D4C0` submission →
factory `0044A470`. A fresh root uses constructor `0044A330`; its FireRescue
UserData initialization is `0049D160`, and setup then runs `00448D60`. Descriptor
`00448070` installs a class-data allocator at vtable slot `+0`; the route
builder calls it before filling the request. A fresh hash-pinned trace shows
that `00448210` zeroes all `0x88` UserData bytes both when allocating a new
block and when reusing an available cached block. The root constructor and
reuse method then use `0049D160` to attach and size-check that pointer;
`0049D160` itself does not clear the data. The dynamic route request therefore
has proven zero values for untouched fields, distinct from placed BSP
defaults. See
[`fire_rescue_route_userdata_allocation_2026-10-21.md`](fire_rescue_route_userdata_allocation_2026-10-21.md).

- `F_FireType`, `C_Model`, and `A_Name` are explicitly zero. The dynamic child
  therefore has no named-fire loop and does not pass the name gate used by
  random spread.
- `H_Radius` and `H_Height` copy the parent's current visual vector at
  `+0x44/+0x48`. `T_UseHeading` is one; `T_Heading` is the normalized route
  direction derived from visual-base endpoints at plus/minus half of the
  parent's `+0x110` radius. `L_FloorMe` copies from the source UserData.
- `P_NumKeys` is reset and populated with three no-model child records:
  `(root+0x108, root+0x10C, distance)` at offset 0; the same dimensions at the
  existing second-key timestamp plus `root+0xB0 * 1000`; then
  `(0, 0, distance)` at the existing third-key timestamp plus
  `root+0xB4 * 1000`. For these authored no-model actors, `root+0x108/+0x10C`
  are initialized from `H_Radius/H_Height`. The retained x87 distance is the
  endpoint separation; it equals `root+0x110` when the route direction is
  unit length. The first explicit timestamp is zero.
- `00448D60` adds the generated key offsets to its current game clock before
  inserting them in the shared timed-vector track. The route updater's
  `004B9400` converts the offset expression to an integer toward zero after
  correcting x87 nearest conversion. The route request allocator clears all
  UserData first, and `00449340` writes each active key's value and timestamp;
  unused key data starts at zero.
- `M_Smoking` starts at zero in the request. If the parent has smoking enabled,
  its `+0x118` counter enables smoke on every second qualifying route child.
  `ZZ_spawn_message`, `K_SprayTime`, and `N_IsAlive` are not written by the
  request builder and remain zero from the allocator's clear. The `00448D60`
  initializer normalizes this zero `K_SprayTime` from radius through
  `00447770`; that is the same radius-based range currently used by the host.

Only two authored `moveto` FireRescue actors appear in the current typed scene
and script exports: MBase `mortarfire1` to `mortar1` and Gas R `Fire42` across
four PathPoints. Neither has a `C_Model`, so these routes take the verified
radius/height branch. See the work-queue audit ledger for the pinned map and
export hashes. The non-null model path remains unsupported. Current files were
rehashed during this audit:

| Evidence | SHA-256 |
|---|---|
| `mbase.bsp` | `4ff3b18dd809c619a6734673a7f90bd2e98d9fc9518ee7bf94ece4b6ff90b266` |
| `mbase.evs` | `75cee4301a5d0dc029db55f5b405d4011867092d4b94acafa0fc76e757aec565` |
| MBase `entities.json` | `bf62ce8b332de1f884e70aebc10e9e027040a66168371ece08836e1ae278d0a2` |
| MBase `scripts.json` | `8fabb9e49c514d22dd6341d5cddaab6e970a648012b969f1a3c6c06feb8d0bf1` |
| `gas_r.bsp` | `f6822f8d598d2888896165bbee8b554351230cd76ad6331abf0d735a03b54cbc` |
| `gas_r.evs` | `1f83f48debdbfe916c0b5e818f35e0dbf6cb9046a996dfecd75882e27e3fa8c0` |
| Gas R `entities.json` | `7c1fa33cd1d38bf22fc231c7131fb0256ca9e2f74d57c29691a3e3bcada7f7f0` |
| Gas R `scripts.json` | `bc39433f7274748a9fdf8cd3ff7ad3d80b7f9a3b5088eb34331fcba41f3f3077` |

Both map exports and source scripts match the independently audited paths;
these hashes pin the authored no-model coverage claim.

The shared host snapshots the old visual position, queues a route child during
the state-4 callback, resets the parent's shared size track with the two keys
above, then moves/rebuilds the parent embedded visual. It drains the child
request after represented contact candidates. The fresh-child approximation
has no name, copied floor behavior, alternating smoke, route heading, and the
generated three-key size track. The route request allocator clears every
UserData byte before those fields are filled, including when it recycles a
cached data block, so untouched generated-child fields and unused key data
begin at zero. The host's zero `K_SprayTime` value and radius-based
normalization are source-backed; x87 edge behavior remains open. Retail factory
reuse attaches the new request's UserData and reruns FireRescue visual setup,
but root reuse eligibility, host pooling, ownership, and child retirement
remain unresolved. Exact callback cadence and sprite/RNG interleaving also
remain open. The renderer currently projects the route
vector to horizontal yaw for child orientation; the full retail heading frame
is not established. This is a source-backed host slice, not live route,
gameplay, or retail parity.

## Remaining work and limits

- Match x87 conversion at parent route-delay integer boundaries and generated
  key-time edges. The route request's active key count, values, and timestamps
  are written by `00449340`; unused data starts at zero.
- Establish queued-child setup, update ownership, and retirement before
  claiming full route parity.
- Trace the non-null `C_Model` dimensions path and exact rendered bounds.
- Establish callback cadence and compare a live route before claiming
  FireRescue route parity.

This is static evidence and a source-corrected host boundary. It is not live
visual, gameplay, or retail parity evidence.
