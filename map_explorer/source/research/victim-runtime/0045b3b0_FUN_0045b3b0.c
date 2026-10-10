/* Ghidra decompilation; names/types are provisional.
 * Entry: 0045b3b0
 * Function: FUN_0045b3b0
 */


undefined4 FUN_0045b3b0(int *param_1)

{
  int iVar1;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_004d0ef0;
  local_c = ExceptionList;
  local_4 = 0;
  local_64 = &PTR_FUN_004dc554;
  ExceptionList = &local_c;
  FUN_004a3118();
  local_60 = "rescue\\VictimRescueEnt.cpp";
  local_5c = 0x2b8;
  local_58 = 0;
  local_4._0_1_ = 1;
  FUN_004a3a19(local_48,"\"PlayAnim\" IS UNIMPLEMENTED");
  FUN_00401360(&local_64,0);
  local_4 = (uint)local_4._1_3_ << 8;
  FUN_004012c0(&local_64);
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (iVar1 = *param_1, *param_1 = iVar1 + -1, iVar1 + -1 < 1)) {
    param_1 = (int *)FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)param_1 >> 8),1);
}

