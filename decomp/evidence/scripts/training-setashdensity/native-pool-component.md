# Native Player ash-pool component

`src/player_ash_pool.h` models the source-mapped state and direct draw ordering
recovered from `004465d0`, `004463f0`, `0047f500`, `00446510`, `004467e0`, and
`00446470`. It is a partial component, not a complete `0x16c`-byte engine
object, game-loop integration, or visual-parity claim. The additional Ghidra
query and field recovery are recorded in `ghidra-helper-recovery.md`.

## Recovered state

- There are 100 entries; density starts at `1.0` and the Player script clamps
  it to `[0,1]` at child `+0x54`.
- Descriptor containers 0 and 1 receive the same five six-word records, with
  IDs 200 and 201. Their flags are `0x35` while records are added and `0x3c`
  afterward. Entry `i` is constructed with container `i & 1`.
- `0047f500` establishes active `+0x30 = 1`, animation-enabled `+0xad = 1`,
  position-initialized `+0x78 = 1`, vector-dirty `+0xa0 = 0`, opacity
  `+0xa8 = 1`, and zero vectors at `+0x7c`, `+0x88`, and `+0x94`. Its call to
  `0047d330` sets activation scale `+0x1c = 1`. The object constructor sets
  deadline `+0x168 = 0`. The spawn-position vector at `+0x5c` is not initialized
  by these stores, so the component represents it as unknown until spawning.
- Initialization and tick require explicit engine-service callbacks. Opaque
  constructor, scene transform, and presentation work cannot silently default
  to no-ops. These receive the shared `Random` where indirect RNG use could
  affect parity. The direct animation reset in `0047D300` is now modeled.

## Update and activation order

- Each tick visits all 100 entries: `00446510` first, then presentation
  `0047f960`. The prefix scan happens afterward and processes
  `int(100*density)` entries. Density therefore changes spawning only; it does
  not skip the all-entry update/presentation pass.
- An active entry whose unsigned deadline is due stores
  `now + 99,999,999`, then draws `(-40,40)`, `(0,10)`, `(-40,40)`, and
  `(-30,-5)`. `0042d280` writes three scalars as a vector. The first draw is
  stored at `vector7c.z`; the second is added to its prior Y; the third becomes
  X. The fourth becomes `vector88.y`. The wrappers set dirty flag `+0xa0`.
- An inactive entry in the scanned prefix draws angle `(0,6.2831855)` and
  distance `(10,400)`, then uses the player/camera transform chain to form its
  spawn point. `00446470` stores that point at `+0x5c`, selects animation
  indices 0 and 1 and timestamps them through `0047D300`, draws `(0.5,1.5)`
  into `+0x1c`, sets active, draws `(0.5,1.5)` into `+0xa8`, clears deadline,
  then draws `(-80,-10)` into `vector7c.y` with X/Z set to 0.
- `tick` now advances the finite ash animation inline before the remaining
  presentation service. It reuses the original-compared `SpriteAnimation`
  implementation with the five literal descriptor durations
  `0.5,0.5,0.5,1.0,1.0`, descriptor flags `0x3c`, the particle's scale and
  timestamp. The fifth due transition deactivates the particle; because this
  happens before the later density scan, an inactive prefix slot can reactivate
  in the same pool tick. This is component integration, not Player scheduling
  or visible-render parity.

## Limits

The direct state writes and draw order are source-mapped, and finite animation
advancement is now connected to the pool. Scene callbacks are not reconstructed
here. Computed engine calls, visibility gates, polygon construction, and the
indirect-call closure remain open.
The spawn-point chain includes computed Player/camera methods. This component
does not identify every indirect RNG consumer in those closures, implement
ash rendering, validate Training cadence, or establish complete RNG parity.

The focused check verifies descriptor words/flags, initial modeled fields,
the 400 direct draws for 100 due entries under no-draw callbacks, and 50
controlled half-density activations (250 draws). Callback no-ops are test
stubs, not claims about the original engine.

The check now also verifies the ash animation boundaries at 499/500, 1,000,
1,500, 2,500 and 3,500 clock units, plus deactivation before presentation and
same-tick reactivation during the later one-entry density scan. The existing
`--sprite-animation-check` separately compares 5,760 calls to original
`0047D660`; this proves the shared update routine but is not a live ash render
comparison.

The original-P-code update oracle at
`evidence/scripts/training-setashdensity/ash-update-pcode-oracle.json` executes
`00446510` after seeding the real game RNG and exercises its real calls to
`0047F1B0`, `0047F1C0`, `0042D280`, and `00498B30`. Five hash-pinned cases cover
due zero/nonzero vectors, not-yet-due, inactive, signed/unsigned deadline
comparison across wrap, and unsigned clock wrap. The
native check compares active state, deadline, dirty flag, output vector words,
and bounded draw count; all six cases match. This proves the isolated update
component only. It does not prove scene callbacks, Player pool scheduling, or
whole-Training RNG parity.

The activation oracle in `ash-activate-pcode-oracle.json` executes original
`00446470` with the real `0047D300`/`0047D2B0` animation-index reset and real
bounded RNG. Three hash-pinned seeds compare activation flags, animation
indices/timestamp, spawn position, activation/opacity scales, deadline, velocity
vector words, and call count; all cases match. The caller's angle/distance draws
and scene transform are outside this routine-level comparison.
