/* Ghidra decompilation; names/types are provisional.
 * Entry: 0045a900
 * Function: FUN_0045a900
 */


void __thiscall FUN_0045a900(void *this,undefined4 *param_1,undefined4 param_2)

{
  int *this_00;
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0ec8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar1 = operator_new(0x10);
  *puVar1 = *param_1;
  puVar1[1] = param_1[1];
  puVar1[2] = param_1[2];
  this_00 = (int *)((int)this + 0x200);
  puVar1[3] = param_2;
  iVar3 = 0;
  if (0 < *(int *)((int)this + 0x204)) {
    piVar2 = (int *)*this_00;
    do {
      if (*piVar2 == 0) goto LAB_0045a978;
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar3 < *(int *)((int)this + 0x204));
  }
  FUN_0043b6e0(this_00,iVar3 + 1);
LAB_0045a978:
  *(undefined4 **)(*this_00 + iVar3 * 4) = puVar1;
  local_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  local_60 = "rescue\\VictimRescueEnt.cpp";
  local_5c = 0x3c;
  local_58 = 0;
  local_4 = 0;
  FUN_004a3a19(local_48,"TE_VictimRescue (");
  FUN_004a3a19(local_48,*(char **)(*(int *)((int)this + 0x150) + 0x10));
  FUN_004a3a19(local_48,"): Registering SpawnSpot");
  FUN_00401360(&local_64,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  ExceptionList = local_c;
  return;
}

