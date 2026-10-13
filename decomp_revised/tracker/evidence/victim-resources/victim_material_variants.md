# Victim actor material variation

Status: **the per-model material-index rotation is mapped; the mapping from an
ACT material entry to the rendered geometry surfaces remains to verify.**

## Evidence scope

Read-only Ghidra queries were run against the saved Ghidra 12.0.4 program for
`FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. No
program labels, comments, or database state were changed.

## Victim material selection

`FUN_0045B690` stores the resolved renderer model ID at `param_1 + 0x168`,
which is `param_1[0x5a]`. For model 14, that field first becomes 11, 12, or 13
according to the previously documented random branch. `FUN_0045D5B0`, whose
decompiler source metadata identifies `rescue\\VictimRescueEnt.cpp` line
`0x8f`, uses that same field as a key into a shared material-cursor tree.

The relevant behavior in `FUN_0045D5B0` is:

```c
next = lookup_material_cursor(model_id);
material_count = geActor_GetMaterialCount(victim_actor);
if (next < material_count) {
  if (next != 0) {
    geActor_GetMaterial(victim_actor, next, &material, &flags);
    geActor_SetMaterial(victim_actor, 0, material, flags);
  }
} else {
  next = 0;
}
cursor = resolve_or_insert_material_cursor(model_id);
cursor->next_material_index = next + 1;
```

`FUN_0045AA10` performs the model-ID lookup in the tree. `FUN_0045D450`
returns the matching cursor node or creates one through `FUN_0045D250`.
Because index zero is the actor's initial material, the first actor in a model
group retains its default slot-zero material. Later actors cycle through
indices 1 through `material_count - 1`, then wrap to zero. Each chosen material
is assigned to actor material slot zero. The cursor is shared by model ID, so
the sequence advances across victims using the same renderer model.

The decoded `boy`, `man`, and `woman` ACTs each contain three material slots.
Those are the renderer resources corresponding to IDs 11, 12, and 13. Thus
the model-14 branch uses these ACTs and varies their slot-zero appearance by
cycling the ACT material index. The `fatman` `vic_fatty.act` resource remains
the declared type's behavior/audio record; this constructor's renderer branch
does not select it. Exact geometry-surface use and visual parity still need
confirmation from the ACT surface/material binding or an original-game capture.

## Generated mapping

`victim_resource_crosswalk.csv` and `actor_asset_inventory.csv` now retain the
declared resource ACT, runtime renderer model IDs, corresponding RRF ACT paths,
and the source-mapped material-cycling rule as separate fields. For `fatman`,
the runtime path list is:

```text
11=data/acts/vic_boy.act
12=data/acts/vic_man.act
13=data/acts/vic_woman.act
```

No original-game material capture or native runtime build was performed for
this mapping.
