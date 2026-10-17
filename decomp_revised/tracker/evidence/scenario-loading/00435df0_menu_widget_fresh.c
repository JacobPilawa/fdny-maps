/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __thiscall FUN_00435df0(void *this,char param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  undefined4 local_20;
  undefined4 local_1c;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  if ((param_1 == '\0') && (iVar2 = 0, 0 < *(int *)((int)this + 0x14))) {
    do {
      if ((iVar2 < 0) || (*(int *)((int)this + 0x14) <= iVar2)) {
        piVar3 = &local_18;
        for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        local_18 = 1;
        local_14 = "Array index out of bounds";
        local_10 = "..\\BluelAPI/Container/SArray.h";
        local_c = 0x1cd;
        FUN_00422060(&local_18);
      }
      local_20 = 0;
      local_1c = 0;
      (**(code **)(**(int **)(*(int *)((int)this + 0x10) + iVar2 * 4) + 0x18))(&local_20);
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)((int)this + 0x14));
  }
  return;
}

 (GhidraScript)
