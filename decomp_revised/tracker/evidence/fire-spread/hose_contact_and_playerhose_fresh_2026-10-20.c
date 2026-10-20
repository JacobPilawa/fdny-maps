/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0044cab0 FUN_0044cab0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __thiscall FUN_0044cab0(void *this,int param_1)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int *piVar3;
  int iVar4;
  int *piVar5;
  int local_94;
  char *local_90;
  char *local_8c;
  undefined4 local_88;
  int iStack_7c;
  char *pcStack_78;
  char *pcStack_74;
  undefined4 uStack_70;
  undefined **ppuStack_64;
  char *pcStack_60;
  undefined4 uStack_5c;
  undefined1 uStack_58;
  undefined1 auStack_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  int *local_4;
  
  local_4 = (int *)0xffffffff;
  puStack_8 = &LAB_004cfb86;
  local_c = ExceptionList;
  if (*(int *)((int)this + 0x144) == 0) {
    uVar1 = 0;
  }
  else {
    iVar4 = *(int *)((int)this + 0x144) + -0xd4;
    ExceptionList = &local_c;
    uVar1 = FUN_00443610(iVar4);
    if ((char)uVar1 != '\0') {
      FUN_00443640(iVar4);
      if (*(int *)((int)this + 0x3568) < 5) {
        piVar3 = &local_94;
        for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        local_94 = 1;
        local_90 = "Array index out of bounds";
        local_8c = "..\\BluelAPI/Container/SArray.h";
        local_88 = 0x1cd;
        FUN_00422060(&local_94);
      }
      piVar3 = *(int **)(*(int *)((int)this + 0x3564) + 0x10);
      if (piVar3 == (int *)0x0) {
        puVar2 = operator_new(0xd54);
        if (puVar2 == (undefined4 *)0x0) {
          piVar3 = (int *)0x0;
        }
        else {
          local_4 = piVar3;
          piVar3 = FUN_004634f0(puVar2);
        }
        local_4 = (int *)0xffffffff;
        (**(code **)(*piVar3 + 8))(this);
        if (*(int *)((int)this + 0x3568) < 5) {
          piVar5 = &iStack_7c;
          for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
            *piVar5 = 0;
            piVar5 = piVar5 + 1;
          }
          iStack_7c = 1;
          pcStack_78 = "Array index out of bounds";
          pcStack_74 = "..\\BluelAPI/Container/SArray.h";
          uStack_70 = 0x1cd;
          FUN_00422060(&iStack_7c);
        }
        *(int **)(*(int *)((int)this + 0x3564) + 0x10) = piVar3;
        FUN_00482780(piVar3 + 0x31c);
        FUN_004571a0((void *)((int)this + 0x5f4),(int *)0x6,(undefined1 *)0x4);
      }
      else if ((float)piVar3[0x32c] * (float)piVar3[0x330] * (float)_DAT_004e1858 < (float)param_1)
      {
        FUN_00482780(piVar3 + 0x31c);
      }
      if ((piVar3[0x31a] == 3) || (piVar3[0x31a] == 2)) {
        ppuStack_64 = &PTR_FUN_004dc554;
        FUN_004a3118();
        pcStack_60 = "rescue\\PlayerRescueEnt.cpp";
        uStack_5c = 0x63a;
        uStack_58 = 0;
        local_4 = (int *)0x1;
        FUN_004a3a19(auStack_48,"Already have hose");
        FUN_00401360(&ppuStack_64,0);
        local_4 = (int *)0xffffffff;
        uVar1 = FUN_004012c0(&ppuStack_64);
      }
      else {
        FUN_00463370(piVar3,2);
        uVar1 = FUN_004a11a0((void *)((int)this + 0x49c),(undefined1 *)0x0);
      }
    }
    uVar1 = CONCAT31((int3)((uint)uVar1 >> 8),1);
  }
  ExceptionList = local_c;
  return uVar1;
}



/* 0x004634f0 FUN_004634f0 */

