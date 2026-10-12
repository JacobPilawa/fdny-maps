/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044d080
 * Match: function entry
 * Entry: 0044d080 FUN_0044d080
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


uint __thiscall FUN_0044d080(void *this,int *param_1)

{
  int *this_00;
  int iVar1;
  byte bVar2;
  undefined3 extraout_var;
  undefined4 *puVar4;
  int *piVar5;
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;
  undefined1 *puVar3;
  
  puStack_8 = &LAB_004cfbd9;
  local_c = ExceptionList;
  local_4 = 0;
  ExceptionList = &local_c;
  bVar2 = FUN_0044ca50((int *)&param_1);
  puVar3 = (undefined1 *)CONCAT31(extraout_var,bVar2);
  if (puVar3 == (undefined1 *)0x0) {
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (iVar1 = *param_1, *param_1 = iVar1 + -1, iVar1 + -1 < 1)) {
      param_1 = (int *)FUN_004011c0(&param_1);
    }
    ExceptionList = local_c;
    return (uint)param_1 & 0xffffff00;
  }
  this_00 = (int *)((int)this + 0x3564);
  FUN_0048d490(this_00,(int)puVar3);
  if (*(int *)(*this_00 + (int)puVar3 * 4) != 0) {
    if (puVar3 != (undefined1 *)0x1) {
      local_4 = 0xffffffff;
      if ((param_1 != &DAT_00501908) && (iVar1 = *param_1, *param_1 = iVar1 + -1, iVar1 + -1 < 1)) {
        param_1 = (int *)FUN_004011c0(&param_1);
      }
LAB_0044d256:
      ExceptionList = local_c;
      return (uint)param_1 & 0xffffff00;
    }
    FUN_0048d490(this_00,1);
    FUN_0045f9f0(*(void **)(*this_00 + 4),'\x01');
    goto LAB_0044d157;
  }
  if (puVar3 == (undefined1 *)0x1) {
    puVar4 = operator_new(0xa94);
    local_4 = CONCAT31(local_4._1_3_,1);
    if (puVar4 == (undefined4 *)0x0) {
LAB_0044d1fd:
      piVar5 = (int *)0x0;
    }
    else {
      piVar5 = FUN_00460a50(puVar4);
    }
  }
  else if (puVar3 == (undefined1 *)0x2) {
    puVar4 = operator_new(0x8d0);
    local_4 = CONCAT31(local_4._1_3_,2);
    if (puVar4 == (undefined4 *)0x0) goto LAB_0044d1fd;
    piVar5 = FUN_00461690(puVar4);
  }
  else {
    if (puVar3 != (undefined1 *)0x3) {
      local_4 = 0xffffffff;
      param_1 = (int *)FUN_00401c90(&param_1);
      goto LAB_0044d256;
    }
    puVar4 = operator_new(0x600);
    local_4 = CONCAT31(local_4._1_3_,3);
    if (puVar4 == (undefined4 *)0x0) goto LAB_0044d1fd;
    piVar5 = FUN_00462960(puVar4);
  }
  local_4 = local_4 & 0xffffff00;
  (**(code **)(*piVar5 + 8))(this);
  FUN_0048d490(this_00,(int)puVar3);
  *(int **)(*this_00 + (int)puVar3 * 4) = piVar5;
  FUN_0044c2a0(this,(int)puVar3,'\0');
  FUN_004571a0((void *)((int)this + 0x5f4),(int *)0x6,puVar3);
  FUN_004a11a0((void *)((int)this + 0x49c),(undefined1 *)0x0);
LAB_0044d157:
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (iVar1 = *param_1, *param_1 = iVar1 + -1, iVar1 + -1 < 1)) {
    param_1 = (int *)FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)param_1 >> 8),1);
}


