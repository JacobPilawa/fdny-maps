/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00471310
 * Match: function entry
 * Entry: 00471310 FUN_00471310
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00471310(char *param_1,char param_2)

{
  undefined4 *puVar1;
  int *piVar2;
  int *local_94;
  int *local_90;
  undefined1 *local_8c;
  undefined4 local_88 [9];
  int *local_64 [12];
  undefined4 local_34 [10];
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d26dc;
  local_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_94 = &DAT_00501908;
  ExceptionList = &local_c;
  FUN_00401d80(&local_94,param_1);
  local_4 = 0;
  puVar1 = FUN_00405f70(&local_90,(int *)&local_94);
  local_4._0_1_ = 1;
  if (local_94 != (int *)*puVar1) {
    if ((local_94 != &DAT_00501908) && (*local_94 = *local_94 + -1, *local_94 < 1)) {
      FUN_004011c0(&local_94);
    }
    local_94 = (int *)*puVar1;
    if (local_94 != &DAT_00501908) {
      *local_94 = *local_94 + 1;
    }
  }
  local_4 = (uint)local_4._1_3_ << 8;
  if (local_90 != &DAT_00501908) {
    *local_90 = *local_90 + -1;
    if (*local_90 < 1) {
      FUN_004011c0(&local_90);
    }
    else {
      local_90 = &DAT_00501908;
    }
  }
  if ((char)local_94[3] != '\0') {
    if (local_94[1] < 4) {
      if (local_94 != &DAT_00501908) {
        *local_94 = *local_94 + -1;
        if (*local_94 < 1) {
          FUN_004011c0(&local_94);
        }
        else {
          local_94 = &DAT_00501908;
        }
      }
    }
    else {
      FUN_00403b50(&local_94,local_94[1] + -3);
    }
  }
  FUN_00401d00(&local_94,"evs");
  FUN_004799b0(local_88);
  local_4 = CONCAT31(local_4._1_3_,2);
  if (local_64[0] != local_94) {
    if ((local_64[0] != &DAT_00501908) && (*local_64[0] = *local_64[0] + -1, *local_64[0] < 1)) {
      FUN_004011c0(local_64);
    }
    local_64[0] = local_94;
    if (local_94 != &DAT_00501908) {
      *local_94 = *local_94 + 1;
    }
  }
  local_8c = &stack0xffffff5c;
  if (param_2 == '\0') {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar2 = &DAT_00501908;
    if ((local_94 != &DAT_00501908) && (piVar2 = local_94, local_94 != &DAT_00501908)) {
      *local_94 = *local_94 + 1;
    }
    FUN_0047a070(piVar2);
  }
  else {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar2 = &DAT_00501908;
    if ((local_94 != &DAT_00501908) && (piVar2 = local_94, local_94 != &DAT_00501908)) {
      *local_94 = *local_94 + 1;
    }
    local_8c = &stack0xffffff5c;
    FUN_0047a130(piVar2);
  }
  local_4._0_1_ = 3;
  FUN_0043eec0(local_34);
  local_4 = (uint)local_4._1_3_ << 8;
  FUN_0043f0b0((int)local_88);
  local_4 = 0xffffffff;
  if ((local_94 != &DAT_00501908) && (*local_94 = *local_94 + -1, *local_94 < 1)) {
    FUN_004011c0(&local_94);
  }
  ExceptionList = local_c;
  return;
}


