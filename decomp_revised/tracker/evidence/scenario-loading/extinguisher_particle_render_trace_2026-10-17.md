# Extinguisher particle request and host rendering (2026-10-17)

Retail binary: `inputs/original/binaries/FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The fresh trace used Ghidra 12.0.4 with a temporary project under
`/private/tmp/fdny_equipment_trace_20261017/FDNYHeldToolAudit`; the saved user
Ghidra project was not opened or modified. Reproduction script:
[`TraceExtinguisherParticle.java`](../../../tools/ghidra-scripts/TraceExtinguisherParticle.java).
Decompiler output is `/private/tmp/fdny_particle_detail_20261017.log`.

## Retail trace

- Extinguisher setup `00461830` configures particle pools through `0047FE70`.
  The pool stores a resource-manager pointer and effect component and sets its
  capacity field to `-1`. The exact runtime link between that manager and a
  particular RRF file remains unresolved.
- After an accepted contact and a passed effect-time gate, `00461F80` reads the
  FireRescue frame origin through `00447870(fire + 0x12c)` and passes the
  resulting position to `0047FF30`. The pool reuses or allocates a particle;
  `0047F1A0` exposes the particle position at `+0x5c`, where the supplied
  origin is stored.
- The extinguisher then zeros particle acceleration at `+0x88`, writes the
  three random velocity components to `+0x7c` using ranges `[-50, 50]`,
  `[100, 400]`, and `[-50, 50]`, and sets byte `+0xa4` to `0x20`.
  `0047F2A0` integrates the particle's velocity and acceleration. With this
  setup, acceleration is zero and movement follows the assigned velocity.
- `0047F960` updates and submits the particle with `geWorld_AddPolyOnce` using
  four vertex records starting at `+0xd8`, the selected resource from
  `0047D820`, the evaluated size from `0047D8A0`, particle scale `+0xa8`, and
  flags `+0xa4`. `0047F5A0` builds those vertices from the particle frame. The
  precise AddPolyOnce quad orientation, clipping, blend interpretation, and
  all key interpolation have not been reconstructed.
- The particle setup's authored effect is two seconds long and includes a
  white color track and a `.75` alpha key. Port rendering currently uses that
  duration and alpha value as a fixed approximation; it does not yet evaluate
  the retail tracks over time.

## Resource candidate

The retail resource definition is `inputs/original/Data/ResDefs/rescue.rrf`,
SHA-256 `8ab12b7c148c4d984a912525e432967556c9267af5a1b0ea86451ed1ee123f33`.
Its resource 14 entry names `data\sprites\bsmoke.bmp` and
`data\sprites\\sm_alpha.bmp`. The copied 32-by-32 source BMP hashes are:

- `bsmoke.bmp`: `b56e9426aa51e99703cfde744a62669d74650d97046d8e5202be9dbabffe8b9d`
- `sm_alpha.bmp`: `2a6a376f7f878726823c767657767efa9194ff52594feac7259899be6f2af03e`

The source pair and hashes are recorded in the local bundle manifest by
[`export_extinguisher_spray_sprites.py`](../../../tools/export_extinguisher_spray_sprites.py).
The resource-manager-to-RRF association is still unverified, so resource 14's
pair remains a strong candidate rather than a proven runtime lookup.

The first packaged render diagnostic found that SDL rejects the retail
`bsmoke.bmp`: its 8-bit header declares 254 palette entries, but ten pixels
use indices 254 or 255. The exporter now writes local 24-bit BMP copies for
SDL, preserving the declared palette and padding the undeclared smoke colors
with black. Source BMPs are unchanged. The retail loader's handling of those
ten pixels is unknown. The bundled hashes and conversion status are recorded
in `.local/actor_exports/extinguisher-spray/manifest.json` (generated local
data, not checked in).

## Port behavior and limits

The shared viewer now consumes each eligible `ExtinguisherEffects` request,
spawning at `FireEntity::origin` with the source-shaped cooldown and random
velocity draws. The renderer expires particles after two seconds and draws
them as camera-facing 3-unit quads with constant alpha 191. Spawn timing,
random velocity, lifetime, and the source resource candidate are mapped;
quad extent/orientation, alpha key progression, blending, clipping, and the
`FireEntity::origin` to retail frame equivalence remain provisional.

The macOS `fdny-scene-viewer` target compiled and the rescue-all app bundle was
rebuilt at this particle-render checkpoint; the build output and packaged
executable match at SHA-256
`b1ec497e7f252484a3e1651aa3bc0da353933958d1e17b394868a18e38c99d88`. In a
later 2026-10-17 held-equipment build, the packaged Axe orientation diagnostic
passed scene startup with 121/121 render groups and 24/24 textures after the
BMP conversion; it exercises asset loading and port rendering only. The later
build hash is recorded in
[`equipment_action_pose_followup_2026-10-17.md`](equipment_action_pose_followup_2026-10-17.md).
The build reports Homebrew `json-c` was built for macOS 27 while the target is
macOS 13.3. No interactive hit, natural suppression, or pixel comparison was
performed; this is implementation progress, not a retail-parity check.
