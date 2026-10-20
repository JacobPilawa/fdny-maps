/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00465b90 FUN_00465b90 */

void * __thiscall FUN_00465b90(void *this,undefined4 param_1,int param_2)

{
  int *this_00;
  undefined4 uVar1;
  void *pvVar2;
  undefined4 *puVar3;
  void *this_01;
  int *piVar4;
  int iVar5;
  int *piVar6;
  int iVar7;
  int local_6c;
  undefined1 local_64 [8];
  int local_5c;
  char *local_58 [4];
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  int local_34;
  char *local_30;
  char *local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 local_18;
  undefined4 local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  int local_4;
  
  puStack_8 = &LAB_004d1b6e;
  local_c = ExceptionList;
  this_00 = (int *)((int)this + 8);
  ExceptionList = &local_c;
  *(undefined4 *)((int)this + 0x1c) = 0x10;
  *(code **)((int)this + 0x18) = FUN_0043f660;
  *this_00 = 0;
  *(undefined4 *)((int)this + 0xc) = 0;
  *(undefined4 *)((int)this + 0x10) = 0;
  *(undefined4 *)((int)this + 0x14) = 0;
  *(int *)this = param_2;
  local_58[0] = *(char **)(param_2 + 0x40);
  local_58[1] = *(char **)(param_2 + 0x48);
  iVar5 = *(int *)(param_2 + 0x38);
  local_58[2] = *(char **)(param_2 + 0x50);
  local_58[3] = *(char **)(param_2 + 0x58);
  local_48 = *(undefined4 *)(param_2 + 0x60);
  local_44 = *(undefined4 *)(param_2 + 0x68);
  local_40 = *(undefined4 *)(param_2 + 0x70);
  local_3c = *(undefined4 *)(param_2 + 0x78);
  local_34 = *(int *)(param_2 + 0x3c);
  local_30 = *(char **)(param_2 + 0x44);
  local_2c = *(char **)(param_2 + 0x4c);
  local_28 = *(undefined4 *)(param_2 + 0x54);
  local_24 = *(undefined4 *)(param_2 + 0x5c);
  local_20 = *(undefined4 *)(param_2 + 100);
  local_1c = *(undefined4 *)(param_2 + 0x6c);
  local_18 = *(undefined4 *)(param_2 + 0x74);
  local_14 = *(undefined4 *)(param_2 + 0x7c);
  local_4 = 0;
  local_38 = 0;
  local_10 = 0;
  local_5c = iVar5;
  if (iVar5 != 0) {
    local_6c = 0;
    do {
      pvVar2 = operator_new(0x14);
      uVar1 = *(undefined4 *)((int)&local_34 + local_6c);
      *(int *)((int)pvVar2 + 4) = iVar5;
      *(undefined4 *)((int)pvVar2 + 8) = uVar1;
      puVar3 = operator_new(0x14);
      local_4._0_1_ = 1;
      if (puVar3 == (undefined4 *)0x0) {
        this_01 = (void *)0x0;
      }
      else {
        this_01 = (void *)FUN_0049f230(puVar3);
      }
      local_4._0_1_ = 0;
      piVar4 = FUN_0049b760(local_64,"RCS_WindowFire",100);
      local_4._0_1_ = 2;
      iVar5 = FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar4);
      local_4 = (uint)local_4._1_3_ << 8;
      FUN_0049b6f0((int)local_64);
      if (iVar5 != 0) {
        *(int *)((int)this_01 + 0xc) = iVar5;
      }
      *(undefined4 *)((int)this_01 + 8) = 0;
      FUN_0049d130(this_01,pvVar2);
      piVar4 = FUN_0049d390((int)this_01);
      iVar5 = 0;
      if (0 < *(int *)((int)this + 0xc)) {
        piVar6 = (int *)*this_00;
        do {
          if (*piVar6 == 0) goto LAB_00465d48;
          iVar5 = iVar5 + 1;
          piVar6 = piVar6 + 1;
        } while (iVar5 < *(int *)((int)this + 0xc));
      }
      FUN_0043b6e0(this_00,iVar5 + 1);
LAB_00465d48:
      *(int **)(*this_00 + iVar5 * 4) = piVar4;
      iVar5 = *(int *)((int)local_58 + local_6c);
      local_6c = local_6c + 4;
    } while (iVar5 != 0);
  }
  FUN_00465940(this,'\0');
  iVar5 = 0;
  if (0 < *(int *)((int)this + 0xc)) {
    do {
      if ((iVar5 < 0) || (*(int *)((int)this + 0xc) <= iVar5)) {
        piVar4 = &local_5c;
        for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_5c = 1;
        local_58[0] = "Array index out of bounds";
        local_58[1] = "..\\BluelAPI/Container/SArray.h";
        local_58[2] = (char *)0x1cd;
        FUN_00422060(&local_5c);
      }
      *(undefined1 *)(*(int *)(*(int *)(*this_00 + iVar5 * 4) + 0x2a8) + 0xe) = 0;
      if ((iVar5 < 0) || (*(int *)((int)this + 0xc) <= iVar5)) {
        piVar4 = &local_34;
        for (iVar7 = 6; iVar7 != 0; iVar7 = iVar7 + -1) {
          *piVar4 = 0;
          piVar4 = piVar4 + 1;
        }
        local_34 = 1;
        local_30 = "Array index out of bounds";
        local_2c = "..\\BluelAPI/Container/SArray.h";
        local_28 = 0x1cd;
        FUN_00422060(&local_34);
      }
      *(undefined1 *)(*(int *)(*(int *)(*this_00 + iVar5 * 4) + 0x2a8) + 0xd) = 0;
      iVar5 = iVar5 + 1;
    } while (iVar5 < *(int *)((int)this + 0xc));
  }
  *(undefined4 *)((int)this + 0x28) = 0;
  *(undefined4 *)((int)this + 4) = 0xffffffff;
  *(undefined1 *)((int)this + 0x2c) = 0;
  *(undefined1 *)((int)this + 0x24) = 1;
  *(undefined4 *)((int)this + 0x20) = *(undefined4 *)(*(int *)this + 0x34);
  ExceptionList = local_c;
  return this;
}



