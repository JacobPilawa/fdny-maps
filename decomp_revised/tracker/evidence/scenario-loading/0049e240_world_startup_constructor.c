/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
undefined4 * __thiscall FUN_0049e240(void *this,void *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d47a8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0049d420(this,(int)param_1);
  local_4 = 0;
  *(undefined ***)this = &PTR_FUN_004e4458;
  FUN_0049d140(param_1,(int *)((int)this + 0x24));
  *(uint *)((int)this + 0x14) = *(uint *)((int)this + 0x14) | 0x14;
  *(undefined1 *)((int)this + 0x28) = 0;
  *(undefined4 *)((int)this + 0x2c) = 4;
  ExceptionList = local_c;
  return this;
}

