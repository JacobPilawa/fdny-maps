/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
void __fastcall FUN_0049e2c0(int param_1)

{
  int *piVar1;
  
  if (*(char *)(param_1 + 0x28) == '\0') {
    piVar1 = (int *)(param_1 + 0x2c);
    *piVar1 = *piVar1 + -1;
    if (*piVar1 == 0) {
      FUN_00471310(*(char **)(param_1 + 0x24),'\0');
      *(undefined1 *)(param_1 + 0x28) = 1;
      *(undefined4 *)(DAT_00502aac + 0x6f0) = 3;
    }
  }
  return;
}

