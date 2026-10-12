
undefined1 * __fastcall FUN_0042b860(undefined1 *param_1)

{
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd9e6;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  FUN_0049c440((undefined4 *)(param_1 + 8));
  local_4 = 0;
  FUN_0049c4e0((undefined4 *)(param_1 + 0x50));
  local_4._0_1_ = 1;
  FUN_0049c490((undefined4 *)(param_1 + 0x98));
  local_4._0_1_ = 2;
  FUN_0049bf10((undefined4 *)(param_1 + 0xe0));
  local_4._0_1_ = 3;
  FUN_0042ecb0((undefined4 *)(param_1 + 0x3c8));
  local_4._0_1_ = 4;
  FUN_00422800((undefined4 *)(param_1 + 0x660));
  local_4._0_1_ = 5;
  FUN_0049f230((undefined4 *)(param_1 + 0x690));
  local_4._0_1_ = 6;
  FUN_0049f230((undefined4 *)(param_1 + 0x698));
  local_4._0_1_ = 7;
  FUN_004a00a0((undefined4 *)(param_1 + 0x69c));
  local_4._0_1_ = 8;
  FUN_004230e0((undefined4 *)(param_1 + 0x6ac));
  local_4 = CONCAT31(local_4._1_3_,9);
  FUN_00425090((int *)(param_1 + 0x6cc));
  DAT_00502aac = param_1;
  *param_1 = 0;
  param_1[2] = 0;
  param_1[4] = 0;
  param_1[5] = 0;
  *(undefined4 *)(param_1 + 0x694) = 0;
  param_1[0x6a9] = 0;
  *(undefined4 *)(param_1 + 0x688) = 0;
  param_1[1] = 1;
  param_1[3] = 1;
  ExceptionList = pvStack_c;
  return param_1;
}

