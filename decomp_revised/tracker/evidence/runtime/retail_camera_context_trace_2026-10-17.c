/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00491c60 FUN_00491c60 */

void __fastcall FUN_00491c60(int param_1)

{
  if (*(int *)(param_1 + 0x5c) != 0) {
                    /* WARNING: Could not recover jumptable at 0x00491c6c. Too many branches */
                    /* WARNING: Treating indirect jump as call */
    geCamera_SetWorldSpaceXForm(*(int *)(param_1 + 0x5c),param_1 + 0x10);
    return;
  }
  return;
}



/* 0x00491be0 FUN_00491be0 */

void __fastcall FUN_00491be0(int param_1)

{
  undefined4 uVar1;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  uStack_10 = *(undefined4 *)(param_1 + 0x68);
  uStack_8 = *(undefined4 *)(param_1 + 0x6c);
  uStack_c = *(undefined4 *)(param_1 + 0x70);
  uStack_4 = *(undefined4 *)(param_1 + 0x74);
  uVar1 = geCamera_Create(&uStack_10,uStack_c,*(undefined4 *)(param_1 + 0x78));
  *(undefined4 *)(param_1 + 0x5c) = uVar1;
  return;
}



/* 0x00491d00 FUN_00491d00 */

void __fastcall FUN_00491d00(int param_1)

{
  *(undefined4 *)(*(int *)(param_1 + 0x54) + 0x170) = *(undefined4 *)(param_1 + 0x5c);
  if (*(int *)(param_1 + 0x60) != 0) {
    FUN_00495450(*(int *)(param_1 + 0x60));
    return;
  }
  return;
}



/* 0x00491d70 FUN_00491d70 */

undefined4 * __thiscall
FUN_00491d70(void *this,void *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int *piVar1;
  int *piVar2;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d4148;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_00492220(this,param_4);
  piVar1 = (int *)((int)this + 0x74);
  piVar2 = (int *)((int)this + 0x70);
  local_4 = 0;
  *(undefined ***)this = &PTR_FUN_004e3b38;
  *(void **)((int)this + 0x58) = param_1;
  *(undefined4 *)((int)this + 0x54) = param_2;
  *(undefined4 *)((int)this + 0x68) = 0;
  *(undefined4 *)((int)this + 0x6c) = 0;
  *(undefined4 *)((int)this + 0x78) = 0x40000000;
  *(undefined4 *)((int)this + 0x5c) = 0;
  FUN_00499cd0(param_1,piVar2,piVar1);
  *piVar1 = *piVar1 + -1;
  *piVar2 = *piVar2 + -1;
  FUN_00492ca0(*(void **)((int)this + 0x54),this);
  *(undefined4 *)((int)this + 0x60) = param_3;
  *(undefined4 *)((int)this + 8) = 1;
  *(undefined1 *)((int)this + 100) = 1;
  ExceptionList = local_c;
  return this;
}



/* 0x004956c0 FUN_004956c0 */

undefined4 * __thiscall FUN_004956c0(void *this,void *param_1)

{
  uint uVar1;
  
  FUN_0042d2d0(this,0);
  *(undefined ***)this = &PTR_FUN_004e4540;
  *(undefined4 *)((int)this + 0x48) = 0;
  *(undefined4 *)((int)this + 0x4c) = 0;
  *(undefined4 *)((int)this + 0x50) = 0;
  uVar1 = FUN_00493af0(param_1,(undefined4 *)((int)this + 0x58),this);
  *(uint *)((int)this + 0x54) = uVar1;
  *(undefined4 *)((int)this + 8) = 0;
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined4 *)((int)this + 0x10) = 0;
  *(undefined4 *)((int)this + 0x18) = 0;
  *(undefined4 *)((int)this + 0x1c) = 0;
  *(undefined4 *)((int)this + 0x20) = 0;
  *(undefined4 *)((int)this + 0x48) = 0;
  *(undefined4 *)((int)this + 0x50) = 0;
  *(undefined4 *)((int)this + 0x40) = 0;
  *(undefined4 *)((int)this + 4) = 0x3f800000;
  *(undefined4 *)((int)this + 0x14) = 0x3f800000;
  *(undefined4 *)((int)this + 0x24) = 0x3f800000;
  *(undefined4 *)((int)this + 0x4c) = 0x3f800000;
  *(undefined1 *)((int)this + 0x44) = 1;
  *(undefined4 *)((int)this + 0x5c) = 0x42f00000;
  return this;
}



