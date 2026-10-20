/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0047df10 FUN_0047df10 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0047df10(int param_1)

{
  void *pvVar1;
  float fVar2;
  float *pfVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  byte local_12;
  byte local_11;
  uint local_10;
  float local_c;
  float local_8;
  float local_4;
  
  if (*(char *)(param_1 + 0x101) != '\0') {
    pvVar1 = *(void **)(param_1 + 0x104);
    if (*(char *)(param_1 + 0x100) == '\0') {
      if ((pvVar1 != (void *)0x0) && (*(char *)(param_1 + 0x110) != '\0')) {
        FUN_0047d660(pvVar1);
        *(undefined1 *)(param_1 + 0x101) = *(undefined1 *)(*(int *)(param_1 + 0x104) + 0x30);
        return;
      }
    }
    else {
      uVar6 = 0;
      local_12 = 0xff;
      local_11 = 0xff;
      if (pvVar1 == (void *)0x0) {
        if (-1 < *(int *)(param_1 + 0x108)) {
          uVar6 = FUN_00493d00(*(void **)(DAT_00503a78 + 4),*(int *)(param_1 + 0x108),'\x01');
        }
      }
      else {
        if (*(char *)(param_1 + 0x110) != '\0') {
          FUN_0047d660(pvVar1);
        }
        *(undefined1 *)(param_1 + 0x101) = *(undefined1 *)((int)*(void **)(param_1 + 0x104) + 0x30);
        FUN_0047d6d0(*(void **)(param_1 + 0x104),&local_12,&local_11);
        uVar6 = FUN_0047d820(*(void **)(param_1 + 0x104),*(void **)(DAT_00503a78 + 4));
        pfVar3 = (float *)FUN_0047d420(*(int *)(param_1 + 0x104));
        local_c = *pfVar3;
        local_8 = pfVar3[1];
        local_4 = pfVar3[2];
        fVar2 = local_c * *(float *)(param_1 + 0x114) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x24) = fVar2;
        fVar2 = local_8 * *(float *)(param_1 + 0x118) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x28) = fVar2;
        fVar2 = local_4 * *(float *)(param_1 + 0x11c) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x2c) = fVar2;
        fVar2 = local_c * *(float *)(param_1 + 0x114) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x48) = fVar2;
        fVar2 = local_8 * *(float *)(param_1 + 0x118) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x4c) = fVar2;
        fVar2 = local_4 * *(float *)(param_1 + 0x11c) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x50) = fVar2;
        fVar2 = local_c * *(float *)(param_1 + 0x114) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x6c) = fVar2;
        fVar2 = local_8 * *(float *)(param_1 + 0x118) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x70) = fVar2;
        fVar2 = local_4 * *(float *)(param_1 + 0x11c) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x74) = fVar2;
        fVar2 = local_c * *(float *)(param_1 + 0x114) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x90) = fVar2;
        fVar2 = local_8 * *(float *)(param_1 + 0x118) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x94) = fVar2;
        fVar2 = local_4 * *(float *)(param_1 + 0x11c) * _DAT_004e3524;
        if (_DAT_004e3524 < fVar2) {
          fVar2 = _DAT_004e3524;
        }
        *(float *)(param_1 + 0x98) = fVar2;
      }
      *(undefined4 *)(param_1 + 0x1c) = *(undefined4 *)(param_1 + 0xd0);
      *(undefined4 *)(param_1 + 0x20) = *(undefined4 *)(param_1 + 0xd4);
      *(undefined4 *)(param_1 + 0x40) = *(undefined4 *)(param_1 + 0xdc);
      *(undefined4 *)(param_1 + 0x44) = *(undefined4 *)(param_1 + 0xe0);
      *(undefined4 *)(param_1 + 100) = *(undefined4 *)(param_1 + 0xe8);
      *(undefined4 *)(param_1 + 0x68) = *(undefined4 *)(param_1 + 0xec);
      *(undefined4 *)(param_1 + 0x88) = *(undefined4 *)(param_1 + 0xf4);
      *(undefined4 *)(param_1 + 0x8c) = *(undefined4 *)(param_1 + 0xf8);
      if (*(char *)(param_1 + 0x120) == '\0') {
        local_10 = (uint)local_12;
        fVar2 = (float)local_10;
        *(float *)(param_1 + 0x30) = fVar2;
        *(float *)(param_1 + 0x54) = fVar2;
        *(float *)(param_1 + 0x78) = fVar2;
        *(float *)(param_1 + 0x9c) = fVar2;
      }
      geEngine_RenderPoly(*(undefined4 *)(**(int **)(DAT_00503a78 + 4) + 0x74),param_1 + 0x10,4,
                          uVar6,-(uint)(*(char *)(param_1 + 0x112) != '\0') & 0x100 |
                                -(uint)(*(char *)(param_1 + 0x111) != '\0') & 0x80 | 0x36);
      if (((*(char *)(param_1 + 0x120) == '\0') && (iVar4 = *(int *)(param_1 + 0x104), iVar4 != 0))
         && ((*(byte *)(iVar4 + 0x18) & 2) != 0)) {
        iVar4 = FUN_0047d3e0(iVar4);
        iVar5 = FUN_0047d400(*(int *)(param_1 + 0x104));
        if ((iVar4 != iVar5) &&
           (iVar4 = FUN_0047d860(*(void **)(param_1 + 0x104),*(void **)(DAT_00503a78 + 4)),
           iVar4 != 0)) {
          if (*(int *)(param_1 + 0x104) != 0) {
            pfVar3 = (float *)FUN_0047d560(*(int *)(param_1 + 0x104));
            local_c = *pfVar3;
            local_8 = pfVar3[1];
            local_4 = pfVar3[2];
            fVar2 = local_c * *(float *)(param_1 + 0x114) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x24) = fVar2;
            fVar2 = local_8 * *(float *)(param_1 + 0x118) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x28) = fVar2;
            fVar2 = local_4 * *(float *)(param_1 + 0x11c) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x2c) = fVar2;
            fVar2 = local_c * *(float *)(param_1 + 0x114) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x48) = fVar2;
            fVar2 = local_8 * *(float *)(param_1 + 0x118) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x4c) = fVar2;
            fVar2 = local_4 * *(float *)(param_1 + 0x11c) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x50) = fVar2;
            fVar2 = local_c * *(float *)(param_1 + 0x114) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x6c) = fVar2;
            fVar2 = local_8 * *(float *)(param_1 + 0x118) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x70) = fVar2;
            fVar2 = local_4 * *(float *)(param_1 + 0x11c) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x74) = fVar2;
            fVar2 = local_c * *(float *)(param_1 + 0x114) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x90) = fVar2;
            fVar2 = local_8 * *(float *)(param_1 + 0x118) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x94) = fVar2;
            fVar2 = local_4 * *(float *)(param_1 + 0x11c) * _DAT_004e3524;
            if (_DAT_004e3524 < fVar2) {
              fVar2 = _DAT_004e3524;
            }
            *(float *)(param_1 + 0x98) = fVar2;
          }
          local_10 = (uint)local_11;
          fVar2 = (float)local_10;
          *(float *)(param_1 + 0x30) = fVar2;
          *(float *)(param_1 + 0x54) = fVar2;
          *(float *)(param_1 + 0x78) = fVar2;
          *(float *)(param_1 + 0x9c) = fVar2;
          geEngine_RenderPoly(*(undefined4 *)(**(int **)(DAT_00503a78 + 4) + 0x74),param_1 + 0x10,4,
                              iVar4,-(*(char *)(param_1 + 0x112) != '\0') & 0x40U |
                                    -(*(char *)(param_1 + 0x111) != '\0') & 0x20U | 0xd);
        }
      }
    }
  }
  return;
}



