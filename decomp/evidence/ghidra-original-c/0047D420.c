/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047d420
 * Match: function entry
 * Entry: 0047d420 FUN_0047d420
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __fastcall FUN_0047d420(int param_1)

{
  float fVar1;
  int iVar2;
  int *piVar3;
  int *this;
  float *pfVar4;
  float *pfVar5;
  undefined4 *puVar6;
  int iVar7;
  float local_28;
  float local_24 [3];
  float local_18 [3];
  float local_c [3];
  
  iVar2 = *(int *)(param_1 + 4);
  if ((*(byte *)(param_1 + 0x18) & 0x20) != 0) {
    piVar3 = *(int **)(param_1 + 0xc);
    FUN_0048d490(piVar3,iVar2);
    iVar7 = DAT_00502afc;
    fVar1 = *(float *)(*(int *)(*piVar3 + iVar2 * 4) + 4);
    if (((float)_DAT_004e0b68 <= fVar1) || (fVar1 <= (float)_DAT_004e0b60)) {
      iVar2 = *(int *)(param_1 + 4);
      piVar3 = *(int **)(param_1 + 0xc);
      FUN_0048d490(piVar3,iVar2);
      iVar7 = iVar7 - *(int *)(param_1 + 0x14);
      local_28 = (float)iVar7;
      if (iVar7 < 0) {
        local_28 = local_28 + _DAT_004dfa30;
      }
      local_28 = local_28 /
                 (*(float *)(*(int *)(*piVar3 + iVar2 * 4) + 4) * *(float *)(param_1 + 0x1c) *
                 _DAT_004e0ed4);
    }
    else {
      local_28 = 0.0;
    }
    iVar2 = *(int *)(param_1 + 8);
    piVar3 = *(int **)(param_1 + 0xc);
    FUN_0048d490(piVar3,iVar2);
    iVar7 = *(int *)(param_1 + 4);
    this = *(int **)(param_1 + 0xc);
    iVar2 = *(int *)(*piVar3 + iVar2 * 4);
    FUN_0048d490(this,iVar7);
    iVar7 = *(int *)(*this + iVar7 * 4);
    pfVar4 = (float *)FUN_00446390(local_24,(float *)(iVar2 + 0x10),local_28);
    pfVar5 = (float *)FUN_00446390(local_18,(float *)(iVar7 + 0x10),_DAT_004dfc24 - local_28);
    puVar6 = (undefined4 *)FUN_0042d050(local_c,pfVar5,pfVar4);
    *(undefined4 *)(param_1 + 0x24) = *puVar6;
    *(undefined4 *)(param_1 + 0x28) = puVar6[1];
    *(undefined4 *)(param_1 + 0x2c) = puVar6[2];
    return (undefined4 *)(param_1 + 0x24);
  }
  piVar3 = *(int **)(param_1 + 0xc);
  FUN_0048d490(piVar3,iVar2);
  return (undefined4 *)(*(int *)(*piVar3 + iVar2 * 4) + 0x10);
}


