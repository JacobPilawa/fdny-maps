/* Read-only fresh Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13; renamed in Ghidra after static trace.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Handles the common ScriptActor `que` command.
   
   Algorithm: Retain the command and pass it to the queue helper using the ScriptActor queue at
   this+0x30.
   
   Parameters: this is the ScriptActor; param_1 is the command payload to enqueue.
   
   Returns: truth-like success value (1 in the observed path).
   
   Evidence: Fresh decompilation at this address; common command router at 0x0046F9D0 dispatches
   `que` here. */

undefined4 __thiscall QueueScriptActorCommand(void *this,int *param_1)

{
  int *piVar1;
  int iVar2;
  int *piVar3;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar1 = param_1;
  puStack_8 = &LAB_004d25e8;
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
  if (param_1 != &DAT_00501908) {
    ExceptionList = &local_c;
    *param_1 = *param_1 + 1;
    piVar3 = param_1;
  }
  iVar2 = FUN_0046e750((void *)((int)this + 0x30),piVar3);
  local_4 = 0xffffffff;
  if (piVar1 != &DAT_00501908) {
    iVar2 = *piVar1 + -1;
    *piVar1 = iVar2;
    if (iVar2 < 1) {
      iVar2 = FUN_004011c0(&param_1);
    }
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}
