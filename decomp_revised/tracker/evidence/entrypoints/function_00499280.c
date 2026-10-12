
void FUN_00499280(void)

{
  undefined4 *puVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d448b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar1 = operator_new(0x40);
  local_4 = 0;
  if (puVar1 != (undefined4 *)0x0) {
    FUN_00499c70(puVar1);
  }
  ExceptionList = local_c;
  return;
}

