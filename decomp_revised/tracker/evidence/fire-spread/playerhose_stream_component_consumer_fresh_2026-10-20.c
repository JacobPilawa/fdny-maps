/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0046d6b0 FUN_0046d6b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0046d6b0(void *this,int *param_1,undefined4 param_2,int *param_3)

{
  int *piVar1;
  float fVar2;
  void *pvVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  int iVar6;
  int *piVar7;
  int local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d221b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(int **)((int)this + 0x20) = param_1;
  *(undefined4 *)((int)this + 4) = param_2;
  *(undefined1 *)((int)this + 0x2f0) = param_3._0_1_;
  *(undefined4 *)((int)this + 0x370) = 0;
  *(undefined4 *)((int)this + 0x374) = 0x3f4ccccd;
  *(undefined4 *)((int)this + 0x378) = 0;
  *(undefined4 *)((int)this + 0x37c) = 0x42c80000;
  *(undefined4 *)((int)this + 0x380) = 0x42f00000;
  *(undefined4 *)((int)this + 900) = 0x42f00000;
  *(undefined4 *)((int)this + 0x388) = 0;
  *(undefined4 *)((int)this + 0x38c) = 0x41200000;
  *(undefined4 *)((int)this + 0x390) = 0x457a0000;
  *(undefined4 *)((int)this + 0x394) = 0x40400000;
  *(undefined4 *)((int)this + 0x398) = 0x3fe00000;
  *(undefined4 *)((int)this + 0x39c) = 0x43960000;
  local_3c = 0x43700000;
  *(undefined4 *)((int)this + 0x3a0) = 0x43700000;
  local_38 = (char *)0x43700000;
  *(undefined4 *)((int)this + 0x3a4) = 0x43700000;
  *(undefined4 *)((int)this + 0x3a8) = 0;
  *(undefined4 *)((int)this + 0x3ac) = 0x42200000;
  *(undefined4 *)((int)this + 0x3b4) = 0x3f800000;
  *(undefined4 *)((int)this + 0x3b8) = 0x3f800000;
  local_34 = (char *)0x0;
  *(undefined4 *)((int)this + 0x3b0) = 0;
  *(undefined4 *)((int)this + 0x3bc) = 0x3d4ccccd;
  *(undefined4 *)((int)this + 0x3c0) = 0x3e800000;
  *(undefined1 *)((int)this + 9) = 0;
  *(undefined4 *)((int)this + 0x3c4) = 0;
  *(undefined4 *)((int)this + 0x3c8) = 0;
  *(undefined1 *)this = 0;
  pvVar3 = operator_new(*(int *)((int)this + 4) << 2);
  *(void **)((int)this + 0x36c) = pvVar3;
  param_3 = (int *)0x0;
  if (0 < *(int *)((int)this + 4)) {
    do {
      fVar2 = (float)(int)param_3 / (float)(*(int *)((int)this + 4) + -1);
      param_3 = (int *)((int)param_3 + 1);
      *(float *)(*(int *)((int)this + 0x36c) + -4 + (int)param_3 * 4) =
           fVar2 * (float)_DAT_004de6f8 + fVar2 * fVar2 * (float)_DAT_004de6f8;
    } while ((int)param_3 < *(int *)((int)this + 4));
  }
  FUN_00469420((void *)((int)this + 0x2f4),1);
  puVar4 = (undefined4 *)((int)this + 0x270);
  iVar6 = 4;
  do {
    puVar4[-1] = 0x437f0000;
    *puVar4 = 0x437f0000;
    puVar4[1] = 0x437f0000;
    puVar4[2] = 0x437f0000;
    puVar4 = puVar4 + 9;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  pvVar3 = *(void **)(DAT_00503a78 + 4);
  iVar6 = (int)pvVar3 + 0xb4;
  if (*(char *)((int)pvVar3 + 0x160) == '\0') {
    iVar6 = (int)pvVar3 + 8;
  }
  param_2 = *(undefined4 *)(iVar6 + 0x78);
  param_1 = (int *)FUN_00493d00(pvVar3,3,'\x01');
  param_3 = (int *)0x0;
  if (0 < *(int *)((int)this + 4)) {
    piVar1 = (int *)((int)this + 0x240);
    do {
      uVar5 = geWorld_AddPoly(param_2,(int)this + 600,4,param_1,0,0x20,0x3f800000);
      iVar6 = 0;
      if (0 < *(int *)((int)this + 0x244)) {
        piVar7 = (int *)*piVar1;
        do {
          if (*piVar7 == 0) goto LAB_0046d8fa;
          iVar6 = iVar6 + 1;
          piVar7 = piVar7 + 1;
        } while (iVar6 < *(int *)((int)this + 0x244));
      }
      FUN_0043b6e0(piVar1,iVar6 + 1);
LAB_0046d8fa:
      *(undefined4 *)(*piVar1 + iVar6 * 4) = uVar5;
      param_3 = (int *)((int)param_3 + 1);
    } while ((int)param_3 < *(int *)((int)this + 4));
  }
  pvVar3 = (void *)((int)this + 0x3cc);
  *(undefined4 *)((int)this + 0x2e8) = 0x3e19999a;
  *(undefined4 *)((int)this + 0x2ec) = 0;
  local_3c = 0x3f800000;
  local_38 = (char *)0x3f800000;
  local_34 = (char *)0x3f800000;
  FUN_0047db10(pvVar3,2,0x3e4ccccd,0x3f400000,0x3f800000,&local_3c);
  local_3c = 0x3f800000;
  local_38 = (char *)0x3f800000;
  local_34 = (char *)0x3f800000;
  FUN_0047db10(pvVar3,2,0x3c23d70a,0,0x3f800000,&local_3c);
  *(uint *)((int)this + 0x3e4) = *(uint *)((int)this + 0x3e4) & 0xfffffffe | 0xc;
  FUN_0047fe70((void *)((int)this + 1000),*(void **)(DAT_00503a78 + 4),pvVar3,'\x01');
  *(undefined4 *)((int)this + 0x418) = 0x14;
  FUN_00469520((void *)((int)this + 0x24),'\x01');
  if (*(char *)((int)this + 0x2f0) != '\0') {
    FUN_0042d280((void *)((int)this + 0x1d4),&local_3c,0,0x43960000,0);
    local_3c = 0x3f800000;
    local_38 = (char *)0x3f800000;
    local_34 = (char *)0x3f800000;
    FUN_0047db10((void *)((int)this + 0x420),6,0x3f800000,0x3f400000,0x3f800000,&local_3c);
    param_3 = operator_new(0x168);
    local_4 = 0;
    if (param_3 == (int *)0x0) {
      pvVar3 = (void *)0x0;
    }
    else {
      pvVar3 = FUN_0047f850(param_3,(int)this + 0x420,0);
    }
    *(void **)((int)this + 0x43c) = pvVar3;
    *(undefined4 *)((int)pvVar3 + 0xb0) = 2;
    *(undefined4 *)(*(int *)((int)this + 0x43c) + 0xa4) = 0x30;
    local_4 = 0xffffffff;
    FUN_0042d280((void *)(*(int *)((int)this + 0x43c) + 0xb4),&local_3c,0x43700000,0x43700000,0);
    *(undefined4 *)((int)this + 0x41c) = 0;
    FUN_00469420((void *)((int)this + 0x330),0);
    local_3c = 0x42800000;
    local_38 = (char *)0x42800000;
    *(undefined4 *)((int)this + 0x34c) = 0x42800000;
    local_34 = (char *)0x0;
    *(undefined4 *)((int)this + 0x350) = 0x42800000;
    *(undefined4 *)((int)this + 0x354) = 0;
    *(undefined1 *)((int)this + 8) = 1;
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_3 = &DAT_00501908;
  FUN_00401d80(&param_3,"TankRefill");
  local_4 = 1;
  ConfigureAndRegisterSoundInstance((void *)((int)this + 0x114),&param_3,0,2);
  piVar1 = param_3;
  local_4 = 0xffffffff;
  if (param_3 != &DAT_00501908) {
    *param_3 = *param_3 + -1;
    if (*param_3 < 1) {
      param_3 = &DAT_00501908;
      if (*piVar1 < 0) {
        piVar7 = &local_3c;
        for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
          *piVar7 = 0;
          piVar7 = piVar7 + 1;
        }
        local_3c = 0;
        local_38 = "Invalid reference count";
        local_34 = "..\\BluelAPI/String/Rope_RatStr.h";
        local_30 = 0xd0;
        FUN_00422060(&local_3c);
      }
      _free(piVar1);
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
  FUN_00401d80(&param_1,"RefillFinish");
  local_4 = 2;
  ConfigureAndRegisterSoundInstance((void *)((int)this + 0x138),&param_1,0,2);
  piVar1 = param_1;
  local_4 = 0xffffffff;
  if (param_1 != &DAT_00501908) {
    *param_1 = *param_1 + -1;
    if (*param_1 < 1) {
      param_1 = &DAT_00501908;
      if (*piVar1 < 0) {
        piVar7 = &local_24;
        for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
          *piVar7 = 0;
          piVar7 = piVar7 + 1;
        }
        local_24 = 0;
        local_20 = "Invalid reference count";
        local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
        local_18 = 0xd0;
        FUN_00422060(&local_24);
      }
      _free(piVar1);
    }
  }
  ExceptionList = local_c;
  return;
}



/* 0x0046dc80 FUN_0046dc80 */

int __fastcall FUN_0046dc80(int param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d22b9;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined4 *)(param_1 + 0xc) = 0;
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined4 *)(param_1 + 0x14) = 0;
  FUN_0046b9d0((undefined4 *)(param_1 + 0x24));
  local_4 = 0;
  InitializeSoundInstance((undefined4 *)(param_1 + 0x114));
  local_4._0_1_ = 1;
  InitializeSoundInstance((undefined4 *)(param_1 + 0x138));
  local_4._0_1_ = 2;
  *(undefined4 *)(param_1 + 0x15c) = 0;
  *(undefined4 *)(param_1 + 0x160) = 0;
  *(undefined4 *)(param_1 + 0x164) = 0;
  FUN_0046c2f0((void *)(param_1 + 0x168),0x3f400000,(void *)0x3d4ccccd);
  local_4._0_1_ = 3;
  FUN_0046c2f0((void *)(param_1 + 0x1d4),0x3f000000,(void *)0x3d4ccccd);
  *(undefined4 *)(param_1 + 0x254) = 0x10;
  *(code **)(param_1 + 0x250) = FUN_0043f660;
  *(undefined4 *)(param_1 + 0x240) = 0;
  *(undefined4 *)(param_1 + 0x244) = 0;
  *(undefined4 *)(param_1 + 0x248) = 0;
  *(undefined4 *)(param_1 + 0x24c) = 0;
  local_4._0_1_ = 5;
  FUN_0046a250((undefined4 *)(param_1 + 0x2f4));
  local_4._0_1_ = 6;
  FUN_0046a250((undefined4 *)(param_1 + 0x330));
  *(undefined4 *)(param_1 + 0x380) = 0;
  *(undefined4 *)(param_1 + 900) = 0;
  *(undefined4 *)(param_1 + 0x388) = 0;
  local_4._0_1_ = 7;
  *(undefined4 *)(param_1 + 0x3a0) = 0;
  *(undefined4 *)(param_1 + 0x3a4) = 0;
  *(undefined4 *)(param_1 + 0x3a8) = 0;
  FUN_0047dba0((undefined4 *)(param_1 + 0x3cc));
  local_4._0_1_ = 8;
  FUN_00480260((undefined4 *)(param_1 + 1000));
  local_4 = CONCAT31(local_4._1_3_,9);
  FUN_0047dba0((undefined4 *)(param_1 + 0x420));
  *(undefined4 *)(param_1 + 0x36c) = 0;
  *(undefined4 *)(param_1 + 0x43c) = 0;
  ExceptionList = local_c;
  return param_1;
}



