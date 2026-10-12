/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00495eb0
 * Match: function entry
 * Entry: 00495eb0 FUN_00495eb0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __cdecl FUN_00495eb0(float *param_1,int param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  fVar7 = *(float *)(param_2 + 0x34) * *param_3;
  fVar5 = *(float *)(param_2 + 0xc);
  fVar9 = *(float *)(param_2 + 0x38) * param_3[1];
  fVar8 = *(float *)(param_2 + 0x3c) * param_3[2];
  fVar6 = *(float *)(param_2 + 8);
  fVar1 = *(float *)(param_2 + 0x14);
  fVar2 = *(float *)(param_2 + 0x20);
  fVar3 = *(float *)(param_2 + 0x18);
  fVar4 = *(float *)(param_2 + 0x24);
  *param_1 = *(float *)(param_2 + 0x10) * fVar9 +
             *(float *)(param_2 + 4) * fVar7 + *(float *)(param_2 + 0x1c) * fVar8;
  param_1[1] = fVar6 * fVar7 + fVar1 * fVar9 + fVar2 * fVar8;
  param_1[2] = fVar5 * fVar7 + fVar3 * fVar9 + fVar4 * fVar8;
  return;
}


