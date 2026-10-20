# Player locomotion frame axes (2026-10-20)

## Provenance

Retail input: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The trace is a read-only Ghidra 12.0.4 review of Player `0044F340`, its frame
vtable, wrapper `0042BD50`, setters `00496210`/`00496360`, and constructor
`00453F8A`.

## Confirmed callback writes

The locomotion frame is Player root `+0x3460`, distinct from view follower
`+0x34A4` and held-target frame `+0x24`. `0044F340` calls frame vtable slot
`+8` at `0044FA75` with vector `(0,1,0)`. The wrapper dispatches to slot `+0xC`
(`00496210`), which normalizes and installs basis column `+0x10`: the up axis
is world `+Y`. A later slot `+0x14` call at `0044FA9C` reaches `00496360` and
sets basis column `+0x1C` from a vector computed using selected-item view/aim
data. The setters repair the remaining axes by normalization and cross
products. This refresh leaves frame translation `+0x28..+0x30` and parent
`+0x40` unchanged.

The constructor initially makes this frame identity, zero-positioned, and
unparented. Entity initialization later copies a transform into it, so the
constructor alone does not establish runtime axes. The callback does not
establish the full world orientation because the indirect second-setter vector
still depends on equipment view data. The normal Player callback is visible at
`00453860`; the recurring manager and cadence remain unknown.

## Host status

No movement transform was changed from this trace. `Player::step` still uses a
camera-pitch substitute for the retail `+0x3320` transform; the host movement
frame orientation and callback cadence remain provisional. Evidence logs from
the read-only headless review were temporary; this note records the durable
address-level result.
