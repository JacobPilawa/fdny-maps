
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   Update the FireRescue actor when its vtable callback runs. State 5 returns without processing.
   For other states, update animation/color and sound state. When the actor's state at +0x4C is 4,
   call AdvanceFireRescuePathMotion with the enclosing entity pointer (receiver - 0x24). The +0xF8
   countdown can early-return ordinary non-state-4 updates; the state-4 branch bypasses that gate.
   The callback's external scheduling interval is not established.
   
   Parameters:
   param_1 is the FireRescue actor subobject used as the vtable receiver.
   
   Returns:
   No return value.
   
   Evidence:
   The function begins at the data-referenced vtable entry 0x004E1774 installed at object +0x24 by
   the constructor at 0x0044A330. At 0x0044A186-0x0044A18F it checks state 4 and calls
   AdvanceFireRescuePathMotion. Fresh pseudocode: tracker/evidence/rescue-moveto/00449FE0_fresh.c.
   EXE SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

void __fastcall UpdateFireRescueActor(int param_1)

{
  char *pcVar1;
  bool bVar2;
  float *pfVar3;
  float *pfVar4;
  void *this;
  ulonglong uVar5;
  undefined4 uVar6;
  float fVar7;
  float fStack_30;
  float fStack_2c;
  float fStack_28;
  float afStack_24 [6];
  float afStack_c [3];
  
  if (*(int *)(param_1 + 0x4c) != 5) {
    if (_DAT_004e1680 <= *(float *)(param_1 + 0x104)) {
      FUN_0044a6e0(param_1 + 0x108);
    }
    else {
      fVar7 = *(float *)(param_1 + 0x104) / _DAT_004e1680;
      fStack_30 = 0.9;
      fStack_2c = 0.3;
      fStack_28 = 0.1;
      afStack_24[0] = 1.0;
      afStack_24[1] = 1.0;
      afStack_24[2] = 1.0;
      pfVar3 = (float *)FUN_00447710(afStack_24 + 3,_DAT_004dfc24 - fVar7,&fStack_30);
      pfVar4 = (float *)FUN_00447710(afStack_c,fVar7,afStack_24);
      pfVar3 = (float *)FUN_0042d050(afStack_24,pfVar4,pfVar3);
      fStack_30 = *pfVar3;
      fStack_2c = pfVar3[1];
      fStack_28 = pfVar3[2];
      FUN_0044a6b0((void *)(param_1 + 0x108),&fStack_30);
    }
    this = (void *)(param_1 + 0x108);
    if ((*(char *)(param_1 + 0xe0) == '\0') && (*(int *)(param_1 + 0x4c) != 4)) {
      pcVar1 = (char *)(param_1 + 0xf8);
      *pcVar1 = *pcVar1 + -1;
      if (*pcVar1 != '\0') {
        FUN_0044b060(this);
        return;
      }
    }
    *(undefined1 *)(param_1 + 0xf8) = 3;
    if ((_DAT_004e0ed4 <= *(float *)(param_1 + 0xfc)) && (*(int **)(param_1 + 0xf0) != (int *)0x0))
    {
      bVar2 = thunk_FUN_0049f260(*(int **)(param_1 + 0xf0));
      if (bVar2) {
        thunk_FUN_0049f340(*(int **)(param_1 + 0xf0));
      }
    }
    if ((*(float *)(param_1 + 0xfc) < _DAT_004e0ed4) && (*(int **)(param_1 + 0xf0) != (int *)0x0)) {
      bVar2 = thunk_FUN_0049f260(*(int **)(param_1 + 0xf0));
      if (!bVar2) {
        FUN_004a11a0(*(void **)(param_1 + 0xf0),(undefined1 *)0x0);
      }
    }
    if ((*(int *)(*(int *)(param_1 + 0x24) + 4) == 0) && (*(int *)(param_1 + 0x194) == 0)) {
      pfVar3 = (float *)FUN_00447850(this,afStack_c);
      if (*pfVar3 < (float)_DAT_004dfc78) {
        *(undefined1 *)(param_1 + -0x14) = 1;
      }
    }
    if (*(int *)(param_1 + 0x4c) == 4) {
      AdvanceFireRescuePathMotion((void *)(param_1 + -0x24));
    }
    FUN_0044b060(this);
    uVar6 = 0x3f800000;
    if (ABS(*(float *)(param_1 + 0x104) - (float)_DAT_004dd808) <= (float)_DAT_004dfa28) {
      fVar7 = 1.0;
    }
    else {
      uVar5 = FUN_004b9400();
      fVar7 = (float)(int)uVar5 * (float)_DAT_004df7b8;
    }
    FUN_0042d280((void *)(param_1 + 0x198),afStack_c,0x3f800000,fVar7,uVar6);
    if (*(char *)(param_1 + 0xe0) == '\0') {
      fVar7 = DAT_00502b04;
      if (*(int *)(param_1 + 0x4c) != 4) {
        fVar7 = DAT_00502b04 * _DAT_004e0510;
      }
      fVar7 = fVar7 * (float)_DAT_004df7b8 + *(float *)(param_1 + 0x104);
      *(float *)(param_1 + 0x104) = fVar7;
      if (_DAT_004dfc24 < fVar7) {
        *(undefined4 *)(param_1 + 0x104) = 0x3f800000;
      }
    }
    if ((float)_DAT_004de6f8 <= *(float *)(param_1 + 0x104)) {
      fVar7 = 1.0;
    }
    else {
      fVar7 = *(float *)(param_1 + 0x104) + *(float *)(param_1 + 0x104);
    }
    FUN_0044a610(this,fVar7);
    *(undefined1 *)(param_1 + 0xe0) = 0;
    *(undefined4 *)(param_1 + 0x94) = 0;
  }
  return;
}


