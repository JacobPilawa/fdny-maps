/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00489200 FUN_00489200 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00489200(void *this,int *param_1)

{
  undefined4 *puVar1;
  bool bVar2;
  int *piVar3;
  void *this_00;
  void *pvVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  int iVar9;
  int *local_74;
  int *local_70;
  undefined4 *local_6c;
  int *local_68;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  uint local_4;
  
  piVar7 = param_1;
  puStack_8 = &LAB_004d3b0b;
  local_c = ExceptionList;
  puVar1 = (undefined4 *)((int)this + 0x1c);
  ExceptionList = &local_c;
  *(undefined4 *)((int)this + 0x18) = 0;
  piVar3 = (int *)*puVar1;
  local_4 = 0;
  local_68 = this;
  if (piVar3 != param_1) {
    if (piVar3 != &DAT_00501908) {
      *piVar3 = *piVar3 + -1;
      if (*(int *)*puVar1 < 1) {
        FUN_004011c0(puVar1);
      }
      else {
        *puVar1 = &DAT_00501908;
      }
    }
    *puVar1 = piVar7;
    if (piVar7 != &DAT_00501908) {
      *piVar7 = *piVar7 + 1;
    }
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_70 = &DAT_00501908;
  local_4 = CONCAT31(local_4._1_3_,1);
  iVar5 = 0;
  FUN_00402770(&local_70,&DAT_004e3888);
  piVar7 = local_70;
  piVar3 = local_70 + 3;
  bVar2 = FUN_004a0e40((char *)piVar3);
  if (bVar2) {
    do {
      local_6c = operator_new(0x24);
      local_4._0_1_ = 2;
      if (local_6c == (undefined4 *)0x0) {
        local_6c = (undefined4 *)0x0;
      }
      else {
        local_6c = FUN_004a13a0(local_6c);
      }
      local_4._0_1_ = 1;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      local_74 = &DAT_00501908;
      FUN_00401d80(&local_74,(char *)piVar3);
      local_4._0_1_ = 3;
      FUN_004a12f0(local_6c,&local_74,0,1);
      local_4 = CONCAT31(local_4._1_3_,1);
      if ((local_74 != &DAT_00501908) && (*local_74 = *local_74 + -1, *local_74 < 1)) {
        FUN_004011c0(&local_74);
      }
      iVar6 = 0;
      if (0 < local_68[1]) {
        piVar3 = (int *)*local_68;
        do {
          if (*piVar3 == 0) goto LAB_004893c0;
          iVar6 = iVar6 + 1;
          piVar3 = piVar3 + 1;
        } while (iVar6 < local_68[1]);
      }
      FUN_0043b6e0(local_68,iVar6 + 1);
LAB_004893c0:
      iVar9 = 0;
      *(undefined4 **)(*local_68 + iVar6 * 4) = local_6c;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar3 = &DAT_00501908;
      if (piVar7 != &DAT_00501908) {
        *piVar7 = *piVar7 + 1;
        piVar3 = piVar7;
      }
      local_74 = (int *)&stack0xffffff70;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = (int *)local_68[7];
      piVar8 = &DAT_00501908;
      if (piVar7 != &DAT_00501908) {
        *piVar7 = *piVar7 + 1;
        piVar8 = piVar7;
      }
      local_4._0_1_ = 5;
      local_64 = &PTR_FUN_004dc554;
      local_6c = (undefined4 *)&stack0xffffff74;
      FUN_004a3118();
      local_60 = "basic\\RadioChatter.cpp";
      local_5c = 0x90;
      local_58 = 0;
      local_4._0_1_ = 7;
      FUN_004a3a19(local_48,"[TChatterSet] ");
      this_00 = FUN_0040f020(&local_64,piVar8);
      pvVar4 = (void *)((int)this_00 + 0x1c);
      local_4._0_1_ = 8;
      FUN_004a3a19(pvVar4," created snd# ");
      FUN_004a3f7a(pvVar4,iVar5);
      FUN_004a3a19(pvVar4," using: ");
      pvVar4 = FUN_0040f020(this_00,piVar3);
      FUN_00401360(pvVar4,iVar9);
      local_4 = CONCAT31(local_4._1_3_,1);
      FUN_004012c0(&local_64);
      iVar5 = iVar5 + 1;
      FUN_00402770(&local_70,&DAT_004e3888);
      piVar7 = local_70;
      piVar3 = local_70 + 3;
      bVar2 = FUN_004a0e40((char *)piVar3);
    } while (bVar2);
  }
  local_4 = local_4 & 0xffffff00;
  if ((piVar7 != &DAT_00501908) && (iVar5 = *piVar7, *piVar7 = iVar5 + -1, iVar5 + -1 < 1)) {
    FUN_004011c0(&local_70);
  }
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (iVar5 = *param_1, *param_1 = iVar5 + -1, iVar5 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return;
}



/* 0x004895c0 FUN_004895c0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004895c0(int *param_1)

{
  undefined4 *this;
  char *pcVar1;
  int iVar2;
  int *piVar3;
  undefined ***local_6c;
  undefined **local_64;
  char *local_60;
  char *local_5c;
  char *local_58;
  char *local_54;
  char *local_50;
  char *local_4c;
  char *local_48;
  char *local_44;
  char *local_40;
  char *local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3b33;
  local_c = ExceptionList;
  pcVar1 = "RC_OxygenLow";
  local_6c = &local_64;
  local_64 = (undefined **)0x4e2170;
  local_60 = "RC_VictPick";
  local_5c = "RC_VictDrop";
  local_58 = "RC_PassOut";
  local_54 = "RC_ResLvlStart";
  local_50 = "RC_ResLvlWin";
  local_4c = "RC_ResLvlLose";
  local_48 = "RC_TimeWarn";
  local_44 = "RC_HoseLvlStart";
  local_40 = "RC_HoseLvlLose";
  local_3c = "RC_HoseLvlWin";
  local_38 = "RC_Pain";
  local_34 = "RC_ResTimeWarn";
  local_30 = 0;
  ExceptionList = &local_c;
  do {
    this = operator_new(0x20);
    if (this == (undefined4 *)0x0) {
      this = (undefined4 *)0x0;
    }
    else {
      this[5] = 0x10;
      this[4] = FUN_0043f660;
      *this = 0;
      this[1] = 0;
      this[2] = 0;
      this[3] = 0;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      this[7] = &DAT_00501908;
    }
    local_4 = 0xffffffff;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar3 = &DAT_00501908;
    FUN_00401d80(&stack0xffffff80,pcVar1);
    FUN_00489200(this,piVar3);
    iVar2 = 0;
    if (0 < param_1[1]) {
      piVar3 = (int *)*param_1;
      do {
        if (*piVar3 == 0) goto LAB_0048972e;
        iVar2 = iVar2 + 1;
        piVar3 = piVar3 + 1;
      } while (iVar2 < param_1[1]);
    }
    FUN_0043b6e0(param_1,iVar2 + 1);
LAB_0048972e:
    local_6c = local_6c + 1;
    *(undefined4 **)(*param_1 + iVar2 * 4) = this;
    pcVar1 = (char *)*local_6c;
    if ((undefined **)pcVar1 == (undefined **)0x0) {
      local_64 = &PTR_FUN_004dc554;
      FUN_004a3118();
      local_60 = "basic\\RadioChatter.cpp";
      local_5c = (char *)0x24;
      local_58 = (char *)((uint)local_58 & 0xffffff00);
      local_4 = 1;
      FUN_004a3a19(&local_48,"[TRadioChatter] Chatter Box Initialized");
      FUN_00401360(&local_64,0);
      local_4 = 0xffffffff;
      FUN_004012c0(&local_64);
      param_1[7] = 0;
      ExceptionList = local_c;
      return;
    }
  } while( true );
}



/* 0x004897c0 FUN_004897c0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_004897c0(void *this,int *param_1,char param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar2 = param_1;
  puStack_8 = &LAB_004d3a78;
  local_c = ExceptionList;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  ExceptionList = &local_c;
  piVar3 = &DAT_00501908;
  if (param_1 != &DAT_00501908) {
    ExceptionList = &local_c;
    *param_1 = *param_1 + 1;
    piVar3 = param_1;
  }
  piVar3 = (int *)FUN_00488e30(this,piVar3);
  if (piVar3 != (int *)0x0) {
    if (param_2 == '\0') {
      FUN_00488d40(piVar3);
    }
    else {
      FUN_00489110(piVar3);
    }
    *(int **)((int)this + 0x18) = piVar3;
    *(undefined1 *)((int)this + 0x20) = 1;
  }
  local_4 = 0xffffffff;
  if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return;
}



/* 0x00488e30 FUN_00488e30 */

undefined4 __thiscall FUN_00488e30(void *this,int *param_1)

{
  undefined4 uVar1;
  void **ppvVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d3a78;
  local_c = ExceptionList;
  iVar3 = *(int *)((int)this + 4);
  iVar4 = 0;
  local_4 = 0;
  ExceptionList = &local_c;
  ppvVar2 = &local_c;
  if (0 < iVar3) {
    do {
      ExceptionList = ppvVar2;
      if ((iVar4 < 0) || (iVar3 <= iVar4)) {
        piVar5 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar5 = 0;
          piVar5 = piVar5 + 1;
        }
        local_24 = 1;
        local_20 = "Array index out of bounds";
        local_1c = "..\\BluelAPI/Container/SArray.h";
        local_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      iVar3 = FUN_0040cb90((void *)(*(int *)(*(int *)this + iVar4 * 4) + 0x1c),(int *)&param_1);
      if ((char)iVar3 != '\0') {
        if ((iVar4 < 0) || (*(int *)((int)this + 4) <= iVar4)) {
          piVar5 = &local_24;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar5 = 0;
            piVar5 = piVar5 + 1;
          }
          local_24 = 1;
          local_20 = "Array index out of bounds";
          local_1c = "..\\BluelAPI/Container/SArray.h";
          local_18 = 0x1cd;
          FUN_00422060(&local_24);
        }
        uVar1 = *(undefined4 *)(*(int *)this + iVar4 * 4);
        local_4 = 0xffffffff;
        if ((param_1 != &DAT_00501908) && (iVar3 = *param_1, *param_1 = iVar3 + -1, iVar3 + -1 < 1))
        {
          FUN_004011c0(&param_1);
        }
        ExceptionList = local_c;
        return uVar1;
      }
      iVar3 = *(int *)((int)this + 4);
      iVar4 = iVar4 + 1;
      ppvVar2 = ExceptionList;
    } while (iVar4 < iVar3);
  }
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (iVar3 = *param_1, *param_1 = iVar3 + -1, iVar3 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return 0;
}



/* 0x00489110 FUN_00489110 */

void __fastcall FUN_00489110(int *param_1)

{
  int iVar1;
  void *this;
  int *extraout_ECX;
  int *piVar2;
  int iVar3;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3ac0;
  local_c = ExceptionList;
  iVar1 = param_1[6];
  ExceptionList = &local_c;
  param_1[6] = iVar1 + 1;
  if (param_1[1] <= iVar1 + 1) {
    param_1[6] = 0;
  }
  iVar1 = param_1[6];
  if (iVar1 < param_1[1]) {
    iVar3 = 0;
    FUN_0048d490(param_1,iVar1);
    piVar2 = extraout_ECX;
    FUN_00401cb0(&stack0xffffff88,(undefined4 *)(*(int *)(*param_1 + iVar1 * 4) + 0x20));
    local_4 = 0;
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "basic\\RadioChatter.cpp";
    local_5c = 0x77;
    local_58 = 0;
    local_4 = CONCAT31(local_4._1_3_,2);
    FUN_004a3a19(local_48,"[TChatterSet] Playing shuffled sound: ");
    this = FUN_0040f020(&local_64,piVar2);
    FUN_00401360(this,iVar3);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_64);
    iVar1 = param_1[6];
    FUN_0048d490(param_1,iVar1);
    FUN_004a11a0(*(void **)(*param_1 + iVar1 * 4),(undefined1 *)0x0);
  }
  ExceptionList = local_c;
  return;
}



/* 0x004a11a0 FUN_004a11a0 */

void __thiscall FUN_004a11a0(void *this,undefined1 *param_1)

{
  bool bVar1;
  
  if ((param_1 == (undefined1 *)0x1) && (bVar1 = FUN_0049f260(this), bVar1)) {
    return;
  }
  FUN_004a0be0(*(void **)(DAT_00503a78 + 8),this,param_1);
  return;
}



