/* Fresh Ghidra decompilation captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 00470840 (DispatchScriptActorContactEvents)
 * The pseudocode/listing is analysis output, not recovered source.
 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Converts contact-pair state into ScriptActor touch events.
   Algorithm:
   - For newly added pairs, resolves the other actor's registered name and dispatches touch.
   - For persistent pairs, dispatches touching.
   - For removed pairs, dispatches untouch.
   - Stores the contact-derived name as ScriptActor context before routing the event and stops a
   pass when the actor's event flag is set.
   Parameters:
   - param_1: receiving ScriptActor.
   - param_2: contact-pair container.
   Returns:
   - No value.
   Evidence:
   - Fresh Ghidra pseudocode at 0x00470840; it calls DispatchScriptActorEvent (0x004705B0).
   Limits:
   - Establishes static event conversion only; it does not prove a particular Gas R contact fires
   live. */

void DispatchScriptActorContactEvents(void *param_1,void *param_2)

{
  int *piVar1;
  undefined4 uVar2;
  int iVar3;
  
  *(undefined1 *)((int)param_1 + 0x74) = 0;
  iVar3 = 0;
  piVar1 = (int *)FUN_00490300(param_2,0);
  while (piVar1 != (int *)0x0) {
    uVar2 = FUN_00470b50(&DAT_00502fb0,piVar1);
    *(undefined4 *)((int)param_1 + 0x70) = uVar2;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar1 = &DAT_00501908;
    FUN_00401d80(&stack0xffffffec,"touch");
    DispatchScriptActorEvent(param_1,piVar1);
    if (*(char *)((int)param_1 + 0x74) != '\0') break;
    iVar3 = iVar3 + 1;
    piVar1 = (int *)FUN_00490300(param_2,iVar3);
  }
  *(undefined1 *)((int)param_1 + 0x74) = 0;
  piVar1 = (int *)FUN_00490380(param_2,0);
  if (piVar1 != (int *)0x0) {
    iVar3 = 0;
    do {
      uVar2 = FUN_00470b50(&DAT_00502fb0,piVar1);
      *(undefined4 *)((int)param_1 + 0x70) = uVar2;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar1 = &DAT_00501908;
      FUN_00401d80(&stack0xffffffec,"touching");
      DispatchScriptActorEvent(param_1,piVar1);
      if (*(char *)((int)param_1 + 0x74) != '\0') break;
      iVar3 = iVar3 + 1;
      piVar1 = (int *)FUN_00490380(param_2,iVar3);
    } while (piVar1 != (int *)0x0);
  }
  *(undefined1 *)((int)param_1 + 0x74) = 0;
  piVar1 = (int *)FUN_004903a0(param_2,0);
  if (piVar1 != (int *)0x0) {
    iVar3 = 0;
    do {
      uVar2 = FUN_00470b50(&DAT_00502fb0,piVar1);
      *(undefined4 *)((int)param_1 + 0x70) = uVar2;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar1 = &DAT_00501908;
      FUN_00401d80(&stack0xffffffec,"untouch");
      DispatchScriptActorEvent(param_1,piVar1);
      if (*(char *)((int)param_1 + 0x74) != '\0') {
        return;
      }
      iVar3 = iVar3 + 1;
      piVar1 = (int *)FUN_004903a0(param_2,iVar3);
    } while (piVar1 != (int *)0x0);
  }
  return;
}

