/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0048e080
 * Match: function entry
 * Entry: 0048e080 FUN_0048e080
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __thiscall FUN_0048e080(void *this,int *param_1,int param_2)

{
  int *piVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  ulonglong uVar5;
  int local_30;
  int *local_2c;
  void *local_28;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3ec6;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  local_28 = this;
  FUN_004a13a0(this);
  piVar2 = (int *)((int)this + 0x24);
  *(undefined4 *)((int)this + 0x38) = 0x10;
  *(code **)((int)this + 0x34) = FUN_0044c130;
  *piVar2 = 0;
  *(undefined4 *)((int)this + 0x28) = 0;
  *(undefined4 *)((int)this + 0x2c) = 0;
  *(undefined4 *)((int)this + 0x30) = 0;
  local_4 = 1;
  *(int **)((int)this + 0x44) = param_1;
  FUN_00498b30(0.0,5000.0);
  uVar5 = FUN_004b9400();
  *(int *)((int)this + 0x40) = (int)uVar5;
  local_30 = 0;
  if (0 < *(int *)(*(int *)((int)this + 0x44) + 0x18)) {
    do {
      local_2c = operator_new(0x24);
      local_4._0_1_ = 2;
      if (local_2c == (int *)0x0) {
        local_2c = (int *)0x0;
      }
      else {
        local_2c = FUN_004a13a0(local_2c);
      }
      iVar4 = *(int *)((int)this + 0x44);
      local_4._0_1_ = 1;
      if ((local_30 < 0) || (*(int *)(iVar4 + 0x18) <= local_30)) {
        piVar1 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar1 = 0;
          piVar1 = piVar1 + 1;
        }
        local_24 = 1;
        local_20 = "Array index out of bounds";
        local_1c = "..\\BluelAPI/Container/SArray.h";
        local_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      param_1 = &DAT_00501908;
      FUN_00401d80(&param_1,(char *)(**(int **)(*(int *)(iVar4 + 0x14) + local_30 * 4) + 0xc));
      local_4._0_1_ = 3;
      FUN_004a12f0(local_2c,&param_1,param_2,2);
      local_4 = CONCAT31(local_4._1_3_,1);
      if (param_1 != &DAT_00501908) {
        *param_1 = *param_1 + -1;
        if (*param_1 < 1) {
          FUN_004011c0(&param_1);
        }
        else {
          param_1 = &DAT_00501908;
        }
      }
      iVar4 = 0;
      if (0 < *(int *)((int)this + 0x28)) {
        piVar1 = (int *)*piVar2;
        do {
          if (*piVar1 == 0) goto LAB_0048e238;
          iVar4 = iVar4 + 1;
          piVar1 = piVar1 + 1;
        } while (iVar4 < *(int *)((int)this + 0x28));
      }
      FUN_0043b6e0(piVar2,iVar4 + 1);
LAB_0048e238:
      *(int **)(*piVar2 + iVar4 * 4) = local_2c;
      local_30 = local_30 + 1;
    } while (local_30 < *(int *)(*(int *)((int)this + 0x44) + 0x18));
  }
  piVar2 = FUN_0048dbe0(*(void **)((int)this + 0x44),&local_2c);
  local_4._0_1_ = 4;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,(char *)(*piVar2 + 0xc));
  local_4._0_1_ = 5;
  FUN_004a12f0(this,&param_1,param_2,2);
  local_4._0_1_ = 4;
  if (param_1 != &DAT_00501908) {
    *param_1 = *param_1 + -1;
    if (*param_1 < 1) {
      FUN_004011c0(&param_1);
    }
    else {
      param_1 = &DAT_00501908;
    }
  }
  local_4 = CONCAT31(local_4._1_3_,1);
  if ((local_2c != &DAT_00501908) && (*local_2c = *local_2c + -1, *local_2c < 1)) {
    FUN_004011c0(&local_2c);
  }
  *(undefined4 *)((int)this + 0x3c) = 0;
  ExceptionList = local_c;
  return this;
}


