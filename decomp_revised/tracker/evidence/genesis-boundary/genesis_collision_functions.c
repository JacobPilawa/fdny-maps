/* Ghidra decompilation excerpts for the shipped FDNY Genesis.dll.
 * Input SHA-256: 5014bb879706bfb5ae6cf9ce1de63dc3deebdcc5b8e1b41f247b284a336c4643
 * Captured 2026-10-13 from the active Ghidra MCP analysis; see
 * ../runtime/genesis_collision_path.md for interpretation and limits.
 * This is analysis output, not recovered original source.
 */
/* Ghidra entry 0x10001E30 */
undefined4
geWorld_Collision(int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
                 undefined4 param_6,undefined4 param_7,undefined4 param_8,undefined4 param_9,
                 undefined4 param_10,undefined4 *param_11)

{
  undefined4 uVar1;
  undefined4 *puVar2;
  int iVar3;
  undefined4 uStack_24;
  undefined4 uStack_20;
  undefined4 uStack_1c;
  undefined4 uStack_18;
  undefined4 uStack_14;
  undefined4 uStack_10;
  undefined4 uStack_c;
  undefined4 uStack_8;

  puVar2 = param_11;
                    /* 0x1e30  504  geWorld_Collision */
  iVar3 = param_1;
  DAT_10362f1c = *(int *)(param_1 + 0x18) + 200;
  DAT_10362f5c = param_6;
  param_6 = 0;
  param_1 = 0;
  uStack_24 = 0;
  *param_11 = 0;
  param_11[1] = 0;
  param_11[2] = 0;
  if ((param_2 == 0) || (param_3 == 0)) {
    DAT_10362ec4 = DAT_10362ec4 + 1;
    iVar3 = FUN_10038470(iVar3,param_4,param_5,param_7,&uStack_20,&uStack_14,&param_6,&param_1,
                         &uStack_24,param_8,param_9,param_10);
    if (iVar3 != 0) {
      puVar2[3] = uStack_20;
      puVar2[4] = uStack_1c;
      puVar2[5] = uStack_18;
      puVar2[7] = uStack_14;
      puVar2[8] = uStack_10;
      puVar2[9] = uStack_c;
      puVar2[10] = uStack_8;
      *puVar2 = param_6;
      uVar1 = DAT_10362e94;
      puVar2[1] = param_1;
      puVar2[2] = uStack_24;
      puVar2[6] = uVar1;
      return 1;
    }
  }
  else {
    DAT_10362f28 = DAT_10362f28 + 1;
    iVar3 = FUN_100372a0(iVar3,param_2,param_3,param_4,param_5,param_7,&uStack_20,&uStack_14,
                         &param_6,&param_1,&uStack_24,param_8,param_9,param_10);
    if (iVar3 != 0) {
      puVar2[3] = uStack_20;
      puVar2[4] = uStack_1c;
      puVar2[5] = uStack_18;
      puVar2[7] = uStack_14;
      puVar2[8] = uStack_10;
      puVar2[9] = uStack_c;
      *puVar2 = param_6;
      uVar1 = DAT_10362e94;
      puVar2[10] = uStack_8;
      puVar2[6] = uVar1;
      puVar2[1] = param_1;
      puVar2[2] = uStack_24;
      return 1;
    }
  }
  return 0;
}

