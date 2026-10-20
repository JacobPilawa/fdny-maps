/* Fresh Ghidra 12.0.4 decompilation excerpts from the saved hash-pinned project.
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured 2026-10-19 UTC. Ghidra function boundaries were used; log framing
 * was removed. Generic names are preserved. This is analysis evidence, not
 * reconstructed source and not a runtime-parity claim.
 */

/* Address 00468050 */
undefined4 * __thiscall FUN_00468050(void *this,void *param_1)

{
  int iVar1;
  void *this_00;
  undefined4 *puVar2;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1f3f;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0043f610(this,(int)param_1);
  local_4 = 0;
  FUN_0046f930((undefined4 *)((int)this + 0x140));
  local_4._0_1_ = 1;
  *(undefined ***)this = &PTR_FUN_004e2d10;
  *(undefined ***)((int)this + 0x24) = &PTR_FUN_004dfc30;
  *(undefined4 *)((int)this + 0x140) = &PTR_FUN_004e2d08;
  InitializeSoundInstance((undefined4 *)((int)this + 0x19c));
  local_4._0_1_ = 2;
  InitializeSoundInstance((undefined4 *)((int)this + 0x1c0));
  local_4._0_1_ = 3;
  InitializeSoundInstance((undefined4 *)((int)this + 0x1e4));
  local_4._0_1_ = 4;
  FUN_00488f90((undefined4 *)((int)this + 0x208));
  *(undefined4 *)((int)this + 0x244) = 0;
  *(undefined4 *)((int)this + 0x248) = 0;
  *(undefined4 *)((int)this + 0x24c) = 0;
  *(undefined4 *)((int)this + 0x250) = 0;
  *(undefined4 *)((int)this + 0x254) = 0;
  *(undefined4 *)((int)this + 600) = 0;
  *(undefined4 *)((int)this + 0x260) = 0;
  *(undefined4 *)((int)this + 0x264) = 0;
  *(undefined4 *)((int)this + 0x268) = 0;
  local_4._0_1_ = 5;
  *(undefined4 *)((int)this + 0x26c) = 0;
  *(undefined4 *)((int)this + 0x270) = 0;
  *(undefined4 *)((int)this + 0x274) = 0;
  FUN_004653f0((undefined4 *)((int)this + 0x278));
  local_4._0_1_ = 6;
  FUN_0046dc80((int)this + 0x1480);
  local_4._0_1_ = 7;
  this_00 = operator_new(0x1c);
  local_4._0_1_ = 8;
  if (this_00 == (void *)0x0) {
    puVar2 = (undefined4 *)0x0;
  }
  else {
    puVar2 = FUN_0047dbd0(this_00,0x10);
  }
  local_4._0_1_ = 7;
  FUN_0047f850((void *)((int)this + 0x18c0),(int)puVar2,1);
  local_4._0_1_ = 9;
  FUN_004904c0((undefined1 *)((int)this + 0x1a28));
  *(undefined4 *)((int)this + 0x1a70) = 0x10;
  *(code **)((int)this + 0x1a6c) = FUN_00467e70;
  *(undefined4 *)((int)this + 0x1a5c) = 0;
  *(undefined4 *)((int)this + 0x1a60) = 0;
  *(undefined4 *)((int)this + 0x1a64) = 0;
  *(undefined4 *)((int)this + 0x1a68) = 0;
  local_4 = CONCAT31(local_4._1_3_,0xb);
  FUN_0049d160(param_1,(int *)((int)this + 0x194),0x14);
  iVar1 = *(int *)((int)this + 0x194);
  *(undefined4 *)((int)this + 0x4c) = *(undefined4 *)(iVar1 + 4);
  *(undefined4 *)((int)this + 0x50) = *(undefined4 *)(iVar1 + 8);
  *(undefined4 *)((int)this + 0x54) = *(undefined4 *)(iVar1 + 0xc);
  *(undefined4 *)((int)this + 0x18) = 1;
  ExceptionList = local_c;
  return this;
}

