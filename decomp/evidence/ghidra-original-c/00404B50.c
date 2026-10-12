/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00404b50
 * Match: function entry
 * Entry: 00404b50 FUN_00404b50
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __cdecl FUN_00404b50(uint param_1)

{
  uint uVar1;
  uint *puVar2;
  int iVar3;
  
  uVar1 = param_1 & 0x8ffffffd | 1;
  DAT_004ff370 = 0;
  puVar2 = &DAT_00501a2c;
  iVar3 = 0x26f;
  DAT_00501a28 = uVar1;
  do {
    uVar1 = uVar1 * 0x10dcd;
    *puVar2 = uVar1;
    puVar2 = puVar2 + 1;
    iVar3 = iVar3 + -1;
  } while (iVar3 != 0);
  return;
}


