/*
 * Fresh Ghidra 12.0.4 decompilation
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0044BC90 (SetPlayerAshDensity)
 */

undefined4 __thiscall SetPlayerAshDensity(void *this,float param_1)

{
  undefined4 uVar1;

  uVar1 = AssignClampedAshDensity((void *)((int)this + 0x55c),param_1);
  return CONCAT31((int3)((uint)uVar1 >> 8),1);
}
