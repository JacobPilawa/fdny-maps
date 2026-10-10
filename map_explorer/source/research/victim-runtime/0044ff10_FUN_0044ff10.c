/* Ghidra decompilation; names/types are provisional.
 * Entry: 0044ff10
 * Function: FUN_0044ff10
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0044ff10(void *this,char param_1)

{
  float *pfVar1;
  int iVar2;
  int *piVar3;
  undefined4 *puVar4;
  int iVar5;
  undefined4 *puVar6;
  char *pcVar7;
  int *piVar8;
  void *pvVar9;
  int *piStack_9c;
  char cStack_95;
  int *piStack_94;
  int local_90;
  int *apiStack_8c [2];
  int local_84;
  int *piStack_80;
  int iStack_7c;
  char *pcStack_78;
  char *pcStack_74;
  undefined4 uStack_70;
  undefined **ppuStack_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  undefined1 uStack_58;
  undefined1 auStack_48 [60];
  void *pvStack_c;
  undefined1 *puStack_8;
  int iStack_4;
  
  iStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d0078;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *(int *)((int)this + 0x3428) = *(int *)((int)this + 0x3428) + 1;
  pfVar1 = (float *)((int)this + 0x4c);
  *pfVar1 = *(float *)((int)this + 0x3400);
  *(undefined4 *)((int)this + 0x50) = *(undefined4 *)((int)this + 0x3404);
  *(undefined4 *)((int)this + 0x54) = *(undefined4 *)((int)this + 0x3408);
  if (*(void **)((int)this + 0x3534) != (void *)0x0) {
    FUN_0045c1a0(*(void **)((int)this + 0x3534),this);
    FUN_0044fce0('\0');
    *pfVar1 = *pfVar1 + _DAT_004e1698;
    iVar2 = (**(code **)(**(int **)((int)this + 0x3534) + 0x18))();
    *(float *)(iVar2 + 0x28) = *(float *)(iVar2 + 0x28) - _DAT_004e1698;
    *(undefined4 *)((int)this + 0x3534) = 0;
  }
  if ((*(int *)(*(int *)(DAT_00502aac + 0x6e4) + 0x2c) == 2) && (param_1 == '\0')) {
    local_90 = 0;
    FUN_00481c00(&local_84);
    iStack_4._0_1_ = 0;
    iStack_4._1_3_ = 0;
    piVar3 = piStack_80;
    while( true ) {
      if ((local_84 == 0) || (piVar3 == (int *)0x0)) {
        iVar2 = 0;
      }
      else {
        iVar2 = *piVar3;
      }
      if ((local_84 != 0) && (piVar3 != (int *)0x0)) {
        piVar3 = (int *)piVar3[1];
        piStack_80 = piVar3;
      }
      if (iVar2 == 0) break;
      if (*(int *)(iVar2 + 0x18) == 2) {
        if (DAT_00501918 == '\0') {
          DAT_00501918 = '\x01';
          DAT_00501908 = 2;
          DAT_0050190c = 0;
          _DAT_00501910 = 1;
          DAT_00501914 = 0;
        }
        piStack_9c = &DAT_00501908;
        FUN_00401d80(&piStack_9c,*(char **)(*(int *)(iVar2 + 0x150) + 0x14));
        puVar4 = (undefined4 *)FUN_0040c330(&piStack_9c,&piStack_94);
        iVar5 = __stricmp((char *)(*(int *)*puVar4 + 0xc),"herodown");
        if (piStack_9c != &DAT_00501908) {
          *piStack_9c = *piStack_9c + -1;
          if (*piStack_9c < 1) {
            FUN_004011c0(&piStack_9c);
          }
          else {
            piStack_9c = &DAT_00501908;
          }
        }
        piVar3 = piStack_80;
        if ((iVar5 == 0) && ((*(int *)(iVar2 + 0x120) == 4 || (*(int *)(iVar2 + 0x120) == 7)))) {
          local_90 = local_90 + 1;
        }
      }
    }
    if (local_90 < 3) {
      puVar6 = operator_new(0x34);
      puVar4 = puVar6;
      for (iVar2 = 0xd; iVar2 != 0; iVar2 = iVar2 + -1) {
        *puVar4 = 0;
        puVar4 = puVar4 + 1;
      }
      puVar6[1] = *(undefined4 *)((int)this + 0x4c);
      puVar6[2] = *(undefined4 *)((int)this + 0x50);
      puVar6[3] = *(undefined4 *)((int)this + 0x54);
      FUN_004054a0(&piStack_9c);
      iStack_4._0_1_ = 1;
      FUN_00402770(&piStack_9c,&DAT_004dd8e4);
      puVar4 = FUN_00402790(&piStack_94,"POFighter");
      iStack_4._0_1_ = 2;
      puVar4 = FUN_004040e0(puVar4,apiStack_8c,(int *)&piStack_9c);
      iStack_4._0_1_ = 3;
      FUN_00403b00(&piStack_9c,puVar4);
      iStack_4._0_1_ = 2;
      if (apiStack_8c[0] != &DAT_00501908) {
        *apiStack_8c[0] = *apiStack_8c[0] + -1;
        if (*apiStack_8c[0] < 1) {
          FUN_004011c0(apiStack_8c);
        }
        else {
          apiStack_8c[0] = &DAT_00501908;
        }
      }
      iStack_4._0_1_ = 1;
      if ((piStack_94 != &DAT_00501908) && (*piStack_94 = *piStack_94 + -1, *piStack_94 < 1)) {
        FUN_004011c0(&piStack_94);
      }
      piVar3 = piStack_9c;
      pcVar7 = operator_new(piStack_9c[1] + 1);
      puVar6[4] = pcVar7;
      FUN_00429cf0(&piStack_9c,pcVar7,piVar3[1] + 1);
      pvVar9 = (void *)0x0;
      *puVar6 = 1000;
      puVar6[5] = "herodown";
      puVar6[6] = 0;
      puVar6[7] = 0x41a00000;
      piStack_94 = operator_new(0x14);
      iStack_4._0_1_ = 4;
      if (piStack_94 != (int *)0x0) {
        pvVar9 = (void *)FUN_0049f230(piStack_94);
      }
      iStack_4._0_1_ = 1;
      piVar8 = FUN_0049b760(apiStack_8c,"RCS_VictimRescue",100);
      iStack_4._0_1_ = 5;
      piStack_94 = (int *)FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar8);
      cStack_95 = piStack_94 != (int *)0x0;
      iStack_4._0_1_ = 1;
      FUN_0049b6f0((int)apiStack_8c);
      if (cStack_95 != '\0') {
        *(int **)((int)pvVar9 + 0xc) = piStack_94;
      }
      *(undefined4 *)((int)pvVar9 + 8) = 0;
      FUN_0049d130(pvVar9,puVar6);
      FUN_0049d4c0(pvVar9);
      *(int *)((int)this + 0x3528) = *(int *)((int)this + 0x3528) + 1;
      FUN_00402790(apiStack_8c,"Rescues");
      iStack_4._0_1_ = 6;
      FUN_00486920((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)apiStack_8c,
                   *(undefined4 *)((int)this + 0x3528));
      iStack_4._0_1_ = 1;
      if ((apiStack_8c[0] != &DAT_00501908) &&
         (*apiStack_8c[0] = *apiStack_8c[0] + -1, *apiStack_8c[0] < 1)) {
        FUN_004011c0(apiStack_8c);
      }
      local_90 = local_90 + 1;
      iStack_4._0_1_ = 0;
      if ((piVar3 != &DAT_00501908) && (iVar2 = *piVar3, *piVar3 = iVar2 + -1, iVar2 + -1 < 1)) {
        FUN_004011c0(&piStack_9c);
      }
    }
    ppuStack_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    pcStack_60 = "rescue\\PlayerRescueEnt.cpp";
    uStack_5c = 0x3a7;
    uStack_58 = 0;
    iStack_4._0_1_ = 7;
    FUN_004a3a19(auStack_48,"Firefighters down = ");
    FUN_004a3f7a(auStack_48,local_90);
    FUN_00401360(&ppuStack_64,3);
    iStack_4 = (uint)iStack_4._1_3_ << 8;
    FUN_004012c0(&ppuStack_64);
    iStack_4 = 0xffffffff;
    if (local_84 != 0) {
      FUN_00429fe0((void *)(local_84 + 0x28),(int)&local_84);
    }
  }
  if (*(int *)((int)this + 0x357c) != 0) {
    if (*(int *)((int)this + 0x357c) == 4) {
      if (*(int *)((int)this + 0x3568) < 5) {
        piVar3 = &iStack_7c;
        for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        iStack_7c = 1;
        pcStack_78 = "Array index out of bounds";
        pcStack_74 = "..\\BluelAPI/Container/SArray.h";
        uStack_70 = 0x1cd;
        FUN_00422060(&iStack_7c);
      }
      pvVar9 = *(void **)(*(int *)((int)this + 0x3564) + 0x10);
      if (*(int *)((int)pvVar9 + 0xc68) == 3) {
        FUN_00463370(pvVar9,2);
      }
    }
    FUN_0044c360(this);
  }
  FUN_0044cd10(this);
  FUN_00456f70((void *)((int)this + 0x5f4));
  ExceptionList = pvStack_c;
  return;
}

