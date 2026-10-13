/* Fresh Ghidra decompilation, captured 2026-10-13.
 * Binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Address: 0046E470 (DispatchNestedScriptActorCommand)
 * The pseudocode and listing are analysis output, not recovered original source.
 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Routes one nested ScriptActor command through RouteParsedScriptActorCommand with
   explicit context value 0. The shared `cmd` handler calls this for each child command. Evidence:
   fresh decompilation at 0x0046E470 and common command router 0x0046F9D0. */

undefined4 DispatchNestedScriptActorCommand(int *param_1)

{
  int *piVar1;
  int iVar2;
  void *in_ECX;
  int *piVar3;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar1 = param_1;
  puStack_8 = &LAB_004d25e8;
  local_c = ExceptionList;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  ExceptionList = &local_c;
  piVar3 = &DAT_00501908;
  if (param_1 != &DAT_00501908) {
    ExceptionList = &local_c;
    *param_1 = *param_1 + 1;
    piVar3 = param_1;
  }
  iVar2 = RouteParsedScriptActorCommand(in_ECX,piVar3,0);
  local_4 = 0xffffffff;
  if (piVar1 != &DAT_00501908) {
    iVar2 = *piVar1 + -1;
    *piVar1 = iVar2;
    if (iVar2 < 1) {
      iVar2 = FUN_004011c0(&param_1);
    }
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)iVar2 >> 8),1);
}

