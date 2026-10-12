/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 004834a0
 * Match: function entry
 * Entry: 004834a0 FUN_004834a0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl
FUN_004834a0(float *param_1,float *param_2,float *param_3,float param_4,float *param_5,
            float *param_6)

{
  float fVar1;
  float fVar2;
  float local_c;
  float local_8;
  float local_4;
  
  FUN_0043b2d0(&local_c,param_3,param_1);
  fVar2 = local_c * local_c + local_8 * local_8 + local_4 * local_4;
  fVar1 = local_c * *param_2 + local_8 * param_2[1] + local_4 * param_2[2];
  if (((param_4 * param_4 <= fVar2) && (fVar1 < _DAT_004dc50c)) ||
     (fVar2 = (param_4 * param_4 - fVar2) + fVar1 * fVar1, fVar2 < _DAT_004dc50c)) {
    return 0;
  }
  fVar2 = SQRT(fVar2);
  if (param_5 != (float *)0x0) {
    *param_5 = fVar1 - fVar2;
  }
  if (param_6 == (float *)0x0) {
    return 1;
  }
  *param_6 = fVar2 + fVar1;
  return 1;
}


