# Confirmed Training findings

This notebook separates observations from reconstruction choices. Addresses are
virtual addresses in the shipped `FDNYFirefighter.exe` (image base `0x00400000`).

## Asset and level format

- The game uses Genesis3D and the Training map is a version-15 GBSP.
- GBSP chunks decode to 6,905 vertices, 6,389 faces, 89 brush models, 29 embedded
  paletted textures, and 262 serialized entities.
- The level contains 25 `RCS_BoundsTrigger` records and five authored zones:
  crouching/jumping, movement/navigation, chopping/throwing, fire extinguishing,
  and survivor extraction.
- The map contains seven fires, two victims, four alternate victim spots, the
  hose, two valves, an extinguisher stand, breakable objects, and liftable boxes.

## Victim spawning

- `VictimRescueEnt::SpawnAtSpot` is at `0x0045D5B0`.
- It chooses an integer candidate index, copies the chosen spot into victim
  position offsets `0x4c`, `0x50`, and `0x54`, derives heading from
  `spotHeading * 30 + 180`, and calls `SnapToFloor` at `0x0049DA00`.
- `SnapToFloor` traces 1,000 units downward and places the victim one unit above
  the collision point.
- `victim1` candidates: `(1104,-60,-4976)`, `(1300,-120,-4952)`,
  `(1472,-120,-4972)`.
- `victim2` candidates: `(1656,-120,-4968)`, `(1820,-120,-4960)`,
  `(1972,-120,-4960)`.
- The random source at `0x00404930..0x00404B1F` is the older 624-word MT19937
  implementation. It lazily uses seed `4357`, expands state with multiplier
  `69069`, applies the standard MT twist/temper, and constructs a `[0,1)` float
  from the low 23 output bits. No direct call or stored function pointer in the
  executable reseeds it.
- Spot selection scales that float over `[0,count-1]` and converts it with x87
  round-to-nearest-even. For three Training spots this gives an approximately
  `1:2:1` distribution, not a uniform integer distribution. The build now
  reproduces that exact generator and exposes its seed/selected indices.
- The generator is global. The exact RNG draw ordinal at a fresh Training victim
  construction is not yet proven because other startup systems may consume it;
  default-seed choices `1,1` are the direct first two draws, not yet a claim
  about the full retail startup call sequence.
- `VictimRescueEnt` builds a `(-24,-24,-24)..(24,24,24)` collision hull.
  `SnapToFloor` first adds 48 to authored Y, sweeps that hull 1,000 internal
  units downward, copies the hit position, then adds one unit to Y. Original
  instrumentation logs show all six Training candidates settling at Y
  `-230.9`; the Training build now reproduces those coordinates from the BSP.

## Player and fire observations

- The player constructor at `0x0044D550` creates bounds with minimum
  `(-30,-30,-30)` and maximum `(30,85,30)`.
- Executable floats at `0x004E1824` and `0x004E1828` are 70 and 10, the observed
  standing/crouching Y offsets. The jump impulse at `0x004E1710` is 400.
- The player spawn entity is `(-1268,-172,4692)`, heading value 3, and its first
  level script is `equip axe`.
- `PlayerRescueEnt::Think` is at `0x00452570`. The standing/crouched caps are
  400/260 units per second; spectator is 800. Forward input is 1.0 and strafe
  input is 0.75 before normalization, so diagonal movement is not faster.
- Ground/climb velocity uses a 0.2 damping fraction and airborne velocity uses
  0.95, both referenced to 27 Hz. Ground drag is `(1000,0,1000)`, air drag is
  zero, and climb/spectator drag is `(1000,1000,1000)`.
- Gravity is `-1000`, jump adds `+400`, and jump has a 500 ms cooldown. The
  integrator at `0x0049DBC0` first updates velocity and then computes
  `delta = updatedVelocity*dt + 0.5*acceleration*dt^2`. This
  The retail clock oracle supplies the actual 17 ms minimum-step timing and the
  integrator oracle matches the original per-step trajectory; the synthetic
  60 Hz apex is not used as a parity claim.
- The retail clock uses integer milliseconds, waits until elapsed time is
  strictly greater than 1/60 second (at least 17 ms), and clamps long frames
  to 0.1 second. Update and rendering share this paced loop in both versions;
  browser timer scheduling still needs runtime comparison.
- Mouse look is linear and frame-rate independent: each accumulated DirectInput
  count is multiplied by the shipped `MouseSens=0.10` degrees, with no curve or
  dt factor. Pitch is limited to 20/40/70 degrees on easy/medium/hard, with a
  minimum 45-degree range while climbing or spectating.
- Collision is a swept AABB over the original GBSP convex leaf sides with
  contents mask `0x43`. The active player callback attempts the original
  16-unit step, then projects velocity and acceleration along the contact
  plane, retrying up to five contacts before its fallback.
- A 1,000-sweep fixture executes the shipped `Genesis.dll` collision export on
  Training data. All 611 fresh contacts reproduce its point and normal within
  `7.45e-9` original unit; 610 points are bit identical. The DLL can return
  retained global contact data for some start/exit-solid queries, so those are
  tagged separately.
- `FireRescueEnt::Spray` at `0x00448970` subtracts
  `dt/sprayTime * (1-(1-influence)^2)` from normalized health and douses on a
  terminal particle when health is strictly below `0.1`. Training's zero-time
  small fires therefore douse on their first positive hit; large fires author
  four seconds and cross the threshold before health reaches zero.

## Actor assets and original inputs

- Genesis3D `.act` files are nested `VF00` virtual filesystems containing a
  `BODY 0xF1` geometry record, `GeBm` material bitmaps, and motion records.
- Retail `axe.act` decodes to 59 skinned vertices, 110 highest-LOD triangles,
  three bones, one material, and one 256×256 paletted texture.
- Its sole named motion is `swing`, with three bone paths and an original time
  extent of `0..2.99996998`. Translation keys use Genesis Hermite interpolation
  and rotation keys use quaternion slerp; both are now decoded and sampled.
- Training type `man` resolves to `vic_man.act`: 303 vertices, 456 triangles,
  26 bones, three materials, and three 256×256 textures. The victim creation
  path at `0x0045B690` passes `(2,2,2)` to the actor scale function.
- `vic_man.act` contains the named clips `onback`, `sit`, `kneel`, `limp`,
  `slowwalk`, and `sneak`. Their state-selection logic remains to be recovered.
- `Data/ResDefs/input.rrf` maps the original arrow-key scan codes to movement,
  left Ctrl to `fire`, key 1 to axe selection, and key 2 to extinguisher
  selection. Mouse steering and mouse-button actions are handled separately.

## Provenance

- File-format implementation is checked against the public Reality Factory
  Genesis3D source (`World/Gbspfile.h`, `Entities/Entities.c`, and
  `World/Surface.c`).
- Richard Franks' `fdny_info` notes and victim-spawn ASI were used as leads. The
  spawn positions and object offsets above were independently checked against
  the local map and executable decompilation.
- Ghidra output is in `analysis/decompiled`; names and inferred types there are
  provisional until labeled.
