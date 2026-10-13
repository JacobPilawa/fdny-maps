/* Read-only fresh Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13; renamed in Ghidra after static trace.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Runs a nested ScriptActor command only once for a generated key in that actor's local
   table.
   
   Algorithm: Format a key from the current context value at ScriptActor+0x78 and command text using
   `%i_%s`. Check the actor-local table at this+0x54 without creation; if absent, create the entry
   and dispatch the command via DispatchScriptActorCommandWithContext. Existing keys suppress
   dispatch. The table's exact lifetime/reset policy is not established here.
   
   Parameters: this is the executing ScriptActor; param_1 is the nested command.
   
   Returns: true-like value when this call creates a new key and dispatches; false-like value when
   the key already exists.
   
   Evidence: Fresh decompilation and assembly at this address; parser routes EVS `once` here. */

uint __thiscall ExecuteScriptCommandOncePerContext(void *this,int *param_1)

{
  int *piVar1;
  int *piVar2;
  undefined1 *puVar3;
  int extraout_EAX;
  uint uVar4;
  int iVar5;
  int *piVar6;
  int *local_18;
  void *local_14;
  undefined1 *local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  piVar1 = param_1;
  puStack_8 = &LAB_004d2be0;
  local_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_18 = &DAT_00501908;
  local_4._0_1_ = 1;
  local_4._1_3_ = 0;
  ExceptionList = &local_c;
  local_14 = this;
  FUN_00402770(&local_18,(byte *)"%i_%s");
  piVar2 = local_18;
  local_10 = &stack0xffffffd0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar6 = &DAT_00501908;
  if (local_18 != &DAT_00501908) {
    *local_18 = *local_18 + 1;
    piVar6 = local_18;
  }
  puVar3 = FindScriptVariableEntry((void *)((int)this + 0x54),piVar6,(int *)0x0);
  if (puVar3 == (undefined1 *)0x0) {
    local_10 = &stack0xffffffd0;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar6 = &DAT_00501908;
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + 1;
      piVar6 = piVar2;
    }
    FindScriptVariableEntry((void *)((int)this + 0x54),piVar6,(int *)0x1);
    local_10 = &stack0xffffffd0;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar6 = &DAT_00501908;
    if (piVar1 != &DAT_00501908) {
      *piVar1 = *piVar1 + 1;
      piVar6 = piVar1;
    }
    DispatchScriptActorCommandWithContext(local_14,piVar6,(void *)0x0);
    local_4 = (uint)local_4._1_3_ << 8;
    iVar5 = extraout_EAX;
    if (piVar2 != &DAT_00501908) {
      iVar5 = *piVar2 + -1;
      *piVar2 = iVar5;
      if (iVar5 < 1) {
        iVar5 = FUN_004011c0(&local_18);
      }
    }
    local_4 = 0xffffffff;
    if (piVar1 != &DAT_00501908) {
      iVar5 = *piVar1 + -1;
      *piVar1 = iVar5;
      if (iVar5 < 1) {
        iVar5 = FUN_004011c0(&param_1);
      }
    }
    uVar4 = CONCAT31((int3)((uint)iVar5 >> 8),1);
  }
  else {
    local_4 = (uint)local_4._1_3_ << 8;
    if (piVar2 != &DAT_00501908) {
      puVar3 = (undefined1 *)(*piVar2 + -1);
      *piVar2 = (int)puVar3;
      if ((int)puVar3 < 1) {
        puVar3 = (undefined1 *)FUN_004011c0(&local_18);
      }
    }
    local_4 = 0xffffffff;
    if (piVar1 != &DAT_00501908) {
      puVar3 = (undefined1 *)(*piVar1 + -1);
      *piVar1 = (int)puVar3;
      if ((int)puVar3 < 1) {
        puVar3 = (undefined1 *)FUN_004011c0(&param_1);
      }
    }
    uVar4 = (uint)puVar3 & 0xffffff00;
  }
  ExceptionList = local_c;
  return uVar4;
}
