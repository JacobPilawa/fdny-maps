# Rescue actor resource map

Status: **all defined victim ACTs decode locally; generic runtime actor
selection remains open.**

## Recovered resource chain

The local `Data/ResDefs/victims.srf` file declares a `Victim` key and Genesis
model index for each rescue actor type. `Data/ResDefs/rescue.rrf` maps those
model indices to `.act` files. The raw ACT sources are preserved with the
other hash-pinned inputs under `inputs/original/Data/Acts/`.

The all-map join is in
[`victim_resource_crosswalk.csv`](../tracker/victim_resource_crosswalk.csv).
Six BSP-authored types join through both resource definitions to existing ACT
files: `boy`, `dog`, `fatman`, `girl`, `man`, and `woman`. Across the 17 BSPs,
the rescue entity records include 71 victims; actor choice is not globally
fixed to the Training `man` model. `herodown` is defined as model 30 and has an
ACT resource, but does not appear as a BSP `RCS_VictimRescue` type; its
relationship to the player-down path should be checked against the mapped
player code.

One authored type remains unresolved: `subway_b` contains one victim whose
`D_Type` is `boy1`, which is not a `Victim:` entry in `victims.srf`. The
`rescue.rrf` does map model 11 to `vic_boy.act`, but that does not prove `boy1`
aliases `boy`; retain the source value and trace the game's lookup/fallback
before using that candidate.

## Reuse and implementation

The existing C++ actor-pose and animation code consumes decoded actor JSON and
is a useful base. The Training host currently loads only
`actors/vic_man.json`, requires exactly six motions, and initializes every
victim with model 12. The actor renderer also selects `vic_man` textures
directly. Those are Training-specific resource bindings, while the pose,
animation, and rescue state transitions can remain shared behind an actor
definition selected from `D_Type`.

The previous `FDNY_trackdown` work contains a reusable ACT parser at
`decomp/tools/extract_act.py`; it was copied into this clean project as
[`tools/extract_act.py`](../tools/extract_act.py). The parser reads nested
Genesis VFS files, BODY geometry, motions, and paletted GeBm textures. The new
[`tools/export_victim_actors.py`](../tools/export_victim_actors.py) resolves
definitions through `victims.srf` and `rescue.rrf`, writes decoded JSON/OBJ/PNG
review assets under ignored `.local/actor_exports/`, and refreshes the compact
[`actor_asset_inventory.csv`](../tracker/actor_asset_inventory.csv). Retail
ACT inputs stay hash-pinned under `inputs/original/`; decoded assets are not
checked in.

The exporter successfully decoded all six types used by BSP victim entities
and the defined `herodown` resource: 7 ACTs, 32 motions, and 17 actor texture
images. All exported actor tracks use interpolation mode 1 for both rotation
and translation, which is within the current `ActorPose` mode handling. This
establishes that the existing pose evaluator's input shape is reusable; it is
not yet an original-runtime parity result. The material records retain opaque
`bitmapId` values, and the original mapping from those IDs to skin/material
selection still needs to be confirmed. The compact inventory records each
material slot, bitmap ID, and numbered embedded bitmap entry for follow-up.

Victim sound cues are also joined from `victims.srf` through `sound.rrf`. The
seven actor definitions reference 20 unique WAV paths. They were missing from
both Desktop game copies, but are present in the supplied InstallShield disc.
All 127 WAV/MP3 files under the disc's `Main/Data/Snd/` were extracted into the
ignored local `inputs/original/Data/Snd/` tree; the five preexisting audio files
matched byte-for-byte. The 20 referenced victim voice paths now resolve. The
versioned input manifest records every extracted file's size and SHA-256; the
retail audio is not checked in.

One authored type has no matching resource definition: `subway_b` contains one
victim whose `D_Type` is `boy1`, absent from `victims.srf`. A same-hash Ghidra
export of `FUN_0045B690` confirms the runtime logs unknown victim types and
continues with its resource manager's configured default type. The exact
default type/model returned for `boy1` remains open, so do not assume it aliases
`boy`. Preserve the authored `D_Type`, then reproduce the verified default
lookup once its configured target is identified. The selected evidence is in
[`victim-resources/`](../tracker/evidence/victim-resources/).
The next implementation step is a data-driven actor catalog and victim
renderer that selects the mapped ACT from each entity type instead of always
loading `vic_man`. Motion lists also differ by actor: the dog has three motions,
the fat victim four, and the human actors six. The runtime must resolve
semantic motion names from each ACT and the original victim behavior, rather
than assuming each actor has the Training man's six motion indices. Actor pose,
animation timing, and victim state transitions remain shared across every
rescue map. `FUN_0045B690` also special-cases resolved model ID 14 to a random
ID from 11 through 13; verify that resource/model behavior before porting the
`fatman` appearance path.
