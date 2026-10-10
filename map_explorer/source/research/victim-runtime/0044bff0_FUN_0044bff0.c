/* Ghidra decompilation; names/types are provisional.
 * Entry: 0044bff0
 * Function: FUN_0044bff0
 */


undefined4 * __fastcall FUN_0044bff0(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfb58;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *param_1 = &PTR_LAB_004e0b88;
  FUN_0043aee0((int)(param_1 + 1));
  local_4 = 0;
  *param_1 = &PTR_LAB_004e1860;
  FUN_0049b690(param_1 + 1,"RCS_PlayerRescue",100);
  ExceptionList = local_c;
  return param_1;
}