undefined4 * __fastcall FUN_004634f0(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d17cc;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0045f7e0(param_1);
  param_1[0x7f] = &PTR_LAB_004e0b7c;
  *(undefined1 *)(param_1 + 0x80) = 0;
  local_4._0_1_ = 1;
  local_4._1_3_ = 0;
  *param_1 = &PTR_FUN_004e2998;
  param_1[0x7f] = &PTR_LAB_004e2988;
  FUN_004a13a0(param_1 + 0x81);
  local_4._0_1_ = 2;
  FUN_004a13a0(param_1 + 0x8a);
  local_4._0_1_ = 3;
  FUN_004a13a0(param_1 + 0x93);
  local_4._0_1_ = 4;
  FUN_0047dba0(param_1 + 0x9c);
  local_4._0_1_ = 5;
  FUN_0047dba0(param_1 + 0xa3);
  local_4._0_1_ = 6;
  FUN_004804f0(param_1 + 0xaa);
  local_4._0_1_ = 7;
  FUN_004804f0(param_1 + 0xcb);
  local_4._0_1_ = 8;
  FUN_0047dba0(param_1 + 0xed);
  local_4._0_1_ = 9;
  FUN_00480260(param_1 + 0xf4);
  local_4._0_1_ = 10;
  _eh_vector_constructor_iterator_(param_1 + 0x102,0x10,0x20,FUN_0044c060,FUN_0043f660);
  local_4._0_1_ = 0xb;
  FUN_0046dc80((int)(param_1 + 0x182));
  local_4._0_1_ = 0xc;
  FUN_0046b9d0(param_1 + 0x292);
  local_4._0_1_ = 0xd;
  FUN_0048c120(param_1 + 0x2ce);
  param_1[0x310] = 0;
  param_1[0x311] = 0;
  param_1[0x312] = 0;
  param_1[0x313] = 0;
  param_1[0x314] = 0;
  param_1[0x315] = 0;
  local_4 = CONCAT31(local_4._1_3_,0xe);
  param_1[0x317] = 0;
  param_1[0x318] = 0;
  param_1[0x319] = 0;
  FUN_00482850(param_1 + 0x31c);
  ExceptionList = local_c;
  return param_1;
}



