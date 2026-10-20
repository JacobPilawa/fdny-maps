/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0046bad0 FUN_0046bad0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_0046bad0(float *param_1,float *param_2,float param_3,undefined4 *param_4,
                 undefined4 *param_5,float *param_6)

{
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  undefined4 *puVar4;
  float local_30 [3];
  float local_24 [3];
  float local_18 [3];
  float local_c [3];
  
  pfVar1 = param_1;
  if ((param_1 != (float *)0x0) && (param_2 != (float *)0x0)) {
    param_1 = (float *)0x0;
    if ((float)_DAT_004e1c10 < *param_2 - *pfVar1) {
      param_1 = (float *)((param_3 - *pfVar1) / (*param_2 - *pfVar1));
    }
    pfVar3 = param_1;
    pfVar2 = (float *)FUN_0043b2d0(local_30,param_2 + 1,pfVar1 + 1);
    pfVar3 = (float *)FUN_00446390(local_24,pfVar2,(float)pfVar3);
    puVar4 = (undefined4 *)FUN_0042d050(local_18,pfVar1 + 1,pfVar3);
    *param_5 = *puVar4;
    param_5[1] = puVar4[1];
    param_5[2] = puVar4[2];
    pfVar3 = param_1;
    pfVar2 = (float *)FUN_0043b2d0(local_18,param_2 + 4,pfVar1 + 4);
    pfVar3 = (float *)FUN_00446390(local_24,pfVar2,(float)pfVar3);
    puVar4 = (undefined4 *)FUN_0042d050(local_c,pfVar1 + 4,pfVar3);
    *param_4 = *puVar4;
    param_4[1] = puVar4[1];
    param_4[2] = puVar4[2];
    *param_6 = (param_2[7] - pfVar1[7]) * (float)param_1 + pfVar1[7];
  }
  return;
}



