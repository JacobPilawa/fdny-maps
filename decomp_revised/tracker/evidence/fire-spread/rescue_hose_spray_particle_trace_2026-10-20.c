/*
   Current-hash particle-template evidence excerpt, exported from the existing
   Ghidra 12.0.4 project in read-only headless mode.

   Input: inputs/original/binaries/FDNYFirefighter.exe
   SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
   The project was opened with -readOnly -noanalysis. These are selected fresh
   decompiler bodies; the note beside this file records interpretation and
   unresolved runtime/material semantics.
*/

/* Ghidra function export 00463700 */
INFO  TraceExtinguisherParticle.java> 
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

/* Ghidra function export 0047D2B0 */
int __thiscall FUN_0047d2b0(void *this,int *param_1)

{
  int iVar1;
  int iVar2;
  uint3 uVar3;
  
  if (*(int *)((int)this + 0xc) == 0) {
    return 0;
  }
  iVar1 = *(int *)(*(int *)((int)this + 0xc) + 4);
  iVar2 = *param_1;
  uVar3 = (uint3)((uint)iVar2 >> 8);
  if (iVar2 < iVar1) {
    return (uint)uVar3 << 8;
  }
  if ((*(byte *)((int)this + 0x18) & 1) == 0) {
    *param_1 = iVar1 + -1;
    return CONCAT31(uVar3,1);
  }
  if (iVar1 == 1) {
    *param_1 = 0;
    return CONCAT31(uVar3,1);
  }
  *param_1 = iVar2 % iVar1;
  return CONCAT31((int3)((uint)(iVar2 / iVar1) >> 8),1);
}

 (GhidraScript)

/* Ghidra function export 0047D330 */
INFO  TraceExtinguisherParticle.java> 
void __fastcall FUN_0047d330(void *param_1)

{
  int *piVar1;
  void *this;
  int extraout_ECX;
  
  piVar1 = (int *)((int)param_1 + 4);
  *(undefined4 *)((int)param_1 + 0x18) = *(undefined4 *)(*(int *)((int)param_1 + 0xc) + 0x18);
  *piVar1 = 0;
  FUN_0047d2b0(param_1,piVar1);
  *(int *)((int)this + 8) = *piVar1 + 1;
  FUN_0047d2b0(this,(int *)((int)this + 8));
  *(undefined4 *)(extraout_ECX + 0x14) = DAT_00502afc;
  *(undefined4 *)(extraout_ECX + 0x1c) = 0x3f800000;
  *(undefined1 *)(extraout_ECX + 0x30) = 1;
  *(undefined4 *)(extraout_ECX + 0x20) = 0x3f800000;
  return;
}

/* Ghidra function export 0047DB10 */
INFO  TraceExtinguisherParticle.java> 
void __thiscall
FUN_0047db10(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 *param_5)

{
  undefined4 *puVar1;
  int *piVar2;
  int iVar3;
  
  puVar1 = operator_new(0x1c);
  if (puVar1 == (undefined4 *)0x0) {
    puVar1 = (undefined4 *)0x0;
  }
  else {
    puVar1[4] = 0;
    puVar1[5] = 0;
    puVar1[6] = 0;
    *puVar1 = param_1;
    puVar1[1] = param_2;
    puVar1[2] = param_3;
    puVar1[3] = param_4;
    puVar1[4] = *param_5;
    puVar1[5] = param_5[1];
    puVar1[6] = param_5[2];
  }
  iVar3 = 0;
  if (0 < *(int *)((int)this + 4)) {
    piVar2 = *(int **)this;
    do {
      if (*piVar2 == 0) {
        *(undefined4 **)(*(int *)this + iVar3 * 4) = puVar1;
        return;
      }
      iVar3 = iVar3 + 1;
      piVar2 = piVar2 + 1;
    } while (iVar3 < *(int *)((int)this + 4));
  }
  FUN_0043b6e0(this,iVar3 + 1);
  *(undefined4 **)(*(int *)this + iVar3 * 4) = puVar1;
  return;
}

/* Ghidra function export 0047D660 */
INFO  TraceExtinguisherParticle.java> 
void __fastcall FUN_0047d660(void *param_1)