/* 0x00463700 FUN_00463700 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00463700(void *this,int *param_1)

{
  void *pvVar1;
  void *this_00;
  int *piVar2;
  undefined4 uVar3;
  int *piVar4;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d17f8;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  FUN_0045f660(this,param_1,3);
  FUN_0042d280((void *)((int)this + 0x8c),&local_24,0x3f59999a,0x3f59999a,0x3f59999a);
  local_24 = 0xc1200000;
  *(undefined4 *)((int)this + 0x80) = 0xc1200000;
  local_20 = 0xc1700000;
  *(undefined4 *)((int)this + 0x84) = 0xc1700000;
  local_1c = 0x41a80000;
  *(undefined4 *)((int)this + 0x88) = 0x41a80000;
  piVar4 = (int *)0x0;
  *(undefined4 *)((int)this + 0xc68) = 0;
  uVar3 = 0xf;
  piVar2 = (int *)(**(code **)(**(int **)((int)this + 0x54) + 0x18))();
  FUN_0046d6b0((void *)((int)this + 0x608),piVar2,uVar3,piVar4);
  *(undefined4 *)((int)this + 0x9c4) = 0;
  *(undefined4 *)((int)this + 0x978) = 0;
  *(undefined4 *)((int)this + 0x97c) = 0x3f4ccccd;
  *(undefined4 *)((int)this + 0x980) = 0;
  *(undefined4 *)((int)this + 0x984) = 0x42c80000;
  *(undefined4 *)((int)this + 0x988) = 0x42f00000;
  *(undefined4 *)((int)this + 0x98c) = 0x42f00000;
  *(undefined4 *)((int)this + 0x990) = 0;
  *(undefined4 *)((int)this + 0x994) = 0x41200000;
  *(undefined4 *)((int)this + 0x998) = 0x451c4000;
  *(undefined4 *)((int)this + 0x99c) = 0x3fc00000;
  *(undefined4 *)((int)this + 0x9a0) = 0x3fe00000;
  *(undefined4 *)((int)this + 0x9a4) = 0x43960000;
  local_24 = 0x43700000;
  *(undefined4 *)((int)this + 0x9a8) = 0x43700000;
  local_20 = 0x43700000;
  *(undefined4 *)((int)this + 0x9ac) = 0x43700000;
  *(undefined4 *)((int)this + 0x9b0) = 0;
  *(undefined4 *)((int)this + 0x9b4) = 0x42200000;
  local_1c = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"ExtStart");
  uStack_4 = 0;
  FUN_004a12f0((void *)((int)this + 0x204),&param_1,0,2);
  uStack_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"HoseSpray");
  uStack_4 = 1;
  FUN_004a12f0((void *)((int)this + 0x228),&param_1,0,2);
  uStack_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"ExtEnd");
  uStack_4 = 2;
  FUN_004a12f0((void *)((int)this + 0x24c),&param_1,0,2);
  uStack_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  pvVar1 = (void *)((int)this + 0x270);
  *(undefined1 *)((int)this + 0xc58) = 0;
  local_24 = 0x3f800000;
  local_20 = 0x3f800000;
  local_1c = 0x3f800000;
  FUN_0047db10(pvVar1,1,0x3f266666,0x3f000000,0x3e800000,&local_24);
  local_24 = 0x3f800000;
  local_20 = 0x3f800000;
  local_1c = 0x3f800000;
  FUN_0047db10(pvVar1,1,0,0,0x40400000,&local_24);
  *(uint *)((int)this + 0x288) = *(uint *)((int)this + 0x288) | 8;
  this_00 = (void *)((int)this + 0x28c);
  local_24 = 0x3f400000;
  local_20 = 0x3f400000;
  local_1c = 0x3f800000;
  FUN_0047db10(this_00,2,0x3f59999a,0x3f800000,0x3d99999a,&local_24);
  local_24 = 0x3f800000;
  local_20 = 0x3f800000;
  local_1c = 0x3f800000;
  FUN_0047db10(this_00,2,0,0,0x3e800000,&local_24);
  *(uint *)((int)this + 0x2a4) = *(uint *)((int)this + 0x2a4) | 8;
  FUN_0047fe70((void *)((int)this + 0x2a8),*(void **)(DAT_00503a78 + 4),pvVar1,'\x01');
  FUN_0047fe70((void *)((int)this + 0x32c),*(void **)(DAT_00503a78 + 4),this_00,'\x01');
  *(undefined4 *)((int)this + 0x2d8) = 10;
  *(undefined4 *)((int)this + 0x35c) = 0x14;
  *(undefined4 *)((int)this + 0x2e0) = 1;
  *(undefined4 *)((int)this + 0x2e4) = 1;
  *(undefined4 *)((int)this + 0x2e8) = 0x3e800000;
  *(undefined4 *)((int)this + 0x2ec) = 0x3f000000;
  FUN_0042d280((void *)((int)this + 0x2f0),&local_24,0,0,0x43960000);
  FUN_0042d280((void *)((int)this + 0x2fc),&local_24,0,0,0x43fa0000);
  *(undefined4 *)((int)this + 0x308) = 0x3f400000;
  *(undefined4 *)((int)this + 0x30c) = 0x3f800000;
  *(undefined4 *)((int)this + 0x310) = 0x3f4ccccd;
  *(undefined4 *)((int)this + 0x314) = 0x3f99999a;
  *(undefined4 *)((int)this + 0x318) = 0x3f400000;
  *(undefined4 *)((int)this + 0x31c) = 0x30;
  FUN_0042d280((void *)((int)this + 800),&local_24,0,0xc1a00000,0);
  *(undefined4 *)((int)this + 0x364) = 1;
  *(undefined4 *)((int)this + 0x368) = 4;
  *(undefined4 *)((int)this + 0x36c) = 0x3dcccccd;
  *(undefined4 *)((int)this + 0x370) = 0x3dcccccd;
  FUN_0042d280((void *)((int)this + 0x374),&local_24,0xc2480000,0xc2480000,0x43480000);
  FUN_0042d280((void *)((int)this + 0x380),&local_24,0x42480000,0x42480000,0x43fa0000);
  *(undefined4 *)((int)this + 0x38c) = 0x3f400000;
  *(undefined4 *)((int)this + 0x390) = 0x3f800000;
  *(undefined4 *)((int)this + 0x394) = 0x3f4ccccd;
  *(undefined4 *)((int)this + 0x398) = 0x3f99999a;
  *(undefined4 *)((int)this + 0x39c) = 0x3f000000;
  *(undefined4 *)((int)this + 0x3a0) = 0x30;
  FUN_0042d280((void *)((int)this + 0x3a4),&local_24,0,0xc2960000,0);
  pvVar1 = (void *)((int)this + 0x3b4);
  local_24 = 0x3f800000;
  local_20 = 0x3f800000;
  local_1c = 0x3f800000;
  FUN_0047db10(pvVar1,0xe,0x40000000,0x3f400000,0x40400000,&local_24);
  uStack_18 = 0x3f800000;
  uStack_14 = 0x3f800000;
  uStack_10 = 0x3f800000;
  FUN_0047db10(pvVar1,0xe,0,0,0x40a00000,&uStack_18);
  *(uint *)((int)this + 0x3cc) = *(uint *)((int)this + 0x3cc) | 8;
  FUN_0047fe70((void *)((int)this + 0x3d0),*(void **)(DAT_00503a78 + 4),pvVar1,'\x01');
  *(undefined4 *)((int)this + 0x400) = 10;
  *(undefined4 *)((int)this + 0x3b0) = 0;
  FUN_0048abb0((void *)((int)this + 0xb38),1,'\x01',-1);
  FUN_00469520((void *)((int)this + 0xa48),'\0');
  *(undefined4 *)((int)this + 0xa8c) = 0x41a00000;
  *(undefined4 *)((int)this + 0xa90) = 0x41f00000;
  FUN_004827b0((int)this + 0xc70);
  FUN_00459050((int)this + 0x1fc);
  ExceptionList = pvStack_c;
  return;
}



/* 0x004632d0 FUN_004632d0 */

void __fastcall FUN_004632d0(int param_1)

