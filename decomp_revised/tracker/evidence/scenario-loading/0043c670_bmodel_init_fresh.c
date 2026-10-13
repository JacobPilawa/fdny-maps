
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall InitializeBModelEntity(void *param_1)

{
  float *pfVar1;
  char *pcVar2;
  undefined ***pppuVar3;
  bool bVar4;
  undefined4 *puVar5;
  int iVar6;
  undefined **ppuVar7;
  undefined ***pppuVar8;
  int *piVar9;
  undefined ***pppuStack_7c;
  undefined1 *apuStack_78 [2];
  float afStack_70 [3];
  undefined **ppuStack_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  uint uStack_58;
  undefined4 uStack_54;
  undefined4 uStack_50;
  undefined4 uStack_4c;
  undefined4 uStack_48;
  undefined4 uStack_44;
  undefined4 uStack_40;
  undefined4 uStack_3c;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004cea98;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *(undefined4 *)((int)param_1 + 0x1e8) = 0x447a0000;
  *(undefined4 *)((int)param_1 + 0x1ec) = 0;
  *(undefined4 *)((int)param_1 + 600) = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  pppuStack_7c = (undefined ***)&DAT_00501908;
  FUN_00401d80(&pppuStack_7c,"ThrowObjectEnd");
  uStack_4 = 0;
  FUN_004a12f0((void *)((int)param_1 + 0x124),&pppuStack_7c,0,2);
  uStack_4 = 0xffffffff;
  if ((pppuStack_7c != (undefined ***)&DAT_00501908) &&
     (*pppuStack_7c = (undefined **)((int)*pppuStack_7c + -1), (int)*pppuStack_7c < 1)) {
    FUN_004011c0(&pppuStack_7c);
  }
  iVar6 = *(int *)((int)param_1 + 0xec);
  *(undefined1 *)((int)param_1 + 0x23c) = 0;
  pcStack_60 = *(char **)(iVar6 + 0xc);
  uStack_5c = *(undefined4 *)(iVar6 + 0x10);
  ppuVar7 = *(undefined ***)(iVar6 + 8);
  uStack_58 = *(uint *)(iVar6 + 0x14);
  uStack_54 = *(undefined4 *)(iVar6 + 0x18);
  uStack_50 = *(undefined4 *)(iVar6 + 0x1c);
  uStack_4c = *(undefined4 *)(iVar6 + 0x20);
  uStack_48 = *(undefined4 *)(iVar6 + 0x24);
  uStack_44 = *(undefined4 *)(iVar6 + 0x28);
  uStack_40 = *(undefined4 *)(iVar6 + 0x2c);
  uStack_3c = 0;
  if (ppuVar7 != (undefined **)0x0) {
    pppuStack_7c = &ppuStack_64;
    ppuStack_64 = ppuVar7;
    do {
      iVar6 = 0;
      if (0 < *(int *)((int)param_1 + 0x1c4)) {
        piVar9 = *(int **)((int)param_1 + 0x1c0);
        do {
          if (*piVar9 == 0) goto LAB_0043c7c1;
          iVar6 = iVar6 + 1;
          piVar9 = piVar9 + 1;
        } while (iVar6 < *(int *)((int)param_1 + 0x1c4));
      }
      FUN_0043b6e0((void *)((int)param_1 + 0x1c0),iVar6 + 1);
LAB_0043c7c1:
      *(undefined ***)(*(int *)((int)param_1 + 0x1c0) + iVar6 * 4) = ppuVar7;
      iVar6 = geWorld_ModelGetMotion();
      if (iVar6 != 0) {
        pfVar1 = (float *)((int)param_1 + 0x1e0);
        geMotion_GetTimeExtents(iVar6,pfVar1);
        if (*pfVar1 < *(float *)((int)param_1 + 0x1e8)) {
          *(float *)((int)param_1 + 0x1e8) = *pfVar1;
        }
        if (*(float *)((int)param_1 + 0x1ec) < *(float *)((int)param_1 + 0x1e4)) {
          *(float *)((int)param_1 + 0x1ec) = *(float *)((int)param_1 + 0x1e4);
        }
      }
      ppuVar7 = pppuStack_7c[1];
      pppuStack_7c = pppuStack_7c + 1;
    } while (ppuVar7 != (undefined **)0x0);
  }
  ppuStack_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  pcStack_60 = "sharedents\\BModelEnt.cpp";
  uStack_5c = 0x45;
  uStack_58 = uStack_58 & 0xffffff00;
  uStack_4 = 1;
  FUN_004a3a19(&uStack_48,"Spawning BModel ");
  FUN_004a3a19(&uStack_48,*(char **)(*(int *)((int)param_1 + 0xec) + 4));
  FUN_004a3a19(&uStack_48,"");
  FUN_00401360(&ppuStack_64,3);
  uStack_4 = 0xffffffff;
  FUN_004012c0(&ppuStack_64);
  if (0 < *(int *)((int)param_1 + 0x1c4)) {
    puVar5 = (undefined4 *)FUN_0049d350(param_1,apuStack_78);
    pppuStack_7c = (undefined ***)&stack0xffffff6c;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    ppuVar7 = (undefined **)&DAT_00501908;
    FUN_00401d80(&stack0xffffff6c,*(char **)(*(int *)((int)param_1 + 0xec) + 4));
    InitializeAndRegisterScriptActor((void *)((int)param_1 + 0x24),(int *)ppuVar7,puVar5);
    *(undefined4 *)((int)param_1 + 0x1d8) = *(undefined4 *)((int)param_1 + 0x1e8);
    *(undefined4 *)((int)param_1 + 0x1dc) = 0x3f800000;
    FUN_0043b7a0((int)param_1);
    puVar5 = (undefined4 *)FUN_0043bc80(param_1,afStack_70);
    *(undefined4 *)((int)param_1 + 0x224) = *puVar5;
    *(undefined4 *)((int)param_1 + 0x228) = puVar5[1];
    *(undefined4 *)((int)param_1 + 0x22c) = puVar5[2];
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x3c);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 3;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x40);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 4;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x44);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 5;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x48);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 6;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x4c);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 7;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x50);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 8;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x54);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 9;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x58);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 10;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x5c);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 0xb;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    pcVar2 = *(char **)(*(int *)((int)param_1 + 0xec) + 0x60);
    if (pcVar2 != (char *)0x0) {
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      pppuStack_7c = (undefined ***)&DAT_00501908;
      FUN_00401d80(&pppuStack_7c,pcVar2);
      pppuVar3 = pppuStack_7c;
      uStack_4 = 0xc;
      if (pppuStack_7c[1] != (undefined **)0x0) {
        apuStack_78[0] = &stack0xffffff6c;
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        pppuVar8 = (undefined ***)&DAT_00501908;
        if (pppuStack_7c != (undefined ***)&DAT_00501908) {
          *pppuStack_7c = (undefined **)((int)*pppuStack_7c + 1);
          pppuVar8 = pppuStack_7c;
        }
        FUN_00475310((void *)((int)param_1 + 0x24),(int *)pppuVar8,(undefined1 *)0x0);
      }
      uStack_4 = 0xffffffff;
      if ((pppuVar3 != (undefined ***)&DAT_00501908) &&
         (ppuVar7 = *pppuVar3, *pppuVar3 = (undefined **)((int)ppuVar7 + -1), (int)ppuVar7 + -1 < 1)
         ) {
        FUN_004011c0(&pppuStack_7c);
      }
    }
    if (*(int *)((int)param_1 + 600) == 0) {
      apuStack_78[0] = &stack0xffffff70;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      piVar9 = &DAT_00501908;
      FUN_00401d80(&stack0xffffff70,"use");
      bVar4 = FUN_0046e3d0((void *)((int)param_1 + 0x24),piVar9);
      if (bVar4) {
        *(undefined4 *)((int)param_1 + 600) = 2;
      }
    }
    if (*(int *)(*(int *)((int)param_1 + 0xec) + 0x38) != 0) {
      FUN_0043be00((int)param_1);
    }
    ExceptionList = pvStack_c;
    return;
  }
  ppuStack_64 = &PTR_FUN_004dc554;
  FUN_004a3118();
  pcStack_60 = "sharedents\\BModelEnt.cpp";
  uStack_5c = 0x48;
  uStack_58 = uStack_58 & 0xffffff00;
  uStack_4 = 2;
  FUN_004a3a19(&uStack_48,"BModel ");
  FUN_004a3a19(&uStack_48,*(char **)(*(int *)((int)param_1 + 0xec) + 4));
  FUN_004a3a19(&uStack_48," has no animation!");
  FUN_00401360(&ppuStack_64,0);
  uStack_4 = 0xffffffff;
  FUN_004012c0(&ppuStack_64);
  ExceptionList = pvStack_c;
  return;
}


