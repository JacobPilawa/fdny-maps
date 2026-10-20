/* Fresh Ghidra 12.0.4 headless decompilation, read-only pass.
 * Imported binary: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * The pseudocode below is decompiler output, not recovered source.
 * 004541A0 loses x87 values in decompilation; its exact arithmetic is
 * backed by the retained instruction trace and existing deadline helper.
 */

/* Function 00454100 */
void __fastcall FUN_00454100(int param_1)

{
  uint uVar1;
  
  *(int *)(param_1 + 0x10) = DAT_00502afc + 120000;
  uVar1 = FUN_00404b30(120000);
  *(uint *)(param_1 + 0x10) = *(int *)(param_1 + 0x10) + uVar1;
  return;
}


/* Function 00454130 */
void __fastcall FUN_00454130(int param_1)

{
  if (DAT_00502afc < *(uint *)(param_1 + 0x10)) {
    *(uint *)(param_1 + 0x10) = *(uint *)(param_1 + 0x10) + 10000;
  }
  if (DAT_00502afc < *(uint *)(param_1 + 0x14)) {
    *(uint *)(param_1 + 0x14) = *(uint *)(param_1 + 0x14) + 10000;
  }
  return;
}


/* Function 004541A0 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004541a0(int param_1)

{
  uint uVar1;
  ulonglong uVar2;
  
  uVar2 = FUN_004b9400();
  if (DAT_00502afc < (uint)uVar2) {
    uVar1 = FUN_00404b30(30000);
    *(uint *)(param_1 + 0x14) = uVar1 + (uint)uVar2;
    return;
  }
  uVar2 = FUN_004b9400();
  *(int *)(param_1 + 0x14) = (int)uVar2;
  uVar1 = FUN_00404b30(60000);
  *(uint *)(param_1 + 0x14) = *(int *)(param_1 + 0x14) + uVar1;
  return;
}


/* Function 00454750 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00454750(void *this,int *param_1,int *param_2)

{
  int *piVar1;
  void *pvVar2;
  uint uVar3;
  int *piVar4;
  int extraout_ECX;
  int *extraout_ECX_00;
  int *extraout_ECX_01;
  int *piVar5;
  int *piVar6;
  int iVar7;
  int *local_88;
  undefined4 local_84;
  int *local_80;
  int *local_7c;
  int *local_78;
  int *local_74;
  int *local_70;
  int *local_6c;
  undefined **local_68;
  char *pcStack_64;
  undefined4 uStack_60;
  undefined1 uStack_5c;
  undefined1 auStack_4c [60];
  void *pvStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0869;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  piVar1 = FUN_00454490(&local_74);
  local_4 = 0;
  pvVar2 = FUN_00412340(&local_78,"^(128,128,240)",piVar1);
  local_4._0_1_ = 1;
  pvVar2 = FUN_00403e40(pvVar2,&local_7c,", from ");
  local_4._0_1_ = 2;
  pvVar2 = FUN_004040e0(pvVar2,&local_80,param_2);
  local_4._0_1_ = 3;
  FUN_00403e40(pvVar2,&local_84,":\n^(255,255,255)");
  local_4._0_1_ = 8;
  if (local_80 != &DAT_00501908) {
    *local_80 = *local_80 + -1;
    if (*local_80 < 1) {
      FUN_004011c0(&local_80);
    }
    else {
      local_80 = &DAT_00501908;
    }
  }
  local_4._0_1_ = 7;
  if (local_7c != &DAT_00501908) {
    *local_7c = *local_7c + -1;
    if (*local_7c < 1) {
      FUN_004011c0(&local_7c);
    }
    else {
      local_7c = &DAT_00501908;
    }
  }
  local_4._0_1_ = 6;
  if (local_78 != &DAT_00501908) {
    *local_78 = *local_78 + -1;
    if (*local_78 < 1) {
      FUN_004011c0(&local_78);
    }
    else {
      local_78 = &DAT_00501908;
    }
  }
  local_4 = CONCAT31(local_4._1_3_,5);
  if (local_74 != &DAT_00501908) {
    *local_74 = *local_74 + -1;
    if (*local_74 < 1) {
      FUN_004011c0(&local_74);
    }
    else {
      local_74 = &DAT_00501908;
    }
  }
  piVar1 = (int *)((int)this + 8);
  uVar3 = FUN_00404b30(*(uint *)(*(int *)((int)this + 8) + 4));
  FUN_00484800(piVar1,uVar3);
  if (1 < *(int *)*piVar1) {
    FUN_00405d00(piVar1);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = *(int **)(*piVar1 + 0x14 + uVar3 * 4);
  local_6c = &DAT_00501908;
  if (piVar1 != &DAT_00501908) {
    *piVar1 = *piVar1 + 1;
    local_6c = piVar1;
  }
  piVar1 = local_6c;
  local_4._0_1_ = 9;
  FUN_0045bdd0(param_1,&local_70);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_88 = &DAT_00501908;
  local_4._0_1_ = 0xb;
  piVar4 = FUN_00402770(&local_88,(byte *)(piVar1 + 3));
  FUN_00401d00(&local_84,(char *)(*piVar4 + 0xc));
  local_4._0_1_ = 10;
  if (local_88 != &DAT_00501908) {
    *local_88 = *local_88 + -1;
    if (*local_88 < 1) {
      FUN_004011c0(&local_88);
    }
    else {
      local_88 = &DAT_00501908;
    }
  }
  local_4._0_1_ = 9;
  if ((local_70 != &DAT_00501908) && (*local_70 = *local_70 + -1, *local_70 < 1)) {
    FUN_004011c0(&local_70);
  }
  FUN_004574b0((void *)(*(int *)(DAT_00503a78 + 0x30) + 0x5f4),&local_84);
  local_70 = (int *)&stack0xffffff60;
  iVar7 = extraout_ECX;
  FUN_00401cb0(&stack0xffffff60,param_2);
  local_88 = (int *)&stack0xffffff5c;
  local_4._0_1_ = 0xc;
  piVar6 = extraout_ECX_00;
  FUN_00454430(param_1,(undefined4 *)&stack0xffffff5c);
  local_68 = (undefined **)&stack0xffffff58;
  piVar5 = (int *)&stack0xffffff58;
  local_4 = CONCAT31(local_4._1_3_,0xd);
  piVar4 = extraout_ECX_01;
  (**(code **)(*param_1 + 0x60))();
  puStack_8._0_1_ = 0xe;
  local_68 = &PTR_FUN_004dc554;
  FUN_004a3118();
  pcStack_64 = "rescue\\RescueBot.cpp";
  uStack_60 = 0xcd;
  uStack_5c = 0;
  puStack_8._0_1_ = 0x10;
  FUN_004a3a19(auStack_4c,"RESCUEBOT: Rescuing ");
  pvVar2 = FUN_0040f020(&local_68,piVar5);
  puStack_8._0_1_ = 0x11;
  FUN_004a3a19((void *)((int)pvVar2 + 0x1c)," (");
  pvVar2 = FUN_0040f020(pvVar2,piVar4);
  puStack_8._0_1_ = 0x12;
  FUN_004a3a19((void *)((int)pvVar2 + 0x1c),") in ");
  pvVar2 = FUN_0040f020(pvVar2,piVar6);
  FUN_00401360(pvVar2,iVar7);
  puStack_8._0_1_ = 9;
  FUN_004012c0(&local_68);
  *(int *)(*(int *)(DAT_00503a78 + 0x30) + 0x3530) =
       *(int *)(*(int *)(DAT_00503a78 + 0x30) + 0x3530) + 1;
  FUN_0045bf70((int)param_1);
  *(int **)((int)this + 4) = param_1;
  iVar7 = DAT_00502afc;
  uVar3 = FUN_00404b30(4000);
  *(uint *)this = uVar3 + 4000 + iVar7;
  puStack_8 = (undefined1 *)CONCAT31(puStack_8._1_3_,5);
  if ((piVar1 != &DAT_00501908) && (iVar7 = *piVar1, *piVar1 = iVar7 + -1, iVar7 + -1 < 1)) {
    FUN_004011c0(&local_70);
  }
  puStack_8 = (undefined1 *)0xffffffff;
  if ((local_88 != &DAT_00501908) && (*local_88 = *local_88 + -1, *local_88 < 1)) {
    FUN_004011c0(&local_88);
  }
  ExceptionList = pvStack_10;
  return;
}


/* Function 00454BA0 */
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


