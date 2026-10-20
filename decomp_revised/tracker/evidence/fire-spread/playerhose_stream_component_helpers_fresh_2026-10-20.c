/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0046ba60 FUN_0046ba60 */

void __fastcall FUN_0046ba60(int param_1)

{
  *(undefined1 *)(param_1 + 0x60) = 0;
  return;
}



/* 0x0046bc10 FUN_0046bc10 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall
FUN_0046bc10(void *this,float param_1,undefined4 *param_2,undefined4 *param_3,undefined4 *param_4,
            float *param_5,char param_6)

{
  int *this_00;
  float *pfVar1;
  undefined4 *puVar2;
  float *pfVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  float *pfVar7;
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
    iVar6 = *(int *)((int)this + 0x3c);
    local_78 = (float *)0x0;
    local_70 = 0;
    if (0 < iVar6) {
      while ((((iVar5 = *(int *)((int)this + 0x38), -1 < iVar5 &&
               (iVar4 = *(int *)((int)this + 0x24), iVar4 != 0)) && (iVar5 < iVar4)) &&
             (iVar4 = (iVar5 + local_70) % iVar4, iVar4 < iVar6))) {
        this_00 = (int *)((int)this + 0x20);
        FUN_0048d490(this_00,iVar4);
        pfVar7 = *(float **)(*this_00 + iVar4 * 4);
        if (pfVar7 == (float *)0x0) break;
        iVar6 = *(int *)((int)this + 0x38);
        if (((iVar6 < 0) || (iVar5 = *(int *)((int)this + 0x24), iVar5 == 0)) ||
           ((iVar5 <= iVar6 ||
            (iVar5 = (iVar6 + 1 + local_70) % iVar5, *(int *)((int)this + 0x3c) <= iVar5))))
        goto LAB_0046bdef;
        FUN_0048d490(this_00,iVar5);
        local_78 = *(float **)(*this_00 + iVar5 * 4);
        if (local_78 == (float *)0x0) goto LAB_0046bdef;
        if ((*pfVar7 < fVar8 != (*pfVar7 == fVar8)) && (fVar8 <= *local_78)) goto LAB_0046bdeb;
        iVar6 = *(int *)((int)this + 0x3c);
        local_70 = local_70 + 1;
        if (iVar6 <= local_70) goto LAB_0046bdeb;
      }
    }
    iVar6 = *(int *)((int)this + 0x38);
    if (iVar6 < 0) {
      return;
    }
    iVar5 = *(int *)((int)this + 0x24);
    if (iVar5 == 0) {
      return;
    }
    if (iVar5 <= iVar6) {
      return;
    }
    iVar6 = iVar6 % iVar5;
    if (*(int *)((int)this + 0x3c) <= iVar6) {
      return;
    }
    FUN_0048d490((int *)((int)this + 0x20),iVar6);
    pfVar7 = *(float **)(*(int *)((int)this + 0x20) + iVar6 * 4);
    if (pfVar7 == (float *)0x0) {
      return;
    }
LAB_0046bdeb:
    if (local_78 == (float *)0x0) {
LAB_0046bdef:
      local_78 = pfVar7;
      pfVar7 = local_78;
    }
    FUN_0046bad0(pfVar7,local_78,fVar8,&local_6c,&local_60,param_5);
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
  pfVar7 = &local_6c;
  fVar9 = fVar8 * *(float *)((int)this + 100);
  pfVar1 = (float *)FUN_00446390(&local_48,this,fVar8);
  puVar2 = (undefined4 *)FUN_0042d050(&local_54,pfVar1,pfVar7);
  fVar10 = (float)_DAT_004dfa28;
  *param_4 = *puVar2;
  param_4[1] = puVar2[1];
  param_4[2] = puVar2[2];
  if (fVar10 <= param_1) {
    pfVar7 = &local_60;
    pfVar1 = (float *)FUN_00446390(&local_48,&local_6c,fVar8);
    fVar10 = fVar8;
    pfVar3 = (float *)FUN_00447710(&local_54,0.5,this);
    pfVar3 = (float *)FUN_00446390(local_3c,pfVar3,fVar8);
    pfVar3 = (float *)FUN_00446390(local_30,pfVar3,fVar10);
    pfVar1 = (float *)FUN_0042d050(local_24,pfVar3,pfVar1);
    puVar2 = (undefined4 *)FUN_0042d050(local_18,pfVar1,pfVar7);
    *param_2 = *puVar2;
    param_2[1] = puVar2[1];
    param_2[2] = puVar2[2];
  }
  else {
    *param_2 = *(undefined4 *)((int)this + 0xc);
    param_2[1] = *(undefined4 *)((int)this + 0x10);
    param_2[2] = *(undefined4 *)((int)this + 0x14);
  }
  local_54 = *(float *)this;
  local_44 = local_68 - _DAT_004e16c0;
  local_4c = *(undefined4 *)((int)this + 8);
  local_48 = local_6c;
  pfVar7 = &local_60;
  local_50 = *(float *)((int)this + 4) + *(float *)((int)this + 4);
  local_40 = local_64;
  pfVar1 = (float *)FUN_00446390(local_18,&local_48,fVar9);
  fVar8 = fVar9;
  pfVar3 = (float *)FUN_00447710(local_24,0.5,&local_54);
  pfVar3 = (float *)FUN_00446390(local_30,pfVar3,fVar9);
  pfVar3 = (float *)FUN_00446390(local_3c,pfVar3,fVar8);
  pfVar1 = (float *)FUN_0042d050(&local_6c,pfVar3,pfVar1);
  puVar2 = (undefined4 *)FUN_0042d050(local_c,pfVar1,pfVar7);
  *param_3 = *puVar2;
  param_3[1] = puVar2[1];
  param_3[2] = puVar2[2];
  return;
}



/* 0x00469f50 FUN_00469f50 */

