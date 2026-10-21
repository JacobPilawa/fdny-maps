/* Fresh read-only Ghidra 12.0.4 decompilation from the disposable FDNYPort project.
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.
   The helper walks a path and invokes CreateDirectoryA at each separator. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_00409b40(int *param_1)

{
  int *piVar1;
  uint uVar2;
  BOOL BVar3;
  DWORD DVar4;
  int iVar5;
  int *piVar6;
  uint *puVar7;
  char local_14 [4];
  uint *local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  piVar1 = param_1;
  puStack_8 = &LAB_004cb6c0;
  local_c = ExceptionList;
  uVar2 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  puVar7 = &DAT_00501908;
  local_10 = &DAT_00501908;
  local_4._0_1_ = 1;
  local_4._1_3_ = 0;
  ExceptionList = &local_c;
  if (0 < param_1[1]) {
    piVar6 = param_1 + 3;
    iVar5 = -0xc - (int)param_1;
    ExceptionList = &local_c;
    do {
      local_14[0] = (char)*piVar6;
      local_14[1] = 0;
      FUN_00401d00(&local_10,local_14);
      puVar7 = local_10;
      if (((char)*piVar6 == '\\') || ((char)*piVar6 == '/')) {
        BVar3 = CreateDirectoryA((LPCSTR)(local_10 + 3),(LPSECURITY_ATTRIBUTES)0x0);
        if (BVar3 == 0) {
          DVar4 = GetLastError();
          if (DVar4 == 0x52) {
            local_4 = (uint)local_4._1_3_ << 8;
            uVar2 = 0;
            if (puVar7 != &DAT_00501908) {
              uVar2 = *puVar7 - 1;
              *puVar7 = uVar2;
              if ((int)uVar2 < 1) {
                uVar2 = FUN_004011c0(&local_10);
              }
            }
            local_4 = 0xffffffff;
            if (piVar1 != &DAT_00501908) {
              uVar2 = *piVar1 - 1;
              *piVar1 = uVar2;
              if ((int)uVar2 < 1) {
                uVar2 = FUN_004011c0(&param_1);
              }
            }
            ExceptionList = local_c;
            return uVar2 & 0xffffff00;
          }
        }
      }
      uVar2 = piVar1[1];
      piVar6 = (int *)((int)piVar6 + 1);
    } while ((int)(iVar5 + (int)piVar6) < (int)uVar2);
  }
  local_4 = (uint)local_4._1_3_ << 8;
  if (puVar7 != &DAT_00501908) {
    uVar2 = *puVar7 - 1;
    *puVar7 = uVar2;
    if ((int)uVar2 < 1) {
      uVar2 = FUN_004011c0(&local_10);
    }
  }
  local_4 = 0xffffffff;
  if (piVar1 != &DAT_00501908) {
    uVar2 = *piVar1 - 1;
    *piVar1 = uVar2;
    if ((int)uVar2 < 1) {
      uVar2 = FUN_004011c0(&param_1);
    }
  }
  ExceptionList = local_c;
  return CONCAT31((int3)(uVar2 >> 8),1);
}
