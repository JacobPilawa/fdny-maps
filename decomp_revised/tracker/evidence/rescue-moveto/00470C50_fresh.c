/* Fresh Ghidra decompiler output; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

undefined4 __thiscall FUN_00470c50(void *this,int *param_1)

{
  int *_Str2;
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  undefined4 local_28;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d2608;
  local_c = ExceptionList;
  iVar4 = 0;
  local_4 = 0;
  piVar5 = param_1;
  ExceptionList = &local_c;
  if (0 < *(int *)((int)this + 0x80)) {
    _Str2 = param_1 + 3;
    ExceptionList = &local_c;
    do {
      if ((iVar4 < 0) || (*(int *)((int)this + 0x80) <= iVar4)) {
        piVar5 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar5 = 0;
          piVar5 = piVar5 + 1;
        }
        local_24 = 1;
        local_20 = "Array index out of bounds";
        local_1c = "..\\BluelAPI/Container/SArray.h";
        local_18 = 0x1cd;
        FUN_00422060(&local_24);
        piVar5 = param_1;
      }
      puVar2 = (undefined4 *)
               FUN_0040c330((void *)(*(int *)(*(int *)((int)this + 0x7c) + iVar4 * 4) + 0x24),
                            &local_28);
      iVar3 = __stricmp((char *)(*(int *)*puVar2 + 0xc),(char *)_Str2);
      if (iVar3 == 0) {
        if ((iVar4 < 0) || (*(int *)((int)this + 0x80) <= iVar4)) {
          piVar5 = &local_24;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar5 = 0;
            piVar5 = piVar5 + 1;
          }
          local_24 = 1;
          local_20 = "Array index out of bounds";
          local_1c = "..\\BluelAPI/Container/SArray.h";
          local_18 = 0x1cd;
          FUN_00422060(&local_24);
          piVar5 = param_1;
        }
        uVar1 = *(undefined4 *)(*(int *)((int)this + 0x7c) + iVar4 * 4);
        local_4 = 0xffffffff;
        if (piVar5 == &DAT_00501908) {
          ExceptionList = local_c;
          return uVar1;
        }
        iVar4 = *piVar5;
        *piVar5 = iVar4 + -1;
        if (0 < iVar4 + -1) {
          ExceptionList = local_c;
          return uVar1;
        }
        FUN_004011c0(&param_1);
        ExceptionList = local_c;
        return uVar1;
      }
      iVar4 = iVar4 + 1;
    } while (iVar4 < *(int *)((int)this + 0x80));
  }
  local_4 = 0xffffffff;
  if ((piVar5 != &DAT_00501908) && (iVar4 = *piVar5, *piVar5 = iVar4 + -1, iVar4 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return 0;
}


