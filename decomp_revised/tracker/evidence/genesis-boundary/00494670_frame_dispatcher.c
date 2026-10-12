
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00494670(void *this,undefined *param_1)

{
  uint uVar1;
  void *this_00;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  int **ppiVar8;
  int *local_60;
  int iStack_5c;
  int *local_58;
  undefined1 *local_54;
  uint local_50;
  undefined4 local_4c;
  undefined4 local_48;
  code *local_44;
  undefined4 local_40;
  int local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d42c0;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  local_58 = this;
  if (DAT_00502a78 != '\0') {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_60 = &DAT_00501908;
    ExceptionList = &pvStack_c;
    FUN_00401d80(&local_60,"DBG_CRASH_TEST");
    ppiVar8 = &local_60;
    local_4 = 0;
    this_00 = (void *)FUN_00499cb0(DAT_00503a78);
    uVar2 = FUN_00438480(this_00,(int *)ppiVar8);
    local_4 = 0xffffffff;
    if ((local_60 != &DAT_00501908) && (*local_60 = *local_60 + -1, *local_60 < 1)) {
      FUN_004011c0(&local_60);
    }
    if ((char)uVar2 != '\0') {
      *DAT_00503258 = 0xdeadbeef;
    }
  }
  iVar5 = (int)this + 0xb4;
  if (*(char *)((int)this + 0x160) == '\0') {
    iVar5 = (int)this + 8;
  }
  FUN_004943e0(iVar5);
  iVar4 = 0;
  local_40 = 0x10;
  local_44 = FUN_0043f660;
  local_54 = (undefined1 *)0x0;
  local_50 = 0;
  local_4c = 0;
  local_48 = 0;
  local_4 = 1;
  iVar5 = (int)this + 0xb4;
  if (*(char *)((int)this + 0x160) == '\0') {
    iVar5 = (int)this + 8;
  }
  iVar3 = *(int *)(iVar5 + 0x80);
  if (0 < iVar3) {
    do {
      if ((iVar4 < 0) || (iVar3 <= iVar4)) {
        piVar7 = &local_3c;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar7 = 0;
          piVar7 = piVar7 + 1;
        }
        local_3c = 1;
        local_38 = "Array index out of bounds";
        local_34 = "..\\BluelAPI/Container/SArray.h";
        local_30 = 0x1cd;
        FUN_00422060(&local_3c);
      }
      piVar7 = *(int **)(*(int *)(iVar5 + 0x7c) + iVar4 * 4);
      if (piVar7 != (int *)0x0) {
        if ((char)piVar7[1] == '\0') {
          (**(code **)(*piVar7 + 4))();
          iVar3 = (**(code **)(*piVar7 + 0xc))();
          uVar1 = local_50;
          if (iVar3 != 0) {
            FUN_0043b6e0(&local_54,local_50 + 1);
            *(int **)(local_54 + uVar1 * 4) = piVar7;
          }
        }
        else {
          if ((iVar4 < 0) || (*(int *)(iVar5 + 0x80) <= iVar4)) {
            piVar6 = &local_24;
            for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
              *piVar6 = 0;
              piVar6 = piVar6 + 1;
            }
            local_24 = 1;
            local_20 = "Array index out of bounds";
            local_1c = "..\\BluelAPI/Container/SArray.h";
            local_18 = 0x1cd;
            FUN_00422060(&local_24);
          }
          *(undefined4 *)(*(int *)(iVar5 + 0x7c) + iVar4 * 4) = 0;
          (**(code **)*piVar7)(1);
        }
      }
      iVar3 = *(int *)(iVar5 + 0x80);
      iVar4 = iVar4 + 1;
      this = local_58;
    } while (iVar4 < iVar3);
  }
  if (*(int *)(DAT_00502aac + 0x6f0) != 2) {
    FUN_004b9d60(local_54,local_50,4,&LAB_00492980);
    iStack_5c = 0;
    if (*(int *)((int)this + 0x170) != 0) {
      geCamera_GetFarClipPlane(*(int *)((int)this + 0x170),&iStack_5c,&local_60);
    }
    if ((((char)*(int *)((int)this + 0x160) == '\0') && (*(int *)((int)this + 0x80) != 0)) &&
       (iStack_5c == 0)) {
      uVar2 = 0;
    }
    else {
      uVar2 = 1;
    }
    geEngine_BeginFrame(*(undefined4 *)(*(int *)this + 0x74),0,uVar2);
    uVar1 = local_50;
    iVar5 = 0;
    if (0 < (int)local_50) {
      do {
        if ((iVar5 < 0) || ((int)uVar1 <= iVar5)) {
          piVar7 = &local_24;
          for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
            *piVar7 = 0;
            piVar7 = piVar7 + 1;
          }
          local_24 = 1;
          local_20 = "Array index out of bounds";
          local_1c = "..\\BluelAPI/Container/SArray.h";
          local_18 = 0x1cd;
          FUN_00422060(&local_24);
        }
        (**(code **)(**(int **)(local_54 + iVar5 * 4) + 0x10))();
        iVar5 = iVar5 + 1;
      } while (iVar5 < (int)uVar1);
    }
    piVar7 = local_58;
    if (*(undefined1 **)(*local_58 + 0x70) != (undefined1 *)0x0) {
      FUN_00427f00(*(undefined1 **)(*local_58 + 0x70));
    }
    if (param_1 != (undefined *)0x0) {
      geEngine_FlushPipeline(*(undefined4 *)(*piVar7 + 0x74));
      (*(code *)param_1)();
    }
    geEngine_EndFrame(*(undefined4 *)(*piVar7 + 0x74));
  }
  local_4 = 0xffffffff;
  if (local_50 != 0) {
    FUN_0043b6e0(&local_54,0);
  }
  ExceptionList = pvStack_c;
  return;
}