/* 0x0047e5b0 FUN_0047e5b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0047e5b0(int param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  int iVar5;
  int iVar6;
  int local_10;
  int local_c;
  int local_8;
  int local_4;
  
  *(undefined1 *)(param_1 + 0x112) = 0;
  if ((DAT_004ffad8 == 0x280) && (DAT_004ffadc == 0x1e0)) {
    if (*(void **)(param_1 + 0x104) == (void *)0x0) {
      if (*(int *)(param_1 + 0x108) < 0) {
        return;
      }
      iVar5 = FUN_00493d00(*(void **)(DAT_00503a78 + 4),*(int *)(param_1 + 0x108),'\x01');
    }
    else {
      iVar5 = FUN_0047d820(*(void **)(param_1 + 0x104),*(void **)(DAT_00503a78 + 4));
    }
    if (iVar5 != 0) {
      iVar6 = geBitmap_Height(iVar5);
      iVar5 = geBitmap_Width(iVar5);
      fVar1 = *(float *)(param_1 + 0xdc);
      fVar2 = *(float *)(param_1 + 0xd0);
      fVar3 = *(float *)(param_1 + 0xf8);
      fVar4 = *(float *)(param_1 + 0xd4);
      FUN_0047dce0(&local_10);
      if ((ABS(ABS((fVar1 - fVar2) * (float)iVar5) - (float)(local_c - local_10)) <=
           (float)_DAT_004e16b8) &&
         (ABS(ABS((fVar3 - fVar4) * (float)iVar6) - (float)(local_4 - local_8)) <=
          (float)_DAT_004e16b8)) {
        *(undefined1 *)(param_1 + 0x112) = 1;
      }
    }
  }
  return;
}



/* 0x0047eac0 FUN_0047eac0 */

