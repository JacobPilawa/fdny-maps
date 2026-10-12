# Tracked original Ghidra C

This directory preserves Ghidra's C-like decompilation for addresses currently
listed in `viewer-progress.json`. It is a comparison aid; it is not the game's
original source text and is not itself a proof of native parity.

## Provenance

- Program: `FDNYFirefighter.exe`
- SHA-256: `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`
- Ghidra: 12.0.4
- Exporter: `tools/ghidra/ExportFunctionC.java`
- Query mode: the preserved project was copied to `/private/tmp`; exports were
  made from that scratch copy so the preserved Ghidra database was not changed.
- Scope: 104 distinct addresses from the progress inventory. 93 were already
  function entries in the saved database; 11 missing boundaries were defined
  only in the scratch copy at addresses already present in the source/evidence
  inventory. The per-address headers and `index.json` distinguish these cases.

For a scratch-defined boundary, treat the C body as Ghidra's interpretation of
instructions beginning at that address, not as proof the original compiler's
function boundary was exactly there. Check the nearby disassembly and the
hash-pinned oracle/evidence linked from the progress page. Several entries are
known only as controlled P-code slices or callsites.

`index.json` maps every tracked address to its source file and boundary status.
The Training page links to the same files from each source-function row.
