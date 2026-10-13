# Unknown victim type fallback

Status: **resolved for the pinned executable and resource snapshot**.

## Result

The map-authored `D_Type=boy1` on `subway_b` resolves at runtime to the first
record in `Data/ResDefs/victims.srf`. The file's first record is `boy`, model
11, and `Data/ResDefs/rescue.rrf` maps model 11 to `data/acts/vic_boy.act`.
The source BSP value remains `boy1`; only resource resolution falls back.

## Executable evidence

The following functions were decompiled read-only with Ghidra 12.0.4 from
`FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
The isolated headless import used for this check was deleted after analysis;
the workspace's saved Ghidra project was not changed.

`FUN_0045B690` calls `FUN_0048DF10` with the resource object initialized from
`victims.srf`; on a type-name mismatch it logs that it is using the default
type, then resolves the model from the returned record. See the preserved
caller decompilation in [`0045B690_FUN_0045B690.c`](0045B690_FUN_0045B690.c).

In `FUN_0048DF10`, `FUN_0048DA10` returns zero when no name matches. The miss
path copies the name from array element zero if the array is nonempty:

```c
iVar2 = FUN_0048da10(this, piVar3);
if (iVar2 == 0) {
  if (*(int *)((int)this + 4) == 0) {
    FUN_00402790(param_1, "");
  }
  else {
    FUN_00401cb0(param_1, (undefined4 *)**(undefined4 **)this);
  }
}
```

`FUN_0048DA10` scans the array from index zero upward and compares each
record's name. It does not sort or rearrange the records.

`FUN_0048E740` loads the resource file sequentially. For each `Victim:` line it
constructs a record, finds the first empty array slot, grows the array if
needed, and stores the new record in that slot. The generic resize helper
`FUN_0043B6E0` uses `realloc` and zeroes new slots, preserving existing entries.
Thus the loaded array retains the declaration order in `victims.srf`.

## Data resolution

The first declaration in the pinned file is:

```text
Victim: boy
Model: 11
```

`rescue.rrf` maps model 11 to `data/acts/vic_boy.act`; its source hash and
decoded motion/texture inventory are recorded in
[`actor_asset_inventory.csv`](../../actor_asset_inventory.csv). The map-wide
crosswalk generator and actor exporter now model the first-entry fallback
explicitly and record both the authored type and resolved resource type.

The special model-14 replacement path in `FUN_0045B690` remains separate:
`fatman` may resolve through model 14 and then be replaced by a random model
from 11 through 13. Its final appearance selection still needs its own trace.
