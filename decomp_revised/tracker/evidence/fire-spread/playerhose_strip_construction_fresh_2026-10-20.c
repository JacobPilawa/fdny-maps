/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00469f50 FUN_00469f50 */

void __thiscall FUN_00469f50(void *this,undefined4 *param_1,undefined4 param_2)

{
  int *piVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 local_10;
  undefined4 local_c;
  undefined4 local_8;
  undefined4 local_4;
  
  piVar1 = (int *)((int)this + 0x14);
  if (*(int *)(*(int *)((int)this + 0x14) + 4) <= *(int *)((int)this + 0x10)) {
    local_c = 0;
    local_8 = 0;
    local_4 = 0;
    local_10 = 0x3f800000;
    do {
      FUN_00469d30(piVar1,&local_c);
      FUN_00454330((void *)((int)this + 0x18),&local_10);
    } while (*(int *)(*piVar1 + 4) <= *(int *)((int)this + 0x10));
  }
  iVar3 = *(int *)((int)this + 0x10);
  FUN_00484800(piVar1,iVar3);
  if (1 < *(int *)*piVar1) {
    FUN_00469960(piVar1);
  }
  puVar2 = (undefined4 *)(*piVar1 + 0x14 + iVar3 * 0xc);
  *puVar2 = *param_1;
  puVar2[1] = param_1[1];
  puVar2[2] = param_1[2];
  iVar3 = *(int *)((int)this + 0x10);
  piVar1 = (int *)((int)this + 0x18);
  FUN_00484800(piVar1,iVar3);
  if (1 < *(int *)*piVar1) {
    FUN_0041aff0(piVar1);
  }
  *(undefined4 *)(*piVar1 + 0x14 + iVar3 * 4) = param_2;
  *(int *)((int)this + 0x10) = *(int *)((int)this + 0x10) + 1;
  return;
}



/* 0x004692f0 FUN_004692f0 */

void __fastcall FUN_004692f0(int param_1)

{
  *(undefined4 *)(param_1 + 0x10) = 0;
  return;
}



/* 0x0046c480 FUN_0046c480 */

void __thiscall FUN_0046c480(void *this,int param_1)

{
  int *this_00;
  
  *(undefined4 *)((int)this + 0x25c) = 0xc9742400;
  *(undefined4 *)((int)this + 0x280) = 0xc9742400;
  *(undefined4 *)((int)this + 0x2a4) = 0xc9742400;
  *(undefined4 *)((int)this + 0x2c8) = 0xc9742400;
  *(undefined4 *)((int)this + 0x2a0) = 0;
  this_00 = (int *)((int)this + 0x240);
  *(undefined4 *)((int)this + 0x260) = 0;
  *(undefined4 *)((int)this + 0x284) = 0;
  *(undefined4 *)((int)this + 0x2a8) = 0;
  *(undefined4 *)((int)this + 0x2cc) = 0;
  *(undefined4 *)((int)this + 600) = 0;
  *(undefined4 *)((int)this + 0x27c) = 0;
  *(undefined4 *)((int)this + 0x2c4) = 0;
  FUN_0048d490(this_00,param_1);
  gePoly_SetLVertex(*(undefined4 *)(*this_00 + param_1 * 4),0,(undefined4 *)((int)this + 600));
  FUN_0048d490(this_00,param_1);
  gePoly_SetLVertex(*(undefined4 *)(*this_00 + param_1 * 4),1,(undefined4 *)((int)this + 0x2a0));
  FUN_0048d490(this_00,param_1);
  gePoly_SetLVertex(*(undefined4 *)(*this_00 + param_1 * 4),2,(undefined4 *)((int)this + 0x2c4));
  FUN_0048d490(this_00,param_1);
  gePoly_SetLVertex(*(undefined4 *)(*this_00 + param_1 * 4),3,(undefined4 *)((int)this + 0x27c));
  return;
}



/* 0x0046a250 FUN_0046a250 */

undefined4 * __fastcall FUN_0046a250(undefined4 *param_1)

{
  undefined4 *puVar1;
  int iVar2;
  undefined4 local_18 [3];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d2083;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_00491bb0(param_1);
  *param_1 = &PTR_FUN_004e2e0c;
  puVar1 = _malloc(0x20);
  param_1[5] = puVar1;
  for (iVar2 = 8; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0;
    puVar1 = puVar1 + 1;
  }
  *(undefined4 *)param_1[5] = 1;
  *(undefined4 *)(param_1[5] + 0xc) = 1;
  puVar1 = _malloc(0x18);
  param_1[6] = puVar1;
  for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
    *puVar1 = 0;
    puVar1 = puVar1 + 1;
  }
  *(undefined4 *)param_1[6] = 1;
  *(undefined4 *)(param_1[6] + 0xc) = 1;
  param_1[7] = 0;
  param_1[8] = 0;
  param_1[9] = 0;
  param_1[4] = 0;
  FUN_0042d280(param_1 + 7,local_18,0x437f0000,0x437f0000,0x437f0000);
  *(undefined1 *)(param_1 + 0xe) = 0;
  *(undefined1 *)(param_1 + 0xc) = 1;
  param_1[0xd] = 4;
  ExceptionList = local_c;
  return param_1;
}



/* 0x00493d00 FUN_00493d00 */

void __thiscall FUN_00493d00(void *this,int param_1,char param_2)

{
  if (*(char *)((int)this + 0x160) != '\0') {
    FUN_00493650((void *)((int)this + 0xb4),param_1,param_2);
    return;
  }
  FUN_00493650((void *)((int)this + 8),param_1,param_2);
  return;
}



