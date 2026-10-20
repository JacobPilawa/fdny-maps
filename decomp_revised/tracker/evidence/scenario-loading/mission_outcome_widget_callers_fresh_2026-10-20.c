/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00458b20 FUN_00458b20 */

undefined4 * __fastcall FUN_00458b20(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0db5;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0047dba0(param_1 + 2);
  local_4 = 0;
  FUN_0048c120(param_1 + 9);
  local_4._0_1_ = 1;
  FUN_0047ddc0(param_1 + 0x4a);
  local_4._0_1_ = 2;
  FUN_00457640(param_1 + 0x93);
  local_4._0_1_ = 3;
  FUN_0048c120(param_1 + 0xdf);
  local_4._0_1_ = 4;
  FUN_00457640(param_1 + 0x120);
  local_4._0_1_ = 5;
  FUN_0048c120(param_1 + 0x16c);
  local_4._0_1_ = 6;
  FUN_0047dba0(param_1 + 0x1af);
  local_4._0_1_ = 7;
  FUN_0048c120(param_1 + 0x1b6);
  local_4._0_1_ = 8;
  FUN_004a13a0(param_1 + 0x1f7);
  local_4._0_1_ = 9;
  FUN_004a13a0(param_1 + 0x200);
  local_4._0_1_ = 10;
  FUN_0048d330(param_1 + 0x209);
  local_4._0_1_ = 0xb;
  FUN_00457640(param_1 + 0x259);
  local_4._0_1_ = 0xc;
  FUN_00457640(param_1 + 0x2a5);
  local_4._0_1_ = 0xd;
  FUN_0047ddc0(param_1 + 0x2f5);
  local_4._0_1_ = 0xe;
  FUN_0047ddc0(param_1 + 0x33e);
  local_4._0_1_ = 0xf;
  FUN_004a13a0(param_1 + 0x387);
  local_4._0_1_ = 0x10;
  FUN_00456360(param_1 + 0x390);
  local_4._0_1_ = 0x11;
  FUN_004563d0((int)(param_1 + 0x42f));
  local_4._0_1_ = 0x12;
  FUN_004558f0(param_1 + 0x6db);
  local_4._0_1_ = 0x13;
  FUN_00455950(param_1 + 0x73c);
  local_4._0_1_ = 0x14;
  _eh_vector_constructor_iterator_(param_1 + 0x863,0x130,5,FUN_00457640,FUN_0044f2d0);
  local_4._0_1_ = 0x15;
  FUN_0048c120(param_1 + 0x9df);
  param_1[0xa25] = 0x10;
  param_1[0xa24] = FUN_0043f660;
  param_1[0xa20] = 0;
  param_1[0xa21] = 0;
  param_1[0xa22] = 0;
  param_1[0xa23] = 0;
  param_1[0xa2b] = 0x10;
  param_1[0xa2a] = FUN_0043f660;
  param_1[0xa26] = 0;
  param_1[0xa27] = 0;
  param_1[0xa28] = 0;
  param_1[0xa29] = 0;
  local_4._0_1_ = 0x18;
  FUN_0047dba0(param_1 + 0xa2d);
  local_4._0_1_ = 0x19;
  FUN_00455d50(param_1 + 0xa34);
  local_4 = CONCAT31(local_4._1_3_,0x1a);
  FUN_0047ddc0(param_1 + 0xacf);
  *param_1 = 0;
  param_1[1] = 0;
  ExceptionList = local_c;
  return param_1;
}



