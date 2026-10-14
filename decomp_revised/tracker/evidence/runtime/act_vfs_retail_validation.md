# ACT/VF00 parser check against retail assets

## Pinned inputs and result

- Retail `Genesis.dll`: SHA-256 `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643`, matching both `inputs/original/binaries/Genesis.dll` and `inputs/reference/raw-copy/Genesis.dll`.
- Raw ACT inputs: `inputs/original/Data/Acts/*.act` (33 files). SHA-256 values are in `inputs/original/manifest.csv`.
- `tools/extract_act.py` parsed every ACT input without an exception. Every outer file has a single VF00 root with `Motions`, `Body`, and `Header`; each embedded Body independently parses as VF00 and yields `Bitmaps` and `Geometry`. Every BODY 0xF1 read ends exactly at the geometry buffer length (no unconsumed tail). Counts vary by asset and are not used as structural assumptions.

Representative retail input, `vic_boy.act` (292,483 bytes; SHA-256 `b5e486263581dff62f1624ffa34bd436403a088564427d8efba87b29324e2516`):

| Structure | Signature / version | Directory | Data length / end | Entries |
|---|---|---:|---:|---|
| Outer VFS | `VF00` / 0 | `0x474ee` | `0x474ee` / `0x47683` | `Motions` (directory), `Body` (224,419 bytes at `0x28`), `Header` (16 bytes at `0x18`) |
| Nested Body VFS | `VF00` / 0 | `0x36ba7` | `0x36ba7` / `0x36ca3` | `Bitmaps` (directory), `Geometry` (24,427 bytes at `0x18`) |
| Geometry | `BODY` / `0xF1` | — | 24,427 bytes | 386 vertices, 564 LOD-0 faces, 26 bones, 3 materials |

The outer header is 24 bytes: signature at `+0`, version WORD at `+4`, two-byte compiler padding at `+6`, dispersed LONG at `+8`, directory offset at `+12`, aggregate data length at `+16`, and end position at `+20`. In this file the padding bytes happen to be `91 00`; they are not a semantic header-size field. The ACT container header starts at `0x18` (`ACTR`, followed by version `0xF1`). The nested body begins at `0x28`. The Body directory's serialized offset/size fields are relative to the embedded VFS, as represented by `Vfs.read`.

## Layout comparison and parser fields

The parser's VF00 signature/version, dispersed flag, directory offset, data length, and end-position checks align with the 32-bit retail layout seen in the copied asset bytes. The ignored WORD is ABI padding before the 32-bit `geBoolean` dispersed field, not part of the directory. DT01 begins at the header directory offset and consists of signature plus declared byte size; its node stream has 32-bit list markers (`0` node, `-1` terminated), NUL-inclusive name length/name, 8-byte `geVFile_Time`, 32-bit attributes, file size, file data offset, hint length/hint bytes, then recursive child and sibling chains. `DIRECTORY == 2` matches the directory entries observed in these files. `Body` and `Geometry` payload offsets are honored as VFS-relative file offsets.

The parser's field sequence matches the public Genesis3D 1.1-derived reference implementation in `VFile/fsvfs.c` (`VFSFileHeader`, `FSVFS_OpenNewSystem`, `FSVFS_Read`) and `VFile/dirtree.c` (`WriteTree`, `ReadTree`, `DirTree_CreateFromFile`). Those files are format cross-checks only; their lineage does not establish identity with FDNY's DLL. The 33 successful complete parses independently check that the proposed fields and offsets fit the retail asset bytes, including recursive directory chains and embedded-file boundaries.

## Retail Ghidra trace and limits

The copied DLL was loaded read-only as Ghidra program **`Genesis.dll`**, image base `0x10000000`; its executable path is `inputs/original/binaries/Genesis.dll` and SHA-256 is the hash above. Auto-analysis produced 1,440 functions. The project path recommended for persistent work, `analysis/ghidra/FDNYPort.gpr`, could not be opened by the headless MCP service in this session; this analysis was therefore transient and was not saved into a project. No Ghidra labels/comments were changed. The hash-pinned inventory `tracker/functions.csv` records these retail function entries:

- `geVFile_OpenNewSystem`: `0x1002F360`, 294 bytes.
- `geVFile_Open`: `0x1002EEC0`, 250 bytes.
- `geVFile_Read`: `0x1002F160`, 31 bytes.
- `geVFile_Seek`: `0x1002F1A0`, 19 bytes.
- `geVFile_CreateFinder`: `0x1002F2C0`, 67 bytes.
- `geVFile_FinderGetNextFile`: `0x1002F330`, 8 bytes.
- `geVFile_FinderGetProperties`: `0x1002F340`, 8 bytes.

Retail decompilation/disassembly confirms the exported API is a dispatch layer. `geVFile_OpenNewSystem` (`0x1002F360`) validates/initializes the registered system table, takes the selected system API pointer, and calls its slot `+0x10`; when asked for a virtual FS with no parent (`FS == NULL`, type `3`), it first opens a DOS file (type `1`) and clears the directory flag for that base open. `geVFile_Open` (`0x1002EEC0`) checks the name through system slot `+0x24`, then invokes open slot `+0x18` with the flags. `geVFile_Read` (`0x1002F160`) dispatches to slot `+0x34` with the buffer/count; its zero-count case returns true. `geVFile_Seek` (`0x1002F1A0`) dispatches to slot `+0x3c`. Finder creation (`0x1002F2C0`) calls the system's finder slot `+0`; next-file and properties wrappers (`0x1002F330`, `0x1002F340`) tail-dispatch to finder slots `+4` and `+8`. This confirms retail API dispatch and caller argument flow, but the VFS filesystem implementation behind those table slots is separate code and is not yet identified here.

The retail trace does **not** yet prove the VFS system's internal header/directory traversal or the exact byte-level `Read` implementation instruction-by-instruction. Those layouts are checked against retail data and cross-checked with public Genesis3D 1.1-derived `FSVFS`/`DirTree` code, not attributed to the shipped DLL. In particular, confirm whether that concrete filesystem read method applies a VFS mask/XOR; the retail ACT payloads tested here parse without such a transform.

Additional unknowns: the empirical all-asset pass is not a malformed-file/fuzzing assessment; arbitrary `Header` semantics beyond the observed 16-byte ACTR record are not decoded here; and no current DLL evidence ties the code to the public source revision. No parser or dashboard code was changed.
