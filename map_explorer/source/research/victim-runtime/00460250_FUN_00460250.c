/* Ghidra decompilation; names/types are provisional.
 * Entry: 00460250
 * Function: FUN_00460250
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00460250(void *this,int *param_1)

{
  void *this_00;
  int *this_01;
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  int *piStack_58;
  int local_54;
  char *local_50;
  char *local_4c;
  undefined4 local_48;
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
  puStack_8 = &LAB_004d1421;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  FUN_0045f660(this,param_1,(-(uint)(*(char *)((int)this + 0x1fc) != '\0') & 5) + 1);
  FUN_00401d80((void *)((int)this + 0x24),"axe");
  this_00 = (void *)((int)this + 0x2d0);
  local_54 = 0x3f800000;
  local_50 = (char *)0x3f800000;
  local_4c = (char *)0x0;
  FUN_0047db10(this_00,2,0x3f800000,0x3f800000,0x3f000000,&local_54);
  local_54 = 0x3f800000;
  local_50 = (char *)0x0;
  local_4c = (char *)0x0;
  FUN_0047db10(this_00,2,0x3f800000,0x3f800000,0x3f000000,&local_54);
  local_54 = 0;
  local_50 = (char *)0x0;
  local_4c = (char *)0x3f800000;
  FUN_0047db10(this_00,2,0x3f800000,0x3f800000,0x3f000000,&local_54);
  iVar4 = 0;
  *(undefined4 *)((int)this + 0x2e8) = 0;
  param_1 = operator_new(0x168);
  local_4 = 0;
  if (param_1 == (int *)0x0) {
    param_1 = (int *)0x0;
  }
  else {
    param_1 = FUN_0047f850(param_1,(int)this_00,0);
  }
  this_01 = (int *)((int)this + 0x2ec);
  local_4 = 0xffffffff;
  if (0 < *(int *)((int)this + 0x2f0)) {
    piVar2 = (int *)*this_01;
    do {
      if (*piVar2 == 0) goto LAB_004603a8;
      iVar4 = iVar4 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar4 < *(int *)((int)this + 0x2f0));
  }
  FUN_0043b6e0(this_01,iVar4 + 1);
LAB_004603a8:
  *(int **)(*this_01 + iVar4 * 4) = param_1;
  param_1 = operator_new(0x168);
  iVar4 = 0;
  local_4 = 1;
  if (param_1 == (int *)0x0) {
    param_1 = (int *)0x0;
  }
  else {
    param_1 = FUN_0047f850(param_1,(int)this_00,0);
  }
  local_4 = 0xffffffff;
  if (0 < *(int *)((int)this + 0x2f0)) {
    piVar2 = (int *)*this_01;
    do {
      if (*piVar2 == 0) goto LAB_0046040c;
      iVar4 = iVar4 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar4 < *(int *)((int)this + 0x2f0));
  }
  FUN_0043b6e0(this_01,iVar4 + 1);
LAB_0046040c:
  *(int **)(*this_01 + iVar4 * 4) = param_1;
  param_1 = operator_new(0x168);
  local_4 = 2;
  if (param_1 == (int *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0047f850(param_1,(int)this_00,0);
  }
  iVar4 = 0;
  local_4 = 0xffffffff;
  if (0 < *(int *)((int)this + 0x2f0)) {
    piVar2 = (int *)*this_01;
    do {
      if (*piVar2 == 0) goto LAB_0046046d;
      iVar4 = iVar4 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar4 < *(int *)((int)this + 0x2f0));
  }
  FUN_0043b6e0(this_01,iVar4 + 1);
LAB_0046046d:
  *(undefined4 **)(*this_01 + iVar4 * 4) = puVar1;
  iVar4 = 0;
  if (0 < *(int *)((int)this + 0x2f0)) {
    do {
      if ((iVar4 < 0) || (*(int *)((int)this + 0x2f0) <= iVar4)) {
        piVar2 = &local_54;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = 0;
          piVar2 = piVar2 + 1;
        }
        local_54 = 1;
        local_50 = "Array index out of bounds";
        local_4c = "..\\BluelAPI/Container/SArray.h";
        local_48 = 0x1cd;
        FUN_00422060(&local_54);
      }
      *(undefined1 *)(*(int *)(*this_01 + iVar4 * 4) + 0xad) = 0;
      if ((iVar4 < 0) || (*(int *)((int)this + 0x2f0) <= iVar4)) {
        piVar2 = &local_3c;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = 0;
          piVar2 = piVar2 + 1;
        }
        local_3c = 1;
        local_38 = "Array index out of bounds";
        local_34 = "..\\BluelAPI/Container/SArray.h";
        local_30 = 0x1cd;
        FUN_00422060(&local_3c);
      }
      FUN_0047d300(*(void **)(*this_01 + iVar4 * 4),iVar4);
      if ((iVar4 < 0) || (*(int *)((int)this + 0x2f0) <= iVar4)) {
        piVar2 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar2 = 0;
          piVar2 = piVar2 + 1;
        }
        local_24 = 1;
        local_20 = "Array index out of bounds";
        local_1c = "..\\BluelAPI/Container/SArray.h";
        local_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      *(undefined4 *)(*(int *)(*this_01 + iVar4 * 4) + 0xa4) = 0x30;
      iVar4 = iVar4 + 1;
    } while (iVar4 < *(int *)((int)this + 0x2f0));
  }
  local_54 = 0x3f800000;
  local_50 = (char *)0x3f800000;
  local_4c = (char *)0x3f800000;
  FUN_0047db10((void *)((int)this + 0x308),0x2c0,0x3f800000,0x3f000000,0x3f800000,&local_54);
  *(undefined4 *)((int)this + 0x368) = 0;
  *(undefined4 *)((int)this + 0x370) = 300;
  (**(code **)(*(int *)((int)this + 0x200) + 0x2c))();
  (**(code **)(*(int *)((int)this + 0x200) + 0x1c))();
  *(undefined4 *)((int)this + 0x340) = 0;
  *(undefined4 *)((int)this + 0x33c) = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"AxeSwing");
  local_4 = 3;
  FUN_004a12f0((void *)((int)this + 0x3b8),&param_1,0,2);
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"AxeHitWood");
  local_4 = 4;
  FUN_004a12f0((void *)((int)this + 0x3dc),&param_1,0,2);
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"AxeHitOther");
  local_4 = 5;
  FUN_004a12f0((void *)((int)this + 0x400),&param_1,0,2);
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_58 = &DAT_00501908;
  FUN_00401d80(&piStack_58,"WoodDoorBreak");
  local_4 = 6;
  FUN_004a12f0((void *)((int)this + 0x424),&piStack_58,0,2);
  local_4 = 0xffffffff;
  if ((piStack_58 != &DAT_00501908) && (*piStack_58 = *piStack_58 + -1, *piStack_58 < 1)) {
    FUN_004011c0(&piStack_58);
  }
  ExceptionList = pvStack_c;
  return;
}

