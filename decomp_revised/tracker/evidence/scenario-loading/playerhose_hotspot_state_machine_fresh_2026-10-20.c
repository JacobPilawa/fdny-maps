/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x004658a0 FUN_004658a0 */

bool __fastcall FUN_004658a0(int param_1)

{
  return *(int *)(param_1 + 0x20) < 1;
}



/* 0x00465940 FUN_00465940 */

void __thiscall FUN_00465940(void *this,char param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int local_30;
  char *local_2c;
  char *local_28;
  undefined4 local_24;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  iVar2 = 0;
  if (0 < *(int *)((int)this + 0xc)) {
    do {
      if ((iVar2 < 0) || (*(int *)((int)this + 0xc) <= iVar2)) {
        piVar3 = &local_30;
        for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        local_30 = 1;
        local_2c = "Array index out of bounds";
        local_28 = "..\\BluelAPI/Container/SArray.h";
        local_24 = 0x1cd;
        FUN_00422060(&local_30);
      }
      *(char *)(*(int *)(*(int *)(*(int *)((int)this + 8) + iVar2 * 4) + 0x2a8) + 0xc) = param_1;
      if (param_1 == '\0') {
        if ((iVar2 < 0) || (*(int *)((int)this + 0xc) <= iVar2)) {
          piVar3 = &local_18;
          for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
            *piVar3 = 0;
            piVar3 = piVar3 + 1;
          }
          local_18 = 1;
          local_14 = "Array index out of bounds";
          local_10 = "..\\BluelAPI/Container/SArray.h";
          local_c = 0x1cd;
          FUN_00422060(&local_18);
        }
        *(undefined1 *)(*(int *)(*(int *)(*(int *)((int)this + 8) + iVar2 * 4) + 0x2a8) + 0xd) = 0;
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)((int)this + 0xc));
  }
  return;
}



/* 0x00465a20 FUN_00465a20 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00465a20(int param_1)

{
  int *piVar1;
  int iVar2;
  float10 fVar3;
  
  iVar2 = *(int *)(param_1 + 4);
  if (iVar2 != -1) {
    piVar1 = (int *)(param_1 + 8);
    FUN_0048d490(piVar1,iVar2);
    if (*(char *)(*(int *)(*(int *)(*piVar1 + iVar2 * 4) + 0x2a8) + 0xd) != '\0') {
      return;
    }
    iVar2 = *(int *)(param_1 + 4);
    FUN_0048d490(piVar1,iVar2);
    *(undefined1 *)(*(int *)(*(int *)(*piVar1 + iVar2 * 4) + 0x2a8) + 0xe) = 0;
  }
  iVar2 = *(int *)(param_1 + 4) + 1;
  *(int *)(param_1 + 4) = iVar2;
  if (*(int *)(param_1 + 0xc) <= iVar2) {
    *(undefined4 *)(param_1 + 4) = 0;
    *(int *)(param_1 + 0x20) = *(int *)(param_1 + 0x20) + -1;
  }
  if (0 < *(int *)(param_1 + 0x20)) {
    iVar2 = *(int *)(param_1 + 4);
    piVar1 = (int *)(param_1 + 8);
    FUN_0048d490(piVar1,iVar2);
    *(undefined1 *)(*(int *)(*(int *)(*piVar1 + iVar2 * 4) + 0x2a8) + 0xe) = 1;
    iVar2 = *(int *)(param_1 + 4);
    FUN_0048d490(piVar1,iVar2);
    FUN_00468850(*(int *)(*piVar1 + iVar2 * 4));
    if ((*(char *)(param_1 + 0x24) == '\0') &&
       (fVar3 = FUN_00498b30(0.0,1.0), (float10)_DAT_004e17e8 < fVar3)) {
      iVar2 = *(int *)(param_1 + 4);
      FUN_0048d490(piVar1,iVar2);
      FUN_004682a0(*(int *)(*piVar1 + iVar2 * 4));
      *(undefined1 *)(param_1 + 0x24) = 1;
      return;
    }
    *(undefined1 *)(param_1 + 0x24) = 0;
  }
  return;
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



/* 0x00465fb0 FUN_00465fb0 */

void __fastcall FUN_00465fb0(void *param_1)

