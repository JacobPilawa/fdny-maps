/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00489ce0
 * Match: scratch-defined function boundary
 * Entry: 00489ce0 FUN_00489ce0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


undefined4 __thiscall
FUN_00489ce0(int param_1,float *param_2,float *param_3,undefined4 *param_4,undefined4 *param_5)

{
  float *pfVar1;
  undefined4 *puVar2;
  int iVar3;
  float *pfVar4;
  undefined4 *puVar5;
  float afStack_24 [3];
  float afStack_18 [3];
  float afStack_c [3];
  
  pfVar4 = param_3;
  pfVar1 = param_2;
  iVar3 = FUN_004834a0(param_2,param_3,(float *)(param_1 + 0x18),*(float *)(param_1 + 0x24),
                       (float *)&param_3,(float *)&param_2);
  puVar2 = param_4;
  if (iVar3 == 0) {
    return 0;
  }
  if (param_4 != (undefined4 *)0x0) {
    pfVar4 = (float *)FUN_0045a230(pfVar4,afStack_24);
    pfVar4 = (float *)FUN_00447710(afStack_18,(float)param_3,pfVar4);
    puVar5 = (undefined4 *)FUN_0042d050(afStack_c,pfVar1,pfVar4);
    *puVar2 = *puVar5;
    puVar2[1] = puVar5[1];
    puVar2[2] = puVar5[2];
  }
  if (param_5 != (undefined4 *)0x0) {
    *param_5 = 0x3f800000;
  }
  return 1;
}


