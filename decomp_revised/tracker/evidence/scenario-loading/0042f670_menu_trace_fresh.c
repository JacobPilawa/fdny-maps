/* Ghidra headless decompilation; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */

undefined4 * __thiscall FUN_0042f670(void *this,undefined4 *param_1)

{
  undefined4 *this_00;
  void *pvVar1;
  undefined4 *puVar2;
  void *pvVar3;
  undefined4 uVar4;
  int iVar5;
  int *piVar6;
  undefined4 *local_40;
  int local_34;
  char *local_30;
  char *local_2c;
  undefined4 local_28;
  int local_1c;
  int local_18;
  undefined4 local_14;
  undefined4 local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cdec3;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_00436630(this,param_1,0xb,0xf);
  local_4 = 0;
  *(undefined ***)this = &PTR_FUN_004dfd14;
  FUN_0047dba0((undefined4 *)((int)this + 0x30));
  local_4._0_1_ = 1;
  FUN_0048c120((undefined4 *)((int)this + 0x50));
  local_4._0_1_ = 2;
  FUN_0048c120((undefined4 *)((int)this + 0x154));
  local_4._0_1_ = 3;
  FUN_0047ddc0((undefined4 *)((int)this + 0x260));
  puVar2 = (undefined4 *)((int)this + 900);
  local_4._0_1_ = 4;
  FUN_0047dba0(puVar2);
  this_00 = (undefined4 *)((int)this + 0x3a0);
  local_4._0_1_ = 5;
  FUN_0047dba0(this_00);
  local_4._0_1_ = 6;
  FUN_0047dba0((undefined4 *)((int)this + 0x3bc));
  local_4._0_1_ = 7;
  FUN_0048abb0((undefined4 *)((int)this + 0x50),1,'\0',-1);
  FUN_0048abb0((void *)((int)this + 0x154),0,'\0',-1);
  local_34 = 0x3f800000;
  local_30 = (char *)0x3f800000;
  local_2c = (char *)0x3f800000;
  FUN_0047db10(puVar2,0x33,0x3f000000,0x3f800000,0x3f800000,&local_34);
  local_34 = 0x3f800000;
  local_30 = (char *)0x3f666666;
  local_2c = (char *)0x3f800000;
  FUN_0047db10(puVar2,0x33,0x3e800000,0x3f4ccccd,0x3f800000,&local_34);
  local_34 = 0x3f800000;
  local_30 = (char *)0x3f800000;
  local_2c = (char *)0x3f800000;
  FUN_0047db10(this_00,0x34,0x3f000000,0x3f800000,0x3f800000,&local_34);
  local_34 = 0x3f800000;
  local_30 = (char *)0x3f666666;
  local_2c = (char *)0x3f800000;
  FUN_0047db10(this_00,0x34,0x3e800000,0x3f4ccccd,0x3f800000,&local_34);
  pvVar3 = (void *)((int)this + 0x3bc);
  local_34 = 0x3f800000;
  local_30 = (char *)0x3f800000;
  local_2c = (char *)0x3f800000;
  FUN_0047db10(pvVar3,0x35,0x3f000000,0x3f800000,0x3f800000,&local_34);
  local_34 = 0x3f800000;
  local_30 = (char *)0x3f666666;
  local_2c = (char *)0x3f800000;
  FUN_0047db10(pvVar3,0x35,0x3e800000,0x3f4ccccd,0x3f800000,&local_34);
  local_1c = 0x1fe;
  local_18 = 0x27e;
  local_14 = 0x1a4;
  local_10 = 0x1e4;
  pvVar1 = operator_new(0x144);
  local_4._0_1_ = 8;
  if (pvVar1 == (void *)0x0) {
    local_40 = (undefined4 *)0x0;
  }
  else {
    local_40 = FUN_00435a10(pvVar1,this,0,&local_1c,0xffffffff,(int)puVar2,0,0,0x3f7f0000,0x3f7f0000
                           );
  }
  local_4 = CONCAT31(local_4._1_3_,7);
  if (*(int *)((int)this + 0x14) < 1) {
    FUN_0043b6e0((void *)((int)this + 0x10),1);
    if (*(int *)((int)this + 0x14) < 1) {
      piVar6 = &local_34;
      for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
        *piVar6 = 0;
        piVar6 = piVar6 + 1;
      }
      local_34 = 1;
      local_30 = "Array index out of bounds";
      local_2c = "..\\BluelAPI/Container/SArray.h";
      local_28 = 0x1cd;
      FUN_00422060(&local_34);
    }
  }
  piVar6 = *(int **)((int)this + 0x10);
  iVar5 = *piVar6;
  if (iVar5 == 0) {
    *piVar6 = (int)local_40;
  }
  else {
    *piVar6 = (int)local_40;
    FUN_0048d9d0((void *)((int)this + 0x10),iVar5);
  }
  pvVar1 = operator_new(0x144);
  local_4._0_1_ = 9;
  if (pvVar1 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_00435a10(pvVar1,this,2,&local_1c,0xffffffff,(int)this_00,0,0,0x3f7f0000,0x3f7f0000)
    ;
  }
  local_4 = CONCAT31(local_4._1_3_,7);
  if (*(int *)((int)this + 0x14) < 3) {
    FUN_0043b6e0((void *)((int)this + 0x10),3);
    if (*(int *)((int)this + 0x14) < 3) {
      piVar6 = &local_34;
      for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
        *piVar6 = 0;
        piVar6 = piVar6 + 1;
      }
      local_34 = 1;
      local_30 = "Array index out of bounds";
      local_2c = "..\\BluelAPI/Container/SArray.h";
      local_28 = 0x1cd;
      FUN_00422060(&local_34);
    }
  }
  iVar5 = *(int *)(*(int *)((int)this + 0x10) + 8);
  *(undefined4 **)(*(int *)((int)this + 0x10) + 8) = puVar2;
  if (iVar5 != 0) {
    FUN_0048d9d0((void *)((int)this + 0x10),iVar5);
  }
  iVar5 = local_18 - local_1c;
  local_1c = local_1c - iVar5;
  local_18 = local_18 - iVar5;
  pvVar1 = operator_new(0x144);
  local_4._0_1_ = 10;
  if (pvVar1 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_00435a10(pvVar1,this,1,&local_1c,0xffffffff,(int)pvVar3,0,0,0x3f7f0000,0x3f7f0000);
  }
  local_4 = CONCAT31(local_4._1_3_,7);
  if (*(int *)((int)this + 0x14) < 2) {
    FUN_0043b6e0((void *)((int)this + 0x10),2);
    if (*(int *)((int)this + 0x14) < 2) {
      piVar6 = &local_34;
      for (iVar5 = 6; iVar5 != 0; iVar5 = iVar5 + -1) {
        *piVar6 = 0;
        piVar6 = piVar6 + 1;
      }
      local_34 = 1;
      local_30 = "Array index out of bounds";
      local_2c = "..\\BluelAPI/Container/SArray.h";
      local_28 = 0x1cd;
      FUN_00422060(&local_34);
    }
  }
  iVar5 = *(int *)(*(int *)((int)this + 0x10) + 4);
  *(undefined4 **)(*(int *)((int)this + 0x10) + 4) = puVar2;
  if (iVar5 != 0) {
    FUN_0048d9d0((void *)((int)this + 0x10),iVar5);
  }
  param_1 = _malloc(0x24);
  puVar2 = param_1;
  for (iVar5 = 9; iVar5 != 0; iVar5 = iVar5 + -1) {
    *puVar2 = 0;
    puVar2 = puVar2 + 1;
  }
  *param_1 = 1;
  param_1[3] = 1;
  local_4._0_1_ = 0xb;
  local_34 = 3;
  local_30 = (char *)0x65;
  local_2c = (char *)0x3;
  local_28 = 0x50;
  FUN_004848c0(&param_1,&local_34);
  local_34 = 4;
  local_30 = (char *)0x64;
  local_2c = (char *)0x2;
  local_28 = 0x74;
  FUN_004848c0(&param_1,&local_34);
  FUN_00436150(this,(int *)&param_1,'\x01');
  local_34 = 0x3f800000;
  local_30 = (char *)0x3f800000;
  local_2c = (char *)0x3f800000;
  FUN_0047db10((void *)((int)this + 0x30),0x36,0x3f800000,0x3f800000,0x3f800000,&local_34);
  pvVar3 = operator_new(0x144);
  local_4._0_1_ = 0xc;
  if (pvVar3 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_00435a10(pvVar3,this,5,(int *)&DAT_004ff570,0xffffffff,(int)this + 0x30,0,0,
                          0x3f7f0000,0x3f7f0000);
  }
  iVar5 = 0;
  local_4._0_1_ = 0xb;
  if (0 < *(int *)((int)this + 0x14)) {
    piVar6 = *(int **)((int)this + 0x10);
    do {
      if (*piVar6 == 0) goto LAB_0042fc49;
      iVar5 = iVar5 + 1;
      piVar6 = piVar6 + 1;
    } while (iVar5 < *(int *)((int)this + 0x14));
  }
  FUN_0043b6e0((void *)((int)this + 0x10),iVar5 + 1);
