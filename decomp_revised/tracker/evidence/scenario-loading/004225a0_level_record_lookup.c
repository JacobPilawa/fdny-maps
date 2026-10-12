/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
undefined4 __thiscall FUN_004225a0(void *this,int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  iVar2 = 0;
  if (0 < *(int *)((int)this + 8)) {
    do {
      if ((iVar2 < 0) || (*(int *)((int)this + 8) <= iVar2)) {
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
      if (*(int *)(*(int *)(*(int *)((int)this + 4) + iVar2 * 4) + 0x18) == param_1) {
        if ((iVar2 < 0) || (*(int *)((int)this + 8) <= iVar2)) {
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
        return *(undefined4 *)(*(int *)((int)this + 4) + iVar2 * 4);
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)((int)this + 8));
  }
  return 0;
}