{
  float *pfVar1;
  float local_c [3];
  
  if (*(char *)((int)param_1 + 0x1489) != '\0') {
    pfVar1 = FUN_00465ef0(param_1,local_c,(float *)0x0);
    FUN_0046d480((void *)((int)param_1 + 0x1480),pfVar1);
    return;
  }
  *(undefined4 *)((int)param_1 + 0x18c) = 4;
  FUN_00465940(*(void **)((int)param_1 + 0x1a74),'\x01');
  *(undefined4 *)((int)param_1 + 0x1a7c) = *(undefined4 *)((int)param_1 + 0x1a84);
  return;
}



/* 0x004674a0 FUN_004674a0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004674a0(int *param_1)

{
  int iVar1;
  bool bVar2;
  void *this;
  undefined4 uVar3;
  int *piVar4;
  ulonglong uVar5;
  int **ppiVar6;
  int *local_1c;
  int *local_18;
  int *local_14;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1d80;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  if (DAT_00502a78 != '\0') {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_1c = &DAT_00501908;
    ExceptionList = &local_c;
    FUN_00401d80(&local_1c,"DBG_SKIP_HOTSPOT");
    ppiVar6 = &local_1c;
    local_4 = 0;
    this = (void *)FUN_00499cb0(DAT_00503a78);
    uVar3 = FUN_00438550(this,(int *)ppiVar6);
    local_4 = 0xffffffff;
    if ((local_1c != &DAT_00501908) && (*local_1c = *local_1c + -1, *local_1c < 1)) {
      FUN_004011c0(&local_1c);
    }
    if ((char)uVar3 != '\0') goto LAB_0046757b;
  }
  if (param_1[0x69d] != 0) {
    bVar2 = FUN_004658a0(param_1[0x69d]);
    if (bVar2) {
LAB_0046757b:
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      local_10 = &DAT_00501908;
      local_4 = 1;
      FUN_0040d6d0(&local_18,'%');
      local_4._0_1_ = 2;
      local_1c = (int *)CONCAT22(local_1c._2_2_,0x69);
      FUN_00401d00(&local_18,(char *)&local_1c);
      FUN_00402770(&local_10,(byte *)(local_18 + 3));
      local_4._0_1_ = 1;
      if (local_18 != &DAT_00501908) {
        *local_18 = *local_18 + -1;
        if (*local_18 < 1) {
          FUN_004011c0(&local_18);
        }
        else {
          local_18 = &DAT_00501908;
        }
      }
      piVar4 = FUN_00412340(&local_14,"Hotspot ",(int *)&local_10);
      local_4._0_1_ = 4;
      uVar5 = FUN_004b9400();
      FUN_004868c0((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),piVar4,(int)uVar5);
      local_4 = CONCAT31(local_4._1_3_,1);
      if (local_14 != &DAT_00501908) {
        *local_14 = *local_14 + -1;
        if (*local_14 < 1) {
          FUN_004011c0(&local_14);
        }
        else {
          local_14 = &DAT_00501908;
        }
      }
      local_4 = 0xffffffff;
      if ((local_10 != &DAT_00501908) && (iVar1 = *local_10, *local_10 = iVar1 + -1, iVar1 + -1 < 1)
         ) {
        FUN_004011c0(&local_10);
      }
      if (param_1[0x69e] == 0) {
        FUN_00466c50((int)param_1);
        FUN_00465940((void *)param_1[0x69d],'\0');
        ExceptionList = local_c;
        return;
      }
      param_1[99] = 3;
      FUN_0046c440((int)(param_1 + 0x520));
      FUN_00464d50(param_1 + 0x9e,1,0);
      FUN_00465940((void *)param_1[0x69d],'\0');
      ExceptionList = local_c;
      return;
    }
  }
  local_10 = (int *)((float)param_1[0x69f] - DAT_00502b04);
  param_1[0x69f] = (int)local_10;
  if ((float)local_10 < _DAT_004dc50c != ((float)local_10 == _DAT_004dc50c)) {
    FUN_00466ce0((int)param_1);
    FUN_00465940((void *)param_1[0x69d],'\0');
    ExceptionList = local_c;
    return;
  }
  FUN_004653b0(param_1 + 0x9e,local_10);
  FUN_004665a0((int)param_1);
  FUN_00466780(param_1);
  FUN_00465a20(param_1[0x69d]);
  param_1[0x98] = param_1[0x13];
  param_1[0x99] = param_1[0x14];
  param_1[0x9a] = param_1[0x15];
  ExceptionList = local_c;
  return;
}



/* 0x004677d0 FUN_004677d0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004677d0(void *param_1)

{
  float fVar1;
  int iVar2;
  undefined4 *puVar3;
  int *piVar4;
  undefined4 uVar5;
  int **ppiVar6;
  void *pvVar7;
  undefined4 *puVar8;
  float *pfVar9;
  int *extraout_ECX;
  ulonglong uVar10;
  int *piVar11;
  char cVar12;
  int *local_f4;
  undefined4 local_f0;
  int *local_ec;
  undefined4 uStack_e8;
  uint local_e0;
  float local_dc;
  float local_d8;
  float local_d4;
  int *local_d0;
  undefined4 local_cc;
  float local_c4 [4];
  undefined **local_b4;
  char *local_b0;
  undefined4 local_ac;
  undefined1 local_a8;
  undefined1 local_98 [36];
  float afStack_74 [9];
  undefined1 local_50 [44];
  void *pvStack_24;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1ddd;
  pvStack_c = ExceptionList;
  local_c4[1] = 0.0;
  local_c4[2] = 0.0;
  local_c4[3] = 0.0;
  ExceptionList = &pvStack_c;
  FUN_0048ff30((undefined4 *)((int)param_1 + 0x4c),2000.0,local_c4 + 1);
  *(float *)((int)param_1 + 0x25c) = local_c4[2];
  FUN_004977b0(local_50,(int)param_1 + 0x24);
  thunk_FUN_0049f340((int *)((int)param_1 + 0x19c));
  fVar1 = _DAT_004e16c0;
  if (*(int *)((int)param_1 + 0x1a58) < 0) {
    fVar1 = _DAT_004e2d04;
  }
  *(float *)((int)param_1 + 0x1a50) = fVar1 / *(float *)((int)param_1 + 0x1a40);
  fVar1 = DAT_00502b04 * *(float *)((int)param_1 + 0x1a50) + *(float *)((int)param_1 + 0x1a4c);
  *(float *)((int)param_1 + 0x1a4c) = fVar1;
  if ((float)_DAT_004dd808 < fVar1) {
    fVar1 = *(float *)((int)param_1 + 0x1a4c);
    do {
      fVar1 = fVar1 - (float)_DAT_004dd808;
    } while ((float)_DAT_004dd808 < fVar1);
    *(float *)((int)param_1 + 0x1a4c) = fVar1;
  }
  if (*(float *)((int)param_1 + 0x1a4c) < _DAT_004dc50c) {
    fVar1 = *(float *)((int)param_1 + 0x1a4c);
    do {
      fVar1 = fVar1 + (float)_DAT_004dd808;
    } while (fVar1 < _DAT_004dc50c);
    *(float *)((int)param_1 + 0x1a4c) = fVar1;
  }
  local_dc = 0.0;
  local_d8 = 0.0;
  local_d4 = 0.0;
  FUN_00490bb0((void *)((int)param_1 + 0x1a28),*(float *)((int)param_1 + 0x1a4c),&local_dc,&local_d4
               ,local_c4,(float *)((int)param_1 + 0x26c),(float *)((int)param_1 + 0x270),
               (float *)((int)param_1 + 0x274),&local_e0);
  puVar3 = (undefined4 *)FUN_0043b2d0((float *)&local_d0,&local_dc,(float *)((int)param_1 + 0x4c));
  puVar8 = (undefined4 *)((int)param_1 + 0x244);
  *puVar8 = *puVar3;
  *(undefined4 *)((int)param_1 + 0x248) = puVar3[1];
  *(undefined4 *)((int)param_1 + 0x24c) = puVar3[2];
  puVar3 = (undefined4 *)FUN_0045a230(puVar8,(float *)&local_d0);
  *puVar8 = *puVar3;
  *(undefined4 *)((int)param_1 + 0x248) = puVar3[1];
  *(undefined4 *)((int)param_1 + 0x24c) = puVar3[2];
  if (local_e0 != 0) {
    local_b4 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_b0 = "hose\\PlayerHoseEnt.cpp";
    local_ac = 0x95;
    local_a8 = 0;
    local_4 = 0;
    FUN_004a3a19(local_98,"Just passed spline anchor..");
    FUN_00401360(&local_b4,0);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_b4);
    piVar4 = (int *)FUN_00466010(param_1,*(int *)(local_e0 + 0x10));
    if ((piVar4 != (int *)0x0) &&
       ((piVar4 == *(int **)((int)param_1 + 0x1a78) || (*(int *)(*piVar4 + 0x24) != 0)))) {
      local_b4 = &PTR_FUN_004dc554;
      if (*(int *)(*piVar4 + 0x24) == 0) {
        FUN_004a3118();
        local_b0 = "hose\\PlayerHoseEnt.cpp";
        local_ac = 0x9f;
        local_a8 = 0;
        local_4 = 2;
        FUN_004a3a19(local_98,"PlayerHoseEnt: ");
        FUN_004a3a19(local_98,"Touched next hotspot.");
        FUN_00401360(&local_b4,0);
        local_4 = 0xffffffff;
        FUN_004012c0(&local_b4);
      }
      else {
        FUN_004a3118();
        local_b0 = "hose\\PlayerHoseEnt.cpp";
        local_ac = 0x9c;
        local_a8 = 0;
        local_4 = 1;
        FUN_004a3a19(local_98,"Touched first hotspot.");
        FUN_00401360(&local_b4,0);
        local_4 = 0xffffffff;
        FUN_004012c0(&local_b4);
        cVar12 = '\x01';
        local_ec = (int *)&stack0xfffffef4;
        piVar11 = extraout_ECX;
        FUN_00402790(&stack0xfffffef4,"RC_HoseLvlStart");
        FUN_004897c0((void *)((int)param_1 + 0x208),piVar11,cVar12);
      }
      *(undefined4 *)(*piVar4 + 0x24) = 0;
      local_ec = (int *)(*(float *)(local_e0 + 0xc) / *(float *)((int)param_1 + 0x1a40));
      *(int **)((int)param_1 + 0x1a4c) = local_ec;
      FUN_00490bb0((void *)((int)param_1 + 0x1a28),(float)local_ec,&local_dc,&local_d4,local_c4,
                   (float *)((int)param_1 + 0x26c),(float *)((int)param_1 + 0x270),
                   (float *)((int)param_1 + 0x274),&local_e0);
      *(int **)((int)param_1 + 0x1a74) = piVar4;
      *(undefined4 *)((int)param_1 + 0x238) = *(undefined4 *)(*piVar4 + 0x30);
      uVar5 = FUN_00466010(param_1,*(int *)(*piVar4 + 0x1c));
      *(undefined4 *)((int)param_1 + 0x1a78) = uVar5;
      *(undefined4 *)((int)param_1 + 0x18c) = 5;
      *(undefined4 *)((int)param_1 + 0x1834) =
           *(undefined4 *)(**(int **)((int)param_1 + 0x1a74) + 0x2c);
      FUN_00465940(*(int **)((int)param_1 + 0x1a74),'\x01');
      FUN_00464d50((void *)((int)param_1 + 0x278),2,*(int *)((int)param_1 + 0x1a74));
      fVar1 = *(float *)(**(int **)((int)param_1 + 0x1a74) + 0x28);
      local_d0 = (int *)0x0;
      *(float *)((int)param_1 + 0x1a84) = fVar1;
      iVar2 = *(int *)(*(int *)(DAT_00502aac + 0x6e4) + 0x2c);
      local_cc = 0x3ff00000;
      local_f4 = (int *)&DAT_cccccccd;
      local_f0 = 0x3ff4cccc;
      local_ec = (int *)&DAT_cccccccd;
      uStack_e8 = 0x3ffccccc;
      if (iVar2 == 0) {
        ppiVar6 = &local_ec;
      }
      else {
        ppiVar6 = &local_f4;
        if (iVar2 != 1) {
          ppiVar6 = &local_d0;
        }
      }
      *(float *)((int)param_1 + 0x1a84) = fVar1 * (float)*(double *)ppiVar6;
      pvVar7 = (void *)FUN_004054a0(&local_ec);
      local_4 = 3;
      piVar4 = FUN_0040dc00(pvVar7,*(int *)((int)param_1 + 0x1a80) + 1,'\0',(int *)0x0);
      piVar4 = FUN_00412340(&local_f4,"Hotspot ",piVar4);
      local_4._0_1_ = 4;
      FUN_00488660((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),piVar4,1,0,0,0,0);
      local_4 = CONCAT31(local_4._1_3_,3);
      if (local_f4 != &DAT_00501908) {
        *local_f4 = *local_f4 + -1;
        if (*local_f4 < 1) {
          FUN_004011c0(&local_f4);
        }
        else {
          local_f4 = &DAT_00501908;
        }
      }
      local_4 = 0xffffffff;
      if ((local_ec != &DAT_00501908) && (*local_ec = *local_ec + -1, *local_ec < 1)) {
        FUN_004011c0(&local_ec);
      }
      pvVar7 = (void *)FUN_004054a0(&local_ec);
      local_4 = 5;
      piVar4 = FUN_0040dc00(pvVar7,*(int *)((int)param_1 + 0x1a80) + 1,'\0',(int *)0x0);
      piVar4 = FUN_00412340(&local_f4,"Hotspot ",piVar4);
      local_4._0_1_ = 6;
      uVar10 = FUN_004b9400();
      FUN_00486920((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),piVar4,(int)uVar10);
      local_4 = CONCAT31(local_4._1_3_,5);
      if (local_f4 != &DAT_00501908) {
        *local_f4 = *local_f4 + -1;
        if (*local_f4 < 1) {
          FUN_004011c0(&local_f4);
        }
        else {
          local_f4 = &DAT_00501908;
        }
      }
      local_4 = 0xffffffff;
      if ((local_ec != &DAT_00501908) && (*local_ec = *local_ec + -1, *local_ec < 1)) {
        FUN_004011c0(&local_ec);
      }
      *(int *)((int)param_1 + 0x1a80) = *(int *)((int)param_1 + 0x1a80) + 1;
    }
  }
  fVar1 = _DAT_004e2b34 + *(float *)((int)param_1 + 0x25c);
  *(float *)((int)param_1 + 0x4c) = local_dc;
  local_ec = (int *)&stack0xfffffef0;
  *(float *)((int)param_1 + 0x54) = local_d4;
  *(float *)((int)param_1 + 0x50) = fVar1;
  (**(code **)(*(int *)((int)param_1 + 0x24) + 0xc))();
  local_dc = *(float *)((int)param_1 + 0x26c) - *(float *)((int)param_1 + 0x4c);
  local_d8 = *(float *)((int)param_1 + 0x270) - *(float *)((int)param_1 + 0x50);
  local_d4 = *(float *)((int)param_1 + 0x274) - *(float *)((int)param_1 + 0x54);
  puVar8 = (undefined4 *)FUN_0045a230(&local_dc,(float *)&stack0xffffff08);
  *(undefined4 *)((int)param_1 + 0x250) = *puVar8;
  *(undefined4 *)((int)param_1 + 0x254) = puVar8[1];
  *(undefined4 *)((int)param_1 + 600) = puVar8[2];
  (**(code **)(*(int *)((int)param_1 + 0x24) + 0x14))();
  pfVar9 = FUN_00465ef0(param_1,afStack_74,(float *)((int)param_1 + 0x260));
  FUN_0046d480((void *)((int)param_1 + 0x1480),pfVar9);
  FUN_004665a0((int)param_1);
  ExceptionList = pvStack_24;
  return;
}



/* 0x004682a0 FUN_004682a0 */

