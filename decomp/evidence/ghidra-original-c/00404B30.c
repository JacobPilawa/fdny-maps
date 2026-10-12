/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00404b30
 * Match: function entry
 * Entry: 00404b30 FUN_00404b30
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


uint __cdecl FUN_00404b30(uint param_1)

{
  uint uVar1;
  
  if ((int)param_1 < 1) {
    return 0;
  }
  uVar1 = FUN_00404a70();
  return uVar1 % param_1;
}


