/* Fresh Ghidra 12.0.4 decompilation excerpts from the saved hash-pinned project.
 * Program: FDNYFirefighter.exe
 * SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e
 * Captured 2026-10-19 UTC from .local/ghidra/playerhose-curve-functions.log.
 * Ghidra function boundaries were used; log framing was removed. Temporary
 * variables and generic names are preserved. This is analysis evidence, not
 * reconstructed source and not a runtime-parity claim.
 */

/* Address 004661A0 */
void __fastcall FUN_004661a0(void *param_1)

{
  undefined **ppuVar1;
  undefined *puVar2;
  undefined **ppuVar3;
  undefined ***pppuVar4;
  undefined1 *this;
  int iVar5;
  int iVar6;
  undefined4 local_f4;
  undefined4 local_f0;
  undefined4 local_ec;
  undefined **local_e8;
  char *local_e4;
  undefined4 local_e0;
  undefined1 local_dc;
  undefined1 local_cc [60];
  void *local_90;
  undefined **local_8c [3];
  undefined1 local_80;
  undefined1 local_70 [100];
  void *local_c;
  undefined1 *puStack_8;
  undefined4 local_4;
  
  local_4 = 0xffffffff;
  puStack_8 = &LAB_004d1be2;
  local_c = ExceptionList;
  local_e8 = &PTR_FUN_004dc554;
  ExceptionList = &local_c;
  local_90 = param_1;
  FUN_004a3118();
  local_e4 = "hose\\PlayerHoseEnt.cpp";
  local_e0 = 0x198;
  local_dc = 0;
  local_4 = 0;
  FUN_004a3a19(local_cc,"PlayerHoseEnt: ");
  FUN_004a3a19(local_cc,"Building spline path");
  FUN_00401360(&local_e8,0);
  local_4 = 0xffffffff;
  FUN_004012c0(&local_e8);
  local_8c[0] = *(undefined ***)(*(int *)((int)param_1 + 0x194) + 0x10);
  if (local_8c[0] == (undefined **)0x0) {
    local_e8 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_e4 = "hose\\PlayerHoseEnt.cpp";
    local_e0 = 0x1a1;
    local_dc = 0;
    local_4 = 1;
    FUN_004a3a19(local_cc,"PlayerHoseEnt: ");
    FUN_004a3a19(local_cc,"Error: Invalid FirstPathPoint.");
    FUN_00401360(&local_e8,0);
    pppuVar4 = &local_e8;
  }
  else {
    iVar6 = 1;
    pppuVar4 = local_8c;
    ppuVar3 = local_8c[0];
    do {
      ppuVar1 = (undefined **)(*pppuVar4)[4];
      if (ppuVar1 == ppuVar3) break;
      if (ppuVar1 == (undefined **)0x0) {
        *(undefined ***)((int)(&local_90)[iVar6] + 0x10) = ppuVar3;
        local_e8 = &PTR_FUN_004dc554;
        FUN_004a3118();
        local_e4 = "hose\\PlayerHoseEnt.cpp";
        local_e0 = 0x1ad;
        local_dc = 0;
        local_4 = 2;
        FUN_004a3a19(local_cc,"PlayerHoseEnt: ");
        FUN_004a3a19(local_cc,"Closing loop at HosePathPoint ");
        FUN_004a3f7a(local_cc,iVar6 + -1);
        FUN_00401360(&local_e8,0);
        local_4 = 0xffffffff;
        FUN_004012c0(&local_e8);
        break;
      }
      pppuVar4[1] = ppuVar1;
      iVar6 = iVar6 + 1;
      pppuVar4 = pppuVar4 + 1;
      ppuVar3 = local_8c[0];
    } while (iVar6 < 0x20);
    local_e8 = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_e4 = "hose\\PlayerHoseEnt.cpp";
    local_e0 = 0x1b6;
    local_dc = 0;
    local_4 = 3;
    FUN_004a3a19(local_cc,"PlayerHoseEnt: ");
    FUN_004a3a19(local_cc,"Found ");
    FUN_004a3f7a(local_cc,iVar6);
    FUN_004a3a19(local_cc," HosePathPoints.");
    FUN_00401360(&local_e8,0);
    local_4 = 0xffffffff;
    FUN_004012c0(&local_e8);
    this = (undefined1 *)((int)param_1 + 0x1a28);
    FUN_00490510(this,iVar6,10,'\x01');
    iVar5 = 0;
    local_f4 = 0;
    local_ec = 0;
    local_f0 = 0;
    if (0 < iVar6) {
      do {
        ppuVar3 = local_8c[iVar5];
        puVar2 = ppuVar3[5];
        if (puVar2 == (undefined *)0x0) {
          local_e8 = &PTR_FUN_004dc554;
          FUN_004a3118();
          local_e4 = "hose\\PlayerHoseEnt.cpp";
          local_e0 = 0x1c0;
          local_dc = 0;
          local_4 = 4;
          FUN_004a3a19(local_cc,"PlayerHoseEnt: ");
          FUN_004a3a19(local_cc,"Invalid LookAtPoint for HosePathPoint ");
          FUN_004a3f7a(local_cc,iVar5);
          FUN_00401360(&local_e8,0);
          local_4 = 0xffffffff;
          FUN_004012c0(&local_e8);
        }
        else {
          local_f4 = *(undefined4 *)(puVar2 + 4);
          local_ec = *(undefined4 *)(puVar2 + 8);
          local_f0 = *(undefined4 *)(puVar2 + 0xc);
        }
        FUN_00490d00(this,ppuVar3[1],ppuVar3[3],0,local_f4,local_ec,local_f0,ppuVar3);
        if (ppuVar3[6] != (undefined *)0x0) {
          FUN_004660f0(local_90,(int)ppuVar3);
        }
        iVar5 = iVar5 + 1;
      } while (iVar5 < iVar6);
    }
    FUN_004905a0(this);
    local_8c[0] = &PTR_FUN_004dc554;
    FUN_004a3118();
    local_8c[1] = (undefined **)0x4e2c20;
    local_8c[2] = (undefined **)0x1cf;
    local_80 = 0;
    local_4 = 5;
    FUN_004a3a19(local_70,"PlayerHoseEnt: ");
    FUN_004a3a19(local_70,"Finished building spline.");
    FUN_00401360(local_8c,0);
    pppuVar4 = local_8c;
  }
  local_4 = 0xffffffff;
  FUN_004012c0(pppuVar4);
  ExceptionList = local_c;
  return;
}

