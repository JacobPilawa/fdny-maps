/* Ghidra decompilation; names/types are provisional.
 * Entry: 00462a60
 * Function: FUN_00462a60
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00462a60(void *this,int *param_1)

{
  void *pvVar1;
  undefined4 local_4c;
  undefined4 local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  int local_1c;
  int local_18;
  int local_14;
  int local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d16b8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0045f660(this,param_1,-1);
  pvVar1 = (void *)((int)this + 0x1fc);
  *(undefined4 *)((int)this + 0x5d8) = 0;
  local_4c = 0x3f7d70a4;
  local_48 = 0x3f7d70a4;
  local_44 = 0x3f7d70a4;
  local_40 = 0x3e700000;
  local_3c = 0x3eb40000;
  local_38 = 0x3f000000;
  local_34 = 0x3ec80000;
  local_30 = 0x3f5c0000;
  local_2c = 0x3f480000;
  local_28 = 0x3f7f0000;
  local_24 = 0x3f7f0000;
  local_20 = 0x3f7f0000;
  FUN_0047db10(pvVar1,0x2bf,0x3dcccccd,0x3f800000,0x3f800000,&local_4c);
  FUN_0047db10(pvVar1,0x2bf,0x3f400000,0x3f800000,0x3f800000,&local_34);
  FUN_0047db10(pvVar1,0x2bf,0x3dcccccd,0x3f800000,0x3f800000,&local_40);
  *(uint *)((int)this + 0x214) = *(uint *)((int)this + 0x214) & 0xfffffffe;
  pvVar1 = (void *)((int)this + 0x218);
  FUN_0047db10(pvVar1,0x2bf,0x3dcccccd,0x3f800000,0x3f800000,&local_4c);
  FUN_0047db10(pvVar1,0x2bf,0x3f400000,0x3f800000,0x3f800000,&local_28);
  FUN_0047db10(pvVar1,0x2bf,0x3dcccccd,0x3f800000,0x3f800000,&local_40);
  *(uint *)((int)this + 0x230) = *(uint *)((int)this + 0x230) & 0xfffffffe;
  pvVar1 = (void *)((int)this + 0x234);
  FUN_0047db10(pvVar1,0xffffffff,0x3dcccccd,0x3f800000,0x3f800000,&local_4c);
  FUN_0047db10(pvVar1,0xffffffff,0x3dcccccd,0x3f800000,0x3f800000,&local_40);
  *(uint *)((int)this + 0x24c) = *(uint *)((int)this + 0x24c) & 0xfffffff6;
  FUN_0047ef60((void *)((int)this + 0x378),300,'\x01');
  FUN_0047ef60((void *)((int)this + 0x49c),0x12d,'\x01');
  FUN_0047ee10((void *)((int)this + 0x254),(int)pvVar1,'\x01');
  local_1c = 0;
  local_18 = 0x280;
  local_14 = 100;
  local_10 = 0x17c;
  FUN_0047edf0((void *)((int)this + 0x254),&local_1c,0);
  FUN_0047ea10((void *)((int)this + 0x378),local_1c,local_14 + -0x20,local_18,local_14,0);
  FUN_0047ea10((void *)((int)this + 0x49c),local_1c,local_10,local_18,local_10 + 0x20,0);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"Goggles");
  local_4 = 0;
  FUN_004a12f0((void *)((int)this + 0x5dc),&param_1,0,2);
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return;
}