/* 0x004683f0 FUN_004683f0 */

void * __thiscall FUN_004683f0(void *this,void *param_1)

{
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1ffe;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0049d420(this,(int)param_1);
  local_4 = 0;
  FUN_0042d2d0((undefined4 *)((int)this + 0x24),0);
  *(undefined ***)this = &PTR_FUN_004e2d88;
  *(undefined4 *)((int)this + 0x24) = &PTR_FUN_004dfc30;
  *(undefined4 *)((int)this + 0x68) = 0;
  *(undefined4 *)((int)this + 0x6c) = 0;
  *(undefined4 *)((int)this + 0x70) = 0;
  *(undefined4 *)((int)this + 0x74) = 0;
  *(undefined4 *)((int)this + 0x78) = 0;
  *(undefined4 *)((int)this + 0x7c) = 0;
  *(undefined4 *)((int)this + 0x80) = 0;
  *(undefined4 *)((int)this + 0x84) = 0;
  *(undefined4 *)((int)this + 0x88) = 0;
  *(undefined4 *)((int)this + 0x90) = 0;
  *(undefined4 *)((int)this + 0x94) = 0;
  *(undefined4 *)((int)this + 0x98) = 0;
  *(undefined4 *)((int)this + 0x9c) = 0;
  *(undefined4 *)((int)this + 0xa0) = 0;
  *(undefined4 *)((int)this + 0xa4) = 0;
  *(undefined4 *)((int)this + 0xa8) = 0;
  *(undefined4 *)((int)this + 0xac) = 0;
  *(undefined4 *)((int)this + 0xb0) = 0;
  *(undefined4 *)((int)this + 0x8c) = 0;
  FUN_004804f0((undefined4 *)((int)this + 0xc0));
  local_4._0_1_ = 1;
  FUN_004804f0((undefined4 *)((int)this + 0x144));
  local_4._0_1_ = 2;
  FUN_004804f0((undefined4 *)((int)this + 0x1c8));
  local_4._0_1_ = 3;
  FUN_004a13a0((undefined4 *)((int)this + 0x260));
  local_4._0_1_ = 4;
  FUN_004a13a0((undefined4 *)((int)this + 0x284));
  local_4 = CONCAT31(local_4._1_3_,5);
  FUN_0049d140(param_1,(int *)((int)this + 0x2a8));
  *(undefined4 *)((int)this + 0xb8) = 0;
  *(undefined4 *)((int)this + 0xb4) = 0;
  *(undefined4 *)((int)this + 0xbc) = 0;
  *(undefined4 *)((int)this + 100) = 0;
  *(undefined4 *)((int)this + 0x18) = 7;
  ExceptionList = local_c;
  return this;
}



