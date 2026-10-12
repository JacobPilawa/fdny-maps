/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0047d370
 * Match: function entry
 * Entry: 0047d370 FUN_0047d370
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


int __thiscall FUN_0047d370(void *this,int param_1,undefined1 param_2)

{
  int *piVar1;
  void *this_00;
  int extraout_ECX;
  
  *(undefined ***)this = &PTR_FUN_004e3520;
  *(undefined4 *)((int)this + 0x24) = 0;
  *(undefined4 *)((int)this + 0x28) = 0;
  *(undefined4 *)((int)this + 0x2c) = 0;
  *(int *)((int)this + 0xc) = param_1;
  *(undefined1 *)((int)this + 0x10) = param_2;
  piVar1 = (int *)((int)this + 4);
  *(undefined4 *)((int)this + 0x18) = *(undefined4 *)(param_1 + 0x18);
  *piVar1 = 0;
  FUN_0047d2b0(this,piVar1);
  *(int *)((int)this_00 + 8) = *piVar1 + 1;
  FUN_0047d2b0(this_00,(int *)((int)this_00 + 8));
  *(undefined4 *)(extraout_ECX + 0x14) = DAT_00502afc;
  *(undefined4 *)(extraout_ECX + 0x1c) = 0x3f800000;
  *(undefined4 *)(extraout_ECX + 0x20) = 0x3f800000;
  *(undefined1 *)(extraout_ECX + 0x30) = 1;
  return extraout_ECX;
}


