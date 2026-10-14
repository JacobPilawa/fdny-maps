/* Hash-pinned retail EXE function 00496020; names and types are provisional. */

float * __cdecl FUN_00496020(float *param_1,int param_2,float *param_3)

{
  float *pfVar1;
  float local_c [3];
  
  if (*(int *)(param_2 + 0x40) != 0) {
    pfVar1 = (float *)FUN_00495d20(local_c,param_2,param_3);
    FUN_00496020(param_1,*(int *)(param_2 + 0x40),pfVar1);
    return param_1;
  }
  FUN_00495d20(param_1,param_2,param_3);
  return param_1;
}

