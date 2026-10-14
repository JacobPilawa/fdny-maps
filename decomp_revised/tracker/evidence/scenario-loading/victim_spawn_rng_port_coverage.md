# Victim spawn RNG: native phases and viewer coverage

Status: the victim-local sequence and FireRescue visual setup are represented
in the Mac viewer. Earlier factory classes and retail runtime seed/cadence are
not fully integrated.

Binary: `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6fca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

## Native phase order

`ProcessEntitySpawnQueue` runs root `+0x20` for the complete batch, then root
`+0x24`. Factory registration orders FireRescue before VictimRescue and
VictimAltSpot after VictimRescue. The victim's first pass therefore occurs
after FireRescue initialization and before alternate spots are registered; its
second pass chooses a spawn spot after those registrations.

For the actor paths currently represented:

1. FireRescue initialization calls `0x0044B270`. For each new visual, enabled
   smoke consumes one range draw; each of three flame sprites and the glow
   consumes a speed draw and an initial-frame draw. A non-null named-fire
   configuration then consumes a sound-choice draw. The old project export
   and reused `FireEffectChoices` component preserve those draws. The exact
   current-binary binding from that sound condition to authored `A_Name` was
   not re-established in this focused pass.
2. VictimRescue initializes its sound controller before choosing model and
   initial ACT motion. Its initial deadline consumes one range draw. A Fatman
   model consumes `bounded(3)`, then an ACT with six motions consumes
   `bounded(3)` for its initial motion; other motion counts consume
   `bounded(count)`.
3. In the second pass, `SpawnVictimRescueAtRegisteredSpot` scales one shared
   random float to `[spot_count - 0.01)` and truncates it to the candidate
   index. A dispatched `roam` script consumes `bounded(3)` when it selects a
   six-motion walk animation. If all four initial roam offsets fail, that
   animation draw is skipped.

The viewer advances the FireRescue visual setup before victims and preserves
the victim sound-deadline draw, Fatman selection, initial motion, spawn-spot
selection, and roam-animation selection on one `Random` object. See [the
map-neutral actor slice](../runtime/preview_rescue_actor_slice.md) and [the
victim spawn factory trace](victim_spawn_factory_and_order.md).

## Limits

This is partial stream reconstruction, not seed parity. `RCS_MiscEmitter`
factory initialization comes before PlayerRescue, FireRescue, and VictimRescue;
its shared-stream draws are not included. Other actor constructors, sound
timing, scripts, runtime callbacks, the original time-based seed, and queue
scheduling can also affect later draws. The viewer cannot yet claim that a
given retail seed selects the same Fatman variant or spawn spot.

The copied previous-project FireRescue pseudocode is preserved under
`reference/previous-work/decomp/evidence/rng-startup/` and
`reference/previous-work/decomp/evidence/rng-callers/`. The reused host
component is `port/spikes/training-native-prototype/src/fire_effects.h`.
