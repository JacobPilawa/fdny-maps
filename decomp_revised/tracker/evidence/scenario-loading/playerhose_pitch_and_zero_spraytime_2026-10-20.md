# PlayerHose pitch guard and authored zero SprayTime

## Provenance

The retail executable is pinned by SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The authored `City_h.bsp` is pinned by SHA-256
`9bc788dcaceb477fea52efd49cfbb2fd8f86824907d73c819280710b04fcb27a`.

## Pitch bound relative to the route

`00466780` first clamps the pitch accumulator to the active hotspot's
symmetric `N_PitchSpread` and yaw to its symmetric limit. It then constrains
the resulting third aim axis to between 10 and 170 degrees from world up.
`00497AA0` computes the normalized dot-product angle between two vectors and
converts it to degrees; the steering trace identifies the other vector as
world up `(0,1,0)`.

For a route frame whose forward elevation is `theta`, the world-up angle is
`90 - theta`. Substituting that into the retail 10-to-170-degree interval
gives steering bounds `theta - 80` through `theta + 80` degrees. This explains
the host's current `routePitch +/- 80` guard. The equation is source-backed;
the exact join between the retail ActorInfo frame and the host rail/look-at
frame remains static and unverified, so no camera correction is claimed.

## Zero SprayTime in authored data

Retail `00465B90` creates one WindowFire child for each non-null
`ZF0..ZF8_FireModel` entry and copies the paired `SprayTime` into linked state
`+0x08`. `00468680` has no zero-divisor guard. When linked active and selected
gates `+0x0C` and `+0x0E` are set, it subtracts frame delta divided by `+0x08`
from the target timer. If the result is strictly below zero, it clears douse
gate `+0x0D`, resets the timer, updates geometry, and sets `+0x0F`.

`City_h.bsp` has `RCS_HosePath3` slot `ZF2_FireModel=fire11` with paired
`ZF2_SprayTime=0.0`. On a qualifying contact update with a positive frame
delta, the source division produces a positive infinity; subtracting it moves
the timer below zero and takes the normal douse-completion branch. This is an
authored case, not merely malformed-data speculation.

The shared host already copies the paired value, divides its frame delta by
that value without a special case, and checks the same strict-below-zero
condition. Its zero-value behavior follows the source arithmetic for positive
frame deltas. Message versus entity scheduling and live contact cadence remain
provisional, and no runtime or retail-parity check was performed.

## Retained evidence hashes

- [`hose_contact_and_playerhose_fresh_2026-10-20.c`](../fire-spread/hose_contact_and_playerhose_fresh_2026-10-20.c):
  `894d0914dea1043be8622c0465eb4fb5a8e341d68252e35e2925363b1f65ed38`
- [`playerhose_douse_windowfire_fresh_2026-10-20.c`](../fire-spread/playerhose_douse_windowfire_fresh_2026-10-20.c):
  `97bdaa2770f2db5429ff5f829c565c6374a28fbab96967f13921c7dd9a59b07f`
- [`00497aa0_compute_vector_angle_degrees_fresh.c`](00497aa0_compute_vector_angle_degrees_fresh.c):
  `874c343f5069890cd3978dd777a7d33b75fc80a0910984cd50b71b61f65c2000`
- [`playerhose_mouse_steer_fresh_2026-10-20.c`](playerhose_mouse_steer_fresh_2026-10-20.c):
  `b4684b597372066ce1eb6dbcaa7a06541977edf83c8c3ef00d7dcc55454a73a1`
