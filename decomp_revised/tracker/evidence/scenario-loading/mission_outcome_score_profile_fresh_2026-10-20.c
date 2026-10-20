/*
 * Read-only Ghidra 12.0.4 decompilation of the mission statistic registry,
 * score profile reader/writer, and Records-page value formatter.
 * Input: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Saved analysis: .local/mission-lifecycle-ghidra/FDNYPort
 * Exported by DecompileTargetAndRefs.java. Signatures and field semantics are
 * kept as Ghidra presented them; no profile-to-display mapping is inferred.
 */

/* FUNCTION 00424710 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00424710(void *this,int *param_1)

{
  void *this_00;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd168;
  local_c = ExceptionList;
  this_00 = (void *)((int)this + 0x30);
  ExceptionList = &local_c;
  local_10 = this;
  FUN_004883d0((int)this_00);
  if ((char)param_1 != '\0') {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Completion Time");
    local_4 = 0;
    FUN_00488660(this_00,(int *)&param_1,1,0,0,0,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    FUN_004868a0((int)this_00);
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Bonus Points");
    local_4 = 1;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    FUN_004868a0((int)this_00);
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Rescues");
    local_4 = 2;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Shortest Rescue Time");
    local_4 = 3;
    FUN_00488660(this_00,(int *)&param_1,1,1,999999999,0,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Longest Rescue Time");
    local_4 = 4;
    FUN_00488660(this_00,(int *)&param_1,1,2,0,0,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    FUN_004868a0((int)this_00);
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Axe Hits");
    local_4 = 5;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Gallons Sprayed");
    local_4 = 6;
    FUN_00488660(this_00,(int *)&param_1,2,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Objects Thrown");
    local_4 = 7;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Air Time");
    local_4 = 8;
    FUN_00488660(this_00,(int *)&param_1,1,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Fires Doused");
    local_4 = 9;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Serious Burns");
    local_4 = 10;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_1 = &DAT_00501908;
    FUN_00401d80(&param_1,"Jumps");
    local_4 = 0xb;
    FUN_00488660(this_00,(int *)&param_1,0,0,0,1,0);
    local_4 = 0xffffffff;
    if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
      FUN_004011c0(&param_1);
    }
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_10 = &DAT_00501908;
    FUN_00401d80(&local_10,"Total Steps");
    local_4 = 0xc;
    FUN_00488660(this_00,(int *)&local_10,0,0,0,0,0);
    local_4 = 0xffffffff;
    if ((local_10 != &DAT_00501908) && (*local_10 = *local_10 + -1, *local_10 < 1)) {
      FUN_004011c0(&local_10);
    }
  }
  ExceptionList = local_c;
  return;
}

/* FUNCTION 00425990 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00425990(void *this,int *param_1)

{
  char cVar1;
  int *piVar2;
  void *pvVar3;
  void *pvVar4;
  int *piVar5;
  int iVar6;
  int *extraout_ECX;
  undefined3 unaff_EBX;
  char *pcVar7;
  char *pcVar8;
  bool bVar9;
  int *piVar10;
  int *piStack_ac;
  int iVar11;
  int *piStack_8c;
  int *piStack_88;
  undefined1 *local_84;
  void *local_80;
  int iStack_78;
  undefined1 local_74 [4];
  undefined **ppuStack_70;
  char *pcStack_6c;
  undefined4 uStack_68;
  undefined1 uStack_64;
  undefined1 auStack_54 [68];
  void *pvStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd286;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  local_80 = this;
  FUN_004883d0((int)this + 8);
  FUN_0040c2d0(local_74,"main\\PlayerManager.cpp",0x2c);
  local_4 = 0;
  piVar2 = (int *)(**(code **)(*param_1 + 0x38))();
  bVar9 = true;
  iVar6 = 6;
  pcVar7 = (char *)(*piVar2 + 0xc);
  pcVar8 = "Score";
  do {
    if (iVar6 == 0) break;
    iVar6 = iVar6 + -1;
    bVar9 = *pcVar7 == *pcVar8;
    pcVar7 = pcVar7 + 1;
    pcVar8 = pcVar8 + 1;
  } while (bVar9);
  piVar2 = (int *)CONCAT13(!bVar9,unaff_EBX);
  if ((piStack_88 != &DAT_00501908) && (*piStack_88 = *piStack_88 + -1, *piStack_88 < 1)) {
    FUN_004011c0(&piStack_88);
  }
  if (!bVar9) {
    iVar11 = 1;
    piStack_ac = (int *)0x425a3e;
    pvVar3 = (void *)(**(code **)(*param_1 + 0x38))();
    local_84 = (undefined1 *)&piStack_ac;
    pvStack_c._0_1_ = 1;
    FUN_0040e9a0(pvVar3,&piStack_ac,(int *)0x22,0x22,(int *)0x1);
    piVar5 = (int *)&stack0xffffff70;
    pvStack_c = (void *)CONCAT31(pvStack_c._1_3_,2);
    pvVar3 = (void *)(**(code **)(*param_1 + 0x4c))();
    local_84 = &stack0xffffff4c;
    pvStack_10._0_1_ = 3;
    piVar10 = extraout_ECX;
    FUN_0040e9a0(pvVar3,&stack0xffffff4c,(int *)0x22,0x22,(int *)0x1);
    pvStack_10._0_1_ = 4;
    ppuStack_70 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_6c = "main\\PlayerManager.cpp";
    uStack_68 = 0x2e;
    uStack_64 = 0;
    pvStack_10._0_1_ = 6;
    FUN_004a3a19(auStack_54,"RichBinaryReader (BeginNamedRBR) : ");
    pvVar4 = FUN_0040f020(&ppuStack_70,piVar10);
    pvVar3 = (void *)((int)pvVar4 + 0x1c);
    pvStack_10 = (void *)CONCAT31(pvStack_10._1_3_,8);
    FUN_004a3a19(pvVar3," : Expected node named \"");
    FUN_004a3a19(pvVar3,"Score");
    FUN_004a3a19(pvVar3,"\" but got node named ");
    pvVar4 = FUN_0040f020(pvVar4,piVar5);
    pvVar3 = (void *)((int)pvVar4 + 0x1c);
    FUN_004a3a19(pvVar3," at position ");
    iVar6 = (**(code **)(*param_1 + 0x40))();
    piStack_ac = (int *)0x425b35;
    FUN_004a3eb9(pvVar3,iVar6);
    piStack_ac = (int *)0x425b41;
    FUN_004a3a19(pvVar3,".\n (file \"");
    piStack_ac = (int *)0x425b4d;
    FUN_004a3a19(pvVar3,"main\\PlayerManager.cpp");
    piStack_ac = (int *)0x425b59;
    FUN_004a3a19(pvVar3,"\" line ");
    piStack_ac = (int *)0x425b62;
    FUN_004a3f7a(pvVar3,0x2e);
    piStack_ac = (int *)0x425b6e;
    FUN_004a3a19(pvVar3,")");
    FUN_00401360(pvVar4,iVar11);
    puStack_8._0_1_ = 7;
    FUN_004012c0(&uStack_68);
    puStack_8._0_1_ = 1;
    if (piStack_8c != &DAT_00501908) {
      *piStack_8c = *piStack_8c + -1;
      if (*piStack_8c < 1) {
        FUN_004011c0(&piStack_8c);
      }
      else {
        piStack_8c = &DAT_00501908;
      }
    }
    puStack_8 = (undefined1 *)((uint)puStack_8._1_3_ << 8);
    if ((piStack_88 != &DAT_00501908) && (*piStack_88 = *piStack_88 + -1, *piStack_88 < 1)) {
      FUN_004011c0(&piStack_88);
    }
  }
  iStack_78 = iStack_78 + 1;
  _DAT_00502404 = "main\\PlayerManager.cpp";
  _DAT_00502408 = 0x2e;
  cVar1 = (**(code **)(*param_1 + 0x30))();
  do {
    if (cVar1 == '\0') {
      _DAT_00502404 = "main\\PlayerManager.cpp";
      _DAT_00502408 = 0x31;
      *(bool *)((int)this + 0x30) = *(int *)(*(int *)((int)this + 0x2c) + 4) != 0;
      iStack_78 = iStack_78 + -1;
      FUN_0040c7b0(&iStack_78);
      puStack_8 = (undefined1 *)0xffffffff;
      FUN_0043f660();
      ExceptionList = pvStack_10;
      return;
    }
    piVar5 = (int *)(**(code **)(*param_1 + 0x38))();
    bVar9 = true;
    iVar6 = 10;
    pcVar7 = (char *)(*piVar5 + 0xc);
    pcVar8 = "LevelName";
    do {
      if (iVar6 == 0) break;
      iVar6 = iVar6 + -1;
      bVar9 = *pcVar7 == *pcVar8;
      pcVar7 = pcVar7 + 1;
      pcVar8 = pcVar8 + 1;
    } while (bVar9);
    if (piVar2 != &DAT_00501908) {
      *piVar2 = *piVar2 + -1;
      if (*piVar2 < 1) {
        FUN_004011c0((undefined4 *)&stack0xffffff70);
      }
      else {
        piVar2 = &DAT_00501908;
      }
    }
    if (bVar9) {
      piStack_ac = (int *)0x425c66;
      (**(code **)(*param_1 + 4))();
    }
    else {
      piStack_ac = (int *)0x425c70;
      piVar5 = (int *)(**(code **)(*param_1 + 0x38))();
      bVar9 = true;
      iVar6 = 6;
      pcVar7 = (char *)(*piVar5 + 0xc);
      pcVar8 = "stats";
      do {
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        bVar9 = *pcVar7 == *pcVar8;
        pcVar7 = pcVar7 + 1;
        pcVar8 = pcVar8 + 1;
      } while (bVar9);
      if (piVar2 != &DAT_00501908) {
        *piVar2 = *piVar2 + -1;
        if (*piVar2 < 1) {
          piStack_ac = (int *)0x425caf;
          FUN_004011c0((undefined4 *)&stack0xffffff70);
        }
        else {
          piVar2 = &DAT_00501908;
        }
      }
      if (bVar9) {
        piStack_ac = piStack_8c + 2;
        (**(code **)*param_1)();
      }
      else {
        piStack_ac = (int *)0x425cca;
        (**(code **)(*param_1 + 0x34))();
      }
    }
    piStack_ac = (int *)0x425cd2;
    cVar1 = (**(code **)(*param_1 + 0x30))();
    this = local_84;
  } while( true );
}

/* FUNCTION 00425D50 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00425d50(int *param_1)

{
  undefined4 *puVar1;
  char cVar2;
  int *piVar3;
  void *pvVar4;
  void *pvVar5;
  undefined4 uVar6;
  int iVar7;
  int *extraout_ECX;
  int *unaff_EBX;
  int *unaff_EBP;
  char *pcVar8;
  char *pcVar9;
  bool bVar10;
  int *piVar11;
  undefined1 *puStack_a8;
  int iVar12;
  int iStack_84;
  int *apiStack_80 [2];
  int local_78;
  undefined1 local_74 [4];
  undefined **ppuStack_70;
  char *pcStack_6c;
  undefined4 uStack_68;
  undefined1 uStack_64;
  undefined1 auStack_54 [68];
  void *pvStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd2c6;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  FUN_0040c2d0(local_74,"main\\PlayerManager.cpp",0xe1);
  local_4 = 0;
  piVar3 = (int *)(**(code **)(*param_1 + 0x38))();
  bVar10 = true;
  iVar7 = 7;
  pcVar8 = (char *)(*piVar3 + 0xc);
  pcVar9 = "Player";
  do {
    if (iVar7 == 0) break;
    iVar7 = iVar7 + -1;
    bVar10 = *pcVar8 == *pcVar9;
    pcVar8 = pcVar8 + 1;
    pcVar9 = pcVar9 + 1;
  } while (bVar10);
  if ((apiStack_80[0] != &DAT_00501908) &&
     (*apiStack_80[0] = *apiStack_80[0] + -1, *apiStack_80[0] < 1)) {
    FUN_004011c0(apiStack_80);
  }
  if (!bVar10) {
    iVar12 = 1;
    puStack_a8 = (undefined1 *)0x425df2;
    pvVar4 = (void *)(**(code **)(*param_1 + 0x38))();
    pvStack_c._0_1_ = 1;
    FUN_0040e9a0(pvVar4,&puStack_a8,(int *)0x22,0x22,(int *)0x1);
    piVar3 = (int *)&stack0xffffff70;
    pvStack_c = (void *)CONCAT31(pvStack_c._1_3_,2);
    pvVar4 = (void *)(**(code **)(*param_1 + 0x4c))();
    pvStack_10._0_1_ = 3;
    piVar11 = extraout_ECX;
    FUN_0040e9a0(pvVar4,&stack0xffffff50,(int *)0x22,0x22,(int *)0x1);
    pvStack_10._0_1_ = 4;
    ppuStack_70 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_6c = "main\\PlayerManager.cpp";
    uStack_68 = 0xe3;
    uStack_64 = 0;
    pvStack_10._0_1_ = 6;
    FUN_004a3a19(auStack_54,"RichBinaryReader (BeginNamedRBR) : ");
    pvVar5 = FUN_0040f020(&ppuStack_70,piVar11);
    pvVar4 = (void *)((int)pvVar5 + 0x1c);
    pvStack_10 = (void *)CONCAT31(pvStack_10._1_3_,8);
    FUN_004a3a19(pvVar4," : Expected node named \"");
    FUN_004a3a19(pvVar4,"Player");
    FUN_004a3a19(pvVar4,"\" but got node named ");
    pvVar5 = FUN_0040f020(pvVar5,piVar3);
    pvVar4 = (void *)((int)pvVar5 + 0x1c);
    FUN_004a3a19(pvVar4," at position ");
    iVar7 = (**(code **)(*param_1 + 0x40))();
    puStack_a8 = (undefined1 *)0x425ee5;
    FUN_004a3eb9(pvVar4,iVar7);
    puStack_a8 = (undefined1 *)0x425ef1;
    FUN_004a3a19(pvVar4,".\n (file \"");
    puStack_a8 = (undefined1 *)0x425efd;
    FUN_004a3a19(pvVar4,"main\\PlayerManager.cpp");
    puStack_a8 = (undefined1 *)0x425f09;
    FUN_004a3a19(pvVar4,"\" line ");
    puStack_a8 = (undefined1 *)0x425f15;
    FUN_004a3f7a(pvVar4,0xe3);
    puStack_a8 = (undefined1 *)0x425f21;
    FUN_004a3a19(pvVar4,")");
    FUN_00401360(pvVar5,iVar12);
    puStack_8._0_1_ = 7;
    FUN_004012c0(&uStack_68);
    puStack_8._0_1_ = 1;
    if (unaff_EBX != &DAT_00501908) {
      *unaff_EBX = *unaff_EBX + -1;
      if (*unaff_EBX < 1) {
        FUN_004011c0((undefined4 *)&stack0xffffff74);
      }
      else {
        unaff_EBX = &DAT_00501908;
      }
    }
    puStack_8 = (undefined1 *)((uint)puStack_8._1_3_ << 8);
    unaff_EBP = (int *)&stack0xffffff50;
    if ((apiStack_80[0] != &DAT_00501908) &&
       (*apiStack_80[0] = *apiStack_80[0] + -1, unaff_EBP = (int *)&stack0xffffff50,
       *apiStack_80[0] < 1)) {
      FUN_004011c0(apiStack_80);
      unaff_EBP = (int *)&stack0xffffff50;
    }
  }
  local_78 = local_78 + 1;
  _DAT_00502404 = "main\\PlayerManager.cpp";
  _DAT_00502408 = 0xe3;
  cVar2 = (**(code **)(*param_1 + 0x30))();
  do {
    if (cVar2 == '\0') {
      local_78 = local_78 + -1;
      _DAT_00502404 = "main\\PlayerManager.cpp";
      _DAT_00502408 = 0xe8;
      FUN_0040c7b0(&local_78);
      puStack_8 = (undefined1 *)0xffffffff;
      FUN_0043f660();
      ExceptionList = pvStack_10;
      return;
    }
    piVar3 = (int *)(**(code **)(*param_1 + 0x38))();
    bVar10 = true;
    iVar7 = 5;
    pcVar8 = (char *)(*piVar3 + 0xc);
    pcVar9 = "Name";
    do {
      if (iVar7 == 0) break;
      iVar7 = iVar7 + -1;
      bVar10 = *pcVar8 == *pcVar9;
      pcVar8 = pcVar8 + 1;
      pcVar9 = pcVar9 + 1;
    } while (bVar10);
    if (unaff_EBP != &DAT_00501908) {
      *unaff_EBP = *unaff_EBP + -1;
      if (*unaff_EBP < 1) {
        FUN_004011c0((undefined4 *)&stack0xffffff70);
      }
      else {
        unaff_EBP = &DAT_00501908;
      }
    }
    if (bVar10) {
      puStack_a8 = (undefined1 *)0x426018;
      (**(code **)(*param_1 + 4))();
    }
    else {
      puStack_a8 = (undefined1 *)0x426025;
      piVar3 = (int *)(**(code **)(*param_1 + 0x38))();
      bVar10 = true;
      iVar7 = 0xb;
      pcVar8 = (char *)(*piVar3 + 0xc);
      pcVar9 = "LevelIndex";
      do {
        if (iVar7 == 0) break;
        iVar7 = iVar7 + -1;
        bVar10 = *pcVar8 == *pcVar9;
        pcVar8 = pcVar8 + 1;
        pcVar9 = pcVar9 + 1;
      } while (bVar10);
      if (unaff_EBP != &DAT_00501908) {
        *unaff_EBP = *unaff_EBP + -1;
        if (*unaff_EBP < 1) {
          puStack_a8 = (undefined1 *)0x426064;
          FUN_004011c0((undefined4 *)&stack0xffffff70);
        }
        else {
          unaff_EBP = &DAT_00501908;
        }
      }
      if (bVar10) {
        puStack_a8 = (undefined1 *)(iStack_84 + 4);
        (**(code **)(*param_1 + 0x14))();
      }
      else {
        puStack_a8 = &stack0xffffff74;
        piVar3 = (int *)(**(code **)(*param_1 + 0x38))();
        bVar10 = true;
        iVar7 = 0x10;
        pcVar8 = (char *)(*piVar3 + 0xc);
        pcVar9 = "DifficultyLevel";
        do {
          if (iVar7 == 0) break;
          iVar7 = iVar7 + -1;
          bVar10 = *pcVar8 == *pcVar9;
          pcVar8 = pcVar8 + 1;
          pcVar9 = pcVar9 + 1;
        } while (bVar10);
        if (unaff_EBP != &DAT_00501908) {
          *unaff_EBP = *unaff_EBP + -1;
          if (*unaff_EBP < 1) {
            FUN_004011c0((undefined4 *)&stack0xffffff70);
          }
          else {
            unaff_EBP = &DAT_00501908;
          }
        }
        if (bVar10) {
          (**(code **)(*param_1 + 0x24))();
        }
        else {
          pvVar4 = (void *)(**(code **)(*param_1 + 0x38))();
          uVar6 = CompareRatStringBytes(pvVar4,(byte *)"Score");
          if (unaff_EBP != &DAT_00501908) {
            *unaff_EBP = *unaff_EBP + -1;
            if (*unaff_EBP < 1) {
              FUN_004011c0((undefined4 *)&stack0xffffff70);
            }
            else {
              unaff_EBP = &DAT_00501908;
            }
          }
          if ((char)uVar6 == '\0') {
            (**(code **)(*param_1 + 0x34))();
          }
          else {
            puVar1 = (undefined4 *)*param_1;
            FUN_00423fa0((int)unaff_EBX);
            (*(code *)*puVar1)();
          }
        }
      }
    }
    puStack_a8 = (undefined1 *)0x426137;
    cVar2 = (**(code **)(*param_1 + 0x30))();
  } while( true );
}

/* FUNCTION 00424DC0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00424dc0(void *this,int *param_1)

{
  int iVar1;
  int *unaff_EBP;
  int iVar2;
  undefined4 *puVar3;
  char *local_38;
  char *local_34;
  undefined4 uStack_30;
  void *pvStack_24;
  uint uStack_1c;
  int *apiStack_14 [2];
  int *piStack_c;
  undefined1 *puStack_8;
  undefined4 *local_4;

  local_4 = (undefined4 *)0xffffffff;
  puStack_8 = &LAB_004cd1a8;
  piStack_c = ExceptionList;
  ExceptionList = &piStack_c;
  FUN_0040c2d0(&local_34,"main\\PlayerManager.cpp",0xf0);
  local_4 = (undefined4 *)0x0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_38 = (char *)&DAT_00501908;
  FUN_00401d80(&local_38,"Name");
  local_4 = (undefined4 *)CONCAT31(local_4._1_3_,1);
  (**(code **)(*param_1 + 0x10))((int)this + 0xc);
  piStack_c = (int *)((uint)piStack_c & 0xffffff00);
  if ((unaff_EBP != &DAT_00501908) && (*unaff_EBP = *unaff_EBP + -1, *unaff_EBP < 1)) {
    FUN_004011c0((undefined4 *)&stack0xffffffc0);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_4 = &DAT_00501908;
  FUN_00401d80(&local_4,"LevelIndex");
  piStack_c = (int *)CONCAT31(piStack_c._1_3_,2);
  (**(code **)(*param_1 + 0x20))(*(undefined4 *)((int)this + 4),&local_4);
  apiStack_14[0] = (int *)((uint)apiStack_14[0] & 0xffffff00);
  if ((piStack_c != &DAT_00501908) && (*piStack_c = *piStack_c + -1, *piStack_c < 1)) {
    FUN_004011c0(&piStack_c);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_c = &DAT_00501908;
  FUN_00401d80(&piStack_c,"DifficultyLevel");
  apiStack_14[0] = (int *)CONCAT31(apiStack_14[0]._1_3_,3);
  (**(code **)(*param_1 + 0x30))(*(undefined1 *)((int)this + 0x2c),&piStack_c);
  uStack_1c = uStack_1c & 0xffffff00;
  if ((apiStack_14[0] != &DAT_00501908) &&
     (*apiStack_14[0] = *apiStack_14[0] + -1, *apiStack_14[0] < 1)) {
    FUN_004011c0(apiStack_14);
  }
  iVar2 = 0;
  if (0 < *(int *)((int)this + 0x18)) {
    do {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      apiStack_14[0] = &DAT_00501908;
      FUN_00401d80(apiStack_14,"Score");
      uStack_1c._0_1_ = 4;
      if ((iVar2 < 0) || (*(int *)((int)this + 0x18) <= iVar2)) {
        puVar3 = (undefined4 *)&stack0xffffffc4;
        for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
          *puVar3 = 0;
          puVar3 = puVar3 + 1;
        }
        local_38 = "Array index out of bounds";
        local_34 = "..\\BluelAPI/Container/SArray.h";
        uStack_30 = 0x1cd;
        FUN_00422060((int *)&stack0xffffffc4);
      }
      (**(code **)(*param_1 + 8))
                (*(undefined4 *)(*(int *)((int)this + 0x14) + iVar2 * 4),apiStack_14);
      uStack_1c = (uint)uStack_1c._1_3_ << 8;
      if (apiStack_14[0] != &DAT_00501908) {
        *apiStack_14[0] = *apiStack_14[0] + -1;
        if (*apiStack_14[0] < 1) {
          FUN_004011c0(apiStack_14);
        }
        else {
          apiStack_14[0] = &DAT_00501908;
        }
      }
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)((int)this + 0x18));
  }
  FUN_0040c7b0((int *)&stack0xffffffb4);
  uStack_1c = 0xffffffff;
  FUN_0043f660();
  ExceptionList = pvStack_24;
  return;
}

/* FUNCTION 00488660 */

