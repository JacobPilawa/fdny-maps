/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Removes and returns the oldest command string from a ScriptActor queue array.
   
   Algorithm: If the queue count is zero, return an empty string. Otherwise retain the command in
   slot zero, clear that slot, compact remaining non-null entries while preserving their order,
   shrink the array, and return the removed command.
   
   Parameters: this is the queue array at ScriptActor+0x30; param_1 receives the command string.
   
   Returns: param_1, containing the removed command or an empty string when the queue is empty.
   
   Evidence: Fresh decompilation and assembly at 0x0046E530; called by
   ProcessOneQueuedScriptActorCommand (0x004704A0). */

undefined4 * __thiscall PopNextScriptActorQueueEntry(void *this,undefined4 *param_1)

{
  undefined4 *_Memory;
  int iVar1;
  int *piVar2;
  int *piVar3;
  int *local_44;
  undefined4 local_40;
  int local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d236f;
  local_c = ExceptionList;
  local_40 = 0;
  if (*(int *)((int)this + 4) == 0) {
    ExceptionList = &local_c;
    FUN_00402790(param_1,"");
  }
  else {
    ExceptionList = &local_c;
    if (*(int *)((int)this + 4) < 1) {
      piVar2 = &local_3c;
      for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
        *piVar2 = 0;
        piVar2 = piVar2 + 1;
      }
      local_3c = 1;
      local_38 = "Array index out of bounds";
      local_34 = "..\\BluelAPI/Container/SArray.h";
      local_30 = 0x1cd;
      ExceptionList = &local_c;
      FUN_00422060(&local_3c);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar2 = *(int **)**(undefined4 **)this;
    local_44 = &DAT_00501908;
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + 1;
      local_44 = piVar2;
    }
    piVar2 = local_44;
    local_4 = 1;
    if (*(int *)((int)this + 4) < 1) {
      piVar3 = &local_24;
      for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
        *piVar3 = 0;
        piVar3 = piVar3 + 1;
      }
      local_24 = 1;
      local_20 = "Array index out of bounds";
      local_1c = "..\\BluelAPI/Container/SArray.h";
      local_18 = 0x1cd;
      FUN_00422060(&local_24);
    }
    _Memory = (undefined4 *)**(undefined4 **)this;
    if (_Memory != (undefined4 *)0x0) {
      piVar3 = (int *)*_Memory;
      if (piVar3 != &DAT_00501908) {
        *piVar3 = *piVar3 + -1;
        if (*(int *)*_Memory < 1) {
          FUN_004011c0(_Memory);
        }
        else {
          *_Memory = &DAT_00501908;
        }
      }
      _free(_Memory);
    }
    if (*(int *)((int)this + 4) < 1) {
      piVar3 = &local_24;
      for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
        *piVar3 = 0;
        piVar3 = piVar3 + 1;
      }
      local_24 = 1;
      local_20 = "Array index out of bounds";
      local_1c = "..\\BluelAPI/Container/SArray.h";
      local_18 = 0x1cd;
      FUN_00422060(&local_24);
    }
    **(undefined4 **)this = 0;
    CompactNullablePointerArray(this);
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    *param_1 = &DAT_00501908;
    if (piVar2 != &DAT_00501908) {
      *param_1 = piVar2;
      *piVar2 = *piVar2 + 1;
    }
    local_40 = 1;
    local_4 = local_4 & 0xffffff00;
    if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&local_44);
    }
  }
  ExceptionList = local_c;
  return param_1;
}
