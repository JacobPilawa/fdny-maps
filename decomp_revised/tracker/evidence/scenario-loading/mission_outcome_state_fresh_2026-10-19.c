/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00458400 FUN_00458400 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00458400(int param_1)

{
  float fVar1;
  char cVar2;
  int *piVar3;
  int *piVar4;
  undefined4 *puVar5;
  int *extraout_ECX;
  int *extraout_ECX_00;
  int *extraout_ECX_01;
  int *extraout_ECX_02;
  int *extraout_ECX_03;
  int *extraout_ECX_04;
  void *pvVar6;
  int iVar7;
  byte *pbVar8;
  undefined1 uVar9;
  undefined4 uVar10;
  int iVar11;
  int *local_4c;
  int *local_48;
  int local_44;
  int local_40;
  undefined4 local_3c;
  int local_34;
  int local_30;
  int local_28;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0c08;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  if (DAT_00502a78 != '\0') {
    ExceptionList = &local_c;
    FUN_0047dc90((void *)(param_1 + 0xcf8),_DAT_00502b0c < (float)_DAT_004e21b0);
  }
  if (*(char *)(param_1 + 0x960) != '\0') {
    FUN_00455b40((void *)(param_1 + 0x10bc),*(int *)(*(int *)(param_1 + 4) + 0x3438) == 2);
    FUN_00481f10((void *)(param_1 + 0xe40),'\0');
    FUN_0040cb80((void *)(param_1 + 0x1cf0),0);
    FUN_0047dc90((void *)(param_1 + 0x1df4),'\0');
    FUN_0047dc90((void *)(param_1 + 0x1f18),'\0');
    FUN_0047dc90((void *)(param_1 + 0x203c),'\0');
    *(undefined1 *)(param_1 + 0x2188) = 0;
    iVar7 = 0;
    if (0 < *(int *)(param_1 + 0x2884)) {
      do {
        if ((iVar7 < 0) || (*(int *)(param_1 + 0x2884) <= iVar7)) {
          piVar3 = &local_24;
          for (iVar11 = 6; iVar11 != 0; iVar11 = iVar11 + -1) {
            *piVar3 = 0;
            piVar3 = piVar3 + 1;
          }
          local_24 = 1;
          local_20 = "Array index out of bounds";
          local_1c = "..\\BluelAPI/Container/SArray.h";
          local_18 = 0x1cd;
          FUN_00422060(&local_24);
        }
        FUN_0047dc90(*(void **)(*(int *)(param_1 + 0x2880) + iVar7 * 4),'\0');
        iVar7 = iVar7 + 1;
      } while (iVar7 < *(int *)(param_1 + 0x2884));
    }
    iVar7 = 0;
    if (0 < *(int *)(param_1 + 0x289c)) {
      do {
        if ((iVar7 < 0) || (*(int *)(param_1 + 0x289c) <= iVar7)) {
          piVar3 = &local_24;
          for (iVar11 = 6; iVar11 != 0; iVar11 = iVar11 + -1) {
            *piVar3 = 0;
            piVar3 = piVar3 + 1;
          }
          local_24 = 1;
          local_20 = "Array index out of bounds";
          local_1c = "..\\BluelAPI/Container/SArray.h";
          local_18 = 0x1cd;
          FUN_00422060(&local_24);
        }
        FUN_0040cb80(*(void **)(*(int *)(param_1 + 0x2898) + iVar7 * 4),0);
        iVar7 = iVar7 + 1;
      } while (iVar7 < *(int *)(param_1 + 0x289c));
    }
    cVar2 = FUN_0048d1a0((void *)(param_1 + 0x824));
    if (cVar2 == '\0') {
      FUN_004295b0(DAT_00502aac,*(int *)(*(int *)(param_1 + 4) + 0x3438) == 1);
    }
    ExceptionList = local_c;
    return;
  }
  iVar7 = *(int *)(*(int *)(param_1 + 4) + 0x3438);
  if (iVar7 == 1) {
    FUN_0042a730(DAT_00502aac,(undefined4 *)0x3,(int *)&DAT_00502ab4);
    FUN_0048d180((void *)(param_1 + 0x824),0x47);
    cVar2 = '\x01';
    local_48 = (int *)&stack0xffffff9c;
    piVar3 = extraout_ECX;
    FUN_00402790(&stack0xffffff9c,"RC_ResLvlWin");
    FUN_004897c0((void *)(*(int *)(param_1 + 4) + 0x3540),piVar3,cVar2);
    pvVar6 = (void *)(param_1 + 0x7dc);
LAB_004584fe:
    FUN_004a11a0(pvVar6,(undefined1 *)0x0);
  }
  else if (iVar7 == 2) {
    FUN_0042a730(DAT_00502aac,(undefined4 *)0x3,(int *)&DAT_00502ab4);
    FUN_0048d180((void *)(param_1 + 0x824),0x48);
    cVar2 = '\x01';
    local_48 = (int *)&stack0xffffff9c;
    piVar3 = extraout_ECX_00;
    FUN_00402790(&stack0xffffff9c,"RC_ResLvlLose");
    FUN_004897c0((void *)(*(int *)(param_1 + 4) + 0x3540),piVar3,cVar2);
    pvVar6 = (void *)(param_1 + 0x800);
    goto LAB_004584fe;
  }
  fVar1 = (*(float *)(*(int *)(param_1 + 4) + 0x3450) - *(float *)(param_1 + 0x28b0)) *
          (float)_DAT_004e0508 + *(float *)(param_1 + 0x28b0);
  *(float *)(param_1 + 0x28b0) = fVar1;
  if (fVar1 <= (float)_DAT_004e2188) {
    FUN_00482480((void *)(param_1 + 0x28d0),'\x01');
    FUN_00482430((void *)(param_1 + 0x28d0),*(float *)(param_1 + 0x28b0));
    piVar3 = extraout_ECX_02;
  }
  else {
    FUN_00482480((void *)(param_1 + 0x28d0),'\0');
    piVar3 = extraout_ECX_01;
  }
  if (_DAT_004e2180 <= *(float *)(param_1 + 0x28b0)) {
    *(undefined1 *)(param_1 + 0x29fc) = 0;
    FUN_0047d300(*(void **)(param_1 + 0x29f0),0);
    piVar3 = extraout_ECX_03;
  }
  else {
    fVar1 = _DAT_004e2180 - *(float *)(param_1 + 0x28b0);
    *(undefined1 *)(param_1 + 0x29fc) = 1;
    *(float *)(*(int *)(param_1 + 0x29f0) + 0x1c) =
         (_DAT_004dfc24 - fVar1 * _DAT_004e0510) * (float)_DAT_004e0eb8 + (float)_DAT_004df7b8;
  }
  if ((float)_DAT_004de6f8 <= *(float *)(param_1 + 0x28b0)) {
    *(undefined1 *)(param_1 + 0x2b34) = 0;
  }
  if ((*(float *)(param_1 + 0x28b0) < (float)_DAT_004e0508) && (*(char *)(param_1 + 0x2b34) == '\0')
     ) {
    cVar2 = '\x01';
    local_48 = (int *)&stack0xffffff9c;
    FUN_00402790(&stack0xffffff9c,"RC_OxygenLow");
    FUN_004897c0((void *)(*(int *)(param_1 + 4) + 0x3540),piVar3,cVar2);
    *(undefined1 *)(param_1 + 0x2b34) = 1;
  }
  local_4c = *(int **)(param_1 + 0x6b8);
  if (local_4c < DAT_00502afc) {
    local_48 = DAT_00502afc;
    fVar1 = (float)(int)DAT_00502afc;
    if ((int)DAT_00502afc < 0) {
      fVar1 = fVar1 + _DAT_004dfa30;
    }
    if (*(float *)(param_1 + 0x6b4) * _DAT_004e0ed4 + (float)(int)local_4c <= fVar1)
    goto LAB_0045865e;
    uVar9 = 1;
  }
  else {
LAB_0045865e:
    uVar9 = 0;
  }
  FUN_0040cb80((void *)(param_1 + 0x6d8),uVar9);
  FUN_00456090((void *)(param_1 + 0x10bc),*(float *)(*(int *)(param_1 + 4) + 0x3538));
  FUN_00455f60((void *)(param_1 + 0x1b6c));
  FUN_0047dc90((void *)(param_1 + 0xa94),*(int *)(*(int *)(param_1 + 4) + 0x3440) != 0);
  FUN_00455c00((void *)(param_1 + 0xa94));
  uVar10 = 0;
  iVar7 = 0;
  piVar3 = &local_24;
  local_48 = (int *)&stack0xffffff94;
  local_24 = 0x6e;
  local_20 = (char *)0x280;
  local_1c = (char *)0x5;
  local_18 = 0x1e0;
  piVar4 = extraout_ECX_04;
  FUN_0044e7e0(*(void **)(param_1 + 4),(undefined4 *)&stack0xffffff94);
  FUN_0048c780((void *)(param_1 + 0x24),piVar4,piVar3,iVar7,uVar10);
  FUN_0048bd40((void *)(param_1 + 0x24),&local_44);
  local_44 = local_44 + -5;
  local_40 = local_40 + 5;
  local_3c = 0;
  FUN_00455c00((void *)(param_1 + 0x964));
  pvVar6 = (void *)(param_1 + 0x22bc);
  iVar7 = 4;
  do {
    cVar2 = FUN_0047dcd0((int)pvVar6);
    if (cVar2 != '\0') {
      local_48 = (int *)((int)DAT_00502afc - *(int *)((int)pvVar6 + 0x124));
      fVar1 = (float)(int)local_48;
      iVar11 = 0;
      if ((int)local_48 < 0) {
        fVar1 = fVar1 + (float)_DAT_004dfc90;
      }
      piVar3 = FUN_00484940((void *)((int)pvVar6 + 0x128),&local_24,fVar1 * (float)_DAT_004dfa28);
      FUN_0047edf0(pvVar6,piVar3,iVar11);
    }
    pvVar6 = (void *)((int)pvVar6 + 0x130);
    iVar7 = iVar7 + -1;
  } while (iVar7 != 0);
  uVar10 = FUN_0044c220(*(int *)(param_1 + 4));
  if (((char)uVar10 == '\0') || (iVar7 = FUN_0044c150(*(int *)(param_1 + 4)), iVar7 == 0)) {
    FUN_0040cb80((void *)(param_1 + 0x277c),0);
  }
  else {
    pvVar6 = (void *)(param_1 + 0x277c);
    FUN_0040cb80(pvVar6,1);
    FUN_0047dce0(&local_34);
    local_30 = local_30 + -2;
    iVar7 = 1;
    local_28 = local_28 + 3;
    pbVar8 = (byte *)0x1;
    piVar3 = &local_34;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    local_4c = &DAT_00501908;
    local_4 = 0;
    piVar4 = (int *)FUN_0044c150(*(int *)(param_1 + 4));
    puVar5 = FUN_00402770(&local_4c,&DAT_004e2168);
    local_48 = (int *)&stack0xffffff94;
    FUN_00401cb0(&stack0xffffff94,puVar5);
    FUN_0048c2a0(pvVar6,piVar4,piVar3,pbVar8,iVar7);
    local_4 = 0xffffffff;
    if (local_4c != &DAT_00501908) {
      *local_4c = *local_4c + -1;
      if (0 < *local_4c) {
        local_4c = &DAT_00501908;
        FUN_0048ab40((int)pvVar6);
        goto LAB_004588a6;
      }
      FUN_004011c0(&local_4c);
    }
    FUN_0048ab40((int)pvVar6);
  }
LAB_004588a6:
  iVar7 = FUN_0044ebe0();
  if (iVar7 == 0) {
    FUN_0040cb80((void *)(param_1 + 0x5b0),0);
    FUN_0047dc90((void *)(param_1 + 0x480),'\0');
  }
  else {
    FUN_0044ebe0();
    FUN_004574c0((void *)(param_1 + 0x480));
  }
  if (*(int *)(*(int *)(param_1 + 4) + 0x352c) != 0) {
    FUN_004574c0((void *)(param_1 + 0x24c));
    FUN_00455ca0((void *)(param_1 + 0x1cf0));
    ExceptionList = local_c;
    return;
  }
  FUN_0040cb80((void *)(param_1 + 0x37c),0);
  FUN_0047dc90((void *)(param_1 + 0x24c),'\0');
  FUN_00455ca0((void *)(param_1 + 0x1cf0));
  ExceptionList = local_c;
  return;
}



