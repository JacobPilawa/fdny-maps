/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0047f270 FUN_0047f270 */

void __thiscall FUN_0047f270(void *this,undefined4 *param_1)

{
  *(undefined4 *)((int)this + 0xc0) = *param_1;
  *(undefined4 *)((int)this + 0xc4) = param_1[1];
  *(undefined4 *)((int)this + 200) = param_1[2];
  *(undefined1 *)((int)this + 0xcc) = 1;
  return;
}



/* 0x0047f1f0 FUN_0047f1f0 */

void __thiscall FUN_0047f1f0(void *this,undefined4 param_1)

{
  *(undefined4 *)((int)this + 0xd0) = param_1;
  *(undefined1 *)((int)this + 0xd4) = 1;
  return;
}



/* 0x0047f1e0 FUN_0047f1e0 */

void __fastcall FUN_0047f1e0(int param_1)

{
  *(undefined1 *)(param_1 + 0xcc) = 0;
  return;
}



/* 0x0047f960 FUN_0047f960 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0047f960(void *this,void *param_1)

{
  float fVar1;
  undefined4 uVar2;
  float fVar3;
  float fVar4;
  float *pfVar5;
  int iVar6;
  int iVar7;
  float10 fVar8;
  ulonglong uVar9;
  byte local_1a;
  byte local_19;
  float local_18;
  undefined4 local_14;
  float local_10;
  float local_c;
  float local_8;
  float local_4;
  
  if (*(char *)((int)this + 0xad) != '\0') {
    FUN_0047d660(this);
  }
  if (*(char *)((int)this + 0xa0) != '\0') {
    FUN_0047f2a0((int)this);
  }
  if (*(char *)((int)this + 0x30) != '\0') {
    fVar1 = *(float *)((int)this + 0xb0);
    if (fVar1 == 0.0) {
      local_14 = 2;
      *(undefined4 *)((int)this + 0xd8) = *(undefined4 *)((int)this + 0x5c);
      *(undefined4 *)((int)this + 0xdc) = *(undefined4 *)((int)this + 0x60);
      *(undefined4 *)((int)this + 0xe0) = *(undefined4 *)((int)this + 100);
      local_18 = 1.4013e-45;
    }
    else if (fVar1 == 1.4013e-45) {
      local_14 = 0;
      if (*(char *)((int)this + 0x78) != '\0') {
        *(undefined1 *)((int)this + 0x78) = 0;
      }
      local_18 = 5.60519e-45;
    }
    else if (fVar1 == 2.8026e-45) {
      local_14 = 0;
      FUN_0047f5a0((int)this);
      local_18 = 5.60519e-45;
    }
    else if (fVar1 == 4.2039e-45) {
      local_14 = 0;
      local_18 = fVar1;
    }
    else if (fVar1 == 5.60519e-45) {
      local_14 = 0;
      local_18 = fVar1;
    }
    fVar1 = local_18;
    if (*(char *)((int)this + 0xac) != '\0') {
      iVar6 = 0;
      local_8 = 0.0;
      local_4 = 0.0;
      local_c = _DAT_004dc50c;
      if (3 < (int)local_18) {
        iVar7 = 3;
        pfVar5 = (float *)((int)this + 0xdc);
        do {
          iVar7 = iVar7 + 4;
          iVar6 = iVar6 + 4;
          local_c = local_c + pfVar5[-1] + pfVar5[8] + pfVar5[0x11] + pfVar5[0x1a];
          local_8 = local_8 + *pfVar5 + pfVar5[9] + pfVar5[0x12] + pfVar5[0x1b];
          local_4 = local_4 + pfVar5[1] + pfVar5[10] + pfVar5[0x13] + pfVar5[0x1c];
          pfVar5 = pfVar5 + 0x24;
        } while (iVar7 < (int)local_18);
      }
      if (iVar6 < (int)local_18) {
        iVar7 = (int)local_18 - iVar6;
        pfVar5 = (float *)((int)this + iVar6 * 0x24 + 0xdc);
        do {
          local_c = local_c + pfVar5[-1];
          iVar7 = iVar7 + -1;
          local_8 = local_8 + *pfVar5;
          local_4 = local_4 + pfVar5[1];
          pfVar5 = pfVar5 + 9;
        } while (iVar7 != 0);
      }
      iVar6 = (int)param_1 + 0xb4;
      local_18 = (float)_DAT_004dd808 / (float)(int)local_18;
      local_c = local_18 * local_c;
      local_8 = local_18 * local_8;
      local_4 = local_18 * local_4;
      if (*(char *)((int)param_1 + 0x160) == '\0') {
        iVar6 = (int)param_1 + 8;
      }
      geWorld_GetLeaf(*(undefined4 *)(iVar6 + 0x78),&local_c,&local_18);
      iVar6 = (int)param_1 + 0xb4;
      if (*(char *)((int)param_1 + 0x160) == '\0') {
        iVar6 = (int)param_1 + 8;
      }
      iVar6 = geWorld_MightSeeLeaf(*(undefined4 *)(iVar6 + 0x78),local_18);
      if (iVar6 == 0) {
        return;
      }
    }
    FUN_0047d6d0(this,&local_1a,&local_19);
    pfVar5 = (float *)FUN_0047d420((int)this);
    local_c = *pfVar5;
    local_8 = pfVar5[1];
    local_4 = pfVar5[2];
    if (*(char *)((int)this + 0xcc) != '\0') {
      local_c = *(float *)((int)this + 0xc0);
      local_8 = *(float *)((int)this + 0xc4);
      local_4 = *(float *)((int)this + 200);
    }
    if (*(char *)((int)this + 0xd4) != '\0') {
      uVar9 = ConvertX87FloatToIntegerTowardZero();
      local_1a = (byte)uVar9;
    }
    fVar3 = local_c * _DAT_004e3524;
    *(float *)((int)this + 0xec) = fVar3;
    fVar4 = local_8 * _DAT_004e3524;
    *(float *)((int)this + 0xf0) = fVar4;
    local_10 = local_4 * _DAT_004e3524;
    *(float *)((int)this + 0xf4) = local_10;
    local_18 = (float)local_1a;
    *(float *)((int)this + 0x160) = local_10;
    *(float *)((int)this + 0xf8) = local_18;
    *(float *)((int)this + 0x164) = local_18;
    *(float *)((int)this + 0x110) = fVar3;
    *(float *)((int)this + 0x114) = fVar4;
    *(float *)((int)this + 0x118) = local_10;
    *(float *)((int)this + 0x11c) = local_18;
    *(float *)((int)this + 0x134) = fVar3;
    *(float *)((int)this + 0x138) = fVar4;
    *(float *)((int)this + 0x13c) = local_10;
    *(float *)((int)this + 0x140) = local_18;
    *(float *)((int)this + 0x158) = fVar3;
    *(float *)((int)this + 0x15c) = fVar4;
    local_18 = (float)FUN_0047d820(this,param_1);
    iVar6 = (int)param_1 + 0xb4;
    if (*(char *)((int)param_1 + 0x160) == '\0') {
      iVar6 = (int)param_1 + 8;
    }
    uVar2 = *(undefined4 *)(iVar6 + 0x78);
    fVar8 = FUN_0047d8a0((int)this);
    geWorld_AddPolyOnce(uVar2,(int)this + 0xd8,fVar1,local_18,local_14,
                        *(undefined4 *)((int)this + 0xa4),
                        (float)(fVar8 * (float10)*(float *)((int)this + 0xa8)));
    if ((*(byte *)((int)this + 0x18) & 2) != 0) {
      iVar6 = FUN_0047d3e0((int)this);
      iVar7 = FUN_0047d400((int)this);
      if (iVar6 != iVar7) {
        pfVar5 = (float *)FUN_0047d560((int)this);
        local_c = *pfVar5;
        local_8 = pfVar5[1];
        local_4 = pfVar5[2];
        if (*(char *)((int)this + 0xcc) != '\0') {
          local_c = *(float *)((int)this + 0xc0);
          local_8 = *(float *)((int)this + 0xc4);
          local_4 = *(float *)((int)this + 200);
        }
        fVar3 = local_c * _DAT_004e3524;
        *(float *)((int)this + 0xec) = fVar3;
        fVar4 = local_8 * _DAT_004e3524;
        *(float *)((int)this + 0xf0) = fVar4;
        local_18 = local_4 * _DAT_004e3524;
        *(float *)((int)this + 0xf4) = local_18;
        *(float *)((int)this + 0x160) = local_18;
        local_10 = (float)local_19;
        *(float *)((int)this + 0xf8) = local_10;
        *(float *)((int)this + 0x164) = local_10;
        *(float *)((int)this + 0x110) = fVar3;
        *(float *)((int)this + 0x114) = fVar4;
        *(float *)((int)this + 0x118) = local_18;
        *(float *)((int)this + 0x11c) = local_10;
        *(float *)((int)this + 0x134) = fVar3;
        *(float *)((int)this + 0x138) = fVar4;
        *(float *)((int)this + 0x13c) = local_18;
        *(float *)((int)this + 0x140) = local_10;
        *(float *)((int)this + 0x158) = fVar3;
        *(float *)((int)this + 0x15c) = fVar4;
        local_18 = (float)FUN_0047d860(this,param_1);
        iVar6 = (int)param_1 + 0xb4;
        if (*(char *)((int)param_1 + 0x160) == '\0') {
          iVar6 = (int)param_1 + 8;
        }
        uVar2 = *(undefined4 *)(iVar6 + 0x78);
        fVar8 = FUN_0047d960((int)this);
        geWorld_AddPolyOnce(uVar2,(int)this + 0xd8,fVar1,local_18,local_14,
                            *(undefined4 *)((int)this + 0xa4),
                            (float)(fVar8 * (float10)*(float *)((int)this + 0xa8)));
      }
    }
  }
  return;
}



/* 0x0047f850 FUN_0047f850 */