/* Address 00490510 */
void __thiscall FUN_00490510(void *this,int param_1,int param_2,char param_3)

{
  void *pvVar1;
  
  if ((param_1 != 0) && (param_2 != 0)) {
    *(char *)((int)this + 0x20) = param_3;
    *(int *)((int)this + 0xc) = param_1;
    if (param_3 == '\0') {
      param_1 = param_1 + -1;
    }
    *(int *)((int)this + 0x14) = param_1 * param_2;
    if (*(void **)((int)this + 0x10) != (void *)0x0) {
      _free(*(void **)((int)this + 0x10));
    }
    if (*(void **)((int)this + 4) != (void *)0x0) {
      _free(*(void **)((int)this + 4));
    }
    *(undefined4 *)((int)this + 8) = 0;
    *(undefined1 *)this = 0;
    *(undefined1 *)((int)this + 1) = 1;
    pvVar1 = operator_new(*(int *)((int)this + 0xc) << 5);
    *(void **)((int)this + 4) = pvVar1;
    pvVar1 = operator_new((*(int *)((int)this + 0x14) + 1) * 0x10);
    *(void **)((int)this + 0x10) = pvVar1;
    *(undefined4 *)((int)this + 0x1c) = 0;
  }
  return;
}

/* Address 004905A0 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004905a0(undefined1 *param_1)

{
  float *pfVar1;
  int iVar2;
  undefined4 *puVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  float fVar10;
  float fVar11;
  float fVar12;
  int iVar13;
  undefined4 *puVar14;
  int iVar15;
  float *pfVar16;
  float *pfVar17;
  int iVar18;
  float *pfVar19;
  uint uVar20;
  int iVar21;
  float *pfVar22;
  uint uVar23;
  int local_44;
  int local_40;
  int local_3c;
  uint local_38;
  int local_30;
  
  if (param_1[1] != '\0') {
    param_1[1] = 0;
    if (param_1[0x20] == '\0') {
      iVar13 = *(int *)(param_1 + 0xc) + -1;
    }
    else {
      iVar13 = *(int *)(param_1 + 0xc);
    }
    iVar13 = *(int *)(param_1 + 0x14) / iVar13;
    *(undefined4 *)(*(int *)(param_1 + 4) + 0xc) = 0;
    *(undefined4 *)(*(int *)(param_1 + 0x10) + 0xc) = 0;
    iVar15 = *(int *)(param_1 + 0xc);
    if (param_1[0x20] == '\0') {
      iVar15 = iVar15 + -1;
    }
    if (0 < iVar15) {
      local_40 = 0;
      local_30 = 0;
      local_44 = -0x20;
      local_38 = 0;
      do {
        pfVar1 = *(float **)(param_1 + 4);
        if (param_1[0x20] == '\0') {
          pfVar17 = pfVar1;
          if (-1 < (int)(local_38 - 1)) {
            pfVar17 = (float *)(local_44 + (int)pfVar1);
          }
          iVar18 = *(int *)(param_1 + 0xc);
          pfVar16 = (float *)(local_44 + 0x20 + (int)pfVar1);
          pfVar19 = (float *)(local_44 + 0x40 + (int)pfVar1);
          pfVar22 = pfVar19;
          if ((int)(local_38 + 2) < iVar18) {
            pfVar22 = (float *)(local_44 + 0x60 + (int)pfVar1);
          }
        }
        else {
          iVar18 = local_44;
          if ((int)(local_38 - 1) < 0) {
            iVar18 = *(int *)(param_1 + 0xc) * 0x20 + -0x20;
          }
          pfVar17 = (float *)(iVar18 + (int)pfVar1);
          iVar18 = *(int *)(param_1 + 0xc);
          for (uVar20 = local_38; iVar18 <= (int)uVar20; uVar20 = uVar20 - iVar18) {
          }
          pfVar16 = pfVar1 + uVar20 * 8;
          iVar21 = uVar20 + 1;
          if (iVar18 <= iVar21) {
            do {
              iVar21 = iVar21 - iVar18;
            } while (*(int *)(param_1 + 0xc) <= iVar21);
          }
          pfVar19 = pfVar1 + iVar21 * 8;
          iVar21 = iVar21 + 1;
          if (iVar18 <= iVar21) {
            do {
              iVar21 = iVar21 - iVar18;
            } while (*(int *)(param_1 + 0xc) <= iVar21);
          }
          pfVar22 = pfVar1 + iVar21 * 8;
        }
        fVar4 = (*pfVar19 - *pfVar17) * (float)_DAT_004de6f8;
        fVar5 = (pfVar19[1] - pfVar17[1]) * (float)_DAT_004de6f8;
        fVar6 = (pfVar19[2] - pfVar17[2]) * (float)_DAT_004de6f8;
        fVar7 = (*pfVar22 - *pfVar16) * (float)_DAT_004de6f8;
        fVar8 = (pfVar22[1] - pfVar16[1]) * (float)_DAT_004de6f8;
        uVar23 = local_38 + 1;
        fVar9 = (pfVar22[2] - pfVar16[2]) * (float)_DAT_004de6f8;
        uVar20 = uVar23 & (iVar18 <= (int)uVar23) - 1;
        pfVar1[uVar20 * 8 + 3] = *(float *)(local_44 + 0x2c + (int)pfVar1);
        local_38 = 0;
        if (0 < iVar13) {
          local_3c = local_40 + -1;
          iVar18 = local_30;
          do {
            if (*(int *)(param_1 + 0x14) <= (int)(local_40 + local_38)) break;
            fVar12 = (float)(int)local_38 / (float)iVar13;
            fVar10 = fVar12 * fVar12;
            fVar11 = fVar10 * fVar12;
            iVar21 = local_3c;
            if (local_3c < 0) {
              iVar21 = 0;
            }
            iVar2 = *(int *)(param_1 + 0x10);
            pfVar17 = (float *)(iVar21 * 0x10 + iVar2);
            *(float *)(iVar2 + iVar18) =
                 fVar4 * _DAT_004dc50c + *pfVar19 * _DAT_004dc50c + fVar7 * _DAT_004dc50c + *pfVar16
                 + (*pfVar16 * _DAT_004dc50c + *pfVar19 * _DAT_004dc50c + fVar7 * _DAT_004dc50c +
                   fVar4) * fVar12 +
                   (((*pfVar16 + *pfVar16) - (*pfVar19 + *pfVar19)) + fVar7 + fVar4) * fVar11 +
                   (((*pfVar19 * _DAT_004e16c4 - *pfVar16 * _DAT_004e16c4) - (fVar4 + fVar4)) -
                   fVar7) * fVar10;
            *(float *)(iVar2 + 4 + iVar18) =
                 fVar5 * _DAT_004dc50c + fVar8 * _DAT_004dc50c + pfVar19[1] * _DAT_004dc50c +
                 pfVar16[1] +
                 (pfVar16[1] * _DAT_004dc50c + fVar8 * _DAT_004dc50c + pfVar19[1] * _DAT_004dc50c +
                 fVar5) * fVar12 +
                 (((pfVar16[1] + pfVar16[1]) - (pfVar19[1] + pfVar19[1])) + fVar8 + fVar5) * fVar11
                 + (((pfVar19[1] * _DAT_004e16c4 - pfVar16[1] * _DAT_004e16c4) - (fVar5 + fVar5)) -
                   fVar8) * fVar10;
            fVar10 = fVar6 * _DAT_004dc50c + fVar9 * _DAT_004dc50c + pfVar19[2] * _DAT_004dc50c +
                     pfVar16[2] +
                     (pfVar16[2] * _DAT_004dc50c + fVar9 * _DAT_004dc50c +
                      pfVar19[2] * _DAT_004dc50c + fVar6) * fVar12 +
                     (((pfVar16[2] + pfVar16[2]) - (pfVar19[2] + pfVar19[2])) + fVar9 + fVar6) *
                     fVar11 + (((pfVar19[2] * _DAT_004e16c4 - pfVar16[2] * _DAT_004e16c4) -
                               (fVar6 + fVar6)) - fVar9) * fVar10;
            *(float *)(iVar2 + 8 + iVar18) = fVar10;
            fVar10 = fVar10 - pfVar17[2];
            pfVar1 = (float *)(uVar20 * 0x20 + 0xc + *(int *)(param_1 + 4));
            fVar12 = *(float *)(iVar2 + 4 + iVar18) - pfVar17[1];
            fVar11 = *(float *)(iVar2 + iVar18) - *pfVar17;
            fVar10 = SQRT(fVar10 * fVar10 + fVar12 * fVar12 + fVar11 * fVar11);
            *pfVar1 = fVar10 + *pfVar1;
            *(float *)(iVar2 + 0xc + iVar18) = fVar10 + pfVar17[3];
            local_3c = local_3c + 1;
            iVar18 = iVar18 + 0x10;
            local_38 = local_38 + 1;
          } while ((int)local_38 < iVar13);
        }
        local_40 = local_40 + iVar13;
        local_30 = local_30 + iVar13 * 0x10;
        local_44 = local_44 + 0x20;
        local_38 = uVar23;
      } while ((int)uVar23 < iVar15);
    }
    if (param_1[0x20] != '\0') {
      pfVar1 = *(float **)(param_1 + 0x10);
      fVar5 = (pfVar1 + *(int *)(param_1 + 0x14) * 4 + -4)[1] - pfVar1[1];
      fVar4 = pfVar1[*(int *)(param_1 + 0x14) * 4 + -4] - *pfVar1;
      *(float *)(*(int *)(param_1 + 4) + 0xc) =
           SQRT((pfVar1[*(int *)(param_1 + 0x14) * 4 + -2] - pfVar1[2]) *
                (pfVar1[*(int *)(param_1 + 0x14) * 4 + -2] - pfVar1[2]) +
                fVar5 * fVar5 + fVar4 * fVar4) + *(float *)(*(int *)(param_1 + 4) + 0xc);
      puVar3 = *(undefined4 **)(param_1 + 4);
      puVar14 = (undefined4 *)(*(int *)(param_1 + 0x14) * 0x10 + *(int *)(param_1 + 0x10));
      *puVar14 = *puVar3;
      puVar14[1] = puVar3[1];
      puVar14[2] = puVar3[2];
      puVar14[3] = puVar3[3];
      *(undefined4 *)(param_1 + 0x18) = *(undefined4 *)(*(int *)(param_1 + 4) + 0xc);
      *(undefined4 *)(*(int *)(param_1 + 4) + 0xc) = 0;
      *param_1 = 1;
      return;
    }
    pfVar1 = (float *)(*(int *)(param_1 + 0x14) * 0x10 + -0x10 + *(int *)(param_1 + 0x10));
    iVar13 = *(int *)(param_1 + 0xc) * 0x20 + *(int *)(param_1 + 4);
    fVar4 = *(float *)(*(int *)(param_1 + 0xc) * 0x20 + -0x18 + *(int *)(param_1 + 4)) - pfVar1[2];
    fVar6 = *(float *)(iVar13 + -0x1c) - pfVar1[1];
    fVar5 = *(float *)(iVar13 + -0x20) - *pfVar1;
    *(float *)(iVar13 + -0x14) =
         SQRT(fVar4 * fVar4 + fVar6 * fVar6 + fVar5 * fVar5) + *(float *)(iVar13 + -0x14);
    iVar15 = *(int *)(param_1 + 0x10);
    iVar13 = *(int *)(param_1 + 0xc) * 0x20 + -0x20 + *(int *)(param_1 + 4);
    iVar18 = *(int *)(param_1 + 0x14);
    *(undefined4 *)(iVar18 * 0x10 + iVar15) =
         *(undefined4 *)(*(int *)(param_1 + 0xc) * 0x20 + -0x20 + *(int *)(param_1 + 4));
    iVar15 = iVar18 * 0x10 + iVar15;
    *(undefined4 *)(iVar15 + 4) = *(undefined4 *)(iVar13 + 4);
    *(undefined4 *)(iVar15 + 8) = *(undefined4 *)(iVar13 + 8);
    *(undefined4 *)(iVar15 + 0xc) = *(undefined4 *)(iVar13 + 0xc);
    *(undefined4 *)(param_1 + 0x18) =
         *(undefined4 *)(*(int *)(param_1 + 0xc) * 0x20 + -0x14 + *(int *)(param_1 + 4));
    *(undefined4 *)(*(int *)(param_1 + 4) + 0xc) = 0;
    *param_1 = 1;
  }
  return;
}

/* Address 00490D00 */
void __thiscall
FUN_00490d00(void *this,undefined4 param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4,
            undefined4 param_5,undefined4 param_6,undefined4 param_7)

