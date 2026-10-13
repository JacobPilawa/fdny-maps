/* Ghidra decompilation; names/types are provisional.
 * Entry: 00448320
 * Function: FUN_00448320
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00448320(void *this,int param_1)

{
  if (*(int *)(*(int *)((int)this + 0x48) + 4) != 0) {
    if ((0x2ee < param_1) || (*(float *)((int)this + 0x120) <= _DAT_004e16b0)) {
      if ((*(float *)((int)this + 0x120) < _DAT_004e16b0 !=
           (*(float *)((int)this + 0x120) == _DAT_004e16b0)) && (0x2ee < param_1)) {
        DAT_00502b90 = DAT_00502b90 + -1;
      }
    }
    else {
      DAT_00502b90 = DAT_00502b90 + 1;
    }
  }
  *(float *)((int)this + 0x120) = (float)param_1;
  if ((float)param_1 < _DAT_004e16b0) {
    FUN_00447a90((int)this);
  }
  return;
}

