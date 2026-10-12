# Level `+0x10` input to Player deadline initialization

## Result

The level `+0x10` field is the parsed `time:` value from a record in `Data/ResDefs/thegame.lev`, converted to float and multiplied by 60. The Training record specifies `time: 500`, so the level record contains `30000.0f` at `+0x10`. Player initializer `0044D550` reads that same offset from the current level object held at application `+0x688`. It applies a further mode multiplier: `1.5` in game modes 0 and 1, `1.0` in other modes. Thus the initialized Player field is `45000.0f` in modes 0/1, and `30000.0f` otherwise. The code establishes the catalog label `time:` and ×60 conversion; this audit does not assign a wall-clock unit beyond those source facts.

## Loader and parser evidence

Read-only Ghidra decompilation of `0042B4E0` constructs the resource name `data\\resdefs\\thegame.lev` and calls catalog parser `00423160`. For each `level:` entry, `00423160` allocates a `0x28`-byte record and calls constructor `00422800`. The parser fills fields in that record: `title:` at `+0x04`, the `rrf:` branch at `+0x08`, and `time:` at `+0x10`. In the `time:` branch it parses the following token to float, stores it at `this+0x10`, and multiplies the field by `DAT_004DF224`; `DataValue.java` confirms that constant is `60.0f` (`0x42700000`). Constructor `00422800` initializes record `+0x10` to immediate `0x44960000` (`1200.0f`), the default when no `time:` branch overwrites it.

The installed Training catalog entry is:

```text
level: training
title: "Training"
rrf: "rescue.rrf"
music: ebtwist.mp3
time: 500
mapcoords: 52,43
```

The parser therefore writes `500.0f` then multiplies by `60.0f`, yielding exactly `30000.0f` at record `+0x10`. Loader `0042AB00` accepts the record pointer, accesses its fields, and assigns that same pointer to application `+0x688` at `0042ABC4` and `0042AC19`. Player initializer `0044D550` reads app `+0x688` plus `+0x10`, closing the catalog-to-Player input path.

`00471030` handles LEVEL/DIFFICULTY script variables, while `00471310` derives an EVS script path and dispatches script loading; neither is the catalog `time:` field writer. The writer is the `time:` branch in `00423160` with record defaulting in `00422800`.

## Available Training data

The installed catalog at `/Users/jacobpilawa/Applications/Sikarugir/FDNYFirefighterFOVFix.app/Contents/SharedSupport/prefix/drive_c/Program Files/FireFighter American Hero/Data/ResDefs/thegame.lev` is ASCII text with SHA-256 `ee6819873cd540e67613774d848506561e2861442b37ef301e27273805d558c6`; it is the source of Training `time: 500`. The available `evidence/original/training.bsp` (SHA-256 `32986212fa875764ca3d5c1630e54ad6315abaea70983203601192cec603a963`) and its decoded entity export `assets/training/entities.json` (SHA-256 `f4bb9740af5811d3ec71472c6ecc92bf5b207e7540e5f068520d4ad81ecb6d4e`) are separate map data, not the source of this catalog setting. `evidence/original/rescue.rrf` remains a resource table rather than the level metadata record.

## Provenance and limits

Queries used the preserved `FDNYFirefighter.exe` Ghidra program read-only with `-noanalysis`; executable SHA-256 is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Relevant outputs are retained under ignored `evidence/victim-ai-audit/level-deadline-authoring-native/`: `catalog-parser.log` includes `00423160` and its `time:` store; `catalog-parser-followup.log` includes `0042B4E0`, `0049A5F0`, and constant `60.0f`; `level-record-constructor.log` includes `00422800` and its default; `training-catalog-record.log` holds the exact Training lines; earlier `trace.log` records `0042AB00` and the app field assignment. This is a bounded source trace of the catalog parser, record constructor, loader, and Player consumer, not a whole-executable alias analysis.