{
  int iVar1;
  
  if (*(char *)((int)this + 1) != '\0') {
    *(undefined4 *)(*(int *)((int)this + 8) * 0x20 + *(int *)((int)this + 4)) = param_1;
    *(undefined4 *)(*(int *)((int)this + 8) * 0x20 + 4 + *(int *)((int)this + 4)) = param_2;
    *(undefined4 *)(*(int *)((int)this + 8) * 0x20 + 8 + *(int *)((int)this + 4)) = param_3;
    *(undefined4 *)(*(int *)((int)this + 8) * 0x20 + 0x14 + *(int *)((int)this + 4)) = param_4;
    *(undefined4 *)(*(int *)((int)this + 8) * 0x20 + 0x18 + *(int *)((int)this + 4)) = param_5;
    *(undefined4 *)(*(int *)((int)this + 8) * 0x20 + 0x1c + *(int *)((int)this + 4)) = param_6;
    *(undefined4 *)(*(int *)((int)this + 8) * 0x20 + 0x10 + *(int *)((int)this + 4)) = param_7;
    iVar1 = *(int *)((int)this + 8) + 1;
    *(int *)((int)this + 8) = iVar1;
    if (*(int *)((int)this + 0xc) <= iVar1) {
      FUN_004905a0(this);
    }
  }
  return;
}

