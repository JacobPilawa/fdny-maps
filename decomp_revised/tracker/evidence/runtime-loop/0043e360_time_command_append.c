
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __thiscall FUN_0043e360(void *this,undefined4 param_1,int *param_2)

{
  void *pvVar1;
  void *extraout_EAX;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  undefined4 *puVar6;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004ceaf8;
  local_c = ExceptionList;
  iVar5 = 0;
  local_4 = 0;
  ExceptionList = &local_c;
  if (0 < *(int *)((int)this + 0x14c)) {
    ExceptionList = &local_c;
    do {
      if ((iVar5 < 0) || (*(int *)((int)this + 0x14c) <= iVar5)) {
        piVar4 = &local_24;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_24 = 1;
        local_20 = "Array index out of bounds";
        local_1c = "..\\BluelAPI/Container/SArray.h";
        local_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      if (**(char **)(*(int *)((int)this + 0x148) + iVar5 * 4) == '\0') {
        if (iVar5 != -1) goto LAB_0043e47f;
        break;
      }
      iVar5 = iVar5 + 1;
    } while (iVar5 < *(int *)((int)this + 0x14c));
  }
  pvVar1 = operator_new(0x10);
  iVar5 = 0;
  if (pvVar1 == (void *)0x0) {
    pvVar1 = (void *)0x0;
  }
  else {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    *(undefined4 **)((int)pvVar1 + 8) = &DAT_00501908;
  }
  piVar4 = (int *)((int)this + 0x148);
  if (0 < *(int *)((int)this + 0x14c)) {
    piVar3 = (int *)*piVar4;
    do {
      if (*piVar3 == 0) goto LAB_0043e47a;
      iVar5 = iVar5 + 1;
      piVar3 = piVar3 + 1;
    } while (iVar5 < *(int *)((int)this + 0x14c));
  }
  FUN_0043b6e0(piVar4,iVar5 + 1);
LAB_0043e47a:
  *(void **)(*piVar4 + iVar5 * 4) = pvVar1;
LAB_0043e47f:
  piVar4 = (int *)((int)this + 0x148);
  if ((iVar5 < 0) || (*(int *)((int)this + 0x14c) <= iVar5)) {
    piVar3 = &local_24;
    for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
      *piVar3 = 0;
      piVar3 = piVar3 + 1;
    }
    local_24 = 1;
    local_20 = "Array index out of bounds";
    local_1c = "..\\BluelAPI/Container/SArray.h";
    local_18 = 0x1cd;
    FUN_00422060(&local_24);
  }
  **(undefined1 **)(*piVar4 + iVar5 * 4) = 1;
  if ((iVar5 < 0) || (*(int *)((int)this + 0x14c) <= iVar5)) {
    piVar3 = &local_24;
    for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
      *piVar3 = 0;
      piVar3 = piVar3 + 1;
    }
    local_24 = 1;
    local_20 = "Array index out of bounds";
    local_1c = "..\\BluelAPI/Container/SArray.h";
    local_18 = 0x1cd;
    FUN_00422060(&local_24);
  }
  *(undefined4 *)(*(int *)(*piVar4 + iVar5 * 4) + 4) = param_1;
  if ((iVar5 < 0) || (*(int *)((int)this + 0x14c) <= iVar5)) {
    piVar3 = &local_24;
    for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
      *piVar3 = 0;
      piVar3 = piVar3 + 1;
    }
    local_24 = 1;
    local_20 = "Array index out of bounds";
    local_1c = "..\\BluelAPI/Container/SArray.h";
    local_18 = 0x1cd;
    FUN_00422060(&local_24);
  }
  iVar2 = *(int *)(*piVar4 + iVar5 * 4);
  piVar3 = *(int **)(iVar2 + 8);
  puVar6 = (undefined4 *)(iVar2 + 8);
  if (piVar3 != param_2) {
    if (piVar3 != &DAT_00501908) {
      *piVar3 = *piVar3 + -1;
      if (*(int *)*puVar6 < 1) {
        FUN_004011c0(puVar6);
      }
      else {
        *puVar6 = &DAT_00501908;
      }
    }
    *puVar6 = param_2;
    if (param_2 != &DAT_00501908) {
      *param_2 = *param_2 + 1;
    }
  }
  if ((iVar5 < 0) || (*(int *)((int)this + 0x14c) <= iVar5)) {
    piVar3 = &local_24;
    for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
      *piVar3 = 0;
      piVar3 = piVar3 + 1;
    }
    local_24 = 1;
    local_20 = "Array index out of bounds";
    local_1c = "..\\BluelAPI/Container/SArray.h";
    local_18 = 0x1cd;
    FUN_00422060(&local_24);
  }
  *(undefined4 *)(*(int *)(*piVar4 + iVar5 * 4) + 0xc) = *(undefined4 *)((int)this + 0x9c);
  local_4 = 0xffffffff;
  if (param_2 != &DAT_00501908) {
    this = (void *)(*param_2 + -1);
    *param_2 = (int)this;
    if ((int)this < 1) {
      if ((int)this < 0) {
        piVar4 = &local_24;
        for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_24 = 0;
        local_20 = "Invalid reference count";
        local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
        local_18 = 0xd0;
        FUN_00422060(&local_24);
      }
      _free(param_2);
      this = extraout_EAX;
    }
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)this >> 8),1);
}