void __thiscall FUN_0047eac0(void *this,int param_1,int param_2)

{
  void *this_00;
  int iVar1;
  undefined4 uVar2;
  int iVar3;
  int iVar4;
  
  this_00 = *(void **)(DAT_00503a78 + 4);
  if (this_00 != (void *)0x0) {
    iVar3 = 0;
    iVar4 = 0;
    if (*(void **)((int)this + 0x104) == (void *)0x0) {
      if (*(int *)((int)this + 0x108) < 0) {
        iVar3 = 10;
        iVar4 = 10;
      }
      else {
        uVar2 = FUN_00493d00(this_00,*(int *)((int)this + 0x108),'\x01');
        iVar3 = geBitmap_Width(uVar2);
        iVar4 = geBitmap_Height(uVar2);
      }
    }
    else {
      iVar1 = FUN_0047d820(*(void **)((int)this + 0x104),this_00);
      if (iVar1 != 0) {
        iVar3 = geBitmap_Width(iVar1);
        iVar4 = geBitmap_Height(iVar1);
      }
    }
    FUN_0047ea10(this,param_1,param_2,iVar3 + param_1,iVar4 + param_2,0);
  }
  return;
}



/* 0x0047ee10 FUN_0047ee10 */

void __thiscall FUN_0047ee10(void *this,int param_1,char param_2)

{
  void *this_00;
  int iVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d340b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  this_00 = operator_new(0x34);
  local_4 = 0;
  if (this_00 == (void *)0x0) {
    iVar1 = 0;
  }
  else {
    iVar1 = FUN_0047d370(this_00,param_1,0);
  }
  local_4 = 0xffffffff;
  *(undefined4 *)((int)this + 0x108) = 0xffffffff;
  *(int *)((int)this + 0x104) = iVar1;
  FUN_0047ecd0(this,param_2);
  ExceptionList = local_c;
  return;
}



