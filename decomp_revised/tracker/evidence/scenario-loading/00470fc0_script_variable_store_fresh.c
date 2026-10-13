/* Read-only fresh Ghidra 12.0.4 decompilation from FDNYFirefighter.exe.
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured: 2026-10-13; renamed in Ghidra after static trace.
 */

/* Summary: Stores one script variable value in the shared variable table at DAT_00503044.
   
   Algorithm: Find or create the entry for param_1, then replace its stored string reference with
   param_2 while maintaining reference counts.
   
   Parameters: param_1 is the variable key; param_2 is the replacement value string.
   
   Returns: void.
   
   Evidence: Ghidra decompilation at 0x00470FC0 and callers 0x004750E0 (EVS set) and 0x004758F0 (EVS
   if). */

void StoreScriptVariableValue(undefined4 *param_1,undefined4 *param_2)

{
  undefined4 *puVar1;
  undefined1 *puVar2;
  int *extraout_var;
  int *piVar3;
  
  piVar3 = (int *)0x1;
  FUN_00401cb0(&stack0xffffffec,param_1);
  puVar2 = FindScriptVariableEntry(&DAT_00503044,extraout_var,piVar3);
  puVar1 = (undefined4 *)(puVar2 + 4);
  piVar3 = (int *)*puVar1;
  if (piVar3 != (int *)*param_2) {
    if (piVar3 != &DAT_00501908) {
      *piVar3 = *piVar3 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    piVar3 = (int *)*param_2;
    *puVar1 = piVar3;
    if (piVar3 != &DAT_00501908) {
      *piVar3 = *piVar3 + 1;
    }
  }
  return;
}
