/* Read-only fresh Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13; renamed in Ghidra after static trace.
 */

/* Summary: Finds a script-variable entry by key, with optional creation.
   
   Algorithm: Search the table. If absent and creation is disabled, return null. If creation is
   enabled, insert a new entry with default value string `0` and return its entry/value-holder
   pointer.
   
   Parameters: this is a variable table; param_1 is the key; param_2 is the create flag.
   
   Returns: entry/value-holder pointer, or null when absent and creation is disabled.
   
   Evidence: Fresh decompilation at this address; shared global table use at DAT_00503044 and
   ScriptActor-local table use at this+0x54. */

undefined1 * __thiscall FindScriptVariableEntry(void *this,int *param_1,int *param_2)

{
  byte *pbVar1;
  int *piVar2;
  int iVar3;
  undefined1 *puVar4;
  undefined4 *puVar5;
  undefined4 local_14 [2];
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  piVar2 = param_1;
  puStack_8 = &LAB_004d3378;
  local_c = ExceptionList;
  pbVar1 = (byte *)(param_1 + 3);
  local_4 = 0;
  ExceptionList = &local_c;
  iVar3 = FUN_0047c990(this,pbVar1);
  if ((iVar3 == 0) || (puVar4 = (undefined1 *)(iVar3 + 0xc), puVar4 == (undefined1 *)0x0)) {
    if ((char)param_2 == '\0') {
      local_4 = 0xffffffff;
      if ((piVar2 != &DAT_00501908) && (iVar3 = *piVar2, *piVar2 = iVar3 + -1, iVar3 + -1 < 1)) {
        FUN_004011c0(&param_1);
      }
      puVar4 = (undefined1 *)0x0;
    }
    else {
      FUN_00402790(&param_2,"0");
      local_4._0_1_ = 1;
      puVar5 = FUN_0047cad0(local_14,&param_1,&param_2);
      local_4._0_1_ = 2;
      puVar4 = FUN_0047cf30(this,pbVar1,puVar5);
      local_4._0_1_ = 1;
      FUN_0043c310(local_14);
      local_4 = (uint)local_4._1_3_ << 8;
      if ((param_2 != &DAT_00501908) && (iVar3 = *param_2, *param_2 = iVar3 + -1, iVar3 + -1 < 1)) {
        FUN_004011c0(&param_2);
      }
      local_4 = 0xffffffff;
      if ((piVar2 != &DAT_00501908) && (iVar3 = *piVar2, *piVar2 = iVar3 + -1, iVar3 + -1 < 1)) {
        FUN_004011c0(&param_1);
      }
    }
  }
  else {
    local_4 = 0xffffffff;
    if ((piVar2 != &DAT_00501908) && (iVar3 = *piVar2, *piVar2 = iVar3 + -1, iVar3 + -1 < 1)) {
      FUN_004011c0(&param_1);
    }
  }
  ExceptionList = local_c;
  return puVar4;
}
