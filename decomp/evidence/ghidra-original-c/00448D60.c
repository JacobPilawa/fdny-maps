/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00448d60
 * Match: scratch-defined function boundary
 * Entry: 00448d60 FUN_00448d60
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00448d60(void *param_1)

{
  void *this;
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  float *pfVar4;
  int *extraout_ECX;
  float10 fVar5;
  ulonglong uVar6;
  int *piVar7;
  float fVar8;
  int *piStack_68;
  float fStack_64;
  float fStack_60;
  float fStack_5c;
  float fStack_58;
  float fStack_54;
  float fStack_50;
  char *apcStack_4c [3];
  float fStack_40;
  undefined4 uStack_3c;
  undefined4 uStack_38;
  float afStack_34 [4];
  float fStack_24;
  float fStack_20;
  float fStack_1c;
  int iStack_18;
  int iStack_14;
  int iStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004cf933;
  pvStack_c = ExceptionList;
  DAT_00502b8c = DAT_00502b8c + 1;
  if (3 < DAT_00502b8c) {
    DAT_00502b8c = 0;
  }
  ExceptionList = &pvStack_c;
  *(char *)((int)param_1 + 0x11c) = (char)DAT_00502b8c;
  this = (void *)((int)param_1 + 300);
  FUN_0044b270(this,*(void **)(DAT_00503a78 + 4),*(int *)(*(int *)((int)param_1 + 0x78) + 0x20) != 0
              );
  *(undefined4 *)((int)param_1 + 0x114) = 0;
  if (*(int *)(*(int *)((int)param_1 + 0x78) + 4) != 0) {
    puVar2 = (undefined4 *)FUN_0049d350(param_1,&fStack_64);
    piStack_68 = (int *)&stack0xffffff80;
    piVar7 = extraout_ECX;
    FUN_00402790(&stack0xffffff80,*(char **)(*(int *)((int)param_1 + 0x78) + 4));
    FUN_0046e320((void *)((int)param_1 + 0x24),piVar7,puVar2);
    piStack_68 = operator_new(0x24);
    uStack_4 = 0;
    if (piStack_68 == (int *)0x0) {
      puVar2 = (undefined4 *)0x0;
    }
    else {
      puVar2 = FUN_004a13a0(piStack_68);
    }
    uStack_4 = 0xffffffff;
    *(undefined4 **)((int)param_1 + 0x114) = puVar2;
    apcStack_4c[0] = "FireBurn1";
    apcStack_4c[1] = "FireBurn2";
    apcStack_4c[2] = "FireBurn3";
    FUN_00498b30(0.0,2.999);
    uVar6 = FUN_004b9400();
    FUN_00402790(&piStack_68,apcStack_4c[(int)uVar6]);
    uStack_4 = 1;
    FUN_004a12f0(*(void **)((int)param_1 + 0x114),&piStack_68,(int)param_1 + 0x7c,2);
    uStack_4 = 0xffffffff;
    if ((piStack_68 != &DAT_00501908) && (*piStack_68 = *piStack_68 + -1, *piStack_68 < 1)) {
      FUN_004011c0(&piStack_68);
    }
  }
  *(undefined4 *)(*(int *)((int)param_1 + 0x78) + 0x24) = 1;
  iVar3 = *(int *)((int)param_1 + 0x78);
  *(undefined4 *)((int)param_1 + 0x128) = 0x3f800000;
  *(undefined1 *)((int)param_1 + 0x104) = 0;
  *(undefined4 *)((int)param_1 + 0x100) = 0;
  if (*(int *)(iVar3 + 8) == 0) {
    piStack_68 = *(int **)(iVar3 + 0x10);
    fStack_58 = *(float *)(iVar3 + 0x7c);
    fStack_54 = *(float *)(iVar3 + 0x80);
    fStack_50 = *(float *)(iVar3 + 0x84);
    FUN_0042d280((void *)((int)param_1 + 0x108),&fStack_40,piStack_68,*(undefined4 *)(iVar3 + 0x14),
                 piStack_68);
  }
  else {
    FUN_00481950(afStack_34);
    iVar3 = *(int *)(DAT_00503a78 + 4);
    if (*(char *)(iVar3 + 0x160) == '\0') {
      iVar3 = iVar3 + 8;
    }
    else {
      iVar3 = iVar3 + 0xb4;
    }
    FUN_004818c0(afStack_34,*(undefined4 *)(iVar3 + 0x78),
                 *(int *)(*(int *)((int)param_1 + 0x78) + 8));
    fVar8 = 0.5;
    pfVar4 = (float *)FUN_0042d050(&fStack_40,afStack_34,afStack_34 + 3);
    pfVar4 = (float *)FUN_00446390((float *)apcStack_4c,pfVar4,fVar8);
    fStack_58 = *pfVar4;
    fStack_50 = pfVar4[2];
    fStack_54 = afStack_34[1];
    FUN_0042d280((void *)((int)param_1 + 0x108),&fStack_40,afStack_34[3] - afStack_34[0],
                 fStack_24 - afStack_34[1],fStack_20 - afStack_34[2]);
    geWorld_ModelSetFlags();
  }
  iVar3 = *(int *)((int)param_1 + 0x78);
  if (*(int *)(iVar3 + 0x6c) != 0) {
    apcStack_4c[0] = (char *)0x0;
    apcStack_4c[1] = (char *)0x0;
    apcStack_4c[2] = (char *)0x0;
    FUN_0042d280(apcStack_4c,&fStack_40,0,0x3f800000,0);
    fStack_64 = *(float *)(iVar3 + 0x70);
    fStack_60 = *(float *)(iVar3 + 0x74);
    fStack_5c = *(float *)(iVar3 + 0x78);
    fVar8 = _DAT_004dfc24 /
            SQRT(fStack_64 * fStack_64 + fStack_60 * fStack_60 + fStack_5c * fStack_5c);
    fStack_64 = fStack_64 * fVar8;
    fStack_60 = fStack_60 * fVar8;
    fStack_5c = fStack_5c * fVar8;
    puVar2 = (undefined4 *)FUN_004476b0(afStack_34,&fStack_64,(float *)apcStack_4c);
    uStack_3c = puVar2[1];
    uStack_38 = puVar2[2];
    uVar1 = *puVar2;
    iVar3 = FUN_0044a6a0((int)this);
    *(undefined4 *)(iVar3 + 4) = uVar1;
    *(undefined4 *)(iVar3 + 8) = uStack_3c;
    *(undefined4 *)(iVar3 + 0xc) = uStack_38;
    iVar3 = FUN_0044a6a0((int)this);
    *(char **)(iVar3 + 0x10) = apcStack_4c[0];
    *(char **)(iVar3 + 0x14) = apcStack_4c[1];
    *(char **)(iVar3 + 0x18) = apcStack_4c[2];
    iVar3 = FUN_0044a6a0((int)this);
    *(float *)(iVar3 + 0x1c) = fStack_64;
    *(float *)(iVar3 + 0x20) = fStack_60;
    *(float *)(iVar3 + 0x24) = fStack_5c;
  }
  iVar3 = *(int *)((int)param_1 + 0x78);
  if (*(int *)(iVar3 + 0x28) == 0) {
    fStack_64 = *(float *)((int)param_1 + 0x108) * (float)_DAT_004e1430;
    fStack_5c = *(float *)((int)param_1 + 0x110) * (float)_DAT_004e1430;
    fStack_60 = *(float *)((int)param_1 + 0x10c) * (float)_DAT_004e0508;
    FUN_0044b620(this,&fStack_64,DAT_00502afc);
  }
  else {
    fStack_1c = (float)(iVar3 + 0x2c);
    iStack_18 = iVar3 + 0x3c;
    iStack_14 = iVar3 + 0x4c;
    iStack_10 = iVar3 + 0x5c;
    afStack_34[0] = *(float *)(iVar3 + 0x38);
    afStack_34[1] = *(float *)(iVar3 + 0x48);
    afStack_34[3] = *(float *)(iVar3 + 0x68);
    fStack_64 = -999.0;
    fStack_60 = -999.0;
    fStack_5c = -999.0;
    afStack_34[2] = *(float *)(iVar3 + 0x58);
    piStack_68 = (int *)0x0;
    if (0 < *(int *)(iVar3 + 0x28)) {
      do {
        pfVar4 = (float *)(&fStack_1c)[(int)piStack_68];
        FUN_0044b620(this,pfVar4,(int)afStack_34[(int)piStack_68] + DAT_00502afc);
        if (fStack_64 < *pfVar4) {
          fStack_64 = *pfVar4;
        }
        if (fStack_60 < pfVar4[1]) {
          fStack_60 = pfVar4[1];
        }
        if (fStack_5c < pfVar4[2]) {
          fStack_5c = pfVar4[2];
        }
        piStack_68 = (int *)((int)piStack_68 + 1);
      } while ((int)piStack_68 < *(int *)(*(int *)((int)param_1 + 0x78) + 0x28));
    }
    *(float *)((int)param_1 + 0x108) = fStack_64;
    *(float *)((int)param_1 + 0x10c) = fStack_60;
    *(float *)((int)param_1 + 0x110) = fStack_5c;
  }
  iVar3 = FUN_0044a6a0((int)this);
  *(float *)(iVar3 + 0x28) = fStack_58;
  *(float *)(iVar3 + 0x2c) = fStack_54;
  *(float *)(iVar3 + 0x30) = fStack_50;
  fStack_40 = 0.0;
  uStack_3c = 0x40800000;
  uStack_38 = 0;
  puVar2 = (undefined4 *)FUN_0042d050(&fStack_1c,&fStack_58,&fStack_40);
  *(undefined4 *)((int)param_1 + 0x7c) = *puVar2;
  *(undefined4 *)((int)param_1 + 0x80) = puVar2[1];
  *(undefined4 *)((int)param_1 + 0x84) = puVar2[2];
  *(undefined4 *)((int)param_1 + 0x88) = *(undefined4 *)(*(int *)((int)param_1 + 0x78) + 0x10);
  if (*(int *)(*(int *)((int)param_1 + 0x78) + 0x1c) != 0) {
    FUN_00448630((int)param_1);
  }
  if (*(int **)((int)param_1 + 0x114) != (int *)0x0) {
    FUN_004a11d0(*(int **)((int)param_1 + 0x114));
  }
  *(undefined4 *)((int)param_1 + 0x118) = 0;
  *(undefined4 *)((int)param_1 + 0x124) = 0;
  *(undefined4 *)((int)param_1 + 0x120) = 0x47c34f80;
  iVar3 = *(int *)((int)param_1 + 0x78);
  if ((*(float *)(iVar3 + 0x18) < (float)_DAT_004e0b68) &&
     ((float)_DAT_004e0b60 < *(float *)(iVar3 + 0x18))) {
    fVar5 = FUN_00447770(*(float *)(iVar3 + 0x10),24.0,288.0);
    *(float *)(iVar3 + 0x18) = (float)(fVar5 * (float10)_DAT_004e16c4 + (float10)_DAT_004dfc24);
    ExceptionList = pvStack_c;
    return;
  }
  ExceptionList = pvStack_c;
  return;
}


