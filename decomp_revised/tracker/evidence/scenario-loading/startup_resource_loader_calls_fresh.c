
/* WARNING: Function: __security_check_cookie replaced with injection: security_check_cookie */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0042b4e0(int param_1)

{
  undefined4 *puVar1;
  int *piVar2;
  void *this;
  int *extraout_ECX;
  uint unaff_retaddr;
  undefined1 local_88 [120];
  uint local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;

  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cd950;
  local_c = ExceptionList;
  local_10 = DAT_005007e0 ^ unaff_retaddr;
  ExceptionList = &local_c;
  *(undefined1 *)(param_1 + 0x68c) = 0;
  puVar1 = operator_new(0x1044);
  local_4 = 0;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0049b9e0(puVar1);
  }
  local_4 = 0xffffffff;
  *(undefined4 **)(param_1 + 0x3b4) = puVar1;
  puVar1 = operator_new(0x98);
  local_4 = 1;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_004a0f00(puVar1);
  }
  local_4 = 0xffffffff;
  *(undefined4 **)(param_1 + 0x3b0) = puVar1;
  piVar2 = operator_new(0x48);
  local_4 = 2;
  if (piVar2 == (int *)0x0) {
    piVar2 = (int *)0x0;
  }
  else {
    piVar2 = FUN_0049f190(piVar2);
  }
  local_4 = 0xffffffff;
  *(int **)(param_1 + 0x3b8) = piVar2;
  puVar1 = operator_new(0x550);
  local_4 = 3;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0042b1f0(puVar1);
  }
  local_4 = 0xffffffff;
  *(undefined4 **)(param_1 + 0x3bc) = puVar1;
  puVar1 = operator_new(0x4c);
  local_4 = 4;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0042b2c0(puVar1);
  }
  local_4 = 0xffffffff;
  *(undefined4 **)(param_1 + 0x3c4) = puVar1;
  puVar1 = operator_new(0x4c);
  local_4 = 5;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = InitializeEntityFactoryQueueService(puVar1);
  }
  *(undefined4 **)(param_1 + 0x3c0) = puVar1;
  *(undefined4 *)(DAT_00503a78 + 8) = *(undefined4 *)(param_1 + 0x3b0);
  *(undefined4 *)(DAT_00503a78 + 0x10) = *(undefined4 *)(param_1 + 0x3b4);
  *(undefined4 *)(DAT_00503a78 + 0x14) = *(undefined4 *)(param_1 + 0x3b8);
  *(undefined4 *)(DAT_00503a78 + 0x1c) = *(undefined4 *)(param_1 + 0x3bc);
  *(undefined4 *)(DAT_00503a78 + 0x20) = *(undefined4 *)(param_1 + 0x3c4);
  local_4 = 0xffffffff;
  *(undefined4 *)(DAT_00503a78 + 0x24) = *(undefined4 *)(param_1 + 0x3c0);
  puVar1 = operator_new(0xc);
  local_4 = 6;
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1 = FUN_0049e360(puVar1);
  }
  local_4 = 0xffffffff;
  FUN_0049f160(*(void **)(DAT_00503a78 + 0x14),(int)puVar1);
  RegisterEntityClassFactories();
  FUN_0049a520((int)local_88);
  LoadResourceDefinitionsFile(local_88,"data\\resdefs\\sound.rrf");
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar2 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff5c,"data\\resdefs\\thegame.lev");
  FUN_00423160(piVar2);
  *(undefined4 *)(param_1 + 0x688) = 0;
  this = operator_new(0x18);
  local_4 = 7;
  if (this == (void *)0x0) {
    piVar2 = (int *)0x0;
  }
  else {
    piVar2 = extraout_ECX;
    FUN_00402790(&stack0xffffff5c,"data\\resdefs\\victims.srf");
    piVar2 = FUN_0048e740(this,piVar2);
  }
  local_4 = 0xffffffff;
  *(int **)(param_1 + 0x6e8) = piVar2;
  *(undefined1 *)(param_1 + 0x6a8) = 0;
  FUN_0042c1a0((int *)&DAT_00502ab8);
  FUN_0042dec0((void *)(param_1 + 0x3c8),param_1);
  FUN_0042d480(param_1 + 0x3c8);
  ExceptionList = local_c;
  return;
}
