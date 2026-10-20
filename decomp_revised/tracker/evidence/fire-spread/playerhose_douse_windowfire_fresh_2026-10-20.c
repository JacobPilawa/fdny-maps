/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00464aa0 FUN_00464aa0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00464aa0(int param_1,int *param_2)

{
  float *pfVar1;
  int iVar2;
  
  if ((*(char *)(*(int *)(*(int *)(param_1 + 0x24) + 4) + 0x1c) != '\0') &&
     (_DAT_00502e64 = (float *)(**(code **)(*param_2 + 0xc))(), _DAT_00502e64 != (float *)0x0)) {
    iVar2 = *(int *)(*(int *)(param_1 + 0x24) + 4);
    pfVar1 = (float *)(iVar2 + 0xc);
    if (((_DAT_00502e64[4] + *_DAT_00502e64 <= *(float *)(iVar2 + 0x18) + *pfVar1) &&
        (((*pfVar1 - *(float *)(iVar2 + 0x18) <= _DAT_00502e64[7] + *_DAT_00502e64 &&
          (_DAT_00502e64[5] + _DAT_00502e64[1] <=
           *(float *)(iVar2 + 0x18) + *(float *)(iVar2 + 0x10))) &&
         (*(float *)(iVar2 + 0x10) - *(float *)(iVar2 + 0x18) <= _DAT_00502e64[8] + _DAT_00502e64[1]
         )))) && ((_DAT_00502e64[6] + _DAT_00502e64[2] <=
                   *(float *)(iVar2 + 0x14) + *(float *)(iVar2 + 0x18) &&
                  (*(float *)(iVar2 + 0x14) - *(float *)(iVar2 + 0x18) <=
                   _DAT_00502e64[9] + _DAT_00502e64[2])))) {
      FUN_00481a60((void *)(param_1 + 0x28),param_2,pfVar1,*(undefined4 *)(iVar2 + 0x3b0));
    }
  }
  return;
}



/* 0x00481a60 FUN_00481a60 */

void __thiscall FUN_00481a60(void *this,void *param_1,undefined4 *param_2,undefined4 param_3)

{
  undefined4 *puVar1;
  undefined4 local_8 [2];
  
  if (*(int *)this < 0x20) {
    puVar1 = (undefined4 *)(*(int *)this * 0x10 + 4 + (int)this);
    *puVar1 = *param_2;
    puVar1[1] = param_2[1];
    puVar1[2] = param_2[2];
    *(undefined4 *)((*(int *)this + 1) * 0x10 + (int)this) = param_3;
    puVar1 = (undefined4 *)FUN_0049d350(param_1,local_8);
    FUN_0049d4e0(*puVar1,puVar1[1],30000,*(int *)this * 0x10 + 4 + (int)this);
    *(int *)this = *(int *)this + 1;
  }
  return;
}



