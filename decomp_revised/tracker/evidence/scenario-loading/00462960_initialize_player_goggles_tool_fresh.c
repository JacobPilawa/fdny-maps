
/* Summary: Initializes the goggles equipment object.

   Algorithm: Runs shared Player-item construction, installs the goggles vtable, and initializes its
   component and sound fields.

   Parameters: param_1 is allocated item storage.

   Returns: The initialized item pointer. */

undefined4 * __fastcall InitializePlayerGogglesTool(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d169a;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0045f7e0(param_1);
  local_4 = 0;
  *param_1 = &PTR_FUN_004e2938;
  FUN_0047dba0(param_1 + 0x7f);
  local_4._0_1_ = 1;
  FUN_0047dba0(param_1 + 0x86);
  local_4._0_1_ = 2;
  FUN_0047dba0(param_1 + 0x8d);
  local_4._0_1_ = 3;
  FUN_0047ddc0(param_1 + 0x95);
  local_4._0_1_ = 4;
  FUN_0047ddc0(param_1 + 0xde);
  local_4._0_1_ = 5;
  FUN_0047ddc0(param_1 + 0x127);
  param_1[0x175] = 0x10;
  param_1[0x174] = FUN_00462640;
  param_1[0x170] = 0;
  param_1[0x171] = 0;
  param_1[0x172] = 0;
  param_1[0x173] = 0;
  local_4 = CONCAT31(local_4._1_3_,7);
  InitializeSoundInstance(param_1 + 0x177);
  ExceptionList = local_c;
  return param_1;
}