/* Ghidra entry 0x100372A0 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4
FUN_100372a0(int param_1,undefined4 *param_2,undefined4 *param_3,float *param_4,float *param_5,
            byte param_6,float *param_7,float *param_8,int *param_9,undefined4 *param_10,
            int *param_11,undefined4 param_12,code *param_13,undefined4 param_14)

{
  float fVar1;
  float fVar2;
  float fVar3;
  int iVar4;
  int iVar5;
  float *pfVar6;
  float *pfVar7;
  float *pfVar8;
  float10 fVar9;
  int local_80;
  float local_7c;
  int local_78;
  int local_74;
  int local_70;
  int local_6c;
  int local_68;
  float local_64;
  float local_60;
  float local_5c;
  float local_58;
  float local_54;
  float local_50;
  float fStack_4c;
  float fStack_48;
  float fStack_44;
  float fStack_40;
  float fStack_3c;
  float local_34 [8];
  float local_14 [5];

  if (param_9 != (int *)0x0) {
    *param_9 = 0;
  }
  if (param_10 != (undefined4 *)0x0) {
    *param_10 = 0;
  }
  if (param_11 != (int *)0x0) {
    *param_11 = 0;
  }
  local_7c = 99999.0;
  local_68 = 0;
  local_70 = 0;
  local_74 = 0;
  if (((param_6 & 4) != 0) &&
     (local_70 = FUN_100360f0(param_2,param_3,param_5,local_34 + 3,param_12,param_13,param_14,
                              &local_7c), local_70 != 0)) {
    local_58 = local_64;
    local_54 = local_60;
    local_50 = local_5c;
    pfVar7 = local_34 + 3;
    pfVar6 = local_14;
    for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
      *pfVar6 = *pfVar7;
      pfVar7 = pfVar7 + 1;
      pfVar6 = pfVar6 + 1;
    }
    local_74 = 1;
  }
  _DAT_10362e78 = *param_2;
  _DAT_10362e7c = param_2[1];
  _DAT_10362e80 = param_2[2];
  _DAT_10362f00 = *param_3;
  _DAT_10362f04 = param_3[1];
  _DAT_10362f08 = param_3[2];
  DAT_10362ecc = *param_4;
  DAT_10362ed0 = param_4[1];
  DAT_10362ed4 = param_4[2];
  _DAT_10362ea8 = *param_5;
  _DAT_10362eac = param_5[1];
  _DAT_10362eb0 = param_5[2];
  iVar4 = *(int *)(param_1 + 0x18);
  local_80 = *(int *)(iVar4 + 0x1d0);
  iVar5 = iVar4 + 200;
  DAT_10362f24 = *(undefined4 *)(iVar4 + 0x114);
  DAT_10362ef0 = *(undefined4 *)(iVar4 + 0x120);
  DAT_10362ef8 = *(undefined4 *)(iVar4 + 0x130);
  DAT_10362f0c = *(undefined4 *)(iVar4 + 0x13c);
  DAT_10362f1c = iVar5;
  if ((param_6 & 2) != 0) {
    FUN_10035470(param_2,param_3,param_4,param_5,local_34 + 3,local_34);
    local_6c = 0;
    if (0 < *(int *)(iVar4 + 0x16c)) {
      local_78 = 0;
      pfVar7 = (float *)(local_80 + 0x6c);
      do {
        if ((((uint)pfVar7[0x18] & 0x10) == 0) &&
           ((param_13 == (code *)0x0 ||
            (iVar4 = (*param_13)(local_80,0,param_14), iVar5 = DAT_10362f1c, iVar4 != 0)))) {
          iVar4 = 0;
          pfVar6 = pfVar7 + 0xb;
          do {
            if ((local_34[iVar4] < pfVar6[-3]) || (*pfVar6 < local_34[iVar4 + 3])) break;
            iVar4 = iVar4 + 1;
            pfVar6 = pfVar6 + 1;
          } while (iVar4 < 3);
          if (iVar4 == 3) {
            pfVar6 = pfVar7 + -10;
            _DAT_10362f18 = 0x461c3c00;
            DAT_10362e90 = 0;
            fVar1 = (*param_4 - pfVar7[0xe]) - pfVar7[-1];
            fVar3 = (param_4[1] - pfVar7[0xf]) - *pfVar7;
            fVar2 = (param_4[2] - pfVar7[0x10]) - pfVar7[1];
            local_64 = fVar3 * pfVar7[-7] + fVar2 * pfVar7[-4] + fVar1 * *pfVar6;
            local_60 = fVar1 * pfVar7[-9] + fVar3 * pfVar7[-6] + fVar2 * pfVar7[-3];
            local_5c = fVar1 * pfVar7[-8] + fVar3 * pfVar7[-5] + fVar2 * pfVar7[-2];
            fVar1 = (*param_5 - pfVar7[0xe]) - pfVar7[-1];
            fVar3 = (param_5[1] - pfVar7[0xf]) - *pfVar7;
            fVar2 = (param_5[2] - pfVar7[0x10]) - pfVar7[1];
            fStack_40 = fVar3 * pfVar7[-7] + fVar2 * pfVar7[-4] + fVar1 * *pfVar6;
            fStack_3c = fVar1 * pfVar7[-9] + fVar3 * pfVar7[-6] + fVar2 * pfVar7[-3];
            DAT_10362ecc = local_64 + pfVar7[0xe];
            DAT_10362ed0 = local_60 + pfVar7[0xf];
            DAT_10362ed4 = local_5c + pfVar7[0x10];
            _DAT_10362ea8 = fStack_40 + pfVar7[0xe];
            _DAT_10362eac = fStack_3c + pfVar7[0xf];
            _DAT_10362eb0 =
                 fVar1 * pfVar7[-8] + fVar3 * pfVar7[-5] + fVar2 * pfVar7[-2] + pfVar7[0x10];
            FUN_10035470(param_2,param_3,&DAT_10362ecc,&DAT_10362ea8,&DAT_10362ed8,&DAT_10362ee4);
            FUN_100371f0(*(undefined4 *)(*(int *)(iVar5 + 0x48) + local_78));
            if (DAT_10362e90 != 0) {
              FUN_10001180(pfVar6,&DAT_10362f48,&DAT_10362f48);
              DAT_10362e9c = DAT_10362e9c - pfVar7[0xe];
              DAT_10362ea0 = DAT_10362ea0 - pfVar7[0xf];
              DAT_10362ea4 = DAT_10362ea4 - pfVar7[0x10];
              FUN_100010f0(pfVar6,&DAT_10362e9c,&local_64);
              DAT_10362e9c = local_64 + pfVar7[0xe];
              DAT_10362ea0 = local_60 + pfVar7[0xf];
              DAT_10362ea4 = local_5c + pfVar7[0x10];
              DAT_10362f54 = DAT_10362f48 * DAT_10362e9c +
                             DAT_10362f4c * DAT_10362ea0 + DAT_10362f50 * DAT_10362ea4;
              fStack_4c = DAT_10362e9c - *param_4;
              fStack_48 = DAT_10362ea0 - param_4[1];
              fStack_44 = DAT_10362ea4 - param_4[2];
              fVar9 = (float10)geVec3d_DotProduct(&fStack_4c,&fStack_4c);
              if (SQRT(fVar9) < (float10)local_7c) {
                local_7c = (float)SQRT(fVar9);
                local_54 = DAT_10362ea0;
                local_58 = DAT_10362e9c;
                local_50 = DAT_10362ea4;
                pfVar6 = &DAT_10362f48;
                pfVar8 = local_14;
                for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
                  *pfVar8 = *pfVar6;
                  pfVar6 = pfVar6 + 1;
                  pfVar8 = pfVar8 + 1;
                }
                local_68 = local_80;
                local_70 = 0;
                local_74 = 1;
              }
            }
            iVar5 = DAT_10362f1c;
            if ((param_6 & 8) != 0) break;
          }
        }
        local_78 = local_78 + 0x50;
        local_6c = local_6c + 1;
        local_80 = local_80 + 0xd4;
        pfVar7 = pfVar7 + 0x35;
      } while (local_6c < *(int *)(iVar5 + 0xa4));
    }
  }
  if (local_74 == 0) {
    return 0;
  }
  if (param_7 != (float *)0x0) {
    *param_7 = local_58;
    param_7[1] = local_54;
    param_7[2] = local_50;
  }
  if (param_8 != (float *)0x0) {
    pfVar7 = local_14;
    for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
      *param_8 = *pfVar7;
      pfVar7 = pfVar7 + 1;
      param_8 = param_8 + 1;
    }
  }
  if (param_10 != (undefined4 *)0x0) {
    *param_10 = 0;
  }
  if (param_9 != (int *)0x0) {
    *param_9 = local_68;
  }
  if (param_11 != (int *)0x0) {
    *param_11 = local_70;
  }
  return 1;
}

/* Ghidra entry 0x10038470 */
undefined4
FUN_10038470(int param_1,float *param_2,float *param_3,byte param_4,float *param_5,float *param_6,
            int *param_7,undefined4 *param_8,int *param_9,undefined4 param_10,code *param_11,
            undefined4 param_12)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  bool bVar7;
  int iVar8;
  float *pfVar9;
  float *pfVar10;
  float *pfVar11;
  float10 fVar12;
  int local_98;
  int local_94;
  float local_8c;
  int local_88;
  float fStack_84;
  float fStack_80;
  float fStack_7c;
  float fStack_78;
  float fStack_74;
  float fStack_70;
  float local_6c;
  float local_68;
  float local_64;
  int local_60;
  int local_5c;
  float local_58;
  float local_54;
  float local_50;
  float local_4c [4];
  undefined4 uStack_3c;
  float fStack_38;
  float fStack_34;
  float fStack_30;
  float fStack_2c;
  float fStack_28;
  float local_20 [5];
  float local_c [3];

  local_98 = *(int *)(*(int *)(param_1 + 0x18) + 0x1d0);
  DAT_10362f1c = *(int *)(param_1 + 0x18) + 200;
  local_88 = 0;
  if (param_7 != (int *)0x0) {
    *param_7 = 0;
  }
  if (param_8 != (undefined4 *)0x0) {
    *param_8 = 0;
  }
  if (param_9 != (int *)0x0) {
    *param_9 = 0;
  }
  local_8c = 99999.0;
  local_5c = 0;
  bVar7 = false;
  if (((param_4 & 4) != 0) &&
     (local_88 = FUN_100360f0(0,0,param_3,local_20,param_10,param_11,param_12,&local_8c),
     local_88 != 0)) {
    local_64 = local_50;
    local_6c = local_58;
    local_68 = local_54;
    pfVar10 = local_20;
    pfVar9 = local_4c;
    for (iVar8 = 5; iVar8 != 0; iVar8 = iVar8 + -1) {
      *pfVar9 = *pfVar10;
      pfVar10 = pfVar10 + 1;
      pfVar9 = pfVar9 + 1;
    }
    bVar7 = true;
  }
  if ((param_4 & 2) != 0) {
    FUN_10035470(s_Eclipse__100aa8bc + 8,&DAT_100aa8d0,param_2,param_3,local_20,local_c);
    local_60 = 0;
    if (0 < *(int *)(DAT_10362f1c + 0xa4)) {
      local_94 = 0;
      pfVar10 = (float *)(local_98 + 0x6c);
      do {
        if ((((uint)pfVar10[0x18] & 0x10) == 0) &&
           ((param_11 == (code *)0x0 || (iVar8 = (*param_11)(local_98,0,param_12), iVar8 != 0)))) {
          iVar8 = 0;
          pfVar9 = pfVar10 + 0xb;
          do {
            if ((local_c[iVar8] < pfVar9[-3]) || (*pfVar9 < local_20[iVar8])) break;
            iVar8 = iVar8 + 1;
            pfVar9 = pfVar9 + 1;
          } while (iVar8 < 3);
          if (iVar8 == 3) {
            pfVar9 = pfVar10 + -10;
            fVar1 = (*param_2 - pfVar10[0xe]) - pfVar10[-1];
            fVar3 = (param_2[1] - pfVar10[0xf]) - *pfVar10;
            fVar2 = (param_2[2] - pfVar10[0x10]) - pfVar10[1];
            local_58 = fVar1 * *pfVar9 + fVar3 * pfVar10[-7] + fVar2 * pfVar10[-4];
            local_54 = fVar1 * pfVar10[-9] + fVar3 * pfVar10[-6] + fVar2 * pfVar10[-3];
            fVar4 = (*param_3 - pfVar10[0xe]) - pfVar10[-1];
            fVar6 = (param_3[1] - pfVar10[0xf]) - *pfVar10;
            fVar5 = (param_3[2] - pfVar10[0x10]) - pfVar10[1];
            fStack_2c = fVar4 * *pfVar9 + fVar6 * pfVar10[-7] + fVar5 * pfVar10[-4];
            DAT_10362f44 = 0;
            fStack_28 = fVar4 * pfVar10[-9] + fVar6 * pfVar10[-6] + fVar5 * pfVar10[-3];
            fStack_78 = local_58 + pfVar10[0xe];
            fStack_74 = local_54 + pfVar10[0xf];
            fStack_70 = fVar1 * pfVar10[-8] + fVar3 * pfVar10[-5] + fVar2 * pfVar10[-2] +
                        pfVar10[0x10];
            fStack_84 = fStack_2c + pfVar10[0xe];
            fStack_80 = fStack_28 + pfVar10[0xf];
            fStack_7c = fVar4 * pfVar10[-8] + fVar6 * pfVar10[-5] + fVar5 * pfVar10[-2] +
                        pfVar10[0x10];
            iVar8 = FUN_100363f0(&fStack_78,&fStack_84,
                                 *(undefined4 *)(*(int *)(DAT_10362f1c + 0x48) + local_94));
            if (iVar8 != 0) {
              FUN_10001180(pfVar9,&DAT_10362f48,&DAT_10362f48);
              DAT_10362e9c = DAT_10362e9c - pfVar10[0xe];
              DAT_10362ea0 = DAT_10362ea0 - pfVar10[0xf];
              DAT_10362ea4 = DAT_10362ea4 - pfVar10[0x10];
              FUN_100010f0(pfVar9,&DAT_10362e9c,&DAT_10362e9c);
              DAT_10362e9c = DAT_10362e9c + pfVar10[0xe];
              DAT_10362ea0 = DAT_10362ea0 + pfVar10[0xf];
              DAT_10362ea4 = DAT_10362ea4 + pfVar10[0x10];
              DAT_10362f54 = DAT_10362f48 * DAT_10362e9c +
                             DAT_10362f4c * DAT_10362ea0 + DAT_10362f50 * DAT_10362ea4;
              fStack_38 = DAT_10362e9c - *param_2;
              fStack_34 = DAT_10362ea0 - param_2[1];
              fStack_30 = DAT_10362ea4 - param_2[2];
              fVar12 = (float10)geVec3d_DotProduct(&fStack_38,&fStack_38);
              if (SQRT(fVar12) < (float10)local_8c) {
                local_8c = (float)SQRT(fVar12);
                local_6c = DAT_10362e9c;
                local_68 = DAT_10362ea0;
                local_64 = DAT_10362ea4;
                pfVar9 = &DAT_10362f48;
                pfVar11 = local_4c;
                for (iVar8 = 5; iVar8 != 0; iVar8 = iVar8 + -1) {
                  *pfVar11 = *pfVar9;
                  pfVar9 = pfVar9 + 1;
                  pfVar11 = pfVar11 + 1;
                }
                if (DAT_10362f64 != 0) {
                  uStack_3c = 6;
                  local_4c[0] = -DAT_10362f48;
                  local_4c[1] = -local_4c[1];
                  local_4c[2] = -local_4c[2];
                  local_4c[3] = -local_4c[3];
                }
                local_88 = 0;
                local_5c = local_98;
                bVar7 = true;
              }
            }
            if ((param_4 & 8) != 0) break;
          }
        }
        local_94 = local_94 + 0x50;
        local_60 = local_60 + 1;
        local_98 = local_98 + 0xd4;
        pfVar10 = pfVar10 + 0x35;
      } while (local_60 < *(int *)(DAT_10362f1c + 0xa4));
    }
  }
  if (!bVar7) {
    return 0;
  }
  if (param_5 != (float *)0x0) {
    *param_5 = local_6c;
    param_5[1] = local_68;
    param_5[2] = local_64;
  }
  if (param_6 != (float *)0x0) {
    pfVar10 = local_4c;
    for (iVar8 = 5; iVar8 != 0; iVar8 = iVar8 + -1) {
      *param_6 = *pfVar10;
      pfVar10 = pfVar10 + 1;
      param_6 = param_6 + 1;
    }
  }
  if (param_7 != (int *)0x0) {
    *param_7 = local_5c;
  }
  if (param_8 != (undefined4 *)0x0) {
    *param_8 = 0;
  }
  if (param_9 != (int *)0x0) {
    *param_9 = local_88;
  }
  return 1;
}

