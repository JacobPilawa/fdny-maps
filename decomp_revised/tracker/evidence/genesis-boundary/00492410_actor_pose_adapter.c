
uint __fastcall FUN_00492410(int param_1)

{
  int iVar1;
  undefined4 uVar2;
  
  iVar1 = *(int *)(param_1 + 0x60);
  if (iVar1 == 0) {
    return param_1 & 0xffffff00;
  }
  if (*(char *)(param_1 + 0x81) == '\0') {
    if (*(int *)(param_1 + 100) == 0) {
      uVar2 = geActor_ClearPose(iVar1,param_1 + 0x10);
      return CONCAT31((int3)((uint)uVar2 >> 8),1);
    }
    param_1 = geActor_SetPose(iVar1,*(int *)(param_1 + 100),*(undefined4 *)(param_1 + 0x6c),
                              param_1 + 0x10);
  }
  return CONCAT31((int3)((uint)param_1 >> 8),1);
}