/* 0x004576b0 FUN_004576b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_004576b0(void *this,int *param_1,int *param_2)

{
  void *pvVar1;
  undefined4 uVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  void *pvVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 *puVar11;
  int *piVar12;
  undefined4 local_64;
  undefined4 local_60;
  undefined4 local_5c;
  undefined4 local_58;
  int local_54 [13];
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d0be8;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  *(int **)((int)this + 4) = param_2;
  pvVar7 = (void *)((int)this + 0x964);
  *(int **)this = param_1;
  FUN_0047ef60(pvVar7,0x37,'\x01');
  FUN_0047ec80(pvVar7,0,0,0x3f820000,0x3f820000);
  FUN_0047dd30(pvVar7,0.75);
  pvVar1 = (void *)((int)this + 0xa8c);
  local_64 = 0x140;
  local_60 = 0x140;
  local_5c = 0xf0;
  local_58 = 0xf0;
  FUN_00485100(pvVar1,0.0,&local_64);
  local_64 = 0x100;
  local_60 = 0x180;
  local_5c = 0xb0;
  local_58 = 0x130;
  FUN_00485100(pvVar1,0.1,&local_64);
  local_64 = 0x100;
  local_60 = 0x180;
  local_5c = 0xb0;
  local_58 = 0x130;
  FUN_00485100(pvVar1,1.0,&local_64);
  local_64 = 0x1f6;
  local_60 = 0x236;
  local_5c = 5;
  local_58 = 0x45;
  FUN_00485100(pvVar1,1.1,&local_64);
  pvVar1 = (void *)((int)this + 0xa94);
  FUN_0047ef60(pvVar1,0x38,'\x01');
  FUN_0047dd30(pvVar1,0.75);
  FUN_0047ec80(pvVar1,0,0,0x3f820000,0x3f820000);
  local_64 = 0x140;
  local_60 = 0x140;
  local_5c = 0xf0;
  local_58 = 0xf0;
  pvVar1 = (void *)((int)this + 0xbbc);
  FUN_00485100(pvVar1,0.0,&local_64);
  local_64 = 0x100;
  local_60 = 0x180;
  local_5c = 0xb0;
  local_58 = 0x130;
  FUN_00485100(pvVar1,0.1,&local_64);
  local_64 = 0x100;
  local_60 = 0x180;
  local_5c = 0xb0;
  local_58 = 0x130;
  FUN_00485100(pvVar1,1.0,&local_64);
  local_64 = 0x23b;
  local_60 = 0x27b;
  local_5c = 5;
  local_58 = 0x45;
  FUN_00485100(pvVar1,1.1,&local_64);
  piVar12 = (int *)((int)this + 0x2880);
  piVar3 = (int *)0x0;
  param_1 = (int *)0x0;
  if (0 < *(int *)((int)this + 0x2884)) {
    piVar4 = (int *)*piVar12;
    do {
      param_1 = piVar3;
      if (*piVar4 == 0) goto LAB_004578f8;
      piVar3 = (int *)((int)piVar3 + 1);
      piVar4 = piVar4 + 1;
      param_1 = piVar3;
    } while ((int)piVar3 < *(int *)((int)this + 0x2884));
  }
  FUN_0043b6e0(piVar12,(int)param_1 + 1);
LAB_004578f8:
  *(void **)(*piVar12 + (int)param_1 * 4) = pvVar7;
  iVar6 = 0;
  if (0 < *(int *)((int)this + 0x2884)) {
    piVar3 = (int *)*piVar12;
    do {
      if (*piVar3 == 0) goto LAB_00457920;
      iVar6 = iVar6 + 1;
      piVar3 = piVar3 + 1;
    } while (iVar6 < *(int *)((int)this + 0x2884));
  }
  FUN_0043b6e0(piVar12,iVar6 + 1);
LAB_00457920:
  *(int *)(*piVar12 + iVar6 * 4) = (int)this + 0xa94;
  if (DAT_00502a78 != '\0') {
    pvVar7 = (void *)((int)this + 0xcf8);
    FUN_0047ef60(pvVar7,0x44c,'\x01');
    FUN_0047eac0(pvVar7,500,0);
    FUN_0047dc90(pvVar7,'\0');
    FUN_0048d9d0(piVar12,pvVar7);
  }
  local_54[0] = 0x110;
  local_54[1] = 0x170;
  local_54[2] = 0xd2;
  local_54[3] = 0x10e;
  FUN_0047ef60((void *)((int)this + 0x22bc),500,'\x01');
  FUN_0047ec80((void *)((int)this + 0x22bc),0,0,0x3f430c31,0x3ef3cf3d);
  pvVar7 = (void *)((int)this + 0x23e4);
  local_64 = 0x140;
  local_60 = 0x140;
  local_5c = 0xf0;
  local_58 = 0xf0;
  FUN_00485100(pvVar7,0.0,&local_64);
  FUN_00485100(pvVar7,0.1,local_54);
  FUN_00485100(pvVar7,1.0,local_54);
  local_64 = 0xf;
  local_60 = 0x3f;
  local_5c = 0x1a4;
  local_58 = 0x1c2;
  FUN_00485100(pvVar7,1.1,&local_64);
  FUN_0047dd30((void *)((int)this + 0x22bc),0.5);
  FUN_0047ef60((void *)((int)this + 0x23ec),500,'\x01');
  FUN_0047ec80((void *)((int)this + 0x23ec),0,0x3ef3cf3d,0x3f430c31,0x3f73cf3d);
  local_64 = 0x140;
  local_60 = 0x140;
  pvVar7 = (void *)((int)this + 0x2514);
  local_5c = 0xf0;
  local_58 = 0xf0;
  FUN_00485100(pvVar7,0.0,&local_64);
  FUN_00485100(pvVar7,0.1,local_54);
  FUN_00485100(pvVar7,1.0,local_54);
  local_64 = 0x44;
  local_60 = 0x74;
  local_5c = 0x1a4;
  local_58 = 0x1c2;
  FUN_00485100(pvVar7,1.1,&local_64);
  FUN_0047dd30((void *)((int)this + 0x23ec),0.5);
  FUN_0047ef60((void *)((int)this + 0x251c),0x1f5,'\x01');
  FUN_0047ec80((void *)((int)this + 0x251c),0,0,0x3f430c31,0x3ef3cf3d);
  pvVar7 = (void *)((int)this + 0x2644);
  local_64 = 0x140;
  local_60 = 0x140;
  local_5c = 0xf0;
  local_58 = 0xf0;
  FUN_00485100(pvVar7,0.0,&local_64);
  FUN_00485100(pvVar7,0.1,local_54);
  FUN_00485100(pvVar7,1.0,local_54);
  local_64 = 0x79;
  local_60 = 0xa9;
  local_5c = 0x1a4;
  local_58 = 0x1c2;
  FUN_00485100(pvVar7,1.1,&local_64);
  FUN_0047dd30((void *)((int)this + 0x251c),0.5);
  FUN_0047ef60((void *)((int)this + 0x264c),0x1f5,'\x01');
  FUN_0047ec80((void *)((int)this + 0x264c),0,0x3ef3cf3d,0x3f430c31,0x3f73cf3d);
  pvVar7 = (void *)((int)this + 0x2774);
  local_64 = 0x140;
  local_60 = 0x140;
  local_5c = 0xf0;
  local_58 = 0xf0;
  FUN_00485100(pvVar7,0.0,&local_64);
  FUN_00485100(pvVar7,0.1,local_54);
  FUN_00485100(pvVar7,1.0,local_54);
  local_64 = 0xae;
  local_60 = 0xde;
  local_5c = 0x1a4;
  local_58 = 0x1c2;
  FUN_00485100(pvVar7,1.1,&local_64);
  FUN_0047dd30((void *)((int)this + 0x264c),0.5);
  iVar6 = (int)this + 0x22bc;
  param_1 = (int *)0x4;
  do {
    iVar5 = 0;
    if (0 < *(int *)((int)this + 0x2884)) {
      piVar3 = (int *)*piVar12;
      do {
        if (*piVar3 == 0) goto LAB_00457c98;
        iVar5 = iVar5 + 1;
        piVar3 = piVar3 + 1;
      } while (iVar5 < *(int *)((int)this + 0x2884));
    }
    FUN_0043b6e0(piVar12,iVar5 + 1);
LAB_00457c98:
    *(int *)(*piVar12 + iVar5 * 4) = iVar6;
    iVar6 = iVar6 + 0x130;
    param_1 = (int *)((int)param_1 + -1);
  } while (param_1 != (int *)0x0);
  FUN_00455de0((void *)((int)this + 0x24c),0x1f6,0,0);
  FUN_00455de0((void *)((int)this + 0x480),0x1f6,1,1);
  pvVar7 = (void *)((int)this + 0x2b3c);
  FUN_0047ef60(pvVar7,0x1f9,'\x01');
  FUN_0047dd30(pvVar7,0.75);
  local_54[0] = 0x120;
  local_54[2] = 0x16c;
  local_54[1] = 0x160;
  local_54[3] = 0x18c;
  FUN_0047edf0(pvVar7,local_54,0);
  pvVar7 = (void *)((int)this + 0xbd4);
  local_54[4] = 400;
  local_54[6] = 0x16d;
  local_54[5] = 0x1e0;
  local_54[7] = 0x1bd;
  FUN_0047ef60(pvVar7,0x12,'\x01');
  FUN_0047edf0(pvVar7,local_54 + 4,0);
  FUN_0047ec80(pvVar7,0x3c23d70a,0x3c23d70a,0x3f7d70a4,0x3f7d70a4);
  iVar6 = *(int *)((int)this + 0x2884);
  FUN_0043b6e0(piVar12,iVar6 + 1);
  *(void **)(*piVar12 + iVar6 * 4) = pvVar7;
  FUN_0048abb0((void *)((int)this + 0x277c),3,'\x01',-1);
  param_2 = *(int **)((int)this + 0x289c);
  piVar3 = (int *)((int)this + 0x2898);
  FUN_0043b6e0(piVar3,(int)param_2 + 1);
  *(void **)(*piVar3 + (int)param_2 * 4) = (void *)((int)this + 0x277c);
  pvVar7 = (void *)((int)this + 0x28b4);
  local_64 = 0x3f800000;
  local_60 = 0x3f800000;
  local_5c = 0x3f800000;
  FUN_0047db10(pvVar7,0xc,0x3f000000,0x3f800000,0x3f800000,&local_64);
  local_64 = 0x3f800000;
  local_60 = 0x3e800000;
  local_5c = 0;
  FUN_0047db10(pvVar7,0xc,0x3f000000,0x3f800000,0x3f800000,&local_64);
  FUN_00482400((void *)((int)this + 0x28d0),*(undefined4 *)this,(int)pvVar7,0xd);
  *(undefined1 *)((int)this + 0x29fc) = 0;
  FUN_00482480((void *)((int)this + 0x28d0),'\0');
  local_54[8] = 0xf;
  local_54[9] = 0x6f;
  local_54[10] = 0x130;
  local_54[0xb] = 400;
  FUN_004824b0((void *)((int)this + 0x28d0),local_54 + 8);
  iVar6 = 0;
  if (0 < *(int *)((int)this + 0x2884)) {
    piVar4 = (int *)*piVar12;
    do {
      if (*piVar4 == 0) goto LAB_00457ecb;
      iVar6 = iVar6 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar6 < *(int *)((int)this + 0x2884));
  }
  FUN_0043b6e0(piVar12,iVar6 + 1);
LAB_00457ecb:
  *(int *)(*piVar12 + iVar6 * 4) = (int)this + 0x28ec;
  iVar6 = 0;
  if (0 < *(int *)((int)this + 0x2884)) {
    piVar4 = (int *)*piVar12;
    do {
      if (*piVar4 == 0) goto LAB_00457ef9;
      iVar6 = iVar6 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar6 < *(int *)((int)this + 0x2884));
  }
  FUN_0043b6e0(piVar12,iVar6 + 1);
LAB_00457ef9:
  puVar11 = &local_64;
  uVar10 = 0x3f800000;
  uVar9 = 0x3f800000;
  uVar8 = 0x3e800000;
  *(int *)(*piVar12 + iVar6 * 4) = (int)this + 0x2a10;
  local_64 = 0x3e800000;
  local_60 = 0x3f800000;
  local_5c = 0x3e800000;
  pvVar7 = (void *)((int)this + 8);
  uVar2 = FUN_0048ab20(0);
  FUN_0047db10(pvVar7,uVar2,uVar8,uVar9,uVar10,puVar11);
  puVar11 = &local_64;
  uVar10 = 0x3f800000;
  uVar9 = 0x3f800000;
  uVar8 = 0x3dcccccd;
  local_64 = 0x3f800000;
  local_60 = 0x3f800000;
  local_5c = 0x3f800000;
  uVar2 = FUN_0048ab20(0);
  FUN_0047db10(pvVar7,uVar2,uVar8,uVar9,uVar10,puVar11);
  puVar11 = &local_64;
  uVar10 = 0x3f800000;
  uVar9 = 0x3f800000;
  uVar8 = 0x3e800000;
  local_64 = 0x3e800000;
  local_60 = 0x3f800000;
  local_5c = 0x3e800000;
  uVar2 = FUN_0048ab20(0);
  FUN_0047db10(pvVar7,uVar2,uVar8,uVar9,uVar10,puVar11);
  puVar11 = &local_64;
  uVar10 = 0x3f800000;
  uVar9 = 0x3f800000;
  uVar8 = 0x3f800000;
  local_64 = 0x3f800000;
  local_60 = 0x3f800000;
  local_5c = 0x3f800000;
  uVar2 = FUN_0048ab20(0);
  FUN_0047db10(pvVar7,uVar2,uVar8,uVar9,uVar10,puVar11);
  *(uint *)((int)this + 0x20) = *(uint *)((int)this + 0x20) & 0xfffffffe;
  FUN_0048ac60((void *)((int)this + 0x24),0,(int)pvVar7,'\x01');
  *(undefined1 *)((int)this + 0x114) = 1;
  iVar6 = 0;
  if (0 < *(int *)((int)this + 0x289c)) {
    piVar12 = (int *)*piVar3;
    do {
      if (*piVar12 == 0) goto LAB_00458048;
      iVar6 = iVar6 + 1;
      piVar12 = piVar12 + 1;
    } while (iVar6 < *(int *)((int)this + 0x289c));
  }
  FUN_0043b6e0(piVar3,iVar6 + 1);
LAB_00458048:
  *(void **)(*piVar3 + iVar6 * 4) = (void *)((int)this + 0x24);
  FUN_0048d390((void *)((int)this + 0x824));
  puVar11 = &local_64;
  uVar10 = 0x3f800000;
  uVar9 = 0x3f800000;
  uVar8 = 0x3f800000;
  local_64 = 0x3f800000;
  local_60 = 0x3f800000;
  local_5c = 0x3f800000;
  pvVar7 = (void *)((int)this + 0x6bc);
  uVar2 = FUN_0048ab20(1);
  FUN_0047db10(pvVar7,uVar2,uVar8,uVar9,uVar10,puVar11);
  puVar11 = &local_64;
  uVar10 = 0x3f800000;
  uVar9 = 0x3f800000;
  uVar8 = 0x3f800000;
  local_64 = 0x3f800000;
  local_60 = 0x3f800000;
  local_5c = 0x3f800000;
  uVar2 = FUN_0048ab20(1);
  FUN_0047db10(pvVar7,uVar2,uVar8,uVar9,uVar10,puVar11);
  puVar11 = &local_64;
  uVar10 = 0x3f800000;
  uVar9 = 0;
  uVar8 = 0x3f800000;
  local_64 = 0x3f800000;
  local_60 = 0x3f800000;
  local_5c = 0x3f800000;
  uVar2 = FUN_0048ab20(1);
  FUN_0047db10(pvVar7,uVar2,uVar8,uVar9,uVar10,puVar11);
  FUN_0048ac60((void *)((int)this + 0x6d8),1,(int)pvVar7,'\x01');
  iVar6 = 0;
  if (0 < *(int *)((int)this + 0x289c)) {
    piVar12 = (int *)*piVar3;
    do {
      if (*piVar12 == 0) goto LAB_0045814f;
      iVar6 = iVar6 + 1;
      piVar12 = piVar12 + 1;
    } while (iVar6 < *(int *)((int)this + 0x289c));
  }
  FUN_0043b6e0(piVar3,iVar6 + 1);
LAB_0045814f:
  *(void **)(*piVar3 + iVar6 * 4) = (void *)((int)this + 0x6d8);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_2 = &DAT_00501908;
  FUN_00401d80(&param_2,"RescueWin");
  local_4 = 0;
  FUN_004a12f0((void *)((int)this + 0x7dc),&param_2,0,1);
  local_4 = 0xffffffff;
  if ((param_2 != &DAT_00501908) && (*param_2 = *param_2 + -1, *param_2 < 1)) {
    FUN_004011c0(&param_2);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_2 = &DAT_00501908;
  FUN_00401d80(&param_2,"RescueLose");
  local_4 = 1;
  FUN_004a12f0((void *)((int)this + 0x800),&param_2,0,1);
  local_4 = 0xffffffff;
  if ((param_2 != &DAT_00501908) && (*param_2 = *param_2 + -1, *param_2 < 1)) {
    FUN_004011c0(&param_2);
  }
  FUN_004567d0((int)this + 0x10bc);
  FUN_00456680((void *)((int)this + 0x1b6c));
  FUN_00482260((void *)((int)this + 0xe40),(int *)0x0);
  FUN_0047dc90((void *)((int)this + 0x2b3c),'\0');
  FUN_00456bf0((void *)((int)this + 0x1cf0));
  pvVar7 = (void *)((int)this + 0x22bc);
  iVar6 = 4;
  do {
    FUN_0047dc90(pvVar7,'\0');
    pvVar7 = (void *)((int)pvVar7 + 0x130);
    iVar6 = iVar6 + -1;
  } while (iVar6 != 0);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  param_1 = &DAT_00501908;
  FUN_00401d80(&param_1,"Hint");
  local_4 = 2;
  FUN_004a12f0((void *)((int)this + 0xe1c),&param_1,0,2);
  piVar12 = param_1;
  local_4 = 0xffffffff;
  if (param_1 != &DAT_00501908) {
    *param_1 = *param_1 + -1;
    iVar6 = *param_1;
    param_1 = &DAT_00501908;
    if (iVar6 < 1) {
      if (*piVar12 < 0) {
        piVar3 = local_54 + 0xc;
        for (iVar6 = 6; iVar6 != 0; iVar6 = iVar6 + -1) {
          *piVar3 = 0;
          piVar3 = piVar3 + 1;
        }
        local_54[0xc] = 0;
        local_20 = "Invalid reference count";
        local_1c = "..\\BluelAPI/String/Rope_RatStr.h";
        local_18 = 0xd0;
        FUN_00422060(local_54 + 0xc);
      }
      _free(piVar12);
    }
  }
  FUN_00456f70(this);
  param_2 = (int *)&stack0xffffff88;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  piVar12 = &DAT_00501908;
  FUN_00401d80(&stack0xffffff88,"Find and rescue the survivors!");
  FUN_004564c0(this,5.0,piVar12);
  ExceptionList = local_c;
  return;
}



/* 0x004895c0 FUN_004895c0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004895c0(int *param_1)

{
  undefined4 *this;
  char *pcVar1;
  int iVar2;
  int *piVar3;
  undefined ***local_6c;
  undefined **local_64;
  char *local_60;
  char *local_5c;
  char *local_58;
  char *local_54;
  char *local_50;
  char *local_4c;
  char *local_48;
  char *local_44;
  char *local_40;
  char *local_3c;
  char *local_38;
  char *local_34;
  undefined4 local_30;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3b33;
  local_c = ExceptionList;
  pcVar1 = "RC_OxygenLow";
  local_6c = &local_64;
  local_64 = (undefined **)0x4e2170;
  local_60 = "RC_VictPick";
  local_5c = "RC_VictDrop";
  local_58 = "RC_PassOut";
  local_54 = "RC_ResLvlStart";
  local_50 = "RC_ResLvlWin";
  local_4c = "RC_ResLvlLose";
  local_48 = "RC_TimeWarn";
  local_44 = "RC_HoseLvlStart";
  local_40 = "RC_HoseLvlLose";
  local_3c = "RC_HoseLvlWin";
  local_38 = "RC_Pain";
  local_34 = "RC_ResTimeWarn";
  local_30 = 0;
  ExceptionList = &local_c;
  do {
    this = operator_new(0x20);
    if (this == (undefined4 *)0x0) {
      this = (undefined4 *)0x0;
    }
    else {
      this[5] = 0x10;
      this[4] = FUN_0043f660;
      *this = 0;
      this[1] = 0;
      this[2] = 0;
      this[3] = 0;
      if (DAT_00501918 == '\0') {
        DAT_00501918 = '\x01';
        DAT_00501908 = 2;
        DAT_0050190c = 0;
        _DAT_00501910 = 1;
        DAT_00501914 = 0;
      }
      this[7] = &DAT_00501908;
    }
    local_4 = 0xffffffff;
    if (DAT_00501918 == '\0') {
      DAT_00501918 = '\x01';
      DAT_00501908 = 2;
      DAT_0050190c = 0;
      _DAT_00501910 = 1;
      DAT_00501914 = 0;
    }
    piVar3 = &DAT_00501908;
    FUN_00401d80(&stack0xffffff80,pcVar1);
    FUN_00489200(this,piVar3);
    iVar2 = 0;
    if (0 < param_1[1]) {
      piVar3 = (int *)*param_1;
      do {
        if (*piVar3 == 0) goto LAB_0048972e;
        iVar2 = iVar2 + 1;
        piVar3 = piVar3 + 1;
      } while (iVar2 < param_1[1]);
    }
    FUN_0043b6e0(param_1,iVar2 + 1);
LAB_0048972e:
    local_6c = local_6c + 1;
    *(undefined4 **)(*param_1 + iVar2 * 4) = this;
    pcVar1 = (char *)*local_6c;
    if ((undefined **)pcVar1 == (undefined **)0x0) {
      local_64 = &PTR_FUN_004dc554;
      FUN_004a3118();
      local_60 = "basic\\RadioChatter.cpp";
      local_5c = (char *)0x24;
      local_58 = (char *)((uint)local_58 & 0xffffff00);
      local_4 = 1;
      FUN_004a3a19(&local_48,"[TRadioChatter] Chatter Box Initialized");
      FUN_00401360(&local_64,0);
      local_4 = 0xffffffff;
      FUN_004012c0(&local_64);
      param_1[7] = 0;
      ExceptionList = local_c;
      return;
    }
  } while( true );
}



/* 0x004897c0 FUN_004897c0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_004897c0(void *this,int *param_1,char param_2)

{
  int iVar1;
  int *piVar2;
  int *piVar3;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  piVar2 = param_1;
  puStack_8 = &LAB_004d3a78;
  local_c = ExceptionList;
  local_4 = 0;
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  ExceptionList = &local_c;
  piVar3 = &DAT_00501908;
  if (param_1 != &DAT_00501908) {
    ExceptionList = &local_c;
    *param_1 = *param_1 + 1;
    piVar3 = param_1;
  }
  piVar3 = (int *)FUN_00488e30(this,piVar3);
  if (piVar3 != (int *)0x0) {
    if (param_2 == '\0') {
      FUN_00488d40(piVar3);
    }
    else {
      FUN_00489110(piVar3);
    }
    *(int **)((int)this + 0x18) = piVar3;
    *(undefined1 *)((int)this + 0x20) = 1;
  }
  local_4 = 0xffffffff;
  if ((piVar2 != &DAT_00501908) && (iVar1 = *piVar2, *piVar2 = iVar1 + -1, iVar1 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return;
}



/* 0x00489110 FUN_00489110 */