/* 0x0046c5a0 FUN_0046c5a0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0046c5a0(void *param_1)

{
  bool bVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  undefined4 uVar5;
  int iVar6;
  float *pfVar7;
  undefined4 *puVar8;
  undefined4 *puVar9;
  float *pfVar10;
  void *pvVar11;
  int *piVar12;
  int iVar13;
  float10 fVar14;
  ulonglong uVar15;
  float fVar16;
  float fVar17;
  undefined1 *puVar18;
  float fVar19;
  int local_f4;
  float local_f0;
  float local_ec;
  float local_e8;
  int local_e4;
  int local_e0;
  float fStack_dc;
  float fStack_d8;
  float fStack_d4;
  undefined4 local_d0;
  undefined1 *local_cc;
  float local_c8 [3];
  float afStack_bc [3];
  float afStack_b0 [3];
  float local_a4 [3];
  float afStack_98 [3];
  undefined4 auStack_8c [3];
  float afStack_80 [3];
  float local_74 [3];
  float afStack_68 [3];
  float local_5c [3];
  undefined1 auStack_50 [68];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d213b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  if ((DAT_00502f1c & 1) == 0) {
    DAT_00502f1c = DAT_00502f1c | 1;
    DAT_00502f10 = 0;
    DAT_00502f14 = 0;
    DAT_00502f18 = 0;
    ExceptionList = &local_c;
    _atexit(FUN_004db870);
  }
  if ((DAT_00502f1c & 2) == 0) {
    DAT_00502f1c = DAT_00502f1c | 2;
    DAT_00502f04 = 0;
    DAT_00502f08 = 0;
    DAT_00502f0c = 0;
    _atexit(FUN_004db860);
  }
  if ((DAT_00502f1c & 4) == 0) {
    DAT_00502f1c = DAT_00502f1c | 4;
    DAT_00502ef8 = 0;
    DAT_00502efc = 0;
    DAT_00502f00 = 0;
    _atexit(FUN_004db850);
  }
  if ((DAT_00502f1c & 8) == 0) {
    DAT_00502f1c = DAT_00502f1c | 8;
    DAT_00502eec = 0;
    DAT_00502ef0 = 0;
    DAT_00502ef4 = 0;
    _atexit(FUN_004db840);
  }
  if ((DAT_00502f1c & 0x10) == 0) {
    DAT_00502f1c = DAT_00502f1c | 0x10;
    DAT_00502ee0 = 0.0;
    DAT_00502ee4 = 0.0;
    DAT_00502ee8 = 0.0;
    _atexit(FUN_004db830);
  }
  if ((DAT_00502f1c & 0x20) == 0) {
    DAT_00502f1c = DAT_00502f1c | 0x20;
    _DAT_00502ed4 = 0;
    _DAT_00502ed8 = 0;
    _DAT_00502edc = 0;
    _atexit(FUN_004db820);
  }
  if ((DAT_00502f1c & 0x40) == 0) {
    DAT_00502f1c = DAT_00502f1c | 0x40;
    _DAT_00502ec8 = 0;
    _DAT_00502ecc = 0;
    _DAT_00502ed0 = 0;
    _atexit(FUN_004db810);
  }
  if ((DAT_00502f1c & 0x80) == 0) {
    DAT_00502f1c = DAT_00502f1c | 0x80;
    DAT_00502ebc = 0;
    DAT_00502ec0 = 0;
    DAT_00502ec4 = 0;
    _atexit(FUN_004db800);
  }
  if ((DAT_00502f1c & 0x100) == 0) {
    DAT_00502f1c = DAT_00502f1c | 0x100;
    DAT_00502eb0 = 0;
    DAT_00502eb4 = 0;
    DAT_00502eb8 = 0;
    _atexit(FUN_004db7f0);
  }
  iVar13 = 0;
  fVar19 = DAT_00502b04 * _DAT_004e2e78;
  *(undefined4 *)((int)param_1 + 0x268) = 0;
  *(undefined4 *)((int)param_1 + 0x28c) = 0;
  local_e0 = 0;
  fVar19 = *(float *)((int)param_1 + 0x2ec) - fVar19;
  *(float *)((int)param_1 + 0x2ec) = fVar19;
  *(float *)((int)param_1 + 0x264) = fVar19;
  *(undefined4 *)((int)param_1 + 0x2ac) = *(undefined4 *)((int)param_1 + 0x2ec);
  *(undefined4 *)((int)param_1 + 0x2b0) = 0x3f666666;
  *(undefined4 *)((int)param_1 + 0x2d4) = 0x3f666666;
  *(float *)((int)param_1 + 0x288) =
       *(float *)((int)param_1 + 0x2ec) + *(float *)((int)param_1 + 0x2e8);
  *(float *)((int)param_1 + 0x2d0) =
       *(float *)((int)param_1 + 0x2ec) + *(float *)((int)param_1 + 0x2e8);
  iVar6 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar6 + 0x160) == '\0') {
    iVar6 = iVar6 + 8;
  }
  else {
    iVar6 = iVar6 + 0xb4;
  }
  local_d0 = *(undefined4 *)(iVar6 + 0x78);
  FUN_0046ba60((int)param_1 + 0x168);
  local_e4 = 0;
  FUN_004692f0((int)param_1 + 0x2f4);
  if (0 < *(int *)((int)param_1 + 4)) {
    do {
      DAT_00502f10 = DAT_00502f04;
      DAT_00502eec = DAT_00502ef8;
      DAT_00502f14 = DAT_00502f08;
      DAT_00502f18 = DAT_00502f0c;
      DAT_00502ef0 = DAT_00502efc;
      DAT_00502ef4 = DAT_00502f00;
      if (local_e0 == 0) {
        FUN_0046bc10((void *)((int)param_1 + 0x168),
                     *(float *)(*(int *)((int)param_1 + 0x36c) + iVar13 * 4),&DAT_00502f04,
                     &DAT_00502ef8,&DAT_00502ee0,&DAT_00502eac,'\x01');
        if (iVar13 == 0) {
          DAT_00502f18 = DAT_00502f0c;
          DAT_00502f10 = DAT_00502f04;
          DAT_00502f14 = DAT_00502f08;
          DAT_00502ef4 = DAT_00502f00;
          DAT_00502eec = DAT_00502ef8;
          DAT_00502ef0 = DAT_00502efc;
          FUN_00469f50((void *)((int)param_1 + 0x2f4),&DAT_00502f04,DAT_00502eac + DAT_00502eac);
        }
        else {
          if (DAT_00502eac <= *(float *)((int)param_1 + 0x374) * (float)_DAT_004e2e70)
          goto LAB_0046c9f2;
          FUN_00469f50((void *)((int)param_1 + 0x2f4),&DAT_00502f04,DAT_00502eac + DAT_00502eac);
          *(undefined4 *)((int)param_1 + 600) = DAT_00502f10;
          *(undefined4 *)((int)param_1 + 0x25c) = DAT_00502f14;
          *(undefined4 *)((int)param_1 + 0x260) = DAT_00502f18;
          *(undefined4 *)((int)param_1 + 0x27c) = DAT_00502f04;
          *(undefined4 *)((int)param_1 + 0x280) = DAT_00502f08;
          *(undefined4 *)((int)param_1 + 0x284) = DAT_00502f0c;
          *(undefined4 *)((int)param_1 + 0x2a0) = DAT_00502eec;
          *(undefined4 *)((int)param_1 + 0x2a4) = DAT_00502ef0;
          *(undefined4 *)((int)param_1 + 0x2a8) = DAT_00502ef4;
          *(undefined4 *)((int)param_1 + 0x2c4) = DAT_00502ef8;
          *(undefined4 *)((int)param_1 + 0x2c8) = DAT_00502efc;
          uVar5 = DAT_00502f00;
          *(float *)((int)param_1 + 0x264) =
               *(float *)((int)param_1 + 0x2e8) + *(float *)((int)param_1 + 0x264);
          *(undefined4 *)((int)param_1 + 0x2cc) = uVar5;
          pvVar11 = (void *)((int)param_1 + 0x240);
          *(float *)((int)param_1 + 0x2ac) =
               *(float *)((int)param_1 + 0x2e8) + *(float *)((int)param_1 + 0x2ac);
          *(float *)((int)param_1 + 0x288) =
               *(float *)((int)param_1 + 0x2e8) + *(float *)((int)param_1 + 0x288);
          *(float *)((int)param_1 + 0x2d0) =
               *(float *)((int)param_1 + 0x2e8) + *(float *)((int)param_1 + 0x2d0);
          FUN_0048d490(pvVar11,iVar13);
          gePoly_SetLVertex();
          FUN_0048d490(pvVar11,iVar13);
          gePoly_SetLVertex();
          FUN_0048d490(pvVar11,iVar13);
          gePoly_SetLVertex();
          FUN_0048d490(pvVar11,iVar13);
          gePoly_SetLVertex();
          _DAT_00502e68 = DAT_00502f10;
          _DAT_00502e74 = DAT_00502f04;
          _DAT_00502e6c = DAT_00502f14;
          _DAT_00502e70 = DAT_00502f18;
          _DAT_00502e78 = DAT_00502f08;
          _DAT_00502e7c = DAT_00502f0c;
          iVar6 = geWorld_Collision(local_d0,0,0,&DAT_00502e68,&DAT_00502e74,0x43,2,0,0,0,
                                    &DAT_00502e80);
          fVar19 = _DAT_004dfc24;
          if (iVar6 != 0) {
            DAT_00502eb8 = DAT_00502ea4;
            DAT_00502eb0 = DAT_00502e9c;
            DAT_00502eb4 = DAT_00502ea0;
            DAT_00502ec4 = DAT_00502e94;
            DAT_00502ebc = DAT_00502e8c;
            DAT_00502ec0 = DAT_00502e90;
            *(undefined4 *)((int)param_1 + 0xc) = DAT_00502e8c;
            *(undefined4 *)((int)param_1 + 0x10) = DAT_00502ec0;
            *(undefined4 *)((int)param_1 + 0x14) = DAT_00502ec4;
            *(undefined1 *)((int)param_1 + 0x1c) = 1;
            local_e0 = 1;
            *(float *)((int)param_1 + 0x18) =
                 *(float *)((int)param_1 + 0x3ac) * *(float *)((int)param_1 + 0x3b0) +
                 (fVar19 - *(float *)((int)param_1 + 0x3b0)) * *(float *)((int)param_1 + 0x38c);
            local_e8 = _DAT_004dfc24 /
                       SQRT(DAT_00502ee0 * DAT_00502ee0 +
                            DAT_00502ee4 * DAT_00502ee4 + DAT_00502ee8 * DAT_00502ee8);
            local_f0 = DAT_00502ee0 * local_e8;
            local_ec = DAT_00502ee4 * local_e8;
            local_e8 = local_e8 * DAT_00502ee8;
            pfVar7 = (float *)FUN_0042d2b0(&DAT_00502eb0,local_5c);
            fVar19 = local_f0 * *pfVar7 + local_ec * pfVar7[1] + local_e8 * pfVar7[2];
            pfVar7 = (float *)FUN_00447710(local_c8,fVar19 + fVar19,(float *)&DAT_00502eb0);
            local_f0 = local_f0 + *pfVar7;
            local_ec = local_ec + pfVar7[1];
            local_e8 = local_e8 + pfVar7[2];
            fVar14 = FUN_00498b30(0.65,1.35);
            fVar19 = (float)(((float10)*(float *)((int)param_1 + 0x398) *
                              (float10)*(float *)((int)param_1 + 0x3b0) +
                             ((float10)_DAT_004dfc24 - (float10)*(float *)((int)param_1 + 0x3b0)) *
                             (float10)*(float *)((int)param_1 + 0x378)) * fVar14);
            pfVar7 = (float *)FUN_00447710(local_a4,fVar19 * _DAT_004e1604,&local_f0);
            puVar8 = (undefined4 *)FUN_0043b2d0(local_74,(float *)&DAT_00502ebc,pfVar7);
            *(undefined4 *)((int)param_1 + 0x4c) = *puVar8;
            *(undefined4 *)((int)param_1 + 0x50) = puVar8[1];
            *(undefined4 *)((int)param_1 + 0x54) = puVar8[2];
            local_cc = &stack0xfffffef0;
            (**(code **)(*(int *)((int)param_1 + 0x24) + 0x14))();
            FUN_0042d280((void *)((int)param_1 + 0x58),auStack_8c,fVar19,fVar19,fVar19);
            FUN_00498b30(5.0,10.0);
            uVar15 = FUN_004b9400();
            iVar6 = (int)uVar15;
            fStack_dc = 0.0;
            fStack_d8 = 0.0;
            fStack_d4 = 0.0;
            if (0 < iVar6) {
              do {
                fVar14 = FUN_00498b30(-1.75,1.75);
                fStack_dc = (float)(fVar14 * (float10)local_f0);
                fVar14 = FUN_00498b30(-1.75,1.75);
                fStack_d8 = (float)(fVar14 * (float10)local_ec);
                fVar14 = FUN_00498b30(-1.75,1.75);
                fStack_d4 = (float)(fVar14 * (float10)local_e8);
                fVar2 = DAT_00502ee8 * DAT_00502ee8;
                fVar4 = DAT_00502ee4 * DAT_00502ee4;
                fVar3 = DAT_00502ee0 * DAT_00502ee0;
                fVar14 = FUN_00498b30(1.0,3.0);
                fVar19 = (float)fVar14;
                fVar14 = FUN_00498b30(0.1,0.5);
                fVar17 = (float)fVar14;
                puVar8 = (undefined4 *)((int)param_1 + 0x168);
                fVar14 = FUN_00498b30(0.1,0.5);
                fVar16 = (float)fVar14;
                pfVar7 = (float *)FUN_00446390(afStack_bc,&fStack_dc,SQRT(fVar3 + fVar4 + fVar2));
                puVar9 = (undefined4 *)FUN_00446390(afStack_b0,pfVar7,fVar16);
                FUN_00480480((void *)((int)param_1 + 1000),&DAT_00502ebc,puVar9,puVar8,fVar17,fVar19
                            );
                iVar6 = iVar6 + -1;
              } while (iVar6 != 0);
            }
            if (*(char *)((int)param_1 + 0x2f0) != '\0') {
              pfVar7 = (float *)FUN_00446390(afStack_98,(float *)((int)param_1 + 0x3a0),
                                             *(float *)((int)param_1 + 0x3b0));
              pfVar10 = (float *)FUN_00446390(afStack_80,(float *)((int)param_1 + 0x380),
                                              _DAT_004dfc24 - *(float *)((int)param_1 + 0x3b0));
              puVar8 = (undefined4 *)FUN_0042d050(afStack_68,pfVar10,pfVar7);
              iVar6 = *(int *)((int)param_1 + 0x43c);
              *(undefined4 *)(iVar6 + 0xb4) = *puVar8;
              *(undefined4 *)(iVar6 + 0xb8) = puVar8[1];
              *(undefined4 *)(iVar6 + 0xbc) = puVar8[2];
              iVar6 = FUN_0047f1a0(*(int *)((int)param_1 + 0x43c));
              *(undefined4 *)(iVar6 + 0x28) = DAT_00502ebc;
              *(undefined4 *)(iVar6 + 0x2c) = DAT_00502ec0;
              puVar18 = auStack_50;
              *(undefined4 *)(iVar6 + 0x30) = DAT_00502ec4;
              pvVar11 = (void *)FUN_0047f1a0(*(int *)((int)param_1 + 0x43c));
              FUN_0042d350(pvVar11,puVar18);
              fVar19 = 30.0;
              pvVar11 = (void *)FUN_0047f1a0(*(int *)((int)param_1 + 0x43c));
              FUN_004966f0(pvVar11,fVar19);
              fVar19 = 30.0;
              pvVar11 = (void *)FUN_0047f1a0(*(int *)((int)param_1 + 0x43c));
              FUN_004968b0(pvVar11,fVar19);
              local_cc = &stack0xfffffef0;
              uStack_4 = 0;
              piVar12 = (int *)FUN_0047f1a0(*(int *)((int)param_1 + 0x43c));
              uStack_4 = 0xffffffff;
              (**(code **)(*piVar12 + 0x14))();
              fVar19 = (*(float *)((int)param_1 + 0x39c) * *(float *)((int)param_1 + 0x3b0) +
                       (_DAT_004dfc24 - *(float *)((int)param_1 + 0x3b0)) *
                       *(float *)((int)param_1 + 0x37c)) * DAT_00502b04 +
                       *(float *)((int)param_1 + 0x41c);
              *(float *)((int)param_1 + 0x41c) = fVar19;
              pvVar11 = (void *)FUN_0047f1a0(*(int *)((int)param_1 + 0x43c));
              FUN_00496d00(pvVar11,fVar19);
              FUN_0047f960(*(void **)((int)param_1 + 0x43c),*(void **)(DAT_00503a78 + 4));
            }
            FUN_00496d00((void *)((int)param_1 + 0x24),DAT_00502b04 * _DAT_004e0ed4);
          }
        }
      }
      else {
LAB_0046c9f2:
        FUN_0046c480(param_1,iVar13);
        local_e4 = local_e4 + 1;
      }
      iVar13 = iVar13 + 1;
    } while (iVar13 < *(int *)((int)param_1 + 4));
  }
  bVar1 = 1 < *(int *)((int)param_1 + 4) - local_e4;
  *(bool *)param_1 = bVar1;
  if ((local_e0 != 0) && (bVar1)) {
    FUN_004696a0((int)param_1 + 0x24);
  }
  if (*(char *)((int)param_1 + 0x2f0) != '\0') {
    FUN_0046ba60((int)param_1 + 0x1d4);
    FUN_004692f0((int)param_1 + 0x330);
    if (*(char *)((int)param_1 + 8) != '\0') {
      local_f4 = 0;
      do {
        FUN_0046bc10((void *)((int)param_1 + 0x1d4),(float)local_f4 * _DAT_004e2e6c,&DAT_00502f04,
                     &DAT_00502ef8,&DAT_00502ee0,&DAT_00502eac,'\x01');
        FUN_00469f50((void *)((int)param_1 + 0x330),&DAT_00502f04,DAT_00502eac);
        local_f4 = local_f4 + 1;
      } while (local_f4 < 10);
    }
  }
  ExceptionList = local_c;
  return;
}



/* 0x00468680 FUN_00468680 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00468680(int param_1)

{
  int iVar1;
  float fVar2;
  
  iVar1 = *(int *)(param_1 + 0x2a8);
  if ((*(char *)(iVar1 + 0xc) != '\0') && (*(char *)(iVar1 + 0xe) != '\0')) {
    fVar2 = *(float *)(param_1 + 600) - DAT_00502b04 / *(float *)(iVar1 + 8);
    *(float *)(param_1 + 600) = fVar2;
    if (fVar2 < _DAT_004dc50c) {
      *(undefined1 *)(iVar1 + 0xd) = 0;
      *(undefined4 *)(param_1 + 600) = 0;
    }
    FUN_00468540(param_1);
    *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xf) = 1;
  }
  return;
}



/* 0x00468a80 FUN_00468a80 */

