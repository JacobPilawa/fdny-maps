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
unavailable model causes victim creation to be rejected. The default
type/model are resolved in [`actor_default_fallback.md`](actor_default_fallback.md).

The first-entry fallback is now resolved by tracing the resource catalog loader
and lookup helpers in the same hash-pinned executable: the `Victim:` entries
are appended in file order, and the first `victims.srf` entry is `boy` (model
11, `vic_boy.act`). Thus `subway_b`'s authored `boy1` value resolves to the
`boy` resource at runtime. See
[`actor_default_fallback.md`](actor_default_fallback.md) for the evidence.

The function also replaces resolved model ID 14 with `11 + random() % 3`
before constructing the renderer actor. The selected renderer model and the
original victim behavior/resource definition follow separate paths; see
[`fatman_model_selection.md`](fatman_model_selection.md). The direct
`vic_fatty.act` RRF mapping therefore does not by itself prove that this
constructor renders that ACT.