void __fastcall FUN_00489110(int *param_1)

{
  int iVar1;
  void *this;
  int *extraout_ECX;
  int *piVar2;
  int iVar3;
  undefined **local_64;
  char *local_60;
  undefined4 local_5c;
  undefined1 local_58;
  undefined1 local_48 [60];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d3ac0;
  local_c = ExceptionList;
  iVar1 = param_1[6];
  ExceptionList = &local_c;
  param_1[6] = iVar1 + 1;
  if (param_1[1] <= iVar1 + 1) {
    param_1[6] = 0;
  }
  iVar1 = param_1[6];
  if (iVar1 < param_1[1]) {
    iVar3 = 0;
    FUN_0048d490(param_1,iVar1);
    piVar2 = extraout_ECX;
    FUN_00401cb0(&stack0xffffff88,(undefined4 *)(*(int *)(*param_1 + iVar1 * 4) + 0x20));
    local_4 = 0;
    local_64 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_60 = "basic\\RadioChatter.cpp";
    local_5c = 0x77;
    local_58 = 0;
    local_4 = CONCAT31(local_4._1_3_,2);
    FUN_004a3a19(local_48,"[TChatterSet] Playing shuffled sound: ");
    this = (void *)FUN_0040f020(&local_64,piVar2);
    FUN_00401360(this,iVar3);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_64);
    iVar1 = param_1[6];
    FUN_0048d490(param_1,iVar1);
    FUN_004a11a0(*(void **)(*param_1 + iVar1 * 4),(undefined1 *)0x0);
  }
  ExceptionList = local_c;
  return;
}



