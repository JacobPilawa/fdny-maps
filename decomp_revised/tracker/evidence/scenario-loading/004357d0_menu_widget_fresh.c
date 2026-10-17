/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __thiscall FUN_004357d0(void *this,undefined4 param_1,undefined4 param_2,undefined4 *param_3)

{
  *(undefined4 *)((int)this + 0x18) = param_1;
  *(undefined ***)this = &PTR_FUN_004e0780;
  *(undefined4 *)((int)this + 4) = param_2;
  *(undefined4 *)((int)this + 8) = *param_3;
  *(undefined4 *)((int)this + 0x10) = param_3[2];
  *(undefined4 *)((int)this + 0xc) = param_3[1];
  *(undefined4 *)((int)this + 0x14) = param_3[3];
  *(undefined1 *)((int)this + 0x1c) = 1;
  return;
}

 (GhidraScript)
