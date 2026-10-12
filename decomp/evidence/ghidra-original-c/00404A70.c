/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00404a70
 * Match: function entry
 * Entry: 00404a70 FUN_00404a70
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


uint FUN_00404a70(void)

{
  uint uVar1;
  
  DAT_004ff370 = DAT_004ff370 + -1;
  if (DAT_004ff370 < 0) {
    uVar1 = FUN_00404960();
    return uVar1;
  }
  uVar1 = *DAT_005023ec;
  DAT_005023ec = DAT_005023ec + 1;
  uVar1 = uVar1 ^ uVar1 >> 0xb;
  uVar1 = uVar1 ^ (uVar1 & 0xff3a58ad) << 7;
  uVar1 = uVar1 ^ (uVar1 & 0xffffdf8c) << 0xf;
  return uVar1 >> 0x12 ^ uVar1;
}


