/* Ghidra decompilation; names/types are provisional.
 * Entry: 0045bef0
 * Function: FUN_0045bef0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0045bef0(int param_1)

{
  int *piVar1;
  
  *(undefined4 *)(param_1 + 0x120) = 5;
  FUN_00481140((void *)(param_1 + 0x170),'\0');
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff4,"pickup");
  FUN_004705b0((void *)(param_1 + 0xd4),piVar1);
  return;
}

