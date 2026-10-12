# Training ash descriptors and finite lifetime

## Scope and provenance

Read-only Ghidra 12.0.4 inspection of the preserved `FDNYFirefighter.exe`
project; executable SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Queried existing function exports/instructions for `004465D0`, `0047DB10`,
`0047DBA0`, `0047D330`, `0047D2B0`, `0047D660`, `0047F960`, and
`004467E0`; read constant `004E0ED4` with `DataValue.java`. The preserved
project was opened with `-noanalysis -readOnly`; the query used an isolated
Java home under `/private/tmp/ash-trace-ghidra-home`. Existing instruction
exports are in `pool-construction-assembly.txt`, `rng-branches-assembly.txt`,
and `effect-assembly.txt`.

## Exact Training pool descriptors

At `004465D0`, the one-time initialization loop seeds ID `200` at
`0044660B`; five calls to `0047DB10` add five records, then the loop increments
the ID and repeats for ID `201`. `0047DB10` allocates a `0x1c`-byte record and
copies six supplied dwords after the ID: three scalar words at record offsets
`+4,+8,+0xc`, then the three-word value at `+0x10,+0x14,+0x18`. Thus both
containers receive these exact values:

| Record | ID 200 / 201 | Scalars (`+4,+8,+0xc`) | Trailing words (`+0x10,+0x14,+0x18`) |
|---:|---:|---|---|
| 0 | 200 / 201 | `3f000000, 3f800000, 3e4ccccd` | `3f666666, 3dcccccd, 00000000` |
| 1 | 200 / 201 | `3f000000, 3f800000, 3e4ccccd` | `3f666666, 3f000000, 00000000` |
| 2 | 200 / 201 | `3f000000, 3f800000, 3e4ccccd` | `3f000000, 3f000000, 3f000000` |
| 3 | 200 / 201 | `3f800000, 3f800000, 3e4ccccd` | `00000000, 00000000, 00000000` |
| 4 | 200 / 201 | `3f800000, 00000000, 3e4ccccd` | `00000000, 00000000, 00000000` |

These are literal constructor arguments and copied words, not inferred float
interpretations. `0047DBA0` initializes each descriptor container with flags
`0x35`; after populating each five-record list, `004465D0` applies
`(flags & ~1) | 8`, yielding `0x3c`. Object constructors at `004463F0`
receive container `i & 1` (`00446760–0044676D`), so the 100 entries alternate
between two containers with identical record words and distinct IDs. Each
object starts active (`+0x30=1` via `0047F500`), has animation enabled
(`+0xad=1`), and gets deadline `+0x168=0`; its animation initializer
`0047D330` starts timestamp `+0x14` at global clock, scale `+0x1c=1`, and
indices 0 and 1.

## Lifetime and density-scan timing

Direct instruction evidence at `0047D660`:

- It fetches the current descriptor at index `+4`, takes its scalar at
  descriptor `+4`, multiplies by animation scale `+0x1c` and constant
  `004E0ED4` (`1000.0f`), adds timestamp `+0x14`, then rounds with
  `004B9400` and compares unsigned against global clock `00502AFC`.
- If due, it stores current clock into `+0x14`, increments current index
  `+4`, validates it through `0047D2B0`, and if that reports completion and
  descriptor flags `+0x18` include bit 3, clears active byte `+0x30`.
  It then sets the next index `+8` to current+1 and validates that too.
- `0047D2B0` returns not-complete for an index within count. With bit 0 clear,
  it clamps an index at/past count to the last record and returns complete.
  Training flags `0x3c` have bit 0 clear and bit 3 set.

The constructor `0047D370` stores the selected collection pointer at particle
`+0x0c` and copies collection flags to particle `+0x18`. `0047D660` indexes
that collection and reads each frame duration from the selected record's
`+4` word. Training's five durations are therefore `0.5, 0.5, 0.5, 1.0,
1.0` seconds. With the initialized scale 1.0, the corresponding global-clock
intervals are 500, 500, 500, 1,000, and 1,000 units. Starting at index 0,
five due steps advance through indices 1, 2, 3 and 4, then attempt index 5,
clamp to 4, report completion, and clear active. The nominal sum is 3,500
clock units. Because each due step resets the timestamp to the clock actually
sampled, missed intervals can make elapsed lifetime longer. This is a
code-derived nominal duration, not a live observation; actual clear occurs on
the first presentation call that completes the fifth due transition.

`0047F960` calls `0047D660` when `+0xad` is set, then tests active byte
`+0x30` for presentation. `004467E0` calls `00446510` and then `0047F960`
for each entry before it computes the density prefix and scans for inactive
slots. Consequently an entry may be cleared by presentation and become
eligible for that same callback's later scan if its fifth-frame deadline is
already due. The all-entry pass occurs even at density zero; density only
limits the later scan. Conversely, the constructor's active/deadline fields
remain intact through the `setashdensity 0` assignment.

## Evidence boundary and next step

**Directly established:** descriptor words, both IDs, container flags, entry
alternation, constructor binding of the collection pointer/flags, the five
frame durations, finite-index clamp, bit-3 deactivation, and
update/presentation-before-scan order.

**Inferred from those instructions:** five interval durations, or nominal
3,500 global-clock units, for an otherwise untouched entry; possible same-call
deactivation-before-scan once that deadline has elapsed.

**Unresolved:** exact global-clock elapsed time from each constructor's
`0047D330` timestamp to the first Player `004467E0` callback, and whether
other callbacks mutate these fields before then. So static evidence cannot
say how many initially active slots are inactive at the first density scan.
No reliable exact first-scan slot state or live lifetime is claimed.

Highest-value next step: get a bounded live Training trace recording
`00502AFC`, each entry's `+0x14`, `+4`, `+8`, `+0x30`, and the entry/exit
clock around the first `004467E0`; this would resolve constructor-to-callback
delay and confirm whether any of the 100 slots deactivate before its first
density scan. The existing activation/update P-code fixtures cover their
respective routines but do not provide this schedule evidence.
