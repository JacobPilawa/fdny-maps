/* Ghidra decompilation; names/types are provisional.
 * Entry: 0044cfd0
 * Function: FUN_0044cfd0
 */


undefined4 FUN_0044cfd0(void)

{
  void *this;
  undefined4 uVar1;
  int *this_00;
  int iVar2;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfba0;
  local_c = ExceptionList;
  iVar2 = 0;
  this_00 = (int *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30);
  ExceptionList = &local_c;
  FUN_00487440(this_00,(undefined4 *)&stack0xffffff90,(int *)0x3f800000);
  local_4 = 0;
  local_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  local_60 = "rescue\\PlayerRescueEnt.cpp";
  local_5c = 0x56e;
  local_58 = 0;
  local_4 = CONCAT31(local_4._1_3_,2);
  this = (void *)FUN_0040f020(&local_64,this_00);
  FUN_00401360(this,iVar2);
  local_4 = 0xffffffff;
  uVar1 = FUN_004012c0(&local_64);
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)uVar1 >> 8),1);
}

