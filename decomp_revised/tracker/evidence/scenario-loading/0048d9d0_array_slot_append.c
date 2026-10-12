/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
int __thiscall FUN_0048d9d0(void *this,undefined4 param_1)

{
  int *piVar1;
  int iVar2;
  
  iVar2 = 0;
  if (0 < *(int *)((int)this + 4)) {
    piVar1 = *(int **)this;
    do {
      if (*piVar1 == 0) goto LAB_0048d9f8;
      iVar2 = iVar2 + 1;
      piVar1 = piVar1 + 1;
    } while (iVar2 < *(int *)((int)this + 4));
  }
  FUN_0043b6e0(this,iVar2 + 1);
LAB_0048d9f8:
  *(undefined4 *)(*(int *)this + iVar2 * 4) = param_1;
  return iVar2;
}