void __thiscall FUN_00468a80(int param_1,int param_2)

{
  if (param_2 == 30000) {
    FUN_00468680(param_1);
  }
  return;
}



/* 0x00468540 FUN_00468540 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00468540(int param_1)

{
  float *pfVar1;
  float *pfVar2;
  int iVar3;
  undefined4 local_c [3];
  
  FUN_0042d280((void *)(*(int *)(param_1 + 0xb4) + 0xb4),local_c,
               (*(float *)(param_1 + 600) * (float)_DAT_004e27d8 + (float)_DAT_004e1b20) *
               *(float *)(param_1 + 0x254),
               (*(float *)(param_1 + 600) * (float)_DAT_004e1b20 + (float)_DAT_004e27d8) *
               *(float *)(param_1 + 0x250),0);
  iVar3 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
  *(undefined4 *)(iVar3 + 0x28) = *(undefined4 *)(param_1 + 0x4c);
  *(undefined4 *)(iVar3 + 0x2c) = *(undefined4 *)(param_1 + 0x50);
  *(undefined4 *)(iVar3 + 0x30) = *(undefined4 *)(param_1 + 0x54);
  iVar3 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
  *(float *)(iVar3 + 0x2c) =
       *(float *)(*(int *)(param_1 + 0xb4) + 0xb8) * (float)_DAT_004de6f8 + *(float *)(iVar3 + 0x2c)
  ;
  iVar3 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
  pfVar1 = (float *)(param_1 + 0x80);
  *pfVar1 = *(float *)(iVar3 + 0x28);
  *(undefined4 *)(param_1 + 0x84) = *(undefined4 *)(iVar3 + 0x2c);
  *(undefined4 *)(param_1 + 0x88) = *(undefined4 *)(iVar3 + 0x30);
  pfVar2 = (float *)(param_1 + 0x90);
  FUN_0042d280(pfVar2,local_c,*(undefined4 *)(param_1 + 0x68),*(undefined4 *)(param_1 + 0x6c),
               *(undefined4 *)(param_1 + 0x70));
  *pfVar2 = *pfVar2 - *pfVar1;
  *(float *)(param_1 + 0x94) = *(float *)(param_1 + 0x94) - *(float *)(param_1 + 0x84);
  *(float *)(param_1 + 0x98) = *(float *)(param_1 + 0x98) - *(float *)(param_1 + 0x88);
  pfVar2 = (float *)(param_1 + 0x9c);
  FUN_0042d280(pfVar2,local_c,*(undefined4 *)(param_1 + 0x74),*(undefined4 *)(param_1 + 0x78),
               *(undefined4 *)(param_1 + 0x7c));
  *pfVar2 = *pfVar2 - *pfVar1;
  *(float *)(param_1 + 0xa0) = *(float *)(param_1 + 0xa0) - *(float *)(param_1 + 0x84);
  *(float *)(param_1 + 0xa4) = *(float *)(param_1 + 0xa4) - *(float *)(param_1 + 0x88);
  return;
}



/* 0x00468870 FUN_00468870 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00468870(int param_1)

{
  float fVar1;
  int iVar2;
  void *this;
  char cVar3;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  if (*(char *)(*(int *)(param_1 + 0x2a8) + 0xd) != '\0') {
    if (*(char *)(param_1 + 0x25d) != '\0') {
      fVar1 = DAT_00502b04 + DAT_00502b04 + *(float *)(param_1 + 600);
      *(float *)(param_1 + 600) = fVar1;
      if (fVar1 <= (float)_DAT_004dd808) {
        FUN_0047f960(*(void **)(param_1 + 0xb4),*(void **)(DAT_00503a78 + 4));
        FUN_00468540(param_1);
      }
      else {
        *(undefined4 *)(param_1 + 600) = 0x3f800000;
        *(undefined1 *)(param_1 + 0x25d) = 0;
      }
    }
    if (*(char *)(*(int *)(param_1 + 0x2a8) + 0x10) != '\0') {
      FUN_00468680(param_1);
    }
    if (*(char *)(*(int *)(param_1 + 0x2a8) + 0xc) == '\0') {
      this = (void *)(param_1 + 0x1c8);
    }
    else if (*(char *)(*(int *)(param_1 + 0x2a8) + 0xf) == '\0') {
      this = (void *)(param_1 + 0xc0);
    }
    else {
      this = (void *)(param_1 + 0x144);
    }
    cVar3 = '\0';
    iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
    FUN_00480090(this,iVar2,cVar3);
    FUN_0047f960(*(void **)(param_1 + 0xb4),*(void **)(DAT_00503a78 + 4));
    if (*(char *)(*(int *)(param_1 + 0x2a8) + 0xc) != '\0') {
      if (*(char *)(param_1 + 0x24d) != '\0') {
        FUN_0047f960(*(void **)(param_1 + 0xbc),*(void **)(DAT_00503a78 + 4));
      }
      iVar2 = *(int *)(param_1 + 0x2a8);
      *(undefined1 *)(param_1 + 0x25c) = *(undefined1 *)(iVar2 + 0xe);
      if (*(int *)(*(int *)(param_1 + 0xbc) + 4) == 0x11) {
        *(undefined1 *)(param_1 + 0x24d) = 0;
      }
      if (*(char *)(iVar2 + 0xf) == '\0') {
        uStack_4 = 0;
        uStack_c = 0x3f800000;
        if (*(char *)(iVar2 + 0xe) == '\0') {
          uStack_8 = 0x3f000000;
        }
        else {
          uStack_8 = 0;
        }
      }
      else {
        uStack_c = 0x3e800000;
        uStack_8 = 0x3e800000;
        uStack_4 = 0x3f800000;
      }
      FUN_0047f270(*(void **)(param_1 + 0xb8),&uStack_c);
      iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xb8));
      *(undefined4 *)(iVar2 + 0x28) = *(undefined4 *)(param_1 + 0x4c);
      *(undefined4 *)(iVar2 + 0x2c) = *(undefined4 *)(param_1 + 0x50);
      *(undefined4 *)(iVar2 + 0x30) = *(undefined4 *)(param_1 + 0x54);
      iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xb8));
      *(float *)(iVar2 + 0x2c) =
           (*(float *)(*(int *)(param_1 + 0xb4) + 0xb8) * (float)_DAT_004de6f8 -
           (float)_DAT_004e2de0) + *(float *)(iVar2 + 0x2c);
      *(undefined1 *)(*(int *)(param_1 + 0xb8) + 0xad) = 1;
      FUN_0047f960(*(void **)(param_1 + 0xb8),*(void **)(DAT_00503a78 + 4));
    }
    *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xf) = 0;
  }
  return;
}



/* 0x004683f0 FUN_004683f0 */