{
  int *piVar1;
  int iVar2;
  ulonglong uVar3;
  
  piVar1 = (int *)((int)param_1 + 4);
  FUN_0048d490(*(void **)((int)param_1 + 0xc),*(int *)((int)param_1 + 4));
  uVar3 = FUN_004b9400();
  if ((uint)uVar3 <= DAT_00502afc) {
    *(uint *)((int)param_1 + 0x14) = DAT_00502afc;
    *piVar1 = *piVar1 + 1;
    iVar2 = FUN_0047d2b0(param_1,piVar1);
    if (((char)iVar2 != '\0') && ((*(byte *)((int)param_1 + 0x18) & 8) != 0)) {
      *(undefined1 *)((int)param_1 + 0x30) = 0;
    }
    *(int *)((int)param_1 + 8) = *piVar1 + 1;
    FUN_0047d2b0(param_1,(int *)((int)param_1 + 8));
  }
  return;
}

/* Ghidra function export 0047D6D0 */
INFO  TraceExtinguisherParticle.java> 
void __thiscall FUN_0047d6d0(void *this,undefined1 *param_1,undefined1 *param_2)

{
  ulonglong uVar1;
  
  FUN_0048d490(*(void **)((int)this + 0xc),*(int *)((int)this + 4));
  uVar1 = FUN_004b9400();
  *param_1 = (char)uVar1;
  FUN_0048d490(*(void **)((int)this + 0xc),*(int *)((int)this + 8));
  uVar1 = FUN_004b9400();
  *param_2 = (char)uVar1;
  if ((*(byte *)((int)this + 0x18) & 6) != 0) {
    FUN_0048d490(*(void **)((int)this + 0xc),*(int *)((int)this + 4));
    if ((*(uint *)((int)this + 0x18) & 2) == 0) {
      if ((*(uint *)((int)this + 0x18) & 4) != 0) {
        uVar1 = FUN_004b9400();
        *param_1 = (char)uVar1;
      }
    }
    else {
      uVar1 = FUN_004b9400();
      *param_1 = (char)uVar1;
      uVar1 = FUN_004b9400();
      *param_2 = (char)uVar1;
    }
    uVar1 = FUN_004b9400();
    *param_1 = (char)uVar1;
    uVar1 = FUN_004b9400();
    *param_2 = (char)uVar1;
  }
  return;
}

/* Ghidra function export 0047D8A0 */
INFO  TraceExtinguisherParticle.java> 
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float10 __fastcall FUN_0047d8a0(int param_1)

{
  int *piVar1;
  int iVar2;
  int *this;
  float fVar3;
  float fVar4;
  int iVar5;
  
  iVar5 = DAT_00502afc;
  if ((*(byte *)(param_1 + 0x18) & 0x10) == 0) {
    iVar5 = *(int *)(param_1 + 4);
    piVar1 = *(int **)(param_1 + 0xc);
    FUN_0048d490(piVar1,iVar5);
    return (float10)*(float *)(*(int *)(*piVar1 + iVar5 * 4) + 0xc);
  }
  iVar2 = *(int *)(param_1 + 4);
  piVar1 = *(int **)(param_1 + 0xc);
  FUN_0048d490(piVar1,iVar2);
  iVar5 = iVar5 - *(int *)(param_1 + 0x14);
  fVar4 = (float)iVar5;
  if (iVar5 < 0) {
    fVar4 = fVar4 + _DAT_004dfa30;
  }
  iVar5 = *(int *)(param_1 + 4);
  this = *(int **)(param_1 + 0xc);
  fVar4 = fVar4 / (*(float *)(*(int *)(*piVar1 + iVar2 * 4) + 4) * *(float *)(param_1 + 0x1c) *
                  _DAT_004e0ed4);
  FUN_0048d490(this,iVar5);
  iVar2 = *(int *)(param_1 + 8);
  piVar1 = *(int **)(param_1 + 0xc);
  fVar3 = *(float *)(*(int *)(*this + iVar5 * 4) + 0xc);
  FUN_0048d490(piVar1,iVar2);
  return ((float10)_DAT_004dfc24 - (float10)fVar4) * (float10)fVar3 +
         (float10)*(float *)(*(int *)(*piVar1 + iVar2 * 4) + 0xc) * (float10)fVar4;
}

