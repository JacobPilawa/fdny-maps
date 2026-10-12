/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0048d8f0
 * Match: function entry
 * Entry: 0048d8f0 FUN_0048d8f0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0048d8f0(int param_1)

{
  int iVar1;
  int iVar2;
  float10 fVar3;
  ulonglong uVar4;
  
  if (*(uint *)(param_1 + 0x40) <= DAT_00502afc) {
    FUN_00498b30(*(float *)(*(int *)(param_1 + 0x44) + 8),*(float *)(*(int *)(param_1 + 0x44) + 0xc)
                );
    uVar4 = FUN_004b9400();
    *(int *)(param_1 + 0x40) = (int)uVar4;
    FUN_00498b30(0.0,(float)*(int *)(param_1 + 0x28) - (float)_DAT_004dfa28);
    uVar4 = FUN_004b9400();
    iVar2 = (int)uVar4;
    if (iVar2 == *(int *)(param_1 + 0x3c)) {
      fVar3 = FUN_00498b30(0.0,1.0);
      if (fVar3 <= (float10)_DAT_004de6f8) {
        iVar1 = -1;
      }
      else {
        iVar1 = 1;
      }
      iVar2 = iVar2 + iVar1;
    }
    iVar1 = *(int *)(param_1 + 0x28);
    if (iVar2 < iVar1) {
      if (iVar2 < 0) {
        iVar2 = iVar1 + -1;
      }
    }
    else {
      iVar2 = 0;
    }
    *(int *)(param_1 + 0x3c) = iVar2;
    if ((-1 < iVar2) && (iVar2 < iVar1)) {
      FUN_0048d490((int *)(param_1 + 0x24),iVar2);
      FUN_004a11a0(*(void **)(*(int *)(param_1 + 0x24) + iVar2 * 4),(undefined1 *)0x0);
    }
  }
  return;
}