/* 0x00495450 FUN_00495450 */

void __fastcall FUN_00495450(int param_1)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  int iVar10;
  int iVar11;
  int iVar12;
  undefined1 local_f4 [40];
  int local_cc;
  int local_c8;
  int local_c4;
  undefined4 local_b0 [3];
  undefined4 local_a4 [3];
  undefined4 local_98 [3];
  undefined4 local_8c [3];
  undefined4 local_80 [3];
  undefined4 local_74 [3];
  undefined4 local_68 [3];
  undefined4 local_5c [3];
  undefined4 local_50 [3];
  undefined1 local_44 [68];
  
  iVar1 = FUN_0042d0a0(local_44,param_1);
  FUN_004977b0(local_f4,iVar1);
  piVar2 = (int *)FUN_0049e770(local_f4,local_50);
  piVar3 = (int *)FUN_00447830(local_f4,local_5c);
  piVar4 = (int *)FUN_00446ee0(local_f4,local_98);
  iVar12 = 0x3f800000;
  iVar11 = 0x3f800000;
  iVar10 = 0x3f800000;
  iVar1 = FUN_0049e770(local_f4,local_68);
  iVar1 = *(int *)(iVar1 + 8);
  iVar5 = FUN_00447830(local_f4,local_80);
  iVar5 = *(int *)(iVar5 + 8);
  iVar6 = FUN_00446ee0(local_f4,local_b0);
  iVar6 = *(int *)(iVar6 + 8);
  iVar7 = FUN_0049e770(local_f4,local_a4);
  iVar7 = *(int *)(iVar7 + 4);
  iVar8 = FUN_00447830(local_f4,local_8c);
  iVar8 = *(int *)(iVar8 + 4);
  iVar9 = FUN_00446ee0(local_f4,local_74);
  FUN_00492110(*(void **)(param_1 + 0x58),*piVar4,*piVar3,*piVar2,local_cc,*(int *)(iVar9 + 4),iVar8
               ,iVar7,local_c8,iVar6,iVar5,iVar1,local_c4,iVar10,iVar11,iVar12);
  FUN_00491d20(*(void **)(param_1 + 0x58),*(float *)(param_1 + 0x5c));
  return;
}



/* 0x00492110 FUN_00492110 */

void __thiscall
FUN_00492110(void *this,int param_1,int param_2,int param_3,int param_4,int param_5,int param_6,
            int param_7,int param_8,int param_9,int param_10,int param_11,int param_12,int param_13,
            int param_14,int param_15)

{
  *(int *)((int)this + 0x10) = param_1;
  *(int *)((int)this + 0x14) = param_2;
  *(int *)((int)this + 0x18) = param_3;
  *(int *)((int)this + 0x1c) = param_5;
  *(int *)((int)this + 0x20) = param_6;
  *(int *)((int)this + 0x24) = param_7;
  *(int *)((int)this + 0x28) = param_9;
  *(int *)((int)this + 0x2c) = param_10;
  *(int *)((int)this + 0x30) = param_11;
  *(int *)((int)this + 0x34) = param_4;
  *(int *)((int)this + 0x38) = param_8;
  *(int *)((int)this + 0x3c) = param_12;
  *(int *)((int)this + 0x40) = param_13;
  *(int *)((int)this + 0x44) = param_14;
  *(int *)((int)this + 0x48) = param_15;
  (**(code **)(*(int *)this + 0x18))();
  return;
}



/* 0x00492450 FUN_00492450 */

void __fastcall FUN_00492450(int param_1)

{
  int iVar1;
  
  if (*(int *)(param_1 + 0x60) != 0) {
    geActor_SetScale(*(int *)(param_1 + 0x60),*(undefined4 *)(param_1 + 0x44),
                     *(undefined4 *)(param_1 + 0x40),*(undefined4 *)(param_1 + 0x44),
                     *(undefined4 *)(param_1 + 0x48));
  }
  iVar1 = *(int *)(param_1 + 0x60);
  if ((iVar1 != 0) && (*(char *)(param_1 + 0x81) == '\0')) {
    if (*(int *)(param_1 + 100) == 0) {
                    /* WARNING: Could not recover jumptable at 0x00492488. Too many branches */
                    /* WARNING: Treating indirect jump as call */
      geActor_ClearPose(iVar1,param_1 + 0x10);
      return;
    }
    geActor_SetPose(iVar1,*(int *)(param_1 + 100),*(undefined4 *)(param_1 + 0x6c),param_1 + 0x10);
  }
  return;
}



