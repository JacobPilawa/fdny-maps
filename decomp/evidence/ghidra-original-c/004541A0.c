/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 004541a0
 * Match: function entry
 * Entry: 004541a0 FUN_004541a0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004541a0(int param_1)

{
  uint uVar1;
  ulonglong uVar2;
  
  uVar2 = FUN_004b9400();
  if (DAT_00502afc < (uint)uVar2) {
    uVar1 = FUN_00404b30(30000);
    *(uint *)(param_1 + 0x14) = uVar1 + (uint)uVar2;
    return;
  }
  uVar2 = FUN_004b9400();
  *(int *)(param_1 + 0x14) = (int)uVar2;
  uVar1 = FUN_00404b30(60000);
  *(uint *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + uVar1;
  return;
}


