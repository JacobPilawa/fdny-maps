# Training valve actor resource and frame audit

Date: 2026-10-12

## Model ID mapping

Training Valve3 (entity 159) and Valve5 (entity 164) both author `D_Model=5`. The preserved `evidence/original/rescue.rrf` binds `ROC_MODEL 5` to `data\\acts\\valve.act` (RRF SHA-256 `8ab12b7c148c4d984a912525e432967556c9267af5a1b0ea86451ed1ee123f33`). This is the actor-resource ID namespace, separate from the Training BSP `%Model%` records. BSP entity/model 5 is `ladder4`; it is not the valve resource.

Read-only Ghidra confirms this distinction in `00494500`: it searches the actor-resource table (`this+0x30`, count `this+0x34`) for an entry whose first word matches the requested ID and returns the resource pointer at entry `+4`. On a hit it constructs an 0x88-byte instance through `004922A0`, stores it through the caller output pointer, and appends it to the resource's instance list. On a miss it reports `Undefined actor resource #`. `00497E90` calls this path when the model mode is 2, passing its integer model ID.

The locally installed retail `Data/Acts/valve.act` has SHA-256 `c31454438bc25c3d3c5f46dfac6f2266b4c45cca5ffed172e468eb52f8785e21`. The existing `tools/extract_act.py` parser reads its geometry: Genesis3D BODY `0xF1`, 288 vertices, 321 LOD0 faces, six materials, one root bone, and no animation motions. The root bone has parent `-1` and an identity attachment matrix. Its bind-geometry bounds are approximately `(0.0345324,-40.8516,-17.4242)` to `(32.3899,92.7484,13.2481)`. Thus the ACT contributes no root-bone rotation or motion track; the PropModel frame code supplies the instance heading/pitch transform.

Extraction limitation: `extract_act.py` writes its JSON and OBJ before it errors while decoding the first embedded bitmap (`pixel_format=10`, flags `0x01`). The current decoder supports only paletted ACT textures. Geometry numbers above were read from those generated files in `/private/tmp`; proprietary mesh/texture output was not added to the repository. Extend true-color GeBm decoding separately before relying on the script for a full actor-asset inventory.

## PropModel frame construction

`00444A40` is the PropModel constructor. It copies authored `ZZ_location` into the embedded transform at `+0x4C/+0x50/+0x54`; its vtable is `004E1390`. The vtable's `+0x20` entry points to `00443860` (data reference at `004E13B0`). That callback loads `D_Model` through `00497E90` / `00494500`, sets scale to `(1,1,1)`, parents the loaded model frame at `+0x40` to PropModel `+0x24`, converts authored heading/pitch, and invokes `004968B0` then `00496B50`. Owner attach resolves this parent chain with `004977B0` before copying and negating resolved basis axis 0. The valve `+0x18` getter's `+0x28` triplet remains the authored world position copied to Hose Spray `+0xC70`.

The callback's angle instructions read full 64-bit constants: `004E1120=1/12`, `004E1118=360.0`, `004E12D8=90.0`, and the 32-bit pitch multiplier `004E12D4=-90.0`. The original expression is `-heading * (1/12) * 360 - 90`; executing instructions `00443948..0044399E` writes heading `-360.0f` (`0xc3b40000`) for Valve3 and `-180.0f` (`0xc3340000`) for Valve5. Both zero-degree authored pitches become `-0.0f` (`0x80000000`). The ACT root attachment and PropModel embedded frame begin as identity, but the Valve5 heading rotation changes its basis; Valve3's 360° result also retains small single-precision trigonometric residuals.

Operand-width pitfall: the instructions use `FMUL double ptr` at `004E1120`, `004E1118`, and `004E12D8`. Reading only the first four bytes made these look like `0x55555555`, zero, and zero, which falsely suggested that headings were discarded. `DataValue.java` now prints both 32-bit and 64-bit interpretations; use the disassembled operand width when interpreting a constant.

