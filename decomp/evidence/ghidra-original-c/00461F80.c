/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00461f80
 * Match: scratch-defined function boundary
 * Entry: 00461f80 FUN_00461f80
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00461f80(int *param_1)

{
  uint uVar1;
  float *pfVar2;
  undefined4 *puVar3;
  undefined4 *puVar4;
  void *pvVar5;
  undefined4 *puVar6;
  float10 fVar7;
  float10 fVar8;
  float10 fVar9;
  ulonglong uVar10;
  float *pfVar11;
  undefined4 uVar12;
  float fVar13;
  undefined4 uVar14;
  float fVar15;
  undefined4 uVar16;
  float fVar17;
  void *pvStack_a0;
  float fStack_9c;
  undefined4 *puStack_98;
  float fStack_94;
  float fStack_90;
  float fStack_8c;
  float fStack_88;
  float afStack_84 [3];
  uint uStack_78;
  undefined1 auStack_74 [40];
  float fStack_4c;
  float fStack_48;
  float fStack_44;
  float afStack_30 [3];
  float afStack_24 [3];
  float afStack_18 [3];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d1619;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  FUN_0045f600((int)param_1);
  if ((char)param_1[0x56] != '\0') {
    FUN_00482430(param_1 + 0x199,(float)param_1[0x232]);
    if ((char)param_1[0x180] != '\0') {
      fVar7 = FUN_00498b30(6.0,8.0);
      fVar7 = fVar7 * (float10)DAT_00502b04 + (float10)(float)param_1[0x181];
      param_1[0x18c] = 0x41000000;
      param_1[0x181] = (int)(float)fVar7;
      fVar8 = (float10)fcos((float10)_DAT_004de6f8 * fVar7);
      fVar9 = (float10)fsin(fVar7);
      fVar7 = (float10)_DAT_004de6f8;
      param_1[0x18a] = (int)(float)fVar8;
      param_1[0x18b] = (int)(float)(fVar9 * fVar7 - (float10)_DAT_004e2924);
      param_1[0x187] = (int)(float)(fVar8 + (float10)(float)param_1[0x184]);
      param_1[0x188] = (int)(float)(fVar9 * fVar7 + (float10)(float)param_1[0x185]);
      param_1[0x189] = (int)(float)(fVar8 + (float10)(float)param_1[0x186]);
      FUN_004977b0(auStack_74,(int)(param_1 + 0x16));
      FUN_00496b50(auStack_74,-90.0);
      fVar17 = 20.0;
      pfVar2 = (float *)FUN_0049e770(auStack_74,&fStack_90);
      pfVar2 = (float *)FUN_00446390(afStack_84,pfVar2,fVar17);
      fStack_4c = fStack_4c + *pfVar2;
      fVar17 = 10.0;
      fStack_48 = fStack_48 + pfVar2[1];
      fStack_44 = fStack_44 + pfVar2[2];
      pfVar2 = (float *)FUN_00447830(auStack_74,afStack_84);
      pfVar2 = (float *)FUN_00446390(&fStack_90,pfVar2,fVar17);
      fStack_4c = fStack_4c + *pfVar2;
      fStack_48 = fStack_48 + pfVar2[1];
      fStack_44 = fStack_44 + pfVar2[2];
      FUN_00480090(param_1 + 0xa8,(int)auStack_74,'\0');
      fVar17 = 10.0;
      pfVar2 = (float *)FUN_0049e770(auStack_74,afStack_84);
      pfVar2 = (float *)FUN_00446390(&fStack_90,pfVar2,fVar17);
      fStack_4c = fStack_4c + *pfVar2;
      fStack_48 = fStack_48 + pfVar2[1];
      fStack_44 = fStack_44 + pfVar2[2];
      FUN_00480090(param_1 + 0xc9,(int)auStack_74,'\0');
      if ((DAT_00502e60 & 1) == 0) {
        DAT_00502e60 = DAT_00502e60 | 1;
        uStack_4 = 0;
        _eh_vector_constructor_iterator_
                  (&DAT_00502e30,0xc,4,(_func_void_void_ptr *)&LAB_0044c060,FUN_0043f660);
        _atexit((_func_4879 *)&LAB_004db7c0);
        uStack_4 = 0xffffffff;
      }
      pvStack_a0 = (void *)0x0;
      puVar4 = &DAT_00502e30;
      do {
        pfVar2 = (float *)FUN_0049e770(auStack_74,afStack_84);
        pfVar2 = (float *)FUN_00447710(&fStack_90,(float)(int)pvStack_a0 * _DAT_004df224,pfVar2);
        puVar3 = (undefined4 *)FUN_0042d050(afStack_30,&fStack_4c,pfVar2);
        *puVar4 = *puVar3;
        puVar4[1] = puVar3[1];
        pvStack_a0 = (void *)((int)pvStack_a0 + 1);
        puVar6 = puVar4 + 3;
        puVar4[2] = puVar3[2];
        puVar4 = puVar6;
      } while ((int)puVar6 < 0x502e60);
      FUN_00481a50(param_1 + 0xff);
      FUN_00481c00((int *)&fStack_9c);
      uStack_4 = 1;
      puVar4 = puStack_98;
      while( true ) {
        if ((fStack_9c == 0.0) || (puVar4 == (undefined4 *)0x0)) {
          pvStack_a0 = (void *)0x0;
        }
        else {
          pvStack_a0 = (void *)*puVar4;
        }
        if ((fStack_9c != 0.0) && (puVar4 != (undefined4 *)0x0)) {
          puVar4 = (undefined4 *)puVar4[1];
          puStack_98 = puVar4;
        }
        if (pvStack_a0 == (void *)0x0) break;
        if (*(int *)((int)pvStack_a0 + 0x18) == 6) {
          pfVar2 = (float *)&DAT_00502e30;
          do {
            fVar7 = FUN_0044a7a0((int)pvStack_a0 + 300,pfVar2,60.0);
            if ((((float10)_DAT_004dfa28 <= fVar7) &&
                (FUN_00481a60(param_1 + 0xff,pvStack_a0,(int *)pfVar2,(int)(float)(fVar7 + fVar7)),
                uVar1 = DAT_00502afc, (float)_DAT_004dfa10 <= *(float *)((int)pvStack_a0 + 0x128)))
               && ((uint)param_1[0xea] <= DAT_00502afc)) {
              FUN_00498b30(10.0,250.0);
              uStack_78 = uVar1;
              uVar10 = FUN_004b9400();
              param_1[0xea] = (int)uVar10;
              puVar4 = (undefined4 *)FUN_00447870((void *)((int)pvStack_a0 + 300),afStack_30);
              puVar4 = FUN_0047ff30(param_1 + 0xf2,puVar4);
              if (puVar4 != (undefined4 *)0x0) {
                uVar16 = 0;
                uVar14 = 0;
                uVar12 = 0;
                pfVar11 = afStack_84;
                pvVar5 = (void *)FUN_0047f1c0((int)puVar4);
                FUN_0042d280(pvVar5,pfVar11,uVar12,uVar14,uVar16);
                fVar7 = FUN_00498b30(-50.0,50.0);
                fVar17 = (float)fVar7;
                fVar7 = FUN_00498b30(100.0,400.0);
                fVar15 = (float)fVar7;
                fVar7 = FUN_00498b30(-50.0,50.0);
                fVar13 = (float)fVar7;
                pfVar11 = &fStack_90;
                pvVar5 = (void *)FUN_0047f1b0((int)puVar4);
                FUN_0042d280(pvVar5,pfVar11,fVar13,fVar15,fVar17);
                puVar4[0x29] = 0x20;
              }
            }
            pfVar2 = pfVar2 + 3;
            puVar4 = puStack_98;
          } while ((int)pfVar2 < 0x502e60);
        }
      }
      fVar17 = (float)param_1[0x232] - DAT_00502b04 * (float)param_1[0x233];
      param_1[0x232] = (int)fVar17;
      if (fVar17 < _DAT_004dc50c) {
        (**(code **)(*param_1 + 0x1c))();
        param_1[0x232] = 0;
      }
      uStack_4 = 0xffffffff;
      if (fStack_9c != 0.0) {
        FUN_00429fe0((void *)((int)fStack_9c + 0x28),(int)&fStack_9c);
      }
    }
    uVar1 = param_1[0x182];
    if ((DAT_00502afc < uVar1) || ((uint)param_1[0x183] <= DAT_00502afc)) {
      if ((char)param_1[0x180] == '\0') {
        ExceptionList = pvStack_c;
        return;
      }
      pfVar2 = (float *)(param_1 + 0x18a);
      pfVar11 = (float *)(param_1 + 0x187);
    }
    else {
      uStack_78 = DAT_00502afc - uVar1;
      fVar17 = (float)(int)uStack_78;
      if ((int)uStack_78 < 0) {
        fVar17 = fVar17 + _DAT_004dfa30;
      }
      fVar17 = fVar17 / (float)(int)(param_1[0x183] - uVar1);
      if ((char)param_1[0x180] == '\0') {
        fStack_9c = (float)param_1[0x18d];
        puStack_98 = (undefined4 *)param_1[0x18e];
        fStack_94 = (float)param_1[399];
        fStack_90 = (float)param_1[400];
        fStack_8c = (float)param_1[0x191];
        fStack_88 = (float)param_1[0x192];
      }
      else {
        fStack_9c = (float)param_1[0x187];
        puStack_98 = (undefined4 *)param_1[0x188];
        fStack_94 = (float)param_1[0x189];
        fStack_90 = (float)param_1[0x18a];
        fStack_8c = (float)param_1[0x18b];
        fStack_88 = (float)param_1[0x18c];
      }
      fVar15 = fVar17;
      pfVar2 = (float *)FUN_0043b2d0(afStack_30,&fStack_9c,(float *)(param_1 + 0x193));
      pfVar2 = (float *)FUN_00446390(afStack_84,pfVar2,fVar15);
      pfVar2 = (float *)FUN_0042d050(afStack_24,(float *)(param_1 + 0x193),pfVar2);
      fStack_9c = *pfVar2;
      puStack_98 = (undefined4 *)pfVar2[1];
      fStack_94 = pfVar2[2];
      pfVar2 = (float *)FUN_0043b2d0(afStack_24,&fStack_90,(float *)(param_1 + 0x196));
      pfVar2 = (float *)FUN_00446390(afStack_30,pfVar2,fVar17);
      pfVar2 = (float *)FUN_0042d050(afStack_18,(float *)(param_1 + 0x196),pfVar2);
      fStack_90 = *pfVar2;
      fStack_8c = pfVar2[1];
      fStack_88 = pfVar2[2];
      pfVar2 = &fStack_90;
      pfVar11 = &fStack_9c;
    }
    FUN_004612b0(param_1,pfVar11,pfVar2);
  }
  ExceptionList = pvStack_c;
  return;
}


