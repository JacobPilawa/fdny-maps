
/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall InitializeBoundsTriggerEntity(void *param_1)

{
  char *pcVar1;
  undefined1 *puVar2;
  undefined4 *puVar3;
  void *this;
  undefined4 uVar4;
  int iVar5;
  int *extraout_ECX;
  float10 fVar6;
  int *piVar7;
  int *piVar8;
  int iVar9;
  undefined1 *puVar10;
  int *local_70;
  undefined1 *local_6c [2];
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cef98;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar3 = (undefined4 *)FUN_0049d350(param_1,local_6c);
  local_70 = (int *)&stack0xffffff78;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar7 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff78,*(char **)(*(int *)((int)param_1 + 0xfc) + 4));
  FUN_0046e320((void *)((int)param_1 + 0x24),piVar7,puVar3);
  iVar9 = *(int *)(*(int *)((int)param_1 + 0xfc) + 8);
  if (iVar9 == 0) {
    *(undefined4 *)((int)param_1 + 0xe4) = *(undefined4 *)((int)param_1 + 200);
    *(undefined4 *)((int)param_1 + 0xe8) = *(undefined4 *)((int)param_1 + 0xcc);
    *(undefined4 *)((int)param_1 + 0xec) = *(undefined4 *)((int)param_1 + 0xd0);
    *(undefined4 *)((int)param_1 + 0xf0) = *(undefined4 *)((int)param_1 + 200);
    *(undefined4 *)((int)param_1 + 0xf4) = *(undefined4 *)((int)param_1 + 0xcc);
    *(undefined4 *)((int)param_1 + 0xf8) = *(undefined4 *)((int)param_1 + 0xd0);
  }
  else {
    iVar5 = *(int *)(DAT_00503a78 + 4);
    if (*(char *)(iVar5 + 0x160) == '\0') {
      iVar5 = iVar5 + 8;
    }
    else {
      iVar5 = iVar5 + 0xb4;
    }
    FUN_004818c0((float *)((int)param_1 + 0xe4),*(undefined4 *)(iVar5 + 0x78),iVar9);
    geWorld_ModelSetFlags();
    fVar6 = FUN_004818a0((float *)((int)param_1 + 0xe4));
    if (fVar6 < (float10)_DAT_004dfa28) {
      iVar9 = 0;
      piVar7 = extraout_ECX;
      local_70 = (int *)&stack0xffffff78;
      FUN_00401cb0(&stack0xffffff78,(undefined4 *)((int)param_1 + 0x48));
      local_4 = 0;
      local_64 = &PTR_FUN_004dc554;
      FUN_004a3118();
      local_60 = "sharedents\\BoundsTriggerEnt.cpp";
      local_5c = 0x2a;
      local_58 = 0;
      local_4 = CONCAT31(local_4._1_3_,2);
      FUN_004a3a19(local_48,"TE_BoundsTrigger: Model with zero volume: \"");
      this = (void *)FUN_0040f020(&local_64,piVar7);
      FUN_004a3a19((void *)((int)this + 0x1c),"\".");
      FUN_00401360(this,iVar9);
      local_4 = 0xffffffff;
      FUN_004012c0(&local_64);
    }
  }
  *(undefined4 *)((int)param_1 + 0x160) = *(undefined4 *)(*(int *)((int)param_1 + 0xfc) + 0xc);
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x10);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 3;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x14);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 4;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x18);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 5;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x1c);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 6;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x20);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 7;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x24);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 8;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x28);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 9;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x2c);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 10;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x30);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 0xb;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if ((piVar7 != &DAT_00501908) && (iVar9 = *piVar7, *piVar7 = iVar9 + -1, iVar9 + -1 < 1)) {
      FUN_004011c0(&local_70);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xfc) + 0x34);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_70,pcVar1);
    piVar7 = local_70;
    local_4 = 0xc;
    if (local_70[1] != 0) {
      puVar10 = (undefined1 *)0x0;
      local_6c[0] = &stack0xffffff78;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      puVar2 = &stack0xffffff78;
      if (local_70 != &DAT_00501908) {
        piVar8 = local_70;
        *local_70 = *local_70 + 1;
        puVar2 = local_6c[0];
      }
      local_6c[0] = puVar2;
      FUN_00475310((void *)((int)param_1 + 0x24),piVar8,puVar10);
    }
    local_4 = 0xffffffff;
    if (piVar7 != &DAT_00501908) {
      iVar9 = *piVar7;
      *piVar7 = iVar9 + -1;
      if (iVar9 + -1 < 1) {
        FUN_004011c0(&local_70);
      }
    }
  }
  local_6c[0] = &stack0xffffff7c;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar7 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff7c,"tell player escape");
  local_70 = (int *)&stack0xffffff78;
  local_4 = 0xd;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar8 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff78,"touch");
  local_4 = 0xffffffff;
  uVar4 = FUN_0046ed70((void *)((int)param_1 + 0x24),piVar8,piVar7);
  *(undefined1 *)((int)param_1 + 0x165) = 0;
  *(char *)((int)param_1 + 0x164) = (char)uVar4;
  ExceptionList = local_c;
  return;
}

