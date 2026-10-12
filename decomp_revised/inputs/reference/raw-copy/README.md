# Raw retail code-file comparison copy

This note records a hash comparison against the raw Desktop folder:

`/Users/jacobpilawa/Desktop/FireFighter American Hero 2/`

The four code-bearing files used for analysis are pinned under `inputs/original/`; this note does not duplicate them. SHA-256 comparison shows that all four are byte-identical:

| File | SHA-256 |
|---|---|
| `FDNYFirefighter.exe` | `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e` |
| `Genesis.dll` | `5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643` |
| `fmod.dll` | `52fd7732e5bc9eeb07ecca843ea53ad73340337ce8ca0095994f131ab9b9bc2a` |
| `Drivers/D3DDrv.dll` | `7727baabe0cc941cf63e4088cf140dbc9389e0d451bd1ac30a053c2e7c9d49e7` |

The same four files inside the Sikarugir/Wine wrapper also match these hashes. On 2026-10-12, the complete 346-file `Data/` tree and these five selected EXE/DLL/config files (351 files total) were compared with `/Users/jacobpilawa/Desktop/FireFighter American Hero 2/`; there were zero hash differences or missing files. Use the canonical pinned copies under `inputs/original/` for static analysis; no duplicate raw asset copy is needed. `RegSetup.exe` and `unpack.exe` were not copied because they are not needed for the initial EXE/DLL map.

## Training asset comparison

The following source files in the same Desktop folder also match the pinned original snapshot byte-for-byte:

| File | SHA-256 |
|---|---|
| Data/Maps/training.evs | 0c01a1f8d88ef7a994be382d223fd5302bd29301496362613fe963332b336061 |
| Data/Maps/training.bsp | 32986212fa875764ca3d5c1630e54ad6315abaea70983203601192cec603a963 |
| Data/ResDefs/thegame.lev | ee6819873cd540e67613774d848506561e2861442b37ef301e27273805d558c6 |

The existing pinned copies are the analysis inputs; no second asset copy was needed.
