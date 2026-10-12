
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00495210(void *this,char *param_1)

{
  bool bVar1;
  undefined4 uVar2;
  undefined4 uVar3;
  int *piVar4;
  undefined **ppuStack_b8;
  char *pcStack_b4;
  undefined4 uStack_b0;
  undefined1 uStack_ac;
  undefined1 auStack_9c [60];
  undefined **ppuStack_60;
  char *pcStack_5c;
  undefined4 uStack_58;
  undefined1 uStack_54;
  undefined1 auStack_44 [56];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d43f3;
  pvStack_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar4 = &DAT_00501908;
  ExceptionList = &pvStack_c;
  FUN_00401d80(&stack0xffffff2c,param_1);
  bVar1 = FUN_004097f0(piVar4);
  if (!bVar1) {
    ppuStack_b8 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_b4 = "ROC\\Renderer\\ROCRender.cpp";
    uStack_b0 = 0x52;
    uStack_ac = 0;
    FUN_004a3a19(auStack_9c,"BSP file not found: ");
    FUN_004a3a19(auStack_9c,param_1);
    FUN_00401360(&ppuStack_b8,2);
    FUN_004012c0(&ppuStack_b8);
  }
  if (*(int *)(*(int *)this + 0x70) != 0) {
    FUN_004275a0(*(int *)(*(int *)this + 0x70));
  }
  uVar2 = geVFile_OpenNewSystem(0,1,param_1);
  uVar3 = geWorld_Create();
  *(undefined4 *)((int)this + 0x80) = uVar3;
  geVFile_Close(uVar2);
  if (*(int *)((int)this + 0x80) == 0) {
    ppuStack_60 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_5c = "ROC\\Renderer\\ROCRender.cpp";
    uStack_58 = 0x5b;
    uStack_54 = 0;
    FUN_004a3a19(auStack_44,"Level load failed: ");
    FUN_004a3a19(auStack_44,param_1);
    FUN_00401360(&ppuStack_60,2);
    FUN_004012c0(&ppuStack_60);
  }
  else {
    geEngine_AddWorld();
    piVar4 = (int *)((int)this + 0xb4);
    if (*(char *)((int)this + 0x160) == '\0') {
      piVar4 = (int *)((int)this + 8);
    }
    FUN_00494f00(piVar4);
  }
  ExceptionList = puStack_8;
  return;
}

