/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00451080
 * Match: function entry
 * Entry: 00451080 FUN_00451080
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00451080(int *param_1)

{
  float fVar1;
  undefined4 auStack_c [3];
  
  fVar1 = (float)(DAT_00502afc - param_1[0xd0b]);
  if (DAT_00502afc - param_1[0xd0b] < 0) {
    fVar1 = fVar1 + (float)_DAT_004dfc90;
  }
  fVar1 = (float)_DAT_004dd808 - fVar1 * (float)_DAT_004e1b18;
  FUN_004a07c0(*(void **)(DAT_00503a78 + 8),2,(int)(DAT_004ff488 * fVar1));
  if (fVar1 < _DAT_004dc50c) {
    FUN_0044ff10(param_1,'\0');
    return;
  }
  FUN_0044f340(param_1,(int *)0x0,'\0',fVar1);
  (**(code **)(*param_1 + 100))();
  FUN_0042d280(param_1 + 0x16e,auStack_c,0,0,0);
  return;
}


