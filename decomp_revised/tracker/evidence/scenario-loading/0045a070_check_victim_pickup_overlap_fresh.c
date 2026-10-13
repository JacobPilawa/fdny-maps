/* Fresh Ghidra decompilation: CheckVictimPickupOverlap at 0x0045a070;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   The function was missing as a defined boundary in the clean Ghidra import. The previous
   project had three exports at this same address; this fresh boundary and decompilation
   match that source-derived control flow and connect it to the VictimRescue root vtable.
   */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall CheckVictimPickupOverlap(void *this,int *otherEntity)

{
  float *pfVar1;
  int iVar2;
  int *piVar3;
  undefined4 auStack_8 [2];

  piVar3 = otherEntity;
  if ((((*(int *)((int)this + 0x120) != 5) && (*(int *)((int)this + 0x120) != 6)) &&
      (otherEntity[6] == 1)) &&
     (pfVar1 = (float *)(**(code **)(*otherEntity + 0xc))(), _DAT_00502cbc = pfVar1,
     pfVar1 != (float *)0x0)) {
    otherEntity = *(int **)(*(int *)((int)this + 0x150) + 0x1c);
    iVar2 = FUN_00459b40((float)otherEntity,0.0);
    if (iVar2 < 1) {
      otherEntity = (int *)0x42700000;
    }
    if ((((pfVar1[4] + *pfVar1 <= (float)otherEntity + *(float *)((int)this + 0x4c)) &&
         (*(float *)((int)this + 0x4c) - (float)otherEntity <= pfVar1[7] + *pfVar1)) &&
        ((pfVar1[6] + pfVar1[2] <= (float)otherEntity + *(float *)((int)this + 0x54) &&
         ((*(float *)((int)this + 0x54) - (float)otherEntity <= pfVar1[9] + pfVar1[2] &&
          (pfVar1[5] + pfVar1[1] <= (float)otherEntity + *(float *)((int)this + 0x50))))))) &&
       (*(float *)((int)this + 0x50) - (float)otherEntity <= pfVar1[8] + pfVar1[1])) {
      piVar3 = (int *)FUN_0049d350(piVar3,auStack_8);
      SendMappedEntityMessage(*piVar3,piVar3[1],0x7532,(int)this);
    }
  }
  return;
}
