/* Fresh read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13.
 */

/* Summary: Applies the ScriptActor wait deadline and pumps one queued command when the actor is
   ready.
   
   Algorithm: If wait state at +0x4C is 1, compare deadline +0x48 with current game-clock value
   DAT_00502AFC; leave the queue paused until the deadline is reached, then clear the wait state.
   Otherwise invoke the actor's primary virtual update and snapshot its state at +0x50. When +0x4C
   is zero, call ProcessOneQueuedScriptActorCommand once.
   
   Parameters: ECX is the ScriptActor instance.
   
   Returns: void.
   
   Evidence: Fresh decompilation and assembly at 0x00470740; wait-state fields are written by
   HandleCommonScriptActorCommand (0x0046F9D0). External callback cadence is not established. */

void __fastcall UpdateScriptActorWaitAndQueue(undefined4 *param_1)

{
  if (param_1[0x13] == 1) {
    if ((uint)param_1[0x12] <= DAT_00502afc) {
      param_1[0x13] = 0;
    }
  }
  else {
    (**(code **)*param_1)();
    param_1[0x14] = param_1[0x13];
  }
  if (param_1[0x13] == 0) {
    ProcessOneQueuedScriptActorCommand(param_1);
    return;
  }
  return;
}