void * __thiscall FUN_0047f850(void *this,int param_1,undefined1 param_2)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3498;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0047d370(this,param_1,param_2);
  local_4 = 0;
  FUN_0042d2d0((undefined4 *)((int)this + 0x34),0);
  *(undefined ***)this = &PTR_FUN_004e3548;
  *(undefined4 *)((int)this + 0x34) = &PTR_FUN_004dfc30;
  *(undefined4 *)((int)this + 0x7c) = 0;
  *(undefined4 *)((int)this + 0x80) = 0;
  *(undefined4 *)((int)this + 0x84) = 0;
  *(undefined4 *)((int)this + 0x88) = 0;
  *(undefined4 *)((int)this + 0x8c) = 0;
  *(undefined4 *)((int)this + 0x90) = 0;
  *(undefined4 *)((int)this + 0x94) = 0;
  *(undefined4 *)((int)this + 0x98) = 0;
  *(undefined4 *)((int)this + 0x9c) = 0;
  *(undefined4 *)((int)this + 0xb4) = 0;
  *(undefined4 *)((int)this + 0xb8) = 0;
  *(undefined4 *)((int)this + 0xbc) = 0;
  *(undefined4 *)((int)this + 0xc0) = 0;
  *(undefined4 *)((int)this + 0xc4) = 0;
  *(undefined4 *)((int)this + 200) = 0;
  *(undefined4 *)((int)this + 0xb0) = 0;
  FUN_0047f500(this);
  *(undefined4 *)((int)this + 0x108) = 0x3f7d70a4;
  *(undefined4 *)((int)this + 300) = 0x3f7d70a4;
  *(undefined4 *)((int)this + 0x130) = 0x3f7d70a4;
  *(undefined4 *)((int)this + 0x154) = 0x3f7d70a4;
  *(undefined4 *)((int)this + 0xe4) = 0;
  *(undefined4 *)((int)this + 0xe8) = 0;
  *(undefined4 *)((int)this + 0x10c) = 0;
  *(undefined4 *)((int)this + 0x150) = 0;
  *(undefined1 *)((int)this + 0xac) = 0;
  ExceptionList = local_c;
  return this;
}



