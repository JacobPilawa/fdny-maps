# Training FireRescue UserData schema and sound RNG gate

Verified from the original Training BSP typedef, the authored FireRescue entities, and read-only Ghidra analysis of the matching executable. No live gameplay capture was used.

## Raw BSP typedef

Training ENTDATA entity 250 is a typedef record: `classname=%typedef%`, `%typename%=RCS_FireRescue`. Its ordered field epairs include repeated `%defaultvalue%` markers, so a dictionary-based entity parser would discard information. `tools/extract_gbsp_typedefs.py` reads the raw epair sequence and preserves the typed fields at `evidence/bsp-typedefs/training.json`.

| Offset | Field | BSP type | Bytes |
|---:|---|---|---:|
| `+0x00` | `ZZ_spawn_message` | `int` | 4 |
| `+0x04` | `A_Name` | `string` | 4 |
| `+0x08` | `C_Model` | `model` | 4 |
| `+0x0C` | `F_FireType` | `int` | 4 |
| `+0x10` | `H_Radius` | `float` | 4 |
| `+0x14` | `H_Height` | `float` | 4 |
| `+0x18` | `K_SprayTime` | `float` | 4 |
| `+0x1C` | `L_FloorMe` | `int` | 4 |
| `+0x20` | `M_Smoking` | `int` | 4 |
| `+0x24` | `N_IsAlive` | `int` | 4 |
| `+0x28` | `P_NumKeys` | `int` | 4 |
| `+0x2C` | `P_Key1Size` | `point` | 12 |
| `+0x38` | `P_Key1TimeOffset` | `int` | 4 |
| `+0x3C` | `P_Key2Size` | `point` | 12 |
| `+0x48` | `P_Key2TimeOffset` | `int` | 4 |
| `+0x4C` | `P_Key3Size` | `point` | 12 |
| `+0x58` | `P_Key3TimeOffset` | `int` | 4 |
| `+0x5C` | `P_Key4Size` | `point` | 12 |
| `+0x68` | `P_Key4TimeOffset` | `int` | 4 |
| `+0x6C` | `T_UseHeading` | `int` | 4 |
| `+0x70` | `T_Heading` | `point` | 12 |
| `+0x7C` | `ZZ_location` | `point` | 12 |

The field widths total `0x88`, matching the runtime class-data size.

## Ghidra corroboration

- FireRescue constructor `0044A330` calls `0049D160` with destination parent `+0x78` and size `0x88`; this is the UserData block.
- Placed-fire initializer `00448D60`, manually defined at its vtable-referenced entry in a temporary Ghidra analysis project, calls the fire-effect initializer first with UserData `+0x20` (`M_Smoking`). It then tests UserData `+0x04`; when nonzero, it passes that value as `char *` to `00402790`, sets up FireBurn playback, and calls shared RNG `00498B30` at `00448E45` before integer conversion and choosing one of `FireBurn1..3`.
- The initializer's nearby reads also line up with the typedef: `+0x08` model, `+0x10/+0x14/+0x18` radius/height/spray time, `+0x1C` floor flag, `+0x28` key count, key data at `+0x2C..+0x68`, heading at `+0x6C/+0x70`, and location at `+0x7C`.
- Staged entity initialization `0049C670` dispatches FireRescue primary slot `+0x20` to `00448D60`, so a successfully created Training fire reaches this initializer.

Together, the BSP's second field `A_Name:string` and Ghidra's pointer/string use at `UserData+0x04` establish the mapping. It is no longer provisional.

## Training RNG consequence and limits

Training FireRescue entities 162, 163, 165–169 have `A_Name` values `fire1`, `fire4`, `fire2`, `fire3`, `fire6`, `fire7`, and `fire5`; each also has `ZZ_spawn_message=10000`. Every successfully initialized Training fire therefore takes the named FireBurn selection branch and consumes one conditional shared-RNG draw after effect initialization. The resource table maps `FireBurn1..3` to `#fireAmedium.wav`, `#fireAlight.wav`, and `#fireBlight.wav`.

The controlled `fire-entity-oracle.json` call fixture reports 9 draws for its null-name case and 10 for its nonnull-name case. That fixture stubs the floor callback `00448630`; a bounded Ghidra direct-call closure from `00448630` to `00498B30` finds no direct path (155 direct functions visited, 125 indirect call sites not followed). Thus the additional named sound draw is verified; do not use the isolated fixture count as a complete original Training startup capture or claim all indirect effects are covered. The native initializer's `!A_Name.empty()` gate now has a source-verified mapping, but chosen sound playback/audio scheduling remains incomplete.

Retail provenance: the user-provided install's Training BSP, EXE, and Genesis DLL SHA-256 hashes match `evidence/original/` exactly. The typedef extraction command is:

```sh
python3 tools/extract_gbsp_typedefs.py evidence/original/training.bsp \
  --class RCS_FireRescue --output evidence/bsp-typedefs/training.json
```

Ghidra 12.0.4 evidence: read-only decompilation of `0044A330`; `00448D60` decompiled in a temporary analysis project after defining the vtable-referenced entry; `00448630` decompilation and `CallGraphPath.java 00448630 00498B30`. No original game process was used.
