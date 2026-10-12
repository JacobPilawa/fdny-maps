/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void * __thiscall FUN_00403e40(void *this,void *param_1,char *param_2)

{
  int *piVar1;
  int iVar2;
  int *local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;
  
  puStack_8 = &LAB_004cb10f;
  local_c = ExceptionList;
  local_10 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = *(int **)this;
  local_14 = &DAT_00501908;
  ExceptionList = &local_c;
  if (piVar1 != &DAT_00501908) {
    ExceptionList = &local_c;
    *piVar1 = *piVar1 + 1;
    local_14 = piVar1;
  }
  local_4 = 1;
  FUN_00401d00(&local_14,param_2);
  FUN_00401cb0(param_1,&local_14);
  local_10 = 1;
  local_4 = local_4 & 0xffffff00;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  ExceptionList = local_c;
  return param_1;
}

