/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0043d660
 * Match: function entry
 * Entry: 0043d660 FUN_0043d660
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


undefined4 __fastcall FUN_0043d660(int param_1)

{
  undefined4 in_EAX;
  uint uVar1;
  int *extraout_ECX;
  int *piVar2;
  
  if (*(int *)(param_1 + 0x70) != 4) {
    *(undefined4 *)(param_1 + 0x70) = 3;
    return CONCAT31((int3)((uint)in_EAX >> 8),1);
  }
  FUN_0046e450(param_1 + 0x24);
  piVar2 = extraout_ECX;
  FUN_00402790(&stack0xfffffff4,"open");
  uVar1 = FUN_0046e9d0((void *)(param_1 + 0x24),piVar2);
  return uVar1 & 0xffffff00;
}


