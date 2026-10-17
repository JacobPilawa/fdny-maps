/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 * __thiscall FUN_00430f00(void *this,int param_1)

{
  int iVar1;
  void *pvVar2;
  undefined4 *this_00;
  undefined4 uVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int local_3c [5];
  int local_28;
  undefined4 local_24;
  undefined4 local_20;
  void *local_1c [5];
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004ce029;
  local_1c[4] = ExceptionList;
  ExceptionList = local_1c + 4;
  FUN_00436630(this,param_1,10,-1);
  local_4 = 0;
  *(undefined ***)this = &PTR_FUN_004e0588;
  FUN_0047dba0((undefined4 *)((int)this + 0x2c));
  local_4 = CONCAT31(local_4._1_3_,1);
  local_3c[0] = 0x3f800000;
  local_3c[1] = 0x3f800000;
  local_3c[2] = 0x3f800000;
  FUN_0047db10((undefined4 *)((int)this + 0x2c),100,0x3e800000,0x3f800000,0x3f800000,local_3c);
  iVar5 = 0x32;
  local_3c[0] = 100;
  local_3c[1] = 0x74;
  local_3c[2] = 0x80;
  local_3c[3] = 0x46;
  local_1c[0] = (void *)0x0;
  local_1c[1] = (void *)0x1;
  local_1c[2] = (void *)0x2;
  local_1c[3] = (void *)0x3;
  param_1 = 0;
  do {
    pvVar2 = operator_new(0x394);
    local_4._0_1_ = 2;
    if (pvVar2 == (void *)0x0) {
      this_00 = (undefined4 *)0x0;
    }
    else {
      local_28 = iVar5 + 0x80;
      local_24 = 0x1b8;
      local_20 = 0x1d8;
      local_3c[4] = iVar5;
      this_00 = FUN_00435b30(pvVar2,this,local_1c[param_1],local_3c + 4,0xffffffff,(int)this + 0x2c,
                             0,(float)param_1 * (float)_DAT_004e0508,0x3f800000,
                             (float)(param_1 + 1) * (float)_DAT_004e0508);
    }
    local_4 = CONCAT31(local_4._1_3_,1);
    FUN_00435990(this_00,5,local_3c[param_1] + -5);
    iVar6 = 0;
    if (0 < *(int *)((int)this + 0x14)) {
      piVar4 = *(int **)((int)this + 0x10);
      do {
        if (*piVar4 == 0) goto LAB_004310a4;
        iVar6 = iVar6 + 1;
        piVar4 = piVar4 + 1;
      } while (iVar6 < *(int *)((int)this + 0x14));
    }
    FUN_0043b6e0((void *)((int)this + 0x10),iVar6 + 1);
LAB_004310a4:
    iVar1 = local_3c[param_1];
    param_1 = param_1 + 1;
    *(undefined4 **)(*(int *)((int)this + 0x10) + iVar6 * 4) = this_00;
    iVar5 = iVar1 + 0x28 + iVar5;
  } while (param_1 < 4);
  pvVar2 = operator_new(0x20);
  local_4._0_1_ = 3;
  if (pvVar2 == (void *)0x0) {
    uVar3 = 0;
  }
  else {
    local_1c[0] = (void *)0x136;
    local_1c[1] = (void *)0x1c7;
    local_1c[2] = (void *)0xcb;
    local_1c[3] = (void *)0x106;
    uVar3 = FUN_004357d0(pvVar2,this,4,local_1c);
  }
  iVar5 = 0;
  local_4 = CONCAT31(local_4._1_3_,1);
  if (0 < *(int *)((int)this + 0x14)) {
    piVar4 = *(int **)((int)this + 0x10);
    do {
      if (*piVar4 == 0) goto LAB_00431141;
      iVar5 = iVar5 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar5 < *(int *)((int)this + 0x14));
  }
  FUN_0043b6e0((void *)((int)this + 0x10),iVar5 + 1);
LAB_00431141:
  *(undefined4 *)(*(int *)((int)this + 0x10) + iVar5 * 4) = uVar3;
  ExceptionList = local_1c[4];
  return this;
}

 (GhidraScript)