/* Function 00455230 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00455230(int param_1)

{
  int *piVar1;
  int *piVar2;
  char cVar3;
  undefined4 *puVar4;
  float *pfVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  undefined4 *puStack_f4;
  int *piStack_f0;
  int iStack_ec;
  int *piStack_e8;
  void *local_e4;
  int iStack_e0;
  char *pcStack_dc;
  char *pcStack_d8;
  undefined4 uStack_d4;
  undefined **ppuStack_c8;
  char *pcStack_c4;
  undefined4 uStack_c0;
  undefined1 uStack_bc;
  undefined1 auStack_ac [60];
  undefined **local_70;
  char *local_6c;
  undefined4 local_68;
  undefined1 local_64;
  undefined1 local_54 [60];
  float afStack_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0929;
  local_c = ExceptionList;
  piVar1 = *(int **)(DAT_00503a78 + 0x30);
  local_e4 = (void *)param_1;
  if (*(int *)(param_1 + 4) != 0) {
    local_70 = &PTR_FUN_004dc554;
    ExceptionList = &local_c;
    FUN_004a3118();
    local_6c = "rescue\\RescueBot.cpp";
    local_68 = 0x9c;
    local_64 = 0;
    local_4 = 0;
    FUN_004a3a19(local_54,"RESCUEBOT: Already rescuing someone");
    FUN_00401360(&local_70,3);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_70);
    ExceptionList = local_c;
    return;
  }
  ExceptionList = &local_c;
  FUN_00454730(piVar1 + 0x176,&puStack_f4);
  local_4 = 1;
  while ((puStack_f4 != (undefined4 *)0x0 && (iStack_ec != 0))) {
    piStack_e8 = (int *)(iStack_ec + 0xc);
    if ((piStack_e8 != (int *)piVar1[0x172]) && (iVar7 = 0, 0 < *(int *)(iStack_ec + 0x30))) {
      do {
        piVar2 = piStack_e8;
        if ((iVar7 < 0) || (piStack_e8[9] <= iVar7)) {
          piVar8 = &iStack_e0;
          for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
            *piVar8 = 0;
            piVar8 = piVar8 + 1;
          }
          iStack_e0 = 1;
          pcStack_dc = "Array index out of bounds";
          pcStack_d8 = "..\\BluelAPI/Container/SArray.h";
          uStack_d4 = 0x1cd;
          FUN_00422060(&iStack_e0);
        }
        piVar2 = *(int **)(piVar2[8] + iVar7 * 4);
        if (((piVar2[0x48] == 4) || (piVar2[0x48] == 7)) && (*(char *)((int)piVar2 + 0x1fd) == '\0')
           ) {
          cVar3 = '\0';
          iVar6 = (**(code **)(*piVar1 + 0x18))();
          puVar4 = (undefined4 *)(iVar6 + 0x28);
          iVar6 = (**(code **)(*piVar2 + 0x18))();
          cVar3 = FUN_00490130((undefined4 *)(iVar6 + 0x28),puVar4,cVar3);
          if (cVar3 == '\0') {
            iVar6 = (**(code **)(*piVar1 + 0x18))();
            pfVar5 = (float *)(iVar6 + 0x28);
            iVar6 = (**(code **)(*piVar2 + 0x18))();
            pfVar5 = (float *)FUN_0043b2d0(afStack_18,(float *)(iVar6 + 0x28),pfVar5);
            if ((float)_DAT_004e1f50 <=
                SQRT(pfVar5[2] * pfVar5[2] + pfVar5[1] * pfVar5[1] + *pfVar5 * *pfVar5)) {
              FUN_00454750(local_e4,piVar2,piStack_e8);
              local_4 = 0xffffffff;
              FUN_00454630((int *)&puStack_f4);
              ExceptionList = local_c;
              return;
            }
          }
        }
        iVar7 = iVar7 + 1;
      } while (iVar7 < piStack_e8[9]);
    }
    FUN_00432a60((int *)&puStack_f4);
  }
  ppuStack_c8 = &PTR_FUN_004dc554;
  FUN_004a3118();
  pcStack_c4 = "rescue\\RescueBot.cpp";
  uStack_c0 = 0xbf;
  uStack_bc = 0;
  local_4._0_1_ = 2;
  FUN_004a3a19(auStack_ac,"RESCUEBOT: No eligible victims");
  FUN_00401360(&ppuStack_c8,3);
  local_4 = CONCAT31(local_4._1_3_,1);
  FUN_004012c0(&ppuStack_c8);
  local_4 = 3;
  if (1 < *piStack_f0) {
    FUN_0041aff0((int *)&piStack_f0);
  }
  if (piStack_f0[1] != 0) {
    FUN_00471760(&piStack_f0,0);
  }
  if ((piStack_f0 != (int *)0x0) && (*piStack_f0 = *piStack_f0 + -1, *piStack_f0 < 1)) {
    if ((void *)piStack_f0[4] != (void *)0x0) {
      _free((void *)piStack_f0[4]);
    }
    _free(piStack_f0);
    piStack_f0 = (int *)0x0;
  }
  local_4 = 0xffffffff;
  if (puStack_f4 != (undefined4 *)0x0) {
    FUN_00429fe0((void *)*puStack_f4,(int)&puStack_f4);
  }
  ExceptionList = local_c;
  return;
}


/* Function 00455580 */
/* WARNING: Type propagation algorithm not settling */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00455580(uint *param_1)