/* 0x0044a840 FUN_0044a840 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0044a840(void *this,undefined4 *param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float *pfVar7;
  int iVar8;
  float *pfVar9;
  float *pfVar10;
  float10 fVar11;
  float10 fVar12;
  float10 fVar13;
  float10 fVar14;
  float10 fVar15;
  float10 fVar16;
  float10 fVar17;
  float10 fVar18;
  double local_54;
  float local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  float local_18 [3];
  float local_c [3];
  
  fVar11 = (float10)fcos((float10)_DAT_004e1808);
  iVar8 = 6;
  fVar12 = (float10)fsin((float10)_DAT_004e1808);
  fVar13 = (float10)fcos((float10)_DAT_004e1800);
  fVar14 = (float10)fsin((float10)_DAT_004e1800);
  fVar15 = (float10)fcos((float10)_DAT_004e17f8);
  fVar16 = (float10)fsin((float10)_DAT_004e17f8);
  fVar17 = (float10)fcos((float10)_DAT_004e17f0);
  fVar18 = (float10)fsin((float10)_DAT_004e17f0);
  pfVar9 = (float *)&stack0xffffff58;
  do {
    pfVar10 = pfVar9 + 3;
    *pfVar10 = *(float *)((int)this + 0x44) * *pfVar10 * (float)_DAT_004e17e8;
    pfVar9[5] = pfVar9[5] * *(float *)((int)this + 0x4c) * (float)_DAT_004e17e8;
    pfVar7 = (float *)FUN_00495d20((float *)&local_54,(int)this,pfVar10);
    *pfVar10 = *pfVar7;
    pfVar9[4] = pfVar7[1];
    iVar8 = iVar8 + -1;
    pfVar9[5] = pfVar7[2];
    pfVar9 = pfVar10;
  } while (iVar8 != 0);
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x144) = 0x3f800000;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x148) = 0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x14c) = 0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x120) = 0xbf800000;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x124) = 0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x128) = 0;
  *(float *)(*(int *)((int)this + 100) + 0x144) = (float)fVar11;
  *(undefined4 *)(*(int *)((int)this + 100) + 0x148) = 0;
  *(float *)(*(int *)((int)this + 100) + 0x14c) = (float)fVar12;
  *(float *)(*(int *)((int)this + 100) + 0x120) = (float)fVar13;
  *(undefined4 *)(*(int *)((int)this + 100) + 0x124) = 0;
  *(float *)(*(int *)((int)this + 100) + 0x128) = (float)fVar14;
  *(float *)(*(int *)((int)this + 0x68) + 0x144) = (float)fVar15;
  *(undefined4 *)(*(int *)((int)this + 0x68) + 0x148) = 0;
  *(float *)(*(int *)((int)this + 0x68) + 0x14c) = (float)fVar16;
  *(float *)(*(int *)((int)this + 0x68) + 0x120) = (float)fVar17;
  *(undefined4 *)(*(int *)((int)this + 0x68) + 0x124) = 0;
  *(float *)(*(int *)((int)this + 0x68) + 0x128) = (float)fVar18;
  fVar1 = *(float *)((int)this + 0x48);
  fVar2 = *(float *)((int)this + 0x48);
  fVar3 = *(float *)((int)this + 0x48);
  fVar4 = *(float *)((int)this + 0x48);
  fVar5 = *(float *)((int)this + 0x48);
  fVar6 = *(float *)((int)this + 0x48);
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0xd8) = 0x3f800000;
  *(float *)(*(int *)((int)this + 0x60) + 0xdc) = fVar1 + 0.0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0xe0) = 0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0xfc) = 0xbf800000;
  *(float *)(*(int *)((int)this + 0x60) + 0x100) = fVar2 + 0.0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x104) = 0;
  *(float *)(*(int *)((int)this + 100) + 0xd8) = (float)fVar11;
  *(float *)(*(int *)((int)this + 100) + 0xdc) = fVar3 + 0.0;
  *(float *)(*(int *)((int)this + 100) + 0xe0) = (float)fVar12;
  *(float *)(*(int *)((int)this + 100) + 0xfc) = (float)fVar13;
  *(float *)(*(int *)((int)this + 100) + 0x100) = fVar4 + 0.0;
  *(float *)(*(int *)((int)this + 100) + 0x104) = (float)fVar14;
  *(float *)(*(int *)((int)this + 0x68) + 0xd8) = (float)fVar15;
  *(float *)(*(int *)((int)this + 0x68) + 0xdc) = fVar5 + 0.0;
  *(float *)(*(int *)((int)this + 0x68) + 0xe0) = (float)fVar16;
  *(float *)(*(int *)((int)this + 0x68) + 0xfc) = (float)fVar17;
  *(float *)(*(int *)((int)this + 0x68) + 0x100) = fVar6 + 0.0;
  *(float *)(*(int *)((int)this + 0x68) + 0x104) = (float)fVar18;
  local_48 = (float)param_1[3];
  local_44 = param_1[1];
  local_40 = param_1[5];
  local_3c = param_1[3];
  local_38 = param_1[1];
  local_34 = param_1[2];
  local_30 = *param_1;
  local_2c = param_1[1];
  local_28 = param_1[2];
  local_24 = *param_1;
  local_20 = param_1[1];
  local_1c = param_1[5];
  if ((DAT_00502c3c & 1) == 0) {
    DAT_00502c3c = DAT_00502c3c | 1;
    _DAT_00502c30 = 0.0;
    DAT_00502c34 = 0.0;
    _DAT_00502c38 = 0.0;
    _atexit(FUN_004db730);
  }
  local_54._0_4_ = 1e+12;
  iVar8 = 0;
  pfVar9 = &local_48;
  do {
    pfVar7 = (float *)FUN_0043b2d0(local_c,pfVar9,(float *)((int)this + 0x28));
    pfVar7 = (float *)FUN_00495eb0(local_18,(int)this,pfVar7);
    _DAT_00502c30 = *pfVar7;
    DAT_00502c34 = pfVar7[1];
    _DAT_00502c38 = pfVar7[2];
    *(float *)(*(int *)((int)this + 0x6c) + 0xd8 + iVar8) =
         _DAT_00502c30 + *(float *)((int)this + 0x28);
    *(float *)(*(int *)((int)this + 0x6c) + 0xdc + iVar8) =
         DAT_00502c34 + *(float *)((int)this + 0x2c);
    *(float *)(*(int *)((int)this + 0x6c) + 0xe0 + iVar8) =
         _DAT_00502c38 + *(float *)((int)this + 0x30);
    if (DAT_00502c34 < local_54._0_4_) {
      local_54._0_4_ = DAT_00502c34;
    }
    iVar8 = iVar8 + 0x24;
    pfVar9 = pfVar9 + 3;
  } while (iVar8 < 0x90);
  if (local_54._0_4_ < _DAT_004dc50c) {
    fVar1 = local_54._0_4_ + local_54._0_4_;
    *(float *)(*(int *)((int)this + 0x60) + 0xb8) =
         *(float *)(*(int *)((int)this + 0x60) + 0xb8) - fVar1;
    *(float *)(*(int *)((int)this + 100) + 0xb8) =
         *(float *)(*(int *)((int)this + 100) + 0xb8) - fVar1;
    *(float *)(*(int *)((int)this + 0x68) + 0xb8) =
         *(float *)(*(int *)((int)this + 0x68) + 0xb8) - fVar1;
    iVar8 = FUN_0047f1a0(*(int *)((int)this + 0x60));
    fVar1 = local_54._0_4_ * (float)_DAT_004de6f8;
    local_54 = (double)fVar1;
    *(float *)(iVar8 + 0x2c) = *(float *)(iVar8 + 0x2c) + fVar1;
    iVar8 = FUN_0047f1a0(*(int *)((int)this + 100));
    *(float *)(iVar8 + 0x2c) = *(float *)(iVar8 + 0x2c) + (float)local_54;
    iVar8 = FUN_0047f1a0(*(int *)((int)this + 0x68));
    *(float *)(iVar8 + 0x2c) = *(float *)(iVar8 + 0x2c) + (float)local_54;
  }
  return;
}



/* 0x0044a330 FUN_0044a330 */

