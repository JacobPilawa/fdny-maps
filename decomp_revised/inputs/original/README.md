# Original input snapshot

This directory identifies the local retail input snapshot used for analysis. The private working copy includes a 346-file `Data/` tree and selected EXE/DLL/config files. This public mirror includes only `manifest.csv` and this note; it does not distribute those game files. The manifest records SHA-256 and byte size for each input.

The main EXE hash is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, matching the existing Ghidra inventory. All included code files identify as 32-bit Windows PE/x86; `fmod.dll` is UPX-compressed and should be treated as a packed library until its image is recovered.

The input directory is selective rather than a full install image. It leaves out saved games, speedrun files, logs, and the local compatibility/mod layer (`D3D8.dll`, `D3D9.dll`, `DDraw.dll`, `dinput8.dll`, dgVoodoo files, and the FOV-fix ASI). Those items are not part of the first-party code baseline. If runtime behavior proves one of them is required for a specific capture, copy that exact file here and record its role in the manifest before using it.

The original installation remains untouched. `Data/` and `binaries/` are local-only inputs and are excluded from Git; the manifest and notes stay versioned.

The snapshot was independently compared with the user's raw folder `the separately supplied raw Desktop installation` on 2026-10-12. All 346 files in Data and the five selected EXE/DLL/config files matched byte-for-byte (351 checked, zero differences). See [the raw-copy comparison note](../reference/raw-copy/README.md).
