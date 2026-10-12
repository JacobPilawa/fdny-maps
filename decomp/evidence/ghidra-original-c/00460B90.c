/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 00460b90
 * Match: function entry
 * Entry: 00460b90 FUN_00460b90
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00460b90(void *param_1)

{
  ushort uVar1;
  float *pfVar2;
  float *pfVar3;
  undefined4 *puVar4;
  undefined4 uVar5;
  int iVar6;
  int iVar7;
  int *piVar8;
  uint uVar9;
  void *pvVar10;
  float fVar11;
  int *local_100;
  float local_fc;
  float local_f8;
  float local_f4;
  float local_f0;
  float local_ec;
  float local_e8;
  int iStack_e4;
  int *piStack_e0;
  char local_d9;
  float local_d8;
  float local_d4;
  float local_d0;
  float local_cc [12];
  float local_9c [3];
  undefined1 *puStack_90;
  float local_8c [3];
  float local_80 [3];
  float local_74 [3];
  undefined4 local_68 [3];
  float local_5c [3];
  undefined1 auStack_50 [68];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1521;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_0045fec0(param_1,1.0);
  if ((float)_DAT_004e28b0 <= *(float *)((int)param_1 + 0x340)) {
    ExceptionList = local_c;
    return;
  }
  if (*(float *)((int)param_1 + 0x33c) <= (float)_DAT_004e28b0) {
    ExceptionList = local_c;
    return;
  }
  iVar6 = *(int *)((int)param_1 + 0x54);
  fVar11 = 20.0;
  pvVar10 = (void *)(iVar6 + 0x34a4);
  local_f0 = 0.0;
  local_ec = 0.0;
  local_e8 = 0.0;
  local_cc[0] = 0.0;
  local_cc[1] = 0.0;
  local_cc[2] = 0.0;
  local_fc = 0.0;
  local_f8 = 0.0;
  local_f4 = 0.0;
  local_cc[3] = 0.0;
  local_cc[4] = 0.0;
  local_cc[5] = 0.0;
  pfVar2 = (float *)FUN_00447830(pvVar10,&local_d8);
  pfVar2 = (float *)FUN_00446390(local_cc + 6,pfVar2,fVar11);
  pfVar2 = (float *)FUN_0042d050(local_80,(float *)(iVar6 + 0x34cc),pfVar2);
  local_fc = *pfVar2;
  local_f8 = pfVar2[1];
  fVar11 = 10.0;
  local_f4 = pfVar2[2];
  pfVar2 = (float *)FUN_00446ee0(pvVar10,local_80);
  local_100 = (int *)((uint)(*(char *)((int)param_1 + 0x1fc) != '\0') * 2 + -1);
  pfVar2 = (float *)FUN_00447710(local_5c,(float)(int)local_100,pfVar2);
  pfVar2 = (float *)FUN_00446390(local_8c,pfVar2,fVar11);
  local_fc = local_fc + *pfVar2;
  fVar11 = 60.0;
  local_f8 = local_f8 + pfVar2[1];
  local_f4 = local_f4 + pfVar2[2];
  pfVar2 = (float *)FUN_00447830(pvVar10,local_8c);
  pfVar2 = (float *)FUN_00446390(local_74,pfVar2,fVar11);
  fVar11 = 100.0;
  pfVar3 = (float *)FUN_0049e770(pvVar10,local_68);
  pfVar3 = (float *)FUN_00446390(local_9c,pfVar3,fVar11);
  puVar4 = (undefined4 *)FUN_0043b2d0(local_cc + 9,pfVar3,pfVar2);
  local_cc[3] = (float)*puVar4;
  local_cc[4] = (float)puVar4[1];
  local_cc[5] = (float)puVar4[2];
  local_cc[6] = 6.0;
  local_cc[7] = 6.0;
  local_cc[8] = 6.0;
  local_d8 = -6.0;
  local_d4 = -6.0;
  local_d0 = -6.0;
  uVar5 = FUN_00492ce0(*(void **)(DAT_00503a78 + 4),&local_fc,&local_d8,local_cc + 6,local_cc + 3,
                       &local_f0,local_cc);
  local_d9 = '\x01' - ((char)uVar5 != '\0');
  if (local_d9 != '\0') {
    ExceptionList = local_c;
    return;
  }
  FUN_00402790(&local_100,"Axe Hits");
  local_4 = 0;
  FUN_00486980((void *)(*(int *)(DAT_00502aac + 0x6e4) + 0x30),(int *)&local_100,1);
  local_4 = 0xffffffff;
  if ((local_100 != &DAT_00501908) && (*local_100 = *local_100 + -1, *local_100 < 1)) {
    FUN_004011c0(&local_100);
  }
  piVar8 = *(int **)((int)param_1 + 0x54);
  fVar11 = 6.0;
  pfVar2 = (float *)FUN_0049e770(piVar8 + 0xd29,local_cc + 9);
  pfVar2 = (float *)FUN_00446390(local_9c,pfVar2,fVar11);
  local_f0 = local_f0 + *pfVar2;
  local_ec = local_ec + pfVar2[1];
  local_e8 = local_e8 + pfVar2[2];
  *(undefined4 *)((int)param_1 + 0x36c) = DAT_00502afc;
  iVar6 = (**(code **)(*piVar8 + 0x18))();
  FUN_00447890((void *)((int)param_1 + 0x374),iVar6);
  local_100 = (int *)((int)param_1 + 0x15c);
  FUN_00483a20(local_100,0,'\x01');
  FUN_0045fb90(param_1,1.99);
  FUN_0042d2d0(auStack_50,0);
  pfVar2 = (float *)FUN_00446390(local_cc + 9,local_cc,-3.0);
  pfVar3 = (float *)FUN_0043b2d0(local_9c,&local_f0,(float *)((int)param_1 + 0x350));
  FUN_0042d050(&local_d8,pfVar3,pfVar2);
  iVar6 = (int)param_1 + 0x200;
  *(float *)((int)param_1 + 0x184) = local_d8 + *(float *)((int)param_1 + 0x184);
  *(float *)((int)param_1 + 0x188) = local_d4 + *(float *)((int)param_1 + 0x188);
  *(float *)((int)param_1 + 0x18c) = local_d0 + *(float *)((int)param_1 + 0x18c);
  iVar7 = FUN_00444c00(iVar6);
  *(float *)(iVar7 + 0x28) = *(float *)((int)param_1 + 0x350);
  *(undefined4 *)(iVar7 + 0x2c) = *(undefined4 *)((int)param_1 + 0x354);
  puStack_90 = &stack0xfffffee4;
  *(undefined4 *)(iVar7 + 0x30) = *(undefined4 *)((int)param_1 + 0x358);
  local_4 = 1;
  piVar8 = (int *)FUN_00444c00(iVar6);
  local_4 = 0xffffffff;
  (**(code **)(*piVar8 + 0xc))();
  FUN_00445c40(iVar6);
  thunk_FUN_0049f340((int *)((int)param_1 + 0x3b8));
  *(undefined4 *)((int)param_1 + 0x368) = 2;
  FUN_00481c00(&iStack_e4);
  local_4 = 2;
  piVar8 = piStack_e0;
  do {
    if ((iStack_e4 == 0) || (piVar8 == (int *)0x0)) {
      iVar6 = 0;
    }
    else {
      iVar6 = *piVar8;
    }
    if ((iStack_e4 != 0) && (piVar8 != (int *)0x0)) {
      piVar8 = (int *)piVar8[1];
      piStack_e0 = piVar8;
    }
    if (iVar6 == 0) {
      thunk_FUN_0049f340((int *)((int)param_1 + 0x400));
      FUN_004a11a0((int *)((int)param_1 + 0x400),(undefined1 *)0x0);
      goto LAB_004610a7;
    }
  } while (((*(int *)(iVar6 + 0x18) != 4) ||
           (uVar1 = FUN_004816c0((void *)(iVar6 + 0x240),&local_f0,8.0), piVar8 = piStack_e0,
           (char)uVar1 == '\0')) || (uVar9 = FUN_0043e2a0(iVar6), piVar8 = piStack_e0, uVar9 == 0));
  if ((uVar9 & 4) == 0) {
    if ((uVar9 & 1) == 0) {
      if ((uVar9 & 2) == 0) goto LAB_0046105a;
      pvVar10 = (void *)((int)param_1 + 0x3dc);
    }
    else {
      pvVar10 = (void *)((int)param_1 + 0x424);
    }
    FUN_004a11a0(pvVar10,(undefined1 *)0x0);
  }
LAB_0046105a:
  if ((uVar9 & 1) != 0) {
    *(undefined4 *)((int)param_1 + 0x368) = 3;
    *(undefined4 *)((int)param_1 + 0x33c) = 0x3f000000;
    FUN_00483a20(local_100,*(int *)((int)param_1 + 0x54) + 0x34a4,'\0');
  }
  DAT_00502e04 = 1;
LAB_004610a7:
  local_4 = 0xffffffff;
  if (iStack_e4 != 0) {
    FUN_00429fe0((void *)(iStack_e4 + 0x28),(int)&iStack_e4);
  }
  ExceptionList = local_c;
  return;
}


