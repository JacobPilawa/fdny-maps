/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00404930
 * Match: function entry
 * Entry: 00404930 FUN_00404930
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void FUN_00404930(void)

{
  uint in_EAX;
  uint uVar1;
  uint *puVar2;
  int iVar3;
  
  uVar1 = in_EAX | 1;
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


