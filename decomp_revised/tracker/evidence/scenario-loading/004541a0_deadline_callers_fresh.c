/* Fresh Ghidra 12.0.4 decompilation, read-only -noanalysis pass.
 * FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Decompiler output is pseudocode, not source. 004541A0 omits its x87 input
 * arithmetic; see the linked evidence note for instruction and P-code results.
 */

/* 00451160 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm:
   - Return without changes when Player +0x3534 has no carried victim.
   - Update rescue-count and rescue-time statistics, then release the victim through 0x0045C000.
   - Update Player status and rescue deadline; clear the carry pointer and reset carry effects.
   - Call UpdateFinalExitAvailability after the release transaction.
   
   Parameters:
   - `param_1`: Player instance.
   
   Returns:
   - void. */

void __fastcall ProcessCarriedVictimRescue(int *param_1)

{
  int *local_14;
  int *local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d02b8;
  local_c = ExceptionList;
  if (param_1[0xd4d] != 0) {
    ExceptionList = &local_c;
    param_1[0xd4b] = param_1[0xd4b] + 1;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_10 = &DAT_00501908;
    FUN_00401d80(&local_10,"Rescues");
    local_4 = 0;
    FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_10,1);
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
    local_10 = &DAT_00501908;
    FUN_00401d80(&local_10,"Longest Rescue Time");
    local_4 = 1;
    FUN_004868c0((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_10,
                 DAT_00502afc - param_1[0x171]);
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
    FUN_00401d80(&local_14,"Shortest Rescue Time");
    local_4 = 2;
    FUN_004868c0((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_14,
                 DAT_00502afc - param_1[0x171]);
    local_4 = 0xffffffff;
    if (local_14 != &DAT_00501908) {
      *local_14 = *local_14 + -1;
      if (*local_14 < 1) {
        FUN_004011c0(&local_14);
      }
      else {
        local_14 = &DAT_00501908;
      }
    }
    FUN_0045c000((void *)param_1[0xd4d],param_1);
    FUN_004571a0(param_1 + 0x17d,(int *)0x2,(undefined1 *)0x0);
    thunk_FUN_004541a0((int)(param_1 + 0xd03));
    param_1[0xd4d] = 0;
    FUN_0044fce0('\0');
    UpdateFinalExitAvailability(param_1);
  }
  ExceptionList = local_c;
  return;
}

/* 004541A0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004541a0(int param_1)

{
  uint uVar1;
  ulonglong uVar2;
  
  uVar2 = ConvertX87FloatToIntegerTowardZero();
  if (DAT_00502afc < (uint)uVar2) {
    uVar1 = FUN_00404b30(30000);
    *(uint *)(param_1 + 0x14) = uVar1 + (uint)uVar2;
    return;
  }
  uVar2 = ConvertX87FloatToIntegerTowardZero();
  *(int *)(param_1 + 0x14) = (int)uVar2;
  uVar1 = FUN_00404b30(60000);
  *(uint *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + uVar1;
  return;
}

/* 00454BA0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00454ba0(int param_1)

{
  int *piVar1;
  int iVar2;
  uint uVar3;
  void *pvVar4;
  int *local_14;
  int local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d08e8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(undefined4 *)(param_1 + 4) = 0;
  *(int *)(param_1 + 0x10) = DAT_00502afc + 120000;
  local_10 = param_1;
  uVar3 = FUN_00404b30(120000);
  *(int *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + uVar3;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"I\'ve located %s, and am heading for the nearest exit.");
  pvVar4 = (void *)(param_1 + 8);
  local_4 = 0;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"I just located %s.  I\'m heading to an exit.");
  local_4 = 1;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"[Cough cough]  I\'ve located %s, proceeding to exit.");
  local_4 = 2;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"I found %s.  Heading to the nearest exit...");
  local_4 = 3;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"I just found %s.  Let\'s get these people to safety!");
  local_4 = 4;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"I\'ve spotted %s here.  I\'ll take care of this one!");
  local_4 = 5;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"I\'m taking %s to safety.  Over and out.");
  local_4 = 6;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"I just spotted %s here.  I\'ll handle this one!");
  local_4 = 7;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"Let\'s move!");
  pvVar4 = (void *)(local_10 + 0xc);
  local_4 = 8;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"Let\'s get this fire under control!");
  local_4 = 9;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"Come on, let\'s hussle!");
  local_4 = 10;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"We\'re counting on you!");
  local_4 = 0xb;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_14 = &DAT_00501908;
  FUN_00401d80(&local_14,"Hurry up, we\'re running out of time!");
  local_4 = 0xc;
  FUN_00407540(pvVar4,&local_14);
  local_4 = 0xffffffff;
  if ((local_14 != &DAT_00501908) && (iVar2 = *local_14, *local_14 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_14);
  }
  iVar2 = local_10;
  *(int *)(local_10 + 0x10) = DAT_00502afc + 120000;
  uVar3 = FUN_00404b30(120000);
  piVar1 = (int *)(iVar2 + 0x10);
  *piVar1 = *piVar1 + uVar3;
  FUN_004541a0(iVar2);
  ExceptionList = local_c;
  return;
}

/* 00455580 */

/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00455580(uint *param_1)

{
  undefined4 *puVar1;
  void *this;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  int *piVar5;
  void *pvVar6;
  int extraout_ECX;
  int extraout_ECX_00;
  ulonglong uVar7;
  char *pcVar8;
  int iVar9;
  int *local_84;
  int local_80 [29];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d097b;
  local_c = ExceptionList;
  if ((char)param_1[6] != '\0') {
    pvVar6 = (void *)DAT_00503a78[0xc];
    local_84 = DAT_00502afc;
    if (*(int *)((int)pvVar6 + 0x18c) != 6) {
      ExceptionList = &local_c;
      if ((param_1[1] != 0) && (ExceptionList = &local_c, (int *)*param_1 < DAT_00502afc)) {
        ExceptionList = &local_c;
        UpdateFinalExitAvailability(pvVar6);
        puVar1 = FUN_00401290(local_80 + 7,"rescue\\RescueBot.cpp",0x3c);
        local_4 = 0;
        FUN_004a3a19(puVar1 + 7,"RESCUEBOT: Victim dropped off");
        FUN_00401360(puVar1,3);
        local_4 = 0xffffffff;
        FUN_004012c0(local_80 + 7);
        param_1[1] = 0;
        FUN_00454130((int)param_1);
        FUN_00454100(extraout_ECX);
      }
      if ((param_1[1] == 0) &&
         (((iVar3 = *(int *)((int)pvVar6 + 0x18c), iVar3 == 3 || (iVar3 == 4)) || (iVar3 == 5)))) {
        if (DAT_00502a78 != '\0') {
          FUN_00402790(local_80,"DBG_ACTIVATE_RESCUEBOT");
          piVar5 = local_80;
          local_4 = 1;
          this = (void *)FUN_00499cb0(DAT_00503a78);
          uVar2 = FUN_00438550(this,piVar5);
          local_4 = 0xffffffff;
          FUN_00401c90(local_80);
          if ((char)uVar2 != '\0') {
            param_1[5] = (int)local_84 + 1;
          }
        }
        if ((int *)param_1[5] < local_84) {
          local_80[1] = 0;
          local_80[2] = 0;
          local_80[3] = 0;
          local_80[4] = 0x3fd00000;
          local_80[5] = 0x9999999a;
          local_80[6] = 0x3fd99999;
          uVar7 = ConvertX87FloatToIntegerTowardZero();
          iVar3 = (int)uVar7;
          if (*(int *)((int)pvVar6 + 0x3530) < iVar3) {
            FUN_00455230((int)param_1);
          }
          else {
            puVar1 = FUN_00401290(local_80 + 7,"rescue\\RescueBot.cpp",100);
            iVar9 = 3;
            pcVar8 = " victims";
            local_4 = 2;
            pvVar6 = (void *)FUN_0040c2f0(puVar1,"RESCUEBOT: Exceeded maximum of ");
            pvVar6 = (void *)FUN_00412130(pvVar6,iVar3);
            pvVar6 = (void *)FUN_0040c2f0(pvVar6,pcVar8);
            FUN_00401360(pvVar6,iVar9);
            local_4 = 0xffffffff;
            FUN_004012c0(local_80 + 7);
          }
          FUN_004541a0((int)param_1);
        }
        if ((int *)param_1[4] < local_84) {
          uVar4 = FUN_00404b30(*(uint *)(param_1[3] + 4));
          puVar1 = (undefined4 *)FUN_00412ed0(param_1 + 3,uVar4);
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          local_84 = &DAT_00501908;
          piVar5 = (int *)*puVar1;
          if (piVar5 != &DAT_00501908) {
            *piVar5 = *piVar5 + 1;
            local_84 = piVar5;
          }
          local_4 = 3;
          piVar5 = FUN_00454490(local_80 + 5);
          local_4._0_1_ = 4;
          pvVar6 = FUN_00412340(local_80 + 3,"^(128,128,240)",piVar5);
          local_4._0_1_ = 5;
          AppendCStringToString(pvVar6,local_80 + 1,":\n^(255,255,255)");
          local_4._0_1_ = 8;
          FUN_00401c90(local_80 + 3);
          local_4._0_1_ = 7;
          FUN_00401c90(local_80 + 5);
          FUN_00401d00(local_80 + 1,(char *)(local_84 + 3));
          FUN_004574b0((void *)(DAT_00503a78[0xc] + 0x5f4),&local_84);
          FUN_00454130((int)param_1);
          FUN_00454100(extraout_ECX_00);
          local_4 = CONCAT31(local_4._1_3_,3);
          FUN_00401c90(local_80 + 1);
          local_4 = 0xffffffff;
          FUN_00401c90(&local_84);
        }
      }
    }
  }
  ExceptionList = local_c;
  return;
}
