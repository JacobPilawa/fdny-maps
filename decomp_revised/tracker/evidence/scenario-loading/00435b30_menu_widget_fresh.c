/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

undefined4 * __thiscall
FUN_00435b30(void *this,undefined4 param_1,undefined4 param_2,int *param_3,undefined4 param_4,
            int param_5,undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9)

{
  undefined4 *this_00;
  int iVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ce464;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_00435a10(this,param_1,param_2,param_3,param_4,param_5,param_6,param_7,param_8,param_9);
  this_00 = (undefined4 *)((int)this + 0x144);
  local_4 = 0;
  *(undefined ***)this = &PTR_FUN_004e0800;
  FUN_0047ddc0(this_00);
  local_4._0_1_ = 1;
  FUN_0047ddc0((undefined4 *)((int)this + 0x268));
  local_4 = CONCAT31(local_4._1_3_,2);
  FUN_0047ee10(this_00,*(int *)(*(int *)((int)this + 0x18) + 0x28) + 0x18c,'\0');
  FUN_0047ee10((undefined4 *)((int)this + 0x268),*(int *)(*(int *)((int)this + 0x18) + 0x28) + 0x1a8
               ,'\0');
  *(undefined4 *)((int)this + 0x374) = 0x42c80000;
  *(undefined1 *)((int)this + 0x255) = 1;
  *(undefined1 *)((int)this + 0x379) = 1;
  *(undefined4 *)((int)this + 0x390) = 0;
  FUN_00435990(this,0,100);
  FUN_0047dc90(this_00,'\0');
  iVar1 = (*(int *)((int)this + 0x14) - *(int *)((int)this + 0x10)) / 2 + *(int *)((int)this + 0x10)
  ;
  FUN_0047ea10(this_00,*(int *)((int)this + 8),iVar1 + -0x80,*(int *)((int)this + 0xc),iVar1,0);
  ExceptionList = local_c;
  return this;
}

 (GhidraScript)
