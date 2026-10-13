
/* Summary: Initializes the axe equipment object.

   Algorithm: Runs shared Player-item construction, installs the axe vtable, and initializes its
   embedded equipment components.

   Parameters: param_1 is allocated item storage.

   Returns: The initialized item pointer. */

undefined4 * __fastcall InitializePlayerAxeTool(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d14f6;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0045f7e0(param_1);
  local_4 = 0;
  *param_1 = &PTR_FUN_004e2868;
  FUN_004600f0(param_1 + 0x80,1);
  local_4 = CONCAT31(local_4._1_3_,1);
  FUN_004600f0(param_1 + 0x192,0);
  param_1[0x2a4] = 0;
  *(undefined1 *)(param_1 + 0x7f) = 0;
  ExceptionList = local_c;
  return param_1;
}