/* Address 00490AB0 */
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

undefined4 __thiscall
FUN_00490ab0(void *this,float param_1,float *param_2,float *param_3,float *param_4)

{
  float *pfVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  uint in_EAX;
  undefined2 uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  int iVar9;
  
  if (*(char *)this != '\0') {
    fVar2 = (float)_DAT_004dd808;
    uVar5 = (undefined2)(in_EAX >> 0x10);
    in_EAX = CONCAT22(uVar5,(ushort)(param_1 < fVar2) << 8 |
                            (ushort)(NAN(param_1) || NAN(fVar2)) << 10 |
                            (ushort)(param_1 == fVar2) << 0xe);
    if ((param_1 < fVar2) &&
       (in_EAX = CONCAT22(uVar5,(ushort)(param_1 < _DAT_004dc50c) << 8 |
                                (ushort)(NAN(param_1) || NAN(_DAT_004dc50c)) << 10 |
                                (ushort)(param_1 == _DAT_004dc50c) << 0xe), param_1 >= _DAT_004dc50c
       )) {
      fVar2 = param_1 * *(float *)((int)this + 0x18);
      iVar9 = 0;
      if (1 < *(int *)((int)this + 0x14)) {
        iVar7 = *(int *)((int)this + 0x14);
        do {
          iVar6 = (iVar7 + iVar9) / 2;
          pfVar1 = (float *)(iVar6 * 0x10 + 0xc + *(int *)((int)this + 0x10));
          iVar8 = iVar7;
          if (((fVar2 <= *pfVar1) && (iVar8 = iVar6, iVar6 = iVar9, *pfVar1 <= fVar2)) &&
             (iVar8 = iVar7, fVar2 == *pfVar1)) break;
          iVar7 = iVar8;
          iVar9 = iVar6;
        } while (iVar6 + 1 < iVar8);
      }
      pfVar1 = (float *)(iVar9 * 0x10 + *(int *)((int)this + 0x10));
      fVar4 = *(float *)(iVar9 * 0x10 + 0x1c + *(int *)((int)this + 0x10)) - pfVar1[3];
      fVar3 = _DAT_004dc50c;
      if ((float)_DAT_004e3540 <= fVar4) {
        fVar3 = (fVar2 - pfVar1[3]) / fVar4;
      }
      *param_2 = (pfVar1[4] - *pfVar1) * fVar3 + *pfVar1;
      *param_3 = (pfVar1[5] - pfVar1[1]) * fVar3 + pfVar1[1];
      *param_4 = (pfVar1[6] - pfVar1[2]) * fVar3 + pfVar1[2];
      return CONCAT31((int3)((uint)param_3 >> 8),1);
    }
  }
  return in_EAX & 0xffffff00;
}

