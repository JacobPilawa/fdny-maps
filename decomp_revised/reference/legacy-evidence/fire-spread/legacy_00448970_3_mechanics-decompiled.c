/* Ghidra decompilation; names/types are provisional.
 * Entry: 00448970
 * Function: FUN_00448970
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00448970(void *this)

{
  float fVar1;
  float in_stack_00000010;
  char cStack00000014;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf8e8;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0x70) != 5) {
    fVar1 = *(float *)((int)this + 0x128) -
            (DAT_00502b04 / *(float *)(*(int *)((int)this + 0x78) + 0x18)) *
            (_DAT_004dfc24 -
            (_DAT_004dfc24 - in_stack_00000010) * (_DAT_004dfc24 - in_stack_00000010));
    ExceptionList = &local_c;
    *(float *)((int)this + 0x128) = fVar1;
    if (fVar1 < _DAT_004dc50c) {
      *(undefined4 *)((int)this + 0x128) = 0;
    }
    if ((cStack00000014 != '\0') && (*(float *)((int)this + 0x128) < (float)_DAT_004df7b8)) {
      *(undefined4 *)((int)this + 0x70) = 5;
      FUN_00402790(&stack0x00000014,"Fires Doused");
      local_4 = 0;
      FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&stack0x00000014,1);
      local_4 = 0xffffffff;
      if (_cStack00000014 != &DAT_00501908) {
        *_cStack00000014 = *_cStack00000014 + -1;
        if (*_cStack00000014 < 1) {
          FUN_004011c0((undefined4 *)&stack0x00000014);
        }
        else {
          _cStack00000014 = &DAT_00501908;
        }
      }
      if (*(int **)((int)this + 0x114) != (int *)0x0) {
        thunk_FUN_0049f340(*(int **)((int)this + 0x114));
      }
      *(undefined1 *)((int)this + 0x1c8) = 0;
    }
    *(undefined1 *)((int)this + 0x104) = 1;
  }
  ExceptionList = local_c;
  return;
}

