/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */

void * __cdecl FUN_00422de0(void *param_1,int *param_2)

{
  int iVar1;
  void *this;
  int *local_18;
  int *local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cce47;
  local_c = ExceptionList;
  local_10 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  ExceptionList = &local_c;
  FUN_00401d80(&local_14,"data\\maps\\");
  local_4 = 1;
  this = FUN_004040e0(&local_14,&local_18,param_2);
  local_4._0_1_ = 2;
  FUN_00403e40(this,param_1,".bsp");
  local_10 = 1;
  local_4._0_1_ = 1;
  if (local_18 != &DAT_00501908) {
    *local_18 = *local_18 + -1;
    if (*local_18 < 1) {
      FUN_004011c0(&local_18);
    }
    else {
      local_18 = &DAT_00501908;
    }
  }
  local_4 = (uint)local_4._1_3_ << 8;
  if ((local_14 != &DAT_00501908) && (iVar1 = *local_14, *local_14 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  ExceptionList = local_c;
  return param_1;
}
