/* FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
   Fresh Ghidra decompilation for the Player ScriptActor hint route. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __thiscall FUN_0044ee50(void *this,float param_1,int *param_2)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar1 = param_2;
  puStack_8 = &LAB_004cfe78;
  local_c = ExceptionList;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  ExceptionList = &local_c;
  piVar3 = &DAT_00501908;
  if (param_2 != &DAT_00501908) {
    ExceptionList = &local_c;
    *param_2 = *param_2 + 1;
    piVar3 = param_2;
  }
  iVar2 = FUN_004564c0((void *)((int)this + 0x5f4),param_1,piVar3);
  local_4 = 0xffffffff;
  if (piVar1 != &DAT_00501908) {
    iVar2 = *piVar1 + -1;
    *piVar1 = iVar2;
    if (iVar2 < 1) {
      iVar2 = FUN_004011c0(&param_2);
    }
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}
