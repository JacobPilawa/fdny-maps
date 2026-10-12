/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_0047a070(int *param_1)

{
  undefined1 uVar1;
  int *piVar2;
  int iVar3;
  char unaff_BL;
  int *unaff_ESI;
  int *piVar4;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar2 = param_1;
  puStack_8 = &LAB_004d2ed8;
  local_c = ExceptionList;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  ExceptionList = &local_c;
  piVar4 = &DAT_00501908;
  if (param_1 != &DAT_00501908) {
    ExceptionList = &local_c;
    *param_1 = *param_1 + 1;
    piVar4 = param_1;
  }
  iVar3 = FUN_004799e0((uint)piVar4,0,unaff_ESI,unaff_BL);
  uVar1 = (undefined1)iVar3;
  local_4 = 0xffffffff;
  if (piVar2 != &DAT_00501908) {
    iVar3 = *piVar2 + -1;
    *piVar2 = iVar3;
    if (iVar3 < 1) {
      iVar3 = FUN_004011c0(&param_1);
    }
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)iVar3 >> 8),uVar1);
}

