
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __thiscall FUN_00422ef0(void *this,undefined4 param_1,undefined4 *param_2)

{
  int iVar1;
  bool bVar2;
  void *this_00;
  int *piVar3;
  int *local_1c;
  int *local_18;
  int *local_14;
  undefined4 local_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cce8f;
  pvStack_c = ExceptionList;
  local_10 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  ExceptionList = &pvStack_c;
  FUN_00401d80(&local_14,"data\\maps\\");
  local_4 = 1;
  this_00 = AppendStringObject(&local_14,&local_18,this);
  local_4._0_1_ = 2;
  AppendCStringToString(this_00,&local_1c,".bmp");
  local_4._0_1_ = 5;
  if (local_18 != &DAT_00501908) {
    *local_18 = *local_18 + -1;
    if (*local_18 < 1) {
      FUN_004011c0(&local_18);
    }
    else {
      local_18 = &DAT_00501908;
    }
  }
  local_4 = CONCAT31(local_4._1_3_,4);
  if ((local_14 != &DAT_00501908) && (iVar1 = *local_14, *local_14 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  local_14 = (int *)&stack0xffffffd4;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar3 = &DAT_00501908;
  if ((local_1c != &DAT_00501908) && (piVar3 = local_1c, local_1c != &DAT_00501908)) {
    *local_1c = *local_1c + 1;
  }
  bVar2 = FUN_004097f0(piVar3);
  if (bVar2) {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    *param_2 = &DAT_00501908;
    if (local_18 == &DAT_00501908) {
      ExceptionList = puStack_8;
      return param_2;
    }
    *param_2 = local_18;
    *local_18 = *local_18 + 1;
    *local_18 = *local_18 + -1;
    iVar1 = *local_18;
  }
  else {
    FUN_00402790(param_2,"data\\maps\\brief.bmp");
    if (local_18 == &DAT_00501908) {
      ExceptionList = puStack_8;
      return param_2;
    }
    *local_18 = *local_18 + -1;
    iVar1 = *local_18;
  }
  pvStack_c = (void *)0x1;
  if (iVar1 < 1) {
    FUN_004011c0(&local_18);
  }
  ExceptionList = puStack_8;
  return param_2;
}