void __thiscall FUN_00469f50(void *this,undefined4 *param_1,undefined4 param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  piVar1 = (int *)((int)this + 0x14);
  if (*(int *)(*(int *)((int)this + 0x14) + 4) <= *(int *)((int)this + 0x10)) {
    local_c = 0;
    local_8 = 0;
    local_4 = 0;
    local_10 = 0x3f800000;
    do {
      FUN_00469d30(piVar1,&local_c);
      FUN_00454330((void *)((int)this + 0x18),&local_10);
    } while (*(int *)(*piVar1 + 4) <= *(int *)((int)this + 0x10));
  }
  iVar3 = *(int *)((int)this + 0x10);
  FUN_00484800(piVar1,iVar3);
  if (1 < *(int *)*piVar1) {
    FUN_00469960(piVar1);
  }
  puVar2 = (undefined4 *)(*piVar1 + 0x14 + iVar3 * 0xc);
  *puVar2 = *param_1;
  puVar2[1] = param_1[1];
  puVar2[2] = param_1[2];
  iVar3 = *(int *)((int)this + 0x10);
  piVar1 = (int *)((int)this + 0x18);
  FUN_00484800(piVar1,iVar3);
  if (1 < *(int *)*piVar1) {
    FUN_0041aff0(piVar1);
  }
  *(undefined4 *)(*piVar1 + 0x14 + iVar3 * 4) = param_2;
  *(int *)((int)this + 0x10) = *(int *)((int)this + 0x10) + 1;
  return;
}



