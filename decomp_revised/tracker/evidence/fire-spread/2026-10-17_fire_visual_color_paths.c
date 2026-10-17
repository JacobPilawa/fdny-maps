/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0044a6b0 FUN_0044a6b0 */

void __thiscall FUN_0044a6b0(void *this,undefined4 *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  
  puVar1 = (undefined4 *)((int)this + 0x60);
  iVar2 = 4;
  do {
    FUN_0047f270((void *)*puVar1,param_1);
    puVar1 = puVar1 + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  return;
}



/* 0x0044a6e0 DisableFireRescueSprites */

void __fastcall DisableFireRescueSprites(void *fireVisual)

{
  int *piVar1;
  int iVar2;
  
  piVar1 = (int *)((int)fireVisual + 0x60);
  if (*(char *)(*(int *)((int)fireVisual + 0x60) + 0xcc) != '\0') {
    iVar2 = 4;
    do {
      FUN_0047f1e0(*piVar1);
      piVar1 = piVar1 + 1;
      iVar2 = iVar2 + -1;
    } while (iVar2 != 0);
  }
  return;
}



/* 0x0044a610 ApplyFireRescueSpriteAlpha */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall ApplyFireRescueSpriteAlpha(void *this,float alpha)

{
  undefined4 *puVar1;
  int iVar2;
  float local_4;
  
  if ((float)_DAT_004e17e0 < alpha) {
    alpha = 0.999;
  }
  puVar1 = (undefined4 *)((int)this + 0x60);
  iVar2 = 4;
  do {
    FUN_0047f1f0((void *)*puVar1,alpha);
    puVar1 = puVar1 + 1;
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  local_4 = alpha * (float)_DAT_004e17d8;
  if (_DAT_004dfc24 < local_4) {
    local_4 = 1.0;
  }
  FUN_0047f1f0(*(void **)((int)this + 0x6c),local_4);
  if (*(int *)((int)this + 0x58) != 0) {
    FUN_0043f670();
  }
  return;
}



/* 0x0044b060 UpdateFireRescueVisualEffects */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall UpdateFireRescueVisualEffects(void *fireVisual)

{
  float *this;
  bool bVar1;
  float *pfVar2;
  int iVar3;
  undefined4 *puVar4;
  float fVar5;
  float local_30;
  undefined4 local_2c;
  float local_28;
  float local_24 [3];
  float local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfa6e;
  local_c = ExceptionList;
  if (*(char *)((int)fireVisual + 0x9c) != '\0') {
    ExceptionList = &local_c;
    UpdateFireRescueTimedVectorTrack((int)fireVisual);
    if (*(char *)((int)fireVisual + 0x50) != '\0') {
      if ((_DAT_00502c58 & 1) == 0) {
        _DAT_00502c58 = _DAT_00502c58 | 1;
        local_4 = 0;
        FUN_00481950((undefined4 *)&DAT_00502c40);
        _atexit(FUN_004db740);
        local_4 = 0xffffffff;
      }
      fVar5 = 0.5;
      this = (float *)((int)fireVisual + 0x44);
      pfVar2 = (float *)FUN_0042d2b0(this,local_24);
      pfVar2 = (float *)FUN_00446390(local_18,pfVar2,fVar5);
      _DAT_00502c40 = *pfVar2;
      _DAT_00502c48 = pfVar2[2];
      _DAT_00502c44 = 0;
      pfVar2 = (float *)FUN_00446390(local_18,this,0.5);
      _DAT_00502c40 = _DAT_00502c40 + *(float *)((int)fireVisual + 0x28);
      _DAT_00502c44 = *(undefined4 *)((int)fireVisual + 0x2c);
      _DAT_00502c48 = _DAT_00502c48 + *(float *)((int)fireVisual + 0x30);
      _DAT_00502c4c = *pfVar2 + *(float *)((int)fireVisual + 0x28);
      _DAT_00502c50 = *(float *)((int)fireVisual + 0x48) + *(float *)((int)fireVisual + 0x2c);
      _DAT_00502c54 = pfVar2[2] + *(float *)((int)fireVisual + 0x30);
      FUN_0044a840(fireVisual,(undefined4 *)&DAT_00502c40);
      if (*(int *)((int)fireVisual + 0x58) != 0) {
        FUN_00447890((void *)(*(int *)((int)fireVisual + 0x58) + 0x84),(int)fireVisual);
        local_30 = *this * (float)_DAT_004e17d8;
        local_2c = *(undefined4 *)((int)fireVisual + 0x54);
        local_28 = *(float *)((int)fireVisual + 0x4c) * (float)_DAT_004e17d8;
        FUN_00458da0(*(void **)((int)fireVisual + 0x58),&local_30);
      }
      *(undefined1 *)((int)fireVisual + 0x50) = 0;
    }
    if (((float)_DAT_004dfc78 <
         *(float *)((int)fireVisual + 0x4c) * *(float *)((int)fireVisual + 0x4c) +
         *(float *)((int)fireVisual + 0x48) * *(float *)((int)fireVisual + 0x48) +
         *(float *)((int)fireVisual + 0x44) * *(float *)((int)fireVisual + 0x44)) &&
       (bVar1 = FUN_0048fe20(*(int *)((int)fireVisual + 0x5c),(undefined4 *)((int)fireVisual + 0x28)
                            ), bVar1)) {
      puVar4 = (undefined4 *)((int)fireVisual + 0x6c);
      iVar3 = 4;
      do {
        FUN_0047f960((void *)*puVar4,*(void **)((int)fireVisual + 0x5c));
        puVar4 = puVar4 + -1;
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
    }
    if (*(int *)((int)fireVisual + 0x58) != 0) {
      FUN_00458d90(*(int *)((int)fireVisual + 0x58));
    }
  }
  ExceptionList = local_c;
  return;
}



/* 0x0044b270 FUN_0044b270 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0044b270(void *this,void *param_1,char param_2)

{
  int iVar1;
  void *pvVar2;
  int iVar3;
  int *piVar4;
  float10 fVar5;
  ulonglong uVar6;
  undefined4 local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfaa1;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0044ad70((int)this);
  *(undefined1 *)((int)this + 0x9c) = 1;
  *(undefined4 *)((int)this + 0x88) = 0;
  *(undefined4 *)((int)this + 0x8c) = 0;
  FUN_0042d280((void *)((int)this + 0x90),local_18,0x3f800000,0x3f800000,0x3f800000);
  if ((param_2 != '\0') && (*(int *)((int)this + 0x58) == 0)) {
    pvVar2 = operator_new(0xcc);
    local_4 = 0;
    if (pvVar2 == (void *)0x0) {
      pvVar2 = (void *)0x0;
    }
    else {
      pvVar2 = FUN_00458e40(pvVar2,param_1);
    }
    local_4 = 0xffffffff;
    *(void **)((int)this + 0x58) = pvVar2;
    fVar5 = ScalePseudoRandomFloatToRange(0.8,1.2);
    *(float *)((int)this + 0x54) = (float)(fVar5 * (float10)_DAT_004e1810);
  }
  if (*(int *)((int)this + 0x5c) != 0) {
    *(undefined1 *)(*(int *)((int)this + 0x60) + 0x30) = 1;
    *(undefined1 *)(*(int *)((int)this + 100) + 0x30) = 1;
    *(undefined1 *)(*(int *)((int)this + 0x68) + 0x30) = 1;
    *(undefined1 *)(*(int *)((int)this + 0x6c) + 0x30) = 1;
    ExceptionList = local_c;
    return;
  }
  *(void **)((int)this + 0x5c) = param_1;
  iVar3 = 3;
  piVar4 = (int *)((int)this + 0x60);
  do {
    pvVar2 = operator_new(0x168);
    local_4 = 1;
    if (pvVar2 == (void *)0x0) {
      pvVar2 = (void *)0x0;
    }
    else {
      pvVar2 = FUN_0047f850(pvVar2,0x502d08,0);
    }
    *piVar4 = (int)pvVar2;
    *(undefined1 *)((int)pvVar2 + 0xad) = 1;
    *(undefined4 *)(*piVar4 + 0xb0) = 4;
    iVar1 = *piVar4;
    local_4 = 0xffffffff;
    fVar5 = ScalePseudoRandomFloatToRange(0.5,1.5);
    *(float *)(iVar1 + 0x1c) = (float)(fVar5 * (float10)*(float *)(iVar1 + 0x1c));
    ScalePseudoRandomFloatToRange(0.0,(float)(DAT_00502d0c + -1));
    uVar6 = ConvertX87FloatToIntegerTowardZero();
    FUN_0047d300((void *)*piVar4,(int)uVar6);
    *(undefined4 *)(*piVar4 + 0xa4) = 0xa0;
    iVar3 = iVar3 + -1;
    *(float *)(*piVar4 + 0xe8) = *(float *)(*piVar4 + 0xe8) + (float)_DAT_004dfc78;
    *(float *)(*piVar4 + 0x10c) = *(float *)(*piVar4 + 0x10c) + (float)_DAT_004dfc78;
    piVar4 = piVar4 + 1;
  } while (iVar3 != 0);
  pvVar2 = operator_new(0x168);
  local_4 = 2;
  if (pvVar2 == (void *)0x0) {
    pvVar2 = (void *)0x0;
  }
  else {
    pvVar2 = FUN_0047f850(pvVar2,0x502cd0,0);
  }
  *(void **)((int)this + 0x6c) = pvVar2;
  *(undefined1 *)((int)pvVar2 + 0xad) = 1;
  *(undefined4 *)(*(int *)((int)this + 0x6c) + 0xb0) = 4;
  iVar3 = *(int *)((int)this + 0x6c);
  local_4 = 0xffffffff;
  fVar5 = ScalePseudoRandomFloatToRange(0.5,1.5);
  *(float *)(iVar3 + 0x1c) = (float)(fVar5 * (float10)*(float *)(iVar3 + 0x1c));
  ScalePseudoRandomFloatToRange(0.0,(float)(DAT_00502d0c + -1));
  uVar6 = ConvertX87FloatToIntegerTowardZero();
  FUN_0047d300(*(void **)((int)this + 0x6c),(int)uVar6);
  *(undefined4 *)(*(int *)((int)this + 0x6c) + 0xa4) = 0xa0;
  *(float *)(*(int *)((int)this + 0x6c) + 0xe8) =
       *(float *)(*(int *)((int)this + 0x6c) + 0xe8) + (float)_DAT_004dfc78;
  *(float *)(*(int *)((int)this + 0x6c) + 0x10c) =
       *(float *)(*(int *)((int)this + 0x6c) + 0x10c) + (float)_DAT_004dfc78;
  ExceptionList = local_c;
  return;
}



/* 0x00449fe0 UpdateFireRescueActor */

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

void __fastcall UpdateFireRescueActor(void *actorSubobject)

{
  char *pcVar1;
  bool bVar2;
  float *pfVar3;
  float *pfVar4;
  void *this;
  ulonglong uVar5;
  undefined4 uVar6;
  float fVar7;
  float local_30;
  float local_2c;
  float local_28;
  float local_24 [6];
  float local_c [3];
  
  if (*(int *)((int)actorSubobject + 0x4c) != 5) {
    if (_DAT_004e1680 <= *(float *)((int)actorSubobject + 0x104)) {
      DisableFireRescueSprites((void *)((int)actorSubobject + 0x108));
    }
    else {
      fVar7 = *(float *)((int)actorSubobject + 0x104) / _DAT_004e1680;
      local_30 = 0.9;
      local_2c = 0.3;
      local_28 = 0.1;
      local_24[0] = 1.0;
      local_24[1] = 1.0;
      local_24[2] = 1.0;
      pfVar3 = (float *)FUN_00447710(local_24 + 3,_DAT_004dfc24 - fVar7,&local_30);
      pfVar4 = (float *)FUN_00447710(local_c,fVar7,local_24);
      pfVar3 = (float *)FUN_0042d050(local_24,pfVar4,pfVar3);
      local_30 = *pfVar3;
      local_2c = pfVar3[1];
      local_28 = pfVar3[2];
      FUN_0044a6b0((void *)((int)actorSubobject + 0x108),&local_30);
    }
    this = (void *)((int)actorSubobject + 0x108);
    if ((*(char *)((int)actorSubobject + 0xe0) == '\0') &&
       (*(int *)((int)actorSubobject + 0x4c) != 4)) {
      pcVar1 = (char *)((int)actorSubobject + 0xf8);
      *pcVar1 = *pcVar1 + -1;
      if (*pcVar1 != '\0') {
        UpdateFireRescueVisualEffects(this);
        return;
      }
    }
    *(undefined1 *)((int)actorSubobject + 0xf8) = 3;
    if ((_DAT_004e0ed4 <= *(float *)((int)actorSubobject + 0xfc)) &&
       (*(int **)((int)actorSubobject + 0xf0) != (int *)0x0)) {
      bVar2 = IsFMODChannelPlaying(*(int **)((int)actorSubobject + 0xf0));
      if (bVar2) {
        thunk_FUN_0049f340(*(int **)((int)actorSubobject + 0xf0));
      }
    }
    if ((*(float *)((int)actorSubobject + 0xfc) < _DAT_004e0ed4) &&
       (*(int **)((int)actorSubobject + 0xf0) != (int *)0x0)) {
      bVar2 = IsFMODChannelPlaying(*(int **)((int)actorSubobject + 0xf0));
      if (!bVar2) {
        RequestSoundInstancePlayback(*(void **)((int)actorSubobject + 0xf0),(undefined1 *)0x0);
      }
    }
    if ((*(int *)(*(int *)((int)actorSubobject + 0x24) + 4) == 0) &&
       (*(int *)((int)actorSubobject + 0x194) == 0)) {
      pfVar3 = (float *)FUN_00447850(this,local_c);
      if (*pfVar3 < (float)_DAT_004dfc78) {
        *(undefined1 *)((int)actorSubobject + -0x14) = 1;
      }
    }
    if (*(int *)((int)actorSubobject + 0x4c) == 4) {
      AdvanceFireRescuePathMotion((void *)((int)actorSubobject + -0x24));
    }
    UpdateFireRescueVisualEffects(this);
    uVar6 = 0x3f800000;
    if (ABS(*(float *)((int)actorSubobject + 0x104) - (float)_DAT_004dd808) <= (float)_DAT_004dfa28)
    {
      fVar7 = 1.0;
    }
    else {
      uVar5 = ConvertX87FloatToIntegerTowardZero();
      fVar7 = (float)(int)uVar5 * (float)_DAT_004df7b8;
    }
    FUN_0042d280((void *)((int)actorSubobject + 0x198),local_c,0x3f800000,fVar7,uVar6);
    if (*(char *)((int)actorSubobject + 0xe0) == '\0') {
      fVar7 = DAT_00502b04;
      if (*(int *)((int)actorSubobject + 0x4c) != 4) {
        fVar7 = DAT_00502b04 * _DAT_004e0510;
      }
      fVar7 = fVar7 * (float)_DAT_004df7b8 + *(float *)((int)actorSubobject + 0x104);
      *(float *)((int)actorSubobject + 0x104) = fVar7;
      if (_DAT_004dfc24 < fVar7) {
        *(undefined4 *)((int)actorSubobject + 0x104) = 0x3f800000;
      }
    }
    if ((float)_DAT_004de6f8 <= *(float *)((int)actorSubobject + 0x104)) {
      fVar7 = 1.0;
    }
    else {
      fVar7 = *(float *)((int)actorSubobject + 0x104) + *(float *)((int)actorSubobject + 0x104);
    }
    ApplyFireRescueSpriteAlpha(this,fVar7);
    *(undefined1 *)((int)actorSubobject + 0xe0) = 0;
    *(undefined4 *)((int)actorSubobject + 0x94) = 0;
  }
  return;
}