/* 0x00488e30 FUN_00488e30 */

undefined4 __thiscall FUN_00488e30(void *this,int *param_1)

{
  undefined4 uVar1;
  void **ppvVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  int local_24;
  char *local_20;
  char *local_1c;
  undefined4 local_18;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  puStack_8 = &LAB_004d3a78;
  local_c = ExceptionList;
  iVar3 = *(int *)((int)this + 4);
  iVar4 = 0;
  local_4 = 0;
  ExceptionList = &local_c;
  ppvVar2 = &local_c;
  if (0 < iVar3) {
    do {
      ExceptionList = ppvVar2;
      if ((iVar4 < 0) || (iVar3 <= iVar4)) {
        piVar5 = &local_24;
        for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
          *piVar5 = 0;
          piVar5 = piVar5 + 1;
        }
        local_24 = 1;
        local_20 = "Array index out of bounds";
        local_1c = "..\\BluelAPI/Container/SArray.h";
        local_18 = 0x1cd;
        FUN_00422060(&local_24);
      }
      iVar3 = FUN_0040cb90((void *)(*(int *)(*(int *)this + iVar4 * 4) + 0x1c),(int *)&param_1);
      if ((char)iVar3 != '\0') {
        if ((iVar4 < 0) || (*(int *)((int)this + 4) <= iVar4)) {
          piVar5 = &local_24;
          for (iVar3 = 6; iVar3 != 0; iVar3 = iVar3 + -1) {
            *piVar5 = 0;
            piVar5 = piVar5 + 1;
          }
          local_24 = 1;
          local_20 = "Array index out of bounds";
          local_1c = "..\\BluelAPI/Container/SArray.h";
          local_18 = 0x1cd;
          FUN_00422060(&local_24);
        }
        uVar1 = *(undefined4 *)(*(int *)this + iVar4 * 4);
        local_4 = 0xffffffff;
        if ((param_1 != &DAT_00501908) && (iVar3 = *param_1, *param_1 = iVar3 + -1, iVar3 + -1 < 1))
        {
          FUN_004011c0(&param_1);
        }
        ExceptionList = local_c;
        return uVar1;
      }
      iVar3 = *(int *)((int)this + 4);
      iVar4 = iVar4 + 1;
      ppvVar2 = ExceptionList;
    } while (iVar4 < iVar3);
  }
  local_4 = 0xffffffff;
  if ((param_1 != &DAT_00501908) && (iVar3 = *param_1, *param_1 = iVar3 + -1, iVar3 + -1 < 1)) {
    FUN_004011c0(&param_1);
  }
  ExceptionList = local_c;
  return 0;
}



/* 0x004a11a0 FUN_004a11a0 */

void __thiscall FUN_004a11a0(void *this,undefined1 *param_1)

{
  bool bVar1;
  
  if ((param_1 == (undefined1 *)0x1) && (bVar1 = FUN_0049f260(this), bVar1)) {
    return;
  }
  FUN_004a0be0(*(void **)(DAT_00503a78 + 8),this,param_1);
  return;
}



/* 0x00455b40 FUN_00455b40 */

void __thiscall FUN_00455b40(void *this,char param_1)

{
  int iVar1;
  void *this_00;
  
  FUN_0047dc90((void *)((int)this + 0x54),param_1);
  FUN_0047dc90((void *)((int)this + 0x178),param_1);
  FUN_0047dc90((void *)((int)this + 0x29c),param_1);
  this_00 = (void *)((int)this + 0x3dc);
  iVar1 = 4;
  do {
    FUN_0047dc90(this_00,param_1);
    this_00 = (void *)((int)this_00 + 0x124);
    iVar1 = iVar1 + -1;
  } while (iVar1 != 0);
  FUN_0047dc90((void *)((int)this + 0x86c),param_1);
  FUN_0040cb80((void *)((int)this + 0x9ac),param_1);
  return;
}