/* Ghidra entry 0x100371F0 */
void FUN_100371f0(int param_1)

{
  uint uVar1;
  int iVar2;

  iVar2 = DAT_10362f24;
  while( true ) {
    if (param_1 < 0) {
      iVar2 = (-1 - param_1) * 0x3c;
      if (((*(uint *)(iVar2 + DAT_10362ef0) & DAT_10362f5c) != 0) &&
         (DAT_10362f44 = 0, *(int *)(iVar2 + DAT_10362ef0 + 0x38) != 0)) {
        FUN_10036ea0(&DAT_10362ecc,&DAT_10362ea8,-1 - param_1,0,1);
      }
      return;
    }
    param_1 = param_1 * 0x2c;
    uVar1 = FUN_10035210(&DAT_10362ed8,&DAT_10362ee4,
                         DAT_10362ef8 + *(int *)(param_1 + 0x10 + iVar2) * 0x14);
    if ((uVar1 & 1) != 0) {
      FUN_100371f0(*(undefined4 *)(param_1 + iVar2));
      iVar2 = DAT_10362f24;
    }
    if ((uVar1 & 2) == 0) break;
    param_1 = *(int *)(param_1 + 4 + iVar2);
  }
  return;
}

/* Ghidra entry 0x100363F0 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

uint FUN_100363f0(float *param_1,float *param_2,int param_3)

{
  float *pfVar1;
  float fVar2;
  float fVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  undefined4 *puVar7;
  undefined4 *puVar8;
  float local_c;
  float local_8;
  float local_4;

  if (param_3 < 0) {
    return (uint)((*(uint *)(*(int *)(DAT_10362f1c + 0x58) + (param_3 + 1) * -0x3c) & DAT_10362f5c)
                 != 0);
  }
  puVar7 = (undefined4 *)(*(int *)(DAT_10362f1c + 0x4c) + param_3 * 0x2c);
  iVar6 = *(int *)(*(int *)(DAT_10362f1c + 0x68) + 0x10 + puVar7[4] * 0x14);
  pfVar1 = (float *)(*(int *)(DAT_10362f1c + 0x68) + puVar7[4] * 0x14);
  fVar2 = pfVar1[3];
  if (iVar6 == 0) {
    fVar2 = *param_1 - fVar2;
  }
  else if (iVar6 == 1) {
    fVar2 = param_1[1] - fVar2;
  }
  else if (iVar6 == 2) {
    fVar2 = param_1[2] - fVar2;
  }
  else {
    fVar2 = (*pfVar1 * *param_1 + pfVar1[1] * param_1[1] + pfVar1[2] * param_1[2]) - fVar2;
  }
  fVar3 = pfVar1[3];
  if (iVar6 == 0) {
    fVar3 = *param_2 - fVar3;
  }
  else if (iVar6 == 1) {
    fVar3 = param_2[1] - fVar3;
  }
  else if (iVar6 == 2) {
    fVar3 = param_2[2] - fVar3;
  }
  else {
    fVar3 = (*pfVar1 * *param_2 + pfVar1[1] * param_2[1] + pfVar1[2] * param_2[2]) - fVar3;
  }
  if ((_DAT_100598d4 <= fVar2) && (_DAT_100598d4 <= fVar3)) {
    uVar4 = FUN_100363f0(param_1,param_2,*puVar7);
    return uVar4;
  }
  if (fVar2 < _DAT_100598d4) {
    if (fVar3 < _DAT_100598d4) {
      uVar4 = FUN_100363f0(param_1,param_2,puVar7[1]);
      return uVar4;
    }
    if (fVar2 < _DAT_100598d4) {
      iVar6 = 1;
      goto LAB_10036546;
    }
  }
  iVar6 = 0;
LAB_10036546:
  fVar2 = fVar2 / (fVar2 - fVar3);
  local_c = (*param_2 - *param_1) * fVar2 + *param_1;
  local_8 = (param_2[1] - param_1[1]) * fVar2 + param_1[1];
  local_4 = (param_2[2] - param_1[2]) * fVar2 + param_1[2];
  iVar5 = FUN_100363f0(param_1,&local_c,
                       *(undefined4 *)(*(int *)(DAT_10362f1c + 0x4c) + (param_3 * 0xb + iVar6) * 4))
  ;
  if (iVar5 == 0) {
    iVar5 = FUN_100363f0(&local_c,param_2,
                         *(undefined4 *)
                          (*(int *)(DAT_10362f1c + 0x4c) + ((uint)(iVar6 == 0) + param_3 * 0xb) * 4)
                        );
    if (iVar5 == 0) {
      return 0;
    }
    if (DAT_10362f44 == 0) {
      DAT_10362f60 = *(int *)(*(int *)(DAT_10362f1c + 0x4c) + 0x10 + param_3 * 0x2c);
      puVar7 = (undefined4 *)(*(int *)(DAT_10362f1c + 0x68) + DAT_10362f60 * 0x14);
      puVar8 = &DAT_10362f48;
      for (iVar5 = 5; iVar5 != 0; iVar5 = iVar5 + -1) {
        *puVar8 = *puVar7;
        puVar7 = puVar7 + 1;
        puVar8 = puVar8 + 1;
      }
      DAT_10362e9c = local_c;
      DAT_10362ea0 = local_8;
      DAT_10362ea4 = local_4;
      DAT_10362ec8 = param_3;
      DAT_10362f44 = 1;
      DAT_10362e94 = fVar2;
      DAT_10362f64 = iVar6;
    }
  }
  return 1;
}

/* Ghidra entry 0x10036EA0 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 FUN_10036ea0(float *param_1,float *param_2,int param_3,int param_4,int param_5)

{
  float fVar1;
  float fVar2;
  float fVar3;
  int iVar4;
  undefined4 uVar5;
  int iVar6;
  float *pfVar7;
  float *pfVar8;
  float10 fVar9;
  float local_2c;
  float local_28;
  float local_24;
  float local_20;
  float local_1c;
  float local_18;
  float local_14 [4];
  int local_4;

  if (param_5 == 0) {
    return 0;
  }
  iVar4 = param_3 * 0x3c + DAT_10362ef0;
  if (*(int *)(iVar4 + 0x38) <= param_4) {
    return 1;
  }
  iVar4 = *(int *)(iVar4 + 0x34) + param_4;
  pfVar7 = (float *)(DAT_10362ef8 + *(int *)(DAT_10362f0c + iVar4 * 8) * 0x14);
  pfVar8 = local_14;
  for (iVar6 = 5; iVar6 != 0; iVar6 = iVar6 + -1) {
    *pfVar8 = *pfVar7;
    pfVar7 = pfVar7 + 1;
    pfVar8 = pfVar8 + 1;
  }
  local_4 = 6;
  if (*(int *)(DAT_10362f0c + 4 + iVar4 * 8) != 0) {
    local_14[0] = -local_14[0];
    local_14[1] = -local_14[1];
    local_14[2] = -local_14[2];
    local_14[3] = -local_14[3];
  }
  FUN_10035320(local_14,&DAT_10362e78,&DAT_10362f00);
  if (local_4 == 0) {
    fVar1 = *param_1;
  }
  else if (local_4 == 1) {
    fVar1 = param_1[1];
  }
  else if (local_4 == 2) {
    fVar1 = param_1[2];
  }
  else {
    fVar1 = local_14[0] * *param_1 + local_14[1] * param_1[1] + local_14[2] * param_1[2];
  }
  fVar1 = fVar1 - local_14[3];
  if (local_4 == 0) {
    fVar2 = *param_2;
  }
  else if (local_4 == 1) {
    fVar2 = param_2[1];
  }
  else if (local_4 == 2) {
    fVar2 = param_2[2];
  }
  else {
    fVar2 = local_14[0] * *param_2 + local_14[1] * param_2[1] + local_14[2] * param_2[2];
  }
  fVar2 = fVar2 - local_14[3];
  if ((_DAT_100598d4 <= fVar1) && (_DAT_100598d4 <= fVar2)) {
    uVar5 = FUN_10036ea0(param_1,param_2,param_3,param_4 + 1,0);
    return uVar5;
  }
  if (fVar1 < _DAT_100598d4) {
    if (fVar2 < _DAT_100598d4) {
      uVar5 = FUN_10036ea0(param_1,param_2,param_3,param_4 + 1,1);
      return uVar5;
    }
    if (fVar1 < _DAT_100598d4) {
      iVar4 = 1;
      goto LAB_1003705e;
    }
  }
  iVar4 = 0;
LAB_1003705e:
  if (_DAT_100598d4 <= fVar1) {
    fVar3 = fVar1 - _DAT_10051330;
  }
  else {
    fVar3 = fVar1 + _DAT_10051330;
  }
  fVar3 = fVar3 / (fVar1 - fVar2);
  fVar1 = _DAT_100598d4;
  if ((_DAT_100598d4 <= fVar3) && (fVar1 = fVar3, _DAT_1005118c < fVar3)) {
    fVar1 = _DAT_1005118c;
  }
  local_2c = (*param_2 - *param_1) * fVar1 + *param_1;
  local_28 = (param_2[1] - param_1[1]) * fVar1 + param_1[1];
  local_24 = (param_2[2] - param_1[2]) * fVar1 + param_1[2];
  iVar6 = FUN_10036ea0(param_1,&local_2c,param_3,param_4 + 1,iVar4);
  if (iVar6 == 0) {
    iVar4 = FUN_10036ea0(&local_2c,param_2,param_3,param_4 + 1,iVar4 == 0);
    if (iVar4 == 0) {
      return 0;
    }
    local_20 = local_2c - DAT_10362ecc;
    local_1c = local_28 - DAT_10362ed0;
    local_18 = local_24 - DAT_10362ed4;
    fVar9 = (float10)geVec3d_DotProduct(&local_20,&local_20);
    fVar9 = SQRT(fVar9);
    if ((fVar9 < (float10)_DAT_10362f18) && (DAT_10362f44 == 0)) {
      _DAT_10362f18 = (float)fVar9;
      DAT_10362e94 = (float)fVar9;
      DAT_10362ea0 = local_28;
      pfVar7 = local_14;
      pfVar8 = (float *)&DAT_10362f48;
      for (iVar4 = 5; iVar4 != 0; iVar4 = iVar4 + -1) {
        *pfVar8 = *pfVar7;
        pfVar7 = pfVar7 + 1;
        pfVar8 = pfVar8 + 1;
      }
      DAT_10362e9c = local_2c;
      _DAT_10362f10 = param_3;
      DAT_10362ea4 = local_24;
      DAT_10362f44 = 1;
      DAT_10362e90 = 1;
      return 1;
    }
  }
  DAT_10362e90 = 1;
  return 1;
}

/* Ghidra entry 0x10035470 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void FUN_10035470(float *param_1,float *param_2,float *param_3,float *param_4,float *param_5,
                 float *param_6)

{
  float fVar1;

  if (*param_4 <= *param_3) {
    *param_5 = (*param_1 + *param_4) - _DAT_1005118c;
    fVar1 = *param_2 + *param_3;
  }
  else {
    *param_5 = (*param_1 + *param_3) - _DAT_1005118c;
    fVar1 = *param_2 + *param_4;
  }
  *param_6 = fVar1 + _DAT_1005118c;
  if (param_4[1] <= param_3[1]) {
    param_5[1] = (param_1[1] + param_4[1]) - _DAT_1005118c;
    fVar1 = param_2[1] + param_3[1];
  }
  else {
    param_5[1] = (param_1[1] + param_3[1]) - _DAT_1005118c;
    fVar1 = param_2[1] + param_4[1];
  }
  param_6[1] = fVar1 + _DAT_1005118c;
  if (param_3[2] < param_4[2]) {
    param_5[2] = (param_1[2] + param_3[2]) - _DAT_1005118c;
    param_6[2] = param_2[2] + param_4[2] + _DAT_1005118c;
    return;
  }
  param_5[2] = (param_1[2] + param_4[2]) - _DAT_1005118c;
  param_6[2] = param_2[2] + param_3[2] + _DAT_1005118c;
  return;
}

/* Ghidra entry 0x10035210 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

byte FUN_10035210(float *param_1,float *param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  byte bVar5;
  float local_10;
  float local_8;

  fVar2 = param_3[4];
  if ((int)fVar2 < 3) {
    bVar5 = param_3[3] <= param_2[(int)fVar2];
    if (param_1[(int)fVar2] < param_3[3]) {
      bVar5 = bVar5 | 2;
    }
    return bVar5;
  }
  if (_DAT_100598d4 <= *param_3) {
    fVar4 = *param_1;
    fVar2 = *param_2;
  }
  else {
    fVar2 = *param_1;
    fVar4 = *param_2;
  }
  local_8 = param_1[1];
  if (_DAT_100598d4 <= param_3[1]) {
    fVar1 = param_2[1];
  }
  else {
    fVar1 = local_8;
    local_8 = param_2[1];
  }
  if (_DAT_100598d4 <= param_3[2]) {
    local_10 = param_2[2];
    fVar3 = param_1[2];
  }
  else {
    fVar3 = param_2[2];
    local_10 = param_1[2];
  }
  bVar5 = _DAT_100598d4 <=
          (fVar2 * *param_3 + fVar1 * param_3[1] + local_10 * param_3[2]) - param_3[3];
  if ((fVar4 * *param_3 + local_8 * param_3[1] + fVar3 * param_3[2]) - param_3[3] < _DAT_100598d4) {
    bVar5 = bVar5 | 2;
  }
  return bVar5;
}
