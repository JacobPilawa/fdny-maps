/* Fresh Ghidra decompilation
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function: ConstructPlayerRescueEntity at 00453D50
 * Ghidra analysis: current hash-pinned project; saved 2026-10-13
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: Construct the PlayerRescue base entity, initialize its embedded ScriptActor at byte
   offset +0x140, install root and ScriptActor vtables, and initialize model, rescue, carry, sound,
   and player state fields.

   Parameters: this is the allocated PlayerRescue entity; param_1 is the factory request/context
   used to resolve its model.

   Returns: this. */

undefined4 * __thiscall ConstructPlayerRescueEntity(void *this,void *param_1)

{
  int iVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d075a;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0043f610(this,(int)param_1);
  local_4 = 0;
  FUN_0046f930((undefined4 *)((int)this + 0x140));
  local_4._0_1_ = 1;
  *(undefined ***)this = &PTR_FUN_004e1c48;
  *(undefined ***)((int)this + 0x24) = &PTR_FUN_004dfc30;
  *(undefined4 *)((int)this + 0x140) = &PTR_LAB_004e1c40;
  FUN_00491460((int)this + 0x198);
  local_4._0_1_ = 2;
  FUN_00481c40((undefined4 *)((int)this + 0x32c));
  local_4._0_1_ = 3;
  *(undefined4 *)((int)this + 0x394) = 0;
  *(undefined4 *)((int)this + 0x398) = 0;
  *(undefined4 *)((int)this + 0x39c) = 0;
  InitializeSoundInstance((undefined4 *)((int)this + 0x3a0));
  local_4._0_1_ = 4;
  InitializeSoundInstance((undefined4 *)((int)this + 0x3c4));
  local_4._0_1_ = 5;
  InitializeSoundInstance((undefined4 *)((int)this + 1000));
  local_4._0_1_ = 6;
  InitializeSoundInstance((undefined4 *)((int)this + 0x40c));
  local_4._0_1_ = 7;
  InitializeSoundInstance((undefined4 *)((int)this + 0x430));
  local_4._0_1_ = 8;
  InitializeSoundInstance((undefined4 *)((int)this + 0x454));
  local_4._0_1_ = 9;
  InitializeSoundInstance((undefined4 *)((int)this + 0x478));
  local_4._0_1_ = 10;
  InitializeSoundInstance((undefined4 *)((int)this + 0x49c));
  local_4._0_1_ = 0xb;
  InitializeSoundInstance((undefined4 *)((int)this + 0x4c0));
  local_4._0_1_ = 0xc;
  InitializeSoundInstance((undefined4 *)((int)this + 0x4e4));
  local_4._0_1_ = 0xd;
  InitializeSoundInstance((undefined4 *)((int)this + 0x508));
  *(undefined4 *)((int)this + 0x534) = 0;
  *(undefined4 *)((int)this + 0x538) = 0;
  *(undefined4 *)((int)this + 0x53c) = 0;
  *(undefined4 *)((int)this + 0x554) = 0x10;
  *(code **)((int)this + 0x550) = FUN_0044c130;
  *(undefined4 *)((int)this + 0x540) = 0;
  *(undefined4 *)((int)this + 0x544) = 0;
  *(undefined4 *)((int)this + 0x548) = 0;
  *(undefined4 *)((int)this + 0x54c) = 0;
  local_4._0_1_ = 0xf;
  FUN_004469f0((void *)((int)this + 0x55c));
  *(undefined4 *)((int)this + 0x5b8) = 0;
  *(undefined4 *)((int)this + 0x5bc) = 0;
  *(undefined4 *)((int)this + 0x5c0) = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  *(undefined4 **)((int)this + 0x5d0) = &DAT_00501908;
  *(undefined4 *)((int)this + 0x5d8) = 0;
  *(undefined4 *)((int)this + 0x5dc) = 0;
  *(undefined4 *)((int)this + 0x5e0) = 0;
  *(undefined4 *)((int)this + 0x5e4) = 0;
  *(undefined4 *)((int)this + 0x5e8) = 0;
  *(undefined4 *)((int)this + 0x5ec) = 0;
  *(undefined1 *)((int)this + 0x5f0) = 1;
  local_4._0_1_ = 0x12;
  FUN_00458b20((undefined4 *)((int)this + 0x5f4));
  local_4._0_1_ = 0x13;
  FUN_0044f170((undefined4 *)((int)this + 0x3254));
  local_4._0_1_ = 0x14;
  *(undefined4 *)((int)this + 0x3400) = 0;
  *(undefined4 *)((int)this + 0x3404) = 0;
  *(undefined4 *)((int)this + 0x3408) = 0;
  FUN_00451480((int)this + 0x340c);
  local_4._0_1_ = 0x15;
  *(undefined4 *)((int)this + 0x3444) = 0;
  *(undefined4 *)((int)this + 0x3448) = 0;
  *(undefined4 *)((int)this + 0x344c) = 0;
  FUN_0042d2d0((void *)((int)this + 0x3460),0);
  FUN_00483b00((undefined4 *)((int)this + 0x34a4));
  *(undefined4 *)((int)this + 0x3528) = 0xffffffff;
  *(undefined4 *)((int)this + 0x3530) = 0;
  *(undefined4 *)((int)this + 0x352c) = 0;
  FUN_00488f90((undefined4 *)((int)this + 0x3540));
  *(undefined4 *)((int)this + 0x3578) = 0x10;
  *(undefined1 **)((int)this + 0x3574) = &LAB_0044be00;
  *(undefined4 *)((int)this + 0x3564) = 0;
  *(undefined4 *)((int)this + 0x3568) = 0;
  *(undefined4 *)((int)this + 0x356c) = 0;
  *(undefined4 *)((int)this + 0x3570) = 0;
  local_4 = CONCAT31(local_4._1_3_,0x17);
  FUN_0049d160(param_1,(int *)((int)this + 0x194),0x3c);
  iVar1 = *(int *)((int)this + 0x194);
  *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)(iVar1 + 0x30);
  *(undefined4 *)((int)this + 0x50) = *(undefined4 *)(iVar1 + 0x34);
  *(undefined4 *)((int)this + 0x54) = *(undefined4 *)(iVar1 + 0x38);
  *(undefined4 *)((int)this + 0x18) = 1;
  *(void **)(DAT_00503a78 + 0x30) = this;
  ExceptionList = local_c;
  return this;
}
