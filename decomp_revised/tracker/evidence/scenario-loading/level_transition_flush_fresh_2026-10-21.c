/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0042a070 FUN_0042a070 */

void __fastcall FUN_0042a070(int param_1)

{
  int iVar1;
  undefined1 auStack_a8 [68];
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [52];
  void *pvStack_14;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd6e3;
  pvStack_c = ExceptionList;
  local_64 = &PTR_FUN_004dc554;
  ExceptionList = &pvStack_c;
  FUN_004a3118();
  local_60 = "main\\evergame.cpp";
  local_5c = 0xcb;
  local_58 = 0;
  local_4 = 0;
  FUN_004a3a19(local_48,"TEverGame: Flushing game...");
  FUN_00401360(&local_64,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  FUN_004a03e0();
  FUN_004a08f0(*(void **)(param_1 + 0x3b0),1);
  FUN_004a08f0(*(void **)(param_1 + 0x3b0),2);
  (**(code **)(**(int **)(param_1 + 0x3c0) + 8))();
  (**(code **)(**(int **)(param_1 + 0x3bc) + 8))();
  FUN_00429ef0((void *)(param_1 + 0xc),*(int **)(param_1 + 0x3c4));
  FUN_00429ef0((void *)(param_1 + 0x54),*(int **)(param_1 + 0x3c4));
  FUN_00429ef0((void *)(param_1 + 0x9c),*(int **)(param_1 + 0x3c4));
  FUN_00429ef0((int *)(param_1 + 0xe4),*(int **)(param_1 + 0x3c4));
  (**(code **)(*(int *)(param_1 + 0xe4) + 8))();
  (**(code **)(*(int *)(*(int *)(param_1 + 0x3c4) + 0x40) + 4))(0,0);
  FUN_0049f040(*(int *)(DAT_00503a78 + 0x14));
  FUN_004944e0(*(int *)(DAT_00503a78 + 4));
  FUN_00470ed0(&DAT_00502fb0);
  *(undefined4 *)(param_1 + 0x688) = 0;
  FUN_004a3118();
  pvStack_c = (void *)0x1;
  iVar1 = *(int *)(*(int *)(DAT_00503a78 + 0x10) + 0x101c);
  FUN_004a3a19(auStack_a8,"Available Entity handles: ");
  FUN_004a3f7a(auStack_a8,iVar1);
  FUN_00401360(&stack0xffffff3c,0);
  pvStack_c = (void *)0xffffffff;
  FUN_004012c0((undefined4 *)&stack0xffffff3c);
  ExceptionList = pvStack_14;
  return;
}



/* 0x00429ef0 FUN_00429ef0 */

void __thiscall FUN_00429ef0(void *this,int *param_1)

{
  int *piVar1;
  int iVar2;
  int iVar3;
  
  *(undefined4 *)((int)this + 4) = *(undefined4 *)((int)this + 0x10);
  *(undefined4 *)((int)this + 8) = 0;
  *(undefined1 *)((int)this + 0xc) = 0;
  *(undefined1 *)((int)this + 0xd) = 1;
LAB_00429f08:
  do {
    piVar1 = *(int **)((int)this + 0x10);
    if (piVar1 == (int *)0x0) {
      return;
    }
    iVar2 = piVar1[1];
    iVar3 = *piVar1;
    *(int *)((int)this + 0x10) = iVar2;
    if (iVar2 == 0) {
      *(undefined4 *)((int)this + 0x14) = 0;
    }
    FUN_00429d20(this,(int)piVar1);
    _free(piVar1);
    *(int *)((int)this + 0x18) = *(int *)((int)this + 0x18) + -1;
    if (iVar3 == 0) {
      return;
    }
    for (piVar1 = (int *)param_1[4]; piVar1 != (int *)0x0; piVar1 = (int *)piVar1[1]) {
      if (*piVar1 == iVar3) goto LAB_00429f08;
    }
    (**(code **)(*param_1 + 4))(iVar3,1);
  } while( true );
}



/* 0x004a03e0 FUN_004a03e0 */

void FUN_004a03e0(void)

{
  int iVar1;
  uint uVar2;
  int iVar3;
  int *piVar4;
  int iStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d4958;
  pvStack_c = ExceptionList;
  iVar3 = 0;
  ExceptionList = &pvStack_c;
  FUN_0049ffb0();
  iVar1 = _FSOUND_GetMaxChannels_0();
  if (0 < iVar1) {
    do {
      piVar4 = &iStack_10;
      iVar1 = iVar3;
      FUN_0049ffb0();
      FUN_0049f400(piVar4,iVar1);
      uStack_4 = 0;
      uVar2 = FUN_0049f280(&iStack_10);
      if ((char)uVar2 != '\0') {
        FUN_0049f340(&iStack_10);
      }
      uStack_4 = 0xffffffff;
      FUN_0043f660();
      iVar3 = iVar3 + 1;
      FUN_0049ffb0();
      iVar1 = _FSOUND_GetMaxChannels_0();
    } while (iVar3 < iVar1);
  }
  ExceptionList = pvStack_c;
  return;
}



/* 0x004a08f0 FUN_004a08f0 */

void __thiscall FUN_004a08f0(void *this,int param_1)

{
  void **ppvVar1;
  int iVar2;
  int iVar3;
  int *piVar4;
  int local_b4;
  int local_b0;
  int local_ac;
  char *local_a8;
  char *local_a4;
  undefined4 local_a0;
  int local_94;
  char *local_90;
  char *local_8c;
  undefined4 local_88;
  int local_7c;
  char *local_78;
  char *local_74;
  undefined4 local_70;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d4978;
  local_c = ExceptionList;
  iVar2 = *(int *)((int)this + 4);
  iVar3 = 0;
  local_b4 = 0;
  local_b0 = 0;
  ExceptionList = &local_c;
  ppvVar1 = &local_c;
  if (0 < iVar2) {
    do {
      if ((iVar3 < 0) || (iVar2 <= iVar3)) {
        piVar4 = &local_ac;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_ac = 1;
        local_a8 = "Array index out of bounds";
        local_a4 = "..\\BluelAPI/Container/SArray.h";
        local_a0 = 0x1cd;
        FUN_00422060(&local_ac);
      }
      if (*(int *)(*(int *)this + iVar3 * 4) != 0) {
        if ((iVar3 < 0) || (*(int *)((int)this + 4) <= iVar3)) {
          piVar4 = &local_7c;
          for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
            *piVar4 = 0;
            piVar4 = piVar4 + 1;
          }
          local_7c = 1;
          local_78 = "Array index out of bounds";
          local_74 = "..\\BluelAPI/Container/SArray.h";
          local_70 = 0x1cd;
          FUN_00422060(&local_7c);
        }
        if (*(int *)(*(int *)(*(int *)this + iVar3 * 4) + 0x14) == param_1) {
          if ((iVar3 < 0) || (*(int *)((int)this + 4) <= iVar3)) {
            piVar4 = &local_94;
            for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
              *piVar4 = 0;
              piVar4 = piVar4 + 1;
            }
            local_94 = 1;
            local_90 = "Array index out of bounds";
            local_8c = "..\\BluelAPI/Container/SArray.h";
            local_88 = 0x1cd;
            FUN_00422060(&local_94);
          }
          local_b4 = local_b4 + 1;
          *(undefined4 *)(*(int *)this + iVar3 * 4) = 0;
        }
        else {
          local_b0 = local_b0 + 1;
        }
      }
      iVar2 = *(int *)((int)this + 4);
      iVar3 = iVar3 + 1;
      ppvVar1 = ExceptionList;
    } while (iVar3 < iVar2);
  }
  ExceptionList = ppvVar1;
  local_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  local_60 = "sound\\SoundSystem.cpp";
  local_5c = 0xa7;
  local_58 = 0;
  local_4 = 0;
  FUN_004a3a19(local_48,"ROCSoundSystem::DetachSoundWraps(): Detached ");
  FUN_004a3f7a(local_48,local_b4);
  FUN_004a3a19(local_48," (type ");
  FUN_004a3f7a(local_48,param_1);
  FUN_004a3a19(local_48,") soundwraps, ");
  FUN_004a3f7a(local_48,local_b0);
  FUN_004a3a19(local_48," remaining.");
  FUN_00401360(&local_64,3);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  ExceptionList = local_c;
  return;
}



