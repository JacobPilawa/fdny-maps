/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0046d1b0 FUN_0046d1b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0046d1b0(void *this,float *param_1,float *param_2,char param_3)

{
  void *this_00;
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  float *pfVar4;
  float fVar5;
  float local_8c [3];
  float local_80;
  float local_7c;
  float local_78 [6];
  float local_60 [3];
  float local_54 [3];
  undefined4 local_48 [3];
  float local_3c [3];
  float local_30 [3];
  float local_24 [3];
  undefined4 local_18 [3];
  float local_c [3];
  
  *(float *)((int)this + 0x15c) = *param_2;
  *(float *)((int)this + 0x160) = param_2[1];
  *(float *)((int)this + 0x164) = param_2[2];
  if (*(char *)((int)this + 9) == '\0') {
    if (param_3 != '\0') {
      fVar5 = *(float *)((int)this + 0x3b8) -
              DAT_00502b04 * *(float *)((int)this + 0x3bc) * *(float *)((int)this + 0x3b0);
      *(float *)((int)this + 0x3b8) = fVar5;
      if (fVar5 < _DAT_004dc50c) {
        *(undefined4 *)((int)this + 0x3b8) = 0;
        *(undefined1 *)((int)this + 9) = 1;
      }
      fVar5 = *(float *)((int)this + 0x3b4) - *(float *)((int)this + 0x3b0);
      goto LAB_0046d2b5;
    }
    fVar5 = DAT_00502b04 * *(float *)((int)this + 0x3c0) * (float)_DAT_004e2e80 +
            *(float *)((int)this + 0x3b8);
    *(float *)((int)this + 0x3b8) = fVar5;
    if ((float)_DAT_004dd808 <= fVar5) {
      *(undefined4 *)((int)this + 0x3b8) = 0x3f800000;
    }
  }
  else {
    fVar5 = DAT_00502b04 * *(float *)((int)this + 0x3c0) + *(float *)((int)this + 0x3b8);
    *(float *)((int)this + 0x3b8) = fVar5;
    if ((float)_DAT_004dd808 <= fVar5) {
      RequestSoundInstancePlayback((void *)((int)this + 0x138),(undefined1 *)0x0);
      *(undefined1 *)((int)this + 9) = 0;
      *(undefined4 *)((int)this + 0x3b8) = 0x3f800000;
      thunk_FUN_0049f340((int *)((int)this + 0x114));
    }
  }
  fVar5 = -*(float *)((int)this + 0x3b0);
LAB_0046d2b5:
  *(float *)((int)this + 0x3b0) = fVar5 * (float)_DAT_004e27d8 + *(float *)((int)this + 0x3b0);
  fVar5 = _DAT_004dfc24 - *(float *)((int)this + 0x3b0);
  local_80 = *(float *)((int)this + 0x390) * *(float *)((int)this + 0x3b0) +
             fVar5 * *(float *)((int)this + 0x370);
  *(float *)((int)this + 0x3c4) = local_80;
  fVar5 = *(float *)((int)this + 0x3b0) * *(float *)((int)this + 0x394) +
          fVar5 * *(float *)((int)this + 0x374);
  *(float *)((int)this + 0x3c8) = fVar5;
  local_7c = fVar5;
  pfVar1 = (float *)FUN_00446390(local_8c,param_2,local_80);
  FUN_0046c000((void *)((int)this + 0x168),param_1,pfVar1,fVar5);
  if (*(char *)((int)this + 0x2f0) != '\0') {
    local_8c[0] = *param_2;
    local_8c[2] = param_2[2];
    this_00 = *(void **)((int)this + 0x20);
    fVar5 = 0.025;
    local_78[0] = 0.0;
    local_78[1] = -1.0;
    local_78[2] = 0.0;
    local_8c[1] = 0.0;
    pfVar1 = (float *)FUN_00446ee0(this_00,local_48);
    pfVar1 = (float *)FUN_00446390(local_c,pfVar1,fVar5);
    pfVar2 = (float *)FUN_00446390(local_60,local_78,0.4);
    fVar5 = 0.5125;
    pfVar3 = (float *)FUN_0049e770(this_00,local_18);
    pfVar3 = (float *)FUN_00446390(local_30,pfVar3,fVar5);
    fVar5 = 0.05;
    pfVar4 = (float *)FUN_0042d2b0(local_8c,local_54);
    pfVar4 = (float *)FUN_00446390(local_3c,pfVar4,fVar5);
    pfVar3 = (float *)FUN_0043b2d0(local_24,pfVar4,pfVar3);
    pfVar2 = (float *)FUN_0042d050(local_78 + 3,pfVar3,pfVar2);
    FUN_0043b2d0(local_8c,pfVar2,pfVar1);
    fVar5 = *(float *)((int)this + 0x394) * (float)_DAT_004e1430 +
            *(float *)((int)this + 0x3c8) * (float)_DAT_004e0508;
    pfVar1 = (float *)FUN_00446390(local_78 + 3,local_8c,2000.0);
    FUN_0046c000((void *)((int)this + 0x1d4),param_1,pfVar1,fVar5);
  }
  *(undefined1 *)((int)this + 0x1c) = 0;
  FUN_0046c5a0(this);
  return;
}