/* 0x00468680 FUN_00468680 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00468680(int param_1)

{
  int iVar1;
  float fVar2;
  
  iVar1 = *(int *)(param_1 + 0x2a8);
  if ((*(char *)(iVar1 + 0xc) != '\0') && (*(char *)(iVar1 + 0xe) != '\0')) {
    fVar2 = *(float *)(param_1 + 600) - DAT_00502b04 / *(float *)(iVar1 + 8);
    *(float *)(param_1 + 600) = fVar2;
    if (fVar2 < _DAT_004dc50c) {
      *(undefined1 *)(iVar1 + 0xd) = 0;
      *(undefined4 *)(param_1 + 600) = 0;
    }
    FUN_00468540(param_1);
    *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xf) = 1;
  }
  return;
}



/* 0x004686e0 FUN_004686e0 */

void * FUN_004686e0(void *param_1)

{
  void *this;
  void *pvVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1f9b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  this = operator_new(0x2ac);
  pvVar1 = (void *)0x0;
  local_4 = 0;
  if (this != (void *)0x0) {
    pvVar1 = FUN_004683f0(this,param_1);
  }
  local_4 = 0xffffffff;
  FUN_0049d450((int)pvVar1);
  ExceptionList = local_c;
  return pvVar1;
}



/* 0x00468870 FUN_00468870 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00468870(int param_1)

{
  float fVar1;
  int iVar2;
  void *this;
  char cVar3;
  undefined4 uStack_c;
  undefined4 uStack_8;
  undefined4 uStack_4;
  
  if (*(char *)(*(int *)(param_1 + 0x2a8) + 0xd) != '\0') {
    if (*(char *)(param_1 + 0x25d) != '\0') {
      fVar1 = DAT_00502b04 + DAT_00502b04 + *(float *)(param_1 + 600);
      *(float *)(param_1 + 600) = fVar1;
      if (fVar1 <= (float)_DAT_004dd808) {
        FUN_0047f960(*(void **)(param_1 + 0xb4),*(void **)(DAT_00503a78 + 4));
        FUN_00468540(param_1);
      }
      else {
        *(undefined4 *)(param_1 + 600) = 0x3f800000;
        *(undefined1 *)(param_1 + 0x25d) = 0;
      }
    }
    if (*(char *)(*(int *)(param_1 + 0x2a8) + 0x10) != '\0') {
      FUN_00468680(param_1);
    }
    if (*(char *)(*(int *)(param_1 + 0x2a8) + 0xc) == '\0') {
      this = (void *)(param_1 + 0x1c8);
    }
    else if (*(char *)(*(int *)(param_1 + 0x2a8) + 0xf) == '\0') {
      this = (void *)(param_1 + 0xc0);
    }
    else {
      this = (void *)(param_1 + 0x144);
    }
    cVar3 = '\0';
    iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
    FUN_00480090(this,iVar2,cVar3);
    FUN_0047f960(*(void **)(param_1 + 0xb4),*(void **)(DAT_00503a78 + 4));
    if (*(char *)(*(int *)(param_1 + 0x2a8) + 0xc) != '\0') {
      if (*(char *)(param_1 + 0x24d) != '\0') {
        FUN_0047f960(*(void **)(param_1 + 0xbc),*(void **)(DAT_00503a78 + 4));
      }
      iVar2 = *(int *)(param_1 + 0x2a8);
      *(undefined1 *)(param_1 + 0x25c) = *(undefined1 *)(iVar2 + 0xe);
      if (*(int *)(*(int *)(param_1 + 0xbc) + 4) == 0x11) {
        *(undefined1 *)(param_1 + 0x24d) = 0;
      }
      if (*(char *)(iVar2 + 0xf) == '\0') {
        uStack_4 = 0;
        uStack_c = 0x3f800000;
        if (*(char *)(iVar2 + 0xe) == '\0') {
          uStack_8 = 0x3f000000;
        }
        else {
          uStack_8 = 0;
        }
      }
      else {
        uStack_c = 0x3e800000;
        uStack_8 = 0x3e800000;
        uStack_4 = 0x3f800000;
      }
      FUN_0047f270(*(void **)(param_1 + 0xb8),&uStack_c);
      iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xb8));
      *(undefined4 *)(iVar2 + 0x28) = *(undefined4 *)(param_1 + 0x4c);
      *(undefined4 *)(iVar2 + 0x2c) = *(undefined4 *)(param_1 + 0x50);
      *(undefined4 *)(iVar2 + 0x30) = *(undefined4 *)(param_1 + 0x54);
      iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xb8));
      *(float *)(iVar2 + 0x2c) =
           (*(float *)(*(int *)(param_1 + 0xb4) + 0xb8) * (float)_DAT_004de6f8 -
           (float)_DAT_004e2de0) + *(float *)(iVar2 + 0x2c);
      *(undefined1 *)(*(int *)(param_1 + 0xb8) + 0xad) = 1;
      FUN_0047f960(*(void **)(param_1 + 0xb8),*(void **)(DAT_00503a78 + 4));
    }
    *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xf) = 0;
  }
  return;
}



/* 0x004688f0 FUN_004688f0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_004688f0(int param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4)

{
  int iVar1;
  int unaff_ESI;
  void *this;
  char cVar2;
  
  FUN_00468680(param_1);
  if (*(char *)(*(int *)(unaff_ESI + 0x2a8) + 0xc) == '\0') {
    this = (void *)(unaff_ESI + 0x1c8);
  }
  else if (*(char *)(*(int *)(unaff_ESI + 0x2a8) + 0xf) == '\0') {
    this = (void *)(unaff_ESI + 0xc0);
  }
  else {
    this = (void *)(unaff_ESI + 0x144);
  }
  cVar2 = '\0';
  iVar1 = FUN_0047f1a0(*(int *)(unaff_ESI + 0xb4));
  FUN_00480090(this,iVar1,cVar2);
  FUN_0047f960(*(void **)(unaff_ESI + 0xb4),*(void **)(DAT_00503a78 + 4));
  if (*(char *)(*(int *)(unaff_ESI + 0x2a8) + 0xc) != '\0') {
    if (*(char *)(unaff_ESI + 0x24d) != '\0') {
      FUN_0047f960(*(void **)(unaff_ESI + 0xbc),*(void **)(DAT_00503a78 + 4));
    }
    iVar1 = *(int *)(unaff_ESI + 0x2a8);
    *(undefined1 *)(unaff_ESI + 0x25c) = *(undefined1 *)(iVar1 + 0xe);
    if (*(int *)(*(int *)(unaff_ESI + 0xbc) + 4) == 0x11) {
      *(undefined1 *)(unaff_ESI + 0x24d) = 0;
    }
    if (*(char *)(iVar1 + 0xf) == '\0') {
      param_4 = 0;
      param_2 = 0x3f800000;
      if (*(char *)(iVar1 + 0xe) == '\0') {
        param_3 = 0x3f000000;
      }
      else {
        param_3 = 0;
      }
    }
    else {
      param_2 = 0x3e800000;
      param_3 = 0x3e800000;
      param_4 = 0x3f800000;
    }
    FUN_0047f270(*(void **)(unaff_ESI + 0xb8),&param_2);
    iVar1 = FUN_0047f1a0(*(int *)(unaff_ESI + 0xb8));
    *(undefined4 *)(iVar1 + 0x28) = *(undefined4 *)(unaff_ESI + 0x4c);
    *(undefined4 *)(iVar1 + 0x2c) = *(undefined4 *)(unaff_ESI + 0x50);
    *(undefined4 *)(iVar1 + 0x30) = *(undefined4 *)(unaff_ESI + 0x54);
    iVar1 = FUN_0047f1a0(*(int *)(unaff_ESI + 0xb8));
    *(float *)(iVar1 + 0x2c) =
         (*(float *)(*(int *)(unaff_ESI + 0xb4) + 0xb8) * (float)_DAT_004de6f8 -
         (float)_DAT_004e2de0) + *(float *)(iVar1 + 0x2c);
    *(undefined1 *)(*(int *)(unaff_ESI + 0xb8) + 0xad) = 1;
    FUN_0047f960(*(void **)(unaff_ESI + 0xb8),*(void **)(DAT_00503a78 + 4));
  }
  *(undefined1 *)(*(int *)(unaff_ESI + 0x2a8) + 0xf) = 0;
  return;
}



/* 0x00468a80 FUN_00468a80 */

