/* Read-only Ghidra 12.0.4 decompilation from FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. Captured 2026-10-12. */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __cdecl FUN_004799e0(uint param_1,undefined4 param_2,int *param_3,char param_4)

{
  byte bVar1;
  int iVar2;
  bool bVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  undefined1 *puVar6;
  int *piVar7;
  int *piVar8;
  int *local_d8;
  int *local_d4;
  int *piStack_d0;
  int *local_cc;
  int *piStack_c8;
  int *piStack_c4;
  int iStack_c0;
  int iStack_bc;
  byte abStack_b8 [4];
  undefined4 auStack_b4 [13];
  undefined **appuStack_80 [9];
  undefined1 auStack_5c [48];
  undefined4 uStack_2c;
  undefined4 uStack_28;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined1 uStack_14;
  undefined4 uStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  void *local_4;
  
  puStack_8 = &LAB_004d2ffb;
  pvStack_c = ExceptionList;
  local_4 = (void *)0x0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_d4 = &DAT_00501908;
  ExceptionList = &pvStack_c;
  FUN_00401d80(&local_d4,"Data\\Scripts\\");
  local_4._0_1_ = 1;
  FUN_004040e0(&local_d4,&local_d8,(int *)&param_1);
  local_4 = (void *)CONCAT31(local_4._1_3_,3);
  if ((local_d4 != &DAT_00501908) && (iVar2 = *local_d4, *local_d4 = iVar2 + -1, iVar2 + -1 < 1)) {
    FUN_004011c0(&local_d4);
  }
  local_cc = (int *)&stack0xffffff14;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar7 = &DAT_00501908;
  if ((local_d8 != &DAT_00501908) && (piVar7 = local_d8, local_d8 != &DAT_00501908)) {
    *local_d8 = *local_d8 + 1;
  }
  bVar3 = FUN_004097f0(piVar7);
  if (bVar3) {
LAB_00479c58:
    FUN_004a3b54();
    param_1 = CONCAT31(param_1._1_3_,6);
    FUN_004a3e23(&iStack_c0,(LPCSTR)(piStack_d0 + 3),0x20,DAT_004e9c24);
    if (((abStack_b8[*(int *)(iStack_c0 + 4)] & 6) == 0) &&
       (puVar6 = (undefined1 *)0xffffffff,
       *(int *)(*(int *)((int)&iStack_bc + *(int *)(iStack_c0 + 4)) + 0x30) != -1)) {
      FUN_0046f930(appuStack_80);
      appuStack_80[0] = &PTR_FUN_004e3354;
      uStack_2c = 0;
      uStack_28 = 0;
      uStack_24 = 0;
      uStack_20 = 0;
      uStack_1c = 0;
      uStack_18 = 0;
      uStack_14 = 0;
      uStack_10 = 0;
      puStack_8 = (undefined1 *)0x0;
      param_1 = CONCAT31(param_1._1_3_,7);
      FUN_00403b00(auStack_5c,&param_3);
      FUN_004054a0(&piStack_c4);
      bVar1 = abStack_b8[*(int *)(iStack_c0 + 4)];
      param_1 = CONCAT31(param_1._1_3_,8);
      piVar7 = piStack_c4;
      do {
        if ((bVar1 & 1) != 0) {
          FUN_004a3e5e(&iStack_c0);
          param_1 = CONCAT31(param_1._1_3_,7);
          if ((piVar7 != &DAT_00501908) && (iVar2 = *piVar7, *piVar7 = iVar2 + -1, iVar2 + -1 < 1))
          {
            FUN_004011c0(&piStack_c4);
          }
          param_1 = CONCAT31(param_1._1_3_,6);
          FUN_0043f130((int)appuStack_80);
          param_1 = CONCAT31(param_1._1_3_,3);
          FUN_004a3da7((int)auStack_b4);
          FUN_004a3426(auStack_b4);
          param_1 = param_1 & 0xffffff00;
          if (piStack_d0 != &DAT_00501908) {
            *piStack_d0 = *piStack_d0 + -1;
            if (*piStack_d0 < 1) {
              FUN_004011c0(&piStack_d0);
            }
            else {
              piStack_d0 = &DAT_00501908;
            }
          }
          param_1 = 0xffffffff;
          if ((param_3 != &DAT_00501908) &&
             (iVar2 = *param_3, *param_3 = iVar2 + -1, iVar2 + -1 < 1)) {
            param_3 = (int *)FUN_004011c0(&param_3);
          }
          ExceptionList = local_4;
          return CONCAT31((int3)((uint)param_3 >> 8),1);
        }
        puVar6 = puVar6 + 1;
        FUN_00422670(&piStack_c4,&iStack_c0);
        piVar7 = piStack_c4;
        if ((char)piStack_c4[3] != ';') {
          if (param_4 != '\0') {
            if (DAT_00501918 == '\0') {
              DAT_00501918 = '\x01';
              DAT_00501908 = 2;
              DAT_0050190c = 0;
              _DAT_00501910 = 1;
              DAT_00501914 = 0;
            }
            local_cc = &DAT_00501908;
            if (piStack_c4 != &DAT_00501908) {
              local_cc = piStack_c4;
              *piStack_c4 = *piStack_c4 + 1;
            }
            param_1 = CONCAT31(param_1._1_3_,9);
            puVar4 = FUN_004172c0(&local_cc,&piStack_c8,(int *)0x0,(char *)0x0,(int *)0x0);
            param_1 = CONCAT31(param_1._1_3_,10);
            if (local_cc != (int *)*puVar4) {
              if ((local_cc != &DAT_00501908) &&
                 (iVar2 = *local_cc, *local_cc = iVar2 + -1, iVar2 + -1 < 1)) {
                FUN_004011c0(&local_cc);
              }
              local_cc = (int *)*puVar4;
              if (local_cc != &DAT_00501908) {
                *local_cc = *local_cc + 1;
              }
            }
            piVar8 = local_cc;
            param_1 = CONCAT31(param_1._1_3_,9);
            if (piStack_c8 != &DAT_00501908) {
              *piStack_c8 = *piStack_c8 + -1;
              if (*piStack_c8 < 1) {
                FUN_004011c0(&piStack_c8);
              }
              else {
                piStack_c8 = &DAT_00501908;
              }
            }
            uVar5 = FUN_00401c30(&local_cc,&DAT_004e3364);
            if (((char)uVar5 != '\0') &&
               (uVar5 = FUN_00401c30(&local_cc,&DAT_004e335c), (char)uVar5 != '\0')) {
              param_1 = CONCAT31(param_1._1_3_,8);
              if ((piVar8 != &DAT_00501908) &&
                 (iVar2 = *piVar8, *piVar8 = iVar2 + -1, iVar2 + -1 < 1)) {
                FUN_004011c0(&local_cc);
              }
              goto LAB_00479e8a;
            }
            param_1 = CONCAT31(param_1._1_3_,8);
            if ((piVar8 != &DAT_00501908) && (iVar2 = *piVar8, *piVar8 = iVar2 + -1, iVar2 + -1 < 1)
               ) {
              FUN_004011c0(&local_cc);
            }
          }
          local_cc = (int *)&stack0xffffff18;
          if (DAT_00501918 == '\0') {
            DAT_00501918 = '\x01';
            DAT_00501908 = 2;
            DAT_0050190c = 0;
            _DAT_00501910 = 1;
            DAT_00501914 = 0;
          }
          piVar8 = &DAT_00501908;
          if (piVar7 != &DAT_00501908) {
            *piVar7 = *piVar7 + 1;
            piVar8 = piVar7;
          }
          FUN_00475310(appuStack_80,piVar8,puVar6);
        }
LAB_00479e8a:
        bVar1 = abStack_b8[*(int *)(iStack_c0 + 4)];
      } while( true );
    }
    param_1 = CONCAT31(param_1._1_3_,3);
    FUN_004a3da7((int)auStack_b4);
    FUN_004a3426(auStack_b4);
    if (piStack_d0 == &DAT_00501908) goto LAB_0047a027;
    *piStack_d0 = *piStack_d0 + -1;
    if (0 < *piStack_d0) {
      piStack_d0 = &DAT_00501908;
      goto LAB_0047a027;
    }
  }
  else {
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_cc = &DAT_00501908;
    FUN_00401d80(&local_cc,"Data\\Maps\\");
    puVar4 = FUN_004040e0(&local_cc,&piStack_d0,&param_2);
    if (local_d4 != (int *)*puVar4) {
      if ((local_d4 != &DAT_00501908) && (*local_d4 = *local_d4 + -1, *local_d4 < 1)) {
        FUN_004011c0(&local_d4);
      }
      local_d4 = (int *)*puVar4;
      if (local_d4 != &DAT_00501908) {
        *local_d4 = *local_d4 + 1;
      }
    }
    if (piStack_d0 != &DAT_00501908) {
      *piStack_d0 = *piStack_d0 + -1;
      if (*piStack_d0 < 1) {
        FUN_004011c0(&piStack_d0);
      }
      else {
        piStack_d0 = &DAT_00501908;
      }
    }
    if ((local_cc != &DAT_00501908) && (iVar2 = *local_cc, *local_cc = iVar2 + -1, iVar2 + -1 < 1))
    {
      FUN_004011c0(&local_cc);
    }
    piStack_c8 = (int *)&stack0xffffff18;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar7 = &DAT_00501908;
    if ((local_d4 != &DAT_00501908) && (piVar7 = local_d4, local_d4 != &DAT_00501908)) {
      *local_d4 = *local_d4 + 1;
    }
    bVar3 = FUN_004097f0(piVar7);
    if (bVar3) goto LAB_00479c58;
    if (piStack_d0 == &DAT_00501908) goto LAB_0047a027;
    *piStack_d0 = *piStack_d0 + -1;
    if (0 < *piStack_d0) {
      piStack_d0 = &DAT_00501908;
      goto LAB_0047a027;
    }
  }
  param_1 = param_1 & 0xffffff00;
  FUN_004011c0(&piStack_d0);
LAB_0047a027:
  param_1 = 0xffffffff;
  if ((param_3 != &DAT_00501908) && (iVar2 = *param_3, *param_3 = iVar2 + -1, iVar2 + -1 < 1)) {
    param_3 = (int *)FUN_004011c0(&param_3);
  }
  ExceptionList = local_4;
  return (uint)param_3 & 0xffffff00;
}

