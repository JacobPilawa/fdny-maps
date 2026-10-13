/* Fresh Ghidra pseudocode export
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Function: DispatchScriptActorCommandWithContext at 00475310
 * Recovered body: 00475310-004753C4
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   - Stores the supplied context pointer at ScriptActor +0x78.
   - Retains the supplied reference-counted command string as needed, calls ScriptActor dispatch
   00470150 with that command and a zero explicit third argument, then releases the temporary
   reference.
   - The common dispatcher uses the ScriptActor's stored context for context-sensitive script
   values; derived command handling remains in vtable slot +4 when the common handler declines.
   
   Parameters:
   - ECX: ScriptActor instance.
   - command_string: shared/ref-counted command string object.
   - context: pointer stored at ScriptActor +0x78 for this dispatch.
   
   Returns:
   - void.
   
   Evidence:
   - Direct callers include the EVS source parser 004799E0, BoundsTrigger and PathPoint setup,
   BModel initialization, and UpdateBModelEntity (0043DC60) timed-event delivery.
   - Fresh pseudocode and raw instructions are archived in
   tracker/evidence/scenario-loading/00475310_scriptactor_context_dispatch.*.
   
   Limits:
   - The command-string object layout is not fully typed. This bridge does not identify the event
   scheduler or callback cadence. */

void __thiscall DispatchScriptActorCommandWithContext(void *this,int *command_string,void *context)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar2 = command_string;
  puStack_8 = &LAB_004d2ed8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(void **)((int)this + 0x78) = context;
  context = &stack0xffffffe8;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar3 = &DAT_00501908;
  if (command_string != &DAT_00501908) {
    *command_string = *command_string + 1;
    piVar3 = command_string;
  }
  FUN_00470150(this,piVar3,0);
  local_4 = 0xffffffff;
  if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&command_string);
  }
  ExceptionList = local_c;
  return;
}


