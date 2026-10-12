
/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00475310(void *this,int *param_1,undefined1 *param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar2 = param_1;
  puStack_8 = &LAB_004d2ed8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined1 **)((int)this + 0x78) = param_2;
  param_2 = &stack0xffffffe8;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar3 = &DAT_00501908;
  if (param_1 != &DAT_00501908) {
    *param_1 = *param_1 + 1;
    piVar3 = param_1;
  }
  FUN_00470150(this,piVar3,0);
  local_4 = 0xffffffff;
  if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return;
}

