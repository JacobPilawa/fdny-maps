# Rescue HoseSpray particle template and draw path

## Evidence boundary

The fresh Ghidra review is pinned to `FDNYFirefighter.exe` SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` and
uses Ghidra 12.0.4 in headless, read-only mode against the existing project.
Selected current decompiler bodies are saved in
[`rescue_hose_spray_particle_trace_2026-10-20.c`](rescue_hose_spray_particle_trace_2026-10-20.c).
The project and temporary full logs remain local Ghidra/build data and are not
part of the GitHub mirror.

## Template keys and initial scalar

HoseSpray setup `00463700` appends two keys to its list at item `+0x3B4` via
`0047DB10`. In call-argument order, each record contains a resource ID, a
duration, a third scalar, a size scalar, and an RGB vector:

| Key | Resource | Duration | Third scalar | Size scalar | RGB |
| --- | ---: | ---: | ---: | ---: | --- |
| 0 | 14 | 2.0 | 0.75 | 3.0 | `(1, 1, 1)` |
| 1 | 14 | 0 | 0 | 5.0 | `(1, 1, 1)` |

`0047DB10` stores those seven DWORD values in each key. Setup ORs `0x8` into
the track flags at item `+0x3CC`. Particle initialization `0047D330` sets
the current key index at track `+0x04` to 0 and the adjacent index at `+0x08`
to 1 for this two-key list; the keys' indexing is checked by `0047D2B0`.

`0047D8A0` interpolates only when track flag `0x10` is set. HoseSpray's
configured flags have `0x10` clear, so the getter reads the size field at
`+0x0C` from the **current** key index (`track + 0x04`). At initialization
that is key 0 and the returned scalar is `3.0`. The `5.0` scalar belongs to
key 1 and is not the initial value. The current host sample uses 3.0 as its
preview size. This resolves the selected scalar but does not establish an
exact mapping from the Genesis polygon scalar to a world-space billboard
width.

The particle track updater `0047D660` advances the current index from the
retail clock and uses flag `0x8` when it reaches the end. The renderer calls
that updater before retrieving the size. The track's later key transition,
terminal-frame behavior, and effective lifetime under the caller's update
cadence are not ported. `0047FEA0` loops over pool entries and calls
`0047F960` once for each; the scheduler cadence of that pool dispatch remains
unresolved.

## Child setup, resource lookup, and submission

Particle initializer `0047F500` invokes `0047D330` and sets particle `+0xA8`
to `1.0`. `0047F960` updates the size track, conditionally integrates motion,
then submits through `geWorld_AddPolyOnce`. The submitted scalar is the
`0047D8A0` result multiplied by particle `+0xA8`; child render flags come from
`+0xA4`. HoseSpray contact code writes `0x20` to that child flag, but the host
renderer does not yet model it.

Resource lookup `0047D820` reads the resource ID from the current key and
resolves it with `00493D00` through the supplied manager. HoseSpray resource
14 in `hose.rrf` names `bsmoke.bmp` and `sm_alpha.bmp`, matching the preview
pair. The exact manager-to-RRF association and the final texture selected at
runtime remain unverified.

`0047D6D0` converts track values to output bytes through `004B9400`; that
helper is an x87 float-to-integer conversion, not an RNG call. The optional
`0047D6D0` branch is guarded by flags `0x6`, which are clear for the configured
HoseSpray flags `0x8`. No additional shared-random draws are attributed to
this color path. This does not resolve the source's alpha progression or
Genesis blend behavior.

## Host update and remaining gaps

The shared host stores a per-particle preview side length. Rescue HoseSpray
passes the source's current key-0 scalar `3.0`; extinguisher particles keep
their existing default `3.0`. This only makes the HoseSpray input explicit.
The host still draws camera-facing quads with constant alpha 191 and borrowed
two-second linear motion. Quad orientation and scalar-to-width mapping remain
provisional. The source's key advancement, final child material, exact alpha,
pool failure behavior, effective lifetime, and draw cadence remain open.

This is static trace and host compilation evidence only. No interactive
sequence, captured retail render, or parity comparison was performed.
