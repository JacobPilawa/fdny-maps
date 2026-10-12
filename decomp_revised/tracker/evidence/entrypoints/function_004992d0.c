
void __fastcall FUN_004992d0(int param_1)

{
  void *pvVar1;
  undefined4 *puVar2;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d44c1;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  pvVar1 = operator_new(0x17c);
  local_4 = 0;
  if (pvVar1 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_00495190(pvVar1,&DAT_004dc748);
  }
  local_4 = 0xffffffff;
  DAT_00503a78[1] = puVar2;
  pvVar1 = operator_new(0x368);
  local_4 = 1;
  if (pvVar1 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_004395c0(pvVar1,*(int **)(DAT_00503a70 + 4));
  }
  *DAT_00503a78 = puVar2;
  local_4 = 0xffffffff;
  if (DAT_00502a78 != '\0') {
    pvVar1 = operator_new(0x4a0);
    local_4 = 2;
    if (pvVar1 != (void *)0x0) {
      puVar2 = FUN_004277d0(pvVar1,0x280,(int *)0x1cc);
      *(undefined4 **)(param_1 + 0x70) = puVar2;
      ExceptionList = local_c;
      return;
    }
    *(undefined4 *)(param_1 + 0x70) = 0;
  }
  ExceptionList = local_c;
  return;
}

