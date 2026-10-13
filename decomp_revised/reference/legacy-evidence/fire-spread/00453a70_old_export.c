/* Ghidra decompilation; names/types are provisional.
 * Entry: 00453a70
 * Function: FUN_00453a70
 */


void __thiscall FUN_00453a70(void *param_1,int param_2,int *param_3)

{
  float *pfVar1;
  float fVar2;
  undefined4 auStack_c [3];
  
  if (*(int *)((int)param_1 + 0x3438) == 0) {
    if (param_2 == 30000) {
      FUN_0044bc00((int)param_1);
    }
    else {
      if (param_2 == 0x7531) {
        fVar2 = (float)param_3[1];
        pfVar1 = (float *)FUN_00447870((void *)(*param_3 + 300),auStack_c);
        FUN_00451640(param_1,pfVar1,fVar2);
        return;
      }
      if (param_2 == 0x7532) {
        FUN_00450440(param_1,(int)param_3);
        return;
      }
    }
  }
  return;
}