/* 0x0046d480 FUN_0046d480 */

void __thiscall FUN_0046d480(void *this,float *param_1)

{
  FUN_0046d1b0(this,param_1,(float *)((int)this + 0x15c),'\0');
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
            fVar14 = ScalePseudoRandomFloatToRange(0.65,1.35);
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
            ScalePseudoRandomFloatToRange(5.0,10.0);
            uVar15 = ConvertX87FloatToIntegerTowardZero();
            iVar6 = (int)uVar15;
            fStack_dc = 0.0;
            fStack_d8 = 0.0;
            fStack_d4 = 0.0;
            if (0 < iVar6) {
              do {
                fVar14 = ScalePseudoRandomFloatToRange(-1.75,1.75);
                fStack_dc = (float)(fVar14 * (float10)local_f0);
                fVar14 = ScalePseudoRandomFloatToRange(-1.75,1.75);
                fStack_d8 = (float)(fVar14 * (float10)local_ec);
                fVar14 = ScalePseudoRandomFloatToRange(-1.75,1.75);
                fStack_d4 = (float)(fVar14 * (float10)local_e8);
                fVar2 = DAT_00502ee8 * DAT_00502ee8;
                fVar4 = DAT_00502ee4 * DAT_00502ee4;
                fVar3 = DAT_00502ee0 * DAT_00502ee0;
                fVar14 = ScalePseudoRandomFloatToRange(1.0,3.0);
                fVar19 = (float)fVar14;
                fVar14 = ScalePseudoRandomFloatToRange(0.1,0.5);
                fVar17 = (float)fVar14;
                puVar8 = (undefined4 *)((int)param_1 + 0x168);
                fVar14 = ScalePseudoRandomFloatToRange(0.1,0.5);
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



