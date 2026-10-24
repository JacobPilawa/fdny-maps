# Fire memory-value audit — 2026-10-21

## Result and limits

The strongest identified read-before-initialization risk is still first-flame
sprite +0xD0. There is also an unchecked divide in floor alignment that can
generate non-finite orientation values from an invalid collision normal.
Neither is a captured retail glitch. No specific buffer overwrite or previous
allocation containing a lethal alpha has been established.

This is a read-only instruction audit against the same retail executable as
[the original findings](fire_glitch_findings.md), SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
No executable or gameplay source was changed.

## 1. Ordinary heap reuse can supply large positive floats

Fire visual setup allocates each flame sprite with `operator new(0x168)`
at `0044B363`/`0044B368`. The new wrapper at `004A5A84` reaches the allocator
at `004B7D5F`; its ordinary heap branch passes allocation flags zero at
`004B7D95`. The allocator also has a small-block path. Neither the inspected
sprite constructor nor animation initializer clears all 0x168 bytes.

Constructor `0047F850` calls `0047D370` and `0047F500`. The latter explicitly
clears the override-enable byte at +0xD4, but leaves float +0xD0 unwritten.
Falloff routine `0044A7A0` reads that float at `0044A829` without checking
the enable byte. Normal updates repair it through `0044A610` -> `0047F1F0`.
The byte-divider delay described in the original findings can postpone that
writer even when Think precedes contact in the normal owner order.

Freshly committed, initially zero-backed pages need not expose this problem.
Reused heap payload can retain data from an earlier owner. A large float in
this field needs no overflow and no illegal write: ordinary bytes from a prior
allocation suffice if they reach this slot and are not cleared. Reuse of a
larger free region or dynamically sized buffer is a possibility, not an
identified allocation history in this game.

Examples, computed by [fire_memory_patterns.py](fire_memory_patterns.py):

| Old bytes | Reinterpreted binary32 | Potential significance |
| --- | ---: | --- |
| `42C80000` | 100 | Ordinary coordinate/size-like float, excessive as alpha |
| `447A0000` | 1000 | Same issue, with a larger multiplier |
| `437F0000` | 255 | A normal floating color-component value would be excessive as alpha |
| `44444444` | about 785.067 | Even text bytes can encode a large positive float |
| `000003E8` | about 1.4013e-42 | Integer 1000 does **not** become float 1000 by reinterpretation |
| `CDCDCDCD` | -431602080 | A negative value; not a straightforward positive burn spike |
| `DDDDDDDD` | about -1.9984e18 | Same caveat |
| `7F800000` | positive infinity | Non-finite input, with branch/arithmetic-dependent consequences |
| `7FC00000` | NaN | Unordered comparisons; do not assume positive lethal damage |

These are illustrative bit patterns, not observed game memory. This audit
does not claim the game's allocator uses the debug fill patterns above.
In particular, "garbage memory" is not automatically a huge positive float.

### An important counterpoint: same-class sprite reuse is usually bounded

A linear instruction scan of literal allocations of 352,356,360,364,368 bytes
found 360-byte allocations in several effect systems. The 364-byte allocation
at `00446743` constructs a sprite-derived object (`004463F0` calls
`0047F850`). The inspected literal sites therefore do not identify an unrelated
same-size object that deliberately leaves a huge alpha at offset +0xD0.
This scan is not an allocation census: runtime-sized buffers, different-size
free-block reuse, other modules, and computed sizes are outside its coverage.

The normal explicit sprite-alpha setter has direct call sites in the fire
alpha writer and at `004672D9`, where the supplied argument is 0.5. The fire
writer normally caps flame alpha to 0.999 and glow alpha to one. A previously
updated fire sprite therefore would normally leave a safe alpha behind.
No particular previous owner producing an excessive residue is proven.

## 2. Floor placement has an unchecked normal-length division

`00448630` traces downward to place the fire. On collision success it takes
the normal stored at globals `00502C08..00502C10` and calculates:

```
inverseLength = 1 / sqrt(nx*nx + ny*ny + nz*nz)
up = normal * inverseLength
```

Fresh instructions `0044883A..004488A9` contain the square root, reciprocal
and multiplications, without a zero-length or finite-value guard between the
collision-success branch and those operations. A zero normal yields infinity
then NaNs under masked floating-point exceptions. The resulting vector is
copied into the visual orientation; subsequent cross products and transforms
can propagate the non-finite values to vertices. This can create malformed
geometry without a high FireRescue strength.

This is a conditional arithmetic defect, not proof that a successful
`geWorld_Collision` returns zero normals in either reported level. The actual
collision-output validity contract must be checked in the supplied Genesis
DLL or captured live. Nor is a malformed polygon established as an
"infinitely high" flame: the graphics driver can reject or mishandle such
coordinates in different ways. It does not directly explain excess burn.

## 3. Other suspicious-looking cases do not yet supply a trigger

- Sprite constructor leaves vertex position data partly unwritten, but the
  normal FireRescue path rebuilds geometry after a significant size update.
  `0044B060` also skips polygon submission for a near-zero size vector.
  Uninitialized constructor vertices alone therefore do not establish an
  actual read-before-placement on this path.
- Timed-vector interpolation (`0044AF2F..0044AF55`) divides by the difference
  between key times without a zero-denominator guard. Duplicate active
  endpoint times can produce non-finite values. Normal random-child grow/end
  offsets are distinct; selection of later keys uses strict ordering. No
  reachable duplicate-current-endpoint case causing the reported glitch was
  established. A similar-looking divide is not by itself evidence of a bug
  happening during ordinary random spawning.
- Spawn consumes `P_NumKeys` with four local key-pointer slots and does not
  visibly cap its loop count to four in `0044912D..004491C5`. A corrupt count
  would be unsafe, but the inspected Bar/Gas placed definitions have count
  zero, and the normal random-child builder writes three. There is no
  identified writer producing a corrupt count.
- Strength itself is explicitly initialized to 1; generated UserData is
  zeroed before being populated. The RNG masks its output into [0,1).
  Those parts are substantially less suspicious than the sprite-alpha hole.

## Practical interpretation

The source supports investigating **uninitialized heap payload** first for
instant damage, and **invalid collision/placement values** for the column.
It does not support naming a known buffer overflow or a confirmed common
producer for both symptoms. A controlled allocation-poison experiment would
establish susceptibility but still would not establish natural occurrence.

A live trace should record first-sprite +0xD0/+0xD4 before its first contact,
allocation/free ancestry, collision normal, visual matrix, size vector, and
submitted vertex coordinates. Those distinguish stale payload, an invalid
normal, and a later overwrite. Keep root +0x128 strength in the same record.

Supporting fresh instructions are in
[fire_memory_audit_excerpts.asm](fire_memory_audit_excerpts.asm); constructor
details are also retained in the earlier
[alpha initialization chain](../tracker/evidence/fire-spread/fire_sprite_alpha_init_chain_2026-10-20.asm).
