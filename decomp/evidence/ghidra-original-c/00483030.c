/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00483030
 * Match: function entry
 * Entry: 00483030 FUN_00483030
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float10 __cdecl FUN_00483030(float param_1,float param_2,float param_3,float param_4,float param_5)

{
  uint uVar1;
  float10 fVar2;
  float10 fVar3;
  float10 fVar4;
  float10 fVar5;
  
  if (param_3 < (float)_DAT_004e3540 != (param_3 == (float)_DAT_004e3540)) {
    return (float10)param_2;
  }
  if ((float)_DAT_004e36f8 <= param_3) {
    return (float10)param_1;
  }
  uVar1 = FUN_0043b120(param_2,param_4,2);
  if ((char)uVar1 != '\0') {
    fVar2 = (float10)FUN_004b9a20();
    return fVar2 * (float10)param_1 + ((float10)_DAT_004dfc24 - fVar2) * (float10)param_2;
  }
  fVar2 = ((float10)_DAT_004e36c0 / (float10)param_3) * (float10)param_5;
  fVar3 = (float10)param_1 - (float10)param_4;
  fVar4 = (float10)(param_2 - param_4) * (float10)_DAT_004de6f8;
  fVar5 = (((float10)(double)(fVar3 * fVar2) * (float10)_DAT_004de6f8 + fVar3) - fVar4) * fVar2;
  fVar4 = ((fVar5 * (float10)_DAT_004de6f8 + fVar3) - fVar4) * fVar2;
  return (fVar4 + (float10)(double)fVar5) * (float10)_DAT_004e0860 +
         ((((float10)param_1 + fVar4) - (float10)param_2) * fVar2 + (float10)(double)(fVar3 * fVar2)
         ) * (float10)_DAT_004e36f0 + (float10)param_1;
}