{
  FUN_004a11d0((int *)(param_1 + 0x228));
  *(undefined1 *)(param_1 + 0xc58) = 1;
  *(int *)(param_1 + 0x3b0) = DAT_00502afc + 100;
  return;
}



/* 0x00463400 FUN_00463400 */

void __thiscall FUN_00463400(void *this,char param_1)

{
  FUN_0045f620(this,param_1);
  FUN_0040cb80((void *)((int)this + 0xb38),param_1);
  if (param_1 == '\0') {
    if (*(int *)((int)this + 0xc68) == 3) {
      *(undefined4 *)((int)this + 0xc68) = 1;
      FUN_004571a0((void *)(*(int *)(DAT_00503a78 + 0x30) + 0x5f4),(int *)0x5,(undefined1 *)0x0);
    }
    (**(code **)(*(int *)this + 0x1c))();
    FUN_0046c570((void *)((int)this + 0x608));
  }
  return;
}



/* 0x00466d60 FUN_00466d60 */

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



/* 0x00466780 FUN_00466780 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00466780(int *param_1)

{
  bool bVar1;
  char cVar2;
  void *pvVar3;
  undefined4 uVar4;
  int iVar5;
  float *pfVar6;
  float10 fVar7;
  float10 fVar8;
  float *pfVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  int **ppiVar13;
  float fVar14;
  int *local_cc;
  int *piStack_c8;
  float afStack_c4 [6];
  undefined1 auStack_ac [16];
  undefined4 uStack_9c;
  undefined4 uStack_98;
  undefined4 uStack_94;
  float afStack_68 [3];
  undefined4 auStack_5c [3];
  undefined1 auStack_50 [68];
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1c1c;
  pvStack_c = ExceptionList;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_cc = &DAT_00501908;
  ExceptionList = &pvStack_c;
  FUN_00401d80(&local_cc,"MOUSE_LB");
  ppiVar13 = &local_cc;
  local_4 = 0;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar4 = FUN_00438480(pvVar3,(int *)ppiVar13);
  if ((((char)uVar4 == '\0') || (*(char *)((int)param_1 + 0x1489) != '\0')) || (param_1[99] == 6)) {
    bVar1 = false;
  }
  else {
    bVar1 = true;
  }
  local_4 = 0xffffffff;
  if ((local_cc != &DAT_00501908) && (*local_cc = *local_cc + -1, *local_cc < 1)) {
    FUN_004011c0(&local_cc);
  }
  if (bVar1) {
    bVar1 = thunk_FUN_0049f260(param_1 + 0x67);
    if (!bVar1) {
      FUN_004a11d0(param_1 + 0x67);
    }
  }
  else {
    thunk_FUN_0049f340(param_1 + 0x67);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_cc = &DAT_00501908;
  FUN_00401d80(&local_cc,"STEER_PITCH");
  ppiVar13 = &local_cc;
  local_4 = 1;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  fVar7 = FUN_004386f0(pvVar3,(int *)ppiVar13);
  param_1[0x8c] = (int)(float)((float10)(float)param_1[0x8c] - fVar7);
  local_4 = 0xffffffff;
  if ((local_cc != &DAT_00501908) && (*local_cc = *local_cc + -1, *local_cc < 1)) {
    FUN_004011c0(&local_cc);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_cc = &DAT_00501908;
  FUN_00401d80(&local_cc,"STEER_YAW");
  ppiVar13 = &local_cc;
  local_4 = 2;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  fVar7 = FUN_004386f0(pvVar3,(int *)ppiVar13);
  param_1[0x8d] = (int)(float)((float10)(float)param_1[0x8d] - fVar7);
  local_4 = 0xffffffff;
  if ((local_cc != &DAT_00501908) && (*local_cc = *local_cc + -1, *local_cc < 1)) {
    FUN_004011c0(&local_cc);
  }
  uVar4 = (**(code **)(*param_1 + 0x18))();
  FUN_0042d2d0(auStack_50,uVar4);
  FUN_004977b0(auStack_ac,(int)auStack_50);
  local_cc = (int *)&stack0xffffff18;
  pfVar9 = (float *)&stack0xffffff18;
  uStack_9c = 0;
  uStack_98 = 0x3f800000;
  uStack_94 = 0;
  iVar5 = (**(code **)(*param_1 + 0x18))();
  pvVar3 = (void *)FUN_0043b2d0(afStack_c4,(float *)(param_1 + 0x9b),(float *)(iVar5 + 0x28));
  FUN_0045a230(pvVar3,pfVar9);
  fVar10 = 6.466554e-39;
  FUN_00496360((int)auStack_ac);
  if ((float)param_1[0x8e] < (float)param_1[0x8c]) {
    param_1[0x8c] = param_1[0x8e];
  }
  if ((float)param_1[0x8c] < -(float)param_1[0x8e]) {
    param_1[0x8c] = (int)-(float)param_1[0x8e];
  }
  if ((float)param_1[0x8f] < (float)param_1[0x8d]) {
    param_1[0x8d] = param_1[0x8f];
  }
  if ((float)param_1[0x8d] < -(float)param_1[0x8f]) {
    param_1[0x8d] = (int)-(float)param_1[0x8f];
  }
  afStack_c4[0] = 0.0;
  afStack_c4[1] = 1.0;
  afStack_c4[2] = 0.0;
  fVar11 = 6.466814e-39;
  FUN_0049e770(auStack_ac,auStack_5c);
  fVar12 = 6.466824e-39;
  fVar7 = FUN_00497aa0();
  fVar8 = (float10)_DAT_004e169c + -fVar7;
  if ((float10)(float)param_1[0x8c] < fVar8) {
    param_1[0x8c] = (int)(float)fVar8;
  }
  fVar7 = (-fVar7 + (float10)_DAT_004e1cbc) - (float10)_DAT_004e169c;
  if (fVar7 < (float10)(float)param_1[0x8c]) {
    param_1[0x8c] = (int)(float)fVar7;
  }
  fVar14 = (float)param_1[0x8c];
  local_cc = (int *)&stack0xffffff14;
  FUN_00446ee0(auStack_ac,(undefined4 *)&stack0xffffff14);
  FUN_00496dd0(auStack_ac,fVar10,fVar11,fVar12,fVar14);
  fVar14 = -(float)param_1[0x8d];
  local_cc = (int *)&stack0xffffff14;
  FUN_00447830(auStack_ac,(undefined4 *)&stack0xffffff14);
  FUN_00496dd0(auStack_ac,fVar10,fVar11,fVar12,fVar14);
  FUN_0049e770(auStack_ac,afStack_c4 + 3);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_c8 = &DAT_00501908;
  FUN_00401d80(&piStack_c8,"MOUSE_LB");
  ppiVar13 = &piStack_c8;
  local_4 = 3;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar4 = FUN_00438480(pvVar3,(int *)ppiVar13);
  cVar2 = (char)uVar4;
  pfVar9 = afStack_c4 + 3;
  pfVar6 = FUN_00465ef0(param_1,afStack_68,(float *)0x0);
  FUN_0046d1b0(param_1 + 0x520,pfVar6,pfVar9,cVar2);
  local_4 = 0xffffffff;
  if ((piStack_c8 != &DAT_00501908) && (*piStack_c8 = *piStack_c8 + -1, *piStack_c8 < 1)) {
    FUN_004011c0(&piStack_c8);
  }
  ExceptionList = pvStack_c;
  return;
}



/* 0x00465ef0 FUN_00465ef0 */

