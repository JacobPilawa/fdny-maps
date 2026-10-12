/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00481a60
 * Match: function entry
 * Entry: 00481a60 FUN_00481a60
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


void __thiscall FUN_00481a60(void *this,void *param_1,int *param_2,int param_3)

{
  int *piVar1;
  undefined4 local_8 [2];
  
  if (*(int *)this < 0x20) {
    piVar1 = (int *)(*(int *)this * 0x10 + 4 + (int)this);
    *piVar1 = *param_2;
    piVar1[1] = param_2[1];
    piVar1[2] = param_2[2];
    *(int *)((*(int *)this + 1) * 0x10 + (int)this) = param_3;
    piVar1 = (int *)FUN_0049d350(param_1,local_8);
    FUN_0049d4e0(*piVar1,piVar1[1],30000,*(int *)this * 0x10 + 4 + (int)this);
    *(int *)this = *(int *)this + 1;
  }
  return;
}


