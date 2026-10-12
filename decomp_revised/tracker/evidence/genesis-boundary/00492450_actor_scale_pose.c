
void __fastcall FUN_00492450(int param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 0x60) != 0) {
    geActor_SetScale(*(int *)(param_1 + 0x60),*(undefined4 *)(param_1 + 0x44),
                     *(undefined4 *)(param_1 + 0x40),*(undefined4 *)(param_1 + 0x44),
                     *(undefined4 *)(param_1 + 0x48));
  }
  iVar1 = *(int *)(param_1 + 0x60);
  if ((iVar1 != 0) && (*(char *)(param_1 + 0x81) == '\0')) {
    if (*(int *)(param_1 + 100) == 0) {
                    /* WARNING: Could not recover jumptable at 0x00492488. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      geActor_ClearPose(iVar1,param_1 + 0x10);
      return;
    }
    geActor_SetPose(iVar1,*(int *)(param_1 + 100),*(undefined4 *)(param_1 + 0x6c),param_1 + 0x10);
  }
  return;
}