/* 0x004653f0 FUN_004653f0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __fastcall FUN_004653f0(undefined4 *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined1 local_4;
  undefined3 uStack_3;
  
  puStack_8 = &LAB_004d1b26;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  param_1[10] = 0x10;
  param_1[9] = &LAB_00464d40;
  param_1[5] = 0;
  param_1[6] = 0;
  param_1[7] = 0;
  param_1[8] = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1[0xb] = &DAT_00501908;
  local_4 = 1;
  uStack_3 = 0;
  FUN_0048c120(param_1 + 0xc);
  local_4 = 2;
  FUN_0048d330(param_1 + 0x4d);
  local_4 = 3;
  FUN_0047ddc0(param_1 + 0xa1);
  local_4 = 4;
  _eh_vector_constructor_iterator_(param_1 + 0xea,0x124,4,FUN_0047ddc0,FUN_0047de60);
  local_4 = 5;
  FUN_0047dba0(param_1 + 0x20e);
  local_4 = 6;
  FUN_0047ddc0(param_1 + 0x215);
  local_4 = 7;
  FUN_0047ddc0(param_1 + 0x25e);
  local_4 = 8;
  FUN_0047ddc0(param_1 + 0x2a7);
  local_4 = 9;
  FUN_00456360(param_1 + 0x2f0);
  local_4 = 10;
  param_1[0x394] = 0;
  param_1[0x393] = 0;
  FUN_0047ddc0(param_1 + 0x395);
  local_4 = 0xb;
  FUN_0047ddc0(param_1 + 0x3de);
  local_4 = 0xc;
  FUN_0047ddc0(param_1 + 0x427);
  local_4 = 0xd;
  FUN_004a13a0(param_1 + 0x470);
  _local_4 = CONCAT31(uStack_3,0xe);
  FUN_004a13a0(param_1 + 0x479);
  *param_1 = 0;
  param_1[1] = 0;
  param_1[2] = 0;
  ExceptionList = local_c;
  return param_1;
}



/* 0x00464fe0 FUN_00464fe0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00464fe0(undefined4 *param_1)

{
  float fVar1;
  char cVar2;
  undefined4 *puVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int *piVar7;
  ulonglong uVar8;
  float local_a8;
  int local_a0;
  int local_98;
  int local_94;
  float local_90;
  int local_8c;
  int local_88;
  int local_84;
  int local_80;
  float local_7c [3];
  int local_70;
  undefined4 local_6c;
  undefined4 local_68;
  undefined4 local_64;
  undefined4 local_60 [3];
  int local_54;
  char *local_50;
  char *local_4c;
  undefined4 local_48;
  int local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  int local_18;
  float local_14 [4];
  
  if (*(char *)(param_1 + 0x9c) != '\0') {
    cVar2 = FUN_0048d1a0(param_1 + 0x4d);
    if (cVar2 == '\0') {
      FUN_004295b0(DAT_00502aac,*(int *)(param_1[1] + 0x198) == 1);
    }
    return;
  }
  if (*(int *)(param_1[1] + 0x198) == 1) {
    FUN_0042a730(DAT_00502aac,(undefined4 *)0x3,(int *)&DAT_00502ab4);
    FUN_0048d180(param_1 + 0x4d,0x47);
    puVar3 = param_1 + 0x470;
  }
  else {
    if (*(int *)(param_1[1] + 0x198) != 2) goto LAB_00465066;
    FUN_0042a730(DAT_00502aac,(undefined4 *)0x3,(int *)&DAT_00502ab4);
    FUN_0048d180(param_1 + 0x4d,0x48);
    puVar3 = param_1 + 0x479;
  }
  FUN_004a11a0(puVar3,(undefined1 *)0x0);
LAB_00465066:
  iVar5 = param_1[3];
  if (iVar5 != 0) {
    local_a0 = *(int *)(iVar5 + 0xc) * -0x28 + 0x1c0;
    fVar1 = (float)(DAT_00502afc - param_1[4]);
    if (DAT_00502afc - param_1[4] < 0) {
      fVar1 = fVar1 + (float)_DAT_004dfc90;
    }
    if (_DAT_004dfc24 <= fVar1 * (float)_DAT_004dfa28) {
      local_a8 = (fVar1 * (float)_DAT_004dfa28 - _DAT_004dfc24) * _DAT_004e2afc;
      if (_DAT_004dfc24 < local_a8) {
        local_a8 = 1.0;
      }
    }
    else {
      local_a8 = 0.0;
    }
    iVar6 = 0;
    if (0 < *(int *)(iVar5 + 0xc)) {
      local_7c[0] = 0.0;
      local_90 = local_a8 * _DAT_004e2af8;
      local_7c[1] = 50.0;
      local_7c[2] = 0.0;
      do {
        if ((iVar6 < 0) || (*(int *)(iVar5 + 0xc) <= iVar6)) {
          piVar7 = &local_3c;
          for (iVar4 = 6; iVar4 != 0; iVar4 = iVar4 + -1) {
            *piVar7 = 0;
            piVar7 = piVar7 + 1;
          }
          local_3c = 1;
          local_38 = "Array index out of bounds";
          local_34 = "..\\BluelAPI/Container/SArray.h";
          local_30 = 0x1cd;
          FUN_00422060(&local_3c);
        }
        puVar3 = (undefined4 *)
                 FUN_0042d050(local_14,(float *)(*(int *)(*(int *)(iVar5 + 8) + iVar6 * 4) + 0x4c),
                              local_7c);
        FUN_00492bc0((void *)*param_1,local_60,puVar3);
        uVar8 = FUN_004b9400();
        local_98 = (int)uVar8;
        uVar8 = FUN_004b9400();
        local_94 = (int)uVar8;
        FUN_0048a5c0(&DAT_004ffad8,&local_98);
        FUN_0048a620(&DAT_004ffad8,&local_94);
        local_84 = local_94 + -0x20;
        local_80 = local_94 + 0x20;
        local_8c = local_98 + -0x20;
        local_88 = local_98 + 0x20;
        local_18 = local_a0 + 0x20;
        uVar8 = FUN_004b9400();
        local_68 = (undefined4)uVar8;
        uVar8 = FUN_004b9400();
        local_64 = (undefined4)uVar8;
        uVar8 = FUN_004b9400();
        local_70 = (int)uVar8;
        uVar8 = FUN_004b9400();
        local_6c = (undefined4)uVar8;
        if ((iVar6 < 0) || ((int)param_1[6] <= iVar6)) {
          piVar7 = &local_54;
          for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
            *piVar7 = 0;
            piVar7 = piVar7 + 1;
          }
          local_54 = 1;
          local_50 = "Array index out of bounds";
          local_4c = "..\\BluelAPI/Container/SArray.h";
          local_48 = 0x1cd;
          FUN_00422060(&local_54);
        }
        FUN_0047edf0(*(void **)(param_1[5] + iVar6 * 4),&local_70,0);
        iVar5 = param_1[3];
        iVar6 = iVar6 + 1;
        local_a0 = local_a0 + 0x28;
      } while (iVar6 < *(int *)(iVar5 + 0xc));
    }
  }
  iVar5 = param_1[0xa0];
  iVar6 = param_1[0x9e];
  iVar4 = 0;
  uVar8 = FUN_004b9400();
  FUN_0047ea10(param_1 + 0xa1,param_1[0x9d],(int)uVar8,iVar6,iVar5,iVar4);
  return;
}



/* 0x00465580 FUN_00465580 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00465580(void *this,int *param_1,int *param_2,undefined4 param_3)

{
  uint uVar1;
  int iVar2;
  void *this_00;
  int *piVar3;
  char cVar4;
  float local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1b40;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(int **)((int)this + 4) = param_2;
  *(int **)this = param_1;
  *(undefined4 *)((int)this + 8) = param_3;
  FUN_0048abb0((void *)((int)this + 0x30),1,'\x01',0x12);
  *(undefined4 *)((int)this + 0x274) = 0x260;
  *(undefined4 *)((int)this + 0x278) = 0x270;
  *(undefined4 *)((int)this + 0x27c) = 0x20;
  *(undefined4 *)((int)this + 0x280) = 0x1c0;
  iVar2 = 4;
  this_00 = (void *)((int)this + 0x3a8);
  do {
    local_18[0] = 1.0;
    local_18[1] = 1.0;
    local_18[2] = 1.0;
    FUN_0047ee90(this_00,local_18,'\x01');
    FUN_0047dd30(this_00,1.0);
    this_00 = (void *)((int)this_00 + 0x124);
    iVar2 = iVar2 + -1;
  } while (iVar2 != 0);
  FUN_0047ea10((void *)((int)this + 0x3a8),*(int *)((int)this + 0x274) + -1,
               *(int *)((int)this + 0x27c),*(int *)((int)this + 0x274),*(int *)((int)this + 0x280),0
              );
  FUN_0047ea10((void *)((int)this + 0x4cc),*(int *)((int)this + 0x278),*(int *)((int)this + 0x27c),
               *(int *)((int)this + 0x278) + 1,*(int *)((int)this + 0x280),0);
  FUN_0047ea10((void *)((int)this + 0x5f0),*(int *)((int)this + 0x274) + -1,
               *(int *)((int)this + 0x27c) + -1,*(int *)((int)this + 0x278) + 1,
               *(int *)((int)this + 0x27c),0);
  FUN_0047ea10((void *)((int)this + 0x714),*(int *)((int)this + 0x274) + -1,
               *(int *)((int)this + 0x280),*(int *)((int)this + 0x278) + 1,
               *(int *)((int)this + 0x280) + 1,0);
  local_18[0] = 1.0;
  local_18[1] = 1.0;
  local_18[2] = 1.0;
  FUN_0047ee90((void *)((int)this + 0x284),local_18,'\x01');
  FUN_0047dd30((void *)((int)this + 0x284),0.5);
  FUN_00482260((void *)((int)this + 0xbc0),(int *)0x2);
  uVar1 = FUN_00481f40((void *)((int)this + 0xbc0),(int *)0x0,5);
  if ((char)uVar1 != '\0') {
    cVar4 = '\x01';
    param_1 = (int *)&stack0xffffffd0;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar3 = &DAT_00501908;
    FUN_00401d80(&stack0xffffffd0,"RC_HoseTimeWarn");
    FUN_004897c0((void *)(*(int *)((int)this + 4) + 0x208),piVar3,cVar4);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_2 = &DAT_00501908;
  FUN_00401d80(&param_2,"HoseWin");
  local_4 = 0;
  FUN_004a12f0((void *)((int)this + 0x11c0),&param_2,0,1);
  local_4 = 0xffffffff;
  if ((param_2 != &DAT_00501908) && (*param_2 = *param_2 + -1, *param_2 < 1)) {
    FUN_004011c0(&param_2);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"HoseLose");
  local_4 = 1;
  FUN_004a12f0((void *)((int)this + 0x11e4),&param_1,0,1);
  local_4 = 0xffffffff;
  if (param_1 != &DAT_00501908) {
    *param_1 = *param_1 + -1;
    if (*param_1 < 1) {
      FUN_004011c0(&param_1);
    }
    else {
      param_1 = &DAT_00501908;
    }
  }
  *(undefined4 *)((int)this + 0xc) = 0;
  FUN_0048d390((void *)((int)this + 0x134));
  ExceptionList = local_c;
  return;
}



/* 0x0048d330 FUN_0048d330 */

undefined4 * __fastcall FUN_0048d330(undefined4 *param_1)

{
  FUN_0047ddc0(param_1);
  param_1[0x4a] = 0;
  param_1[0x49] = 0;
  param_1[0x49] = 0x1e0;
  param_1[0x4a] = 400;
  param_1[0x4e] = 0;
  *(undefined1 *)(param_1 + 0x4f) = 0;
  param_1[0x4b] = 0xffffffff;
  param_1[0x4c] = 1000;
  param_1[0x4d] = 5000;
  return param_1;
}



