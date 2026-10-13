/* Fresh Ghidra decompiler output; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __thiscall FUN_0044b620(void *this,undefined4 *param_1,uint param_2)

{
  undefined1 *puVar1;
  
  puVar1 = *(undefined1 **)((int)this + 0x88);
  if (puVar1 == (undefined1 *)0x0) {
    puVar1 = FUN_0044b510((int)this);
    *(undefined1 **)((int)this + 0x88) = puVar1;
    *(uint *)(puVar1 + 4) = param_2;
    *(undefined4 *)(puVar1 + 8) = *param_1;
    *(undefined4 *)(puVar1 + 0xc) = param_1[1];
    *(undefined4 *)(puVar1 + 0x10) = param_1[2];
    *puVar1 = 1;
    return;
  }
  if (DAT_00502afc <= param_2) {
    if (*(int *)((int)this + 0x8c) == 0) {
      puVar1 = FUN_0044b510((int)this);
      *(undefined1 **)((int)this + 0x8c) = puVar1;
    }
    else {
      if ((int)param_2 < *(int *)(*(int *)((int)this + 0x8c) + 4)) {
        *(uint *)(puVar1 + 4) = DAT_00502afc;
        *(undefined4 *)(puVar1 + 8) = *(undefined4 *)((int)this + 0x44);
        *(undefined4 *)(puVar1 + 0xc) = *(undefined4 *)((int)this + 0x48);
        *(undefined4 *)(puVar1 + 0x10) = *(undefined4 *)((int)this + 0x4c);
        *puVar1 = 1;
        puVar1 = *(undefined1 **)((int)this + 0x8c);
        *(uint *)(puVar1 + 4) = param_2;
        *(undefined4 *)(puVar1 + 8) = *param_1;
        *(undefined4 *)(puVar1 + 0xc) = param_1[1];
        *(undefined4 *)(puVar1 + 0x10) = param_1[2];
        *puVar1 = 1;
        return;
      }
      puVar1 = FUN_0044b510((int)this);
    }
  }
  *(uint *)(puVar1 + 4) = param_2;
  *(undefined4 *)(puVar1 + 8) = *param_1;
  *(undefined4 *)(puVar1 + 0xc) = param_1[1];
  *(undefined4 *)(puVar1 + 0x10) = param_1[2];
  *puVar1 = 1;
  return;
}