/* Ghidra function export 0047D820 */
INFO  TraceExtinguisherParticle.java> 
undefined4 __thiscall FUN_0047d820(void *this,void *param_1)

{
  int iVar1;
  int *this_00;
  undefined4 uVar2;
  
  iVar1 = *(int *)((int)this + 4);
  this_00 = *(int **)((int)this + 0xc);
  FUN_0048d490(this_00,iVar1);
  iVar1 = **(int **)(*this_00 + iVar1 * 4);
  if (iVar1 < 0) {
    return 0;
  }
  uVar2 = FUN_00493d00(param_1,iVar1,'\x01');
  return uVar2;
}

/* Ghidra function export 0047F500 */
INFO  TraceExtinguisherParticle.java> 
void __fastcall FUN_0047f500(void *param_1)

{
  undefined4 local_c [3];
  
  *(undefined4 *)((int)param_1 + 0x74) = 0;
  FUN_0047d330(param_1);
  *(undefined4 *)((int)param_1 + 0xa8) = 0x3f800000;
  FUN_0042d280((void *)((int)param_1 + 0xb4),local_c,0x3f800000,0x3f800000,0x3f800000);
  FUN_0042d280((void *)((int)param_1 + 0x7c),local_c,0,0,0);
  FUN_0042d280((void *)((int)param_1 + 0x88),local_c,0,0,0);
  FUN_0042d280((void *)((int)param_1 + 0x94),local_c,0,0,0);
  *(undefined1 *)((int)param_1 + 0xa0) = 0;
  *(undefined4 *)((int)param_1 + 0xa4) = 0;
  *(undefined1 *)((int)param_1 + 0xcc) = 0;
  *(undefined1 *)((int)param_1 + 0xd4) = 0;
  *(undefined1 *)((int)param_1 + 0x30) = 1;
  *(undefined1 *)((int)param_1 + 0xad) = 1;
  *(undefined1 *)((int)param_1 + 0x78) = 1;
  return;
}

/* Ghidra function export 0047F960 */
INFO  TraceExtinguisherParticle.java> 
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0047f960(void *this,void *param_1)