`tools/ghidra/TrainingValveFramePcodeOracle.java` executes the original angle-conversion instruction slice, both rotation routines, and recursive resolver `004977B0`. It checks the full immutable constant words and records the modified angle fields. Ghidra's emulator lacks x87 `FCOS`/`FSIN`, so those two operations use Java `Math` double results converted to x87 extended format. Current resulting axis words are therefore emulator-substituted pending a hardware/runtime comparison: Valve3 `[0x3f800000,0,0xb3510b46, 0,0x3f800000,0, 0x33510b46,0,0x3f800000]`; Valve5 `[0xbf800000,0,0x32d10b46, 0,0x3f800000,0, 0xb2d10b46,0,0xbf800000]`. The resolver positions are exact authored words: `[0x44ab0000,0xc3300000,0xc5020000]` = `(1368,-176,-2080)` and `[0x43d00000,0xc3300000,0xc5164000]` = `(416,-176,-2404)`.

Those model-frame words were then fed into the original `0044C840` owner P-code fixture. It commits the authored positions and currently produces Valve3 offset `[0xbf800000,0x80000000,0x33510b46]` and Valve5 offset `[0x3f800000,0x80000000,0xb2d10b46]`. These offset tail components inherit the Java trig substitution and must not yet be treated as hardware-confirmed.

### Callback scheduling evidence and limit

The best-supported app path for `00443860` is the normal owner pass `0042AEA0 → 00429520 → 0049C670`: the entity-staging helper calls each newly admitted entity's virtual slots `+0x20` then `+0x24`, and PropModel vtable `004E1390` binds `+0x20` to `00443860`. On this path, the owner-manager passes run before staged admission, so the setup callback follows those manager passes when a queued entity is drained. This supports a one-time setup callback on admission; it is not evidence of a recurring per-frame PropModel update.

The path is still a candidate for the two Training valves: successful Training factory dispatch for their specific PropModel records has not been traced through the staging helper, and other `00443860` call paths have not been ruled out. It also does not establish timing relative to Player USE. Player input calls into the owner pass, but the recurring caller of Player movement updater `00452570` remains unknown; the local USE callback route cannot supply that missing scheduler order. Thus neither “valve frame is ready before first USE” nor a per-frame refresh is established. See [`scriptactor-common-scheduler-audit.md`](../scriptactor-common-scheduler-audit.md) and [`player-update-scheduling-audit.md`](../player-update-scheduling-audit.md).

The angle fields, resource identity, authored positions, and source call path are established. Exact `FCOS`/`FSIN` float words and callback timing still need a safe original-runtime or equivalent x87 capture. Loader-to-staging dispatch for the Training valves, stream-node list state, follower updates, and downstream stream/suppression remain open.

## Reproduction inputs and hashes

| Input | Evidence | SHA-256 |
|---|---|---|
| Retail executable | `evidence/original/FDNYFirefighter.exe` | `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` |
| Resource registry | `evidence/original/rescue.rrf` | `8ab12b7c148c4d984a912525e432967556c9267af5a1b0ea86451ed1ee123f33` |
| Installed valve actor | `Data/Acts/valve.act` | `c31454438bc25c3d3c5f46dfac6f2266b4c45cca5ffed172e468eb52f8785e21` |

The Ghidra traces were run against the preserved Ghidra 12.0.4 project with `-noanalysis -readOnly`, JDK 21. Address-level facts: `00444A40`, `00443860`, `00497E90`, `00494500`, `004934E0`, `004922A0`, `004977B0`; the source exports remain under the existing local evidence exclusions.

## Remaining exactness step

Resolve x87 `FCOS`/`FSIN` outputs and the first PropModel callback relative to USE; then feed hardware-confirmed valve frames into the native owner state. Recover Hose follower parenting, stream-pose list writes, UI, and suppression cadence. Separately add true-color GeBm support to `extract_act.py`; this enables reliable cataloguing of ACT assets reused across later levels.