void * __thiscall FUN_004683f0(void *this,void *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1ffe;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0049d420(this,(int)param_1);
  local_4 = 0;
  FUN_0042d2d0((undefined4 *)((int)this + 0x24),0);
  *(undefined ***)this = &PTR_FUN_004e2d88;
  *(undefined4 *)((int)this + 0x24) = &PTR_FUN_004dfc30;
  *(undefined4 *)((int)this + 0x68) = 0;
  *(undefined4 *)((int)this + 0x6c) = 0;
  *(undefined4 *)((int)this + 0x70) = 0;
  *(undefined4 *)((int)this + 0x74) = 0;
  *(undefined4 *)((int)this + 0x78) = 0;
  *(undefined4 *)((int)this + 0x7c) = 0;
  *(undefined4 *)((int)this + 0x80) = 0;
  *(undefined4 *)((int)this + 0x84) = 0;
  *(undefined4 *)((int)this + 0x88) = 0;
  *(undefined4 *)((int)this + 0x90) = 0;
  *(undefined4 *)((int)this + 0x94) = 0;
  *(undefined4 *)((int)this + 0x98) = 0;
  *(undefined4 *)((int)this + 0x9c) = 0;
  *(undefined4 *)((int)this + 0xa0) = 0;
  *(undefined4 *)((int)this + 0xa4) = 0;
  *(undefined4 *)((int)this + 0xa8) = 0;
  *(undefined4 *)((int)this + 0xac) = 0;
  *(undefined4 *)((int)this + 0xb0) = 0;
  *(undefined4 *)((int)this + 0x8c) = 0;
  FUN_004804f0((undefined4 *)((int)this + 0xc0));
  local_4._0_1_ = 1;
  FUN_004804f0((undefined4 *)((int)this + 0x144));
  local_4._0_1_ = 2;
  FUN_004804f0((undefined4 *)((int)this + 0x1c8));
  local_4._0_1_ = 3;
  FUN_004a13a0((undefined4 *)((int)this + 0x260));
  local_4._0_1_ = 4;
  FUN_004a13a0((undefined4 *)((int)this + 0x284));
  local_4 = CONCAT31(local_4._1_3_,5);
  FUN_0049d140(param_1,(int *)((int)this + 0x2a8));
  *(undefined4 *)((int)this + 0xb8) = 0;
  *(undefined4 *)((int)this + 0xb4) = 0;
  *(undefined4 *)((int)this + 0xbc) = 0;
  *(undefined4 *)((int)this + 100) = 0;
  *(undefined4 *)((int)this + 0x18) = 7;
  ExceptionList = local_c;
  return this;
}



