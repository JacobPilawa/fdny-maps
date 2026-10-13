
/* Summary: Initializes the extinguisher equipment object.

   Algorithm: Runs shared Player-item construction, installs the extinguisher vtable, and
   initializes its sound, motion, and component fields.

   Parameters: param_1 is allocated item storage.

   Returns: The initialized item pointer. */

undefined4 * __fastcall InitializePlayerExtinguisherTool(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d15f4;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0045f7e0(param_1);
  local_4 = 0;
  *param_1 = &PTR_FUN_004e28c0;
  InitializeSoundInstance(param_1 + 0x7f);
  local_4._0_1_ = 1;
  InitializeSoundInstance(param_1 + 0x88);
  local_4._0_1_ = 2;
  InitializeSoundInstance(param_1 + 0x91);
  local_4._0_1_ = 3;
  FUN_0047dba0(param_1 + 0x9a);
  local_4._0_1_ = 4;
  FUN_0047dba0(param_1 + 0xa1);
  local_4._0_1_ = 5;
  FUN_004804f0(param_1 + 0xa8);
  local_4._0_1_ = 6;
  FUN_004804f0(param_1 + 0xc9);
  local_4._0_1_ = 7;
  FUN_0047dba0(param_1 + 0xeb);
  local_4._0_1_ = 8;
  FUN_00480260(param_1 + 0xf2);
  local_4._0_1_ = 9;
  _eh_vector_constructor_iterator_
            (param_1 + 0x100,0x10,0x20,(_func_void_void_ptr *)&LAB_0044c060,FUN_0043f660);
  param_1[0x184] = 0;
  param_1[0x185] = 0;
  param_1[0x186] = 0;
  param_1[0x187] = 0;
  param_1[0x188] = 0;
  param_1[0x189] = 0;
  param_1[0x18a] = 0;
  param_1[0x18b] = 0;
  param_1[0x18c] = 0;
  param_1[0x18d] = 0;
  param_1[0x18e] = 0;
  param_1[399] = 0;
  param_1[400] = 0;
  param_1[0x191] = 0;
  param_1[0x192] = 0;
  param_1[0x193] = 0;
  param_1[0x194] = 0;
  param_1[0x195] = 0;
  local_4 = CONCAT31(local_4._1_3_,10);
  param_1[0x196] = 0;
  param_1[0x197] = 0;
  param_1[0x198] = 0;
  FUN_00455d50(param_1 + 0x199);
  ExceptionList = local_c;
  return param_1;
}
