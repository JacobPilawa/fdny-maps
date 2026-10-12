
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_00494ad0(undefined4 param_1,char *param_2,undefined4 param_3,char *param_4)

{
  undefined *puVar1;
  undefined **ppuVar2;
  bool bVar3;
  undefined4 uVar4;
  undefined4 *puVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  void *this;
  int *extraout_ECX;
  int *extraout_ECX_00;
  int *piVar8;
  undefined **ppuStack_b8;
  char *pcStack_b4;
  undefined4 uStack_b0;
  undefined1 uStack_ac;
  undefined1 auStack_9c [64];
  undefined **ppuStack_5c;
  char *pcStack_58;
  undefined4 uStack_54;
  undefined1 uStack_50;
  undefined1 auStack_40 [52];
  void *pvStack_c;
  undefined1 *puStack_8;
  void *pvStack_4;
  
  pvStack_4 = (void *)0xffffffff;
  puStack_8 = &LAB_004d4311;
  pvStack_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar8 = &DAT_00501908;
  ExceptionList = &pvStack_c;
  FUN_00401d80(&stack0xffffff28,param_2);
  bVar3 = FUN_004097f0(piVar8);
  piVar8 = extraout_ECX;
  if (!bVar3) {
    ppuStack_b8 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_b4 = "ROC\\Renderer\\ROCRender.cpp";
    uStack_b0 = 0x23a;
    uStack_ac = 0;
    FUN_004a3a19(auStack_9c,"Texture bitmap not found: ");
    FUN_004a3a19(auStack_9c,param_2);
    FUN_00401360(&ppuStack_b8,2);
    FUN_004012c0(&ppuStack_b8);
    piVar8 = extraout_ECX_00;
  }
  if (param_4 != (char *)0x0) {
    FUN_00402790(&stack0xffffff2c,param_4);
    bVar3 = FUN_004097f0(piVar8);
    if (!bVar3) {
      ppuStack_5c = &PTR_FUN_004dc554;
      FUN_004a3118();
      pcStack_58 = "ROC\\Renderer\\ROCRender.cpp";
      uStack_54 = 0x23c;
      uStack_50 = 0;
      FUN_004a3a19(auStack_40,"Texture alpha bitmap not found: ");
      FUN_004a3a19(auStack_40,param_4);
      FUN_00401360(&ppuStack_5c,2);
      FUN_004012c0(&ppuStack_5c);
    }
  }
  ppuVar2 = ppuStack_b8;
  if (ppuStack_b8[0x1e] == (undefined *)0x0) {
    this = operator_new(0x10);
    if (this == (void *)0x0) {
      puVar5 = (undefined4 *)0x0;
    }
    else {
      puVar5 = FUN_00494a00(this,param_2,param_4,param_3,0);
    }
    puVar1 = ppuVar2[7];
    uVar4 = FUN_0043b6e0(ppuVar2 + 6,(int)(puVar1 + 1));
    *(undefined4 **)(ppuVar2[6] + (int)puVar1 * 4) = puVar5;
  }
  else {
    uVar4 = geVFile_OpenNewSystem(0,1);
    puVar5 = operator_new(8);
    puVar1 = ppuVar2[0x13];
    FUN_0043b6e0(ppuVar2 + 0x12,(int)(puVar1 + 1));
    *(undefined4 **)(ppuVar2[0x12] + (int)puVar1 * 4) = puVar5;
    uVar6 = geBitmap_CreateFromFile(uVar4);
    *puVar5 = uVar6;
    puVar5[1] = param_3;
    if (param_4 != (char *)0x0) {
      uVar6 = geVFile_OpenNewSystem(0,1);
      uVar7 = geBitmap_CreateFromFile(uVar6);
      geWorld_AddBitmap();
      geBitmap_SetAlpha(*puVar5,uVar7);
      geVFile_Close(uVar6);
    }
    geWorld_AddBitmap();
    uVar4 = geVFile_Close(uVar4);
  }
  ExceptionList = pvStack_4;
  return CONCAT31((int3)((uint)uVar4 >> 8),1);
}