/* 0x0046c2f0 FUN_0046c2f0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void * __thiscall FUN_0046c2f0(void *this,int param_1,void *param_2)

{
  int *this_00;
  int *piVar1;
  int iVar2;
  undefined4 local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d211b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined4 *)this = 0;
  *(undefined4 *)((int)this + 4) = 0;
  *(undefined4 *)((int)this + 8) = 0;
  this_00 = (int *)((int)this + 0x20);
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined4 *)((int)this + 0x10) = 0;
  *(undefined4 *)((int)this + 0x14) = 0;
  *(undefined4 *)((int)this + 0x34) = 0x10;
  *(undefined1 **)((int)this + 0x30) = &LAB_00486640;
  *this_00 = 0;
  *(undefined4 *)((int)this + 0x24) = 0;
  *(undefined4 *)((int)this + 0x28) = 0;
  *(undefined4 *)((int)this + 0x2c) = 0;
  *(undefined4 *)((int)this + 0x44) = 0;
  *(undefined4 *)((int)this + 0x48) = 0;
  *(undefined4 *)((int)this + 0x4c) = 0;
  *(undefined4 *)((int)this + 0x50) = 0;
  *(undefined4 *)((int)this + 0x54) = 0;
  *(undefined4 *)((int)this + 0x58) = 0;
  *(void **)((int)this + 0x1c) = param_2;
  local_4 = 0;
  *(int *)((int)this + 0x18) = param_1;
  FUN_0042d280(this,local_18,0,0xc4fa0000,0);
  *(undefined4 *)((int)this + 0x38) = 0xffffffff;
  *(undefined4 *)((int)this + 0x68) = 0x3e4ccccd;
  *(undefined4 *)((int)this + 100) = 0x3f19999a;
  param_1 = 0;
  if (_DAT_004dc50c < *(float *)((int)this + 0x18) / *(float *)((int)this + 0x1c)) {
    do {
      param_2 = operator_new(0x20);
      if (param_2 == (void *)0x0) {
        param_2 = (void *)0x0;
      }
      else {
        *(undefined4 *)((int)param_2 + 4) = 0;
        *(undefined4 *)((int)param_2 + 8) = 0;
        *(undefined4 *)((int)param_2 + 0xc) = 0;
        *(undefined4 *)((int)param_2 + 0x10) = 0;
        *(undefined4 *)((int)param_2 + 0x14) = 0;
        *(undefined4 *)((int)param_2 + 0x18) = 0;
      }
      iVar2 = 0;
      if (0 < *(int *)((int)this + 0x24)) {
        piVar1 = (int *)*this_00;
        do {
          if (*piVar1 == 0) goto LAB_0046c3f7;
          iVar2 = iVar2 + 1;
          piVar1 = piVar1 + 1;
        } while (iVar2 < *(int *)((int)this + 0x24));
      }
      FUN_0043b6e0(this_00,iVar2 + 1);
LAB_0046c3f7:
      param_1 = param_1 + 1;
      *(void **)(*this_00 + iVar2 * 4) = param_2;
    } while ((float)param_1 < *(float *)((int)this + 0x18) / *(float *)((int)this + 0x1c));
  }
  *(undefined4 *)((int)this + 0x3c) = 0;
  ExceptionList = local_c;
  return this;
}



/* 0x00469420 FUN_00469420 */

void __thiscall FUN_00469420(void *this,undefined4 param_1)

{
  *(undefined4 *)((int)this + 0x2c) = param_1;
  FUN_00493d60(*(void **)(DAT_00503a78 + 4),(int)this,0);
  *(undefined4 *)((int)this + 8) = 1000;
  return;
}



/* 0x00469520 FUN_00469520 */

void __thiscall FUN_00469520(void *this,char param_1)

{
  void *this_00;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  *(char *)((int)this + 0xec) = param_1;
  *(undefined4 *)((int)this + 0x44) = 0x42b40000;
  *(undefined4 *)((int)this + 0x48) = 0x42fa0000;
  if (param_1 != '\0') {
    this_00 = (void *)((int)this + 0x4c);
    local_c = 0x3f666666;
    local_8 = 0x3f666666;
    local_4 = 0x3f800000;
    FUN_0047db10(this_00,1,0x3f000000,0x3f800000,0x3f800000,&local_c);
    local_c = 0x3f800000;
    local_8 = 0x3f800000;
    local_4 = 0x3f800000;
    FUN_0047db10(this_00,1,0x3f000000,0,0x40400000,&local_c);
    *(uint *)((int)this + 100) = *(uint *)((int)this + 100) & 0xfffffffe | 8;
    FUN_0047fe70((void *)((int)this + 0x68),*(void **)(DAT_00503a78 + 4),this_00,'\x01');
    *(undefined4 *)((int)this + 0x98) = 5;
    *(undefined4 *)((int)this + 0xa0) = 1;
    *(undefined4 *)((int)this + 0xa4) = 2;
    *(undefined4 *)((int)this + 0xa8) = 0x3d4ccccd;
    *(undefined4 *)((int)this + 0xac) = 0x3dcccccd;
    FUN_0042d280((void *)((int)this + 0xb0),&local_c,0,0x437a0000,0);
    FUN_0042d280((void *)((int)this + 0xbc),&local_c,0,0x43960000,0);
    *(undefined4 *)((int)this + 200) = 0x3f400000;
    *(undefined4 *)((int)this + 0xcc) = 0x3f800000;
    *(undefined4 *)((int)this + 0xd0) = 0x40000000;
    *(undefined4 *)((int)this + 0xd4) = 0x40a00000;
    *(undefined4 *)((int)this + 0xd8) = 0x3f800000;
    *(undefined4 *)((int)this + 0xdc) = 0x20;
    FUN_0042d280((void *)((int)this + 0xe0),&local_c,0,0x41a00000,0);
  }
  return;
}



