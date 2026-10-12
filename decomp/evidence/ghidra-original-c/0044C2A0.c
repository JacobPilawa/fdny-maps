/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044c2a0
 * Match: function entry
 * Entry: 0044c2a0 FUN_0044c2a0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


undefined4 __thiscall FUN_0044c2a0(void *this,int param_1,char param_2)

{
  uint *this_00;
  uint in_EAX;
  undefined4 *puVar1;
  int iVar2;
  
  if (param_1 < 5) {
    this_00 = (uint *)((int)this + 0x3564);
    FUN_0048d490(this_00,param_1);
    in_EAX = *this_00;
    if (*(int *)(in_EAX + param_1 * 4) != 0) {
      iVar2 = *(int *)((int)this + 0x357c);
      if (param_1 != iVar2) {
        if (iVar2 == 0) {
          *(int *)((int)this + 0x357c) = param_1;
          FUN_0048d490(this_00,param_1);
          (**(code **)(**(int **)(*this_00 + param_1 * 4) + 0x3c))(1);
        }
        else {
          if (param_2 == '\0') {
            puVar1 = (undefined4 *)FUN_0040cdf0(this_00,iVar2);
            in_EAX = (**(code **)(*(int *)*puVar1 + 0x38))();
            if ((char)in_EAX == '\0') goto LAB_0044c358;
          }
          puVar1 = (undefined4 *)FUN_0040cdf0(this_00,*(int *)((int)this + 0x357c));
          (**(code **)(*(int *)*puVar1 + 0x3c))(0);
          *(int *)((int)this + 0x357c) = param_1;
          puVar1 = (undefined4 *)FUN_0040cdf0(this_00,param_1);
          (**(code **)(*(int *)*puVar1 + 0x3c))(1);
        }
        iVar2 = FUN_004571a0((void *)((int)this + 0x5f4),(int *)0x5,(undefined1 *)0x0);
      }
      return CONCAT31((int3)((uint)iVar2 >> 8),1);
    }
  }
LAB_0044c358:
  return in_EAX & 0xffffff00;
}


