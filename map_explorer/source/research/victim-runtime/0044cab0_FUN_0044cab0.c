/* Ghidra decompilation; names/types are provisional.
 * Entry: 0044cab0
 * Function: FUN_0044cab0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __thiscall FUN_0044cab0(void *this,int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  int local_94;
  char *local_90;
  char *local_8c;
  undefined4 local_88;
  int iStack_7c;
  char *pcStack_78;
  char *pcStack_74;
  undefined4 uStack_70;
  undefined **ppuStack_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  undefined1 uStack_58;
  undefined1 auStack_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  int *local_4;
  
  local_4 = (int *)0xffffffff;
  puStack_8 = &LAB_004cfb86;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0x144) == 0) {
    uVar1 = 0;
  }
  else {
    iVar4 = *(int *)((int)this + 0x144) + -0xd4;
    ExceptionList = &local_c;
    uVar1 = FUN_00443610(iVar4);
    if ((char)uVar1 != '\0') {
      FUN_00443640(iVar4);
      if (*(int *)((int)this + 0x3568) < 5) {
        piVar3 = &local_94;
        for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        local_94 = 1;
        local_90 = "Array index out of bounds";
        local_8c = "..\\BluelAPI/Container/SArray.h";
        local_88 = 0x1cd;
        FUN_00422060(&local_94);
      }
      piVar3 = *(int **)(*(int *)((int)this + 0x3564) + 0x10);
      if (piVar3 == (int *)0x0) {
        puVar2 = operator_new(0xd54);
        if (puVar2 == (undefined4 *)0x0) {
          piVar3 = (int *)0x0;
        }
        else {
          local_4 = piVar3;
          piVar3 = FUN_004634f0(puVar2);
        }
        local_4 = (int *)0xffffffff;
        (**(code **)(*piVar3 + 8))(this);
        if (*(int *)((int)this + 0x3568) < 5) {
          piVar5 = &iStack_7c;
          for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
            *piVar5 = 0;
            piVar5 = piVar5 + 1;
          }
          iStack_7c = 1;
          pcStack_78 = "Array index out of bounds";
          pcStack_74 = "..\\BluelAPI/Container/SArray.h";
          uStack_70 = 0x1cd;
          FUN_00422060(&iStack_7c);
        }
        *(int **)(*(int *)((int)this + 0x3564) + 0x10) = piVar3;
        FUN_00482780(piVar3 + 0x31c);
        FUN_004571a0((void *)((int)this + 0x5f4),(int *)0x6,(undefined1 *)0x4);
      }
      else if ((float)piVar3[0x32c] * (float)piVar3[0x330] * (float)_DAT_004e1858 < (float)param_1)
      {
        FUN_00482780(piVar3 + 0x31c);
      }
      if ((piVar3[0x31a] == 3) || (piVar3[0x31a] == 2)) {
        ppuStack_64 = &PTR_FUN_004dc554;
        FUN_004a3118();
        pcStack_60 = "rescue\\PlayerRescueEnt.cpp";
        uStack_5c = 0x63a;
        uStack_58 = 0;
        local_4 = (int *)0x1;
        FUN_004a3a19(auStack_48,"Already have hose");
        FUN_00401360(&ppuStack_64,0);
        local_4 = (int *)0xffffffff;
        uVar1 = FUN_004012c0(&ppuStack_64);
      }
      else {
        FUN_00463370(piVar3,2);
        uVar1 = FUN_004a11a0((void *)((int)this + 0x49c),(undefined1 *)0x0);
      }
    }
    uVar1 = CONCAT31((int3)((uint)uVar1 >> 8),1);
  }
  ExceptionList = local_c;
  return uVar1;
}

