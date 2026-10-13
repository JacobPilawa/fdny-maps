
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00442c90(void *param_1)

{
  void *this;
  char *pcVar1;
  int iVar2;
  undefined1 *puVar3;
  undefined4 *puVar4;
  int *piVar5;
  int *piVar6;
  undefined1 *puVar7;
  int *local_18;
  undefined1 *local_14 [2];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf290;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar4 = (undefined4 *)FUN_0049d350(param_1,local_14);
  local_18 = (int *)&stack0xffffffcc;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar5 = &DAT_00501908;
  FUN_00401d80(&stack0xffffffcc,*(char **)(*(int *)((int)param_1 + 0xe4) + 4));
  this = (void *)((int)param_1 + 0x68);
  FUN_0046e320(this,piVar5,puVar4);
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 8);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 0;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0xc);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 1;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0x10);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 2;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0x14);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 3;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0x18);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 4;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0x1c);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 5;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0x20);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 6;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0x24);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 7;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0x28);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 8;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  pcVar1 = *(char **)(*(int *)((int)param_1 + 0xe4) + 0x2c);
  if (pcVar1 != (char *)0x0) {
    FUN_00402790(&local_18,pcVar1);
    piVar5 = local_18;
    local_4 = 9;
    if (local_18[1] != 0) {
      puVar7 = (undefined1 *)0x0;
      local_14[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_18 != &DAT_00501908) {
        piVar6 = local_18;
        *local_18 = *local_18 + 1;
        puVar3 = local_14[0];
      }
      local_14[0] = puVar3;
      FUN_00475310(this,piVar6,puVar7);
    }
    local_4 = 0xffffffff;
    if ((piVar5 != &DAT_00501908) && (iVar2 = *piVar5, *piVar5 = iVar2 + -1, iVar2 + -1 < 1)) {
      FUN_004011c0(&local_18);
    }
  }
  ExceptionList = local_c;
  return;
}


