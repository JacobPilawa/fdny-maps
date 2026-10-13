# Victim resource lookup evidence

`0045B690_FUN_0045B690.c` is a Ghidra C-like decompilation of
`FDNYFirefighter.exe` at entry `0x0045B690`. Its source archive records the
same SHA-256 as the current pinned EXE:
`4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`.
Ghidra version 12.0.4; the source export and hash provenance are preserved in
the old project's `decomp/evidence/ghidra-original-c/README.md`. This is
decompiler output, not recovered original source code.

The function reads the authored victim type, requests its resource definition,
and compares the requested string with the returned definition. On mismatch it
logs that the type does not exist and that it is “Using default type” before
continuing with the returned definition. It then resolves a model ID; an
unavailable model causes victim creation to be rejected. The exact default
type name/model selected by the resource manager remains to be traced.

The function also has a special branch for resolved model ID 14 that replaces
it with a random ID in 11–13 before checking/creating the model. Preserve this
as a separate behavior question when binding `fatman`; do not infer the
resulting actor appearance from the `victims.srf` row alone.
