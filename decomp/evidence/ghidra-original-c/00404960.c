/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00404960
 * Match: function entry
 * Entry: 00404960 FUN_00404960
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


uint FUN_00404960(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  uint *puVar6;
  uint *puVar7;
  uint *puVar8;
  
  puVar8 = &DAT_00501a28;
  puVar7 = &DAT_00501a30;
  if (DAT_004ff370 < -1) {
    FUN_00404930();
  }
  DAT_004ff370 = 0x26f;
  DAT_005023ec = &DAT_00501a2c;
  iVar4 = 0xe3;
  uVar1 = DAT_00501a2c;
  uVar5 = DAT_00501a28;
  do {
    uVar3 = uVar1;
    *puVar8 = ((uVar3 ^ uVar5) & 0x7ffffffe ^ uVar5) >> 1 ^ -(uint)((uVar3 & 1) != 0) & 0x9908b0df ^
              puVar7[0x18b];
    uVar1 = *puVar7;
    puVar8 = puVar8 + 1;
    puVar7 = puVar7 + 1;
    iVar4 = iVar4 + -1;
    uVar5 = uVar3;
  } while (iVar4 != 0);
  puVar6 = &DAT_00501a28;
  iVar4 = 0x18c;
  do {
    uVar2 = uVar1;
    *puVar8 = ((uVar2 ^ uVar3) & 0x7ffffffe ^ uVar3) >> 1 ^ -(uint)((uVar2 & 1) != 0) & 0x9908b0df ^
              *puVar6;
    uVar5 = DAT_00501a28;
    uVar1 = *puVar7;
    puVar8 = puVar8 + 1;
    puVar6 = puVar6 + 1;
    puVar7 = puVar7 + 1;
    iVar4 = iVar4 + -1;
    uVar3 = uVar2;
  } while (iVar4 != 0);
  *puVar8 = ((DAT_00501a28 ^ uVar2) & 0x7ffffffe ^ uVar2) >> 1 ^
            -(uint)((DAT_00501a28 & 1) != 0) & 0x9908b0df ^ *puVar6;
  uVar5 = uVar5 ^ uVar5 >> 0xb;
  uVar5 = uVar5 ^ (uVar5 & 0xff3a58ad) << 7;
  uVar5 = uVar5 ^ (uVar5 & 0xffffdf8c) << 0xf;
  return uVar5 >> 0x12 ^ uVar5;
}


