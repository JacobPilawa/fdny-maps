/* Fresh Ghidra decompilation: DispatchVictimRescueScriptCommand at 0x0045cdb0; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Common ScriptActor handling runs first; this class-specific fallback handles playanim,
   forcespawnspot, roam, and norescuebot. The playanim helper logs that the command is
   unimplemented. Authored numeric-only script fields remain unresolved. */

undefined4 __thiscall DispatchVictimRescueScriptCommand(void *this,int *param_1)

{
  int *this_00;
  char cVar1;
  int iVar2;
  undefined4 *puVar3;
  undefined4 uVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int *piStack_1c;
  undefined1 auStack_18 [4];
  void *local_14;
  undefined1 *puStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  int iStack_4;

  this_00 = param_1;
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d1040;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  local_14 = this;
  iVar2 = FUN_0047a1f0(this,param_1);
  if ((char)iVar2 == '\0') {
    puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar2 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"playanim");
    cVar1 = '\x01' - (iVar2 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 != '\0') {
      iVar2 = this_00[7];
      iVar6 = 0;
      if (0 < iVar2) {
        do {
          FUN_00473d20(this_00,&param_1,iVar6,0);
          iStack_4 = 0;
          FUN_00473d20(this_00,&piStack_1c,iVar6,1);
          iStack_4._0_1_ = 1;
          cVar1 = FUN_0040cd50(&piStack_1c,auStack_18);
          iStack_4 = (uint)iStack_4._1_3_ << 8;
          if (cVar1 == '\0') {
            if (piStack_1c != &DAT_00501908) {
              *piStack_1c = *piStack_1c + -1;
              if (*piStack_1c < 1) {
                FUN_004011c0(&piStack_1c);
              }
              else {
                piStack_1c = &DAT_00501908;
              }
            }
          }
          else {
            if (piStack_1c != &DAT_00501908) {
              *piStack_1c = *piStack_1c + -1;
              if (*piStack_1c < 1) {
                FUN_004011c0(&piStack_1c);
              }
              else {
                piStack_1c = &DAT_00501908;
              }
            }
            puStack_10 = &stack0xffffffcc;
            if (DAT_00501918 == '\0') {
              DAT_00501918 = '\x01';
              DAT_00501908 = 2;
              DAT_0050190c = 0;
              _DAT_00501910 = 1;
              DAT_00501914 = 0;
            }
            piVar7 = &DAT_00501908;
            if ((param_1 != &DAT_00501908) && (piVar7 = param_1, param_1 != &DAT_00501908)) {
              *param_1 = *param_1 + 1;
            }
            FUN_0045b3b0(piVar7);
          }
          iStack_4 = 0xffffffff;
          if (param_1 != &DAT_00501908) {
            *param_1 = *param_1 + -1;
            if (*param_1 < 1) {
              FUN_004011c0(&param_1);
            }
            else {
              param_1 = &DAT_00501908;
            }
          }
          iVar2 = this_00[7];
          iVar6 = iVar6 + 1;
        } while (iVar6 < iVar2);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar2 >> 8),1);
    }
    puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar2 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"forcespawnspot");
    cVar1 = '\x01' - (iVar2 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 != '\0') {
      iVar2 = this_00[7];
      iVar6 = 0;
      if (0 < iVar2) {
        do {
          FUN_00473d20(this_00,&param_1,iVar6,0);
          iStack_4 = 2;
          cVar1 = FUN_0040cd50(&param_1,&local_14);
          iStack_4 = 0xffffffff;
          if (cVar1 == '\0') {
            if (param_1 != &DAT_00501908) {
              *param_1 = *param_1 + -1;
              if (*param_1 < 1) {
                FUN_004011c0(&param_1);
              }
              else {
                param_1 = &DAT_00501908;
              }
            }
          }
          else {
            if (param_1 != &DAT_00501908) {
              *param_1 = *param_1 + -1;
              if (*param_1 < 1) {
                FUN_004011c0(&param_1);
              }
              else {
                param_1 = &DAT_00501908;
              }
            }
            ForceVictimSpawnSpot((void *)((int)this + -0xd4),(int)local_14);
          }
          iVar2 = this_00[7];
          iVar6 = iVar6 + 1;
        } while (iVar6 < iVar2);
      }
      ExceptionList = pvStack_c;
      return CONCAT31((int3)((uint)iVar2 >> 8),1);
    }
    puVar3 = (undefined4 *)FUN_0040c330(this_00,&param_1);
    iVar2 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"roam");
    cVar1 = '\x01' - (iVar2 != 0);
    param_1 = (int *)CONCAT31(param_1._1_3_,cVar1);
    if (cVar1 == '\0') {
      puVar3 = (undefined4 *)FUN_0040c330(this_00,&puStack_10);
      uVar5 = __stricmp((char *)(*(int *)*puVar3 + 0xc),"norescuebot");
      if (uVar5 == 0) {
        iVar2 = 0;
        if (0 < this_00[7]) {
          iVar2 = 0;
          do {
            *(undefined1 *)((int)this + 0x129) = 1;
            iVar2 = iVar2 + 1;
          } while (iVar2 < this_00[7]);
        }
        ExceptionList = pvStack_c;
        return CONCAT31((int3)((uint)iVar2 >> 8),1);
      }
      ExceptionList = pvStack_c;
      return uVar5 & 0xffffff00;
    }
    iVar2 = this_00[7];
    iVar6 = 0;
    if (0 < iVar2) {
      do {
        FUN_00473d20(this_00,&param_1,iVar6,0);
        uVar4 = ParseFloatFromString(&param_1,&local_14);
        iStack_4 = 0xffffffff;
        if ((char)uVar4 == '\0') {
          if (param_1 != &DAT_00501908) {
            *param_1 = *param_1 + -1;
            if (*param_1 < 1) {
              FUN_004011c0(&param_1);
            }
            else {
              param_1 = &DAT_00501908;
            }
          }
        }
        else {
          if (param_1 != &DAT_00501908) {
            *param_1 = *param_1 + -1;
            if (*param_1 < 1) {
              FUN_004011c0(&param_1);
            }
            else {
              param_1 = &DAT_00501908;
            }
          }
          StartVictimRoam((void *)((int)this + -0xd4),(int)local_14);
        }
        iVar2 = this_00[7];
        iVar6 = iVar6 + 1;
      } while (iVar6 < iVar2);
    }
  }
  ExceptionList = pvStack_c;
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}
