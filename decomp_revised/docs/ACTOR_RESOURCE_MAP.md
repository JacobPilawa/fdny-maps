# Rescue actor resource map

Status: **all defined victim ACT assets currently decode for inspection; the
unknown-type fallback and constructor model/material selection are
source-mapped; runtime loading and material-to-surface binding remain to
implement and validate. The inherited ACT decoder is near-identical to the
legacy map explorer parser, so its output is provisional until the serialized
format is independently checked against the retail Genesis.dll and raw ACT
inputs.**

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

`subway_b` contains one victim whose authored `D_Type` is `boy1`, which has no
exact `Victim:` entry. The runtime lookup is now resolved: on a miss it returns
the first entry from the loaded `victims.srf` array, and the loader preserves
the file's declaration order. The first entry is `boy` (model 11), which
`rescue.rrf` maps to `vic_boy.act`. Keep `boy1` as the authored map value while
resolving its runtime resource to `boy`; the generated crosswalk and actor
inventory record this fallback explicitly. See
[`actor_default_fallback.md`](../tracker/evidence/victim-resources/actor_default_fallback.md)
for the hash-pinned code evidence.

The `fatman` resource record declares model 14, but victim initialization
replaces that ID with `11 + random() % 3` before looking up and constructing
the renderer actor. Its runtime actor model is therefore selected from IDs
11–13. The original `fatman` resource record is still passed to the separate
victim behavior/resource initializer, so its authored sound and speech
configuration remains associated with the victim. The direct RRF mapping to
`vic_fatty.act` does not establish that this constructor renders that ACT; its
use elsewhere remains open. The generated inventories now keep the declared
resource ACT and runtime actor model selection in separate columns. See
[`fatman_model_selection.md`](../tracker/evidence/victim-resources/fatman_model_selection.md).

The renderer model IDs resolve through `rescue.rrf` to concrete ACT paths.
For `fatman`, the selected runtime paths are `vic_boy.act`, `vic_man.act`, and
`vic_woman.act`, while the original `fatman` record continues to supply its
victim behavior and audio configuration. The generated inventory records both
the declared resource ACT and the runtime renderer ACT paths.

`FUN_0045D5B0` also cycles a material index separately for each selected model
ID. It advances the cached index, wraps it when it reaches that ACT's material
count, and copies each selected material onto actor material slot 0. For the
three renderer models used by the model-14 branch, the decoded ACTs expose
three material slots each. This maps the variant-selection rule, while the
mapping from ACT material records to rendered surface indices still needs
confirmation. See
[`victim_material_variants.md`](../tracker/evidence/victim-resources/victim_material_variants.md).

## Reuse and implementation

The existing C++ actor-pose and animation code consumes decoded actor JSON and
is a useful base. The Training host currently loads only
`actors/vic_man.json`, requires exactly six motions, and initializes every
victim with model 12. The actor renderer also selects `vic_man` textures
directly. Those are Training-specific resource bindings, while the pose,
animation, and rescue state transitions can remain shared behind an actor
definition selected from `D_Type`.

The previous `FDNY_trackdown` work contains an ACT parser at
`decomp/tools/extract_act.py`; it was copied into this clean project as
[`tools/extract_act.py`](../tools/extract_act.py). Hash comparison shows that
copy is byte-identical to the archived file and 99.8% line-similar to
`map_explorer/source/tools/extract_act.py`. This is inherited asset-inspection
code, not an independently derived retail port. Its current output is useful
for locating candidate resources, while the parser/layout needs independent
retail validation. It reads nested Genesis VFS files, BODY geometry, motions,
and paletted GeBm textures. The new
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
`bitmapId` values. Ghidra maps per-model material-index cycling and the
slot-zero override, while the mapping from ACT material entries to rendered
geometry surfaces and bitmap-loading behavior still needs confirmation. The
compact inventory records each material slot, bitmap ID, and numbered
embedded bitmap entry for that follow-up.

Victim sound cues are also joined from `victims.srf` through `sound.rrf`. The
seven actor definitions reference 20 unique WAV paths. They were missing from
both Desktop game copies, but are present in the supplied InstallShield disc.
All 127 WAV/MP3 files under the disc's `Main/Data/Snd/` were extracted into the
ignored local `inputs/original/Data/Snd/` tree; the five preexisting audio files
matched byte-for-byte. The 20 referenced victim voice paths now resolve. The
versioned input manifest records every extracted file's size and SHA-256; the
retail audio is not checked in.

The unknown `boy1` type in `subway_b` is now mapped to the runtime's first-entry
fallback, `boy` (model 11 / `vic_boy.act`). Preserve `boy1` in the imported BSP
entity data and resolve it through the same first-entry rule as the original
resource manager. The model-14 path now records its runtime actor choice as
IDs 11–13; whether another game path uses `vic_fatty.act` remains open.
The next implementation step is a data-driven actor catalog and victim
renderer that selects the mapped ACT from each entity type instead of always
loading `vic_man`. Motion lists also differ by actor: the dog has three motions,
the fat victim four, and the human actors six. The runtime must resolve
semantic motion names from each ACT and the original victim behavior, rather
than assuming each actor has the Training man's six motion indices. Actor pose,
animation timing, and victim state transitions remain shared across every
rescue map. See [`fatman_model_selection.md`](../tracker/evidence/victim-resources/fatman_model_selection.md)
for the separate declared-resource and runtime-actor model behavior.
