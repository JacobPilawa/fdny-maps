
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

bool __cdecl FUN_0048fea0(int param_1,undefined4 *param_2,undefined4 *param_3,char param_4)

{
  int iVar1;
  
  _DAT_005031a4 = *param_2;
  _DAT_005031a8 = param_2[1];
  _DAT_005031ac = param_2[2];
  _DAT_00503198 = *param_3;
  _DAT_0050319c = param_3[1];
  _DAT_005031a0 = param_3[2];
  if (*(char *)(param_1 + 0x160) == '\0') {
    iVar1 = param_1 + 8;
  }
  else {
    iVar1 = param_1 + 0xb4;
  }
  iVar1 = geWorld_Collision(*(undefined4 *)(iVar1 + 0x78),0,0,&DAT_005031a4,&DAT_00503198,0x43,
                            (param_4 != '\0') - 1U & 2,0,0,0,&DAT_0050316c);
  return (bool)('\x01' - (iVar1 != 0));
}

