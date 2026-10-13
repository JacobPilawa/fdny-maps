/* Ghidra decompilation; names/types are provisional.
 * Entry: 004640f0
 * Function: FUN_004640f0
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004640f0(int param_1)

{
  int iVar1;
  char cVar2;
  undefined1 uVar3;
  float *pfVar4;
  undefined4 *puVar5;
  void *pvVar6;
  float *pfVar7;
  float *pfVar8;
  void *pvVar9;
  float10 fVar10;
  int *piVar11;
  float fVar12;
  float fVar13;
  float fVar14;
  int *piStack_a8;
  float afStack_a4 [4];
  float afStack_94 [3];
  float afStack_88 [5];
  undefined4 uStack_74;
  undefined4 uStack_70;
  undefined1 auStack_6c [40];
  float fStack_44;
  float fStack_40;
  float fStack_3c;
  undefined1 *puStack_28;
  float afStack_24 [3];
  float afStack_18 [3];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d193b;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  FUN_0045f600(param_1);
  if ((*(int *)(param_1 + 0xc68) == 3) || (*(int *)(param_1 + 0xc68) == 1)) {
    cVar2 = '\x01';
  }
  else {
    cVar2 = '\0';
  }
  pvVar9 = (void *)(param_1 + 0xc70);
  *(char *)(param_1 + 0xd08) = cVar2;
  if ((cVar2 == '\0') || (*(char *)(param_1 + 0xd50) == '\0')) {
    uVar3 = 0;
  }
  else {
    uVar3 = 1;
  }
  *(undefined1 *)(param_1 + 0xd44) = uVar3;
  *(bool *)(param_1 + 0xd50) = *(int *)(param_1 + 0xc68) == 1;
  if (*(int *)(param_1 + 0xc68) == 1) {
    FUN_004826b0(pvVar9,(float *)0x0);
    FUN_00482950((int)pvVar9);
  }
  else if (*(char *)(param_1 + 0xd08) != '\0') {
    fVar10 = FUN_00482aa0(*(float *)(param_1 + 0x88),
                          (float)(int)((-(uint)(*(char *)(param_1 + 0xc58) != '\0') & 0xfffffffa) +
                                      0x15));
    *(float *)(param_1 + 0x88) = (float)fVar10;
    FUN_004977b0(auStack_6c,param_1 + 0x58);
    afStack_a4[0] = 0.0;
    afStack_a4[1] = 0.0;
    afStack_a4[2] = 8.0;
    pfVar4 = (float *)FUN_00495d20(afStack_94,(int)auStack_6c,afStack_a4);
    fStack_44 = *pfVar4;
    fStack_40 = pfVar4[1];
    fStack_3c = pfVar4[2];
    if (*(char *)(param_1 + 0xc58) != '\0') {
      FUN_00480090((void *)(param_1 + 0x2a8),(int)auStack_6c,'\0');
      fVar13 = 5.0;
      pfVar4 = (float *)FUN_0049e770(auStack_6c,afStack_94);
      pfVar4 = (float *)FUN_00446390(afStack_a4,pfVar4,fVar13);
      fStack_44 = fStack_44 + *pfVar4;
      fStack_40 = fStack_40 + pfVar4[1];
      fStack_3c = fStack_3c + pfVar4[2];
      FUN_00480090((void *)(param_1 + 0x32c),(int)auStack_6c,'\0');
      pvVar6 = (void *)(param_1 + 0xa48);
      FUN_00447890(pvVar6,(int)auStack_6c);
      pfVar4 = (float *)FUN_0049e770(auStack_6c,afStack_94);
      pfVar4 = (float *)FUN_00447710(afStack_a4,-5.0,pfVar4);
      *(float *)(param_1 + 0xa70) = *(float *)(param_1 + 0xa70) + *pfVar4;
      *(float *)(param_1 + 0xa74) = pfVar4[1] + *(float *)(param_1 + 0xa74);
      *(float *)(param_1 + 0xa78) = pfVar4[2] + *(float *)(param_1 + 0xa78);
      fVar13 = (float)DAT_00502afc;
      if (DAT_00502afc < 0) {
        fVar13 = fVar13 + (float)_DAT_004dfc90;
      }
      FUN_00496d00(pvVar6,fVar13 * (float)_DAT_004e29f8);
      fVar10 = FUN_00404ac0();
      fVar13 = (float)(fVar10 * (float10)_DAT_004e0508 + (float10)_DAT_004e1430);
      FUN_0042d280((void *)(param_1 + 0xa7c),afStack_94,fVar13,fVar13,fVar13);
      FUN_004696a0((int)pvVar6);
      FUN_00463ef0(param_1);
    }
    puVar5 = (undefined4 *)(**(code **)(**(int **)(DAT_00503a78 + 0x30) + 0xc))();
    afStack_88[4] = (float)*puVar5;
    uStack_74 = puVar5[1];
    uStack_70 = puVar5[2];
    FUN_004826b0(pvVar9,afStack_88 + 4);
    FUN_00482950((int)pvVar9);
    cVar2 = *(char *)(param_1 + 0xc58);
    pfVar4 = (float *)FUN_0049e770(auStack_6c,afStack_94);
    FUN_0046d1b0((void *)(param_1 + 0x608),&fStack_44,pfVar4,cVar2);
    fVar10 = (float10)FUN_00485bc0((int)pvVar9);
    afStack_a4[3] = (float)fVar10;
    iVar1 = *(int *)(param_1 + 0xcc0);
    fVar13 = *(float *)(param_1 + 0xcb0);
    FUN_004b9400();
    FUN_004b9400();
    FUN_004054a0(&piStack_a8);
    uStack_4 = 0;
    FUN_00402770(&piStack_a8,(byte *)"%d/%d");
    puStack_28 = &stack0xffffff34;
    afStack_88[0] = 0.0;
    afStack_88[1] = 8.82818e-43;
    afStack_88[2] = 4.90454e-43;
    afStack_88[3] = 6.72623e-43;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar11 = &DAT_00501908;
    if (piStack_a8 != &DAT_00501908) {
      *piStack_a8 = *piStack_a8 + 1;
      piVar11 = piStack_a8;
    }
    FUN_0048c780((void *)(param_1 + 0xb38),piVar11,afStack_88,1,0);
    fVar12 = *(float *)(param_1 + 0x9b8);
    fVar14 = 50.0;
    pfVar4 = afStack_94;
    pvVar6 = (void *)FUN_0049e770(auStack_6c,afStack_a4);
    pfVar4 = (float *)FUN_0042d2b0(pvVar6,pfVar4);
    pfVar4 = (float *)FUN_00446390(afStack_18,pfVar4,fVar12);
    puVar5 = (undefined4 *)FUN_00446390(afStack_24,pfVar4,fVar14);
    fVar12 = (float)_DAT_004e0eb8;
    *(undefined4 *)(param_1 + 0xc4c) = *puVar5;
    *(undefined4 *)(param_1 + 0xc50) = puVar5[1];
    *(undefined4 *)(param_1 + 0xc54) = puVar5[2];
    if ((float)iVar1 * fVar13 * fVar12 <= afStack_a4[3]) {
      pfVar4 = afStack_a4;
      *(undefined1 *)(param_1 + 0xc3c) = 1;
      pfVar7 = (float *)FUN_00485c40(pvVar9,afStack_24);
      pfVar8 = (float *)FUN_00485ca0(pvVar9,afStack_18);
      pvVar9 = (void *)FUN_0043b2d0(afStack_88,pfVar8,pfVar7);
      FUN_0045a230(pvVar9,pfVar4);
      *(float *)(param_1 + 0xc40) = afStack_a4[0];
      afStack_a4[1] = 0.0;
      *(undefined4 *)(param_1 + 0xc44) = 0;
      *(float *)(param_1 + 0xc48) = afStack_a4[2];
    }
    else {
      *(undefined1 *)(param_1 + 0xc3c) = 0;
    }
    uStack_4 = 0xffffffff;
    if ((piStack_a8 != &DAT_00501908) &&
       (iVar1 = *piStack_a8, *piStack_a8 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&piStack_a8);
    }
  }
  ExceptionList = pvStack_c;
  return;
}

