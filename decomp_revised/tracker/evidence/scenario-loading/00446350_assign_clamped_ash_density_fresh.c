/*
 * Fresh Ghidra 12.0.4 decompilation
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 00446350 (AssignClampedAshDensity)
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall AssignClampedAshDensity(void *this,float param_1)

{
  float fVar1;

  fVar1 = _DAT_004dfc24;
  *(float *)((int)this + 0x54) = param_1;
  if (fVar1 < param_1) {
    *(undefined4 *)((int)this + 0x54) = 0x3f800000;
  }
  if (*(float *)((int)this + 0x54) < _DAT_004dc50c) {
    *(undefined4 *)((int)this + 0x54) = 0;
  }
  return;
}
