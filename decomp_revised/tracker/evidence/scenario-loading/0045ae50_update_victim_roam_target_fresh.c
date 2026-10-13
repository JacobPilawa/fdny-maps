/* Fresh Ghidra decompilation: UpdateVictimRoamTarget at 0x0045ae50; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* When the victim is within 36 units of its current target, computes a subsequent candidate using
   stored direction/target fields and player position, validates candidates, and updates target
   state. The helper passed 432.0 and some target-selection constants are not semantically resolved.
    */

void __fastcall UpdateVictimRoamTarget(int *param_1)

{
  float *this;
  char cVar1;
  float *pfVar2;
  uint uVar3;
  int iVar4;
  undefined4 uVar5;
  void *pvVar6;
  float *pfVar7;
  float10 fVar8;
  float10 fVar9;
  float local_48;
  float local_44;
  float local_40;
  float local_3c;
  float local_38;
  float local_34;
  float fStack_30;
  undefined4 uStack_2c;
  undefined4 uStack_28;
  float afStack_24 [3];
  float afStack_18 [3];
  float afStack_c [3];

  this = (float *)(param_1 + 0x72);
  local_48 = 0.0;
  local_44 = 0.0;
  local_40 = 0.0;
  FUN_0043b2d0(&local_3c,this,(float *)(param_1 + 0x13));
  if (_DAT_004e2328 < local_3c * local_3c + local_38 * local_38 + local_34 * local_34) {
    return;
  }
  if ((char)param_1[0x7b] != '\0') {
    pfVar7 = (float *)(param_1 + 0x78);
    fVar8 = FUN_00498b30(6.0,18.0);
    FUN_0045a1e0(pfVar7,(float)fVar8);
    fVar8 = FUN_00404ac0();
    pfVar2 = (float *)FUN_0045a190(pfVar7,&local_3c,(float)(fVar8 * (float10)_DAT_004e1c24));
    *pfVar7 = *pfVar2;
    param_1[0x79] = (int)pfVar2[1];
    param_1[0x7a] = (int)pfVar2[2];
    param_1[0x79] = 0;
    local_48 = *pfVar7;
    local_44 = (float)param_1[0x79];
    local_40 = (float)param_1[0x7a];
    uVar3 = ValidateVictimRoamCandidate(param_1,&local_48);
    if ((char)uVar3 == '\0') {
      return;
    }
    *(undefined1 *)(param_1 + 0x7b) = 0;
    goto LAB_0045b1b6;
  }
  iVar4 = (**(code **)(**(int **)(DAT_00503a78 + 0x30) + 0x18))();
  fStack_30 = *(float *)(iVar4 + 0x28);
  uStack_2c = *(undefined4 *)(iVar4 + 0x2c);
  uStack_28 = *(undefined4 *)(iVar4 + 0x30);
  uVar5 = FUN_00459be0(this,&fStack_30,432.0);
  if ((char)uVar5 == '\0') {
LAB_0045b0ad:
    pfVar7 = afStack_c;
    pvVar6 = (void *)FUN_0043b2d0(afStack_18,this,(float *)(param_1 + 0x75));
    pfVar7 = (float *)FUN_0045a230(pvVar6,pfVar7);
    pfVar7 = (float *)FUN_00447710(afStack_24,12.0,pfVar7);
    local_48 = *pfVar7;
    local_44 = pfVar7[1];
    local_40 = pfVar7[2];
  }
  else {
    pfVar7 = &fStack_30;
    pvVar6 = (void *)(**(code **)(*param_1 + 0x18))();
    fVar8 = (float10)FUN_00459c70(pvVar6,pfVar7);
    if (((float10)_DAT_004e1c1c <= fVar8) ||
       (cVar1 = FUN_00490130(this,&fStack_30,'\0'), cVar1 == '\0')) goto LAB_0045b0ad;
    pfVar7 = afStack_24;
    pvVar6 = (void *)FUN_0043b2d0(afStack_18,&fStack_30,this);
    pfVar7 = (float *)FUN_0045a230(pvVar6,pfVar7);
    pfVar7 = (float *)FUN_00447710(afStack_c,12.0,pfVar7);
    local_48 = *pfVar7;
    local_44 = pfVar7[1];
    local_40 = pfVar7[2];
    FUN_0043b2d0(afStack_c,&fStack_30,this);
    fVar8 = (float10)FUN_00441e70();
    if (((*(int **)(DAT_00503a78 + 0x30))[0xd4d] != 0) || ((float)fVar8 < _DAT_004e2324)) {
LAB_0045b089:
      pfVar7 = (float *)FUN_0042d2b0(&local_48,afStack_c);
      local_48 = *pfVar7;
      local_44 = pfVar7[1];
      local_40 = pfVar7[2];
    }
    else {
      pfVar7 = this;
      pvVar6 = (void *)(**(code **)(**(int **)(DAT_00503a78 + 0x30) + 0x18))();
      fVar9 = (float10)FUN_00459c70(pvVar6,pfVar7);
      if ((float10)_DAT_004e1c1c < fVar9) goto LAB_0045b089;
      if ((float)fVar8 < _DAT_004e2320) {
        FUN_00404b30(0xf);
      }
    }
  }
  if (local_44 * local_44 + local_48 * local_48 + local_40 * local_40 <= (float)_DAT_004e16b8) {
LAB_0045b1f0:
    param_1[0x78] = (int)local_3c;
    param_1[0x79] = (int)local_38;
    param_1[0x7a] = (int)local_34;
    *(undefined1 *)(param_1 + 0x7b) = 1;
    return;
  }
  local_44 = 0.0;
  uVar3 = ValidateVictimRoamCandidate(param_1,&local_48);
  if ((char)uVar3 == '\0') {
    FUN_00459c30(&local_48,2.0);
    pfVar7 = (float *)FUN_0045a190(&local_48,afStack_c,30.0);
    local_48 = *pfVar7;
    local_44 = pfVar7[1];
    local_40 = pfVar7[2];
    uVar3 = ValidateVictimRoamCandidate(param_1,&local_48);
    if ((char)uVar3 == '\0') {
      pfVar7 = (float *)FUN_0045a190(&local_48,afStack_c,-60.0);
      local_48 = *pfVar7;
      local_44 = pfVar7[1];
      local_40 = pfVar7[2];
      uVar3 = ValidateVictimRoamCandidate(param_1,&local_48);
      if ((char)uVar3 == '\0') goto LAB_0045b1f0;
    }
  }
LAB_0045b1b6:
  param_1[0x75] = (int)*this;
  param_1[0x76] = param_1[0x73];
  param_1[0x77] = param_1[0x74];
  *this = local_48 + *this;
  param_1[0x73] = (int)(local_44 + (float)param_1[0x73]);
  param_1[0x74] = (int)(local_40 + (float)param_1[0x74]);
  return;
}
