/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00435840(int param_1)

{
  float fVar1;
  float fVar2;
  
  FUN_0047df10(param_1 + 0x144);
  fVar2 = _DAT_004dc50c;
  fVar1 = DAT_00502b04 * *(float *)(param_1 + 0x38c) + *(float *)(param_1 + 0x390);
  *(float *)(param_1 + 0x390) = fVar1;
  if (fVar2 < fVar1) {
    if (_DAT_004dfc24 < fVar1) {
      *(undefined4 *)(param_1 + 0x390) = 0x3f800000;
    }
    FUN_0047dd30((void *)(param_1 + 0x268),*(float *)(param_1 + 0x390));
    *(undefined4 *)(param_1 + 0x374) = 0x42c80000;
    FUN_0047df10(param_1 + 0x268);
    FUN_0047df10(param_1 + 0x20);
    return;
  }
  *(undefined4 *)(param_1 + 0x390) = 0;
  FUN_0047df10(param_1 + 0x20);
  return;
}

 (GhidraScript)
