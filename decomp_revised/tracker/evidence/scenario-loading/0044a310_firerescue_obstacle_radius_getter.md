# FireRescue radius field used by victim roam obstacle collection

Binary: `FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.

The victim obstacle builder `BuildVictimRoamObstacleList` (`0x0045C4D0`)
calls each eligible world entity's virtual slot `+0x0C`, then reads `+0x0C`
from the returned pointer as its radius.

The FireRescue constructor `0x0044A330` installs the primary vtable at
`0x004E1780` and sets object `+0x18` to 6. Its primary-vtable slot `+0x0C`
points to `0x0044A310`. The entry bytes are:

```text
0044A310  8D 41 7C    lea eax,[ecx+0x7C]
0044A313  C3          ret
```

Therefore the radius read is at FireRescue root `+0x7C + 0x0C = +0x88`.
The builder accepts 24 through 1200 and clamps out-of-range values to the
nearest endpoint after logging. Its center comes from a second FireRescue
virtual call and the returned transform's `+0x28/+0x2C/+0x30` coordinates.

The constructor passes entity UserData through `0x0049D160`, but the inspected
code does not prove which authored key populates root `+0x88`. The typed BSP
inventory records authored `H_Radius`, which the existing FireRescue host path
also uses. The map-neutral preview therefore uses `H_Radius` as an explicit
static proxy and reproduces the floor probe for entities with `L_FloorMe`;
runtime UserData mapping, fire movement, and script-driven radius changes
remain open.