/* Address 0049D160 */
void __thiscall FUN_0049d160(void *this,int *param_1,int param_2)

{
  int iVar1;
  undefined **local_bc;
  char *local_b8;
  undefined4 local_b4;
  undefined1 local_b0;
  undefined1 local_a0 [60];
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d46ee;
  local_c = ExceptionList;
  if (*(int *)this == 0) {
    ExceptionList = &local_c;
    *param_1 = 0;
    local_bc = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_b8 = "ROC\\ROC AI\\Entity\\CreateStruct.cpp";
    local_b4 = 0x36;
    local_b0 = 0;
    local_4 = 0;
    FUN_004a3a19(local_a0,"UserData pointer is null");
    FUN_00401360(&local_bc,2);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_bc);
  }
  else {
    ExceptionList = &local_c;
    *param_1 = *(int *)this;
  }
  iVar1 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar1 + 0x160) == '\0') {
    iVar1 = iVar1 + 8;
  }
  else {
    iVar1 = iVar1 + 0xb4;
  }
  iVar1 = geEntity_GetUserDataSizeForClass
                    (*(undefined4 *)(iVar1 + 0x78),*(undefined4 *)(*(int *)((int)this + 0xc) + 8));
  if (iVar1 == 0) {
    local_bc = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_b8 = "ROC\\ROC AI\\Entity\\CreateStruct.cpp";
    local_b4 = 0x3c;
    local_b0 = 0;
    local_4 = 1;
    FUN_004a3a19(local_a0,"Cannot validate UserData because class is not registered");
    FUN_00401360(&local_bc,2);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_bc);
  }
  if (param_2 != iVar1) {
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "ROC\\ROC AI\\Entity\\CreateStruct.cpp";
    local_5c = 0x3f;
    local_58 = 0;
    local_4 = 2;
    FUN_004a3a19(local_48,"EntDefs are out of sync");
    FUN_00401360(&local_64,2);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_64);
  }
  ExceptionList = local_c;
  return;
}

/* Address 00468230 */
undefined4 * FUN_00468230(void *param_1)

{
  void *this;
  undefined4 *puVar1;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1f5b;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  this = operator_new(0x1a88);
  puVar1 = (undefined4 *)0x0;
  local_4 = 0;
  if (this != (void *)0x0) {
    puVar1 = FUN_00468050(this,param_1);
  }
  local_4 = 0xffffffff;
  FUN_0049d450((int)puVar1);
  ExceptionList = local_c;
  return puVar1;
}

/* Address 0043B2D0 */
void __cdecl FUN_0043b2d0(float *param_1,float *param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  
  fVar1 = param_2[1];
  fVar2 = param_3[1];
  fVar3 = param_2[2];
  fVar4 = param_3[2];
  *param_1 = *param_2 - *param_3;
  param_1[1] = fVar1 - fVar2;
  param_1[2] = fVar3 - fVar4;
  return;
}

/* Address 0048FF30 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl FUN_0048ff30(undefined4 *param_1,float param_2,undefined4 *param_3)

{
  int iVar1;
  
  _DAT_005031dc = *param_1;
  _DAT_005031e4 = param_1[2];
  _DAT_005031e0 = (float)param_1[1] - param_2;
  _DAT_005031ec = (float)param_1[1] + _DAT_004e170c;
  iVar1 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar1 + 0x160) == '\0') {
    iVar1 = iVar1 + 8;
  }
  else {
    iVar1 = iVar1 + 0xb4;
  }
  _DAT_005031e8 = _DAT_005031dc;
  _DAT_005031f0 = _DAT_005031e4;
  iVar1 = geWorld_Collision(*(undefined4 *)(iVar1 + 0x78),0,0,&DAT_005031e8,&DAT_005031dc,1,2,0,0,0,
                            &DAT_005031b0);
  if (iVar1 == 0) {
    return 0;
  }
  *param_3 = *param_1;
  param_3[1] = param_1[1];
  param_3[2] = param_1[2];
  param_3[1] = DAT_005031c0;
  return CONCAT31((int3)((uint)param_3 >> 8),1);
}
