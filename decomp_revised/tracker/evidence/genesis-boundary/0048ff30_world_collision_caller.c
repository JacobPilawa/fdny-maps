
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

