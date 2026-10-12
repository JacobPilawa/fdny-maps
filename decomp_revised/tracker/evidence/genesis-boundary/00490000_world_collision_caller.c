
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __cdecl
FUN_00490000(undefined4 *param_1,undefined4 *param_2,float param_3,undefined4 *param_4,
            undefined4 *param_5)

{
  int iVar1;
  
  if ((float)_DAT_004e16b8 <= param_3) {
    _DAT_00503224 = &DAT_0050324c;
    _DAT_00503240 = param_3 * (float)_DAT_004de6f8;
    _DAT_00503220 = &DAT_00503240;
    _DAT_0050324c = -_DAT_00503240;
    _DAT_00503244 = _DAT_00503240;
    _DAT_00503248 = _DAT_00503240;
    _DAT_00503250 = _DAT_0050324c;
    _DAT_00503254 = _DAT_0050324c;
  }
  else {
    _DAT_00503220 = (undefined *)0x0;
    _DAT_00503224 = (undefined *)0x0;
  }
  _DAT_00503234 = *param_1;
  _DAT_00503238 = param_1[1];
  _DAT_0050323c = param_1[2];
  _DAT_00503228 = *param_2;
  _DAT_0050322c = param_2[1];
  _DAT_00503230 = param_2[2];
  iVar1 = *(int *)(DAT_00503a78 + 4);
  if (*(char *)(iVar1 + 0x160) == '\0') {
    iVar1 = iVar1 + 8;
  }
  else {
    iVar1 = iVar1 + 0xb4;
  }
  iVar1 = geWorld_Collision(*(undefined4 *)(iVar1 + 0x78),_DAT_00503224,_DAT_00503220,&DAT_00503234,
                            &DAT_00503228,1,2,0,0,0,&DAT_005031f4);
  if (iVar1 == 0) {
    return 0;
  }
  *param_4 = DAT_00503200;
  param_4[1] = DAT_00503204;
  param_4[2] = DAT_00503208;
  *param_5 = DAT_00503210;
  param_5[1] = DAT_00503214;
  param_5[2] = DAT_00503218;
  return CONCAT31((int3)((uint)param_5 >> 8),1);
}