{
  void *pvVar1;
  undefined4 uVar2;
  int iVar3;
  uint uVar4;
  undefined4 *puVar5;
  int *piVar6;
  void *pvVar7;
  int extraout_ECX;
  int extraout_ECX_00;
  ulonglong uVar8;
  char *pcVar9;
  int iVar10;
  int *local_84;
  int local_80 [29];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d097b;
  local_c = ExceptionList;
  if ((char)param_1[6] != '\0') {
    pvVar7 = (void *)DAT_00503a78[0xc];
    local_84 = DAT_00502afc;
    if (*(int *)((int)pvVar7 + 0x18c) != 6) {
      ExceptionList = &local_c;
      if ((param_1[1] != 0) && (ExceptionList = &local_c, (int *)*param_1 < DAT_00502afc)) {
        ExceptionList = &local_c;
        FUN_004504c0(pvVar7);
        pvVar1 = FUN_00401290(local_80 + 7,"rescue\\RescueBot.cpp",0x3c);
        local_4 = 0;
        FUN_004a3a19((void *)((int)pvVar1 + 0x1c),"RESCUEBOT: Victim dropped off");
        FUN_00401360(pvVar1,3);
        local_4 = 0xffffffff;
        FUN_004012c0(local_80 + 7);
        param_1[1] = 0;
        FUN_00454130((int)param_1);
        FUN_00454100(extraout_ECX);
      }
      if ((param_1[1] == 0) &&
         (((iVar3 = *(int *)((int)pvVar7 + 0x18c), iVar3 == 3 || (iVar3 == 4)) || (iVar3 == 5)))) {
        if (DAT_00502a78 != '\0') {
          FUN_00402790(local_80,"DBG_ACTIVATE_RESCUEBOT");
          piVar6 = local_80;
          local_4 = 1;
          pvVar1 = (void *)FUN_00499cb0(DAT_00503a78);
          uVar2 = FUN_00438550(pvVar1,piVar6);
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
          uVar8 = FUN_004b9400();
          iVar3 = (int)uVar8;
          if (*(int *)((int)pvVar7 + 0x3530) < iVar3) {
            FUN_00455230((int)param_1);
          }
          else {
            pvVar7 = FUN_00401290(local_80 + 7,"rescue\\RescueBot.cpp",100);
            iVar10 = 3;
            pcVar9 = " victims";
            local_4 = 2;
            pvVar7 = FUN_0040c2f0(pvVar7,"RESCUEBOT: Exceeded maximum of ");
            pvVar7 = FUN_00412130(pvVar7,iVar3);
            pvVar7 = FUN_0040c2f0(pvVar7,pcVar9);
            FUN_00401360(pvVar7,iVar10);
            local_4 = 0xffffffff;
            FUN_004012c0(local_80 + 7);
          }
          FUN_004541a0((int)param_1);
        }
        if ((int *)param_1[4] < local_84) {
          uVar4 = FUN_00404b30(*(uint *)(param_1[3] + 4));
          puVar5 = (undefined4 *)FUN_00412ed0(param_1 + 3,uVar4);
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          local_84 = &DAT_00501908;
          piVar6 = (int *)*puVar5;
          if (piVar6 != &DAT_00501908) {
            *piVar6 = *piVar6 + 1;
            local_84 = piVar6;
          }
          local_4 = 3;
          piVar6 = FUN_00454490(local_80 + 5);
          local_4._0_1_ = 4;
          pvVar7 = FUN_00412340(local_80 + 3,"^(128,128,240)",piVar6);
          local_4._0_1_ = 5;
          FUN_00403e40(pvVar7,local_80 + 1,":\n^(255,255,255)");
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


/* Function 0045BF70 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0045bf70(int param_1)

{
  int *piVar1;
  
  *(undefined4 *)(param_1 + 0x120) = 6;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar1 = &DAT_00501908;
  FUN_00401d80(&stack0xfffffff4,"botrescue");
  FUN_004705b0((void *)(param_1 + 0xd4),piVar1);
  *(undefined4 *)(param_1 + 0x1f0) = 0xffffffff;
  FUN_00480d40(param_1 + 0x170);
  FUN_00481140((void *)(param_1 + 0x170),'\0');
  return;
}

