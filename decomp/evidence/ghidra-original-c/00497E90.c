/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00497e90
 * Match: function entry
 * Entry: 00497e90 FUN_00497e90
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_00497e90(void *this,void *param_1,int param_2,int param_3,undefined1 param_4)

{
  int iVar1;
  
  *(undefined4 *)((int)this + 0x34) = 0x3f800000;
  *(undefined4 *)((int)this + 0x38) = 0x3f800000;
  *(undefined4 *)((int)this + 0x3c) = 0x3f800000;
  *(void **)((int)this + 0x50) = param_1;
  *(undefined4 *)((int)this + 0x94) = 0;
  if (param_2 == 2) {
    iVar1 = FUN_00494500(param_1,param_3,(undefined4 *)((int)this + 0x94),this,param_4);
    *(int *)((int)this + 0x48) = iVar1;
  }
  *(int *)((int)this + 0x4c) = param_2;
  *(undefined1 *)((int)this + 0x45) = 1;
  *(undefined1 *)((int)this + 0x44) = 1;
  *(undefined1 *)((int)this + 0x46) = 0;
  return;
}


