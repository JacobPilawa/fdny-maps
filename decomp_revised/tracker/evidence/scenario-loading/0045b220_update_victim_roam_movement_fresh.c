/* Fresh Ghidra decompilation: UpdateVictimRoamMovement at 0x0045b220; FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e. */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Per-update victim state-7 handler. Calls the roam-target updater, turns toward the stored target
   when the direction exceeds the 36-squared threshold, and advances the victim position using the
   movement scalar at +0x228. */

void __fastcall UpdateVictimRoamMovement(int *param_1)

{
  float *pfVar1;
  float *pfVar2;
  float10 fVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  undefined4 uVar7;
  float local_60;
  float local_5c;
  float local_58;
  float local_54;
  float local_50 [3];
  undefined1 local_44 [68];

  if (*(char *)(DAT_00502aac + 0x6a9) != '\0') {
    uVar7 = 0x40a00000;
    iVar6 = 0xff;
    iVar5 = 0;
    iVar4 = 0;
    local_5c = 0.0;
    local_58 = 24.0;
    local_54 = 0.0;
    pfVar1 = (float *)FUN_0042d050(local_50,(float *)(param_1 + 0x72),&local_5c);
    FUN_00492ee0((float *)(param_1 + 0x72),pfVar1,iVar4,iVar5,iVar6,uVar7);
  }
  UpdateVictimRoamTarget(param_1);
  pfVar1 = (float *)(param_1 + 0x13);
  FUN_0043b2d0(&local_5c,(float *)(param_1 + 0x72),pfVar1);
  param_1[0x73] = param_1[0x14];
  if (_DAT_004e2338 < local_54 * local_54 + local_5c * local_5c + local_58 * local_58) {
    fVar3 = (float10)fpatan(-(float10)local_5c,(float10)local_54);
    local_60 = (float)(fVar3 * (float10)_DAT_004e2330);
    if ((float10)_DAT_004e1cbc < fVar3 * (float10)_DAT_004e2330 - (float10)(float)param_1[0x5b]) {
      local_60 = local_60 - _DAT_004e1c24;
    }
    if (_DAT_004e1cbc < (float)param_1[0x5b] - local_60) {
      local_60 = local_60 + _DAT_004e1c24;
    }
    fVar3 = FUN_00482aa0((float)param_1[0x5b],local_60);
    param_1[0x5b] = (int)(float)fVar3;
    FUN_0042d350(param_1 + 9,local_44);
    FUN_00496c30(param_1 + 9,(float)param_1[0x5b]);
  }
  pfVar2 = FUN_00483580(local_50,pfVar1,(float *)(param_1 + 0x72),(float)param_1[0x8a]);
  *pfVar1 = *pfVar2;
  param_1[0x14] = (int)pfVar2[1];
  param_1[0x15] = (int)pfVar2[2];
  return;
}
