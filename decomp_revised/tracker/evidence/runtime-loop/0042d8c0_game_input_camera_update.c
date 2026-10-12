
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_0042d8c0(int param_1)

{
  float fVar1;
  int *piVar2;
  void *pvVar3;
  undefined4 uVar4;
  float *pfVar5;
  undefined4 *puVar6;
  int iVar7;
  int iVar8;
  float10 fVar9;
  ulonglong uVar10;
  int **ppiVar11;
  int *local_70;
  int *local_6c;
  int local_68;
  undefined4 local_64;
  float local_60;
  float local_5c;
  float local_58;
  float local_54;
  float local_50;
  float local_4c;
  float local_48 [3];
  undefined1 local_3c [36];
  float local_18;
  float local_14;
  float local_10;
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004cdca0;
  local_c = ExceptionList;
  ExceptionList = &local_c;
  FUN_004a05a0((void *)DAT_00503a78[2],param_1 + 0x1c4);
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_70 = &DAT_00501908;
  FUN_00401d80(&local_70,"MOUSE_X");
  ppiVar11 = &local_70;
  local_4 = 0;
  piVar2 = (int *)FUN_00499cb0(DAT_00503a78);
  FUN_004386f0(piVar2,(int *)ppiVar11);
  uVar10 = FUN_004b9400();
  *(int *)(param_1 + 0x288) = (int)uVar10;
  local_4 = 0xffffffff;
  if ((local_70 != &DAT_00501908) && (*local_70 = *local_70 + -1, *local_70 < 1)) {
    FUN_004011c0(&local_70);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_70 = &DAT_00501908;
  FUN_00401d80(&local_70,"MOUSE_Y");
  ppiVar11 = &local_70;
  local_4 = 1;
  piVar2 = (int *)FUN_00499cb0(DAT_00503a78);
  FUN_004386f0(piVar2,(int *)ppiVar11);
  uVar10 = FUN_004b9400();
  *(int *)(param_1 + 0x28c) = (int)uVar10;
  local_4 = 0xffffffff;
  if ((local_70 != &DAT_00501908) && (*local_70 = *local_70 + -1, *local_70 < 1)) {
    FUN_004011c0(&local_70);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_70 = &DAT_00501908;
  FUN_00401d80(&local_70,"MOUSE_LB");
  ppiVar11 = &local_70;
  local_4 = 2;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  uVar4 = FUN_00438480(pvVar3,(int *)ppiVar11);
  *(char *)(param_1 + 0x285) = (char)uVar4;
  local_4 = 0xffffffff;
  if ((local_70 != &DAT_00501908) && (*local_70 = *local_70 + -1, *local_70 < 1)) {
    FUN_004011c0(&local_70);
  }
  if (DAT_00501918 == '\0') {
    DAT_00501918 = '\x01';
    DAT_00501908 = 2;
    DAT_0050190c = 0;
    _DAT_00501910 = 1;
    DAT_00501914 = 0;
  }
  local_70 = &DAT_00501908;
  FUN_00401d80(&local_70,"MOUSE_RB");
  ppiVar11 = &local_70;
  local_4 = 3;
  pvVar3 = (void *)FUN_00499cb0(DAT_00503a78);
  FUN_00438480(pvVar3,(int *)ppiVar11);
  local_4 = 0xffffffff;
  if ((local_70 != &DAT_00501908) && (*local_70 = *local_70 + -1, *local_70 < 1)) {
    FUN_004011c0(&local_70);
  }
  iVar7 = *(int *)(param_1 + 0x28c) + *(int *)(param_1 + 0x294);
  iVar8 = *(int *)(param_1 + 0x288) + *(int *)(param_1 + 0x290);
  *(int *)(param_1 + 0x288) = iVar8;
  *(int *)(param_1 + 0x28c) = iVar7;
  if (iVar7 < *(int *)(param_1 + 0x27c)) {
    *(int *)(param_1 + 0x28c) = *(int *)(param_1 + 0x27c);
  }
  if (*(int *)(param_1 + 0x280) < *(int *)(param_1 + 0x28c)) {
    *(int *)(param_1 + 0x28c) = *(int *)(param_1 + 0x280);
  }
  if (iVar8 < *(int *)(param_1 + 0x274)) {
    *(int *)(param_1 + 0x288) = *(int *)(param_1 + 0x274);
  }
  if (*(int *)(param_1 + 0x278) < *(int *)(param_1 + 0x288)) {
    *(int *)(param_1 + 0x288) = *(int *)(param_1 + 0x278);
  }
  FUN_00494670((void *)DAT_00503a78[1],&LAB_0042cf70);
  if (*(int *)(param_1 + 0x4c) != 0) {
    iVar7 = DAT_00503a78[1];
    if (*(char *)(iVar7 + 0x160) == '\0') {
      iVar7 = iVar7 + 8;
    }
    else {
      iVar7 = iVar7 + 0xb4;
    }
    geWorld_SetActorFlags
              (*(undefined4 *)(iVar7 + 0x78),*(undefined4 *)(*(int *)(param_1 + 0xe4) + 0x60),0x1000
              );
    local_54 = 1.0;
    local_50 = -1.0;
    local_4c = 0.0;
    fVar9 = (float10)geVec3d_DotProduct(&local_54,&local_54);
    if (SQRT(fVar9) != (float10)_DAT_004dfc28) {
      fVar9 = (float10)_DAT_004dfc24 / SQRT(fVar9);
      local_54 = (float)((float10)local_54 * fVar9);
      local_50 = (float)((float10)local_50 * fVar9);
      local_4c = (float)(fVar9 * (float10)local_4c);
    }
    geActor_SetLightingOptions
              (*(undefined4 *)(*(int *)(param_1 + 0xe4) + 0x60),1,&local_54,0x437f0000,0x437f0000,
               0x437f0000,0x435c0000,0x435c0000,0x435c0000,0,0,0,0);
    fVar1 = (float)DAT_00502afc;
    if (DAT_00502afc < 0) {
      fVar1 = fVar1 + (float)_DAT_004dfc90;
    }
    FUN_00497e30(param_1 + 0x50,fVar1 * (float)_DAT_004dfa28);
    FUN_00492410(*(int *)(param_1 + 0xe4));
    local_70 = (int *)(float)(int)((-(uint)(*(int *)(param_1 + 0x4c) != 1) & 200) - 100);
    if (*(char *)(param_1 + 0x150) == '\0') {
      FUN_0042d280((void *)(param_1 + 0x78),local_48,local_70,0xc2a00000,0);
    }
    else {
      geActor_GetBoneTransform
                (*(undefined4 *)(*(int *)(param_1 + 0xe4) + 0x60),"BIP01 R TOE0",local_3c);
      local_60 = local_18 - *(float *)(param_1 + 0x78);
      local_5c = local_14 - *(float *)(param_1 + 0x7c);
      ppiVar11 = &local_6c;
      local_58 = local_10 - *(float *)(param_1 + 0x80);
      local_6c = local_70;
      local_68 = 0xc2a00000;
      local_64 = 0;
      pfVar5 = (float *)FUN_0042d2b0(&local_60,local_48);
      puVar6 = (undefined4 *)FUN_0042d050(&local_60,pfVar5,(float *)ppiVar11);
      *(undefined4 *)(param_1 + 0x78) = *puVar6;
      *(undefined4 *)(param_1 + 0x7c) = puVar6[1];
      *(undefined4 *)(param_1 + 0x80) = puVar6[2];
    }
  }
  if (*(int **)(param_1 + 0x10) != (int *)0x0) {
    local_6c = (int *)(*(int *)(param_1 + 0x290) - *(int *)(param_1 + 0x274));
    local_68 = *(int *)(param_1 + 0x294) - *(int *)(param_1 + 0x27c);
    local_60 = (float)(*(int *)(param_1 + 0x288) - *(int *)(param_1 + 0x274));
    local_5c = (float)(*(int *)(param_1 + 0x28c) - *(int *)(param_1 + 0x27c));
    (**(code **)(**(int **)(param_1 + 0x10) + 0x1c))
              (*(undefined1 *)(param_1 + 0x285),*(undefined1 *)(param_1 + 0x286),&local_60,&local_6c
              );
    if ((DAT_00502a7a != '\0') && (DAT_00502b1c < 3)) {
      iVar7 = DAT_00502b1c * 4;
      DAT_00502b1c = DAT_00502b1c + 1;
      (**(code **)(**(int **)(param_1 + 0x10) + 4))(*(undefined4 *)(&DAT_00502b20 + iVar7));
    }
  }
  *(undefined4 *)(param_1 + 0x290) = *(undefined4 *)(param_1 + 0x288);
  *(undefined4 *)(param_1 + 0x294) = *(undefined4 *)(param_1 + 0x28c);
  *(undefined1 *)(param_1 + 0x286) = *(undefined1 *)(param_1 + 0x285);
  ExceptionList = local_c;
  return;
}

