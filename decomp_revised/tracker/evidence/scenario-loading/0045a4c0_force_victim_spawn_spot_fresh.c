/* Fresh Ghidra decompilation: ForceVictimSpawnSpot at 0x0045a4c0; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* For a valid integer index, reads a previously registered spawn-spot record from the victim's
   +0x200 list, copies its position into the actor location, adjusts actor height from the spot
   radius, and returns success. */

undefined4 __thiscall ForceVictimSpawnSpot(void *this,int param_1)

{
  int *this_00;
  undefined4 *puVar1;
  uint in_EAX;
  undefined4 uVar2;
  undefined1 local_44 [68];

  if ((-1 < param_1) && (param_1 < *(int *)((int)this + 0x204))) {
    FUN_0042d350((void *)((int)this + 0x24),local_44);
    this_00 = (int *)((int)this + 0x200);
    FUN_0048d490(this_00,param_1);
    puVar1 = *(undefined4 **)(*this_00 + param_1 * 4);
    *(undefined4 *)((int)this + 0x4c) = *puVar1;
    *(undefined4 *)((int)this + 0x50) = puVar1[1];
    *(undefined4 *)((int)this + 0x54) = puVar1[2];
    FUN_0048d490(this_00,param_1);
    uVar2 = FUN_00496c30((void *)((int)this + 0x24),
                         *(float *)(*(int *)(*this_00 + param_1 * 4) + 0xc) * _DAT_004e1698 +
                         _DAT_004e1cbc);
    return CONCAT31((int3)((uint)uVar2 >> 8),1);
  }
  return in_EAX & 0xffffff00;
}
