/* Ghidra decompilation; names/types are provisional.
 * Entry: 00459a50
 * Function: FUN_00459a50
 */


void __thiscall FUN_00459a50(void *this,int param_1)

{
  uint uVar1;
  undefined4 uVar2;
  
  if (param_1 == 0) {
    FUN_00480d40((int)this + 0x170);
    return;
  }
  if (*(uint *)((int)this + 0x180) != 6) {
    uVar2 = 0x3e4ccccd;
    uVar1 = FUN_00404b30(*(uint *)((int)this + 0x180));
    FUN_00480ea0((void *)((int)this + 0x170),uVar1,uVar2);
    *(undefined4 *)((int)this + 0x120) = 4;
    return;
  }
  if (param_1 == 1) {
    uVar2 = 0x3f800000;
    uVar1 = FUN_00404b30(3);
    FUN_00480ea0((void *)((int)this + 0x170),uVar1,uVar2);
    return;
  }
  uVar1 = FUN_00404b30(3);
  if (uVar1 == 0) {
    *(undefined4 *)((int)this + 0x228) = 0x4249999a;
  }
  else if ((uVar1 == 1) || (uVar1 == 2)) {
    *(undefined4 *)((int)this + 0x228) = 0x4199999a;
  }
  FUN_00480ea0((void *)((int)this + 0x170),uVar1 + 3,0x3f800000);
  return;
}

