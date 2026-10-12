
/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
uint __thiscall FUN_0046ed70(void *this,int *param_1,int *param_2)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int *piVar6;
  uint *puVar7;
  int local_28;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;
  
  piVar2 = param_1;
  puStack_8 = &LAB_004d2440;
  local_c = ExceptionList;
  local_4 = 1;
  ExceptionList = &local_c;
  iVar3 = FUN_0046e210((void *)((int)this + 8),(byte *)(param_1 + 3));
  if ((iVar3 == 0) || (piVar1 = (int *)(iVar3 + 0xc), piVar1 == (int *)0x0)) {
    local_4 = local_4 & 0xffffff00;
    if ((piVar2 != &DAT_00501908) && (iVar3 = *piVar2, *piVar2 = iVar3 + -1, iVar3 + -1 < 1)) {
      FUN_004011c0(&param_1);
    }
    local_4 = 0xffffffff;
    if ((param_2 != &DAT_00501908) && (iVar3 = *param_2, *param_2 = iVar3 + -1, iVar3 + -1 < 1)) {
      param_2 = (int *)FUN_004011c0(&param_2);
    }
    ExceptionList = local_c;
    return (uint)param_2 & 0xffffff00;
  }
  uVar4 = *(uint *)(*piVar1 + 4);
  local_28 = 0;
  puVar7 = (uint *)param_2;
  if (0 < (int)uVar4) {
    iVar3 = 0x14;
    do {
      if ((iVar3 < 0x14) || ((int)uVar4 <= local_28)) {
        piVar6 = &local_24;
        for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
          *piVar6 = 0;
          piVar6 = piVar6 + 1;
        }
        local_24 = 1;
        local_20 = "Array index out of bounds";
        local_1c = "..\\BluelAPI/Container/SArray.h";
        local_18 = 0x66;
        FUN_00422060(&local_24);
        puVar7 = (uint *)param_2;
      }
      if (1 < *(int *)*piVar1) {
        FUN_00405d00(piVar1);
      }
      uVar4 = FUN_00404c80((void *)(*piVar1 + iVar3),(char *)(puVar7 + 3),0,'\x01');
      if (-1 < (int)uVar4) {
        local_4 = local_4 & 0xffffff00;
        if (piVar2 != &DAT_00501908) {
          uVar4 = *piVar2 - 1;
          *piVar2 = uVar4;
          if ((int)uVar4 < 1) {
            uVar4 = FUN_004011c0(&param_1);
          }
        }
        local_4 = 0xffffffff;
        if (puVar7 != &DAT_00501908) {
          uVar4 = *puVar7 - 1;
          *puVar7 = uVar4;
          if ((int)uVar4 < 1) {
            uVar4 = FUN_004011c0(&param_2);
          }
        }
        ExceptionList = local_c;
        return CONCAT31((int3)(uVar4 >> 8),1);
      }
      uVar4 = *(uint *)(*piVar1 + 4);
      local_28 = local_28 + 1;
      iVar3 = iVar3 + 4;
    } while (local_28 < (int)uVar4);
  }
  local_4 = local_4 & 0xffffff00;
  if (piVar2 != &DAT_00501908) {
    uVar4 = *piVar2 - 1;
    *piVar2 = uVar4;
    if ((int)uVar4 < 1) {
      uVar4 = FUN_004011c0(&param_1);
    }
  }
  local_4 = 0xffffffff;
  if (puVar7 != &DAT_00501908) {
    uVar4 = *puVar7 - 1;
    *puVar7 = uVar4;
    if ((int)uVar4 < 1) {
      uVar4 = FUN_004011c0(&param_2);
    }
  }
  ExceptionList = local_c;
  return uVar4 & 0xffffff00;
}