/* 0x0046c000 FUN_0046c000 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0046c000(void *this,float *param_1,float *param_2,float param_3)

{
  float *pfVar1;
  float fVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  *(float *)((int)this + 0xc) = *param_1;
  *(float *)((int)this + 0x10) = param_1[1];
  *(float *)((int)this + 0x14) = param_1[2];
  fVar2 = (float)DAT_00502afc;
  if (DAT_00502afc < 0) {
    fVar2 = fVar2 + (float)_DAT_004dfc90;
  }
  fVar2 = fVar2 * (float)_DAT_004dfa28;
  iVar4 = *(int *)((int)this + 0x38);
  if ((((-1 < iVar4) && (iVar3 = *(int *)((int)this + 0x24), iVar3 != 0)) && (iVar4 < iVar3)) &&
     (iVar4 = iVar4 % iVar3, iVar4 < *(int *)((int)this + 0x3c))) {
    FUN_0048d490((int *)((int)this + 0x20),iVar4);
    pfVar1 = *(float **)(*(int *)((int)this + 0x20) + iVar4 * 4);
    if ((pfVar1 != (float *)0x0) && (fVar2 < *(float *)((int)this + 0x1c) + *pfVar1)) {
      return;
    }
  }
  if (*(int *)((int)this + 0x3c) < *(int *)((int)this + 0x24)) {
    do {
      iVar4 = *(int *)((int)this + 0x3c);
      if ((iVar4 < 0) || (*(int *)((int)this + 0x24) <= iVar4)) {
        piVar5 = &local_18;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar5 = 0;
          piVar5 = piVar5 + 1;
        }
        local_18 = 1;
        local_14 = "Array index out of bounds";
        local_10 = "..\\BluelAPI/Container/SArray.h";
        local_c = 0x1cd;
        FUN_00422060(&local_18);
      }
      pfVar1 = *(float **)(*(int *)((int)this + 0x20) + iVar4 * 4);
      *pfVar1 = fVar2;
      pfVar1[1] = *param_1;
      pfVar1[2] = param_1[1];
      pfVar1[3] = param_1[2];
      pfVar1[4] = *param_2;
      pfVar1[5] = param_2[1];
      pfVar1[6] = param_2[2];
      pfVar1[7] = param_3;
      iVar4 = *(int *)((int)this + 0x3c) + 1;
      *(int *)((int)this + 0x3c) = iVar4;
    } while (iVar4 < *(int *)((int)this + 0x24));
  }
  iVar4 = *(int *)((int)this + 0x38) + 1;
  *(int *)((int)this + 0x38) = iVar4;
  if (*(int *)((int)this + 0x24) <= iVar4) {
    *(undefined4 *)((int)this + 0x38) = 0;
  }
  iVar4 = *(int *)((int)this + 0x38);
  FUN_0048d490((int *)((int)this + 0x20),iVar4);
  pfVar1 = *(float **)(*(int *)((int)this + 0x20) + iVar4 * 4);
  *pfVar1 = fVar2;
  pfVar1[1] = *param_1;
  pfVar1[2] = param_1[1];
  pfVar1[3] = param_1[2];
  pfVar1[4] = *param_2;
  pfVar1[5] = param_2[1];
  pfVar1[6] = param_2[2];
  pfVar1[7] = param_3;
  return;
}



/* 0x00466d60 FUN_00466d60 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00466d60(int *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  void *pvVar3;
  int *piVar4;
  int iVar5;
  bool bVar6;
  int *piVar7;
  undefined4 uVar8;
  int *piVar9;
  float fVar10;
  int *local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1c76;
  pvStack_c = ExceptionList;
  local_64 = &PTR_FUN_004dc554;
  ExceptionList = &pvStack_c;
  FUN_004a3118();
  local_60 = "hose\\PlayerHoseEnt.cpp";
  local_5c = 0x152;
  local_58 = 0;
  local_4 = 0;
  FUN_004a3a19(local_48,"PlayerHoseEnt: ");
  FUN_004a3a19(local_48,"Created PlayeHose Ent");
  FUN_00401360(&local_64,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  puVar1 = (undefined4 *)FUN_0049d350(param_1,&local_70);
  local_74 = (int *)&stack0xffffff70;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar7 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff70,"Player");
  InitializeAndRegisterScriptActor(param_1 + 0x50,piVar7,puVar1);
  FUN_00497e90((void *)param_1[0x34],*(void **)(DAT_00503a78 + 4),2,10,0);
  *(undefined4 *)(param_1[0x34] + 0x40) = 0;
  *(undefined1 *)(param_1[0x34] + 0x46) = 1;
  puVar1 = operator_new(0x20);
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
  }
  puVar2 = (undefined4 *)FUN_0049d350(param_1,&local_70);
  puVar1[3] = *puVar2;
  puVar1[4] = puVar2[1];
  puVar1[5] = 0;
  FUN_0042d280(param_1 + 0x9b,&local_70,0,0,0);
  puVar1[6] = param_1 + 0x9b;
  local_74 = operator_new(0x14);
  local_4 = 1;
  if (local_74 == (int *)0x0) {
    pvVar3 = (void *)0x0;
  }
  else {
    pvVar3 = (void *)FUN_0049f230(local_74);
  }
  local_4 = 0xffffffff;
  piVar7 = FUN_0049b760(&local_70,"RCS_CamHose",100);
  local_4 = 2;
  local_74 = (int *)FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar7);
  bVar6 = local_74 != (int *)0x0;
  local_4 = 0xffffffff;
  FUN_0049b6f0((int)&local_70);
  if (bVar6) {
    *(int **)((int)pvVar3 + 0xc) = local_74;
  }
  *(undefined4 *)((int)pvVar3 + 8) = 0;
  FUN_0049d130(pvVar3,puVar1);
  FUN_0049d4c0(pvVar3);
  param_1[0x1d] = 0x42340000;
  FUN_0042d280(param_1 + 0x1e,&local_70,0xc1f00000,0xc2340000,0xc1f00000);
  FUN_0042d280(param_1 + 0x21,&local_70,0x41f00000,0x42340000,0x41f00000);
  local_70 = 0;
  param_1[0x42] = 0;
  local_6c = 0xc4fa0000;
  param_1[0x43] = -0x3b060000;
  local_68 = 0;
  param_1[0x44] = 0;
  param_1[0x8f] = 0x42700000;
  param_1[0x8e] = 0x42340000;
  param_1[0x8c] = 0;
  param_1[0x8d] = 0;
  param_1[0x90] = 0;
  param_1[0x694] = 0;
  param_1[0x693] = 0;
  param_1[0x696] = 1;
  param_1[0x69f] = 0;
  param_1[0x6a0] = 0;
  param_1[99] = 3;
  FUN_004661a0(param_1);
  piVar9 = (int *)0x1;
  uVar8 = 0x19;
  piVar7 = param_1 + 0x520;
  piVar4 = (int *)(**(code **)(*param_1 + 0x18))();
  FUN_0046d6b0(piVar7,piVar4,uVar8,piVar9);
  FUN_00465580(param_1 + 0x9e,*(int **)(DAT_00503a78 + 4),param_1,piVar7);
  pvVar3 = operator_new(8);
  *(int **)((int)pvVar3 + 4) = piVar7;
  local_74 = operator_new(0x14);
  local_4 = 3;
  if (local_74 == (int *)0x0) {
    local_74 = (int *)0x0;
  }
  else {
    local_74 = (int *)FUN_0049f230(local_74);
  }
  local_4 = 0xffffffff;
  piVar7 = FUN_0049b760(&local_70,"RCS_DouseHose",100);
  local_4 = 4;
  iVar5 = FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar7);
  local_4 = 0xffffffff;
  FUN_0049b6f0((int)&local_70);
  if (iVar5 != 0) {
    local_74[3] = iVar5;
  }
  local_74[2] = 0;
  FUN_0049d130(local_74,pvVar3);
  FUN_0049d4c0(local_74);
  param_1[0x66] = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_74 = &DAT_00501908;
  FUN_00401d80(&local_74,"HoseSpray");
  local_4 = 5;
  ConfigureAndRegisterSoundInstance(param_1 + 0x67,&local_74,0,2);
  local_4 = 0xffffffff;
  if ((local_74 != &DAT_00501908) && (*local_74 = *local_74 + -1, *local_74 < 1)) {
    FUN_004011c0(&local_74);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_74 = &DAT_00501908;
  FUN_00401d80(&local_74,"LevelEndWin");
  local_4 = 6;
  ConfigureAndRegisterSoundInstance(param_1 + 0x70,&local_74,0,2);
  local_4 = 0xffffffff;
  if ((local_74 != &DAT_00501908) && (*local_74 = *local_74 + -1, *local_74 < 1)) {
    FUN_004011c0(&local_74);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_74 = &DAT_00501908;
  FUN_00401d80(&local_74,"LevelEndLose");
  local_4 = 7;
  ConfigureAndRegisterSoundInstance(param_1 + 0x79,&local_74,0,2);
  local_4 = 0xffffffff;
  if ((local_74 != &DAT_00501908) && (*local_74 = *local_74 + -1, *local_74 < 1)) {
    FUN_004011c0(&local_74);
  }
  FUN_004895c0(param_1 + 0x82);
  param_1[0x65c] = 2;
  FUN_0042d280(param_1 + 0x65d,&local_70,0x42480000,0x42480000,0x42480000);
  FUN_0047f1f0(param_1 + 0x630,0x3f000000);
  fVar10 = 90.0;
  param_1[0x659] = 0x20;
  pvVar3 = (void *)FUN_0047f1a0((int)(param_1 + 0x630));
  FUN_00496b50(pvVar3,fVar10);
  ExceptionList = pvStack_c;
  return;
}



/* 0x00466780 FUN_00466780 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00466780(int *param_1)

{
  bool bVar1;
  char cVar2;
  void *pvVar3;
  undefined4 uVar4;
  int iVar5;
  float *pfVar6;
  float10 fVar7;
  float fVar8;
  float *pfVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  int **ppiVar13;
  float fVar14;
  int *local_cc;
  int *piStack_c8;
  float afStack_c4 [6];
  undefined1 auStack_ac [16];
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  float afStack_68 [3];
  undefined4 auStack_5c [3];
  undefined1 auStack_50 [68];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1c1c;
  pvStack_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_cc = &DAT_00501908;
  ExceptionList = &pvStack_c;
  FUN_00401d80(&local_cc,"MOUSE_LB");
  ppiVar13 = &local_cc;
  local_4 = 0;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar4 = FUN_00438480(pvVar3,(int *)ppiVar13);
  if ((((char)uVar4 == '\0') || (*(char *)((int)param_1 + 0x1489) != '\0')) || (param_1[99] == 6)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  local_4 = 0xffffffff;
  if ((local_cc != &DAT_00501908) && (*local_cc = *local_cc + -1, *local_cc < 1)) {
    FUN_004011c0(&local_cc);
  }
  if (bVar1) {
    bVar1 = IsFMODChannelPlaying(param_1 + 0x67);
    if (!bVar1) {
      FUN_004a11d0(param_1 + 0x67);
    }
  }
  else {
    thunk_FUN_0049f340(param_1 + 0x67);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_cc = &DAT_00501908;
  FUN_00401d80(&local_cc,"STEER_PITCH");
  ppiVar13 = &local_cc;
  local_4 = 1;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  fVar7 = FUN_004386f0(pvVar3,(int *)ppiVar13);
  param_1[0x8c] = (int)(float)((float10)(float)param_1[0x8c] - fVar7);
  local_4 = 0xffffffff;
  if ((local_cc != &DAT_00501908) && (*local_cc = *local_cc + -1, *local_cc < 1)) {
    FUN_004011c0(&local_cc);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_cc = &DAT_00501908;
  FUN_00401d80(&local_cc,"STEER_YAW");
  ppiVar13 = &local_cc;
  local_4 = 2;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  fVar7 = FUN_004386f0(pvVar3,(int *)ppiVar13);
  param_1[0x8d] = (int)(float)((float10)(float)param_1[0x8d] - fVar7);
  local_4 = 0xffffffff;
  if ((local_cc != &DAT_00501908) && (*local_cc = *local_cc + -1, *local_cc < 1)) {
    FUN_004011c0(&local_cc);
  }
  uVar4 = (**(code **)(*param_1 + 0x18))();
  FUN_0042d2d0(auStack_50,uVar4);
  FUN_004977b0(auStack_ac,(int)auStack_50);
  local_cc = (int *)&stack0xffffff18;
  pfVar9 = (float *)&stack0xffffff18;
  uStack_9c = 0;
  uStack_98 = 0x3f800000;
  uStack_94 = 0;
  iVar5 = (**(code **)(*param_1 + 0x18))();
  pvVar3 = (void *)FUN_0043b2d0(afStack_c4,(float *)(param_1 + 0x9b),(float *)(iVar5 + 0x28));
  FUN_0045a230(pvVar3,pfVar9);
  fVar10 = 6.466554e-39;
  FUN_00496360((int)auStack_ac);
  if ((float)param_1[0x8e] < (float)param_1[0x8c]) {
    param_1[0x8c] = param_1[0x8e];
  }
  if ((float)param_1[0x8c] < -(float)param_1[0x8e]) {
    param_1[0x8c] = (int)-(float)param_1[0x8e];
  }
  if ((float)param_1[0x8f] < (float)param_1[0x8d]) {
    param_1[0x8d] = param_1[0x8f];
  }
  if ((float)param_1[0x8d] < -(float)param_1[0x8f]) {
    param_1[0x8d] = (int)-(float)param_1[0x8f];
  }
  pfVar9 = afStack_c4;
  afStack_c4[0] = 0.0;
  afStack_c4[1] = 1.0;
  afStack_c4[2] = 0.0;
  fVar11 = 6.466814e-39;
  pvVar3 = (void *)FUN_0049e770(auStack_ac,auStack_5c);
  fVar12 = 6.466824e-39;
  fVar8 = ComputeVectorAngleDegrees(pvVar3,pfVar9);
  fVar14 = _DAT_004e169c + -fVar8;
  if ((float)param_1[0x8c] < fVar14) {
    param_1[0x8c] = (int)fVar14;
  }
  fVar14 = (-fVar8 + _DAT_004e1cbc) - _DAT_004e169c;
  if (fVar14 < (float)param_1[0x8c]) {
    param_1[0x8c] = (int)fVar14;
  }
  fVar14 = (float)param_1[0x8c];
  local_cc = (int *)&stack0xffffff14;
  FUN_00446ee0(auStack_ac,(undefined4 *)&stack0xffffff14);
  FUN_00496dd0(auStack_ac,fVar10,fVar11,fVar12,fVar14);
  fVar14 = -(float)param_1[0x8d];
  local_cc = (int *)&stack0xffffff14;
  FUN_00447830(auStack_ac,(undefined4 *)&stack0xffffff14);
  FUN_00496dd0(auStack_ac,fVar10,fVar11,fVar12,fVar14);
  FUN_0049e770(auStack_ac,afStack_c4 + 3);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_c8 = &DAT_00501908;
  FUN_00401d80(&piStack_c8,"MOUSE_LB");
  ppiVar13 = &piStack_c8;
  local_4 = 3;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar4 = FUN_00438480(pvVar3,(int *)ppiVar13);
  cVar2 = (char)uVar4;
  pfVar9 = afStack_c4 + 3;
  pfVar6 = FUN_00465ef0(param_1,afStack_68,(float *)0x0);
  FUN_0046d1b0(param_1 + 0x520,pfVar6,pfVar9,cVar2);
  local_4 = 0xffffffff;
  if ((piStack_c8 != &DAT_00501908) && (*piStack_c8 = *piStack_c8 + -1, *piStack_c8 < 1)) {
    FUN_004011c0(&piStack_c8);
  }
  ExceptionList = pvStack_c;
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
      uVar10 = ConvertX87FloatToIntegerTowardZero();
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



/* 0x00468850 FUN_00468850 */

void __fastcall FUN_00468850(int param_1)

{
  *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xd) = 1;
  *(undefined1 *)(param_1 + 0x25d) = 1;
  *(undefined4 *)(param_1 + 600) = 0;
  FUN_00468540(param_1);
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



