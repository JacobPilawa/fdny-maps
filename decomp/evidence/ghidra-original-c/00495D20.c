/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00495d20
 * Match: function entry
 * Entry: 00495d20 FUN_00495d20
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __cdecl FUN_00495d20(float *param_1,int param_2,float *param_3)

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
  float fVar10;
  float fVar11;
  
  fVar9 = *(float *)(param_2 + 0x34) * *param_3;
  fVar7 = *(float *)(param_2 + 0xc);
  fVar11 = *(float *)(param_2 + 0x38) * param_3[1];
  fVar10 = *(float *)(param_2 + 0x3c) * param_3[2];
  fVar8 = *(float *)(param_2 + 8);
  fVar1 = *(float *)(param_2 + 0x14);
  fVar2 = *(float *)(param_2 + 0x20);
  fVar3 = *(float *)(param_2 + 0x18);
  fVar4 = *(float *)(param_2 + 0x24);
  fVar5 = *(float *)(param_2 + 0x2c);
  fVar6 = *(float *)(param_2 + 0x30);
  *param_1 = *(float *)(param_2 + 4) * fVar9 +
             *(float *)(param_2 + 0x10) * fVar11 + *(float *)(param_2 + 0x1c) * fVar10 +
             *(float *)(param_2 + 0x28);
  param_1[1] = fVar8 * fVar9 + fVar1 * fVar11 + fVar2 * fVar10 + fVar5;
  param_1[2] = fVar7 * fVar9 + fVar3 * fVar11 + fVar4 * fVar10 + fVar6;
  return;
}


