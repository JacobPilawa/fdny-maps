# `fatman` runtime actor model selection

Status: **the victim constructor's model-ID rewrite is resolved; use of the
direct `vic_fatty.act` mapping elsewhere remains open**.

## Evidence scope

The functions below were queried read-only from the Ghidra 12.0.4 project for
`FDNYFirefighter.exe`, SHA-256
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
No names or data were changed in the saved Ghidra project.

## Victim constructor

`FUN_0045B690` first resolves the victim's declared `Victim:` record and reads
its model ID. When the ID is 14, it replaces the actor's model field at
`param_1 + 0x168`:

```c
if (iVar4 == 0xe) {
  uVar6 = FUN_00404b30(3);
  *(uint *)((int)param_1 + 0x168) = uVar6 + 0xb;
}
```

`FUN_00404B30(n)` returns the pseudorandom value modulo `n`; therefore this
branch selects model ID `11 + random() % 3`, with possible IDs 11, 12, or 13.
The evidence establishes the range, not equal probability.

The selected ID then flows through the model-existence check at
`FUN_00493CE0` and into `FUN_00497E90`. That routine calls `FUN_00494500`,
which looks up the supplied ID in the loaded renderer-model table through
`FUN_004934E0`, then constructs the actor from the matching model record. So
the victim constructor's renderer actor uses the selected 11–13 model path.

## Separate victim resource record

The original resource record pointer is retained after the model field is
rewritten. Later in `FUN_0045B690`, the function calls `FUN_0048E330` with that
original record and the victim position. `FUN_0048E330` looks up the record by
its original type name and constructs a separate per-victim object through
`FUN_0048E080`. That constructor reads the record's Beckon and speech-related
collections. Thus `fatman` remains the victim behavior/audio definition while
the renderer actor model is selected from IDs 11–13.

`victims.srf` declares `fatman` as model 14, and `rescue.rrf` directly maps
model 14 to `data/acts/vic_fatty.act`. Preserve that source mapping in the
asset inventory, but keep it distinct from the IDs used by this victim
constructor. Whether another code path actually instantiates
`vic_fatty.act` remains open.