float * __thiscall FUN_00465ef0(void *this,float *param_1,float *param_2)

{
  void *pvVar1;
  float *pfVar2;
  float *pfVar3;
  float *pfVar4;
  undefined4 *puVar5;
  float fVar6;
  undefined4 local_60 [3];
  float afStack_54 [3];
  undefined4 auStack_48 [3];
  float afStack_3c [3];
  undefined4 auStack_30 [3];
  float afStack_24 [3];
  float afStack_18 [3];
  float afStack_c [3];
  
  if (param_2 == (float *)0x0) {
    param_2 = (float *)((int)this + 0x4c);
  }
  fVar6 = 10.0;
  puVar5 = local_60;
  pvVar1 = (void *)(**(code **)(*(int *)this + 0x18))();
  pfVar2 = (float *)FUN_0049e770(pvVar1,puVar5);
  pfVar2 = (float *)FUN_00446390(afStack_54,pfVar2,fVar6);
  fVar6 = 15.0;
  puVar5 = auStack_48;
  pvVar1 = (void *)(**(code **)(*(int *)this + 0x18))();
  pfVar3 = (float *)FUN_00447830(pvVar1,puVar5);
  pfVar3 = (float *)FUN_00446390(afStack_3c,pfVar3,fVar6);
  fVar6 = -10.0;
  puVar5 = auStack_30;
  pvVar1 = (void *)(**(code **)(*(int *)this + 0x18))();
  pfVar4 = (float *)FUN_00446ee0(pvVar1,puVar5);
  pfVar4 = (float *)FUN_00446390(afStack_24,pfVar4,fVar6);
  pfVar4 = (float *)FUN_0042d050(afStack_18,param_2,pfVar4);
  pfVar3 = (float *)FUN_0042d050(afStack_c,pfVar4,pfVar3);
  FUN_0042d050(param_1,pfVar3,pfVar2);
  return param_1;
}