void __fastcall FUN_004682a0(int param_1)

{
  FUN_0047d300(*(void **)(param_1 + 0xbc),0);
  *(undefined1 *)(param_1 + 0x24d) = 1;
  FUN_004a11a0((void *)(param_1 + 0x284),(undefined1 *)0x0);
  return;
}



/* 0x00468850 FUN_00468850 */

void __fastcall FUN_00468850(int param_1)

{
  *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xd) = 1;
  *(undefined1 *)(param_1 + 0x25d) = 1;
  *(undefined4 *)(param_1 + 600) = 0;
  FUN_00468540(param_1);
  return;
}



/* 0x00466c50 FUN_00466c50 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00466c50(int param_1)

{
  int *piVar1;
  char cVar2;
  
  FUN_004a11a0((void *)(param_1 + 0x1c0),(undefined1 *)0x0);
  cVar2 = '\x01';
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff0,"RC_HoseLvlWin");
  FUN_004897c0((void *)(param_1 + 0x208),piVar1,cVar2);
  *(undefined4 *)(param_1 + 0x198) = 1;
  *(undefined4 *)(param_1 + 0x18c) = 6;
  return;
}



/* 0x00466ce0 FUN_00466ce0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00466ce0(int param_1)

{
  int *piVar1;
  char cVar2;
  
  FUN_004a11a0((void *)(param_1 + 0x1e4),(undefined1 *)0x0);
  cVar2 = '\x01';
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff0,"RC_HoseLvlLose");
  FUN_004897c0((void *)(param_1 + 0x208),piVar1,cVar2);
  *(undefined4 *)(param_1 + 0x198) = 2;
  *(undefined4 *)(param_1 + 0x18c) = 6;
  return;
}



