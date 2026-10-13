/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Processes at most one queued command for a ScriptActor.
   
   Algorithm: Pop the oldest command from the queue at ScriptActor+0x30. If its string is non-empty,
   dispatch it once through RouteParsedScriptActorCommand; if empty, return without dispatch.
   
   Parameters: param_1 is the ScriptActor.
   
   Returns: truth-like success value.
   
   Evidence: Fresh decompilation at 0x004704A0; invoked by UpdateScriptActorWaitAndQueue
   (0x00470740). */

uint __fastcall ProcessOneQueuedScriptActorCommand(void *param_1)

{
  int *piVar1;
  int *local_14;
  undefined1 *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d25c8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  PopNextScriptActorQueueEntry((void *)((int)param_1 + 0x30),&local_14);
  local_4 = 0;
  if (local_14[1] == 0) {
    local_4 = 0xffffffff;
    if ((local_14 != &DAT_00501908) && (*local_14 = *local_14 + -1, *local_14 < 1)) {
      local_14 = (int *)FUN_004011c0(&local_14);
    }
    ExceptionList = local_c;
    return (uint)local_14 & 0xffffff00;
  }
  local_10 = &stack0xffffffe0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  if (local_14 != &DAT_00501908) {
    piVar1 = local_14;
    if (local_14 != &DAT_00501908) {
      *local_14 = *local_14 + 1;
    }
  }
  RouteParsedScriptActorCommand(param_1,piVar1,0);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (*local_14 = *local_14 + -1, *local_14 < 1)) {
    local_14 = (int *)FUN_004011c0(&local_14);
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)local_14 >> 8),1);
}
