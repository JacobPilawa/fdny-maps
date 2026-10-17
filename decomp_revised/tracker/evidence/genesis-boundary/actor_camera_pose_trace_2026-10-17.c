/* Ghidra pseudocode export; names and types are provisional. */
/* Program: Genesis.dll */

/* Input SHA-256: 5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643 */

/* 0x100016d0 geCamera_Project */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* __fastcall geCamera_Project,12 */

void __fastcall geCamera_Project(int param_1,float *param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  
                    /* 0x16d0  155  @geCamera_Project@12 */
  fVar1 = -param_2[2];
  if (-param_2[2] < _DAT_100511a4) {
    fVar1 = _DAT_100511a4;
  }
  fVar2 = *(float *)(param_1 + 0x24) / fVar1;
  param_3[2] = fVar1 * *(float *)(param_1 + 0x11c);
  *param_3 = fVar2 * *param_2 + *(float *)(param_1 + 0x28);
  param_3[1] = *(float *)(param_1 + 0x2c) - fVar2 * param_2[1];
  return;
}



/* 0x100018a0 geCamera_TransformAndProject */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* __fastcall geCamera_TransformAndProject,12 */

void __fastcall geCamera_TransformAndProject(int param_1,undefined4 param_2,float *param_3)

{
  float fVar1;
  
                    /* 0x18a0  164  @geCamera_TransformAndProject@12 */
  FUN_100010f0(param_1 + 0x30,param_2,param_3);
  fVar1 = -param_3[2];
  if (-param_3[2] <= _DAT_100511a4) {
    fVar1 = _DAT_100511a4;
  }
  param_3[2] = fVar1 * *(float *)(param_1 + 0x11c);
  fVar1 = *(float *)(param_1 + 0x24) / fVar1;
  *param_3 = fVar1 * *param_3 + *(float *)(param_1 + 0x28);
  param_3[1] = *(float *)(param_1 + 0x2c) - fVar1 * param_3[1];
  return;
}



/* 0x10001990 geCamera_SetWorldSpaceXForm */

/* __fastcall geCamera_SetWorldSpaceXForm,8 */

undefined4 __fastcall geCamera_SetWorldSpaceXForm(int param_1,undefined4 *param_2)

{
  int iVar1;
  undefined4 *puVar2;
  undefined4 *puVar3;
  
                    /* 0x1990  161  @geCamera_SetWorldSpaceXForm@8 */
  puVar2 = param_2;
  puVar3 = (undefined4 *)(param_1 + 0xd0);
  for (iVar1 = 0xc; iVar1 != 0; iVar1 = iVar1 + -1) {
    *puVar3 = *puVar2;
    puVar2 = puVar2 + 1;
    puVar3 = puVar3 + 1;
  }
  geXForm3d_GetTranspose(param_2,param_1 + 0x30);
  *(undefined4 *)(param_1 + 0x100) = param_2[9];
  *(undefined4 *)(param_1 + 0x104) = param_2[10];
  *(undefined4 *)(param_1 + 0x108) = param_2[0xb];
  return 1;
}



/* 0x10007180 geActor_SetPose */

/* __fastcall geActor_SetPose,16 */

void __fastcall geActor_SetPose(void)

{
                    /* 0x7180  63  @geActor_SetPose@16 */
  FUN_10010650();
  return;
}



/* 0x10010650 FUN_10010650 */

void __fastcall FUN_10010650(int *param_1,int param_2,undefined4 param_3,undefined4 *param_4)

