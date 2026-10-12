
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00480d50(int param_1)

{
  if ((*(char *)(param_1 + 0x30) == '\0') && (*(int *)(param_1 + 0x14) != 0)) {
    geActor_SetPose(*(undefined4 *)(param_1 + 8),*(int *)(param_1 + 0x14),0,
                    *(undefined4 *)(param_1 + 4));
    return;
  }
  if (*(int *)(param_1 + 0x14) == 0) {
                    /* WARNING: Could not recover jumptable at 0x00480d82. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    geActor_ClearPose(*(undefined4 *)(param_1 + 8));
    return;
  }
  geActor_SetPose(*(undefined4 *)(param_1 + 8),*(int *)(param_1 + 0x14),
                  (float)(int)((uint)(DAT_00502afc - *(int *)(param_1 + 0x28)) %
                              *(uint *)(param_1 + 0x2c)) * *(float *)(param_1 + 0x24) *
                  (float)_DAT_004dfa28 + *(float *)(param_1 + 0x1c),*(undefined4 *)(param_1 + 4));
  return;
}

