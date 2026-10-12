/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00452570
 * Match: scratch-defined function boundary
 * Entry: 00452570 FUN_00452570
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00452570(int param_1)

{
  char cVar1;
  void *pvVar2;
  undefined4 uVar3;
  uint uVar4;
  int *piVar5;
  int **ppiVar6;
  undefined4 *puVar7;
  char *pcVar8;
  float *pfVar9;
  float *pfVar10;
  float *pfVar11;
  int *extraout_ECX;
  undefined4 extraout_ECX_00;
  undefined4 extraout_ECX_01;
  uint3 uVar13;
  int *piVar12;
  int iVar14;
  float10 fVar15;
  ulonglong uVar16;
  undefined4 uVar17;
  undefined4 uVar18;
  float fVar19;
  int *piStack_dc;
  int *piStack_d8;
  float fStack_d4;
  float fStack_d0;
  float fStack_cc;
  int *piStack_c8;
  float fStack_c4;
  float fStack_c0;
  float fStack_bc;
  float *pfStack_b8;
  void *pvStack_b4;
  float afStack_b0 [3];
  float fStack_a4;
  char *pcStack_a0;
  char *pcStack_9c;
  undefined4 uStack_98;
  float afStack_8c [3];
  float afStack_80 [3];
  float afStack_74 [3];
  undefined4 auStack_68 [3];
  float afStack_5c [3];
  undefined1 auStack_50 [68];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d0490;
  pvStack_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_d8 = &DAT_00501908;
  ExceptionList = &pvStack_c;
  FUN_00401d80(&piStack_d8,"DBG_TOGGLE_SPECT");
  ppiVar6 = &piStack_d8;
  uStack_4 = 0;
  pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar3 = FUN_00438550(pvVar2,(int *)ppiVar6);
  uStack_4 = 0xffffffff;
  if ((piStack_d8 != &DAT_00501908) && (*piStack_d8 = *piStack_d8 + -1, *piStack_d8 < 1)) {
    FUN_004011c0(&piStack_d8);
  }
  if ((char)uVar3 != '\0') {
    *(undefined1 *)(param_1 + 0x240) = 1;
    *(undefined4 *)(param_1 + 0x4c) = 3;
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_d8 = &DAT_00501908;
  FUN_00401d80(&piStack_d8,"DBG_WIN_LEVEL");
  ppiVar6 = &piStack_d8;
  uStack_4 = 1;
  pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar3 = FUN_00438550(pvVar2,(int *)ppiVar6);
  uStack_4 = 0xffffffff;
  if ((piStack_d8 != &DAT_00501908) && (*piStack_d8 = *piStack_d8 + -1, *piStack_d8 < 1)) {
    FUN_004011c0(&piStack_d8);
  }
  if ((char)uVar3 != '\0') {
    *(int *)(param_1 + 0x33ec) = *(int *)(param_1 + 0x33e8) - *(int *)(param_1 + 0x33f0);
    FUN_004504c0((void *)(param_1 + -0x140));
  }
  if (*(char *)(param_1 + 0x240) != '\0') {
    piStack_c8 = (int *)0x42f00000;
    ppiVar6 = (int **)(param_1 + 0x32fc);
    if (*(float *)(param_1 + 0x32fc) <= _DAT_004e1c3c) {
      ppiVar6 = &piStack_c8;
    }
    *(int **)(param_1 + 0x32fc) = *ppiVar6;
    *(undefined4 *)(param_1 + 0x33f8) = 0;
    *(undefined4 *)(param_1 + 0x3310) = 0x3f800000;
  }
  *(float *)(param_1 + 0x32fc) = *(float *)(param_1 + 0x32fc) - DAT_00502b04;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_d8 = &DAT_00501908;
  FUN_00401d80(&piStack_d8,"Completion Time");
  uStack_4 = 2;
  uVar16 = FUN_004b9400();
  FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&piStack_d8,(int)uVar16);
  uStack_4 = 0xffffffff;
  if ((piStack_d8 != &DAT_00501908) && (*piStack_d8 = *piStack_d8 + -1, *piStack_d8 < 1)) {
    FUN_004011c0(&piStack_d8);
  }
  uVar4 = FUN_00481f40((void *)(param_1 + 0x12f4),*(int **)(param_1 + 0x32fc),0x1e);
  if ((char)uVar4 != '\0') {
    cVar1 = '\x01';
    pfStack_b8 = (float *)&stack0xffffff0c;
    piVar5 = extraout_ECX;
    FUN_00402790(&stack0xffffff0c,"RC_ResTimeWarn");
    FUN_004897c0((void *)(param_1 + 0x3400),piVar5,cVar1);
  }
  if (*(int *)(param_1 + 0x33e8) == -1) {
    FUN_0044fba0(DAT_00503a78[0xc]);
  }
  piVar5 = *(int **)(param_1 + 0x33f0);
  if ((*(int *)(param_1 + 0x33e8) - (int)piVar5 == *(int *)(param_1 + 0x33ec)) &&
     (*(uint *)(param_1 + 0x48c) < DAT_00502afc)) {
    pfStack_b8 = (float *)&stack0xffffff10;
    *(uint *)(param_1 + 0x48c) = DAT_00502afc + 30000;
    FUN_00401cb0(&stack0xffffff10,(undefined4 *)(param_1 + 0x490));
    FUN_004564c0((void *)(param_1 + 0x4b4),5.0,piVar5);
  }
  if (_DAT_004dc50c <= *(float *)(param_1 + 0x32fc)) {
    if (*(char *)(param_1 + -0x14) == '\0') {
      FUN_00402790(&piStack_dc,"Air Time");
      uStack_4 = 3;
      uVar16 = FUN_004b9400();
      FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&piStack_dc,(int)uVar16);
      uStack_4 = 0xffffffff;
      if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
        FUN_004011c0(&piStack_dc);
      }
    }
    FUN_004467e0(param_1 + 0x41c);
    fVar19 = *(float *)(param_1 + 0x33f8) * (float)_DAT_004e17d8 +
             (_DAT_004dfc24 - *(float *)(param_1 + 0x3310));
    if ((float)_DAT_004e0508 < fVar19) {
      FUN_00481d90((int *)(param_1 + 0x1ec),fVar19 - (float)_DAT_004e0508);
      FUN_00481ce0((int *)(param_1 + 0x1ec));
    }
    if (*(int *)(param_1 + 0x4c) == 6) {
      FUN_00451080((int *)(param_1 + -0x140));
    }
    else {
      pvStack_b4 = (void *)(param_1 + 0x4b4);
      FUN_004559d0(pvStack_b4,*(char *)(param_1 + 0x494));
      *(undefined1 *)(param_1 + 0x494) = 0;
      if (((((*(char *)(param_1 + 0x33fc) == '\0') && (*(char *)(param_1 + 0x33fd) == '\0')) &&
           (*(char *)(param_1 + 0x33fe) == '\0')) &&
          ((*(char *)(param_1 + 0x33ff) == '\0' &&
           (fVar19 = *(float *)(param_1 + 0x33f8) - _DAT_004e1814 * DAT_00502b04,
           *(float *)(param_1 + 0x33f8) = fVar19, fVar19 < (float)_DAT_004df7b8)))) &&
         ((*(char *)(param_1 + -0x14) != '\0' &&
          ((float)_DAT_004de6f8 < *(float *)(param_1 + 0x3310))))) {
        FUN_00441e90((void *)(param_1 + 0x32c0),(undefined4 *)(param_1 + -0xf4));
      }
      if (*(float *)(param_1 + 0x33f8) < _DAT_004dc50c) {
        *(undefined4 *)(param_1 + 0x33f8) = 0;
      }
      *(undefined1 *)(param_1 + 0x33ff) = 0;
      *(undefined1 *)(param_1 + 0x33fe) = 0;
      *(undefined1 *)(param_1 + 0x33fd) = 0;
      *(undefined1 *)(param_1 + 0x33fc) = 0;
      if (((*(int *)(param_1 + 0x4c) == 3) && (*(char *)(param_1 + -0x14) != '\0')) &&
         ((*(char *)(param_1 + -0x13) == '\0' && (*(float *)(param_1 + -0x4c) < _DAT_004e1c28)))) {
        piStack_c8 = (int *)(-*(float *)(param_1 + -0x4c) * (float)_DAT_004e1858);
        FUN_00490e60((void *)(param_1 + 0x58),piStack_c8);
        if (*(int *)(param_1 + 0x24c) + 500U < DAT_00502afc) {
          *(uint *)(param_1 + 0x24c) = DAT_00502afc;
          FUN_004a11a0((void *)(param_1 + 0x2a8),(undefined1 *)0x0);
        }
      }
      FUN_00402790(&piStack_dc,"STEER_YAW");
      ppiVar6 = &piStack_dc;
      uStack_4 = 4;
      piVar5 = (int *)FUN_00499cb0(DAT_00503a78);
      fVar15 = FUN_004386f0(piVar5,(int *)ppiVar6);
      *(float *)(param_1 + 0x32f4) = (float)(fVar15 + (float10)*(float *)(param_1 + 0x32f4));
      uStack_4 = 0xffffffff;
      if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
        FUN_004011c0(&piStack_dc);
      }
      if (_DAT_004e1c24 < *(float *)(param_1 + 0x32f4)) {
        fVar19 = *(float *)(param_1 + 0x32f4);
        do {
          fVar19 = fVar19 - _DAT_004e1c24;
        } while (_DAT_004e1c24 < fVar19);
        *(float *)(param_1 + 0x32f4) = fVar19;
      }
      if (*(float *)(param_1 + 0x32f4) < _DAT_004e1c20) {
        fVar19 = *(float *)(param_1 + 0x32f4);
        do {
          fVar19 = fVar19 + _DAT_004e1c24;
        } while (fVar19 < _DAT_004e1c20);
        *(float *)(param_1 + 0x32f4) = fVar19;
      }
      FUN_00402790(&piStack_dc,"STEER_PITCH");
      ppiVar6 = &piStack_dc;
      uStack_4 = 5;
      piVar5 = (int *)FUN_00499cb0(DAT_00503a78);
      fVar15 = FUN_004386f0(piVar5,(int *)ppiVar6);
      *(float *)(param_1 + 0x32f0) = (float)(fVar15 + (float10)*(float *)(param_1 + 0x32f0));
      uStack_4 = 0xffffffff;
      if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
        FUN_004011c0(&piStack_dc);
      }
      if (_DAT_004e1c24 < *(float *)(param_1 + 0x32f0)) {
        fVar19 = *(float *)(param_1 + 0x32f0);
        do {
          fVar19 = fVar19 - _DAT_004e1c24;
        } while (_DAT_004e1c24 < fVar19);
        *(float *)(param_1 + 0x32f0) = fVar19;
      }
      if (*(float *)(param_1 + 0x32f0) < _DAT_004e1c20) {
        fVar19 = *(float *)(param_1 + 0x32f0);
        do {
          fVar19 = fVar19 + _DAT_004e1c24;
        } while (fVar19 < _DAT_004e1c20);
        *(float *)(param_1 + 0x32f0) = fVar19;
      }
      iVar14 = *(int *)(*(int *)(DAT_00502aac + 0x6e4) + 0x2c);
      piStack_d8 = (int *)0x46;
      piStack_dc = (int *)0x28;
      piStack_c8 = (int *)0x14;
      if (iVar14 == 0) {
        ppiVar6 = &piStack_c8;
      }
      else {
        ppiVar6 = &piStack_dc;
        if (iVar14 != 1) {
          ppiVar6 = &piStack_d8;
        }
      }
      piVar5 = (int *)(float)(int)*ppiVar6;
      piStack_c8 = (int *)(float)(int)*ppiVar6;
      if ((*(int *)(param_1 + 0x4c) == 5) || (*(char *)(param_1 + 0x240) != '\0')) {
        piStack_dc = (int *)0x42340000;
        ppiVar6 = &piStack_c8;
        if ((float)piVar5 <= _DAT_004e1c1c) {
          ppiVar6 = &piStack_dc;
        }
        piVar5 = *ppiVar6;
      }
      if ((float)piVar5 < *(float *)(param_1 + 0x32f0)) {
        *(int **)(param_1 + 0x32f0) = piVar5;
      }
      if (*(float *)(param_1 + 0x32f0) < -(float)piVar5) {
        *(float *)(param_1 + 0x32f0) = -(float)piVar5;
      }
      puVar7 = (undefined4 *)FUN_0049e770((void *)(param_1 + 0x3364),&fStack_c4);
      *(undefined4 *)(param_1 + 0x3304) = *puVar7;
      *(undefined4 *)(param_1 + 0x3308) = puVar7[1];
      *(undefined4 *)(param_1 + 0x330c) = puVar7[2];
      FUN_0042d350((void *)(param_1 + -0x11c),auStack_50);
      FUN_004968b0((void *)(param_1 + -0x11c),*(float *)(param_1 + 0x32f4));
      piStack_d8 = (int *)0x0;
      if (*(int *)(param_1 + 0x4c) != 7) {
        iVar14 = 0;
        if (0 < *(int *)(param_1 + 0x3428)) {
          piVar5 = (int *)(param_1 + 0x3424);
          do {
            FUN_0048d490(piVar5,iVar14);
            if (*(int *)(*piVar5 + iVar14 * 4) != 0) {
              FUN_0048d490(piVar5,iVar14);
              pcVar8 = (char *)(**(code **)(**(int **)(*piVar5 + iVar14 * 4) + 0xc))();
              FUN_00402790(&piStack_d8,pcVar8);
              ppiVar6 = &piStack_d8;
              uStack_4 = 6;
              pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
              uVar3 = FUN_00438550(pvVar2,(int *)ppiVar6);
              uStack_4 = 0xffffffff;
              if (piStack_d8 != &DAT_00501908) {
                *piStack_d8 = *piStack_d8 + -1;
                if (*piStack_d8 < 1) {
                  FUN_004011c0(&piStack_d8);
                }
                else {
                  piStack_d8 = &DAT_00501908;
                }
              }
              if ((char)uVar3 != '\0') {
                uVar3 = FUN_0044c2a0((void *)(param_1 + -0x140),iVar14,'\0');
                if ((char)uVar3 == '\0') {
                  FUN_004a11a0((void *)(param_1 + 0x3c8),(undefined1 *)0x0);
                }
                break;
              }
            }
            iVar14 = iVar14 + 1;
          } while (iVar14 < *(int *)(param_1 + 0x3428));
        }
        fStack_d4 = 0.0;
        fStack_d0 = 0.0;
        fStack_cc = 0.0;
        if (*(char *)(param_1 + 0x331c) == '\0') {
          FUN_00402790(&piStack_dc,"MOVE_FORWARD");
          ppiVar6 = &piStack_dc;
          uStack_4 = 7;
          pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
          uVar3 = FUN_00438480(pvVar2,(int *)ppiVar6);
          uStack_4 = 0xffffffff;
          if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
            FUN_004011c0(&piStack_dc);
          }
          if ((char)uVar3 != '\0') {
            fStack_cc = fStack_cc + _DAT_004dfc24;
          }
          FUN_00402790(&piStack_dc,"MOVE_BACKWARD");
          ppiVar6 = &piStack_dc;
          uStack_4 = 8;
          pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
          uVar3 = FUN_00438480(pvVar2,(int *)ppiVar6);
          uStack_4 = 0xffffffff;
          if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
            FUN_004011c0(&piStack_dc);
          }
          if ((char)uVar3 != '\0') {
            fStack_cc = fStack_cc - _DAT_004dfc24;
          }
          FUN_00402790(&piStack_dc,"MOVE_LEFT");
          ppiVar6 = &piStack_dc;
          uStack_4 = 9;
          pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
          uVar3 = FUN_00438480(pvVar2,(int *)ppiVar6);
          uStack_4 = 0xffffffff;
          if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
            FUN_004011c0(&piStack_dc);
          }
          if ((char)uVar3 != '\0') {
            fStack_d4 = fStack_d4 + _DAT_004e1c18;
          }
          FUN_00402790(&piStack_dc,"MOVE_RIGHT");
          ppiVar6 = &piStack_dc;
          uStack_4 = 10;
          pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
          uVar3 = FUN_00438480(pvVar2,(int *)ppiVar6);
          uStack_4 = 0xffffffff;
          if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
            FUN_004011c0(&piStack_dc);
          }
          if ((char)uVar3 != '\0') {
            fStack_d4 = fStack_d4 - _DAT_004e1c18;
          }
        }
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piStack_d8 = &DAT_00501908;
        FUN_00401d80(&piStack_d8,"CROUCH");
        ppiVar6 = &piStack_d8;
        uStack_4 = 0xb;
        pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
        uVar3 = FUN_00438550(pvVar2,(int *)ppiVar6);
        uStack_4 = 0xffffffff;
        if ((piStack_d8 != &DAT_00501908) && (*piStack_d8 = *piStack_d8 + -1, *piStack_d8 < 1)) {
          FUN_004011c0(&piStack_d8);
        }
        if ((char)uVar3 != '\0') {
          if (*(int *)(param_1 + 0x4c) == 4) {
            FUN_0044bce0(param_1 + -0x140);
          }
          else if (*(int *)(param_1 + 0x4c) == 3) {
            FUN_0044bcb0(param_1 + -0x140);
          }
        }
        if ((*(int *)(param_1 + 0x4c) == 5) || (*(char *)(param_1 + 0x240) != '\0')) {
          uVar18 = 0x447a0000;
          uVar17 = 0x447a0000;
          uVar3 = 0x447a0000;
        }
        else if (*(char *)(param_1 + -0x14) == '\0') {
          uVar18 = 0;
          uVar17 = 0;
          uVar3 = 0;
        }
        else {
          uVar18 = 0x447a0000;
          uVar17 = 0;
          uVar3 = 0x447a0000;
        }
        FUN_0042d280((void *)(param_1 + -0x2c),&fStack_c4,uVar3,uVar17,uVar18);
        piStack_dc = (int *)0x43c80000;
        if (*(int *)(param_1 + 0x4c) == 4) {
          piStack_dc = (int *)0x43820000;
        }
        if (*(char *)(param_1 + 0x240) != '\0') {
          piStack_dc = (int *)0x44480000;
        }
        piStack_d8 = (int *)SQRT(fStack_d0 * fStack_d0 +
                                 fStack_cc * fStack_cc + fStack_d4 * fStack_d4);
        if ((float)_DAT_004e1c10 < (float)piStack_d8) {
          if ((*(int *)(param_1 + 0x4c) == 5) || (*(char *)(param_1 + 0x240) != '\0')) {
            pfVar9 = &fStack_d4;
            pvVar2 = FUN_004977b0(auStack_50,param_1 + 0x3320);
            pfVar9 = (float *)FUN_00495eb0(&fStack_c4,(int)pvVar2,pfVar9);
            fStack_d4 = *pfVar9;
            fStack_d0 = pfVar9[1];
            fStack_cc = pfVar9[2];
          }
          else {
            pfVar9 = (float *)FUN_00495eb0(&fStack_c4,-(uint)(param_1 != 0x140) & param_1 - 0x11cU,
                                           &fStack_d4);
            fStack_d4 = *pfVar9;
            fStack_d0 = pfVar9[1];
            fStack_cc = pfVar9[2];
          }
          if ((float)_DAT_004dd808 < (float)piStack_d8) {
            fVar19 = (float)_DAT_004dd808 / (float)piStack_d8;
            fStack_d4 = fStack_d4 * fVar19;
            fStack_d0 = fStack_d0 * fVar19;
            fStack_cc = fStack_cc * fVar19;
          }
          if (((*(char *)(param_1 + -0x14) != '\0') || (*(int *)(param_1 + 0x4c) == 5)) ||
             (piStack_c8 = (int *)0x3f733333, *(char *)(param_1 + 0x240) != '\0')) {
            piStack_c8 = (int *)0x3e4ccccd;
          }
          pfStack_b8 = *(float **)(param_1 + -0x40);
          FUN_00482ad0((float)piStack_c8,27.0);
          pfVar9 = (float *)FUN_00446390(&fStack_c4,&fStack_d4,(float)piStack_dc);
          pfVar9 = FUN_00482fc0(afStack_b0,(float *)(param_1 + -0x44),pfVar9);
          *(float *)(param_1 + -0x44) = *pfVar9;
          *(float *)(param_1 + -0x40) = pfVar9[1];
          *(float *)(param_1 + -0x3c) = pfVar9[2];
          if (((*(int *)(param_1 + 0x4c) != 5) && (*(char *)(param_1 + 0x240) == '\0')) &&
             (*(float **)(param_1 + -0x40) = pfStack_b8, *(char *)(param_1 + 0x58) != '\0')) {
            FUN_0044ea90(param_1 + -0x140);
          }
        }
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piStack_dc = &DAT_00501908;
        FUN_00401d80(&piStack_dc,"USE");
        ppiVar6 = &piStack_dc;
        uStack_4 = 0xc;
        pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
        uVar3 = FUN_00438550(pvVar2,(int *)ppiVar6);
        uStack_4 = 0xffffffff;
        if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
          FUN_004011c0(&piStack_dc);
        }
        if (((char)uVar3 != '\0') && (uVar3 = FUN_00458fa0(param_1 + 0x3114), (char)uVar3 == '\0'))
        {
          FUN_0044c480((void *)(param_1 + -0x140));
        }
        if ((*(int *)(param_1 + 0x4c) == 3) && (*(int *)(param_1 + 0x343c) == 4)) {
          if (*(int *)(param_1 + 0x3428) < 5) {
            pfVar9 = &fStack_a4;
            for (iVar14 = 6; iVar14 != 0; iVar14 = iVar14 + -1) {
              *pfVar9 = 0.0;
              pfVar9 = pfVar9 + 1;
            }
            fStack_a4 = 1.4013e-45;
            pcStack_a0 = "Array index out of bounds";
            pcStack_9c = "..\\BluelAPI/Container/SArray.h";
            uStack_98 = 0x1cd;
            FUN_00422060((int *)&fStack_a4);
          }
          FUN_00485b00(*(int *)(*(int *)(param_1 + 0x3424) + 0x10) + 0xc70);
        }
        iVar14 = *(int *)(param_1 + 0x343c);
        if (iVar14 != 0) {
          if (*(char *)(param_1 + -0x14) != '\0') {
            piVar5 = (int *)(param_1 + 0x3424);
            if ((float)piStack_d8 <= (float)_DAT_004e1c10) {
              FUN_0048d490(piVar5,iVar14);
              pfVar9 = (float *)(**(code **)(**(int **)(*piVar5 + iVar14 * 4) + 0x34))();
              if ((float)_DAT_004e1c10 <
                  pfVar9[2] * pfVar9[2] + pfVar9[1] * pfVar9[1] + *pfVar9 * *pfVar9) {
                iVar14 = *(int *)(param_1 + 0x343c);
                FUN_0048d490(piVar5,iVar14);
                puVar7 = (undefined4 *)(**(code **)(**(int **)(*piVar5 + iVar14 * 4) + 0x34))();
                *(undefined4 *)(param_1 + -0x44) = *puVar7;
                *(undefined4 *)(param_1 + -0x40) = puVar7[1];
                *(undefined4 *)(param_1 + -0x3c) = puVar7[2];
              }
            }
            else {
              FUN_0048d490(piVar5,iVar14);
              pfVar9 = (float *)(**(code **)(**(int **)(*piVar5 + iVar14 * 4) + 0x34))();
              *(float *)(param_1 + -0x44) = *(float *)(param_1 + -0x44) + *pfVar9;
              *(float *)(param_1 + -0x40) = pfVar9[1] + *(float *)(param_1 + -0x40);
              *(float *)(param_1 + -0x3c) = pfVar9[2] + *(float *)(param_1 + -0x3c);
            }
          }
          iVar14 = *(int *)(param_1 + 0x343c);
          fStack_c4 = 0.0;
          fStack_c0 = 0.0;
          fStack_bc = 0.0;
          FUN_0048d490((int *)(param_1 + 0x3424),iVar14);
          cVar1 = (**(code **)(**(int **)(*(int *)(param_1 + 0x3424) + iVar14 * 4) + 0x30))();
          if (cVar1 != '\0') {
            fVar19 = fStack_c4 * *(float *)(param_1 + -0x38) +
                     fStack_c0 * *(float *)(param_1 + -0x34) +
                     fStack_bc * *(float *)(param_1 + -0x30);
            if (fVar19 < _DAT_004dc50c) {
              pfVar9 = (float *)FUN_00447710(afStack_b0,-fVar19,&fStack_c4);
              *(float *)(param_1 + -0x38) = *pfVar9 + *(float *)(param_1 + -0x38);
              *(float *)(param_1 + -0x34) = pfVar9[1] + *(float *)(param_1 + -0x34);
              *(float *)(param_1 + -0x30) = pfVar9[2] + *(float *)(param_1 + -0x30);
            }
            fVar19 = fStack_c4 * *(float *)(param_1 + -0x44) +
                     fStack_c0 * *(float *)(param_1 + -0x40) +
                     fStack_bc * *(float *)(param_1 + -0x3c);
            if (fVar19 < _DAT_004dc50c) {
              pfVar9 = (float *)FUN_00447710(afStack_b0,-fVar19,&fStack_c4);
              *(float *)(param_1 + -0x44) = *pfVar9 + *(float *)(param_1 + -0x44);
              *(float *)(param_1 + -0x40) = pfVar9[1] + *(float *)(param_1 + -0x40);
              *(float *)(param_1 + -0x3c) = pfVar9[2] + *(float *)(param_1 + -0x3c);
            }
          }
        }
        if ((*(int *)(param_1 + 0x4c) == 5) ||
           (fStack_c0 = -1000.0, *(char *)(param_1 + 0x240) != '\0')) {
          fStack_c0 = 0.0;
        }
        fStack_c4 = 0.0;
        *(undefined4 *)(param_1 + -0x38) = 0;
        fStack_bc = 0.0;
        *(float *)(param_1 + -0x34) = fStack_c0;
        *(undefined4 *)(param_1 + -0x30) = 0;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piStack_dc = &DAT_00501908;
        FUN_00401d80(&piStack_dc,"JUMP");
        ppiVar6 = &piStack_dc;
        uStack_4 = 0xd;
        pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
        uVar3 = FUN_00438480(pvVar2,(int *)ppiVar6);
        uStack_4 = 0xffffffff;
        if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
          FUN_004011c0(&piStack_dc);
        }
        if ((char)uVar3 != '\0') {
          if (*(char *)(param_1 + 0x240) != '\0') {
            *(undefined1 *)(param_1 + 0x240) = 0;
            *(undefined4 *)(param_1 + 0x4c) = 3;
          }
          if (*(int *)(param_1 + 0x4c) == 4) {
            FUN_00402790(&piStack_dc,"JUMP");
            ppiVar6 = &piStack_dc;
            uStack_4 = 0xe;
            pvVar2 = (void *)FUN_00499cb0(DAT_00503a78);
            uVar3 = FUN_00438550(pvVar2,(int *)ppiVar6);
            uStack_4 = 0xffffffff;
            if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1))
            {
              FUN_004011c0(&piStack_dc);
            }
            if ((char)uVar3 == '\0') {
              ExceptionList = pvStack_c;
              return;
            }
            FUN_0044bce0(param_1 + -0x140);
            ExceptionList = pvStack_c;
            return;
          }
          FUN_0044e990((int *)(param_1 + -0x140));
        }
      }
      pfStack_b8 = (float *)(param_1 + 0x478);
      pfVar9 = (float *)FUN_00446390(afStack_b0,pfStack_b8,DAT_00502b04);
      piVar5 = (int *)(param_1 + -0x140);
      *(float *)(param_1 + -0x44) = *pfVar9 + *(float *)(param_1 + -0x44);
      *(float *)(param_1 + -0x40) = pfVar9[1] + *(float *)(param_1 + -0x40);
      *(float *)(param_1 + -0x3c) = pfVar9[2] + *(float *)(param_1 + -0x3c);
      (**(code **)(*piVar5 + 100))();
      uVar3 = extraout_ECX_00;
      if (*(char *)(param_1 + 0x240) == '\0') {
        FUN_0043f6d0(piVar5);
        uVar3 = extraout_ECX_01;
      }
      *(float *)(param_1 + -0xf4) = *(float *)(param_1 + -0xb0) + *(float *)(param_1 + -0xf4);
      *(float *)(param_1 + -0xf0) = *(float *)(param_1 + -0xac) + *(float *)(param_1 + -0xf0);
      *(float *)(param_1 + -0xec) = *(float *)(param_1 + -0xa8) + *(float *)(param_1 + -0xec);
      uVar13 = (uint3)((uint)uVar3 >> 8);
      if ((float)piStack_d8 <= (float)_DAT_004e1c10) {
        piVar12 = (int *)((uint)uVar13 << 8);
      }
      else {
        piVar12 = (int *)CONCAT31(uVar13,1);
      }
      piStack_c8 = (int *)&stack0xfffffef8;
      FUN_0044f340(piVar5,piVar12,'\x01',1.0);
      FUN_00459630((void *)(param_1 + 0x3114));
      FUN_00455580((uint *)(param_1 + 0x32cc));
      if (*(int *)(param_1 + 0x3300) != 0) {
        fVar19 = 90.0;
        pfVar10 = afStack_b0;
        pfVar9 = (float *)(param_1 + 0x338c);
        pvVar2 = (void *)(**(code **)(*piVar5 + 0x18))();
        pfVar10 = (float *)FUN_0049e770(pvVar2,pfVar10);
        pfVar10 = (float *)FUN_00446390(afStack_5c,pfVar10,fVar19);
        fVar19 = -60.0;
        puVar7 = auStack_68;
        pvVar2 = (void *)(**(code **)(*piVar5 + 0x18))();
        pfVar11 = (float *)FUN_00447830(pvVar2,puVar7);
        pfVar11 = (float *)FUN_00446390(afStack_74,pfVar11,fVar19);
        pfVar11 = (float *)FUN_0042d050(afStack_8c,pfVar9,pfVar11);
        puVar7 = (undefined4 *)FUN_0042d050(afStack_80,pfVar11,pfVar10);
        iVar14 = *(int *)(param_1 + 0x3300);
        *(undefined4 *)(iVar14 + 0x224) = *puVar7;
        *(undefined4 *)(iVar14 + 0x228) = puVar7[1];
        *(undefined4 *)(iVar14 + 0x22c) = puVar7[2];
        FUN_0043bd90(*(void **)(param_1 + 0x3300));
        pfVar10 = (float *)FUN_0044bed0(*(void **)(param_1 + 0x3300),&fStack_a4);
        FUN_0043b2d0(&fStack_d4,pfVar10,pfVar9);
        FUN_0049e770((void *)(param_1 + 0x3364),&fStack_c4);
        fStack_d0 = 0.0;
        fStack_c0 = 0.0;
        fVar15 = FUN_00497aa0();
        if ((float10)_DAT_004e169c <= fVar15) {
          iVar14 = FUN_004476b0(&fStack_a4,&fStack_d4,&fStack_c4);
          pvVar2 = pvStack_b4;
          if (*(float *)(iVar14 + 4) <= _DAT_004dc50c) {
            iVar14 = 1;
          }
          else {
            iVar14 = 2;
          }
        }
        else {
          FUN_0043b580(*(int *)(param_1 + 0x3300));
          pvVar2 = pvStack_b4;
          FUN_004571a0(pvStack_b4,(int *)0x4,(undefined1 *)0x0);
          *(undefined4 *)(param_1 + 0x3300) = 0;
          *(undefined4 *)(param_1 + 0x4c) = 3;
          FUN_00402790(&piStack_dc,"Objects Thrown");
          uStack_4 = 0xf;
          FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&piStack_dc,1);
          uStack_4 = 0xffffffff;
          if ((piStack_dc != &DAT_00501908) && (*piStack_dc = *piStack_dc + -1, *piStack_dc < 1)) {
            FUN_004011c0(&piStack_dc);
          }
          iVar14 = 0;
        }
        FUN_004559e0(pvVar2,iVar14);
      }
      FUN_0042d280(pfStack_b8,&fStack_a4,0,0,0);
      FUN_00451560(piVar5);
    }
  }
  else {
    *(undefined4 *)(param_1 + 0x32fc) = 0;
    *(undefined4 *)(param_1 + 0x32f8) = 2;
  }
  ExceptionList = pvStack_c;
  return;
}


