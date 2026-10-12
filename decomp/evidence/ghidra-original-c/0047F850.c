/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047f850
 * Match: function entry
 * Entry: 0047f850 FUN_0047f850
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


undefined4 * __thiscall FUN_0047f850(void *this,int param_1,undefined1 param_2)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3498;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0047d370(this,param_1,param_2);
  local_4 = 0;
  FUN_0042d2d0((undefined4 *)((int)this + 0x34),0);
  *(undefined ***)this = &PTR_FUN_004e3548;
  *(undefined4 *)((int)this + 0x34) = &PTR_FUN_004dfc30;
  *(undefined4 *)((int)this + 0x7c) = 0;
  *(undefined4 *)((int)this + 0x80) = 0;
  *(undefined4 *)((int)this + 0x84) = 0;
  *(undefined4 *)((int)this + 0x88) = 0;
  *(undefined4 *)((int)this + 0x8c) = 0;
  *(undefined4 *)((int)this + 0x90) = 0;
  *(undefined4 *)((int)this + 0x94) = 0;
  *(undefined4 *)((int)this + 0x98) = 0;
  *(undefined4 *)((int)this + 0x9c) = 0;
  *(undefined4 *)((int)this + 0xb4) = 0;
  *(undefined4 *)((int)this + 0xb8) = 0;
  *(undefined4 *)((int)this + 0xbc) = 0;
  *(undefined4 *)((int)this + 0xc0) = 0;
  *(undefined4 *)((int)this + 0xc4) = 0;
  *(undefined4 *)((int)this + 200) = 0;
  *(undefined4 *)((int)this + 0xb0) = 0;
  FUN_0047f500(this);
  *(undefined4 *)((int)this + 0x108) = 0x3f7d70a4;
  *(undefined4 *)((int)this + 300) = 0x3f7d70a4;
  *(undefined4 *)((int)this + 0x130) = 0x3f7d70a4;
  *(undefined4 *)((int)this + 0x154) = 0x3f7d70a4;
  *(undefined4 *)((int)this + 0xe4) = 0;
  *(undefined4 *)((int)this + 0xe8) = 0;
  *(undefined4 *)((int)this + 0x10c) = 0;
  *(undefined4 *)((int)this + 0x150) = 0;
  *(undefined1 *)((int)this + 0xac) = 0;
  ExceptionList = local_c;
  return this;
}


