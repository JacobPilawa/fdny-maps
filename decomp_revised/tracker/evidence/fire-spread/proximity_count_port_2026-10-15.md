# FireRescue nearby-fire population counter port

## Plain-language explanation

Each named fire remembers whether its last Player contact point was within 750
units. Entering or leaving that range changes one shared fire count, which
affects the wait before that fire can try spreading again. The counter includes
fires exactly 750 units away, although those fires do not attempt a spread roll
until they are closer than 750.

## Retail behavior

The pinned retail executable is
`FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
`00448D60` initializes FireRescue root `+0x120` to float 99999
(`0x47C34F80`). The callback
`004483A0` skips FireRescue mode 5, then computes a truncated maximum-axis
distance and calls `00448320`. That function checks the nonempty-name string at
root `+0x48`, adjusts aggregate `DAT_00502B90` across the inclusive 750-unit
boundary using the previous root `+0x120` value, and stores the new distance.
Its spread attempt gate is separate and strict: the new distance must be below
750. The selected instructions are preserved in
[`00448320_proximity_count_assembly_excerpt.asm`](00448320_proximity_count_assembly_excerpt.asm).

The aggregate is consumed by `00447A90` when deriving a successful parent's
next spread deadline. Cleanup `00448550` subtracts the fire from the aggregate
when its name is nonempty and the remembered distance is at or below 750, then
sets `+0x120` to float 9999999 (`0x4B18967F`). The initializer later writes
99999 back to `+0x120`. Mode 5 by itself does not adjust the count because `004483A0` returns
before `00448320`; an already-counted fire stays counted until cleanup or reuse
handles the decrement.

## Port behavior and limits

`PreviewFire::lastSpreadProximity` represents root `+0x120`, and
`nearbySpreadFireCount` represents `DAT_00502B90`. The host updates them in
candidate order immediately before that fire's spread-attempt check. This
matches the source transition order for the viewer's single Player candidate
and stable fire vector. Generated children have an empty name gate and do not
affect the count. Already-counted mode-5 fires stay resident and counted in the
host, matching the retail callback behavior until teardown.

The viewer currently has no FireRescue destruction or pool-reuse path. It resets
the aggregate when loading the scene. Retail list membership, all candidate
types, frame cadence, and the exact cleanup timing during a live scene change
remain unverified. This implementation does not claim full multi-candidate or
actor-pool parity.