/* 0x0044f340 FUN_0044f340 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0044f340(void *this,int *param_1,char param_2,float param_3)

{
  void *pvVar1;
  undefined4 uVar2;
  float *pfVar3;
  int *piVar4;
  undefined4 *puVar5;
  int iVar6;
  float *pfVar7;
  int *unaff_EDI;
  void **ppvVar8;
  ulonglong uVar9;
  float in_stack_0000001c;
  int iVar10;
  int **ppiVar11;
  float local_78;
  float fStack_60;
  float fStack_5c;
  undefined4 uStack_58;
  float local_54;
  char *local_50;
  char *local_4c;
  undefined4 local_48;
  float local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  void *pvStack_24;
  char *pcStack_20;
  char *pcStack_1c;
  undefined4 uStack_18;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004cffa8;
  pvStack_c = ExceptionList;
  if (*(int *)((int)this + 0x18c) == 5) {
    if ((char)param_1 != '\0') {
      iVar10 = 4;
      goto LAB_0044f3d9;
    }
LAB_0044f3d7:
    iVar10 = 0;
  }
  else {
    if (*(char *)((int)this + 300) != '\0') {
      if ((char)param_1 != '\0') {
        iVar10 = 1;
        if (*(int *)((int)this + 0x18c) == 4) {
          iVar10 = 3;
        }
        goto LAB_0044f3d9;
      }
      if (*(char *)((int)this + 300) != '\0') goto LAB_0044f3d7;
    }
    iVar10 = 2;
  }
LAB_0044f3d9:
  pfVar7 = (float *)((int)this + 0x3488);
  ExceptionList = &pvStack_c;
  FUN_00491140((void *)((int)this + 0x198),pfVar7,iVar10);
  iVar10 = 0;
  *(undefined4 *)((int)this + 0x3490) = 0;
  if (*(int *)((int)this + 0x18c) == 4) {
    local_78 = DAT_004e1828;
  }
  else {
    local_78 = DAT_004e1824;
  }
  *(float *)((int)this + 0x348c) = local_78 + *(float *)((int)this + 0x348c);
  if (0 < *(int *)((int)this + 0x3568)) {
    do {
      if ((iVar10 < 0) || (*(int *)((int)this + 0x3568) <= iVar10)) {
        pfVar3 = &local_3c;
        for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
          *pfVar3 = 0.0;
          pfVar3 = pfVar3 + 1;
        }
        local_3c = 1.4013e-45;
        local_38 = "Array index out of bounds";
        local_34 = "..\\BluelAPI/Container/SArray.h";
        local_30 = 0x1cd;
        FUN_00422060((int *)&local_3c);
      }
      if (*(int *)(*(int *)((int)this + 0x3564) + iVar10 * 4) != 0) {
        if ((iVar10 < 0) || (*(int *)((int)this + 0x3568) <= iVar10)) {
          pfVar3 = &local_54;
          for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
            *pfVar3 = 0.0;
            pfVar3 = pfVar3 + 1;
          }
          local_54 = 1.4013e-45;
          local_50 = "Array index out of bounds";
          local_4c = "..\\BluelAPI/Container/SArray.h";
          local_48 = 0x1cd;
          FUN_00422060((int *)&local_54);
        }
        (**(code **)(**(int **)(*(int *)((int)this + 0x3564) + iVar10 * 4) + 0x20))();
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 < *(int *)((int)this + 0x3568));
  }
  iVar10 = *(int *)((int)this + 0x357c);
  if ((iVar10 < 0) || (*(int *)((int)this + 0x3568) <= iVar10)) {
    pfVar3 = &local_3c;
    for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
      *pfVar3 = 0.0;
      pfVar3 = pfVar3 + 1;
    }
    local_3c = 1.4013e-45;
    local_38 = "Array index out of bounds";
    local_34 = "..\\BluelAPI/Container/SArray.h";
    local_30 = 0x1cd;
    FUN_00422060((int *)&local_3c);
  }
  if (*(int *)(*(int *)((int)this + 0x3564) + iVar10 * 4) != 0) {
    if ((param_2 != '\0') && (*(int *)((int)this + 0x3440) == 0)) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      param_1 = &DAT_00501908;
      FUN_00401d80(&param_1,"ACTION");
      ppiVar11 = &param_1;
      uStack_4 = 0;
      pvVar1 = (void *)FUN_00499cb0(DAT_00503a78);
      uVar2 = FUN_00438550(pvVar1,(int *)ppiVar11);
      uStack_4 = 0xffffffff;
      if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
        FUN_004011c0(&param_1);
      }
      if ((char)uVar2 != '\0') {
        iVar10 = *(int *)((int)this + 0x357c);
        FUN_0048d490((int *)((int)this + 0x3564),iVar10);
        (**(code **)(**(int **)(*(int *)((int)this + 0x3564) + iVar10 * 4) + 0x14))();
      }
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      param_1 = &DAT_00501908;
      FUN_00401d80(&param_1,"ACTION");
      ppiVar11 = &param_1;
      uStack_4 = 1;
      pvVar1 = (void *)FUN_00499cb0(DAT_00503a78);
      uVar2 = FUN_00438480(pvVar1,(int *)ppiVar11);
      uStack_4 = 0xffffffff;
      if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
        FUN_004011c0(&param_1);
      }
      if ((char)uVar2 != '\0') {
        iVar10 = *(int *)((int)this + 0x357c);
        FUN_0048d490((int *)((int)this + 0x3564),iVar10);
        (**(code **)(**(int **)(*(int *)((int)this + 0x3564) + iVar10 * 4) + 0x18))();
        if (*(int *)((int)this + 0x357c) == 4) {
          FUN_00402790(&param_1,"Gallons Sprayed");
          uStack_4 = 2;
          uVar9 = FUN_004b9400();
          FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&param_1,(int)uVar9);
          uStack_4 = 0xffffffff;
          if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
            FUN_004011c0(&param_1);
          }
        }
      }
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      param_1 = &DAT_00501908;
      FUN_00401d80(&param_1,"ACTION");
      ppiVar11 = &param_1;
      uStack_4 = 3;
      pvVar1 = (void *)FUN_00499cb0(DAT_00503a78);
      uVar2 = FUN_00438620(pvVar1,(int *)ppiVar11);
      uStack_4 = 0xffffffff;
      if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
        FUN_004011c0(&param_1);
      }
      if ((char)uVar2 != '\0') {
        iVar10 = *(int *)((int)this + 0x357c);
        FUN_0048d490((int *)((int)this + 0x3564),iVar10);
        (**(code **)(**(int **)(*(int *)((int)this + 0x3564) + iVar10 * 4) + 0x1c))();
      }
    }
    iVar10 = *(int *)((int)this + 0x357c);
    if ((iVar10 < 0) || (*(int *)((int)this + 0x3568) <= iVar10)) {
      pfVar3 = &local_3c;
      for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
        *pfVar3 = 0.0;
        pfVar3 = pfVar3 + 1;
      }
      local_3c = 1.4013e-45;
      local_38 = "Array index out of bounds";
      local_34 = "..\\BluelAPI/Container/SArray.h";
      local_30 = 0x1cd;
      FUN_00422060((int *)&local_3c);
    }
    pfVar3 = (float *)FUN_00447710(&fStack_60,in_stack_0000001c,
                                   (float *)(*(int *)(*(int *)((int)this + 0x3564) + iVar10 * 4) +
                                            0x1e4));
    *pfVar7 = *pfVar3 + *pfVar7;
    *(float *)((int)this + 0x348c) = pfVar3[1] + *(float *)((int)this + 0x348c);
    *(float *)((int)this + 0x3490) = pfVar3[2] + *(float *)((int)this + 0x3490);
    iVar10 = *(int *)((int)this + 0x357c);
    if ((iVar10 < 0) || (*(int *)((int)this + 0x3568) <= iVar10)) {
      ppvVar8 = &pvStack_24;
      for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
        *ppvVar8 = (void *)0x0;
        ppvVar8 = ppvVar8 + 1;
      }
      pvStack_24 = (void *)0x1;
      pcStack_20 = "Array index out of bounds";
      pcStack_1c = "..\\BluelAPI/Container/SArray.h";
      uStack_18 = 0x1cd;
      FUN_00422060((int *)&pvStack_24);
    }
    fStack_5c = local_78;
    fStack_60 = 0.0;
    uStack_58 = 0x447a0000;
    pfVar3 = (float *)FUN_0042d050(&local_54,&fStack_60,
                                   (float *)(*(int *)(*(int *)((int)this + 0x3564) + iVar10 * 4) +
                                            0x1f0));
    FUN_00447710(&local_3c,param_3,pfVar3);
  }
  pfVar3 = (float *)FUN_00446390(&local_3c,(float *)&stack0x00000020,
                                 _DAT_004dfc24 - in_stack_0000001c);
  *pfVar7 = *pfVar3 + *pfVar7;
  *(float *)((int)this + 0x348c) = pfVar3[1] + *(float *)((int)this + 0x348c);
  *(float *)((int)this + 0x3490) = pfVar3[2] + *(float *)((int)this + 0x3490);
  FUN_00446390((float *)&stack0x00000020,(float *)&stack0x00000010,_DAT_004dfc24 - param_3);
  iVar10 = 0;
  piVar4 = (int *)((int)this + 0x3460);
  (**(code **)(*(int *)((int)this + 0x3460) + 8))();
  puStack_8 = &stack0xffffff60;
  (**(code **)(*piVar4 + 0x14))();
  FUN_00496b50(piVar4,*(float *)((int)this + 0x3430));
  FUN_004964b0((int)piVar4);
  if (((*(int *)((int)this + 0x18c) == 4) || (*(char *)((int)this + 0x13c) != '\0')) ||
     (*(char *)((int)this + 0x380) != '\0')) {
    FUN_00483680((void *)((int)this + 0x34a4),1);
  }
  FUN_00483720((int)this + 0x34a4);
  FUN_00481c00((int *)&stack0xffffff74);
  pcStack_1c = (char *)0x4;
  do {
    if ((iVar10 == 0) || (unaff_EDI == (int *)0x0)) {
      iVar6 = 0;
    }
    else {
      iVar6 = *unaff_EDI;
    }
    if ((iVar10 != 0) && (unaff_EDI != (int *)0x0)) {
      unaff_EDI = (int *)unaff_EDI[1];
    }
    if (iVar6 == 0) goto LAB_0044fb36;
  } while (*(int *)(iVar6 + 0x18) != 5);
  FUN_00495450(iVar6 + 0x24);
LAB_0044fb36:
  iVar10 = 0;
  if (0 < *(int *)((int)this + 0x3568)) {
    do {
      piVar4 = (int *)FUN_0040cdf0((void *)((int)this + 0x3564),iVar10);
      if (*piVar4 != 0) {
        puVar5 = (undefined4 *)FUN_0040cdf0((void *)((int)this + 0x3564),iVar10);
        (**(code **)(*(int *)*puVar5 + 0x24))();
      }
      iVar10 = iVar10 + 1;
    } while (iVar10 < *(int *)((int)this + 0x3568));
  }
  pcStack_1c = (char *)0xffffffff;
  FUN_004983e0((int *)&stack0xffffff74);
  ExceptionList = pvStack_24;
  return;
}



/* 0x0045f660 FUN_0045f660 */

