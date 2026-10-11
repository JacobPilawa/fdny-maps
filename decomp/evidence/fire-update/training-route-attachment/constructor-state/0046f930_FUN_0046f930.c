/* Ghidra decompilation; names/types are provisional.
 * Entry: 0046f930
 * Function: FUN_0046f930
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0046f930(undefined4 *param_1)

{
  *param_1 = &PTR_FUN_004e2d08;
  param_1[2] = 0;
  param_1[3] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  *(undefined1 *)(param_1 + 8) = 1;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1[9] = &DAT_00501908;
  param_1[0xc] = 0;
  param_1[0xd] = 0;
  param_1[0xe] = 0;
  param_1[0xf] = 0;
  param_1[0x11] = 0x10;
  param_1[0x10] = FUN_00432a40;
  param_1[0x13] = 0;
  return;
}
