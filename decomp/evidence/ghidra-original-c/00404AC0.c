/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00404ac0
 * Match: function entry
 * Entry: 00404ac0 FUN_00404ac0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float10 FUN_00404ac0(void)

{
  uint uVar1;
  
  DAT_004ff370 = DAT_004ff370 + -1;
  if (DAT_004ff370 < 0) {
    uVar1 = FUN_00404960();
  }
  else {
    uVar1 = *DAT_005023ec;
    DAT_005023ec = DAT_005023ec + 1;
    uVar1 = uVar1 ^ uVar1 >> 0xb;
    uVar1 = uVar1 ^ (uVar1 & 0xff3a58ad) << 7;
    uVar1 = uVar1 ^ (uVar1 & 0xffffdf8c) << 0xf;
    uVar1 = uVar1 >> 0x12 ^ uVar1;
  }
  return (float10)(float)(uVar1 & 0x7fffff | 0x3f800000) - (float10)_DAT_004dd808;
}