/* 0x0046d1b0 FUN_0046d1b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0046d1b0(void *this,float *param_1,float *param_2,char param_3)

{
  void *this_00;
  float *pfVar1;
  float *pfVar2;
  float *pfVar3;
  float *pfVar4;
  float fVar5;
  float local_8c [3];
  float local_80;
  float local_7c;
  float local_78 [6];
  float local_60 [3];
  float local_54 [3];
  undefined4 local_48 [3];
  float local_3c [3];
  float local_30 [3];
  float local_24 [3];
  undefined4 local_18 [3];
  float local_c [3];
  
  *(float *)((int)this + 0x15c) = *param_2;
  *(float *)((int)this + 0x160) = param_2[1];
  *(float *)((int)this + 0x164) = param_2[2];
  if (*(char *)((int)this + 9) == '\0') {
    if (param_3 != '\0') {
      fVar5 = *(float *)((int)this + 0x3b8) -
              DAT_00502b04 * *(float *)((int)this + 0x3bc) * *(float *)((int)this + 0x3b0);
      *(float *)((int)this + 0x3b8) = fVar5;
      if (fVar5 < _DAT_004dc50c) {
        *(undefined4 *)((int)this + 0x3b8) = 0;
        *(undefined1 *)((int)this + 9) = 1;
      }
      fVar5 = *(float *)((int)this + 0x3b4) - *(float *)((int)this + 0x3b0);
      goto LAB_0046d2b5;
    }
    fVar5 = DAT_00502b04 * *(float *)((int)this + 0x3c0) * (float)_DAT_004e2e80 +
            *(float *)((int)this + 0x3b8);
    *(float *)((int)this + 0x3b8) = fVar5;
    if ((float)_DAT_004dd808 <= fVar5) {
      *(undefined4 *)((int)this + 0x3b8) = 0x3f800000;
    }
  }
  else {
    fVar5 = DAT_00502b04 * *(float *)((int)this + 0x3c0) + *(float *)((int)this + 0x3b8);
    *(float *)((int)this + 0x3b8) = fVar5;
    if ((float)_DAT_004dd808 <= fVar5) {
      FUN_004a11a0((void *)((int)this + 0x138),(undefined1 *)0x0);
      *(undefined1 *)((int)this + 9) = 0;
      *(undefined4 *)((int)this + 0x3b8) = 0x3f800000;
      thunk_FUN_0049f340((int *)((int)this + 0x114));
    }
  }
  fVar5 = -*(float *)((int)this + 0x3b0);
LAB_0046d2b5:
  *(float *)((int)this + 0x3b0) = fVar5 * (float)_DAT_004e27d8 + *(float *)((int)this + 0x3b0);
  fVar5 = _DAT_004dfc24 - *(float *)((int)this + 0x3b0);
  local_80 = *(float *)((int)this + 0x390) * *(float *)((int)this + 0x3b0) +
             fVar5 * *(float *)((int)this + 0x370);
  *(float *)((int)this + 0x3c4) = local_80;
  fVar5 = *(float *)((int)this + 0x3b0) * *(float *)((int)this + 0x394) +
          fVar5 * *(float *)((int)this + 0x374);
  *(float *)((int)this + 0x3c8) = fVar5;
  local_7c = fVar5;
  pfVar1 = (float *)FUN_00446390(local_8c,param_2,local_80);
  FUN_0046c000((void *)((int)this + 0x168),param_1,pfVar1,fVar5);
  if (*(char *)((int)this + 0x2f0) != '\0') {
    local_8c[0] = *param_2;
    local_8c[2] = param_2[2];
    this_00 = *(void **)((int)this + 0x20);
    fVar5 = 0.025;
    local_78[0] = 0.0;
    local_78[1] = -1.0;
    local_78[2] = 0.0;
    local_8c[1] = 0.0;
    pfVar1 = (float *)FUN_00446ee0(this_00,local_48);
    pfVar1 = (float *)FUN_00446390(local_c,pfVar1,fVar5);
    pfVar2 = (float *)FUN_00446390(local_60,local_78,0.4);
    fVar5 = 0.5125;
    pfVar3 = (float *)FUN_0049e770(this_00,local_18);
    pfVar3 = (float *)FUN_00446390(local_30,pfVar3,fVar5);
    fVar5 = 0.05;
    pfVar4 = (float *)FUN_0042d2b0(local_8c,local_54);
    pfVar4 = (float *)FUN_00446390(local_3c,pfVar4,fVar5);
    pfVar3 = (float *)FUN_0043b2d0(local_24,pfVar4,pfVar3);
    pfVar2 = (float *)FUN_0042d050(local_78 + 3,pfVar3,pfVar2);
    FUN_0043b2d0(local_8c,pfVar2,pfVar1);
    fVar5 = *(float *)((int)this + 0x394) * (float)_DAT_004e1430 +
            *(float *)((int)this + 0x3c8) * (float)_DAT_004e0508;
    pfVar1 = (float *)FUN_00446390(local_78 + 3,local_8c,2000.0);
    FUN_0046c000((void *)((int)this + 0x1d4),param_1,pfVar1,fVar5);
  }
  *(undefined1 *)((int)this + 0x1c) = 0;
  FUN_0046c5a0(this);
  return;
}



/* 0x0046d6b0 FUN_0046d6b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0046d6b0(void *this,int *param_1,undefined4 param_2,int *param_3)

{
  int *piVar1;
  float fVar2;
  void *pvVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  int iVar6;
  int *piVar7;
  int local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d221b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(int **)((int)this + 0x20) = param_1;
  *(undefined4 *)((int)this + 4) = param_2;
  *(undefined1 *)((int)this + 0x2f0) = param_3._0_1_;
  *(undefined4 *)((int)this + 0x370) = 0;
  *(undefined4 *)((int)this + 0x374) = 0x3f4ccccd;
  *(undefined4 *)((int)this + 0x378) = 0;
  *(undefined4 *)((int)this + 0x37c) = 0x42c80000;
  *(undefined4 *)((int)this + 0x380) = 0x42f00000;
  *(undefined4 *)((int)this + 900) = 0x42f00000;
  *(undefined4 *)((int)this + 0x388) = 0;
  *(undefined4 *)((int)this + 0x38c) = 0x41200000;
  *(undefined4 *)((int)this + 0x390) = 0x457a0000;
  *(undefined4 *)((int)this + 0x394) = 0x40400000;
  *(undefined4 *)((int)this + 0x398) = 0x3fe00000;
  *(undefined4 *)((int)this + 0x39c) = 0x43960000;
  local_3c = 0x43700000;
  *(undefined4 *)((int)this + 0x3a0) = 0x43700000;
  local_38 = (char *)0x43700000;
  *(undefined4 *)((int)this + 0x3a4) = 0x43700000;
  *(undefined4 *)((int)this + 0x3a8) = 0;
  *(undefined4 *)((int)this + 0x3ac) = 0x42200000;
  *(undefined4 *)((int)this + 0x3b4) = 0x3f800000;
  *(undefined4 *)((int)this + 0x3b8) = 0x3f800000;
  local_34 = (char *)0x0;
  *(undefined4 *)((int)this + 0x3b0) = 0;
  *(undefined4 *)((int)this + 0x3bc) = 0x3d4ccccd;
  *(undefined4 *)((int)this + 0x3c0) = 0x3e800000;
  *(undefined1 *)((int)this + 9) = 0;
  *(undefined4 *)((int)this + 0x3c4) = 0;
  *(undefined4 *)((int)this + 0x3c8) = 0;
  *(undefined1 *)this = 0;
  pvVar3 = operator_new(*(int *)((int)this + 4) << 2);
  *(void **)((int)this + 0x36c) = pvVar3;
  param_3 = (int *)0x0;
  if (0 < *(int *)((int)this + 4)) {
    do {
      fVar2 = (float)(int)param_3 / (float)(*(int *)((int)this + 4) + -1);
      param_3 = (int *)((int)param_3 + 1);
      *(float *)(*(int *)((int)this + 0x36c) + -4 + (int)param_3 * 4) =
           fVar2 * (float)_DAT_004de6f8 + fVar2 * fVar2 * (float)_DAT_004de6f8;
    } while ((int)param_3 < *(int *)((int)this + 4));
  }
  FUN_00469420((void *)((int)this + 0x2f4),1);
  puVar4 = (undefined4 *)((int)this + 0x270);
  iVar6 = 4;
  do {
    puVar4[-1] = 0x437f0000;
    *puVar4 = 0x437f0000;
    puVar4[1] = 0x437f0000;
    puVar4[2] = 0x437f0000;
    puVar4 = puVar4 + 9;
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  pvVar3 = *(void **)(DAT_00503a78 + 4);
  iVar6 = (int)pvVar3 + 0xb4;
  if (*(char *)((int)pvVar3 + 0x160) == '\0') {
    iVar6 = (int)pvVar3 + 8;
  }
  param_2 = *(undefined4 *)(iVar6 + 0x78);
  param_1 = (int *)FUN_00493d00(pvVar3,3,'\x01');
  param_3 = (int *)0x0;
  if (0 < *(int *)((int)this + 4)) {
    piVar1 = (int *)((int)this + 0x240);
    do {
      uVar5 = geWorld_AddPoly(param_2,(int)this + 600,4,param_1,0,0x20,0x3f800000);
      iVar6 = 0;
      if (0 < *(int *)((int)this + 0x244)) {
        piVar7 = (int *)*piVar1;
        do {
          if (*piVar7 == 0) goto LAB_0046d8fa;
          iVar6 = iVar6 + 1;
          piVar7 = piVar7 + 1;
        } while (iVar6 < *(int *)((int)this + 0x244));
      }
      FUN_0043b6e0(piVar1,iVar6 + 1);
LAB_0046d8fa:
      *(undefined4 *)(*piVar1 + iVar6 * 4) = uVar5;
      param_3 = (int *)((int)param_3 + 1);
    } while ((int)param_3 < *(int *)((int)this + 4));
  }
  pvVar3 = (void *)((int)this + 0x3cc);
  *(undefined4 *)((int)this + 0x2e8) = 0x3e19999a;
  *(undefined4 *)((int)this + 0x2ec) = 0;
  local_3c = 0x3f800000;
  local_38 = (char *)0x3f800000;
  local_34 = (char *)0x3f800000;
  FUN_0047db10(pvVar3,2,0x3e4ccccd,0x3f400000,0x3f800000,&local_3c);
  local_3c = 0x3f800000;
  local_38 = (char *)0x3f800000;
  local_34 = (char *)0x3f800000;
  FUN_0047db10(pvVar3,2,0x3c23d70a,0,0x3f800000,&local_3c);
  *(uint *)((int)this + 0x3e4) = *(uint *)((int)this + 0x3e4) & 0xfffffffe | 0xc;
  FUN_0047fe70((void *)((int)this + 1000),*(void **)(DAT_00503a78 + 4),pvVar3,'\x01');
  *(undefined4 *)((int)this + 0x418) = 0x14;
  FUN_00469520((void *)((int)this + 0x24),'\x01');
  if (*(char *)((int)this + 0x2f0) != '\0') {
    FUN_0042d280((void *)((int)this + 0x1d4),&local_3c,0,0x43960000,0);
    local_3c = 0x3f800000;
    local_38 = (char *)0x3f800000;
    local_34 = (char *)0x3f800000;
    FUN_0047db10((void *)((int)this + 0x420),6,0x3f800000,0x3f400000,0x3f800000,&local_3c);
    param_3 = operator_new(0x168);
    local_4 = 0;
    if (param_3 == (int *)0x0) {
      pvVar3 = (void *)0x0;
    }
    else {
      pvVar3 = FUN_0047f850(param_3,(int)this + 0x420,0);
    }
    *(void **)((int)this + 0x43c) = pvVar3;
    *(undefined4 *)((int)pvVar3 + 0xb0) = 2;
    *(undefined4 *)(*(int *)((int)this + 0x43c) + 0xa4) = 0x30;
    local_4 = 0xffffffff;
    FUN_0042d280((void *)(*(int *)((int)this + 0x43c) + 0xb4),&local_3c,0x43700000,0x43700000,0);
    *(undefined4 *)((int)this + 0x41c) = 0;
    FUN_00469420((void *)((int)this + 0x330),0);
    local_3c = 0x42800000;
    local_38 = (char *)0x42800000;
    *(undefined4 *)((int)this + 0x34c) = 0x42800000;
    local_34 = (char *)0x0;
    *(undefined4 *)((int)this + 0x350) = 0x42800000;
    *(undefined4 *)((int)this + 0x354) = 0;
    *(undefined1 *)((int)this + 8) = 1;
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_3 = &DAT_00501908;
  FUN_00401d80(&param_3,"TankRefill");
  local_4 = 1;
  FUN_004a12f0((void *)((int)this + 0x114),&param_3,0,2);
  piVar1 = param_3;
  local_4 = 0xffffffff;
  if (param_3 != &DAT_00501908) {
    *param_3 = *param_3 + -1;
    if (*param_3 < 1) {
      param_3 = &DAT_00501908;
      if (*piVar1 < 0) {
        piVar7 = &local_3c;
        for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
          *piVar7 = 0;
          piVar7 = piVar7 + 1;
        }
        local_3c = 0;
        local_38 = "Invalid reference count";
        local_34 = "..\\BluelAPI/String/Rope_RatStr.h";
        local_30 = 0xd0;
        FUN_00422060(&local_3c);
      }
      _free(piVar1);
    }
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"RefillFinish");
  local_4 = 2;
  FUN_004a12f0((void *)((int)this + 0x138),&param_1,0,2);
  piVar1 = param_1;
  local_4 = 0xffffffff;
  if (param_1 != &DAT_00501908) {
    *param_1 = *param_1 + -1;
    if (*param_1 < 1) {
      param_1 = &DAT_00501908;
      if (*piVar1 < 0) {
        piVar7 = &local_24;
        for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
          *piVar7 = 0;
          piVar7 = piVar7 + 1;
        }
        local_24 = 0;
        local_20 = "Invalid reference count";
        local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
        local_18 = 0xd0;
        FUN_00422060(&local_24);
      }
      _free(piVar1);
    }
  }
  ExceptionList = local_c;
  return;
}



