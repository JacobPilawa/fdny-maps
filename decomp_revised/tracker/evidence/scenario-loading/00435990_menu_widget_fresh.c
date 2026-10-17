/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __thiscall FUN_00435990(void *this,int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  ulonglong uVar3;
  int local_10;
  int local_c;
  undefined4 local_8;
  int local_4;
  
  local_10 = *(int *)((int)this + 8) + param_1;
  local_c = *(int *)((int)this + 8) + param_2;
  iVar1 = *(int *)((int)this + 0x14);
  iVar2 = *(int *)((int)this + 0x10);
  uVar3 = ConvertX87FloatToIntegerTowardZero();
  local_8 = (undefined4)uVar3;
  local_4 = iVar2 + (iVar1 - iVar2) / 2;
  FUN_0047edf0((void *)((int)this + 0x268),&local_10,0);
  return;
}

 (GhidraScript)
