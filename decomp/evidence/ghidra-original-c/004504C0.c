/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 004504c0
 * Match: function entry
 * Entry: 004504c0 FUN_004504c0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004504c0(void *param_1)

{
  undefined4 uVar1;
  float *pfVar2;
  int iVar3;
  void *pvVar4;
  int iVar5;
  int *extraout_ECX;
  int iVar6;
  int *piVar7;
  int *piVar8;
  int local_c4;
  int *local_c0;
  void *local_bc;
  undefined1 *local_b8;
  int local_b4;
  int local_b0;
  undefined4 local_ac;
  undefined4 local_a8;
  code *local_a4;
  undefined4 local_a0;
  int local_9c;
  int local_98;
  undefined4 local_94;
  undefined4 local_90;
  code *local_8c;
  undefined4 local_88;
  float local_84;
  char *local_80;
  char *local_7c;
  undefined4 local_78;
  undefined1 *local_6c;
  undefined4 *local_68;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d00d7;
  local_c = ExceptionList;
  local_68 = (undefined4 *)((int)param_1 + 0x5d8);
  ExceptionList = &local_c;
  local_bc = param_1;
  FUN_00484260(local_68);
  if (*(int *)((int)param_1 + 0x3528) == -1) {
    FUN_0044fba0(*(int *)(DAT_00503a78 + 0x30));
  }
  iVar5 = *(int *)((int)param_1 + 0x3528) - *(int *)((int)param_1 + 0x3530);
  if (iVar5 != *(int *)((int)param_1 + 0x352c) && -1 < iVar5 - *(int *)((int)param_1 + 0x352c)) {
    ExceptionList = local_c;
    return;
  }
  local_88 = 0x10;
  local_8c = FUN_0043f660;
  local_9c = 0;
  local_98 = 0;
  local_94 = 0;
  local_90 = 0;
  local_a0 = 0x10;
  local_a4 = FUN_0043f660;
  local_b4 = 0;
  local_b0 = 0;
  local_ac = 0;
  local_a8 = 0;
  local_4._0_1_ = 1;
  local_4._1_3_ = 0;
  FUN_00481c00(&local_c4);
  local_4._0_1_ = 2;
LAB_00450590:
  do {
    if ((local_c4 == 0) || (local_c0 == (int *)0x0)) {
      iVar5 = 0;
    }
    else {
      iVar5 = *local_c0;
    }
    if ((local_c4 != 0) && (local_c0 != (int *)0x0)) {
      local_c0 = (int *)local_c0[1];
    }
    if (iVar5 == 0) break;
    if (*(int *)(iVar5 + 0x18) == 8) {
      local_6c = &stack0xffffff28;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar8 = &DAT_00501908;
      FUN_00401d80(&stack0xffffff28,"tell player finalexit");
      local_b8 = &stack0xffffff24;
      local_4._0_1_ = 3;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar7 = &DAT_00501908;
      FUN_00401d80(&stack0xffffff24,"touch");
      local_4._0_1_ = 2;
      uVar1 = FUN_0046ed70((void *)(iVar5 + 0x24),piVar7,piVar8);
      if ((char)uVar1 == '\0') {
        local_b8 = &stack0xffffff28;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar8 = &DAT_00501908;
        FUN_00401d80(&stack0xffffff28,"tell player (finalexitloadmap");
        local_6c = &stack0xffffff24;
        local_4._0_1_ = 4;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piVar7 = &DAT_00501908;
        FUN_00401d80(&stack0xffffff24,"touch");
        local_4._0_1_ = 2;
        uVar1 = FUN_0046ed70((void *)(iVar5 + 0x24),piVar7,piVar8);
        if ((char)uVar1 == '\0') goto LAB_00450590;
      }
      iVar6 = local_98;
      FUN_0043b6e0(&local_9c,local_98 + 1);
      *(int *)(local_9c + iVar6 * 4) = iVar5;
      pfVar2 = FUN_0044be90((void *)(iVar5 + 0xe4),&local_84);
      iVar3 = FUN_00483e10(local_68,pfVar2);
      iVar6 = local_b0;
      if (iVar3 == 0) {
        local_b8 = &stack0xffffff24;
        piVar8 = extraout_ECX;
        FUN_00401cb0(&stack0xffffff24,(undefined4 *)(iVar5 + 0x48));
        local_4._0_1_ = 5;
        local_64 = &PTR_FUN_004dc554;
        FUN_004a3118();
        local_60 = "rescue\\PlayerRescueEnt.cpp";
        local_5c = 0x407;
        local_58 = 0;
        local_4._0_1_ = 7;
        FUN_004a3a19(local_48,"ERROR: final exit bounding box \"");
        pvVar4 = (void *)FUN_0040f020(&local_64,piVar8);
        FUN_004a3a19((void *)((int)pvVar4 + 0x1c),"\" not located in any zone");
        FUN_00401360(pvVar4,iVar3);
        local_4._0_1_ = 2;
        FUN_004012c0(&local_64);
      }
      else {
        iVar5 = 0;
        if (0 < local_b0) {
          do {
            if (*(int *)(local_b4 + iVar5 * 4) == iVar3) {
              if (-1 < iVar5) goto LAB_00450590;
              break;
            }
            iVar5 = iVar5 + 1;
          } while (iVar5 < local_b0);
        }
        FUN_0043b6e0(&local_b4,local_b0 + 1);
        *(int *)(local_b4 + iVar6 * 4) = iVar3;
      }
    }
  } while( true );
  if (local_b0 == 0) {
    *(undefined4 *)((int)local_bc + 0x3438) = 1;
    FUN_0044c360(local_bc);
    local_4 = CONCAT31(local_4._1_3_,1);
    iVar5 = local_98;
    if (local_c4 != 0) {
      FUN_00429fe0((void *)(local_c4 + 0x28),(int)&local_c4);
      local_c4 = 0;
      local_c0 = (int *)0x0;
      iVar5 = local_98;
    }
  }
  else {
    pvVar4 = (void *)((int)local_bc + 0x5d0);
    FUN_00401d80(pvVar4,"All of the survivors have been rescued!  Exit through:\n");
    iVar5 = 0;
    if (0 < local_b0) {
      iVar6 = local_b0 + -1;
      do {
        if ((iVar5 < 0) || (local_b0 <= iVar5)) {
          pfVar2 = &local_84;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *pfVar2 = 0.0;
            pfVar2 = pfVar2 + 1;
          }
          local_84 = 1.4013e-45;
          local_80 = "Array index out of bounds";
          local_7c = "..\\BluelAPI/Container/SArray.h";
          local_78 = 0x1cd;
          FUN_00422060((int *)&local_84);
        }
        FUN_00401d00(pvVar4,(char *)(**(int **)(local_b4 + iVar5 * 4) + 0xc));
        if (iVar5 == iVar6) break;
        FUN_00401d00(pvVar4,",\n");
        if (iVar5 == local_b0 + -2) {
          FUN_00401d00(pvVar4,"or ");
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < local_b0);
    }
    iVar6 = 0;
    *(undefined4 *)((int)local_bc + 0x5cc) = 0;
    iVar5 = local_98;
    if (0 < local_98) {
      do {
        if ((iVar6 < 0) || (iVar5 <= iVar6)) {
          pfVar2 = &local_84;
          for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
            *pfVar2 = 0.0;
            pfVar2 = pfVar2 + 1;
          }
          local_84 = 1.4013e-45;
          local_80 = "Array index out of bounds";
          local_7c = "..\\BluelAPI/Container/SArray.h";
          local_78 = 0x1cd;
          FUN_00422060((int *)&local_84);
          iVar5 = local_98;
        }
        FUN_00440f50(*(void **)(local_9c + iVar6 * 4),'\x01');
        iVar6 = iVar6 + 1;
      } while (iVar6 < iVar5);
    }
    local_4._0_1_ = 1;
    if (local_c4 != 0) {
      FUN_00429fe0((void *)(local_c4 + 0x28),(int)&local_c4);
      local_c4 = 0;
      local_c0 = (int *)0x0;
    }
    local_4 = (uint)local_4._1_3_ << 8;
    FUN_0043b6e0(&local_b4,0);
  }
  local_4 = 0xffffffff;
  if (iVar5 != 0) {
    FUN_0043b6e0(&local_9c,0);
  }
  ExceptionList = local_c;
  return;
}


