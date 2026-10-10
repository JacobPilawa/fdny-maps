/* Ghidra decompilation; names/types are provisional.
 * Entry: 00463700
 * Function: FUN_00463700
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00463700(void *this,int *param_1)

{
  void *pvVar1;
  void *this_00;
  int *piVar2;
  undefined4 uVar3;
  int *piVar4;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  void *pvStack_c;
  undefined1 *puStack_8;
  undefined4 uStack_4;
  
  uStack_4 = 0xffffffff;
  puStack_8 = &LAB_004d17f8;
  pvStack_c = ExceptionList;
  ExceptionList = &pvStack_c;
  FUN_0045f660(this,param_1,3);
  FUN_0042d280((void *)((int)this + 0x8c),&local_24,0x3f59999a,0x3f59999a,0x3f59999a);
  local_24 = 0xc1200000;
  *(undefined4 *)((int)this + 0x80) = 0xc1200000;
  local_20 = 0xc1700000;
  *(undefined4 *)((int)this + 0x84) = 0xc1700000;
  local_1c = 0x41a80000;
  *(undefined4 *)((int)this + 0x88) = 0x41a80000;
  piVar4 = (int *)0x0;
  *(undefined4 *)((int)this + 0xc68) = 0;
  uVar3 = 0xf;
  piVar2 = (int *)(**(code **)(**(int **)((int)this + 0x54) + 0x18))();
  FUN_0046d6b0((void *)((int)this + 0x608),piVar2,uVar3,piVar4);
  *(undefined4 *)((int)this + 0x9c4) = 0;
  *(undefined4 *)((int)this + 0x978) = 0;
  *(undefined4 *)((int)this + 0x97c) = 0x3f4ccccd;
  *(undefined4 *)((int)this + 0x980) = 0;
  *(undefined4 *)((int)this + 0x984) = 0x42c80000;
  *(undefined4 *)((int)this + 0x988) = 0x42f00000;
  *(undefined4 *)((int)this + 0x98c) = 0x42f00000;
  *(undefined4 *)((int)this + 0x990) = 0;
  *(undefined4 *)((int)this + 0x994) = 0x41200000;
  *(undefined4 *)((int)this + 0x998) = 0x451c4000;
  *(undefined4 *)((int)this + 0x99c) = 0x3fc00000;
  *(undefined4 *)((int)this + 0x9a0) = 0x3fe00000;
  *(undefined4 *)((int)this + 0x9a4) = 0x43960000;
  local_24 = 0x43700000;
  *(undefined4 *)((int)this + 0x9a8) = 0x43700000;
  local_20 = 0x43700000;
  *(undefined4 *)((int)this + 0x9ac) = 0x43700000;
  *(undefined4 *)((int)this + 0x9b0) = 0;
  *(undefined4 *)((int)this + 0x9b4) = 0x42200000;
  local_1c = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"ExtStart");
  uStack_4 = 0;
  FUN_004a12f0((void *)((int)this + 0x204),&param_1,0,2);
  uStack_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"HoseSpray");
  uStack_4 = 1;
  FUN_004a12f0((void *)((int)this + 0x228),&param_1,0,2);
  uStack_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"ExtEnd");
  uStack_4 = 2;
  FUN_004a12f0((void *)((int)this + 0x24c),&param_1,0,2);
  uStack_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (*param_1 = *param_1 + -1, *param_1 < 1)) {
    FUN_004011c0(&param_1);
  }
  pvVar1 = (void *)((int)this + 0x270);
  *(undefined1 *)((int)this + 0xc58) = 0;
  local_24 = 0x3f800000;
  local_20 = 0x3f800000;
  local_1c = 0x3f800000;
  FUN_0047db10(pvVar1,1,0x3f266666,0x3f000000,0x3e800000,&local_24);
  local_24 = 0x3f800000;
  local_20 = 0x3f800000;
  local_1c = 0x3f800000;
  FUN_0047db10(pvVar1,1,0,0,0x40400000,&local_24);
  *(uint *)((int)this + 0x288) = *(uint *)((int)this + 0x288) | 8;
  this_00 = (void *)((int)this + 0x28c);
  local_24 = 0x3f400000;
  local_20 = 0x3f400000;
  local_1c = 0x3f800000;
  FUN_0047db10(this_00,2,0x3f59999a,0x3f800000,0x3d99999a,&local_24);
  local_24 = 0x3f800000;
  local_20 = 0x3f800000;
  local_1c = 0x3f800000;
  FUN_0047db10(this_00,2,0,0,0x3e800000,&local_24);
  *(uint *)((int)this + 0x2a4) = *(uint *)((int)this + 0x2a4) | 8;
  FUN_0047fe70((void *)((int)this + 0x2a8),*(void **)(DAT_00503a78 + 4),pvVar1,'\x01');
  FUN_0047fe70((void *)((int)this + 0x32c),*(void **)(DAT_00503a78 + 4),this_00,'\x01');
  *(undefined4 *)((int)this + 0x2d8) = 10;
  *(undefined4 *)((int)this + 0x35c) = 0x14;
  *(undefined4 *)((int)this + 0x2e0) = 1;
  *(undefined4 *)((int)this + 0x2e4) = 1;
  *(undefined4 *)((int)this + 0x2e8) = 0x3e800000;
  *(undefined4 *)((int)this + 0x2ec) = 0x3f000000;
  FUN_0042d280((void *)((int)this + 0x2f0),&local_24,0,0,0x43960000);
  FUN_0042d280((void *)((int)this + 0x2fc),&local_24,0,0,0x43fa0000);
  *(undefined4 *)((int)this + 0x308) = 0x3f400000;
  *(undefined4 *)((int)this + 0x30c) = 0x3f800000;
  *(undefined4 *)((int)this + 0x310) = 0x3f4ccccd;
  *(undefined4 *)((int)this + 0x314) = 0x3f99999a;
  *(undefined4 *)((int)this + 0x318) = 0x3f400000;
  *(undefined4 *)((int)this + 0x31c) = 0x30;
  FUN_0042d280((void *)((int)this + 800),&local_24,0,0xc1a00000,0);
  *(undefined4 *)((int)this + 0x364) = 1;
  *(undefined4 *)((int)this + 0x368) = 4;
  *(undefined4 *)((int)this + 0x36c) = 0x3dcccccd;
  *(undefined4 *)((int)this + 0x370) = 0x3dcccccd;
  FUN_0042d280((void *)((int)this + 0x374),&local_24,0xc2480000,0xc2480000,0x43480000);
  FUN_0042d280((void *)((int)this + 0x380),&local_24,0x42480000,0x42480000,0x43fa0000);
  *(undefined4 *)((int)this + 0x38c) = 0x3f400000;
  *(undefined4 *)((int)this + 0x390) = 0x3f800000;
  *(undefined4 *)((int)this + 0x394) = 0x3f4ccccd;
  *(undefined4 *)((int)this + 0x398) = 0x3f99999a;
  *(undefined4 *)((int)this + 0x39c) = 0x3f000000;
  *(undefined4 *)((int)this + 0x3a0) = 0x30;
  FUN_0042d280((void *)((int)this + 0x3a4),&local_24,0,0xc2960000,0);
  pvVar1 = (void *)((int)this + 0x3b4);
  local_24 = 0x3f800000;
  local_20 = 0x3f800000;
  local_1c = 0x3f800000;
  FUN_0047db10(pvVar1,0xe,0x40000000,0x3f400000,0x40400000,&local_24);
  uStack_18 = 0x3f800000;
  uStack_14 = 0x3f800000;
  uStack_10 = 0x3f800000;
  FUN_0047db10(pvVar1,0xe,0,0,0x40a00000,&uStack_18);
  *(uint *)((int)this + 0x3cc) = *(uint *)((int)this + 0x3cc) | 8;
  FUN_0047fe70((void *)((int)this + 0x3d0),*(void **)(DAT_00503a78 + 4),pvVar1,'\x01');
  *(undefined4 *)((int)this + 0x400) = 10;
  *(undefined4 *)((int)this + 0x3b0) = 0;
  FUN_0048abb0((void *)((int)this + 0xb38),1,'\x01',-1);
  FUN_00469520((void *)((int)this + 0xa48),'\0');
  *(undefined4 *)((int)this + 0xa8c) = 0x41a00000;
  *(undefined4 *)((int)this + 0xa90) = 0x41f00000;
  FUN_004827b0((int)this + 0xc70);
  FUN_00459050((int)this + 0x1fc);
  ExceptionList = pvStack_c;
  return;
}

