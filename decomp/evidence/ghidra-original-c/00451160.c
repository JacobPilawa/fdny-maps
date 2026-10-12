/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00451160
 * Match: function entry
 * Entry: 00451160 FUN_00451160
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00451160(int *param_1)

{
  int *local_14;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d02b8;
  local_c = ExceptionList;
  if (param_1[0xd4d] != 0) {
    ExceptionList = &local_c;
    param_1[0xd4b] = param_1[0xd4b] + 1;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_10 = &DAT_00501908;
    FUN_00401d80(&local_10,"Rescues");
    local_4 = 0;
    FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_10,1);
    local_4 = 0xffffffff;
    if ((local_10 != &DAT_00501908) && (*local_10 = *local_10 + -1, *local_10 < 1)) {
      FUN_004011c0(&local_10);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_10 = &DAT_00501908;
    FUN_00401d80(&local_10,"Longest Rescue Time");
    local_4 = 1;
    FUN_004868c0((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_10,
                 DAT_00502afc - param_1[0x171]);
    local_4 = 0xffffffff;
    if ((local_10 != &DAT_00501908) && (*local_10 = *local_10 + -1, *local_10 < 1)) {
      FUN_004011c0(&local_10);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_14 = &DAT_00501908;
    FUN_00401d80(&local_14,"Shortest Rescue Time");
    local_4 = 2;
    FUN_004868c0((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_14,
                 DAT_00502afc - param_1[0x171]);
    local_4 = 0xffffffff;
    if (local_14 != &DAT_00501908) {
      *local_14 = *local_14 + -1;
      if (*local_14 < 1) {
        FUN_004011c0(&local_14);
      }
      else {
        local_14 = &DAT_00501908;
      }
    }
    FUN_0045c000((void *)param_1[0xd4d],param_1);
    FUN_004571a0(param_1 + 0x17d,(int *)0x2,(undefined1 *)0x0);
    thunk_FUN_004541a0((int)(param_1 + 0xd03));
    param_1[0xd4d] = 0;
    FUN_0044fce0('\0');
    FUN_004504c0(param_1);
  }
  ExceptionList = local_c;
  return;
}


