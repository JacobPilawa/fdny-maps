/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00483720
 * Match: function entry
 * Entry: 00483720 FUN_00483720
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00483720(int param_1)

{
  uint uVar1;
  undefined4 *puVar2;
  float10 fVar3;
  undefined4 local_50 [3];
  undefined1 local_44 [40];
  float local_1c;
  float local_18;
  float local_14;
  
  if (*(int *)(param_1 + 0x70) != 0) {
    FUN_004977b0(local_44,*(int *)(param_1 + 0x70));
    if (*(char *)(param_1 + 0x80) == '\0') {
      if ((*(byte *)(param_1 + 0x6c) & 0x10) == 0) {
        *(float *)(param_1 + 0x28) = local_1c;
      }
      else {
        fVar3 = (float10)FUN_00483140(*(float *)(param_1 + 0x28),local_1c,*(float *)(param_1 + 0x5c)
                                      ,*(float *)(param_1 + 0x74));
        *(float *)(param_1 + 0x28) = (float)fVar3;
      }
      if ((*(byte *)(param_1 + 0x6c) & 0x20) == 0) {
        *(float *)(param_1 + 0x2c) = local_18;
      }
      else {
        fVar3 = (float10)FUN_00483140(*(float *)(param_1 + 0x2c),local_18,*(float *)(param_1 + 0x60)
                                      ,*(float *)(param_1 + 0x78));
        *(float *)(param_1 + 0x2c) = (float)fVar3;
      }
      if ((*(byte *)(param_1 + 0x6c) & 0x40) == 0) {
        *(float *)(param_1 + 0x30) = local_14;
      }
      else {
        fVar3 = (float10)FUN_00483140(*(float *)(param_1 + 0x30),local_14,*(float *)(param_1 + 100),
                                      *(float *)(param_1 + 0x7c));
        *(float *)(param_1 + 0x30) = (float)fVar3;
      }
      if (((((*(char *)(param_1 + 0x81) != '\0') &&
            (uVar1 = *(uint *)(param_1 + 0x6c), (uVar1 & 0x10) != 0)) &&
           (ABS(local_1c - *(float *)(param_1 + 0x28)) < (float)_DAT_004dfa28)) &&
          (((uVar1 & 0x20) != 0 &&
           (ABS(local_18 - *(float *)(param_1 + 0x2c)) < (float)_DAT_004dfa28)))) &&
         (((uVar1 & 0x40) != 0 &&
          (ABS(local_14 - *(float *)(param_1 + 0x30)) < (float)_DAT_004dfa28)))) {
        *(undefined1 *)(param_1 + 0x80) = 1;
        *(undefined1 *)(param_1 + 0x82) = 0;
      }
    }
    else if ((((*(char *)(param_1 + 0x82) != '\0') ||
              ((uVar1 = *(uint *)(param_1 + 0x6c), (uVar1 & 2) != 0 &&
               (*(float *)(param_1 + 0x44) < ABS(local_1c - *(float *)(param_1 + 0x28)))))) ||
             (((uVar1 & 4) != 0 &&
              (*(float *)(param_1 + 0x48) < ABS(local_18 - *(float *)(param_1 + 0x2c)))))) ||
            (((uVar1 & 8) != 0 &&
             (*(float *)(param_1 + 0x4c) < ABS(local_14 - *(float *)(param_1 + 0x30)))))) {
      if ((*(byte *)(param_1 + 0x6c) & 2) == 0) {
        *(float *)(param_1 + 0x28) = local_1c;
      }
      else {
        fVar3 = (float10)FUN_00483140(*(float *)(param_1 + 0x28),local_1c,*(float *)(param_1 + 0x50)
                                      ,*(float *)(param_1 + 0x74));
        *(float *)(param_1 + 0x28) = (float)fVar3;
      }
      if ((*(byte *)(param_1 + 0x6c) & 4) == 0) {
        *(float *)(param_1 + 0x2c) = local_18;
      }
      else {
        fVar3 = (float10)FUN_00483140(*(float *)(param_1 + 0x2c),local_18,*(float *)(param_1 + 0x54)
                                      ,*(float *)(param_1 + 0x78));
        *(float *)(param_1 + 0x2c) = (float)fVar3;
      }
      if ((*(byte *)(param_1 + 0x6c) & 8) == 0) {
        *(float *)(param_1 + 0x30) = local_14;
      }
      else {
        fVar3 = (float10)FUN_00483140(*(float *)(param_1 + 0x30),local_14,*(float *)(param_1 + 0x58)
                                      ,*(float *)(param_1 + 0x7c));
        *(float *)(param_1 + 0x30) = (float)fVar3;
      }
      uVar1 = *(uint *)(param_1 + 0x6c);
      if (((((uVar1 & 2) != 0) &&
           (*(float *)(param_1 + 0x44) < ABS(local_1c - *(float *)(param_1 + 0x28)))) ||
          (((uVar1 & 4) != 0 &&
           (*(float *)(param_1 + 0x48) < ABS(local_18 - *(float *)(param_1 + 0x2c)))))) ||
         (((uVar1 & 8) != 0 &&
          (*(float *)(param_1 + 0x4c) < ABS(local_14 - *(float *)(param_1 + 0x30)))))) {
        *(undefined1 *)(param_1 + 0x82) = 1;
      }
      else {
        *(undefined1 *)(param_1 + 0x82) = 0;
      }
    }
    else {
      *(float *)(param_1 + 0x28) = local_1c;
      *(float *)(param_1 + 0x2c) = local_18;
      *(float *)(param_1 + 0x30) = local_14;
    }
    puVar2 = (undefined4 *)FUN_00446ee0(local_44,local_50);
    *(undefined4 *)(param_1 + 4) = *puVar2;
    *(undefined4 *)(param_1 + 8) = puVar2[1];
    *(undefined4 *)(param_1 + 0xc) = puVar2[2];
    puVar2 = (undefined4 *)FUN_00447830(local_44,local_50);
    *(undefined4 *)(param_1 + 0x10) = *puVar2;
    *(undefined4 *)(param_1 + 0x14) = puVar2[1];
    *(undefined4 *)(param_1 + 0x18) = puVar2[2];
    puVar2 = (undefined4 *)FUN_0049e770(local_44,local_50);
    *(undefined4 *)(param_1 + 0x1c) = *puVar2;
    *(undefined4 *)(param_1 + 0x20) = puVar2[1];
    *(undefined4 *)(param_1 + 0x24) = puVar2[2];
    *(float *)(param_1 + 0x74) = local_1c;
    *(float *)(param_1 + 0x78) = local_18;
    *(float *)(param_1 + 0x7c) = local_14;
  }
  return;
}


