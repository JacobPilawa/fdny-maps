/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00449340
 * Match: function entry
 * Entry: 00449340 FUN_00449340
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00449340(void *param_1)

{
  void *this;
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  char cVar5;
  float *pfVar6;
  float *pfVar7;
  undefined4 *puVar8;
  int *piVar9;
  int iVar10;
  int iVar11;
  bool bVar12;
  float10 extraout_ST0;
  ulonglong uVar13;
  float fVar14;
  void *local_6c;
  undefined4 *local_68;
  float fStack_64;
  float fStack_60;
  float local_5c;
  float local_58;
  float local_54;
  float local_50;
  undefined4 local_4c;
  undefined4 local_48;
  float local_44;
  float fStack_40;
  float afStack_38 [3];
  float fStack_2c;
  int iStack_28;
  int iStack_24;
  int iStack_20;
  float *pfStack_1c;
  int iStack_18;
  int iStack_14;
  int iStack_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cf95b;
  local_c = ExceptionList;
  fVar14 = DAT_00502b04;
  if (*(int *)((int)param_1 + 0x70) != 4) {
    fVar14 = DAT_00502b04 * _DAT_004e0510;
  }
  fVar2 = fVar14 * *(float *)((int)param_1 + 0xfc) + *(float *)((int)param_1 + 0xe8);
  fVar14 = *(float *)((int)param_1 + 0xec);
  ExceptionList = &local_c;
  *(float *)((int)param_1 + 0xe8) = fVar2;
  fVar1 = *(float *)((int)param_1 + 0x108);
  fVar3 = (float)_DAT_004de6f8;
  pfVar6 = (float *)FUN_00446390(&local_44,(float *)((int)param_1 + 0xdc),fVar2);
  FUN_0042d050(&local_50,(float *)((int)param_1 + 0xf0),pfVar6);
  iVar10 = *(int *)((int)param_1 + 0x100);
  pfVar6 = (float *)(iVar10 + 0x4c);
  FUN_0043b2d0(&local_5c,pfVar6,&local_50);
  cVar5 = FUN_0043b340(pfVar6,&local_50);
  if (cVar5 != '\0') {
    fVar4 = _DAT_004dfc24 / SQRT(local_5c * local_5c + local_58 * local_58 + local_54 * local_54);
    local_5c = local_5c * fVar4;
    local_58 = local_58 * fVar4;
    local_54 = local_54 * fVar4;
    pfVar7 = (float *)FUN_0043b2d0(&local_44,&local_5c,(float *)((int)param_1 + 0xdc));
    if (_DAT_004dfc24 < SQRT(pfVar7[2] * pfVar7[2] + pfVar7[1] * pfVar7[1] + *pfVar7 * *pfVar7)) {
      local_50 = *pfVar6;
      local_4c = *(undefined4 *)(iVar10 + 0x50);
      local_48 = *(undefined4 *)(iVar10 + 0x54);
      *(undefined4 *)((int)param_1 + 0x70) = 0;
      puVar8 = (undefined4 *)FUN_0049d350(param_1,&local_5c);
      FUN_00442bf0(*(void **)((int)param_1 + 0x100),*puVar8,puVar8[1]);
      goto LAB_004494d0;
    }
  }
  if (fVar2 - fVar14 <= fVar1 * fVar3) {
    ExceptionList = local_c;
    return;
  }
LAB_004494d0:
  *(undefined4 *)((int)param_1 + 0xec) = *(undefined4 *)((int)param_1 + 0xe8);
  this = (void *)((int)param_1 + 300);
  FUN_00447850(this,&local_44);
  piVar9 = FUN_0049b760(&local_68,"RCS_FireRescue",100);
  local_4 = 0;
  puVar8 = (undefined4 *)FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar9);
  local_4 = 0xffffffff;
  FUN_0049b6f0((int)&local_68);
  iVar10 = (**(code **)*puVar8)();
  iVar11 = FUN_0044a6a0((int)this);
  *(undefined4 *)(iVar10 + 0x7c) = *(undefined4 *)(iVar11 + 0x28);
  iVar11 = FUN_0044a6a0((int)this);
  *(undefined4 *)(iVar10 + 0x80) = *(undefined4 *)(iVar11 + 0x2c);
  iVar11 = FUN_0044a6a0((int)this);
  *(undefined4 *)(iVar10 + 0x84) = *(undefined4 *)(iVar11 + 0x30);
  *(undefined4 *)(iVar10 + 0xc) = 0;
  iVar11 = FUN_00447850(this,&local_5c);
  *(undefined4 *)(iVar10 + 0x14) = *(undefined4 *)(iVar11 + 4);
  puVar8 = (undefined4 *)FUN_00447850(this,&local_5c);
  *(undefined4 *)(iVar10 + 0x10) = *puVar8;
  *(undefined4 *)(iVar10 + 4) = 0;
  *(undefined4 *)(iVar10 + 8) = 0;
  fVar14 = -0.5;
  pfVar6 = (float *)FUN_00446390(&local_5c,(float *)((int)param_1 + 0xdc),
                                 *(float *)((int)param_1 + 0x110));
  pfVar6 = (float *)FUN_00446390(afStack_38,pfVar6,fVar14);
  iVar11 = FUN_0044a6a0((int)this);
  FUN_0042d050((float *)&local_68,(float *)(iVar11 + 0x28),pfVar6);
  fVar14 = 0.5;
  pfVar6 = (float *)FUN_00446390(afStack_38,(float *)((int)param_1 + 0xdc),
                                 *(float *)((int)param_1 + 0x110));
  pfVar6 = (float *)FUN_00446390(&fStack_2c,pfVar6,fVar14);
  iVar11 = FUN_0044a6a0((int)this);
  FUN_0042d050(&local_5c,(float *)(iVar11 + 0x28),pfVar6);
  local_5c = local_5c - (float)local_68;
  local_58 = local_58 - fStack_64;
  local_54 = local_54 - fStack_60;
  fVar14 = SQRT(local_5c * local_5c + local_58 * local_58 + local_54 * local_54);
  fVar1 = (float)_DAT_004dd808 / fVar14;
  local_5c = local_5c * fVar1;
  *(float *)(iVar10 + 0x70) = local_5c;
  local_58 = local_58 * fVar1;
  *(float *)(iVar10 + 0x74) = local_58;
  local_54 = local_54 * fVar1;
  *(float *)(iVar10 + 0x78) = local_54;
  *(undefined4 *)(iVar10 + 0x6c) = 1;
  *(undefined4 *)(iVar10 + 0x1c) = *(undefined4 *)(*(int *)((int)param_1 + 0x78) + 0x1c);
  *(undefined4 *)(iVar10 + 0x20) = 0;
  if ((*(int *)(*(int *)((int)param_1 + 0x78) + 0x20) != 0) &&
     (iVar11 = *(int *)((int)param_1 + 0x118) + 1, *(int *)((int)param_1 + 0x118) = iVar11,
     1 < iVar11)) {
    *(undefined4 *)((int)param_1 + 0x118) = 0;
    *(undefined4 *)(iVar10 + 0x20) = 1;
  }
  iStack_18 = iVar10 + 0x3c;
  iStack_10 = iVar10 + 0x5c;
  iStack_28 = iVar10 + 0x48;
  iStack_14 = iVar10 + 0x4c;
  iStack_20 = iVar10 + 0x68;
  *(undefined4 *)(iVar10 + 0x28) = 0;
  fStack_2c = (float)(iVar10 + 0x38);
  pfStack_1c = (float *)(iVar10 + 0x2c);
  iStack_24 = iVar10 + 0x58;
  if (*(int *)((int)param_1 + 0x1b8) == 0) {
    *pfStack_1c = *(float *)((int)param_1 + 0x108);
    (&pfStack_1c)[*(int *)(iVar10 + 0x28)][1] = *(float *)((int)param_1 + 0x10c);
    (&pfStack_1c)[*(int *)(iVar10 + 0x28)][2] = fVar14;
    *(undefined4 *)(&fStack_2c)[*(int *)(iVar10 + 0x28)] = 0;
  }
  else {
    *pfStack_1c = local_44;
    (&pfStack_1c)[*(int *)(iVar10 + 0x28)][1] = fStack_40;
    (&pfStack_1c)[*(int *)(iVar10 + 0x28)][2] = fVar14;
    *(undefined4 *)(&fStack_2c)[*(int *)(iVar10 + 0x28)] = 0;
    iVar11 = *(int *)(iVar10 + 0x28) + 1;
    *(int *)(iVar10 + 0x28) = iVar11;
    *(&pfStack_1c)[iVar11] = *(float *)((int)param_1 + 0x108);
    (&pfStack_1c)[*(int *)(iVar10 + 0x28)][1] = *(float *)((int)param_1 + 0x10c);
    (&pfStack_1c)[*(int *)(iVar10 + 0x28)][2] = fVar14;
    *(int *)(&fStack_2c)[*(int *)(iVar10 + 0x28)] =
         *(int *)(*(int *)((int)param_1 + 0x1b8) + 4) - DAT_00502afc;
  }
  iVar11 = *(int *)(iVar10 + 0x28) + 1;
  *(int *)(iVar10 + 0x28) = iVar11;
  *(&pfStack_1c)[iVar11] = *(float *)((int)param_1 + 0x108);
  (&pfStack_1c)[*(int *)(iVar10 + 0x28)][1] = *(float *)((int)param_1 + 0x10c);
  (&pfStack_1c)[*(int *)(iVar10 + 0x28)][2] = fVar14;
  iVar11 = *(int *)(iVar10 + 0x28);
  uVar13 = FUN_004b9400();
  *(int *)(&fStack_2c)[iVar11] = (int)uVar13;
  iVar11 = *(int *)(iVar10 + 0x28) + 1;
  *(int *)(iVar10 + 0x28) = iVar11;
  *(&pfStack_1c)[iVar11] = 0.0;
  (&pfStack_1c)[*(int *)(iVar10 + 0x28)][1] = 0.0;
  (&pfStack_1c)[*(int *)(iVar10 + 0x28)][2] = (float)extraout_ST0;
  iVar11 = *(int *)(iVar10 + 0x28);
  uVar13 = FUN_004b9400();
  *(int *)(&fStack_2c)[iVar11] = (int)uVar13;
  *(int *)(iVar10 + 0x28) = *(int *)(iVar10 + 0x28) + 1;
  local_68 = operator_new(0x14);
  local_4 = 1;
  if (local_68 == (undefined4 *)0x0) {
    local_6c = (void *)0x0;
  }
  else {
    local_6c = (void *)FUN_0049f230(local_68);
  }
  local_4 = 0xffffffff;
  piVar9 = FUN_0049b760(&local_5c,"RCS_FireRescue",100);
  local_4 = 2;
  local_68 = (undefined4 *)FUN_00429c00(*(void **)(DAT_00503a78 + 0x14),piVar9);
  bVar12 = local_68 != (undefined4 *)0x0;
  local_4 = 0xffffffff;
  FUN_0049b6f0((int)&local_5c);
  if (bVar12) {
    *(undefined4 **)((int)local_6c + 0xc) = local_68;
  }
  *(undefined4 *)((int)local_6c + 8) = 0;
  FUN_0049d130(local_6c,iVar10);
  FUN_0049d4c0(local_6c);
  FUN_0044ad70((int)this);
  local_5c = 0.0;
  local_58 = 0.0;
  local_54 = 0.0;
  FUN_0044b620(this,&local_5c,DAT_00502afc - 10);
  uVar13 = FUN_004b9400();
  FUN_0044b620(this,(undefined4 *)((int)param_1 + 0x108),(int)uVar13 + DAT_00502afc);
  iVar10 = FUN_0044a6a0((int)this);
  *(float *)(iVar10 + 0x28) = local_50;
  *(undefined4 *)(iVar10 + 0x2c) = local_4c;
  *(undefined4 *)(iVar10 + 0x30) = local_48;
  local_5c = 0.0;
  local_58 = 4.0;
  local_54 = 0.0;
  puVar8 = (undefined4 *)FUN_0042d050((float *)&pfStack_1c,&local_50,&local_5c);
  *(undefined4 *)((int)param_1 + 0x7c) = *puVar8;
  *(undefined4 *)((int)param_1 + 0x80) = puVar8[1];
  *(undefined4 *)((int)param_1 + 0x84) = puVar8[2];
  if (*(int **)((int)param_1 + 0x114) != (int *)0x0) {
    thunk_FUN_0049f340(*(int **)((int)param_1 + 0x114));
    FUN_004a11d0(*(int **)((int)param_1 + 0x114));
  }
  FUN_0044a710((int)this);
  if (*(int *)(*(int *)((int)param_1 + 0x78) + 0x1c) != 0) {
    FUN_00448630((int)param_1);
  }
  ExceptionList = local_c;
  return;
}


