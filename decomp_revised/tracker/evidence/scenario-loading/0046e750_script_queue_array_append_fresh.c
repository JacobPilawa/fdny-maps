/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Adds a reference-counted command string to a ScriptActor command queue array.
   
   Algorithm: Allocate a queue entry, scan the array for the first null slot, grow the array if no
   slot is available, then store the entry at that index.
   
   Parameters: this is the queue array at ScriptActor+0x30; param_1 is the command string.
   
   Returns: void.
   
   Evidence: Fresh decompilation and assembly at 0x0046E750; called by QueueScriptActorCommand
   (0x0046E9D0). Queue consumption and callback cadence are outside this function. */

void __thiscall AppendScriptActorQueueEntry(void *this,int *param_1)

{
  int *piVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;
  
  puStack_8 = &LAB_004d2393;
  local_c = ExceptionList;
  iVar4 = 0;
  local_4 = 0;
  ExceptionList = &local_c;
  puVar2 = operator_new(4);
  piVar1 = param_1;
  if (puVar2 == (undefined4 *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    *puVar2 = &DAT_00501908;
    if (param_1 != &DAT_00501908) {
      *puVar2 = param_1;
      *param_1 = *param_1 + 1;
    }
  }
  local_4 = local_4 & 0xffffff00;
  if (0 < *(int *)((int)this + 4)) {
    piVar3 = *(int **)this;
    do {
      if (*piVar3 == 0) goto LAB_0046e7f8;
      iVar4 = iVar4 + 1;
      piVar3 = piVar3 + 1;
    } while (iVar4 < *(int *)((int)this + 4));
  }
  FUN_0043b6e0(this,iVar4 + 1);
LAB_0046e7f8:
  *(undefined4 **)(*(int *)this + iVar4 * 4) = puVar2;
  local_4 = 0xffffffff;
  if ((piVar1 != &DAT_00501908) && (iVar4 = *piVar1, *piVar1 = iVar4 + -1, iVar4 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return;
}
