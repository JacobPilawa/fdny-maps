/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00483140
 * Match: function entry
 * Entry: 00483140 FUN_00483140
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl FUN_00483140(float param_1,float param_2,float param_3,float param_4)

{
  float fVar1;
  uint uVar2;
  float10 fVar3;
  undefined4 local_8;
  
  local_8 = DAT_00502b04;
  while( true ) {
    uVar2 = FUN_0043b120(local_8,0.01,2);
    if (((char)uVar2 != '\0') || (local_8 <= _DAT_004e3700)) break;
    fVar1 = (_DAT_004e3700 / local_8) * param_2 +
            (_DAT_004dfc24 - _DAT_004e3700 / local_8) * param_4;
    local_8 = local_8 - _DAT_004e3700;
    fVar3 = FUN_00483030(param_1,fVar1,param_3,param_4,0.01);
    param_1 = (float)fVar3;
    param_4 = fVar1;
  }
  FUN_00483030(param_1,param_2,param_3,param_4,local_8);
  return;
}


