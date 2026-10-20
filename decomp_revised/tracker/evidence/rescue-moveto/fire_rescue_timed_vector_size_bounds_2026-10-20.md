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
The audited `00449340` body contains no write to the FireRescue root world
translation `+0x4C..+0x54`.

The shared host previously moved `FireVisual::position` and `FireEntity::origin`
at each half-radius threshold and at route completion. That behavior was not
supported by the retail path. The host now advances route progress and keeps
the threshold as the future child-emission boundary without translating the
source fire. It does not yet reproduce the generated source size keys or
deferred children.

## Remaining work and limits

- Decode how the two trailing `moveto` values select timestamps across the
  source and child tracks, including interactions with consecutive `setsize`
  commands.
- Port the shared route-generated size track and deferred child request,
  then establish the queued child's setup, update ownership, and retirement.
- Trace the non-null `C_Model` dimensions path and exact rendered bounds.
- Establish callback cadence and compare a live route before claiming
  FireRescue route parity.

This is static evidence and a source-corrected host boundary. It is not live
visual, gameplay, or retail parity evidence.
