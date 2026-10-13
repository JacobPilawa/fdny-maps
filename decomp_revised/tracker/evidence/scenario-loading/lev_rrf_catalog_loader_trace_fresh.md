# `.lev` / `.rrf` catalog and loader trace

## Provenance

Fresh read-only queries against the active Ghidra program `FDNYFirefighter.exe` (x86:LE:32, image base `0x00400000`), whose executable path is `inputs/original/binaries/FDNYFirefighter.exe`. SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`. Static analysis only; no original-game runtime was used. The direct-call/xref assembly excerpts are preserved separately.

## Loader boundaries and callers

- Subsystem startup `0042B4E0` calls `LoadResourceDefinitionsFile` (`0049B3B0`) with literal `data\\resdefs\\sound.rrf` (call at `0042B714`); it then opens `data\\resdefs\\thegame.lev` through the string/stream helper and calls `00423160` (call at `0042B761`). This is the only data xref to the literal `thegame.lev` in the executable.
- Menu constructor `0042DEC0`, itself called from `0042B4E0`, invokes the same RRF loader on literal `data\\resdefs\\menu.rrf` (`0042DF45`).
- The catalog parser `00423160` opens the constructor-supplied stream and reads tokens in a loop. Its failing-open path logs “Couldn't load level definitions file” with source `main\\LevelManager.cpp:0x57`.
- Per-level transition handler `0042AB00` (fresh evidence already present at `0042ab00_process_level_load_request_fresh.c`) passes the pending request's resource path at +8 directly to `0049B3B0`, then derives the BSP/EVS path from its map-name field.
- ScriptActor command handlers `00478150` and `004783D0` supply the fixed paths `data\\resdefs\\rescue.rrf` and `data\\resdefs\\hose.rrf` to `SetPendingLevelRequest`; those are queued request resource paths, not aliases in `thegame.lev`. The common pending-level consumer calls `0049B3B0` on the supplied path. The xrefs and decompilations distinguish these request paths from the two startup RRF loads above.

## Catalog record parse and field flow

Parser `00423160` consumes catalog tokens and builds 0x28-byte level records. `level:` starts a record; the next token is stored at +0 (the map key), and records receive a sequential ordinal at +0x18. `title:` stores +4, `rrf:` stores +8, `music:` stores +0xC, `time:` parses a float to +0x10 (with a scale multiply), and `mapcoords:` parses two comma-separated values into +0x20/+0x24. `menumusic:` updates a separate parser-owned/global string rather than a per-level field. The parser appends records to the selector collection as documented in [catalog collection owner resolution](catalog_collection_owner_resolution.md).

For `rrf:`, the value is concatenated to the literal prefix `data\\resdefs\\` and stored as the record resource path; no extension is appended by this parser. The selected-level request later passes that stored full path to the common RRF loader. For example, a catalog value `rescue.rrf` resolves statically to `data\\resdefs\\rescue.rrf`.

## RRF token/file parse

`LoadResourceDefinitionsFile` (`0049B3B0`) opens the exact supplied path using `rb`, then repeatedly calls tokenizer `0049A5F0`. It dispatches exact token strings `ROC_SOUND_BUFFER2D` and `ROC_SOUND_BUFFER3D` to the sound-buffer parser, `ROC_MODEL` to `0049ADE0`, `ROC_BITMAP` to `0049AEE0`, and `ROC_ALPHA_TEXTURE` to `0049AFE0`. A failed open returns -1; a non-record/end token result ends the loop with 0. No extension or directory rewrite occurs in this loader.

The tokenizer recognizes identifiers from alphabetic characters, digits, and underscore; it returns punctuation as token kinds and string contents between quotes. The loader dispatch comparisons are byte-for-byte comparisons. The catalog key/field checks in `00423160` are likewise exact byte comparisons (including the `level:`, `rrf:`, `title:`, `music:`, `time:`, and `mapcoords:` spellings). No case-folding or alias table is evidenced on these paths. This does not establish Windows filesystem case behavior or whether other, untraced subsystems accept aliases.

## Asset-name resolution

- **BSP:** selected catalog key `<key>` is combined with `data\\maps\\` and `.bsp` by the selected-level path helper chain; the resulting `data\\maps\\<key>.bsp` is used by the EVS dispatcher. The existing [level-to-EVS static trace](level_to_evs_static_trace.md) pins the subsequent extension conversion to `.evs` and parser mode.
- **EVS:** the path is based on that same catalog key and map directory. This report relies on the existing EVS trace and does not re-derive its parser/mode behavior.
- **Briefing image:** selector update `0042F280` passes the matched catalog record to `00422EF0`; the latter uses that record's first string (the key) to build `data\\maps\\<key>.bmp` and passes it to `004097F0`, which calls `FindFirstFileA`. If no file is found, it returns fixed fallback `data\\maps\\brief.bmp`. The fallback literal is xrefed from `00423036`. Thus the key has a per-map briefing bitmap candidate, with a generic fallback.
- **Resource definitions:** catalog `rrf:` value becomes `data\\resdefs\\<value>` as above; request producers may also supply the fixed `rescue.rrf` or `hose.rrf` path directly.

No map-key alias, case normalization, key-to-different-BSP mapping, or per-map briefing alias table was found in the inspected loader path.

## Evidence files

Fresh pseudocode and caller-site evidence are adjacent:
- `startup_resource_loader_calls_fresh.c` (`0042B4E0`)
- `00423160_level_catalog_parser_fresh.c` (`00423160`)
- `0049b3b0_resource_definition_loader_fresh.c` (`0049B3B0`)
- `0049a5f0_resource_file_tokenizer_fresh.c` (`0049A5F0`)
- `00478150_rescue_resource_request_caller_fresh.c` and `004783d0_hose_resource_request_caller_fresh.c`
- `00422ef0_map_briefing_path_fresh.c` (`00422EF0`)
- `0042f280_catalog_selection_briefing_caller_fresh.c` (`0042F280`)
- `004097f0_path_findfirstfile_probe_fresh.c` (`004097F0`)
- `catalog_loader_callsite_context_fresh.json`: Ghidra assembly context at the `.lev`, startup `.rrf`, menu `.rrf`, briefing fallback, and `rrf:` key xrefs.
- Existing same-hash transition/path evidence: `0042ab00_process_level_load_request_fresh.c`, `00422de0_selected_level_map_path.c`, `00403e40_level_path_suffix.c`, and `level_to_evs_static_trace.md`.

## Exact unknowns

- The loader's reached resource record parsers are identified by dispatch, but this pass did not trace every record's full resource creation/registration semantics.
- No live run verified which catalog rows load or whether every referenced map/resource file succeeds.
- The tokenizer's string handling and key dispatch establish case-sensitive parse behavior; filesystem case handling is outside this evidence.
- No alias resolution was found in the inspected path. This is a bounded negative result, not proof that no alias mechanism exists elsewhere.