void * __thiscall FUN_0044a330(void *this,void *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cfa31;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0049d420(this,(int)param_1);
  local_4 = 0;
  FUN_0046f930((undefined4 *)((int)this + 0x24));
  *(undefined ***)this = &PTR_FUN_004e1780;
  *(undefined4 *)((int)this + 0x24) = &PTR_UpdateFireRescueActor_004e1774;
  *(undefined4 *)((int)this + 0x7c) = 0;
  *(undefined4 *)((int)this + 0x80) = 0;
  *(undefined4 *)((int)this + 0x84) = 0;
  *(undefined4 *)((int)this + 0x8c) = 0;
  *(undefined4 *)((int)this + 0x90) = 0;
  *(undefined4 *)((int)this + 0x94) = 0;
  *(undefined4 *)((int)this + 0x98) = 0;
  *(undefined4 *)((int)this + 0x9c) = 0;
  *(undefined4 *)((int)this + 0xa0) = 0;
  *(undefined4 *)((int)this + 0xa4) = 0;
  *(undefined4 *)((int)this + 0xa8) = 0;
  *(undefined4 *)((int)this + 0xac) = 0;
  *(undefined4 *)((int)this + 0x88) = 0;
  *(undefined4 *)((int)this + 0xdc) = 0;
  *(undefined4 *)((int)this + 0xe0) = 0;
  *(undefined4 *)((int)this + 0xe4) = 0;
  *(undefined4 *)((int)this + 0xf0) = 0;
  *(undefined4 *)((int)this + 0xf4) = 0;
  *(undefined4 *)((int)this + 0xf8) = 0;
  local_4._0_1_ = 1;
  *(undefined4 *)((int)this + 0x108) = 0;
  *(undefined4 *)((int)this + 0x10c) = 0;
  *(undefined4 *)((int)this + 0x110) = 0;
  FUN_0044b790((undefined4 *)((int)this + 300));
  local_4 = CONCAT31(local_4._1_3_,2);
  FUN_0049d310(this,(int)param_1);
  FUN_0049d160(param_1,(int *)((int)this + 0x78),0x88);
  *(undefined4 *)((int)this + 0x18) = 6;
  FUN_0049d330(this,1);
  ExceptionList = local_c;
  return this;
}



