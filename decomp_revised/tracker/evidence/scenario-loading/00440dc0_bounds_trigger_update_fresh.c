/* Fresh Ghidra decompilation captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 00440DC0 (DispatchBoundsTriggerContactEvents)
 * The pseudocode/listing is analysis output, not recovered source.
 */
/* Summary: Processes a BoundsTrigger's contact container through its embedded ScriptActor and
   updates trigger model flags.
   Algorithm:
   - Receives the embedded ScriptActor at BoundsTrigger root +0x24.
   - Checks the authored model and trigger prerequisites.
   - Calls DispatchScriptActorContactEvents (0x00470840) with the contact pairs at root +0x100.
   - Rotates/clears the contact container after event dispatch.
   - Applies a time-dependent model flag update on the secondary path.
   Parameters:
   - ECX/receiver: BoundsTrigger embedded ScriptActor.
   Returns:
   - No value.
   Evidence:
   - Fresh Ghidra pseudocode and assembly at 0x00440DC0; root+0x14 adjustment is described in the
   BoundsTrigger vtable trace.
   Limits:
   - The owner +0x50 root-slot dispatcher and its order before +0xE0 contact production are
   mapped in root_plus14_owner_scheduler_trace_2026-10-20.md. The interval between normal owner
   passes and live manager membership/cadence are still unverified. */

void __fastcall DispatchBoundsTriggerContactEvents(uint param_1)

{
  undefined4 *this;
  int *piVar1;
  bool bVar2;
  int iVar3;
  undefined4 *this_00;
  ulonglong uVar4;
  undefined4 auStack_64 [22];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004cef18;
  pvStack_c = ExceptionList;
  if (*(int *)(*(int *)(param_1 + 0xd8) + 8) == 0) {
    return;
  }
  ExceptionList = &pvStack_c;
  iVar3 = FUN_00482630(param_1);
  if (iVar3 == 0) {
    ExceptionList = pvStack_c;
    return;
  }
  piVar1 = (int *)(param_1 + 0xdc);
  *(undefined4 *)(param_1 + 0x70) = 0;
  DispatchScriptActorContactEvents((void *)(-(uint)(param_1 != 0x24) & param_1),piVar1);
  FUN_00490430(piVar1);
  FUN_0043aed0((int)piVar1);
  if (*(char *)(param_1 + 0x141) == '\0') {
    ExceptionList = pvStack_c;
    return;
  }
  if (*(int *)(*(int *)(param_1 + 0xd8) + 8) == 0) {
    this_00 = FUN_00401290(auStack_64,"sharedents\\BoundsTriggerEnt.cpp",0x58);
    this = this_00 + 7;
    uStack_4 = 0;
    FUN_004a3a19(this,"BoundsTrigger \"");
    FUN_004a3a19(this,*(char **)(*(int *)(param_1 + 0xd8) + 4));
    FUN_004a3a19(this,"\" has no model");
    FUN_00401360(this_00,2);
    uStack_4 = 0xffffffff;
    FUN_004012c0(auStack_64);
  }
  uVar4 = FUN_004b9400();
  iVar3 = (int)((ulonglong)((longlong)(int)uVar4 * -0x10624dd3) >> 0x20);
  iVar3 = (int)uVar4 + ((iVar3 >> 6) - (iVar3 >> 0x1f)) * 1000;
  if (iVar3 < 500) {
LAB_00440efd:
    bVar2 = false;
  }
  else {
    if (599 < iVar3) {
      if (iVar3 < 700) goto LAB_00440efd;
      if (799 < iVar3) {
        bVar2 = 899 < iVar3;
        goto LAB_00440f16;
      }
    }
    bVar2 = true;
  }
LAB_00440f16:
  geWorld_ModelSetFlags(*(undefined4 *)(*(int *)(param_1 + 0xd8) + 8),bVar2);
  ExceptionList = pvStack_c;
  return;
}

