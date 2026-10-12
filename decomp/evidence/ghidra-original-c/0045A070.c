/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045a070
 * Match: scratch-defined function boundary
 * Entry: 0045a070 FUN_0045a070
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0045a070(int param_1,int *param_2)

{
  float *pfVar1;
  int iVar2;
  int *piVar3;
  undefined4 auStack_8 [2];
  
  piVar3 = param_2;
  if ((((*(int *)(param_1 + 0x120) != 5) && (*(int *)(param_1 + 0x120) != 6)) && (param_2[6] == 1))
     && (pfVar1 = (float *)(**(code **)(*param_2 + 0xc))(), _DAT_00502cbc = pfVar1,
        pfVar1 != (float *)0x0)) {
    param_2 = *(int **)(*(int *)(param_1 + 0x150) + 0x1c);
    iVar2 = FUN_00459b40((float)param_2,0.0);
    if (iVar2 < 1) {
      param_2 = (int *)0x42700000;
    }
    if ((((pfVar1[4] + *pfVar1 <= (float)param_2 + *(float *)(param_1 + 0x4c)) &&
         (*(float *)(param_1 + 0x4c) - (float)param_2 <= pfVar1[7] + *pfVar1)) &&
        ((pfVar1[6] + pfVar1[2] <= (float)param_2 + *(float *)(param_1 + 0x54) &&
         ((*(float *)(param_1 + 0x54) - (float)param_2 <= pfVar1[9] + pfVar1[2] &&
          (pfVar1[5] + pfVar1[1] <= (float)param_2 + *(float *)(param_1 + 0x50))))))) &&
       (*(float *)(param_1 + 0x50) - (float)param_2 <= pfVar1[8] + pfVar1[1])) {
      piVar3 = (int *)FUN_0049d350(piVar3,auStack_8);
      FUN_0049d4e0(*piVar3,piVar3[1],0x7532,param_1);
    }
  }
  return;
}


