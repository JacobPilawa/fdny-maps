/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

undefined4 * __thiscall FUN_00436630(void *this,undefined4 param_1,int param_2,int param_3)

{
  undefined4 uVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004ce49b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined ***)this = &PTR_FUN_004e0840;
  *(undefined4 *)((int)this + 0x24) = 0x10;
  *(undefined1 **)((int)this + 0x20) = &LAB_004326e0;
  *(undefined4 *)((int)this + 0x10) = 0;
  *(undefined4 *)((int)this + 0x14) = 0;
  *(undefined4 *)((int)this + 0x18) = 0;
  *(undefined4 *)((int)this + 0x1c) = 0;
  *(undefined4 *)((int)this + 0x28) = param_1;
  *(undefined4 *)((int)this + 0xc) = 0;
  local_4 = 0;
  uVar1 = FUN_00493d20(*(void **)(DAT_00503a78 + 4),param_2,'\x01');
  *(undefined4 *)((int)this + 4) = uVar1;
  if (param_3 < 0) {
    *(undefined4 *)((int)this + 8) = 0;
  }
  else {
    uVar1 = FUN_00493d20(*(void **)(DAT_00503a78 + 4),param_3,'\x01');
    *(undefined4 *)((int)this + 8) = uVar1;
  }
  ExceptionList = local_c;
  return this;
}

 (GhidraScript)