LAB_0042fc49:
  *(undefined4 **)(*(int *)((int)this + 0x10) + iVar5 * 4) = puVar2;
  pvVar3 = operator_new(0x144);
  local_4._0_1_ = 0xd;
  if (pvVar3 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_00435a10(pvVar3,this,6,(int *)&DAT_004ff580,0xffffffff,(int)this + 0x30,0x3f800000,
                          0,0,0x3f800000);
  }
  iVar5 = 0;
  local_4._0_1_ = 0xb;
  if (0 < *(int *)((int)this + 0x14)) {
    piVar6 = *(int **)((int)this + 0x10);
    do {
      if (*piVar6 == 0) goto LAB_0042fcc8;
      iVar5 = iVar5 + 1;
      piVar6 = piVar6 + 1;
    } while (iVar5 < *(int *)((int)this + 0x14));
  }
  FUN_0043b6e0((void *)((int)this + 0x10),iVar5 + 1);
LAB_0042fcc8:
  *(undefined4 **)(*(int *)((int)this + 0x10) + iVar5 * 4) = puVar2;
  *(undefined4 *)((int)this + 600) = 0;
  uVar4 = FUN_00493d20(*(void **)(DAT_00503a78 + 4),0x17,'\x01');
  *(undefined4 *)((int)this + 0x25c) = uVar4;
  FUN_0047ef60((void *)((int)this + 0x260),0x37,'\0');
  *(undefined4 *)((int)this + 0x4c) = 0;
  local_4 = CONCAT31(local_4._1_3_,7);
  FUN_0048b180((int *)&param_1);
  ExceptionList = local_c;
  return this;
}
