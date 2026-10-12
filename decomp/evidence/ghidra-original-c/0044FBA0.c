/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044fba0
 * Match: function entry
 * Entry: 0044fba0 FUN_0044fba0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0044fba0(int param_1)

{
  int iVar1;
  int *piVar2;
  int *local_18;
  int local_14;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cffd0;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined4 *)(param_1 + 0x3528) = 0;
  FUN_00481c00(&local_14);
  local_4 = 0;
  piVar2 = local_10;
  while( true ) {
    if ((local_14 == 0) || (piVar2 == (int *)0x0)) {
      iVar1 = 0;
    }
    else {
      iVar1 = *piVar2;
    }
    if ((local_14 != 0) && (piVar2 != (int *)0x0)) {
      piVar2 = (int *)piVar2[1];
      local_10 = piVar2;
    }
    if (iVar1 == 0) break;
    if (*(int *)(iVar1 + 0x18) == 2) {
      *(int *)(param_1 + 0x3528) = *(int *)(param_1 + 0x3528) + 1;
    }
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_18 = &DAT_00501908;
  FUN_00401d80(&local_18,"Rescues");
  local_4._0_1_ = 1;
  FUN_00486920((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_18,
               *(undefined4 *)(param_1 + 0x3528));
  local_4 = (uint)local_4._1_3_ << 8;
  if (local_18 != &DAT_00501908) {
    *local_18 = *local_18 + -1;
    if (*local_18 < 1) {
      FUN_004011c0(&local_18);
    }
    else {
      local_18 = &DAT_00501908;
    }
  }
  FUN_00484260((undefined4 *)(param_1 + 0x5d8));
  local_4 = 0xffffffff;
  if (local_14 != 0) {
    FUN_00429fe0((void *)(local_14 + 0x28),(int)&local_14);
  }
  ExceptionList = local_c;
  return;
}


