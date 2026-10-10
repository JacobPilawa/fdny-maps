/* Ghidra decompilation; names/types are provisional.
 * Entry: 004513a0
 * Function: FUN_004513a0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __fastcall FUN_004513a0(int *param_1)

{
  undefined4 uVar1;
  
  if (((char)param_1[0x4b] != '\0') && ((float)_DAT_004de6f8 < (float)param_1[0xd14])) {
    param_1[0x14d] = param_1[0x13];
    param_1[0x14e] = param_1[0x14];
    param_1[0x14f] = param_1[0x15];
    param_1[0x14c] = param_1[0xd0d];
  }
  uVar1 = FUN_00451160(param_1);
  return CONCAT31((int3)((uint)uVar1 >> 8),1);
}

