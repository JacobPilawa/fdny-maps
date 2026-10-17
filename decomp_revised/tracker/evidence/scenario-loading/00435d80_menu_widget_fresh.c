/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __fastcall FUN_00435d80(int param_1)

{
  if (*(int *)(param_1 + 4) != 0) {
    geEngine_DrawBitmap(*(undefined4 *)(DAT_00503a70 + 0x74),*(int *)(param_1 + 4),0,
                        *(undefined4 *)(*(int *)(param_1 + 0x28) + 0x274),
                        *(undefined4 *)(*(int *)(param_1 + 0x28) + 0x27c));
  }
  if (*(int *)(param_1 + 8) != 0) {
    geEngine_DrawBitmap(*(undefined4 *)(DAT_00503a70 + 0x74),*(int *)(param_1 + 8),0,
                        *(int *)(*(int *)(param_1 + 0x28) + 0x274) + 0xdc,
                        *(undefined4 *)(*(int *)(param_1 + 0x28) + 0x27c));
  }
  FUN_00435cb0(param_1);
  return;
}

 (GhidraScript)
