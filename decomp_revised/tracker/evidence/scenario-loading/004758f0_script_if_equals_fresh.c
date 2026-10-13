/* Read-only fresh Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13; renamed in Ghidra after static trace.
 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Implements the EVS `if <name> <expected> <command>` condition.
   
   Algorithm: Find or create the named entry in the shared table at DAT_00503044 (new entries
   default to string `0`), compare stored and expected text case-insensitively, and dispatch the
   nested command through DispatchScriptActorCommandWithContext only when equal.
   
   Parameters: this is the executing ScriptActor; param_1 is variable name; param_2 is expected
   value; param_3 is nested command.
   
   Returns: true-like value when equal and false-like value when not equal; the nested command is
   dispatched only on equality.
   
   Evidence: Parser extraction at 0x0047A1F0; fresh decompilation and assembly at this address;
   default creation in FindScriptVariableEntry. */

uint __thiscall ExecuteScriptIfValueMatches(void *this,int *param_1,int *param_2,int *param_3)

{
  int *piVar1;
  int *piVar2;
  undefined1 *puVar3;
  undefined4 *puVar4;
  int iVar5;
  int extraout_EAX;
  uint uVar6;
  int *piVar7;
  int *command_string;
  undefined1 *local_14;
  undefined1 *local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  piVar1 = param_1;
  puStack_8 = &LAB_004d2bb8;
  local_c = ExceptionList;
  local_14 = &stack0xffffffd0;
  local_4 = 2;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  ExceptionList = &local_c;
  piVar7 = &DAT_00501908;
  if (param_1 != &DAT_00501908) {
    ExceptionList = &local_c;
    *param_1 = *param_1 + 1;
    piVar7 = param_1;
  }
  puVar3 = FindScriptVariableEntry(&DAT_00503044,piVar7,(int *)0x1);
  puVar4 = (undefined4 *)FUN_0040c330(puVar3 + 4,&local_14);
  piVar7 = param_2;
  iVar5 = __stricmp((char *)(*(int *)*puVar4 + 0xc),(char *)(param_2 + 3));
  piVar2 = param_3;
  if (iVar5 == 0) {
    local_10 = &stack0xffffffd0;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    command_string = &DAT_00501908;
    if (param_3 != &DAT_00501908) {
      *param_3 = *param_3 + 1;
      command_string = param_3;
    }
    DispatchScriptActorCommandWithContext(this,command_string,(void *)0x0);
    local_4._0_1_ = 1;
    iVar5 = extraout_EAX;
    if (piVar1 != &DAT_00501908) {
      iVar5 = *piVar1 + -1;
      *piVar1 = iVar5;
      if (iVar5 < 1) {
        iVar5 = FUN_004011c0(&param_1);
      }
    }
    local_4 = (uint)local_4._1_3_ << 8;
    if (piVar7 != &DAT_00501908) {
      iVar5 = *piVar7 + -1;
      *piVar7 = iVar5;
      if (iVar5 < 1) {
        iVar5 = FUN_004011c0(&param_2);
      }
    }
    local_4 = 0xffffffff;
    if (piVar2 != &DAT_00501908) {
      iVar5 = *piVar2 + -1;
      *piVar2 = iVar5;
      if (iVar5 < 1) {
        iVar5 = FUN_004011c0(&param_3);
      }
    }
    uVar6 = CONCAT31((int3)((uint)iVar5 >> 8),1);
  }
  else {
    local_4._0_1_ = 1;
    if ((piVar1 != &DAT_00501908) && (iVar5 = *piVar1, *piVar1 = iVar5 + -1, iVar5 + -1 < 1)) {
      FUN_004011c0(&param_1);
    }
    local_4 = (uint)local_4._1_3_ << 8;
    if ((piVar7 != &DAT_00501908) && (iVar5 = *piVar7, *piVar7 = iVar5 + -1, iVar5 + -1 < 1)) {
      FUN_004011c0(&param_2);
    }
    local_4 = 0xffffffff;
    if ((param_3 != &DAT_00501908) && (iVar5 = *param_3, *param_3 = iVar5 + -1, iVar5 + -1 < 1)) {
      param_3 = (int *)FUN_004011c0(&param_3);
    }
    uVar6 = (uint)param_3 & 0xffffff00;
  }
  ExceptionList = local_c;
  return uVar6;
}
