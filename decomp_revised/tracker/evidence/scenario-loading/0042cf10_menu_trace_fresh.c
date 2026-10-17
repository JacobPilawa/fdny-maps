/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __thiscall FUN_0042cf10(void *this,undefined4 param_1)

{
  int iVar1;
  undefined4 *puVar2;
  int iVar3;
  
  iVar1 = *(int *)((int)this + 0x3c);
  if (9 < iVar1) {
    iVar3 = 0;
    if (0 < iVar1) {
      puVar2 = (undefined4 *)((int)this + 0x14);
      do {
        *puVar2 = puVar2[1];
        iVar3 = iVar3 + 1;
        puVar2 = puVar2 + 1;
      } while (iVar3 < *(int *)((int)this + 0x3c));
    }
    *(undefined4 *)((int)this + *(int *)((int)this + 0x3c) * 4 + 0x14) = param_1;
    return;
  }
  *(undefined4 *)((int)this + iVar1 * 4 + 0x14) = param_1;
  *(int *)((int)this + 0x3c) = *(int *)((int)this + 0x3c) + 1;
  return;
}