void __thiscall FUN_0045f660(void *this,undefined4 param_1,int param_2)

{
  void *this_00;
  void *this_01;
  undefined4 local_c [3];
  
  *(undefined4 *)((int)this + 0x54) = param_1;
  if (-1 < param_2) {
    FUN_00497e90((void *)((int)this + 0x58),*(void **)(DAT_00503a78 + 4),2,param_2,1);
  }
  this_01 = (void *)((int)this + 0x15c);
  FUN_00483a20(this_01,*(int *)((int)this + 0x54) + 0x34a4,'\x01');
  FUN_00483a80(this_01,0.006,0.009,0.006);
  FUN_00483680(this_01,0);
  *(void **)((int)this + 0x98) = this_01;
  FUN_0042d280((void *)((int)this + 0x1e4),local_c,0,0,0);
  FUN_0042d280((void *)((int)this + 0x1f0),local_c,0,0,0);
  this_00 = (void *)((int)this + 0x58);
  FUN_00497e10(this_00,0);
  FUN_00497e30((int)this_00,0.0);
  *(undefined1 *)((int)this + 0x1e0) = 1;
  *(undefined1 *)((int)this + 0x158) = 0;
  FUN_00497de0(this_00,0);
  if (*(char *)((int)this + 0x158) != '\0') {
    FUN_00483a20(this_01,*(int *)((int)this + 0x54) + 0x34a4,'\x01');
  }
  return;
}



