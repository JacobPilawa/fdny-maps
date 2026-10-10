/* Ghidra decompilation; names/types are provisional.
 * Entry: 0043dc60
 * Function: FUN_0043dc60
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0043dc60(void *param_1)

{
  char *pcVar1;
  undefined1 *puVar2;
  float fVar3;
  float fVar4;
  char cVar5;
  void *pvVar6;
  undefined4 uVar7;
  int iVar8;
  int iVar9;
  float *pfVar10;
  int *piVar11;
  float10 fVar12;
  int *piVar13;
  int **ppiVar14;
  int *piVar15;
  int *piStack_d8;
  float fStack_d4;
  undefined4 uStack_d0;
  undefined4 uStack_cc;
  int iStack_c8;
  char *pcStack_c4;
  char *pcStack_c0;
  undefined4 uStack_bc;
  float fStack_b0;
  char *pcStack_ac;
  char *pcStack_a8;
  undefined4 uStack_a4;
  int iStack_98;
  char *pcStack_94;
  char *pcStack_90;
  undefined4 uStack_8c;
  undefined1 auStack_80 [48];
  undefined1 auStack_50 [68];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004ceadb;
  pvStack_c = ExceptionList;
  if (*(int *)((int)param_1 + 0x1a0) < 1) {
    return;
  }
  ExceptionList = &pvStack_c;
  *(undefined4 *)((int)param_1 + 0x70) = 0;
  piVar11 = (int *)((int)param_1 + -0x24);
  FUN_0043c1a0(piVar11,(float)_DAT_004dfa28 <
                       ABS(*(float *)((int)param_1 + 0x1b4) - *(float *)((int)param_1 + 0x1c4)));
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_d8 = &DAT_00501908;
  FUN_00401d80(&piStack_d8,"DBG_CLOSE_ALL_AREAS");
  ppiVar14 = &piStack_d8;
  uStack_4 = 0;
  pvVar6 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar7 = FUN_00438550(pvVar6,(int *)ppiVar14);
  uStack_4 = 0xffffffff;
  if ((piStack_d8 != &DAT_00501908) && (*piStack_d8 = *piStack_d8 + -1, *piStack_d8 < 1)) {
    FUN_004011c0(&piStack_d8);
  }
  if ((char)uVar7 != '\0') {
    FUN_0043c1a0(piVar11,0);
  }
  piVar13 = (int *)((int)param_1 + 0x13c);
  FUN_00470840((void *)(-(uint)(param_1 != (void *)0x24) & (uint)param_1),piVar13);
  iVar8 = FUN_00490300(piVar13,0);
  if (iVar8 == 0) {
    iVar8 = FUN_004903a0(piVar13,0);
    if (iVar8 != 0) {
      *(undefined1 *)((int)param_1 + 0x218) = 0;
    }
  }
  else {
    *(undefined1 *)((int)param_1 + 0x218) = 1;
  }
  if (*(int *)((int)param_1 + 0x234) != 0) {
    if (*(char *)((int)param_1 + 0x218) == '\0') {
      FUN_00459090((int)param_1 + 0x7c);
    }
    else {
      FUN_00459050((int)param_1 + 0x7c);
    }
    FUN_0043d330(piVar11);
  }
  if ((((*(int *)(*(int *)((int)param_1 + 200) + 0x34) != 0) &&
       (*(int *)(*(int *)((int)param_1 + 200) + 0x38) == 0)) &&
      (((float)_DAT_004e0b68 <= *(float *)((int)param_1 + 0x1c8) ||
       (*(float *)((int)param_1 + 0x1c8) <= (float)_DAT_004e0b60)))) &&
     (*(char *)((int)param_1 + 0x218) != '\0')) {
    piVar15 = &iStack_c8;
    pvVar6 = (void *)(**(code **)(*(int *)DAT_00503a78[0xc] + 0x18))();
    FUN_0049e770(pvVar6,piVar15);
    iVar8 = (**(code **)(*(int *)DAT_00503a78[0xc] + 0x18))();
    fStack_d4 = *(float *)(iVar8 + 0x28);
    uStack_d0 = *(undefined4 *)(iVar8 + 0x2c);
    uStack_cc = *(undefined4 *)(iVar8 + 0x30);
    iVar8 = (**(code **)(*piVar11 + 0x18))();
    FUN_004977b0(auStack_50,iVar8);
    pfVar10 = &fStack_d4;
    iVar8 = (**(code **)(*piVar11 + 0x18))();
    FUN_0043b2d0(&fStack_b0,(float *)(iVar8 + 0x28),pfVar10);
    fVar12 = FUN_00497aa0();
    if (fVar12 < (float10)_DAT_004df224) {
      cVar5 = '\x01';
      iVar8 = (**(code **)(*piVar11 + 0x18))();
      cVar5 = FUN_00490130(&fStack_d4,(undefined4 *)(iVar8 + 0x28),cVar5);
      if (cVar5 != '\0') {
        *(undefined1 *)(DAT_00503a78[0xc] + 0x5d4) = 1;
      }
    }
  }
  FUN_00490430(piVar13);
  FUN_0043aed0((int)piVar13);
  iVar8 = *(int *)((int)param_1 + 0x4c);
  if (iVar8 == 0) {
    ExceptionList = pvStack_c;
    return;
  }
  piStack_d8 = *(int **)((int)param_1 + 0x1b4);
  if (iVar8 == 5) {
    fVar3 = DAT_00502b04 * *(float *)((int)param_1 + 0x1b8) + *(float *)((int)param_1 + 0x1b4);
    *(float *)((int)param_1 + 0x1b4) = fVar3;
    if (*(float *)((int)param_1 + 0x1c8) <= fVar3) {
      do {
        *(float *)((int)param_1 + 0x1b4) =
             *(float *)((int)param_1 + 0x1b4) -
             (*(float *)((int)param_1 + 0x1c8) - *(float *)((int)param_1 + 0x1c4));
      } while (*(float *)((int)param_1 + 0x1c8) <= *(float *)((int)param_1 + 0x1b4));
    }
    if (*(float *)((int)param_1 + 0x1b4) < *(float *)((int)param_1 + 0x1c4) !=
        (*(float *)((int)param_1 + 0x1b4) == *(float *)((int)param_1 + 0x1c4))) {
      do {
        *(float *)((int)param_1 + 0x1b4) =
             (*(float *)((int)param_1 + 0x1c8) - *(float *)((int)param_1 + 0x1c4)) +
             *(float *)((int)param_1 + 0x1b4);
      } while (*(float *)((int)param_1 + 0x1b4) < *(float *)((int)param_1 + 0x1c4) !=
               (*(float *)((int)param_1 + 0x1b4) == *(float *)((int)param_1 + 0x1c4)));
    }
    goto LAB_0043e05c;
  }
  if (iVar8 == 3) {
    fVar3 = DAT_00502b04 * *(float *)((int)param_1 + 0x1b8) + *(float *)((int)param_1 + 0x1b4);
    *(float *)((int)param_1 + 0x1b4) = fVar3;
    if (*(float *)((int)param_1 + 0x1c8) <= fVar3) goto LAB_0043e04d;
    if (fVar3 < *(float *)((int)param_1 + 0x1c4) == (fVar3 == *(float *)((int)param_1 + 0x1c4)))
    goto LAB_0043e05c;
    *(undefined4 *)((int)param_1 + 0x1b4) = *(undefined4 *)((int)param_1 + 0x1c4);
  }
  else {
    if (iVar8 != 4) goto LAB_0043e05c;
    fVar3 = *(float *)((int)param_1 + 0x1b4) - DAT_00502b04 * *(float *)((int)param_1 + 0x1b8);
    *(float *)((int)param_1 + 0x1b4) = fVar3;
    if (fVar3 < *(float *)((int)param_1 + 0x1c4) != (fVar3 == *(float *)((int)param_1 + 0x1c4))) {
      *(undefined4 *)((int)param_1 + 0x1b4) = *(undefined4 *)((int)param_1 + 0x1c4);
      *(undefined4 *)((int)param_1 + 0x4c) = 0;
    }
    if (*(float *)((int)param_1 + 0x1b4) < *(float *)((int)param_1 + 0x1c8)) goto LAB_0043e05c;
LAB_0043e04d:
    *(undefined4 *)((int)param_1 + 0x1b4) = *(undefined4 *)((int)param_1 + 0x1c8);
  }
  *(undefined4 *)((int)param_1 + 0x4c) = 0;
LAB_0043e05c:
  iVar8 = 0;
  if (0 < *(int *)((int)param_1 + 0x1a0)) {
    do {
      if ((iVar8 < 0) || (*(int *)((int)param_1 + 0x1a0) <= iVar8)) {
        pfVar10 = &fStack_b0;
        for (iVar9 = 6; iVar9 != 0; iVar9 = iVar9 + -1) {
          *pfVar10 = 0.0;
          pfVar10 = pfVar10 + 1;
        }
        fStack_b0 = 1.4013e-45;
        pcStack_ac = "Array index out of bounds";
        pcStack_a8 = "..\\BluelAPI/Container/SArray.h";
        uStack_a4 = 0x1cd;
        FUN_00422060((int *)&fStack_b0);
      }
      if ((iVar8 < 0) || (*(int *)((int)param_1 + 0x1a0) <= iVar8)) {
        piVar11 = &iStack_c8;
        for (iVar9 = 6; iVar9 != 0; iVar9 = iVar9 + -1) {
          *piVar11 = 0;
          piVar11 = piVar11 + 1;
        }
        iStack_c8 = 1;
        pcStack_c4 = "Array index out of bounds";
        pcStack_c0 = "..\\BluelAPI/Container/SArray.h";
        uStack_bc = 0x1cd;
        FUN_00422060(&iStack_c8);
      }
      iVar9 = geWorld_ModelGetMotion();
      if (iVar9 != 0) {
        uVar7 = geMotion_GetPath(iVar9,0);
        gePath_Sample(uVar7,auStack_80);
        iVar9 = DAT_00503a78[1];
        if (*(char *)(iVar9 + 0x160) == '\0') {
          iVar9 = iVar9 + 8;
        }
        else {
          iVar9 = iVar9 + 0xb4;
        }
        geWorld_SetModelXForm(*(undefined4 *)(iVar9 + 0x78));
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 < *(int *)((int)param_1 + 0x1a0));
  }
  if (*(int *)(*(int *)((int)param_1 + 200) + 0x38) != 0) {
    FUN_0043b7a0((int)param_1 + -0x24);
  }
  iVar8 = 0;
  if (0 < *(int *)((int)param_1 + 0x128)) {
    do {
      if ((iVar8 < 0) || (*(int *)((int)param_1 + 0x128) <= iVar8)) {
        piVar11 = &iStack_98;
        for (iVar9 = 6; iVar9 != 0; iVar9 = iVar9 + -1) {
          *piVar11 = 0;
          piVar11 = piVar11 + 1;
        }
        iStack_98 = 1;
        pcStack_94 = "Array index out of bounds";
        pcStack_90 = "..\\BluelAPI/Container/SArray.h";
        uStack_8c = 0x1cd;
        FUN_00422060(&iStack_98);
      }
      pcVar1 = *(char **)(*(int *)((int)param_1 + 0x124) + iVar8 * 4);
      if ((*pcVar1 != '\0') &&
         (fVar3 = ABS(*(float *)((int)param_1 + 0x1b4) - *(float *)(pcVar1 + 4)),
         fVar4 = ABS(*(float *)((int)param_1 + 0x1b4) - (float)piStack_d8),
         fVar4 < fVar3 == (fVar4 == fVar3))) {
        puVar2 = *(undefined1 **)(pcVar1 + 0xc);
        piStack_d8 = (int *)&stack0xffffff10;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar11 = *(int **)(pcVar1 + 8);
        piVar13 = &DAT_00501908;
        if (piVar11 != &DAT_00501908) {
          *piVar11 = *piVar11 + 1;
          piVar13 = piVar11;
        }
        FUN_00475310(param_1,piVar13,puVar2);
        *pcVar1 = '\0';
        ExceptionList = pvStack_c;
        return;
      }
      iVar8 = iVar8 + 1;
    } while (iVar8 < *(int *)((int)param_1 + 0x128));
  }
  ExceptionList = pvStack_c;
  return;
}

