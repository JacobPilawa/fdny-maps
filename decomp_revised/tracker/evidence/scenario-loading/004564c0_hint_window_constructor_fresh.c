/* FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
   Fresh Ghidra decompilation for the retail ScriptActor hint widget constructor. */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_004564c0(void *this,float param_1,int *param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d0ad8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(float *)((int)this + 0x6b4) = param_1;
  local_4 = 0;
  if (*(int *)((int)this + 0x6c0) < 1) {
    piVar2 = &local_24;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar2 = 0;
      piVar2 = piVar2 + 1;
    }
    local_24 = 1;
    local_20 = "Array index out of bounds";
    local_1c = "..\\BluelAPI/Container/SArray.h";
    local_18 = 0x1cd;
    FUN_00422060(&local_24);
  }
  *(float *)(**(int **)((int)this + 0x6bc) + 4) = param_1 * (float)_DAT_004e1430;
  if (*(int *)((int)this + 0x6c0) < 2) {
    piVar2 = &local_24;
    for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
      *piVar2 = 0;
      piVar2 = piVar2 + 1;
    }
    local_24 = 1;
    local_20 = "Array index out of bounds";
    local_1c = "..\\BluelAPI/Container/SArray.h";
    local_18 = 0x1cd;
    FUN_00422060(&local_24);
  }
  *(float *)(*(int *)(*(int *)((int)this + 0x6bc) + 4) + 4) = param_1 * (float)_DAT_004e0508;
  FUN_0047d300(*(void **)((int)this + 0x7c4),0);
  piVar2 = param_2;
  *(undefined4 *)((int)this + 0x6b8) = DAT_00502afc;
  local_34 = 0x78;
  local_30 = 0x208;
  local_2c = 0x28;
  local_28 = 0x1b8;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar3 = &DAT_00501908;
  if (param_2 != &DAT_00501908) {
    *param_2 = *param_2 + 1;
    piVar3 = param_2;
  }
  FUN_0048c780((void *)((int)this + 0x6d8),piVar3,&local_34,2,2);
  local_4 = 0xffffffff;
  if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&param_2);
  }
  ExceptionList = local_c;
  return;
}
