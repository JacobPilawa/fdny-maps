/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045b690
 * Match: function entry
 * Entry: 0045b690 FUN_0045b690
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0045b690(void *param_1)

{
  byte bVar1;
  float fVar2;
  undefined4 *puVar3;
  int iVar4;
  void *pvVar5;
  uint uVar6;
  byte *pbVar7;
  byte *pbVar8;
  bool bVar9;
  int *piVar10;
  undefined4 uVar11;
  int *local_78;
  undefined1 *local_74;
  undefined4 local_70 [3];
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0f38;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined4 *)((int)param_1 + 0x164) = 0;
  puVar3 = (undefined4 *)FUN_0049d350(param_1,local_70);
  local_74 = &stack0xffffff70;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar10 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff70,*(char **)(*(int *)((int)param_1 + 0x150) + 0x10));
  FUN_0046e320((void *)((int)param_1 + 0xd4),piVar10,puVar3);
  local_74 = &stack0xffffff74;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar10 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff74,*(char **)(*(int *)((int)param_1 + 0x150) + 0x14));
  FUN_0048df10(*(void **)(DAT_00502aac + 0x6e8),&local_78,piVar10);
  pbVar7 = *(byte **)(*(int *)((int)param_1 + 0x150) + 0x14);
  local_4 = 0;
  pbVar8 = (byte *)(local_78 + 3);
  do {
    bVar1 = *pbVar8;
    bVar9 = bVar1 < *pbVar7;
    if (bVar1 != *pbVar7) {
LAB_0045b7b9:
      iVar4 = (1 - (uint)bVar9) - (uint)(bVar9 != 0);
      goto LAB_0045b7be;
    }
    if (bVar1 == 0) break;
    bVar1 = pbVar8[1];
    bVar9 = bVar1 < pbVar7[1];
    if (bVar1 != pbVar7[1]) goto LAB_0045b7b9;
    pbVar8 = pbVar8 + 2;
    pbVar7 = pbVar7 + 2;
  } while (bVar1 != 0);
  iVar4 = 0;
