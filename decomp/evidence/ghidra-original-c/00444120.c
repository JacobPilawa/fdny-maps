/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00444120
 * Match: function entry
 * Entry: 00444120 FUN_00444120
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00444120(void *this,int param_1)

{
  int *piVar1;
  
  if ((*(int *)((int)this + 0x70) == 1) &&
     (FUN_0044c840(*(void **)(DAT_00503a78 + 0x30),(int *)((int)this + -0x150),param_1),
     param_1 == 0x3ea)) {
    return;
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff4,"use");
  FUN_004705b0((void *)((int)this + -0x7c),piVar1);
  return;
}


