/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0048d180 FUN_0048d180 */

void __thiscall FUN_0048d180(void *this,undefined4 param_1)

{
  *(undefined4 *)((int)this + 300) = DAT_00502afc;
  *(undefined1 *)((int)this + 0x13c) = 1;
  *(undefined4 *)((int)this + 0x108) = param_1;
  return;
}



/* 0x0048d390 FUN_0048d390 */

void __fastcall FUN_0048d390(void *param_1)

{
  float local_c [3];
  
  local_c[0] = 1.0;
  local_c[1] = 1.0;
  local_c[2] = 1.0;
  FUN_0047ee90(param_1,local_c,'\x01');
  FUN_0047dc90(param_1,'\0');
  return;
}



/* 0x0048d1a0 FUN_0048d1a0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

char __fastcall FUN_0048d1a0(void *param_1)

{
  float fVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  ulonglong uVar5;
  int iVar6;
  
  if (*(char *)((int)param_1 + 0x13c) == '\0') {
    return '\x01';
  }
  FUN_0047dc90(param_1,'\x01');
  iVar2 = DAT_00502afc - *(int *)((int)param_1 + 300);
  fVar1 = (float)iVar2;
  if (iVar2 < 0) {
    fVar1 = fVar1 + _DAT_004dfa30;
  }
  fVar1 = fVar1 / (float)*(int *)((int)param_1 + 0x130);
  if (fVar1 < (float)_DAT_004dd808 == (fVar1 == (float)_DAT_004dd808)) {
    FUN_0047ec80(param_1,0,0,0x3f800000,0x3f800000);
    iVar6 = 0;
    uVar5 = FUN_004b9400();
    iVar2 = (int)uVar5;
    uVar5 = FUN_004b9400();
    iVar3 = (int)uVar5;
    uVar5 = FUN_004b9400();
    iVar4 = (int)uVar5;
    uVar5 = FUN_004b9400();
    FUN_0047ea10(param_1,(int)uVar5,iVar4,iVar3,iVar2,iVar6);
  }
  else {
    iVar6 = 0;
    uVar5 = FUN_004b9400();
    iVar2 = (int)uVar5;
    uVar5 = FUN_004b9400();
    iVar3 = (int)uVar5;
    uVar5 = FUN_004b9400();
    iVar4 = (int)uVar5;
    uVar5 = FUN_004b9400();
    FUN_0047ea10(param_1,(int)uVar5,iVar4,iVar3,iVar2,iVar6);
    FUN_0047eb60(param_1,0x140,0xf0,fVar1 * _DAT_004e3a28);
  }
  return '\x01' - ((uint)(*(int *)((int)param_1 + 0x134) + *(int *)((int)param_1 + 0x130) +
                         *(int *)((int)param_1 + 300)) < DAT_00502afc);
}



