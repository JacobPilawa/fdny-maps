# Developer console command lead

The user-provided [`fdny_info` research repository](https://github.com/RichardFranksComputer/fdny_info) describes an F8 developer console. The F8 key binding itself has not been verified in the local binary. A read-only Ghidra 12.0.4 audit of the exact analyzed `FDNYFirefighter.exe` (SHA-256 `4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e`) confirms these command branches in dispatcher `0047A1F0`:

- `debugvictims` calls `00474F60`, toggles byte `DAT_00502AAC+0x6A9`, and logs `Victim debugging is ON/OFF`.
- `listmaps` calls `00479670`, enumerates `Data\\Maps\\*.bsp`, and logs `Available maps:` followed by names.
- `listvars` calls `00478B80`, enumerates the script-variable table at `DAT_00503044`, and formats each name/value pair for the game's logger.
- `runrescuebot` calls `00475020`; its effects have not been audited.

The `listvars` path could expose Training script-variable values through the rendered console/log. No file export or host-readable transport has been verified, so this remains a future capture lead rather than runtime evidence. No console command was entered during this audit. Raw local Ghidra exports are under `evidence/ghidra-refresh/console-audit-20261011/` and are excluded from Git by `decomp/.gitignore`.
