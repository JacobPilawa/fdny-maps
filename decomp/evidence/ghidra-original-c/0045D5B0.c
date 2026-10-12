/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045d5b0
 * Match: function entry
 * Entry: 0045d5b0 FUN_0045d5b0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0045d5b0(int *param_1)

{
  undefined4 uVar1;
  int iVar2;
  int *piVar3;
  int iVar4;
  ulonglong uVar5;
  int *piVar6;
  undefined1 local_7d;
  int *local_7c [2];
  undefined4 local_74 [2];
  undefined1 *local_6c;
  undefined4 local_68;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d108e;
  local_c = ExceptionList;
  local_7c[0] = (int *)param_1[0x81];
  ExceptionList = &local_c;
  FUN_00498b30(0.0,(float)(int)local_7c[0] - (float)_DAT_004dfc78);
  uVar5 = FUN_004b9400();
  iVar2 = (int)uVar5;
  local_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  local_60 = "rescue\\VictimRescueEnt.cpp";
  local_5c = 0x8f;
  local_58 = 0;
  iVar4 = param_1[0x81];
  local_4 = 0;
  FUN_004a3a19(local_48,"TE_VictimRescue (");
  FUN_004a3a19(local_48,*(char **)(param_1[0x54] + 0x10));
  FUN_004a3a19(local_48,"): Spawning at spot# ");
  FUN_004a3f7a(local_48,iVar2 + 1);
  FUN_004a3a19(local_48," of ");
  FUN_004a3f7a(local_48,iVar4);
  FUN_004a3a19(local_48," available spots. ");
  FUN_00401360(&local_64,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  if ((-1 < iVar2) && (iVar2 < param_1[0x81])) {
    FUN_0042d350(param_1 + 9,&local_64);
    piVar3 = param_1 + 0x80;
    FUN_0048d490(piVar3,iVar2);
    piVar6 = *(int **)(*piVar3 + iVar2 * 4);
    param_1[0x13] = *piVar6;
    param_1[0x14] = piVar6[1];
    param_1[0x15] = piVar6[2];
    FUN_0048d490(piVar3,iVar2);
    FUN_00496c30(param_1 + 9,
                 *(float *)(*(int *)(*piVar3 + iVar2 * 4) + 0xc) * _DAT_004e1698 + _DAT_004e1cbc);
    FUN_0049da00(param_1);
  }
  uVar1 = *(undefined4 *)(*(int *)(param_1[0x34] + 0x94) + 0x60);
  if ((DAT_00502ccc & 1) == 0) {
    DAT_00502ccc = DAT_00502ccc | 1;
    DAT_00502cc0 = local_7d;
    local_4 = 1;
    DAT_00502cc4 = (int *)FUN_0045a5e0();
    *(undefined1 *)((int)DAT_00502cc4 + 0x15) = 1;
    DAT_00502cc4[1] = (int)DAT_00502cc4;
    *DAT_00502cc4 = (int)DAT_00502cc4;
    DAT_00502cc4[2] = (int)DAT_00502cc4;
    DAT_00502cc8 = 0;
    _atexit((_func_4879 *)&LAB_004db770);
    local_4 = 0xffffffff;
  }
  FUN_0045aa10(&DAT_00502cc0,(int *)local_7c,param_1 + 0x5a);
  iVar4 = 0;
  if (local_7c[0] != DAT_00502cc4) {
    iVar4 = local_7c[0][4];
  }
  iVar2 = geActor_GetMaterialCount(uVar1);
  if (iVar4 < iVar2) {
    if (iVar4 != 0) {
      geActor_GetMaterial(uVar1,iVar4,&local_6c,local_74);
      geActor_SetMaterial(uVar1,0,local_6c,local_74[0]);
    }
  }
  else {
    iVar4 = 0;
  }
  local_6c = (undefined1 *)param_1[0x5a];
  local_68 = 0;
  piVar3 = (int *)FUN_0045d450(&DAT_00502cc0,local_74,(int *)&local_6c);
  *(int *)(*piVar3 + 0x10) = iVar4 + 1;
  if (*(int *)(DAT_00503a78 + 0x30) != 0) {
    *(undefined4 *)(*(int *)(DAT_00503a78 + 0x30) + 0x3528) = 0xffffffff;
  }
  if (*(char **)(param_1[0x54] + 0x20) != (char *)0x0) {
    FUN_00402790(local_7c,*(char **)(param_1[0x54] + 0x20));
    piVar3 = local_7c[0];
    local_4 = 2;
    if (local_7c[0][1] != 0) {
      local_6c = &stack0xffffff68;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      if (local_7c[0] != &DAT_00501908) {
        *local_7c[0] = *local_7c[0] + 1;
        piVar6 = local_7c[0];
      }
      FUN_00475310(param_1 + 0x35,piVar6,(undefined1 *)0x0);
    }
    local_4 = 0xffffffff;
    if ((piVar3 != &DAT_00501908) && (iVar4 = *piVar3, *piVar3 = iVar4 + -1, iVar4 + -1 < 1)) {
      FUN_004011c0(local_7c);
    }
  }
  if (*(char **)(param_1[0x54] + 0x24) != (char *)0x0) {
    FUN_00402790(local_7c,*(char **)(param_1[0x54] + 0x24));
    piVar3 = local_7c[0];
    local_4 = 3;
    if (local_7c[0][1] != 0) {
      local_6c = &stack0xffffff68;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      if (local_7c[0] != &DAT_00501908) {
        *local_7c[0] = *local_7c[0] + 1;
        piVar6 = local_7c[0];
      }
      FUN_00475310(param_1 + 0x35,piVar6,(undefined1 *)0x0);
    }
    local_4 = 0xffffffff;
    if ((piVar3 != &DAT_00501908) && (iVar4 = *piVar3, *piVar3 = iVar4 + -1, iVar4 + -1 < 1)) {
      FUN_004011c0(local_7c);
    }
  }
  if (*(char **)(param_1[0x54] + 0x28) != (char *)0x0) {
    FUN_00402790(local_7c,*(char **)(param_1[0x54] + 0x28));
    piVar3 = local_7c[0];
    local_4 = 4;
    if (local_7c[0][1] != 0) {
      local_6c = &stack0xffffff68;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      if (local_7c[0] != &DAT_00501908) {
        *local_7c[0] = *local_7c[0] + 1;
        piVar6 = local_7c[0];
      }
      FUN_00475310(param_1 + 0x35,piVar6,(undefined1 *)0x0);
    }
    local_4 = 0xffffffff;
    if ((piVar3 != &DAT_00501908) && (iVar4 = *piVar3, *piVar3 = iVar4 + -1, iVar4 + -1 < 1)) {
      FUN_004011c0(local_7c);
    }
  }
  if (*(char **)(param_1[0x54] + 0x2c) != (char *)0x0) {
    FUN_00402790(local_7c,*(char **)(param_1[0x54] + 0x2c));
    piVar3 = local_7c[0];
    local_4 = 5;
    if (local_7c[0][1] != 0) {
      local_6c = &stack0xffffff68;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      if (local_7c[0] != &DAT_00501908) {
        *local_7c[0] = *local_7c[0] + 1;
        piVar6 = local_7c[0];
      }
      FUN_00475310(param_1 + 0x35,piVar6,(undefined1 *)0x0);
    }
    local_4 = 0xffffffff;
    if ((piVar3 != &DAT_00501908) && (iVar4 = *piVar3, *piVar3 = iVar4 + -1, iVar4 + -1 < 1)) {
      FUN_004011c0(local_7c);
    }
  }
  if (*(char **)(param_1[0x54] + 0x30) != (char *)0x0) {
    FUN_00402790(local_7c,*(char **)(param_1[0x54] + 0x30));
    piVar3 = local_7c[0];
    local_4 = 6;
    if (local_7c[0][1] != 0) {
      local_6c = &stack0xffffff68;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar6 = &DAT_00501908;
      if (local_7c[0] != &DAT_00501908) {
        *local_7c[0] = *local_7c[0] + 1;
        piVar6 = local_7c[0];
      }
      FUN_00475310(param_1 + 0x35,piVar6,(undefined1 *)0x0);
    }
    local_4 = 0xffffffff;
    if ((piVar3 != &DAT_00501908) && (iVar4 = *piVar3, *piVar3 = iVar4 + -1, iVar4 + -1 < 1)) {
      FUN_004011c0(local_7c);
    }
  }
  ExceptionList = local_c;
  return;
}