/* 0x0049f040 FUN_0049f040 */

void __fastcall FUN_0049f040(int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  
  *(undefined4 *)(param_1 + 0xc) = *(undefined4 *)(param_1 + 0x18);
  *(undefined4 *)(param_1 + 0x10) = 0;
  *(undefined1 *)(param_1 + 0x14) = 0;
  *(undefined1 *)(param_1 + 0x15) = 1;
  while( true ) {
    puVar1 = *(undefined4 **)(param_1 + 0xc);
    if (puVar1 == (undefined4 *)0x0) {
      return;
    }
    if (*(char *)(param_1 + 0x15) == '\0') {
      if (*(char *)(param_1 + 0x14) == '\0') {
        *(undefined4 **)(param_1 + 0x10) = puVar1;
        puVar1 = *(undefined4 **)(*(int *)(param_1 + 0xc) + 4);
        *(undefined4 **)(param_1 + 0xc) = puVar1;
        if (puVar1 == (undefined4 *)0x0) {
          return;
        }
        piVar2 = (int *)*puVar1;
      }
      else {
        *(undefined1 *)(param_1 + 0x14) = 0;
        piVar2 = (int *)**(undefined4 **)(param_1 + 0xc);
      }
    }
    else {
      *(undefined1 *)(param_1 + 0x15) = 0;
      piVar2 = (int *)*puVar1;
    }
    if (piVar2 == (int *)0x0) break;
    (**(code **)(*piVar2 + 0xc))();
  }
  return;
}