/* 0x00465b90 FUN_00465b90 */

void * __thiscall FUN_00465b90(void *this,undefined4 param_1,int param_2)

{
  int *this_00;
  undefined4 uVar1;
  void *pvVar2;
  undefined4 *puVar3;
  void *this_01;
  int *piVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int local_6c;
  undefined1 local_64 [8];
  int local_5c;
  char *local_58 [4];
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  int local_34;
  char *local_30;
  char *local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_004d1b6e;
  local_c = ExceptionList;
  this_00 = (int *)((int)this + 8);
  ExceptionList = &local_c;
  *(undefined4 *)((int)this + 0x1c) = 0x10;
  *(code **)((int)this + 0x18) = FUN_0043f660;
  *this_00 = 0;
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined4 *)((int)this + 0x10) = 0;
  *(undefined4 *)((int)this + 0x14) = 0;
  *(int *)this = param_2;
  local_58[0] = *(char **)(param_2 + 0x40);
  local_58[1] = *(char **)(param_2 + 0x48);
  iVar5 = *(int *)(param_2 + 0x38);
  local_58[2] = *(char **)(param_2 + 0x50);
  local_58[3] = *(char **)(param_2 + 0x58);
  local_48 = *(undefined4 *)(param_2 + 0x60);
  local_44 = *(undefined4 *)(param_2 + 0x68);
  local_40 = *(undefined4 *)(param_2 + 0x70);
  local_3c = *(undefined4 *)(param_2 + 0x78);
  local_34 = *(int *)(param_2 + 0x3c);
  local_30 = *(char **)(param_2 + 0x44);
  local_2c = *(char **)(param_2 + 0x4c);
  local_28 = *(undefined4 *)(param_2 + 0x54);
  local_24 = *(undefined4 *)(param_2 + 0x5c);
  local_20 = *(undefined4 *)(param_2 + 100);
  local_1c = *(undefined4 *)(param_2 + 0x6c);
  local_18 = *(undefined4 *)(param_2 + 0x74);
  local_14 = *(undefined4 *)(param_2 + 0x7c);
  local_4 = 0;
  local_38 = 0;
  local_10 = 0;
  local_5c = iVar5;
  if (iVar5 != 0) {
    local_6c = 0;
    do {
      pvVar2 = operator_new(0x14);
      uVar1 = *(undefined4 *)((int)&local_34 + local_6c);
      *(int *)((int)pvVar2 + 4) = iVar5;
      *(undefined4 *)((int)pvVar2 + 8) = uVar1;
      puVar3 = operator_new(0x14);
      local_4._0_1_ = 1;
      if (puVar3 == (undefined4 *)0x0) {
        this_01 = (void *)0x0;
      }
      else {
        this_01 = (void *)FUN_0049f230(puVar3);
      }
      local_4._0_1_ = 0;
      piVar4 = FUN_0049b760(local_64,"RCS_WindowFire",100);
      local_4._0_1_ = 2;
      iVar5 = FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar4);
      local_4 = (uint)local_4._1_3_ << 8;
      FUN_0049b6f0((int)local_64);
      if (iVar5 != 0) {
        *(int *)((int)this_01 + 0xc) = iVar5;
      }
      *(undefined4 *)((int)this_01 + 8) = 0;
      FUN_0049d130(this_01,pvVar2);
      piVar4 = FUN_0049d390((int)this_01);
      iVar5 = 0;
      if (0 < *(int *)((int)this + 0xc)) {
        piVar6 = (int *)*this_00;
        do {
          if (*piVar6 == 0) goto LAB_00465d48;
          iVar5 = iVar5 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar5 < *(int *)((int)this + 0xc));
      }
      FUN_0043b6e0(this_00,iVar5 + 1);
LAB_00465d48:
      *(int **)(*this_00 + iVar5 * 4) = piVar4;
      iVar5 = *(int *)((int)local_58 + local_6c);
      local_6c = local_6c + 4;
    } while (iVar5 != 0);
  }
  FUN_00465940(this,'\0');
  iVar5 = 0;
  if (0 < *(int *)((int)this + 0xc)) {
    do {
      if ((iVar5 < 0) || (*(int *)((int)this + 0xc) <= iVar5)) {
        piVar4 = &local_5c;
        for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_5c = 1;
        local_58[0] = "Array index out of bounds";
        local_58[1] = "..\\BluelAPI/Container/SArray.h";
        local_58[2] = (char *)0x1cd;
        FUN_00422060(&local_5c);
      }
      *(undefined1 *)(*(int *)(*(int *)(*this_00 + iVar5 * 4) + 0x2a8) + 0xe) = 0;
      if ((iVar5 < 0) || (*(int *)((int)this + 0xc) <= iVar5)) {
        piVar4 = &local_34;
        for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_34 = 1;
        local_30 = "Array index out of bounds";
        local_2c = "..\\BluelAPI/Container/SArray.h";
        local_28 = 0x1cd;
        FUN_00422060(&local_34);
      }
      *(undefined1 *)(*(int *)(*(int *)(*this_00 + iVar5 * 4) + 0x2a8) + 0xd) = 0;
      iVar5 = iVar5 + 1;
    } while (iVar5 < *(int *)((int)this + 0xc));
  }
  *(undefined4 *)((int)this + 0x28) = 0;
  *(undefined4 *)((int)this + 4) = 0xffffffff;
  *(undefined1 *)((int)this + 0x2c) = 0;
  *(undefined1 *)((int)this + 0x24) = 1;
  *(undefined4 *)((int)this + 0x20) = *(undefined4 *)(*(int *)this + 0x34);
  ExceptionList = local_c;
  return this;
}



