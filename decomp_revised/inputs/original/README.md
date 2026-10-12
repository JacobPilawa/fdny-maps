# Original input snapshot

This directory contains local copies used for the new workspace. The copied `Data/` tree contains 468 files (about 188 MB). `binaries/` contains the main game EXE, `Genesis.dll`, `D3DDrv.dll`, packed `fmod.dll`, and `FDNYFireFighter.cfg`. `manifest.csv` records a SHA-256 and byte size for every copied file (474 total, including this README).

Source directory: `<game-install>/`.

The main EXE hash is `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`, matching the existing Ghidra inventory. All included code files identify as 32-bit Windows PE/x86; `fmod.dll` is UPX-compressed and should be treated as a packed library until its image is recovered.

The input directory is selective rather than a full install image. It leaves out saved games, speedrun files, logs, and the local compatibility/mod layer (`D3D8.dll`, `D3D9.dll`, `DDraw.dll`, `dinput8.dll`, dgVoodoo files, and the FOV-fix ASI). Those items are not part of the first-party code baseline. If runtime behavior proves one of them is required for a specific capture, copy that exact file here and record its role in the manifest before using it.

The original Desktop files remain untouched. `Data/` and `binaries/` are local-only inputs and are excluded from Git; the manifest and notes stay versioned.

The snapshot was independently compared with the user's raw folder `<game-install>/` on 2026-10-12. All 346 files in the original Data snapshot and the five selected EXE/DLL/config files matched byte-for-byte (351 checked, zero differences). The ISO's InstallShield cabinet supplied the 122 additional missing audio files, bringing `Data/Snd/` to the full 127 WAV/MP3 entries listed by the installer; the five existing audio files matched the cabinet byte-for-byte. These source audio files and other `Data/` inputs remain local-only and are excluded from Git. See [the raw-copy comparison note](../reference/raw-copy/README.md).
