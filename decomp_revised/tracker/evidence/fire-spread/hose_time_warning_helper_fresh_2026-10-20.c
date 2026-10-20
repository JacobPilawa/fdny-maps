/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00481f40 FUN_00481f40 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint __thiscall FUN_00481f40(void *this,int *param_1,uint param_2)

{
  int iVar1;
  float fVar2;
  float fVar3;
  uint uVar4;
  undefined4 *puVar5;
  undefined2 unaff_DI;
  float10 fVar6;
  ulonglong uVar7;
  int *piVar8;
  int *piVar9;
  int *local_2c;
  int *local_28;
  double local_24;
  undefined4 local_1c;
  int local_18;
  int local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d35e8;
  local_c = ExceptionList;
  fVar2 = ABS((float)*(int *)((int)this + 0x278) - (float)param_1);
  fVar3 = (float)_DAT_004de6f8;
  if (fVar2 < fVar3) {
    return CONCAT22((short)((uint)ExceptionList >> 0x10),
                    (ushort)(fVar2 < fVar3) << 8 | (ushort)(NAN(fVar2) || NAN(fVar3)) << 10 |
                    (ushort)(fVar2 == fVar3) << 0xe);
  }
  ExceptionList = &local_c;
  fVar6 = FUN_004b98b0((double)(float)param_1,unaff_DI);
  uVar4 = param_2;
  if ((float10)(int)param_2 == fVar6) {
    fVar6 = FUN_004b98b0((double)*(int *)((int)this + 0x278),unaff_DI);
    iVar1 = uVar4 + 1;
    param_2 = CONCAT31((int3)((uint)iVar1 >> 8),1);
    if ((float10)iVar1 != fVar6) goto LAB_00481fd2;
  }
  else {
LAB_00481fd2:
    param_2 = param_2 & 0xffffff00;
  }
  if ((0x14 < (int)uVar4) && ((float)param_1 < _DAT_004e1604)) {
    fVar6 = FUN_004b98b0((double)(float)param_1,unaff_DI);
    local_24 = (double)fVar6;
    fVar6 = FUN_004b98b0((double)*(int *)((int)this + 0x278),unaff_DI);
    if (fVar6 != (float10)local_24) {
      FUN_004a11a0((void *)((int)this + 0x1c),(undefined1 *)0x0);
    }
  }
  uVar7 = FUN_004b9400();
  FUN_004054e0(&local_28,(int)uVar7,'\x01','\0');
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_2c = &DAT_00501908;
  if (local_28 != &DAT_00501908) {
    local_2c = local_28;
    *local_28 = *local_28 + 1;
  }
  local_4._0_1_ = 1;
  local_4._1_3_ = 0;
  if (_DAT_004e169c <= (float)param_1) {
    if ((float)param_1 < _DAT_004df224) {
      piVar9 = (int *)0x6;
    }
    else if ((float)param_1 < _DAT_004e36a4) {
      piVar9 = (int *)0x4;
    }
    else if ((float)param_1 < _DAT_004e36a0) {
      piVar9 = (int *)0x3;
    }
    else {
      if (_DAT_004e369c <= (float)param_1) goto LAB_00482106;
      piVar9 = (int *)0x1;
    }
  }
  else {
    piVar9 = (int *)0x7;
  }
  FUN_00405af0(&local_2c,piVar9);
LAB_00482106:
  if (5 < local_2c[1]) {
    puVar5 = FUN_00404050(&local_2c,&param_1,5);
    local_4._0_1_ = 2;
    FUN_00403b00(&local_2c,puVar5);
    local_4._0_1_ = 1;
    if (param_1 != &DAT_00501908) {
      *param_1 = *param_1 + -1;
      if (*param_1 < 1) {
        FUN_004011c0(&param_1);
      }
      else {
        param_1 = &DAT_00501908;
      }
    }
  }
  piVar9 = local_2c;
  *(undefined4 *)((int)this + 0x138) = 0x3f000000;
  *(undefined4 *)((int)this + 0x13c) = 0x3f000000;
  local_1c = *(undefined4 *)((int)this + 0x144);
  local_10 = *(undefined4 *)((int)this + 0x150);
  local_18 = *(int *)((int)this + 0x148) + -10;
  local_14 = *(int *)((int)this + 0x14c) + 3;
  local_24 = (double)CONCAT44(local_24._4_4_,&stack0xffffffb4);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar8 = &DAT_00501908;
  if (local_2c != &DAT_00501908) {
    *local_2c = *local_2c + 1;
    piVar8 = local_2c;
  }
  FUN_0048c780((void *)((int)this + 0x40),piVar8,&local_1c,1,2);
  *(int *)((int)this + 0x278) = (int)uVar7;
  local_4 = (uint)local_4._1_3_ << 8;
  if ((piVar9 != &DAT_00501908) && (iVar1 = *piVar9, *piVar9 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&local_2c);
  }
  local_4 = 0xffffffff;
  if ((local_28 != &DAT_00501908) && (*local_28 = *local_28 + -1, *local_28 < 1)) {
    local_28 = (int *)FUN_004011c0(&local_28);
  }
  ExceptionList = local_c;
  return CONCAT31((int3)((uint)local_28 >> 8),(undefined1)param_2);
}



