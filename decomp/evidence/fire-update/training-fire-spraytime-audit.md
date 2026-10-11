# Training fire spray-time and douse behavior

Read-only Ghidra 12.0.4 review of the original `FDNYFirefighter.exe` plus the preserved Training BSP extraction. This resolves how the authored small-fire `K_SprayTime=0` value interacts with the original FireRescue spray routine and the extinguisher message path. No runtime capture or native gameplay change was used.

## Training's authored spray-time groups

`RCS_FireRescue.K_SprayTime` is a four-byte float at UserData offset `+0x18` in the BSP typedef ([`bsp-typedefs/training.json`](../bsp-typedefs/training.json); source BSP SHA-256 `32986212fa875764ca3d5c1630e54ad6315abaea70983203601192cec603a963`). Ghidra's constructor/UserData cross-check ties the FireRescue data block to the initializer ([`training-userdata-layout-audit.md`](training-userdata-layout-audit.md)). The seven placed Training records in [`entities.json`](../../assets/training/entities.json) divide into:

| BSP entity index | `A_Name` | `H_Radius` | `H_Height` | `K_SprayTime` |
|---:|---|---:|---:|---:|
| 162 | `fire1` | 150 | 250 | 0 |
| 165 | `fire2` | 150 | 250 | 0 |
| 166 | `fire3` | 150 | 250 | 0 |
| 163 | `fire4` | 500 | 800 | 4 |
| 169 | `fire5` | 350 | 800 | 4 |
| 167 | `fire6` | 500 | 800 | 4 |
| 168 | `fire7` | 500 | 800 | 4 |

These values are authored data; the table does not claim any runtime ordering beyond the BSP entity indices.

## Original spray routine

Preserved-project `DecompileFunctions.java 00448970` identifies the direct calculation in `00448970`: for a FireRescue not already in state 5, it subtracts

```text
(frame_delta / UserData[+0x18]) * (1 - (1 - influence) * (1 - influence))
```

from fire strength at object `+0x128`, clamps a result below zero to zero, and tests the terminal byte separately. Only when that byte is nonzero and the new strength is strictly below the double-precision constant `0.1` does it set state `+0x70` to 5, count `Fires Doused`, optionally stop the fire sound, and clear the flame-enabled byte. It sets the sprayed flag at `+0x104` after the branch. Ghidra `DataValue.java` reads of `004DF7B8` and `004DF7BC` give the little-endian double words `0x3fb99999:0x9999999a`, i.e. IEEE-754 `0.1`; `004DC50C` is float zero.

The zero spray-time result is verified against the original-x86 fixture [`fire-suppression-oracle.json`](../fire-suppression-oracle.json) (EXE SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e). With strength `1.0`, frame delta `0.1`, spray time `0`, and influence `0.25`, the original returns strength `0.0`. With terminal `0`, state remains 0 and no douse is counted; with only terminal changed to `1`, state becomes 5 and the douse counter and stop callback fire. This directly confirms that a positive accepted hit on a zero-spray-time Training fire collapses its strength to zero, while dousing remains a separate terminal decision. The zero-spray-time/zero-influence case is not generalized here; the original fixture produces a negative quiet NaN for that input.

## Extinguisher-specific consequence

Ghidra `AddressXrefs.java` locates the call from `00449A40` into `00448970` at `00449A74`; `InstructionContext.java 00449A74 10 5` confirms the message handler pushes `0` as the terminal argument. The handler accepts message `0x7530` (decimal 30000), the extinguisher suppression message documented in [`contact-list-trace.md`](../item-pose/live-extinguisher/contact-dispatch/contact-list-trace.md). Therefore extinguisher message delivery to any of the three zero-spray-time Training fires can set strength to zero, but that message path does not mark the fire doused, count it, stop its sound, or disable its flame.

If the separate FireRescue Think `00449FE0` later runs, its source behavior is also bounded: while the sprayed flag is set it skips strength recovery and clears that flag at the end of a full update; a later unsprayed full update in a non-state-4 fire adds `frame_delta * 4 * 0.1`, capped at 1.0 (`004E0510` is float 4.0; `004DF7B8` is double 0.1). This means the source supports a temporary strength drop followed by possible recovery rather than permanent dousing on the extinguisher message path. The unresolved Think dispatcher/cadence means this is a conditional source-level result, not a claim about the observed Training frame schedule ([`training-think-behavior-audit.md`](training-think-behavior-audit.md)).

## Provenance and limits

- Original EXE SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
- Ghidra 12.0.4 preserved project, all queries read-only: `DecompileFunctions.java 00448970`; `AddressXrefs.java 00448970 00449A40`; `InstructionContext.java 00449A74 10 5`; `DataValue.java 004DC50C 004DF7B8 004DF7BC 004E0510`.
- The authored BSP typedef and `entities.json` extraction identify Training inputs; the suppression fixture executes the original spray routine with controlled inputs. This does not establish which extinguisher influence values occur in a live Training session, the Fire Think cadence, or a complete Training startup/suppression trace.