void __thiscall
FUN_00488660(void *this,int *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6)

{
  undefined4 *puVar1;
  int iVar2;
  void *this_00;
  int *piVar3;
  undefined1 *puVar4;

  iVar2 = *param_1;
  this_00 = *(void **)((int)this + 4);
  if ((this_00 != (void *)0x0) && (*(int *)((int)this_00 + 4) != 0)) {
    FUN_0043b6e0(this_00,0);
  }
  puVar4 = FUN_004880c0((void *)((int)this + 4),(int *)((int)this + 0x18),(byte *)(iVar2 + 0xc));
  piVar3 = *(int **)(puVar4 + 0x1c);
  puVar1 = (undefined4 *)(puVar4 + 0x1c);
  if (piVar3 != (int *)*param_1) {
    if (piVar3 != &DAT_00501908) {
      *piVar3 = *piVar3 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    piVar3 = (int *)*param_1;
    *puVar1 = piVar3;
    if (piVar3 != &DAT_00501908) {
      *piVar3 = *piVar3 + 1;
    }
  }
  *(undefined4 *)(puVar4 + 0x24) = param_4;
  *(undefined4 *)(puVar4 + 0x10) = param_2;
  *(undefined4 *)(puVar4 + 0x14) = param_3;
  *(undefined4 *)(puVar4 + 0x28) = param_6;
  *(undefined4 *)(puVar4 + 0x18) = param_5;
  *(int *)(puVar4 + 0x20) = *(int *)((int)this + 0xc) + -1;
  return;
}

/* FUNCTION 00486EC0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __thiscall
FUN_00486ec0(void *this,undefined4 *param_1,int *param_2,int param_3,int *param_4)

{
  int iVar1;
  undefined4 *puVar2;
  int *piVar3;
  void *pvVar4;
  int *piVar5;
  bool bVar6;
  bool bVar7;
  ulonglong uVar8;
  char cVar9;
  char cVar10;
  int *local_20;
  int *local_1c;
  int *local_18;
  int *local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;

  puStack_8 = &LAB_004d3887;
  local_c = ExceptionList;
  local_10 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_20 = &DAT_00501908;
  local_4 = 1;
  if (*(int *)((int)this + 4) == 0) {
    ExceptionList = &local_c;
    ConvertX87FloatToIntegerTowardZero();
    puVar2 = FUN_00406880(&param_4);
    piVar5 = (int *)*puVar2;
    piVar3 = &DAT_00501908;
    if (piVar5 != &DAT_00501908) {
      *piVar5 = *piVar5 + 1;
      piVar3 = piVar5;
      local_20 = piVar5;
    }
    local_4._0_1_ = 1;
    if ((param_4 != &DAT_00501908) && (*param_4 = *param_4 + -1, *param_4 < 1)) {
      FUN_004011c0(&param_4);
    }
  }
  else if (*(int *)((int)this + 4) == 2) {
    ExceptionList = &local_c;
    ConvertX87FloatToIntegerTowardZero();
    puVar2 = FUN_00406880(&param_4);
    local_4._0_1_ = 3;
    FUN_00403b00(&local_20,puVar2);
    local_4._0_1_ = 1;
    piVar3 = local_20;
    if (param_4 != &DAT_00501908) {
      *param_4 = *param_4 + -1;
      iVar1 = *param_4;
LAB_00487007:
      local_4._0_1_ = 1;
      piVar3 = local_20;
      if (iVar1 < 1) {
        FUN_004011c0(&param_4);
        piVar3 = local_20;
      }
    }
  }
  else {
    cVar10 = '\x01';
    cVar9 = '\x01';
    ExceptionList = &local_c;
    uVar8 = ConvertX87FloatToIntegerTowardZero();
    puVar2 = FUN_004054e0(&param_4,(int)uVar8,cVar9,cVar10);
    local_4._0_1_ = 4;
    FUN_00403b00(&local_20,puVar2);
    local_4._0_1_ = 1;
    piVar3 = local_20;
    if (param_4 != &DAT_00501908) {
      *param_4 = *param_4 + -1;
      iVar1 = *param_4;
      goto LAB_00487007;
    }
  }
  if (*(int *)((int)this + 0x1c) != 0) {
    FUN_00401d00(&local_20," / ");
    if (*(int *)((int)this + 4) == 0) {
      piVar3 = FUN_00406880(&param_4);
      local_4._0_1_ = 5;
      FUN_00401d00(&local_20,(char *)(*piVar3 + 0xc));
      local_4._0_1_ = 1;
      piVar3 = local_20;
      if (param_4 == &DAT_00501908) goto LAB_00487119;
      *param_4 = *param_4 + -1;
      bVar7 = *param_4 < 0;
      bVar6 = *param_4 == 0;
    }
    else if (*(int *)((int)this + 4) == 2) {
      ConvertX87FloatToIntegerTowardZero();
      piVar3 = FUN_00406880(&param_4);
      local_4._0_1_ = 6;
      FUN_00401d00(&local_20,(char *)(*piVar3 + 0xc));
      local_4._0_1_ = 1;
      piVar3 = local_20;
      if (param_4 == &DAT_00501908) goto LAB_00487119;
      *param_4 = *param_4 + -1;
      bVar7 = *param_4 < 0;
      bVar6 = *param_4 == 0;
    }
    else {
      cVar10 = '\x01';
      cVar9 = '\x01';
      uVar8 = ConvertX87FloatToIntegerTowardZero();
      piVar3 = FUN_004054e0(&param_4,(int)uVar8,cVar9,cVar10);
      local_4._0_1_ = 7;
      FUN_00401d00(&local_20,(char *)(*piVar3 + 0xc));
      local_4._0_1_ = 1;
      piVar3 = local_20;
      if (param_4 == &DAT_00501908) goto LAB_00487119;
      *param_4 = *param_4 + -1;
      bVar7 = *param_4 < 0;
      bVar6 = *param_4 == 0;
    }
    local_4._0_1_ = 1;
    piVar3 = local_20;
    if (bVar6 || bVar7) {
      FUN_004011c0(&param_4);
      piVar3 = local_20;
    }
  }
LAB_00487119:
  if ((char)param_2 != '\0') {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_14 = &DAT_00501908;
    FUN_00401d80(&local_14," ");
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    param_2 = &DAT_00501908;
    local_4._0_1_ = 9;
    pvVar4 = AppendCStringToString((int *)((int)this + 0x10),&local_18,":");
    local_4._0_1_ = 10;
    piVar5 = FUN_004329b0(&param_2,(int *)&local_14,
                          ((param_3 - piVar3[1]) - *(int *)(*(int *)((int)this + 0x10) + 4)) + -1);
    pvVar4 = AppendStringObject(pvVar4,&local_1c,piVar5);
    local_4._0_1_ = 0xb;
    puVar2 = AppendStringObject(pvVar4,&param_4,(int *)&local_20);
    local_4._0_1_ = 0xc;
    if (piVar3 != (int *)*puVar2) {
      if ((piVar3 != &DAT_00501908) && (iVar1 = *piVar3, *piVar3 = iVar1 + -1, iVar1 + -1 < 1)) {
        FUN_004011c0(&local_20);
      }
      piVar3 = (int *)*puVar2;
      local_20 = piVar3;
      if (piVar3 != &DAT_00501908) {
        *piVar3 = *piVar3 + 1;
      }
    }
    local_4._0_1_ = 0xb;
    if (param_4 != &DAT_00501908) {
      *param_4 = *param_4 + -1;
      if (*param_4 < 1) {
        FUN_004011c0(&param_4);
      }
      else {
        param_4 = &DAT_00501908;
      }
    }
    local_4._0_1_ = 10;
    if (local_1c != &DAT_00501908) {
      *local_1c = *local_1c + -1;
      if (*local_1c < 1) {
        FUN_004011c0(&local_1c);
      }
      else {
        local_1c = &DAT_00501908;
      }
    }
    local_4._0_1_ = 9;
    if (local_18 != &DAT_00501908) {
      *local_18 = *local_18 + -1;
      if (*local_18 < 1) {
        FUN_004011c0(&local_18);
      }
      else {
        local_18 = &DAT_00501908;
      }
    }
    local_4._0_1_ = 8;
    if (param_2 != &DAT_00501908) {
      *param_2 = *param_2 + -1;
      if (*param_2 < 1) {
        FUN_004011c0(&param_2);
      }
      else {
        param_2 = &DAT_00501908;
      }
    }
    local_4._0_1_ = 1;
    if ((local_14 != &DAT_00501908) && (iVar1 = *local_14, *local_14 = iVar1 + -1, iVar1 + -1 < 1))
    {
      FUN_004011c0(&local_14);
    }
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  *param_1 = &DAT_00501908;
  if (piVar3 != &DAT_00501908) {
    *param_1 = piVar3;
    *piVar3 = *piVar3 + 1;
  }
  local_10 = 1;
  local_4 = (uint)local_4._1_3_ << 8;
  if ((piVar3 != &DAT_00501908) && (iVar1 = *piVar3, *piVar3 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&local_20);
  }
  ExceptionList = local_c;
  return param_1;
}