{
  float fVar1;
  undefined4 uVar2;
  float fVar3;
  float fVar4;
  float *pfVar5;
  int iVar6;
  int iVar7;
  float10 fVar8;
  ulonglong uVar9;
  byte local_1a;
  byte local_19;
  float local_18;
  undefined4 local_14;
  float local_10;
  float local_c;
  float local_8;
  float local_4;
  
  if (*(char *)((int)this + 0xad) != '\0') {
    FUN_0047d660(this);
  }
  if (*(char *)((int)this + 0xa0) != '\0') {
    FUN_0047f2a0((int)this);
  }
  if (*(char *)((int)this + 0x30) != '\0') {
    fVar1 = *(float *)((int)this + 0xb0);
    if (fVar1 == 0.0) {
      local_14 = 2;
      *(undefined4 *)((int)this + 0xd8) = *(undefined4 *)((int)this + 0x5c);
      *(undefined4 *)((int)this + 0xdc) = *(undefined4 *)((int)this + 0x60);
      *(undefined4 *)((int)this + 0xe0) = *(undefined4 *)((int)this + 100);
      local_18 = 1.4013e-45;
    }
    else if (fVar1 == 1.4013e-45) {
      local_14 = 0;
      if (*(char *)((int)this + 0x78) != '\0') {
        *(undefined1 *)((int)this + 0x78) = 0;
      }
      local_18 = 5.60519e-45;
    }
    else if (fVar1 == 2.8026e-45) {
      local_14 = 0;
      FUN_0047f5a0((int)this);
      local_18 = 5.60519e-45;
    }
    else if (fVar1 == 4.2039e-45) {
      local_14 = 0;
      local_18 = fVar1;
    }
    else if (fVar1 == 5.60519e-45) {
      local_14 = 0;
      local_18 = fVar1;
    }
    fVar1 = local_18;
    if (*(char *)((int)this + 0xac) != '\0') {
      iVar6 = 0;
      local_8 = 0.0;
      local_4 = 0.0;
      local_c = _DAT_004dc50c;
      if (3 < (int)local_18) {
        iVar7 = 3;
        pfVar5 = (float *)((int)this + 0xdc);
        do {
          iVar7 = iVar7 + 4;
          iVar6 = iVar6 + 4;
          local_c = local_c + pfVar5[-1] + pfVar5[8] + pfVar5[0x11] + pfVar5[0x1a];
          local_8 = local_8 + *pfVar5 + pfVar5[9] + pfVar5[0x12] + pfVar5[0x1b];
          local_4 = local_4 + pfVar5[1] + pfVar5[10] + pfVar5[0x13] + pfVar5[0x1c];
          pfVar5 = pfVar5 + 0x24;
        } while (iVar7 < (int)local_18);
      }
      if (iVar6 < (int)local_18) {
        iVar7 = (int)local_18 - iVar6;
        pfVar5 = (float *)((int)this + iVar6 * 0x24 + 0xdc);
        do {
          local_c = local_c + pfVar5[-1];
          iVar7 = iVar7 + -1;
          local_8 = local_8 + *pfVar5;
          local_4 = local_4 + pfVar5[1];
          pfVar5 = pfVar5 + 9;
        } while (iVar7 != 0);
      }
      iVar6 = (int)param_1 + 0xb4;
      local_18 = (float)_DAT_004dd808 / (float)(int)local_18;
      local_c = local_18 * local_c;
      local_8 = local_18 * local_8;
      local_4 = local_18 * local_4;
      if (*(char *)((int)param_1 + 0x160) == '\0') {
        iVar6 = (int)param_1 + 8;
      }
      geWorld_GetLeaf(*(undefined4 *)(iVar6 + 0x78),&local_c,&local_18);
      iVar6 = (int)param_1 + 0xb4;
      if (*(char *)((int)param_1 + 0x160) == '\0') {
        iVar6 = (int)param_1 + 8;
      }
      iVar6 = geWorld_MightSeeLeaf(*(undefined4 *)(iVar6 + 0x78),local_18);
      if (iVar6 == 0) {
        return;
      }
    }
    FUN_0047d6d0(this,&local_1a,&local_19);
    pfVar5 = (float *)FUN_0047d420((int)this);
    local_c = *pfVar5;
    local_8 = pfVar5[1];
    local_4 = pfVar5[2];
    if (*(char *)((int)this + 0xcc) != '\0') {
      local_c = *(float *)((int)this + 0xc0);
      local_8 = *(float *)((int)this + 0xc4);
      local_4 = *(float *)((int)this + 200);
    }
    if (*(char *)((int)this + 0xd4) != '\0') {
      uVar9 = FUN_004b9400();
      local_1a = (byte)uVar9;
    }
    fVar3 = local_c * _DAT_004e3524;
    *(float *)((int)this + 0xec) = fVar3;
    fVar4 = local_8 * _DAT_004e3524;
    *(float *)((int)this + 0xf0) = fVar4;
    local_10 = local_4 * _DAT_004e3524;
    *(float *)((int)this + 0xf4) = local_10;
    local_18 = (float)local_1a;
    *(float *)((int)this + 0x160) = local_10;
    *(float *)((int)this + 0xf8) = local_18;
    *(float *)((int)this + 0x164) = local_18;
    *(float *)((int)this + 0x110) = fVar3;
    *(float *)((int)this + 0x114) = fVar4;
    *(float *)((int)this + 0x118) = local_10;
    *(float *)((int)this + 0x11c) = local_18;
    *(float *)((int)this + 0x134) = fVar3;
    *(float *)((int)this + 0x138) = fVar4;
    *(float *)((int)this + 0x13c) = local_10;
    *(float *)((int)this + 0x140) = local_18;
    *(float *)((int)this + 0x158) = fVar3;
    *(float *)((int)this + 0x15c) = fVar4;
    local_18 = (float)FUN_0047d820(this,param_1);
    iVar6 = (int)param_1 + 0xb4;
    if (*(char *)((int)param_1 + 0x160) == '\0') {
      iVar6 = (int)param_1 + 8;
    }
    uVar2 = *(undefined4 *)(iVar6 + 0x78);
    fVar8 = FUN_0047d8a0((int)this);
    geWorld_AddPolyOnce(uVar2,(int)this + 0xd8,fVar1,local_18,local_14,
                        *(undefined4 *)((int)this + 0xa4),
                        (float)(fVar8 * (float10)*(float *)((int)this + 0xa8)));
    if ((*(byte *)((int)this + 0x18) & 2) != 0) {
      iVar6 = FUN_0047d3e0((int)this);
      iVar7 = FUN_0047d400((int)this);
      if (iVar6 != iVar7) {
        pfVar5 = (float *)FUN_0047d560((int)this);
        local_c = *pfVar5;
        local_8 = pfVar5[1];
        local_4 = pfVar5[2];
        if (*(char *)((int)this + 0xcc) != '\0') {
          local_c = *(float *)((int)this + 0xc0);
          local_8 = *(float *)((int)this + 0xc4);
          local_4 = *(float *)((int)this + 200);
        }
        fVar3 = local_c * _DAT_004e3524;
        *(float *)((int)this + 0xec) = fVar3;
        fVar4 = local_8 * _DAT_004e3524;
        *(float *)((int)this + 0xf0) = fVar4;
        local_18 = local_4 * _DAT_004e3524;
        *(float *)((int)this + 0xf4) = local_18;
        *(float *)((int)this + 0x160) = local_18;
        local_10 = (float)local_19;
        *(float *)((int)this + 0xf8) = local_10;
        *(float *)((int)this + 0x164) = local_10;
        *(float *)((int)this + 0x110) = fVar3;
        *(float *)((int)this + 0x114) = fVar4;
        *(float *)((int)this + 0x118) = local_18;
        *(float *)((int)this + 0x11c) = local_10;
        *(float *)((int)this + 0x134) = fVar3;
        *(float *)((int)this + 0x138) = fVar4;
        *(float *)((int)this + 0x13c) = local_18;
        *(float *)((int)this + 0x140) = local_10;
        *(float *)((int)this + 0x158) = fVar3;
        *(float *)((int)this + 0x15c) = fVar4;
        local_18 = (float)FUN_0047d860(this,param_1);
        iVar6 = (int)param_1 + 0xb4;
        if (*(char *)((int)param_1 + 0x160) == '\0') {
          iVar6 = (int)param_1 + 8;
        }
        uVar2 = *(undefined4 *)(iVar6 + 0x78);
        fVar8 = FUN_0047d960((int)this);
        geWorld_AddPolyOnce(uVar2,(int)this + 0xd8,fVar1,local_18,local_14,
                            *(undefined4 *)((int)this + 0xa4),
                            (float)(fVar8 * (float10)*(float *)((int)this + 0xa8)));
      }
    }
  }
  return;
}

/* Ghidra function export 0047FEA0 */
INFO  TraceExtinguisherParticle.java> 
void __fastcall FUN_0047fea0(int param_1)

{
  int iVar1;
  int iVar2;
  int *piVar3;
  int local_18;
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  if (((*(int *)(param_1 + 0x28) != 0) && (*(int *)(param_1 + 0x2c) != 0)) &&
     (iVar2 = 0, 0 < *(int *)(param_1 + 0x14))) {
    do {
      if ((iVar2 < 0) || (*(int *)(param_1 + 0x14) <= iVar2)) {
        piVar3 = &local_18;
        for (iVar1 = 6; iVar1 != 0; iVar1 = iVar1 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        local_18 = 1;
        local_14 = "Array index out of bounds";
        local_10 = "..\\BluelAPI/Container/SArray.h";
        local_c = 0x1cd;
        FUN_00422060(&local_18);
      }
      FUN_0047f960(*(void **)(*(int *)(param_1 + 0x10) + iVar2 * 4),*(void **)(param_1 + 0x28));
      iVar2 = iVar2 + 1;
    } while (iVar2 < *(int *)(param_1 + 0x14));
  }
  return;
}