/* 0x0047ee90 FUN_0047ee90 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0047ee90(void *this,float *param_1,char param_2)

{
  *(undefined4 *)((int)this + 0x108) = 0xffffffff;
  FUN_0047ecd0(this,param_2);
  *(float *)((int)this + 0x24) = *param_1 * _DAT_004e3524;
  *(float *)((int)this + 0x28) = param_1[1] * _DAT_004e3524;
  *(float *)((int)this + 0x2c) = param_1[2] * _DAT_004e3524;
  *(float *)((int)this + 0x48) = *param_1 * _DAT_004e3524;
  *(float *)((int)this + 0x4c) = param_1[1] * _DAT_004e3524;
  *(float *)((int)this + 0x50) = param_1[2] * _DAT_004e3524;
  *(float *)((int)this + 0x6c) = *param_1 * _DAT_004e3524;
  *(float *)((int)this + 0x70) = param_1[1] * _DAT_004e3524;
  *(float *)((int)this + 0x74) = param_1[2] * _DAT_004e3524;
  *(float *)((int)this + 0x90) = *param_1 * _DAT_004e3524;
  *(float *)((int)this + 0x94) = param_1[1] * _DAT_004e3524;
  *(float *)((int)this + 0x98) = param_1[2] * _DAT_004e3524;
  *(undefined1 *)((int)this + 0x120) = 0;
  *(undefined4 *)((int)this + 0x104) = 0;
  return;
}



/* 0x0047ef60 FUN_0047ef60 */

void __thiscall FUN_0047ef60(void *this,undefined4 param_1,char param_2)

{
  *(undefined4 *)((int)this + 0x108) = param_1;
  *(undefined4 *)((int)this + 0x104) = 0;
  FUN_0047ecd0(this,param_2);
  return;
}



/* 0x0047f0f0 FUN_0047f0f0 */

undefined4 * __thiscall FUN_0047f0f0(void *this,undefined4 param_1,char param_2)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3458;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_00491bb0(this);
  local_4 = 0;
  *(undefined ***)this = &PTR_FUN_004e3528;
  _eh_vector_constructor_iterator_
            ((void *)((int)this + 0xa0),0xc,4,(_func_void_void_ptr *)&LAB_0044c060,FUN_0043f660);
  local_4._0_1_ = 1;
  _eh_vector_constructor_iterator_
            ((void *)((int)this + 0xd0),0xc,4,(_func_void_void_ptr *)&LAB_0044c060,FUN_0043f660);
  *(undefined4 *)((int)this + 0x114) = 0;
  *(undefined4 *)((int)this + 0x118) = 0;
  *(undefined4 *)((int)this + 0x11c) = 0;
  local_4 = CONCAT31(local_4._1_3_,2);
  *(undefined4 *)((int)this + 0x108) = param_1;
  *(undefined4 *)((int)this + 0x104) = 0;
  FUN_0047ecd0(this,param_2);
  ExceptionList = local_c;
  return this;
}



/* 0x0047f850 FUN_0047f850 */

undefined4 * __thiscall FUN_0047f850(void *this,int param_1,undefined1 param_2)

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



/* 0x0047d330 FUN_0047d330 */

void __fastcall FUN_0047d330(void *param_1)

{
  int *piVar1;
  void *this;
  int extraout_ECX;
  
  piVar1 = (int *)((int)param_1 + 4);
  *(undefined4 *)((int)param_1 + 0x18) = *(undefined4 *)(*(int *)((int)param_1 + 0xc) + 0x18);
  *piVar1 = 0;
  FUN_0047d2b0(param_1,piVar1);
  *(int *)((int)this + 8) = *piVar1 + 1;
  FUN_0047d2b0(this,(int *)((int)this + 8));
  *(undefined4 *)(extraout_ECX + 0x14) = DAT_00502afc;
  *(undefined4 *)(extraout_ECX + 0x1c) = 0x3f800000;
  *(undefined1 *)(extraout_ECX + 0x30) = 1;
  *(undefined4 *)(extraout_ECX + 0x20) = 0x3f800000;
  return;
}



