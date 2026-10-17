/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __thiscall FUN_0042d3e0(void *this,int param_1)

{
  int *this_00;
  undefined4 *puVar1;
  int iVar2;
  int *piVar3;
  
  if ((-1 < param_1) && (param_1 < *(int *)((int)this + 0x15c))) {
    this_00 = (int *)((int)this + 0x158);
    iVar2 = 0;
    if (0 < *(int *)((int)this + 0x15c)) {
      piVar3 = (int *)*this_00;
      do {
        if (*piVar3 == *(int *)((int)this + 0x10)) goto LAB_0042d41f;
        iVar2 = iVar2 + 1;
        piVar3 = piVar3 + 1;
      } while (iVar2 < *(int *)((int)this + 0x15c));
    }
    iVar2 = -1;
LAB_0042d41f:
    if (param_1 != iVar2) {
      if (*(undefined4 **)((int)this + 0x10) != (undefined4 *)0x0) {
        (**(code **)**(undefined4 **)((int)this + 0x10))(0);
      }
      FUN_0048d490(this_00,param_1);
      puVar1 = *(undefined4 **)(*this_00 + param_1 * 4);
      *(undefined4 **)((int)this + 0x10) = puVar1;
      (**(code **)*puVar1)(1);
      FUN_0042cf10(this,param_1);
    }
  }
  return;
}
