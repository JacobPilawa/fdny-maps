/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */

undefined4 __thiscall FUN_00429c00(void *this,int *param_1)

{
  int iVar1;
  bool bVar2;
  int iVar3;
  undefined3 extraout_var;
  undefined3 extraout_var_00;
  undefined4 *puVar4;
  
  iVar1 = *param_1;
  iVar3 = *(int *)(*(int *)((int)this + 4) + iVar1 * 4);
  if (iVar3 == 0) {
    return 0;
  }
  if (*(int *)(iVar3 + 0x18) == 1) {
    if (*(int **)(iVar3 + 0x10) == (int *)0x0) {
      iVar3 = 0;
    }
    else {
      iVar3 = **(int **)(iVar3 + 0x10);
    }
    bVar2 = FUN_0049b700(*(int *)(iVar3 + 4),(int)param_1);
    if (CONCAT31(extraout_var,bVar2) != 0) {
      puVar4 = *(undefined4 **)(*(int *)(*(int *)((int)this + 4) + iVar1 * 4) + 0x10);
      if (puVar4 == (undefined4 *)0x0) {
        return uRam00000000;
      }
      return *(undefined4 *)*puVar4;
    }
  }
  else {
    *(undefined4 *)(iVar3 + 4) = *(undefined4 *)(iVar3 + 0x10);
    *(undefined4 *)(iVar3 + 8) = 0;
    *(undefined1 *)(iVar3 + 0xc) = 0;
    *(undefined1 *)(iVar3 + 0xd) = 1;
    while( true ) {
      iVar3 = *(int *)(*(int *)((int)this + 4) + iVar1 * 4);
      puVar4 = *(undefined4 **)(iVar3 + 4);
      if (puVar4 == (undefined4 *)0x0) break;
      if (*(char *)(iVar3 + 0xd) == '\0') {
        if (*(char *)(iVar3 + 0xc) == '\0') {
          *(undefined4 **)(iVar3 + 8) = puVar4;
          puVar4 = *(undefined4 **)(*(int *)(iVar3 + 4) + 4);
          *(undefined4 **)(iVar3 + 4) = puVar4;
          if (puVar4 == (undefined4 *)0x0) {
            return 0;
          }
          goto LAB_00429cb6;
        }
        *(undefined1 *)(iVar3 + 0xc) = 0;
        puVar4 = (undefined4 *)**(undefined4 **)(iVar3 + 4);
      }
      else {
        *(undefined1 *)(iVar3 + 0xd) = 0;
LAB_00429cb6:
        puVar4 = (undefined4 *)*puVar4;
      }
      if (puVar4 == (undefined4 *)0x0) {
        return 0;
      }
      bVar2 = FUN_0049b700(puVar4[1],(int)param_1);
      if (CONCAT31(extraout_var_00,bVar2) != 0) {
        return *puVar4;
      }
    }
  }
  return 0;
}
