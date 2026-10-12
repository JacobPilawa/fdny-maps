/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0043e2a0
 * Match: function entry
 * Entry: 0043e2a0 FUN_0043e2a0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __fastcall FUN_0043e2a0(int param_1)

{
  undefined4 uVar1;
  uint uVar2;
  int *piVar3;
  
  if (((*(int *)(*(int *)(param_1 + 0xec) + 0x34) != 0) &&
      (piVar3 = *(int **)(*(int *)(param_1 + 0xec) + 0x38), piVar3 == (int *)0x0)) &&
     (((float)_DAT_004e0b68 <= *(float *)(param_1 + 0x1ec) ||
      (*(float *)(param_1 + 0x1ec) <= (float)_DAT_004e0b60)))) {
    uVar2 = 2;
    FUN_00402790(&stack0xffffffec,"chop");
    uVar1 = FUN_004705b0((void *)(param_1 + 0x24),piVar3);
    if ((char)uVar1 != '\0') {
      uVar2 = 6;
    }
    piVar3 = (int *)(*(int *)(param_1 + 0xec) + 0x34);
    *piVar3 = *piVar3 + -1;
    piVar3 = *(int **)(*(int *)(param_1 + 0xec) + 0x34);
    if (piVar3 == (int *)0x0) {
      FUN_00402790(&stack0xffffffec,"break");
      uVar1 = FUN_004705b0((void *)(param_1 + 0x24),piVar3);
      if ((char)uVar1 == '\0') {
        uVar2 = uVar2 & 0xfffffffb;
      }
      uVar2 = uVar2 | 1;
      FUN_0043d660(param_1);
    }
    return uVar2;
  }
  return 0;
}