/* 0x0046bc10 FUN_0046bc10 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall
FUN_0046bc10(void *this,float param_1,float *param_2,undefined4 *param_3,undefined4 *param_4,
            float *param_5,char param_6)

{
  int *this_00;
  float *pfVar1;
  undefined4 *puVar2;
  float *pfVar3;
  float *pfVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float *local_78;
  int local_70;
  float local_6c;
  float local_68;
  undefined4 local_64;
  float local_60;
  undefined4 local_5c;
  undefined4 local_58;
  float local_54;
  float local_50;
  undefined4 local_4c;
  float local_48;
  float local_44;
  undefined4 local_40;
  float local_3c [3];
  float local_30 [3];
  float local_24 [3];
  float local_18 [3];
  float local_c [3];
  
  fVar8 = (float)DAT_00502afc;
  if (DAT_00502afc < 0) {
    fVar8 = fVar8 + (float)_DAT_004dfc90;
  }
  local_6c = 0.0;
  local_68 = 0.0;
  fVar8 = fVar8 * (float)_DAT_004dfa28 - param_1 * *(float *)((int)this + 0x18);
  local_64 = 0;
  local_60 = 0.0;
  local_5c = 0;
  local_58 = 0;
  if ((param_6 == '\0') || (*(char *)((int)this + 0x60) == '\0')) {
    iVar7 = *(int *)((int)this + 0x3c);
    local_78 = (float *)0x0;
    local_70 = 0;
    if (0 < iVar7) {
      while ((((iVar6 = *(int *)((int)this + 0x38), -1 < iVar6 &&
               (iVar5 = *(int *)((int)this + 0x24), iVar5 != 0)) && (iVar6 < iVar5)) &&
             (iVar5 = (iVar6 + local_70) % iVar5, iVar5 < iVar7))) {
        this_00 = (int *)((int)this + 0x20);
        FUN_0048d490(this_00,iVar5);
        pfVar3 = *(float **)(*this_00 + iVar5 * 4);
        if (pfVar3 == (float *)0x0) break;
        iVar7 = *(int *)((int)this + 0x38);
        if (((iVar7 < 0) || (iVar6 = *(int *)((int)this + 0x24), iVar6 == 0)) ||
           ((iVar6 <= iVar7 ||
            (iVar6 = (iVar7 + 1 + local_70) % iVar6, *(int *)((int)this + 0x3c) <= iVar6))))
        goto LAB_0046bdef;
        FUN_0048d490(this_00,iVar6);
        local_78 = *(float **)(*this_00 + iVar6 * 4);
        if (local_78 == (float *)0x0) goto LAB_0046bdef;
        if ((*pfVar3 < fVar8 != (*pfVar3 == fVar8)) && (fVar8 <= *local_78)) goto LAB_0046bdeb;
        iVar7 = *(int *)((int)this + 0x3c);
        local_70 = local_70 + 1;
        if (iVar7 <= local_70) goto LAB_0046bdeb;
      }
    }
    iVar7 = *(int *)((int)this + 0x38);
    if (iVar7 < 0) {
      return;
    }
    iVar6 = *(int *)((int)this + 0x24);
    if (iVar6 == 0) {
      return;
    }
    if (iVar6 <= iVar7) {
      return;
    }
    iVar7 = iVar7 % iVar6;
    if (*(int *)((int)this + 0x3c) <= iVar7) {
      return;
    }
    FUN_0048d490((int *)((int)this + 0x20),iVar7);
    pfVar3 = *(float **)(*(int *)((int)this + 0x20) + iVar7 * 4);
    if (pfVar3 == (float *)0x0) {
      return;
    }
LAB_0046bdeb:
    if (local_78 == (float *)0x0) {
LAB_0046bdef:
      local_78 = pfVar3;
      pfVar3 = local_78;
    }
    FUN_0046bad0(pfVar3,local_78,fVar8,&local_6c,&local_60,param_5);
    fVar8 = param_1 * *(float *)((int)this + 0x18);
  }
  else {
    local_6c = *(float *)((int)this + 0x50);
    local_68 = *(float *)((int)this + 0x54);
    local_64 = *(undefined4 *)((int)this + 0x58);
    local_60 = *(float *)((int)this + 0x44);
    local_5c = *(undefined4 *)((int)this + 0x48);
    local_58 = *(undefined4 *)((int)this + 0x4c);
    *param_5 = *(float *)((int)this + 0x5c);
    fVar8 = *(float *)((int)this + 0x40) - fVar8;
  }
  pfVar3 = &local_6c;
  fVar9 = fVar8 * *(float *)((int)this + 100);
  pfVar1 = (float *)FUN_00446390(&local_48,this,fVar8);
  puVar2 = (undefined4 *)FUN_0042d050(&local_54,pfVar1,pfVar3);
  fVar10 = (float)_DAT_004dfa28;
  *param_4 = *puVar2;
  param_4[1] = puVar2[1];
  param_4[2] = puVar2[2];
  if (fVar10 <= param_1) {
    pfVar3 = &local_60;
    pfVar1 = (float *)FUN_00446390(&local_48,&local_6c,fVar8);
    fVar10 = fVar8;
    pfVar4 = (float *)FUN_00447710(&local_54,0.5,this);
    pfVar4 = (float *)FUN_00446390(local_3c,pfVar4,fVar8);
    pfVar4 = (float *)FUN_00446390(local_30,pfVar4,fVar10);
    pfVar1 = (float *)FUN_0042d050(local_24,pfVar4,pfVar1);
    pfVar3 = (float *)FUN_0042d050(local_18,pfVar1,pfVar3);
    *param_2 = *pfVar3;
    param_2[1] = pfVar3[1];
    param_2[2] = pfVar3[2];
  }
  else {
    *param_2 = *(float *)((int)this + 0xc);
    param_2[1] = *(float *)((int)this + 0x10);
    param_2[2] = *(float *)((int)this + 0x14);
  }
  local_54 = *(float *)this;
  local_44 = local_68 - _DAT_004e16c0;
  local_4c = *(undefined4 *)((int)this + 8);
  local_48 = local_6c;
  pfVar3 = &local_60;
  local_50 = *(float *)((int)this + 4) + *(float *)((int)this + 4);
  local_40 = local_64;
  pfVar1 = (float *)FUN_00446390(local_18,&local_48,fVar9);
  fVar8 = fVar9;
  pfVar4 = (float *)FUN_00447710(local_24,0.5,&local_54);
  pfVar4 = (float *)FUN_00446390(local_30,pfVar4,fVar9);
  pfVar4 = (float *)FUN_00446390(local_3c,pfVar4,fVar8);
  pfVar1 = (float *)FUN_0042d050(&local_6c,pfVar4,pfVar1);
  puVar2 = (undefined4 *)FUN_0042d050(local_c,pfVar1,pfVar3);
  *param_3 = *puVar2;
  param_3[1] = puVar2[1];
  param_3[2] = puVar2[2];
  return;
}



/* 0x0046c000 FUN_0046c000 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0046c000(void *this,float *param_1,float *param_2,float param_3)

{
  float *pfVar1;
  float fVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  *(float *)((int)this + 0xc) = *param_1;
  *(float *)((int)this + 0x10) = param_1[1];
  *(float *)((int)this + 0x14) = param_1[2];
  fVar2 = (float)DAT_00502afc;
  if (DAT_00502afc < 0) {
    fVar2 = fVar2 + (float)_DAT_004dfc90;
  }
  fVar2 = fVar2 * (float)_DAT_004dfa28;
  iVar4 = *(int *)((int)this + 0x38);
  if ((((-1 < iVar4) && (iVar3 = *(int *)((int)this + 0x24), iVar3 != 0)) && (iVar4 < iVar3)) &&
     (iVar4 = iVar4 % iVar3, iVar4 < *(int *)((int)this + 0x3c))) {
    FUN_0048d490((int *)((int)this + 0x20),iVar4);
    pfVar1 = *(float **)(*(int *)((int)this + 0x20) + iVar4 * 4);
    if ((pfVar1 != (float *)0x0) && (fVar2 < *(float *)((int)this + 0x1c) + *pfVar1)) {
      return;
    }
  }
  if (*(int *)((int)this + 0x3c) < *(int *)((int)this + 0x24)) {
    do {
      iVar4 = *(int *)((int)this + 0x3c);
      if ((iVar4 < 0) || (*(int *)((int)this + 0x24) <= iVar4)) {
        piVar5 = &local_18;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar5 = 0;
          piVar5 = piVar5 + 1;
        }
        local_18 = 1;
        local_14 = "Array index out of bounds";
        local_10 = "..\\BluelAPI/Container/SArray.h";
        local_c = 0x1cd;
        FUN_00422060(&local_18);
      }
      pfVar1 = *(float **)(*(int *)((int)this + 0x20) + iVar4 * 4);
      *pfVar1 = fVar2;
      pfVar1[1] = *param_1;
      pfVar1[2] = param_1[1];
      pfVar1[3] = param_1[2];
      pfVar1[4] = *param_2;
      pfVar1[5] = param_2[1];
      pfVar1[6] = param_2[2];
      pfVar1[7] = param_3;
      iVar4 = *(int *)((int)this + 0x3c) + 1;
      *(int *)((int)this + 0x3c) = iVar4;
    } while (iVar4 < *(int *)((int)this + 0x24));
  }
  iVar4 = *(int *)((int)this + 0x38) + 1;
  *(int *)((int)this + 0x38) = iVar4;
  if (*(int *)((int)this + 0x24) <= iVar4) {
    *(undefined4 *)((int)this + 0x38) = 0;
  }
  iVar4 = *(int *)((int)this + 0x38);
  FUN_0048d490((int *)((int)this + 0x20),iVar4);
  pfVar1 = *(float **)(*(int *)((int)this + 0x20) + iVar4 * 4);
  *pfVar1 = fVar2;
  pfVar1[1] = *param_1;
  pfVar1[2] = param_1[1];
  pfVar1[3] = param_1[2];
  pfVar1[4] = *param_2;
  pfVar1[5] = param_2[1];
  pfVar1[6] = param_2[2];
  pfVar1[7] = param_3;
  return;
}



