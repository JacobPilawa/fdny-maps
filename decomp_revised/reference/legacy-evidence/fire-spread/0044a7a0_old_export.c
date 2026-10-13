/* Ghidra decompilation; names/types are provisional.
 * Entry: 0044a7a0
 * Function: FUN_0044a7a0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float10 __thiscall FUN_0044a7a0(int param_1,float *param_2,float param_3)

{
  float fVar1;
  float10 fVar2;
  float local_c;
  float local_8;
  float local_4;
  
  fVar1 = (*(float *)(param_1 + 0x4c) + *(float *)(param_1 + 0x44)) * (float)_DAT_004de6f8;
  if ((float)_DAT_004dfc78 <= fVar1) {
    fVar1 = fVar1 + param_3;
    FUN_0043b2d0(&local_c,param_2,(float *)(param_1 + 0x28));
    fVar2 = (float10)local_8 + (float10)local_8;
    fVar2 = fVar2 * fVar2 +
            (float10)local_c * (float10)local_c + (float10)local_4 * (float10)local_4;
    if (fVar2 <= (float10)fVar1 * (float10)fVar1) {
      return ((float10)*(float *)(*(int *)(param_1 + 0x60) + 0xd0) / (float10)fVar1) *
             ((float10)fVar1 - SQRT(fVar2));
    }
  }
  return (float10)_DAT_004dc50c;
}