{
  bool bVar1;
  int iVar2;
  undefined4 uVar3;
  int iVar4;
  float *pfVar5;
  undefined4 *puVar6;
  undefined4 uVar7;
  float *pfVar8;
  float *pfVar9;
  undefined4 local_30 [9];
  int local_c;
  int local_8;
  int local_4;
  
  param_1[0x47] = -2;
  if (param_1[9] == 0) {
    iVar2 = geMotion_GetTransform(param_2,local_30,param_3);
    if (iVar2 == 0) {
      if (param_4 == (undefined4 *)0x0) goto LAB_100106ea;
      puVar6 = local_30;
      for (iVar2 = 0xc; iVar2 != 0; iVar2 = iVar2 + -1) {
        *puVar6 = *param_4;
        param_4 = param_4 + 1;
        puVar6 = puVar6 + 1;
      }
    }
    else if (param_4 != (undefined4 *)0x0) {
      geXForm3d_Multiply(param_4,local_30,local_30);
    }
    FUN_1002b730();
    param_1[0x23] = local_c;
    param_1[0x24] = local_8;
    param_1[0x25] = local_4;
    param_1[0x2a] = 1;
  }
LAB_100106ea:
  if (param_2 == 0) {
    return;
  }
  iVar2 = geActor_GetMotionCount(param_2);
  if (iVar2 != 0) {
    iVar2 = geMotion_GetNameChecksum(param_2);
    bVar1 = false;
    if (iVar2 == param_1[1]) goto LAB_10010715;
  }
  bVar1 = true;
LAB_10010715:
  iVar2 = 0;
  param_1[2] = 1;
  if (0 < *param_1) {
    pfVar5 = (float *)(param_1[0x46] + 100);
    do {
      pfVar8 = pfVar5 + 3;
      if (bVar1) {
        uVar7 = param_3;
        pfVar9 = pfVar5;
        uVar3 = FUN_10013b20(param_3,pfVar8,pfVar5);
        iVar4 = geMotion_SampleChannelsNamed(param_2,uVar3,uVar7,pfVar8,pfVar9);
        if (iVar4 != 0) goto LAB_1001076d;
      }
      else {
        geMotion_SampleChannels(param_2,iVar2,param_3,pfVar8,pfVar5);
LAB_1001076d:
        pfVar5[7] = 1.4013e-45;
        *pfVar5 = (float)param_1[4] * *pfVar5;
        pfVar5[1] = (float)param_1[5] * pfVar5[1];
        pfVar5[2] = (float)param_1[6] * pfVar5[2];
      }
      iVar2 = iVar2 + 1;
      pfVar5 = pfVar5 + 0x23;
    } while (iVar2 < *param_1);
  }
  return;
}



/* 0x1000c600 geMotion_GetTransform */

/* __fastcall geMotion_GetTransform,12 */

undefined4 __fastcall geMotion_GetTransform(int param_1,float *param_2,float param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  int iVar5;
  float *pfVar6;
  int iVar7;
  float *pfVar8;
  int local_68;
  int local_60;
  int local_5c;
  float local_54;
  float local_50;
  float local_4c;
  float local_3c [3];
  undefined1 local_30 [48];
  
                    /* 0xc600  212  @geMotion_GetTransform@12 */
  iVar7 = 0;
  if ((*(int *)(param_1 + 0xc) != 0) && (*(int *)(param_1 + 0xc) == 1)) {
    local_60 = 0;
    if ((*(int *)(param_1 + 0x10) != 0) && (local_5c = 0, 0 < *(int *)(param_1 + 0x10))) {
      local_68 = 0;
      do {
        iVar5 = *(int *)(param_1 + 0x18);
        pfVar6 = (float *)(iVar5 + local_68);
        fVar4 = (param_3 - *(float *)(iVar5 + 4 + local_68)) * *pfVar6;
        iVar5 = geMotion_GetTransform(*(undefined4 *)(iVar5 + 0x40 + local_68),param_2,fVar4);
        if (iVar5 == 0) {
          if (pfVar6[0xf] != 0.0) {
            pfVar6 = pfVar6 + 3;
            pfVar8 = param_2;
            for (iVar7 = 0xc; iVar7 != 0; iVar7 = iVar7 + -1) {
              *pfVar8 = *pfVar6;
              pfVar6 = pfVar6 + 1;
              pfVar8 = pfVar8 + 1;
            }
            goto LAB_1000c69b;
          }
        }
        else {
          local_60 = iVar7;
          if (pfVar6[0xf] != 0.0) {
            geXForm3d_Multiply(pfVar6 + 3,param_2,param_2);
          }
LAB_1000c69b:
          if (local_60 == 0) {
            FUN_1002b730();
            local_54 = param_2[9];
            local_50 = param_2[10];
            local_4c = param_2[0xb];
          }
          else {
            FUN_1002b730();
            fVar1 = param_2[9];
            fVar2 = param_2[10];
            fVar3 = param_2[0xb];
            FUN_1000e430(fVar4,local_3c);
            FUN_1002ba30(local_3c[0],local_30);
            local_54 = (fVar1 - local_54) * local_3c[0] + local_54;
            local_50 = (fVar2 - local_50) * local_3c[0] + local_50;
            local_4c = (fVar3 - local_4c) * local_3c[0] + local_4c;
          }
          iVar7 = local_60 + 1;
          local_60 = iVar7;
        }
        local_5c = local_5c + 1;
        local_68 = local_68 + 0x44;
      } while (local_5c < *(int *)(param_1 + 0x10));
      if (0 < iVar7) {
        geQuaternion_ToMatrix(local_30,param_2);
        param_2[9] = local_54;
        param_2[10] = local_50;
        param_2[0xb] = local_4c;
        return 1;
      }
    }
  }
  return 0;
}



