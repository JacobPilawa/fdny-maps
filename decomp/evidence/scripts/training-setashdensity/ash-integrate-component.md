# Training ash integration: `0047F2A0`

Snapshot: 2026-10-12. Executable SHA-256:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Recovered source behavior

`0047F2A0` reads the global timestep at `00502B04`. For an ash particle it:

1. Multiplies the vector at `+0x88` by `dt` through `00446390`, then adds the
   rounded vector into `+0x7C`.
2. Multiplies the updated `+0x7C` by `dt`; separately multiplies `+0x88` by
   `dt` twice; sums the latter vector with the former through componentwise
   helper `0042D050`; then adds that result into the position/origin at
   `+0x5C`.
3. For each `+0x7C` component, computes the rounded float `dt * +0x94` and
   applies the original directional clamp. It compares the float component
   against the binary64 threshold at `004E3540` (`1e-5`) and uses the original
   binary32 zero sentinel at `004DC50C` (`+0.0`).

The helper `0042D050` adds vectors component by component; it is not a cross
product. The routine's binary64 clamp threshold at `004E3540` is exactly
`1e-5`. `player_ash_integrate.h` retains the exact field mapping and exposes an
adapter for `PlayerAshParticle`: `+0x7C` position, `+0x88` velocity,
`+0x94` acceleration, and `+0x5C` origin. It receives `dt` explicitly so it
does not assume who schedules the particle update.

## Timestep source

The global timestep at 00502B04 is field +0x4C of the clock wrapper at 00502AB8. On the recovered application idle path, owner callback 0042AEA0 calls 0042C080 to update that field before the owner manager pass. 0042C080 selects a timer subobject, whose 0042BF70 update converts elapsed Windows milliseconds to seconds using 0.001 and caps the interval at 0.1. The callback and field transfer are source-mapped; who recurringly calls Player root slot +0x14, and thus when the ash pool runs, remains unknown. Full query and ordering evidence: `../../game-clock/frame-delta-provenance.md`.

## Original comparison

`AshIntegratePcodeOracle.java` runs original `0047F2A0` and its actual
`00446390`/`0042D050` helpers under Ghidra EmulatorHelper. It overrides only
the timestep; hash-pinned image data at `004DC50C` supplies the zero sentinel.
Eight finite cases include varied vectors, signed zero, both clamp directions,
and the adjacent binary32 values around the binary64 threshold. The native
`player_ash_integrate_pcode-check` compares all six changed vector components;
all eight cases match bit-for-bit. CTest `player_ash_integrate_pcode` passes.

## Limits

The oracle resets the x87 control/status state and provides a controlled global
timestep, so these cases establish the isolated routine's arithmetic and
branches for the authored inputs. A float cannot equal the binary64 `1e-5`
constant exactly, so that comparison boundary is not directly representable.
The recovered `0047F960` caller invokes `0047F2A0` when particle byte `+0xA0`
is set, after finite-animation advancement and before later presentation gates.
The helper is not yet wired into a source-verified Player cadence. The static
global timestep producer and its position before the owner manager pass are
recovered, but the Player callback cadence, live clock values, full particle
state trace, and whole-Training shared-RNG parity remain open. The routine
makes no RNG calls.