/* Address 00490BB0 */
uint __thiscall
FUN_00490bb0(void *this,float param_1,float *param_2,float *param_3,float *param_4,float *param_5,
            float *param_6,float *param_7,uint *param_8)

{
  int iVar1;
  float fVar2;
  int iVar3;
  float fVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  float *pfVar8;
  
  uVar5 = FUN_00490ab0(this,param_1,param_2,param_3,param_4);
  if ((char)uVar5 == '\0') {
    return uVar5;
  }
  fVar4 = param_1 * *(float *)((int)this + 0x18);
  iVar3 = *(int *)((int)this + 0xc);
  if (0 < iVar3) {
    iVar7 = *(int *)((int)this + 4);
    pfVar8 = (float *)(iVar7 + 0xc);
    iVar6 = 0;
    do {
      iVar1 = iVar6 + 1;
      if (iVar1 < iVar3) {
        fVar2 = pfVar8[8];
      }
      else {
        fVar2 = *(float *)((int)this + 0x18);
      }
      if ((*pfVar8 <= fVar4) && (fVar4 <= fVar2)) {
        uVar5 = iVar6 * 0x20 + iVar7;
        if (iVar6 + 1 < iVar3) {
          iVar7 = uVar5 + 0x20;
        }
        fVar4 = ((fVar4 - *(float *)(uVar5 + 0xc)) / (fVar2 - *(float *)(uVar5 + 0xc)) +
                (float)iVar6) - (float)iVar6;
        *param_5 = (*(float *)(iVar7 + 0x14) - *(float *)(uVar5 + 0x14)) * fVar4 +
                   *(float *)(uVar5 + 0x14);
        *param_6 = (*(float *)(iVar7 + 0x18) - *(float *)(uVar5 + 0x18)) * fVar4 +
                   *(float *)(uVar5 + 0x18);
        *param_7 = (*(float *)(iVar7 + 0x1c) - *(float *)(uVar5 + 0x1c)) * fVar4 +
                   *(float *)(uVar5 + 0x1c);
        *param_8 = (uVar5 == *(uint *)((int)this + 0x1c)) - 1 & uVar5;
        *(uint *)((int)this + 0x1c) = uVar5;
        return CONCAT31((int3)(uVar5 >> 8),1);
      }
      uVar5 = *(uint *)((int)this + 0xc);
      pfVar8 = pfVar8 + 8;
      iVar6 = iVar1;
    } while (iVar1 < (int)uVar5);
  }
  return uVar5 & 0xffffff00;
}
