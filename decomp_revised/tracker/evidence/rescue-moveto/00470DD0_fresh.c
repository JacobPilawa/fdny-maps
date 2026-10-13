
void __thiscall FUN_00470dd0(void *this,int param_1)

{
  void *this_00;
  int *piVar1;
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
  puStack_8 = &LAB_004d2630;
  local_c = ExceptionList;
  iVar3 = 0;
  ExceptionList = &local_c;
  piVar2 = this;
  FUN_00401cb0(&stack0xffffff84,(undefined4 *)(param_1 + 0x24));
  local_4 = 0;
  local_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  local_60 = "script\\scriptengine.cpp";
  local_5c = 0x12;
  local_58 = 0;
  local_4 = CONCAT31(local_4._1_3_,2);
  FUN_004a3a19(local_48,"- added actor to ScriptEngine: ");
  this_00 = (void *)FUN_0040f020(&local_64,piVar2);
  FUN_00401360(this_00,iVar3);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  piVar2 = (int *)((int)this + 0x7c);
  iVar3 = 0;
  if (0 < *(int *)((int)this + 0x80)) {
    piVar1 = (int *)*piVar2;
    do {
      if (*piVar1 == 0) {
        *(int *)(*piVar2 + iVar3 * 4) = param_1;
        ExceptionList = local_c;
        return;
      }
      iVar3 = iVar3 + 1;
      piVar1 = piVar1 + 1;
    } while (iVar3 < *(int *)((int)this + 0x80));
  }
  FUN_0043b6e0(piVar2,iVar3 + 1);
  *(int *)(*piVar2 + iVar3 * 4) = param_1;
  ExceptionList = local_c;
  return;
}


