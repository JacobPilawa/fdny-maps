# FireRescue named loop audio port — 2026-10-15

## Retail evidence

The pinned input is `inputs/original/binaries/FDNYFirefighter.exe`,
SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The hash was rechecked for this trace. In initializer `00448D60`, the function
clears FireRescue root `+0x114`, then checks UserData `+4`. For a named fire it
allocates the sound instance, selects one of three sound-name pointers, and
configures the instance at FireRescue root `+0x7C` with mode 2 through
`004A12F0`; `004A11D0` starts the instance after the FireRescue visual setup.
The reused hash-matched Ghidra export
[`00448d60_FUN_00448d60.c`](../../reference/previous-work/decomp/evidence/rng-startup/00448d60_FUN_00448d60.c)
shows the pointer table in order: `FireBurn1`, `FireBurn2`, `FireBurn3`.
Direct instructions can be reproduced with:

```sh
objdump -d -Mintel --start-address=0x448d60 --stop-address=0x448e95 inputs/original/binaries/FDNYFirefighter.exe
```

`Data/ResDefs/sound.rrf` lines 116–118 define these names as 3D sounds with
minimum distance 200 and maximum distance 1200:

| Name | WAV source | SHA-256 |
| --- | --- | --- |
| `FireBurn1` | `Data/Snd/#fireAmedium.wav` | `3e554a30923ea268c4621a02760ed1b90e0e0a33c2a05b3d3b7e7b752aabe370` |
| `FireBurn2` | `Data/Snd/#fireAlight.wav` | `448a9046934c59198491987f5b7bf322f3ce15b292753f40361f62c06d54b5bb` |
| `FireBurn3` | `Data/Snd/#fireBlight.wav` | `c247ec02d6e3cb6e6e541909950a74089870e48f2b6d5352c6541092b3977532` |

The class initializer only creates the named sound instance when UserData
`A_Name` is present. Spread-created FireRescue records clear that field before
factory creation, so the port leaves those unnamed children silent, matching
the initializer gate.

## Port

`tools/export_evs_bundle.py` now bundles all three resources for every scene,
with each source-file hash and `sound.rrf` definition identity retained in
`sounds.json`. The shared viewer maps the existing source-ordered sound-choice
draw to the selected name, starts one owner-keyed 3D loop at `runtime.origin`
(the FireRescue root position), updates the listener and yaw each frame, and
updates the source when a routed fire changes position. A FireRescue that
enters terminal state 5 stops its owned loop. Exact FMOD mixing, attenuation,
and audible parity remain open.

## Validation

The rescue-all app packager regenerated the 13 scene bundles and copied the
FireBurn sources into each package. The shared viewer built successfully on
macOS. `--scene-audio-check` uses SDL's dummy audio device and verifies each
named fire has the selected bundled 3D loop, owner, source position, listener,
channel group, and 200/1200 distance metadata. All 13 Training/rescue/stage
packages passed, covering 370 named FireRescue loops. The same check retains
the existing authored ambient-loop and oxygen-category checks; the legacy
`--ambient-audio-check` option remains an alias. These are controlled port
checks and do not compare decoded audio output with a live retail session.
