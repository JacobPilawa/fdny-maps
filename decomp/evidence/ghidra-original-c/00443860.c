/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00443860
 * Match: function entry
 * Entry: 00443860 FUN_00443860
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00443860(void *param_1)

{
  int iVar1;
  char *pcVar2;
  undefined1 *puVar3;
  bool bVar4;
  undefined4 *puVar5;
  int *extraout_ECX;
  int *extraout_ECX_00;
  int *extraout_ECX_01;
  int *extraout_ECX_02;
  int *extraout_ECX_03;
  int *extraout_ECX_04;
  int *extraout_ECX_05;
  int *extraout_ECX_06;
  int *extraout_ECX_07;
  int *piVar6;
  int *piVar7;
  undefined1 *puVar8;
  int *local_1c;
  undefined1 *local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf3d0;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  puVar5 = (undefined4 *)FUN_0049d350(param_1,local_18);
  local_1c = (int *)&stack0xffffffcc;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar6 = &DAT_00501908;
  FUN_00401d80(&stack0xffffffcc,*(char **)(*(int *)((int)param_1 + 0x1bc) + 0x10));
  FUN_0046e320((void *)((int)param_1 + 0xd4),piVar6,puVar5);
  *(undefined4 *)((int)param_1 + 0x1c0) = 0;
  iVar1 = *(int *)(*(int *)((int)param_1 + 0x1bc) + 0x14);
  if (-1 < iVar1) {
    FUN_00497e90(*(void **)((int)param_1 + 0xd0),*(void **)(DAT_00503a78 + 4),2,iVar1,0);
    FUN_0042d280((void *)(*(int *)((int)param_1 + 0xd0) + 0x34),local_18,0x3f800000,0x3f800000,
                 0x3f800000);
    *(int *)(*(int *)((int)param_1 + 0xd0) + 0x40) = (int)param_1 + 0x24;
    *(float *)(*(int *)((int)param_1 + 0x1bc) + 0x18) =
         -*(float *)(*(int *)((int)param_1 + 0x1bc) + 0x18) * (float)_DAT_004e1120 *
         (float)_DAT_004e1118 - (float)_DAT_004e12d8;
    *(float *)(*(int *)((int)param_1 + 0x1bc) + 0x1c) =
         *(float *)(*(int *)((int)param_1 + 0x1bc) + 0x1c) * _DAT_004e12d4;
    FUN_004968b0(*(void **)((int)param_1 + 0xd0),*(float *)(*(int *)((int)param_1 + 0x1bc) + 0x18));
    FUN_00496b50(*(void **)((int)param_1 + 0xd0),*(float *)(*(int *)((int)param_1 + 0x1bc) + 0x1c));
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x24);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar6 = local_1c;
    local_4 = 0;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar7 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar7,puVar8);
    }
    local_4 = 0xffffffff;
    if ((piVar6 != &DAT_00501908) && (iVar1 = *piVar6, *piVar6 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&local_1c);
    }
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x28);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar6 = local_1c;
    local_4 = 1;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar7 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar7,puVar8);
    }
    local_4 = 0xffffffff;
    if ((piVar6 != &DAT_00501908) && (iVar1 = *piVar6, *piVar6 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&local_1c);
    }
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x2c);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar6 = local_1c;
    local_4 = 2;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar7 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar7,puVar8);
    }
    local_4 = 0xffffffff;
    if ((piVar6 != &DAT_00501908) && (iVar1 = *piVar6, *piVar6 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&local_1c);
    }
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x30);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar6 = local_1c;
    local_4 = 3;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar7 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar7,puVar8);
    }
    local_4 = 0xffffffff;
    if ((piVar6 != &DAT_00501908) && (iVar1 = *piVar6, *piVar6 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&local_1c);
    }
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x34);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar6 = local_1c;
    local_4 = 4;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar7 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar7,puVar8);
    }
    local_4 = 0xffffffff;
    if ((piVar6 != &DAT_00501908) && (iVar1 = *piVar6, *piVar6 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&local_1c);
    }
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x38);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar6 = local_1c;
    local_4 = 5;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar7 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar7,puVar8);
    }
    local_4 = 0xffffffff;
    if ((piVar6 != &DAT_00501908) && (iVar1 = *piVar6, *piVar6 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&local_1c);
    }
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x3c);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar6 = local_1c;
    local_4 = 6;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar7 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar7,puVar8);
    }
    local_4 = 0xffffffff;
    if ((piVar6 != &DAT_00501908) && (iVar1 = *piVar6, *piVar6 = iVar1 + -1, iVar1 + -1 < 1)) {
      FUN_004011c0(&local_1c);
    }
  }
  piVar6 = *(int **)((int)param_1 + 0x1bc);
  if ((char *)piVar6[0x10] != (char *)0x0) {
    FUN_00402790(&local_1c,(char *)piVar6[0x10]);
    piVar7 = local_1c;
    local_4 = 7;
    piVar6 = extraout_ECX;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar6 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar6,puVar8);
      piVar6 = extraout_ECX_00;
    }
    local_4 = 0xffffffff;
    if (piVar7 != &DAT_00501908) {
      piVar6 = (int *)(*piVar7 + -1);
      *piVar7 = (int)piVar6;
      if ((int)piVar6 < 1) {
        FUN_004011c0(&local_1c);
        piVar6 = extraout_ECX_01;
      }
    }
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x44);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar7 = local_1c;
    local_4 = 8;
    piVar6 = extraout_ECX_02;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar6 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar6,puVar8);
      piVar6 = extraout_ECX_03;
    }
    local_4 = 0xffffffff;
    if (piVar7 != &DAT_00501908) {
      piVar6 = (int *)(*piVar7 + -1);
      *piVar7 = (int)piVar6;
      if ((int)piVar6 < 1) {
        FUN_004011c0(&local_1c);
        piVar6 = extraout_ECX_04;
      }
    }
  }
  pcVar2 = *(char **)(*(int *)((int)param_1 + 0x1bc) + 0x48);
  if (pcVar2 != (char *)0x0) {
    FUN_00402790(&local_1c,pcVar2);
    piVar7 = local_1c;
    local_4 = 9;
    piVar6 = extraout_ECX_05;
    if (local_1c[1] != 0) {
      puVar8 = (undefined1 *)0x0;
      local_18[0] = &stack0xffffffcc;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      puVar3 = &stack0xffffffcc;
      if (local_1c != &DAT_00501908) {
        piVar6 = local_1c;
        *local_1c = *local_1c + 1;
        puVar3 = local_18[0];
      }
      local_18[0] = puVar3;
      FUN_00475310((void *)((int)param_1 + 0xd4),piVar6,puVar8);
      piVar6 = extraout_ECX_06;
    }
    local_4 = 0xffffffff;
    if (piVar7 != &DAT_00501908) {
      piVar6 = (int *)(*piVar7 + -1);
      *piVar7 = (int)piVar6;
      if ((int)piVar6 < 1) {
        FUN_004011c0(&local_1c);
        piVar6 = extraout_ECX_07;
      }
    }
  }
  if (*(int *)((int)param_1 + 0x1c0) == 0) {
    local_18[0] = &stack0xffffffd0;
    FUN_00402790(&stack0xffffffd0,"use");
    bVar4 = FUN_0046e3d0((void *)((int)param_1 + 0xd4),piVar6);
    if (bVar4) {
      *(undefined4 *)((int)param_1 + 0x1c0) = 2;
    }
  }
  *(undefined4 *)((int)param_1 + 0x1e8) = *(undefined4 *)(*(int *)((int)param_1 + 0x1bc) + 0x20);
  *(undefined4 *)((int)param_1 + 0x1dc) = *(undefined4 *)((int)param_1 + 0x4c);
  *(undefined4 *)((int)param_1 + 0x1e0) = *(undefined4 *)((int)param_1 + 0x50);
  *(undefined4 *)((int)param_1 + 0x1e4) = *(undefined4 *)((int)param_1 + 0x54);
  *(undefined1 *)((int)param_1 + 0x1b8) = 0;
  ExceptionList = local_c;
  return;
}


