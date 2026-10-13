
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   If FireRescue mode +0x70 is 5, return. Otherwise compute influence strength as 1-(1-influence)^2,
   subtract frameDelta/duration times that strength from intensity +0x128, store as float, and clamp
   negative values to zero. If finalize is nonzero and stored intensity is strictly below 0.1, set
   mode 5, increment Fires Doused, stop the sound at +0x114 when present, and clear +0x1C8. Mark
   +0x104 recently sprayed after nonterminal or non-doused updates.
   Parameters:
   this is the FireRescue root. contactX/contactY/contactZ occupy ABI stack slots but are not read
   by this routine. influence is the fourth stack argument; finalize is the fifth. Frame delta comes
   from DAT_00502B04; duration is definition +0x18.
   Returns:
   void. */

void __thiscall
ApplyFireSuppressionContact
          (void *this,float contactX,float contactY,float contactZ,float influence,char finalize)

{
  float fVar1;
  undefined3 in_stack_00000015;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf8e8;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0x70) != 5) {
    fVar1 = *(float *)((int)this + 0x128) -
            (DAT_00502b04 / *(float *)(*(int *)((int)this + 0x78) + 0x18)) *
            (_DAT_004dfc24 - (_DAT_004dfc24 - influence) * (_DAT_004dfc24 - influence));
    ExceptionList = &local_c;
    *(float *)((int)this + 0x128) = fVar1;
    if (fVar1 < _DAT_004dc50c) {
      *(undefined4 *)((int)this + 0x128) = 0;
    }
    if ((finalize != '\0') && (*(float *)((int)this + 0x128) < (float)_DAT_004df7b8)) {
      *(undefined4 *)((int)this + 0x70) = 5;
      FUN_00402790(&finalize,"Fires Doused");
      local_4 = 0;
      FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&finalize,1);
      local_4 = 0xffffffff;
      if (_finalize != &DAT_00501908) {
        *_finalize = *_finalize + -1;
        if (*_finalize < 1) {
          FUN_004011c0((undefined4 *)&finalize);
        }
        else {
          _finalize = &DAT_00501908;
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

