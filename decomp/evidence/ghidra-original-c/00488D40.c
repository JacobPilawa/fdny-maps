/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00488d40
 * Match: function entry
 * Entry: 00488d40 FUN_00488d40
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00488d40(int *param_1)

{
  int iVar1;
  int iVar2;
  float10 fVar3;
  ulonglong uVar4;
  
  FUN_00498b30(0.0,(float)param_1[1] - (float)_DAT_004dfa28);
  uVar4 = FUN_004b9400();
  iVar2 = (int)uVar4;
  if (iVar2 == param_1[6]) {
    fVar3 = FUN_00498b30(0.0,1.0);
    if (fVar3 <= (float10)_DAT_004de6f8) {
      iVar1 = -1;
    }
    else {
      iVar1 = 1;
    }
    iVar2 = iVar2 + iVar1;
  }
  iVar1 = param_1[1];
  if (iVar2 < iVar1) {
    if (iVar2 < 0) {
      iVar2 = iVar1 + -1;
    }
  }
  else {
    iVar2 = 0;
  }
  param_1[6] = iVar2;
  if ((-1 < iVar2) && (iVar2 < iVar1)) {
    FUN_0048d490(param_1,iVar2);
    FUN_004a11a0(*(void **)(*param_1 + iVar2 * 4),(undefined1 *)0x0);
  }
  return;
}


