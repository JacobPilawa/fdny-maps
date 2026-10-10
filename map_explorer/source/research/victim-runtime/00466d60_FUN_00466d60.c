/* Ghidra decompilation; names/types are provisional.
 * Entry: 00466d60
 * Function: FUN_00466d60
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00466d60(int *param_1)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  void *pvVar3;
  int *piVar4;
  int iVar5;
  bool bVar6;
  int *piVar7;
  undefined4 uVar8;
  int *piVar9;
  float fVar10;
  int *local_74;
  undefined4 local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1c76;
  pvStack_c = ExceptionList;
  local_64 = &PTR_FUN_004dc554;
  ExceptionList = &pvStack_c;
  FUN_004a3118();
  local_60 = "hose\\PlayerHoseEnt.cpp";
  local_5c = 0x152;
  local_58 = 0;
  local_4 = 0;
  FUN_004a3a19(local_48,"PlayerHoseEnt: ");
  FUN_004a3a19(local_48,"Created PlayeHose Ent");
  FUN_00401360(&local_64,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_64);
  puVar1 = (undefined4 *)FUN_0049d350(param_1,&local_70);
  local_74 = (int *)&stack0xffffff70;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar7 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff70,"Player");
  FUN_0046e320(param_1 + 0x50,piVar7,puVar1);
  FUN_00497e90((void *)param_1[0x34],*(void **)(DAT_00503a78 + 4),2,10,0);
  *(undefined4 *)(param_1[0x34] + 0x40) = 0;
  *(undefined1 *)(param_1[0x34] + 0x46) = 1;
  puVar1 = operator_new(0x20);
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    *puVar1 = 0;
    puVar1[1] = 0;
    puVar1[2] = 0;
  }
  puVar2 = (undefined4 *)FUN_0049d350(param_1,&local_70);
  puVar1[3] = *puVar2;
  puVar1[4] = puVar2[1];
  puVar1[5] = 0;
  FUN_0042d280(param_1 + 0x9b,&local_70,0,0,0);
  puVar1[6] = param_1 + 0x9b;
  local_74 = operator_new(0x14);
  local_4 = 1;
  if (local_74 == (int *)0x0) {
    pvVar3 = (void *)0x0;
  }
  else {
    pvVar3 = (void *)FUN_0049f230(local_74);
  }
  local_4 = 0xffffffff;
  piVar7 = FUN_0049b760(&local_70,"RCS_CamHose",100);
  local_4 = 2;
  local_74 = (int *)FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar7);
  bVar6 = local_74 != (int *)0x0;
  local_4 = 0xffffffff;
  FUN_0049b6f0((int)&local_70);
  if (bVar6) {
    *(int **)((int)pvVar3 + 0xc) = local_74;
  }
  *(undefined4 *)((int)pvVar3 + 8) = 0;
  FUN_0049d130(pvVar3,puVar1);
  FUN_0049d4c0(pvVar3);
  param_1[0x1d] = 0x42340000;
  FUN_0042d280(param_1 + 0x1e,&local_70,0xc1f00000,0xc2340000,0xc1f00000);
  FUN_0042d280(param_1 + 0x21,&local_70,0x41f00000,0x42340000,0x41f00000);
  local_70 = 0;
  param_1[0x42] = 0;
  local_6c = 0xc4fa0000;
  param_1[0x43] = -0x3b060000;
  local_68 = 0;
  param_1[0x44] = 0;
  param_1[0x8f] = 0x42700000;
  param_1[0x8e] = 0x42340000;
  param_1[0x8c] = 0;
  param_1[0x8d] = 0;
  param_1[0x90] = 0;
  param_1[0x694] = 0;
  param_1[0x693] = 0;
  param_1[0x696] = 1;
  param_1[0x69f] = 0;
  param_1[0x6a0] = 0;
  param_1[99] = 3;
  FUN_004661a0(param_1);
  piVar9 = (int *)0x1;
  uVar8 = 0x19;
  piVar7 = param_1 + 0x520;
  piVar4 = (int *)(**(code **)(*param_1 + 0x18))();
  FUN_0046d6b0(piVar7,piVar4,uVar8,piVar9);
  FUN_00465580(param_1 + 0x9e,*(int **)(DAT_00503a78 + 4),param_1,piVar7);
  pvVar3 = operator_new(8);
  *(int **)((int)pvVar3 + 4) = piVar7;
  local_74 = operator_new(0x14);
  local_4 = 3;
  if (local_74 == (int *)0x0) {
    local_74 = (int *)0x0;
  }
  else {
    local_74 = (int *)FUN_0049f230(local_74);
  }
  local_4 = 0xffffffff;
  piVar7 = FUN_0049b760(&local_70,"RCS_DouseHose",100);
  local_4 = 4;
  iVar5 = FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar7);
  local_4 = 0xffffffff;
  FUN_0049b6f0((int)&local_70);
  if (iVar5 != 0) {
    local_74[3] = iVar5;
  }
  local_74[2] = 0;
  FUN_0049d130(local_74,pvVar3);
  FUN_0049d4c0(local_74);
  param_1[0x66] = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_74 = &DAT_00501908;
  FUN_00401d80(&local_74,"HoseSpray");
  local_4 = 5;
  FUN_004a12f0(param_1 + 0x67,&local_74,0,2);
  local_4 = 0xffffffff;
  if ((local_74 != &DAT_00501908) && (*local_74 = *local_74 + -1, *local_74 < 1)) {
    FUN_004011c0(&local_74);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_74 = &DAT_00501908;
  FUN_00401d80(&local_74,"LevelEndWin");
  local_4 = 6;
  FUN_004a12f0(param_1 + 0x70,&local_74,0,2);
  local_4 = 0xffffffff;
  if ((local_74 != &DAT_00501908) && (*local_74 = *local_74 + -1, *local_74 < 1)) {
    FUN_004011c0(&local_74);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_74 = &DAT_00501908;
  FUN_00401d80(&local_74,"LevelEndLose");
  local_4 = 7;
  FUN_004a12f0(param_1 + 0x79,&local_74,0,2);
  local_4 = 0xffffffff;
  if ((local_74 != &DAT_00501908) && (*local_74 = *local_74 + -1, *local_74 < 1)) {
    FUN_004011c0(&local_74);
  }
  FUN_004895c0(param_1 + 0x82);
  param_1[0x65c] = 2;
  FUN_0042d280(param_1 + 0x65d,&local_70,0x42480000,0x42480000,0x42480000);
  FUN_0047f1f0(param_1 + 0x630,0x3f000000);
  fVar10 = 90.0;
  param_1[0x659] = 0x20;
  pvVar3 = (void *)FUN_0047f1a0((int)(param_1 + 0x630));
  FUN_00496b50(pvVar3,fVar10);
  ExceptionList = pvStack_c;
  return;
}