LAB_0045b7be:
  if (iVar4 != 0) {
    iVar4 = 0;
    local_74 = &stack0xffffff70;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar10 = &DAT_00501908;
    if ((local_78 != &DAT_00501908) && (piVar10 = local_78, local_78 != &DAT_00501908)) {
      *local_78 = *local_78 + 1;
    }
    local_4._0_1_ = 1;
    local_4._1_3_ = 0;
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "rescue\\VictimRescueEnt.cpp";
    local_5c = 0x51;
    local_58 = 0;
    local_4._0_1_ = 3;
    FUN_004a3a19(local_48,"TE_VictimRescue (");
    FUN_004a3a19(local_48,*(char **)(*(int *)((int)param_1 + 0x150) + 0x10));
    FUN_004a3a19(local_48,"): ERROR! Victim Type \"");
    FUN_004a3a19(local_48,*(char **)(*(int *)((int)param_1 + 0x150) + 0x14));
    FUN_004a3a19(local_48,"\" does not exist. Using default type \"");
    pvVar5 = (void *)FUN_0040f020(&local_64,piVar10);
    FUN_004a3a19((void *)((int)pvVar5 + 0x1c),"\" instead.");
    FUN_00401360(pvVar5,iVar4);
    local_4 = (uint)local_4._1_3_ << 8;
    FUN_004012c0(&local_64);
  }
  local_74 = &stack0xffffff74;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar10 = &DAT_00501908;
  if ((local_78 != &DAT_00501908) && (piVar10 = local_78, local_78 != &DAT_00501908)) {
    *local_78 = *local_78 + 1;
  }
  iVar4 = FUN_0048de20(*(void **)(DAT_00502aac + 0x6e8),piVar10);
  *(int *)((int)param_1 + 0x168) = iVar4;
  if (iVar4 < 0) {
    iVar4 = 0;
    local_74 = &stack0xffffff70;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar10 = &DAT_00501908;
    if ((local_78 != &DAT_00501908) && (piVar10 = local_78, local_78 != &DAT_00501908)) {
      *local_78 = *local_78 + 1;
    }
    local_4._0_1_ = 4;
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "rescue\\VictimRescueEnt.cpp";
    local_5c = 0x56;
    local_58 = 0;
    local_4._0_1_ = 6;
    FUN_004a3a19(local_48,"TE_VictimRescue (");
    FUN_004a3a19(local_48,*(char **)(*(int *)((int)param_1 + 0x150) + 0x10));
    FUN_004a3a19(local_48,"): ERROR! Victim Type \"");
    pvVar5 = (void *)FUN_0040f020(&local_64,piVar10);
    FUN_004a3a19((void *)((int)pvVar5 + 0x1c),"\" does not exist. Victim will not be created.");
    FUN_00401360(pvVar5,iVar4);
    local_4 = (uint)local_4._1_3_ << 8;
    FUN_004012c0(&local_64);
    *(undefined1 *)((int)param_1 + 0x10) = 1;
  }
  else {
    if (iVar4 == 0xe) {
      uVar6 = FUN_00404b30(3);
      *(uint *)((int)param_1 + 0x168) = uVar6 + 0xb;
    }
    iVar4 = FUN_00493ce0(*(void **)(DAT_00503a78 + 4),*(int *)((int)param_1 + 0x168));
    local_64 = &PTR_FUN_004dc554;
    if (iVar4 == 0) {
      FUN_004a3118();
      local_60 = "rescue\\VictimRescueEnt.cpp";
      local_5c = 0x60;
      local_58 = 0;
      local_4._0_1_ = 7;
      FUN_004a3a19(local_48,"TE_VictimRescue (");
      FUN_004a3a19(local_48,*(char **)(*(int *)((int)param_1 + 0x150) + 0x10));
      FUN_004a3a19(local_48,"): ERROR! Model #");
      FUN_004a3f7a(local_48,*(int *)((int)param_1 + 0x168));
      FUN_004a3a19(local_48," does not exist for Victim Type \"");
      FUN_004a3a19(local_48,*(char **)(*(int *)((int)param_1 + 0x150) + 0x14));
      FUN_004a3a19(local_48,"\". Victim will not be created.");
      FUN_00401360(&local_64,0);
      local_4 = (uint)local_4._1_3_ << 8;
      FUN_004012c0(&local_64);
      *(undefined1 *)((int)param_1 + 0x10) = 1;
      if (local_78 == &DAT_00501908) {
        ExceptionList = local_c;
        return;
      }
      *local_78 = *local_78 + -1;
      iVar4 = *local_78;
      goto LAB_0045bdaa;
    }
    FUN_004a3118();
    local_60 = "rescue\\VictimRescueEnt.cpp";
    local_5c = 0x65;
    local_58 = 0;
    local_4._0_1_ = 8;
    FUN_004a3a19(local_48,"TE_VictimRescue (");
    FUN_004a3a19(local_48,*(char **)(*(int *)((int)param_1 + 0x150) + 0x10));
    FUN_004a3a19(local_48,"): creating with Model #");
    FUN_004a3f7a(local_48,*(int *)((int)param_1 + 0x168));
    FUN_00401360(&local_64,0);
    local_4 = (uint)local_4._1_3_ << 8;
    FUN_004012c0(&local_64);
    *(undefined4 *)((int)param_1 + 0x16c) = 0;
    FUN_00497e90(*(void **)((int)param_1 + 0xd0),*(void **)(DAT_00503a78 + 4),2,
                 *(int *)((int)param_1 + 0x168),0);
    FUN_0042d350(*(void **)((int)param_1 + 0xd0),&local_64);
    FUN_0042d280((void *)(*(int *)((int)param_1 + 0xd0) + 0x34),local_70,0x40000000,0x40000000,
                 0x40000000);
    iVar4 = *(int *)((int)param_1 + 0xd0);
    fVar2 = *(float *)(iVar4 + 0x2c) - _DAT_004e2358;
    *(undefined4 *)(iVar4 + 0x28) = *(undefined4 *)(iVar4 + 0x28);
    *(undefined4 *)(iVar4 + 0x30) = *(undefined4 *)(iVar4 + 0x30);
    *(float *)(iVar4 + 0x2c) = fVar2;
    FUN_00496b50(*(void **)((int)param_1 + 0xd0),90.0);
    *(void **)(*(int *)((int)param_1 + 0xd0) + 0x40) = (void *)((int)param_1 + 0x24);
    FUN_00496c30((void *)((int)param_1 + 0x24),
                 *(float *)(*(int *)((int)param_1 + 0x150) + 0x18) * _DAT_004e1698 + _DAT_004e1cbc);
    *(undefined4 *)((int)param_1 + 0x120) = 4;
    (**(code **)(**(int **)(*(int *)((int)param_1 + 0xd0) + 0x94) + 8))();
    pvVar5 = (void *)((int)param_1 + 0x170);
    FUN_00480d00(pvVar5,*(int *)(*(int *)((int)param_1 + 0xd0) + 0x94));
    FUN_0045a3d0((int)param_1);
    *(undefined4 *)((int)param_1 + 0x218) = *(undefined4 *)((int)param_1 + 0x4c);
    *(undefined4 *)((int)param_1 + 0x21c) = *(undefined4 *)((int)param_1 + 0x50);
    *(undefined4 *)((int)param_1 + 0x220) = *(undefined4 *)((int)param_1 + 0x54);
    local_74 = &stack0xffffff70;
    *(undefined4 *)((int)param_1 + 0x224) = 0;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar10 = &DAT_00501908;
    if ((local_78 != &DAT_00501908) && (piVar10 = local_78, local_78 != &DAT_00501908)) {
      *local_78 = *local_78 + 1;
    }
    puVar3 = FUN_0048e330(*(void **)(DAT_00502aac + 0x6e8),piVar10,(int)param_1 + 0x4c);
    *(undefined4 **)((int)param_1 + 0x154) = puVar3;
    *(undefined1 *)((int)param_1 + 0x1fd) = 0;
    if (*(uint *)((int)param_1 + 0x180) == 6) {
      uVar11 = 0x3f800000;
      uVar6 = FUN_00404b30(3);
      FUN_00480ea0(pvVar5,uVar6,uVar11);
    }
    else {
      uVar11 = 0x3e4ccccd;
      uVar6 = FUN_00404b30(*(uint *)((int)param_1 + 0x180));
      FUN_00480ea0(pvVar5,uVar6,uVar11);
      *(undefined4 *)((int)param_1 + 0x120) = 4;
    }
  }
  if (local_78 == &DAT_00501908) {
    ExceptionList = local_c;
    return;
  }
  *local_78 = *local_78 + -1;
  iVar4 = *local_78;
LAB_0045bdaa:
  local_4 = 0xffffffff;
  if (iVar4 < 1) {
    FUN_004011c0(&local_78);
  }
  ExceptionList = local_c;
  return;
}