/* 0x004944e0 FUN_004944e0 */

void __fastcall FUN_004944e0(int param_1)

{
  FUN_00493ea0((void *)(param_1 + 8));
  *(undefined4 *)(param_1 + 0x170) = 0;
  return;
}



/* 0x00470ed0 FUN_00470ed0 */

void __fastcall FUN_00470ed0(void *param_1)

{
  if (*(int *)((int)param_1 + 0x80) != 0) {
    FUN_0043b6e0((void *)((int)param_1 + 0x7c),0);
  }
  thunk_FUN_0043ee40((undefined4 *)((int)param_1 + 0x94));
  FUN_00470c40((void *)((int)param_1 + 0xb0));
  ScriptEngineAddActor(param_1,(int)param_1);
  return;
}



/* 0x00471030 FUN_00471030 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_00471030(void)

{
  int iVar1;
  int *piVar2;
  int *local_14;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d2660;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_004443e0(&local_10);
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"DIFFICULTY");
  local_4._0_1_ = 1;
  StoreScriptVariableValue(&local_14,&local_10);
  local_4 = (uint)local_4._1_3_ << 8;
  if ((local_14 != &DAT_00501908) && (iVar1 = *local_14, *local_14 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  local_4 = 0xffffffff;
  if ((local_10 != &DAT_00501908) && (*local_10 = *local_10 + -1, *local_10 < 1)) {
    FUN_004011c0(&local_10);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  local_4 = 2;
  if ((*(undefined4 **)(DAT_00502aac + 0x688) != (undefined4 *)0x0) &&
     (piVar2 = (int *)**(undefined4 **)(DAT_00502aac + 0x688), piVar2 != &DAT_00501908)) {
    *piVar2 = *piVar2 + 1;
    local_14 = piVar2;
  }
  piVar2 = local_14;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_10 = &DAT_00501908;
  FUN_00401d80(&local_10,"LEVEL");
  local_4._0_1_ = 3;
  StoreScriptVariableValue(&local_10,&local_14);
  local_4 = CONCAT31(local_4._1_3_,2);
  if ((local_10 != &DAT_00501908) && (iVar1 = *local_10, *local_10 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&local_10);
  }
  local_4 = 0xffffffff;
  if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  ExceptionList = local_c;
  return;
}



