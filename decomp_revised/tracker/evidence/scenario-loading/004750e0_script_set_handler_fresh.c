/* Read-only fresh Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13; renamed in Ghidra after static trace.
 */

/* Summary: Handles the EVS `set <name> <value>` command after parser token extraction.
   
   Algorithm: Forward both strings to StoreScriptVariableValue, then release the wrapper's retained
   references.
   
   Parameters: param_1 is the variable name; param_2 is the value string.
   
   Returns: truth-like success value (1 in the observed path).
   
   Evidence: Script command parser at 0x0047A1F0 routes `set` here; fresh decompilation at this
   address. */

undefined4 HandleScriptSetCommand(int *param_1,int *param_2)

{
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;
  
  puStack_8 = &LAB_004d2b60;
  local_c = ExceptionList;
  local_4 = 1;
  ExceptionList = &local_c;
  StoreScriptVariableValue(&param_1,&param_2);
  local_4 = local_4 & 0xffffff00;
  if (param_1 != &DAT_00501908) {
    *param_1 = *param_1 + -1;
    if (*param_1 < 1) {
      FUN_004011c0(&param_1);
    }
    else {
      param_1 = &DAT_00501908;
    }
  }
  local_4 = 0xffffffff;
  if (param_2 != &DAT_00501908) {
    *param_2 = *param_2 + -1;
    if (*param_2 < 1) {
      param_2 = (int *)FUN_004011c0(&param_2);
    }
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)param_2 >> 8),1);
}