void __thiscall FUN_00468a80(int param_1,int param_2)

{
  if (param_2 == 30000) {
    FUN_00468680(param_1);
  }
  return;
}



/* 0x00468ac0 FUN_00468ac0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00468ac0(int param_1)

{
  float *this;
  int *piVar1;
  int iVar2;
  float *pfVar3;
  float *pfVar4;
  undefined4 *puVar5;
  void *pvVar6;
  void *this_00;
  float10 fVar7;
  ulonglong uVar8;
  float fVar9;
  void *pvVar10;
  int *piStack_84;
  int *piStack_80;
  void *pvStack_7c;
  float fStack_78;
  float fStack_74;
  float fStack_70;
  float fStack_6c;
  float fStack_68;
  float fStack_64;
  char *apcStack_60 [3];
  float fStack_54;
  float fStack_50;
  float fStack_4c;
  float afStack_48 [3];
  undefined4 uStack_3c;
  undefined4 uStack_38;
  undefined4 uStack_34;
  undefined4 uStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  char *pcStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  float fStack_18;
  float fStack_14;
  float fStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d2044;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xd) = 0;
  *(undefined4 *)(param_1 + 600) = 0;
  *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0xf) = 0;
  *(undefined1 *)(*(int *)(param_1 + 0x2a8) + 0x10) = 0;
  *(undefined1 *)(param_1 + 0x24d) = 0;
  *(undefined1 *)(param_1 + 0x25c) = 0;
  geWorld_ModelSetFlags(*(undefined4 *)(*(int *)(param_1 + 0x2a8) + 4),0);
  iVar2 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar2 + 0x160) == '\0') {
    iVar2 = iVar2 + 8;
  }
  else {
    iVar2 = iVar2 + 0xb4;
  }
  geWorld_GetModelXForm
            (*(undefined4 *)(iVar2 + 0x78),*(undefined4 *)(*(int *)(param_1 + 0x2a8) + 4),&uStack_3c
            );
  iVar2 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar2 + 0x160) == '\0') {
    iVar2 = iVar2 + 8;
  }
  else {
    iVar2 = iVar2 + 0xb4;
  }
  geWorld_ModelGetBBox
            (*(undefined4 *)(iVar2 + 0x78),*(undefined4 *)(*(int *)(param_1 + 0x2a8) + 4),&fStack_78
             ,&fStack_6c);
  iVar2 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar2 + 0x160) == '\0') {
    iVar2 = iVar2 + 8;
  }
  else {
    iVar2 = iVar2 + 0xb4;
  }
  geWorld_GetModelRotationalCenter
            (*(undefined4 *)(iVar2 + 0x78),*(undefined4 *)(*(int *)(param_1 + 0x2a8) + 4),&fStack_54
            );
  *(float *)(param_1 + 0x4c) = fStack_18 + fStack_54;
  pvVar10 = (void *)(param_1 + 0x24);
  *(float *)(param_1 + 0x50) = fStack_14 + fStack_50;
  *(float *)(param_1 + 0x54) = fStack_10 + fStack_4c;
  *(undefined4 *)(param_1 + 0x28) = uStack_3c;
  fStack_78 = fStack_78 + _DAT_004e2928;
  *(undefined4 *)(param_1 + 0x2c) = uStack_38;
  *(undefined4 *)(param_1 + 0x30) = uStack_34;
  *(undefined4 *)(param_1 + 0x34) = uStack_30;
  fStack_74 = fStack_74 + _DAT_004e2928;
  *(undefined4 *)(param_1 + 0x38) = uStack_2c;
  *(undefined4 *)(param_1 + 0x3c) = uStack_28;
  fStack_70 = fStack_70 + _DAT_004e2928;
  *(char **)(param_1 + 0x40) = pcStack_24;
  *(undefined4 *)(param_1 + 0x44) = uStack_20;
  fStack_6c = fStack_6c - _DAT_004e2928;
  apcStack_60[2] = (char *)uStack_1c;
  *(undefined4 *)(param_1 + 0x48) = uStack_1c;
  apcStack_60[0] = pcStack_24;
  apcStack_60[1] = (char *)uStack_20;
  fStack_68 = fStack_68 - _DAT_004e2928;
  fStack_64 = fStack_64 - _DAT_004e2928;
  FUN_004964b0((int)pvVar10);
  pfVar4 = (float *)(param_1 + 0x68);
  FUN_0042d280(pfVar4,apcStack_60,fStack_78,fStack_74,fStack_70);
  this = (float *)(param_1 + 0x74);
  FUN_0042d280(this,apcStack_60,fStack_6c,fStack_68,fStack_64);
  pfVar3 = (float *)FUN_00495d20((float *)apcStack_60,(int)pvVar10,pfVar4);
  *pfVar4 = *pfVar3;
  *(float *)(param_1 + 0x6c) = pfVar3[1];
  *(float *)(param_1 + 0x70) = pfVar3[2];
  pfVar3 = (float *)FUN_00495d20((float *)apcStack_60,(int)pvVar10,this);
  *this = *pfVar3;
  *(float *)(param_1 + 0x78) = pfVar3[1];
  *(float *)(param_1 + 0x7c) = pfVar3[2];
  piStack_80 = (int *)(*this - *pfVar4);
  pvStack_7c = (void *)(*(float *)(param_1 + 0x78) - *(float *)(param_1 + 0x6c));
  piStack_84 = (int *)(*(float *)(param_1 + 0x7c) - *(float *)(param_1 + 0x70));
  if ((float)piStack_80 < (float)piStack_84) {
    FUN_00496c30(pvVar10,90.0);
    piStack_80 = piStack_84;
  }
  fVar9 = 0.5;
  pfVar4 = (float *)FUN_0042d050((float *)apcStack_60,pfVar4,this);
  puVar5 = (undefined4 *)FUN_00446390(afStack_48,pfVar4,fVar9);
  *(undefined4 *)(param_1 + 0x4c) = *puVar5;
  *(undefined4 *)(param_1 + 0x50) = puVar5[1];
  *(undefined4 *)(param_1 + 0x54) = puVar5[2];
  *(undefined4 *)(param_1 + 0x50) = *(undefined4 *)(param_1 + 0x6c);
  fVar7 = FUN_00498b30(0.0,1.0);
  piVar1 = piStack_80;
  if ((float10)_DAT_004de6f8 < fVar7) {
    piVar1 = (int *)((float)piStack_80 * _DAT_004e1608);
  }
  *(int **)(param_1 + 0x254) = piVar1;
  *(float *)(param_1 + 0x250) = (float)pvStack_7c * (float)_DAT_004e17d8;
  pvStack_7c = operator_new(0x168);
  uStack_4 = 0;
  if (pvStack_7c == (void *)0x0) {
    pvVar6 = (void *)0x0;
  }
  else {
    pvVar6 = FUN_0047f850(pvStack_7c,0x502f3c,0);
  }
  *(void **)(param_1 + 0xb4) = pvVar6;
  *(undefined4 *)((int)pvVar6 + 0xb0) = 2;
  *(undefined1 *)(*(int *)(param_1 + 0xb4) + 0xad) = 1;
  uStack_4 = 0xffffffff;
  pvVar6 = pvVar10;
  this_00 = (void *)FUN_0047f1a0(*(int *)(param_1 + 0xb4));
  FUN_00447890(this_00,(int)pvVar6);
  pvStack_7c = (void *)(DAT_00502f40 + -1);
  FUN_00498b30(0.0,(float)(int)pvStack_7c);
  uVar8 = FUN_004b9400();
  FUN_0047d300(*(void **)(param_1 + 0xb4),(int)uVar8);
  iVar2 = *(int *)(param_1 + 0xb4);
  fVar7 = FUN_00498b30(0.5,1.5);
  *(float *)(iVar2 + 0x1c) = (float)(fVar7 * (float10)*(float *)(iVar2 + 0x1c));
  *(undefined4 *)(*(int *)(param_1 + 0xb4) + 0xa4) = 0xa0;
  FUN_00468540(param_1);
  *(undefined4 *)(*(int *)(param_1 + 0xb4) + 0xe8) = 0x3c23d70a;
  *(undefined4 *)(*(int *)(param_1 + 0xb4) + 0x10c) = 0x3c23d70a;
  pvStack_7c = operator_new(0x168);
  uStack_4 = 1;
  if (pvStack_7c == (void *)0x0) {
    pvVar6 = (void *)0x0;
  }
  else {
    pvVar6 = FUN_0047f850(pvStack_7c,0x502f90,0);
  }
  *(void **)(param_1 + 0xb8) = pvVar6;
  *(undefined4 *)((int)pvVar6 + 0xa8) = 0x40200000;
  *(undefined4 *)(*(int *)(param_1 + 0xb8) + 0xa4) = 0xb0;
  uStack_4 = 0xffffffff;
  *(undefined1 *)(*(int *)(param_1 + 0xb8) + 0xad) = 0;
  pvStack_7c = operator_new(0x168);
  uStack_4 = 2;
  if (pvStack_7c == (void *)0x0) {
    pvVar6 = (void *)0x0;
  }
  else {
    pvVar6 = FUN_0047f850(pvStack_7c,0x502f20,0);
  }
  *(void **)(param_1 + 0xbc) = pvVar6;
  *(undefined4 *)((int)pvVar6 + 0xb0) = 0;
  *(undefined1 *)(*(int *)(param_1 + 0xbc) + 0xad) = 1;
  uStack_4 = 0xffffffff;
  pvVar6 = (void *)FUN_0047f1a0(*(int *)(param_1 + 0xbc));
  FUN_00447890(pvVar6,(int)pvVar10);
  *(undefined4 *)(*(int *)(param_1 + 0xbc) + 0xa8) = 0x40400000;
  *(float *)(*(int *)(param_1 + 0xbc) + 0x1c) =
       *(float *)(*(int *)(param_1 + 0xbc) + 0x1c) * (float)_DAT_004e2e00;
  *(undefined4 *)(*(int *)(param_1 + 0xbc) + 0xa4) = 0xb0;
  *(undefined4 *)(*(int *)(param_1 + 0xbc) + 0xe8) = 0x3c23d70a;
  *(undefined4 *)(*(int *)(param_1 + 0xbc) + 0x10c) = 0x3c23d70a;
  iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xbc));
  *(undefined4 *)(iVar2 + 0x28) = *(undefined4 *)(param_1 + 0x4c);
  *(undefined4 *)(iVar2 + 0x2c) = *(undefined4 *)(param_1 + 0x50);
  *(undefined4 *)(iVar2 + 0x30) = *(undefined4 *)(param_1 + 0x54);
  iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xb4));
  pvStack_7c = (void *)(*(float *)(*(int *)(param_1 + 0xb4) + 0xb8) * (float)_DAT_004e2df8 +
                       *(float *)(iVar2 + 0x2c));
  iVar2 = FUN_0047f1a0(*(int *)(param_1 + 0xbc));
  *(void **)(iVar2 + 0x2c) = pvStack_7c;
  pvVar10 = (void *)(param_1 + 0xc0);
  FUN_0047fe70(pvVar10,*(void **)(DAT_00503a78 + 4),&DAT_00502f74,'\x01');
  *(undefined4 *)(param_1 + 0xf0) = 0x14;
  FUN_0042d280((void *)(param_1 + 0x108),afStack_48,0xc1a00000,0x41a00000,0);
  FUN_0042d280((void *)(param_1 + 0x114),afStack_48,0x41a00000,0x43af0000,0x41200000);
  *(undefined4 *)(param_1 + 0x124) = 0x40400000;
  *(undefined4 *)(param_1 + 0x128) = 0x40400000;
  *(undefined4 *)(param_1 + 0x134) = 0xb0;
  *(undefined4 *)(param_1 + 0x120) = 0x3f000000;
  *(undefined4 *)(param_1 + 300) = 0x40a00000;
  *(undefined4 *)(param_1 + 0xf8) = 1;
  *(undefined4 *)(param_1 + 0xfc) = 1;
  *(undefined4 *)(param_1 + 0x100) = 0x3e800000;
  *(undefined4 *)(param_1 + 0x104) = 0x3f400000;
  FUN_0047fe70((void *)(param_1 + 0x144),*(void **)(DAT_00503a78 + 4),&DAT_00502f58,'\x01');
  *(undefined4 *)(param_1 + 0x174) = 0x14;
  FUN_00468300((void *)(param_1 + 0x144),(int)pvVar10);
  *(undefined4 *)(param_1 + 0x184) = 0x3dcccccd;
  *(undefined4 *)(param_1 + 0x188) = 0x3e99999a;
  FUN_0047fe70((void *)(param_1 + 0x1c8),*(void **)(DAT_00503a78 + 4),&DAT_00502f74,'\x01');
  *(undefined4 *)(param_1 + 0x1f8) = 0x14;
  FUN_00468300((void *)(param_1 + 0x1c8),(int)pvVar10);
  *(undefined4 *)(param_1 + 0x200) = 1;
  *(undefined4 *)(param_1 + 0x204) = 1;
  *(undefined4 *)(param_1 + 0x23c) = 0x20;
  apcStack_60[0] = "FireBurn1";
  apcStack_60[1] = "FireBurn2";
  apcStack_60[2] = "FireBurn3";
  FUN_00498b30(0.0,2.999);
  uVar8 = FUN_004b9400();
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_84 = &DAT_00501908;
  FUN_00401d80(&piStack_84,apcStack_60[(int)uVar8]);
  uStack_4 = 3;
  FUN_004a12f0((int *)(param_1 + 0x260),&piStack_84,param_1 + 0x4c,2);
  uStack_4 = 0xffffffff;
  if ((piStack_84 != &DAT_00501908) && (*piStack_84 = *piStack_84 + -1, *piStack_84 < 1)) {
    FUN_004011c0(&piStack_84);
  }
  FUN_004a11d0((int *)(param_1 + 0x260));
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piStack_80 = &DAT_00501908;
  FUN_00401d80(&piStack_80,"FlameBurst");
  uStack_4 = 4;
  FUN_004a12f0((void *)(param_1 + 0x284),&piStack_80,0,2);
  uStack_4 = 0xffffffff;
  if ((piStack_80 != &DAT_00501908) && (*piStack_80 = *piStack_80 + -1, *piStack_80 < 1)) {
    FUN_004011c0(&piStack_80);
  }
  ExceptionList = pvStack_c;
  return;
}



/* 0x0049d140 FUN_0049d140 */

undefined4 __thiscall FUN_0049d140(void *this,int *param_1)

{
  int iVar1;
  
  iVar1 = *(int *)this;
  if (iVar1 != 0) {
    *param_1 = iVar1;
    return CONCAT31((int3)((uint)iVar1 >> 8),1);
  }
  *param_1 = 0;
  return 0;
}



