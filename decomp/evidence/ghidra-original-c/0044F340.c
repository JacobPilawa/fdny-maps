/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044f340
 * Match: function entry
 * Entry: 0044f340 FUN_0044f340
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


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