/* 0x1002a010 geXForm3d_Multiply */

/* __fastcall geXForm3d_Multiply,12 */

void __fastcall geXForm3d_Multiply(float *param_1,float *param_2,float *param_3)

{
  int iVar1;
  float *pfVar2;
  float local_60 [4];
  float local_50;
  float local_4c;
  float local_48;
  float local_44;
  float local_40;
  float local_3c;
  float local_38;
  float local_34;
  float local_30 [4];
  float local_20;
  float local_1c;
  float local_18;
  float local_14;
  float local_10;
  float local_c;
  float local_8;
  float local_4;
  
  pfVar2 = local_60;
                    /* 0x2a010  409  @geXForm3d_Multiply@12 */
  for (iVar1 = 0xc; iVar1 != 0; iVar1 = iVar1 + -1) {
    *pfVar2 = *param_1;
    param_1 = param_1 + 1;
    pfVar2 = pfVar2 + 1;
  }
  pfVar2 = local_30;
  for (iVar1 = 0xc; iVar1 != 0; iVar1 = iVar1 + -1) {
    *pfVar2 = *param_2;
    param_2 = param_2 + 1;
    pfVar2 = pfVar2 + 1;
  }
  *param_3 = local_30[0] * local_60[0] + local_30[3] * local_60[1] + local_18 * local_60[2];
  param_3[1] = local_30[1] * local_60[0] + local_20 * local_60[1] + local_14 * local_60[2];
  param_3[2] = local_30[2] * local_60[0] + local_1c * local_60[1] + local_10 * local_60[2];
  param_3[3] = local_60[3] * local_30[0] + local_50 * local_30[3] + local_4c * local_18;
  param_3[4] = local_60[3] * local_30[1] + local_50 * local_20 + local_4c * local_14;
  param_3[5] = local_60[3] * local_30[2] + local_50 * local_1c + local_4c * local_10;
  param_3[6] = local_48 * local_30[0] + local_44 * local_30[3] + local_40 * local_18;
  param_3[7] = local_48 * local_30[1] + local_44 * local_20 + local_40 * local_14;
  param_3[8] = local_48 * local_30[2] + local_44 * local_1c + local_40 * local_10;
  param_3[9] = local_c * local_60[0] + local_8 * local_60[1] + local_4 * local_60[2] + local_3c;
  param_3[10] = local_c * local_60[3] + local_8 * local_50 + local_4 * local_4c + local_38;
  param_3[0xb] = local_c * local_48 + local_8 * local_44 + local_4 * local_40 + local_34;
  return;
}



/* 0x1002aaf0 geXForm3d_Transform */

/* __fastcall geXForm3d_Transform,12 */

void __fastcall geXForm3d_Transform(float *param_1,float *param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  
                    /* 0x2aaf0  424  @geXForm3d_Transform@12 */
  fVar1 = *param_2;
  fVar2 = param_2[1];
  fVar3 = param_2[2];
  *param_3 = fVar1 * *param_1 + fVar2 * param_1[1] + fVar3 * param_1[2] + param_1[9];
  param_3[1] = fVar1 * param_1[3] + fVar2 * param_1[4] + fVar3 * param_1[5] + param_1[10];
  param_3[2] = fVar1 * param_1[6] + fVar2 * param_1[7] + fVar3 * param_1[8] + param_1[0xb];
  return;
}



