/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047d8a0
 * Match: function entry
 * Entry: 0047d8a0 FUN_0047d8a0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float10 __fastcall FUN_0047d8a0(int param_1)

{
  int *piVar1;
  int iVar2;
  int *this;
  float fVar3;
  float fVar4;
  int iVar5;
  
  iVar5 = DAT_00502afc;
  if ((*(byte *)(param_1 + 0x18) & 0x10) == 0) {
    iVar5 = *(int *)(param_1 + 4);
    piVar1 = *(int **)(param_1 + 0xc);
    FUN_0048d490(piVar1,iVar5);
    return (float10)*(float *)(*(int *)(*piVar1 + iVar5 * 4) + 0xc);
  }
  iVar2 = *(int *)(param_1 + 4);
  piVar1 = *(int **)(param_1 + 0xc);
  FUN_0048d490(piVar1,iVar2);
  iVar5 = iVar5 - *(int *)(param_1 + 0x14);
  fVar4 = (float)iVar5;
  if (iVar5 < 0) {
    fVar4 = fVar4 + _DAT_004dfa30;
  }
  iVar5 = *(int *)(param_1 + 4);
  this = *(int **)(param_1 + 0xc);
  fVar4 = fVar4 / (*(float *)(*(int *)(*piVar1 + iVar2 * 4) + 4) * *(float *)(param_1 + 0x1c) *
                  _DAT_004e0ed4);
  FUN_0048d490(this,iVar5);
  iVar2 = *(int *)(param_1 + 8);
  piVar1 = *(int **)(param_1 + 0xc);
  fVar3 = *(float *)(*(int *)(*this + iVar5 * 4) + 0xc);
  FUN_0048d490(piVar1,iVar2);
  return ((float10)_DAT_004dfc24 - (float10)fVar4) * (float10)fVar3 +
         (float10)*(float *)(*(int *)(*piVar1 + iVar2 * 4) + 0xc) * (float10)fVar4;
}


