# Training ash descriptor presentation component

Snapshot: 2026-10-12. Original executable SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Recovered helpers

`src/player_ash_presentation.h` implements three bounded helpers used by
`0047F960`:

- `0047D420` selects record RGB at `+0x10..+0x18` or, with flags bit `0x20`,
  interpolates current × `(1-f)` plus next × `f`. The time fraction is stored
  as binary32 before `00446390` multiplies each vector; each product is then
  stored as binary32 before `0042D050` adds them. The result is cached at
  particle `+0x24..+0x2c`. The two binary64 duration guards are `+1e-28` and
  `-1e-28`; durations inside that interval select fraction zero.
- `0047D6D0` reads each descriptor's `+0x08` scalar, multiplies by 255, and
  passes the x87 value through `004B9400`. Despite the x87 nearest-integer
  intermediate, that helper corrects by the signed fractional residue and
  returns truncation toward zero; the callers keep AL. Flag `0x02` independently
  weights current by `(1-f)` and next by `f`. Otherwise flag `0x04` crossfades
  those bytes into current. Both then receive particle `+0x20` and the same
  truncating conversion. Flag `0x02` takes precedence if both bits are set.
- `0047D8A0` returns current descriptor `+0x0c` when flag `0x10` is clear;
  otherwise it returns `(1-f)*current + f*next` in x87 precision. Its caller
  `0047F960` multiplies by particle `+0xa8` and spills a binary32 alpha operand
  for `geWorld_AddPolyOnce`. The alpha helper has no near-zero-duration guard.

The shared fraction uses unsigned clock subtraction (`00502AFC` minus particle
`+0x14`), converted to signed x87 integer then adjusted by `2^32` when negative.
It divides by descriptor duration × animation rate (`+0x1c`) × `1000.0f`.
The renderer and byte helper preserve this x87 result; the RGB path stores it as
binary32 before passing the vector helpers. `0047D330` and `0047D370`
initialize particle `+0x20` to 1.0; `0047F500` initializes final opacity
`+0xa8` to 1.0. The activation path later replaces `+0xa8` with its
source-mapped random scale. `PlayerAshParticle` records the `+0x20` default,
and `playerAshPresentationInputs` maps its recovered animation fields into
these helpers. The non-unit scale cases below are controlled helper tests, not
claims about the initial Training values.

## Original comparison

`AshPresentationPcodeOracle.java` executes the original three helper entries,
the vector multiply/add helpers, and the actual `004B9400` byte conversion where
used. It supplies a valid five-record descriptor array and stubs only
`0048D490`'s bounds check for those valid indices. The fixture contains 11
controlled cases: Training flags and records, start/quarter/half/wrap timing,
non-unit channel scaling, all byte flag branches, direct-pointer RGB control,
and a non-Training alpha pair (`0.125` to `0.875`). Alpha comparison retains a
binary64 diagnostic of the helper's x87 result and checks the caller's actual
opacity multiplication/binary32 spill. The native check matches every RGB
word, byte channel, alpha diagnostic and spilled alpha word.

The native check obtains its five records from `PlayerAshPool::descriptorFor`
through `playerAshTrainingPresentationFrames`; it checks both native ash
containers against the hash-pinned original record words before running the
helper cases. This ties the component to the recovered Training descriptor
table rather than a second hard-coded native fixture.

Regenerate with the preserved read-only Ghidra project using
`AshPresentationPcodeOracle.java` and the original executable path, as listed
in `tools/ghidra/README.md`. The fixture is
`ash-presentation-pcode-oracle.json`.

## Limits and next link

This is a helper-level component, not a complete ash draw port. The bounded
direct-store scan keeps particle `+0x20` at 1.0 in the constructor/bind path,
but does not exclude alias or indirect writes
(`ash-particle-channel-scale-audit.md`). Resource-table instances, culling
context, vertex records, polygon-list state, and callback scheduling are not
fully connected. The
zero-duration alpha case is deliberately not
claimed: the original alpha helper divides without the color helper's guard,
and the current finite `Extended` model does not represent its resulting
non-finite x87 values. Continue from `0047F960` into the source-backed particle
constructor and renderer payload before claiming visible parity.
