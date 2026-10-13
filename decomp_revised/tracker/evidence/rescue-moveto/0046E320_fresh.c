
void __thiscall FUN_0046e320(void *this,int *param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  int *piVar4;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar4 = param_1;
  puStack_8 = &LAB_004d25e8;
  local_c = ExceptionList;
  piVar2 = *(int **)((int)this + 0x24);
  puVar1 = (undefined4 *)((int)this + 0x24);
  local_4 = 0;
  ExceptionList = &local_c;
  if (piVar2 != param_1) {
    ExceptionList = &local_c;
    if (piVar2 != &DAT_00501908) {
      ExceptionList = &local_c;
      *piVar2 = *piVar2 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    *puVar1 = piVar4;
    if (piVar4 != &DAT_00501908) {
      *piVar4 = *piVar4 + 1;
    }
  }
  *(undefined4 *)((int)this + 0x28) = *param_2;
  *(undefined4 *)((int)this + 0x2c) = param_2[1];
  FUN_00470dd0(&DAT_00502fb0,(int)this);
  local_4 = 0xffffffff;
  if ((piVar4 != &DAT_00501908) && (iVar3 = *piVar4, *piVar4 = iVar3 + -1, iVar3 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return;
}


