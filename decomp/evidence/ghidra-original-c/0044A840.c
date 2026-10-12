/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0044a840
 * Match: function entry
 * Entry: 0044a840 FUN_0044a840
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0044a840(void *this,undefined4 *param_1)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float *pfVar7;
  int iVar8;
  float *pfVar9;
  float *pfVar10;
  float10 fVar11;
  float10 fVar12;
  float10 fVar13;
  float10 fVar14;
  float10 fVar15;
  float10 fVar16;
  float10 fVar17;
  float10 fVar18;
  double local_54;
  float local_48;
  undefined4 local_44;
  undefined4 local_40;
  undefined4 local_3c;
  undefined4 local_38;
  undefined4 local_34;
  undefined4 local_30;
  undefined4 local_2c;
  undefined4 local_28;
  undefined4 local_24;
  undefined4 local_20;
  undefined4 local_1c;
  float local_18 [3];
  float local_c [3];
  
  fVar11 = (float10)fcos((float10)_DAT_004e1808);
  iVar8 = 6;
  fVar12 = (float10)fsin((float10)_DAT_004e1808);
  fVar13 = (float10)fcos((float10)_DAT_004e1800);
  fVar14 = (float10)fsin((float10)_DAT_004e1800);
  fVar15 = (float10)fcos((float10)_DAT_004e17f8);
  fVar16 = (float10)fsin((float10)_DAT_004e17f8);
  fVar17 = (float10)fcos((float10)_DAT_004e17f0);
  fVar18 = (float10)fsin((float10)_DAT_004e17f0);
  pfVar9 = (float *)&stack0xffffff58;
  do {
    pfVar10 = pfVar9 + 3;
    *pfVar10 = *(float *)((int)this + 0x44) * *pfVar10 * (float)_DAT_004e17e8;
    pfVar9[5] = pfVar9[5] * *(float *)((int)this + 0x4c) * (float)_DAT_004e17e8;
    pfVar7 = (float *)FUN_00495d20((float *)&local_54,(int)this,pfVar10);
    *pfVar10 = *pfVar7;
    pfVar9[4] = pfVar7[1];
    iVar8 = iVar8 + -1;
    pfVar9[5] = pfVar7[2];
    pfVar9 = pfVar10;
  } while (iVar8 != 0);
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x144) = 0x3f800000;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x148) = 0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x14c) = 0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x120) = 0xbf800000;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x124) = 0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x128) = 0;
  *(float *)(*(int *)((int)this + 100) + 0x144) = (float)fVar11;
  *(undefined4 *)(*(int *)((int)this + 100) + 0x148) = 0;
  *(float *)(*(int *)((int)this + 100) + 0x14c) = (float)fVar12;
  *(float *)(*(int *)((int)this + 100) + 0x120) = (float)fVar13;
  *(undefined4 *)(*(int *)((int)this + 100) + 0x124) = 0;
  *(float *)(*(int *)((int)this + 100) + 0x128) = (float)fVar14;
  *(float *)(*(int *)((int)this + 0x68) + 0x144) = (float)fVar15;
  *(undefined4 *)(*(int *)((int)this + 0x68) + 0x148) = 0;
  *(float *)(*(int *)((int)this + 0x68) + 0x14c) = (float)fVar16;
  *(float *)(*(int *)((int)this + 0x68) + 0x120) = (float)fVar17;
  *(undefined4 *)(*(int *)((int)this + 0x68) + 0x124) = 0;
  *(float *)(*(int *)((int)this + 0x68) + 0x128) = (float)fVar18;
  fVar1 = *(float *)((int)this + 0x48);
  fVar2 = *(float *)((int)this + 0x48);
  fVar3 = *(float *)((int)this + 0x48);
  fVar4 = *(float *)((int)this + 0x48);
  fVar5 = *(float *)((int)this + 0x48);
  fVar6 = *(float *)((int)this + 0x48);
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0xd8) = 0x3f800000;
  *(float *)(*(int *)((int)this + 0x60) + 0xdc) = fVar1 + 0.0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0xe0) = 0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0xfc) = 0xbf800000;
  *(float *)(*(int *)((int)this + 0x60) + 0x100) = fVar2 + 0.0;
  *(undefined4 *)(*(int *)((int)this + 0x60) + 0x104) = 0;
  *(float *)(*(int *)((int)this + 100) + 0xd8) = (float)fVar11;
  *(float *)(*(int *)((int)this + 100) + 0xdc) = fVar3 + 0.0;
  *(float *)(*(int *)((int)this + 100) + 0xe0) = (float)fVar12;
  *(float *)(*(int *)((int)this + 100) + 0xfc) = (float)fVar13;
  *(float *)(*(int *)((int)this + 100) + 0x100) = fVar4 + 0.0;
  *(float *)(*(int *)((int)this + 100) + 0x104) = (float)fVar14;
  *(float *)(*(int *)((int)this + 0x68) + 0xd8) = (float)fVar15;
  *(float *)(*(int *)((int)this + 0x68) + 0xdc) = fVar5 + 0.0;
  *(float *)(*(int *)((int)this + 0x68) + 0xe0) = (float)fVar16;
  *(float *)(*(int *)((int)this + 0x68) + 0xfc) = (float)fVar17;
  *(float *)(*(int *)((int)this + 0x68) + 0x100) = fVar6 + 0.0;
  *(float *)(*(int *)((int)this + 0x68) + 0x104) = (float)fVar18;
  local_48 = (float)param_1[3];
  local_44 = param_1[1];
  local_40 = param_1[5];
  local_3c = param_1[3];
  local_38 = param_1[1];
  local_34 = param_1[2];
  local_30 = *param_1;
  local_2c = param_1[1];
  local_28 = param_1[2];
  local_24 = *param_1;
  local_20 = param_1[1];
  local_1c = param_1[5];
  if ((DAT_00502c3c & 1) == 0) {
    DAT_00502c3c = DAT_00502c3c | 1;
    _DAT_00502c30 = 0.0;
    DAT_00502c34 = 0.0;
    _DAT_00502c38 = 0.0;
    _atexit((_func_4879 *)&DAT_004db730);
  }
  local_54._0_4_ = 1e+12;
  iVar8 = 0;
  pfVar9 = &local_48;
  do {
    pfVar7 = (float *)FUN_0043b2d0(local_c,pfVar9,(float *)((int)this + 0x28));
    pfVar7 = (float *)FUN_00495eb0(local_18,(int)this,pfVar7);
    _DAT_00502c30 = *pfVar7;
    DAT_00502c34 = pfVar7[1];
    _DAT_00502c38 = pfVar7[2];
    *(float *)(*(int *)((int)this + 0x6c) + 0xd8 + iVar8) =
         _DAT_00502c30 + *(float *)((int)this + 0x28);
    *(float *)(*(int *)((int)this + 0x6c) + 0xdc + iVar8) =
         DAT_00502c34 + *(float *)((int)this + 0x2c);
    *(float *)(*(int *)((int)this + 0x6c) + 0xe0 + iVar8) =
         _DAT_00502c38 + *(float *)((int)this + 0x30);
    if (DAT_00502c34 < local_54._0_4_) {
      local_54._0_4_ = DAT_00502c34;
    }
    iVar8 = iVar8 + 0x24;
    pfVar9 = pfVar9 + 3;
  } while (iVar8 < 0x90);
  if (local_54._0_4_ < _DAT_004dc50c) {
    fVar1 = local_54._0_4_ + local_54._0_4_;
    *(float *)(*(int *)((int)this + 0x60) + 0xb8) =
         *(float *)(*(int *)((int)this + 0x60) + 0xb8) - fVar1;
    *(float *)(*(int *)((int)this + 100) + 0xb8) =
         *(float *)(*(int *)((int)this + 100) + 0xb8) - fVar1;
    *(float *)(*(int *)((int)this + 0x68) + 0xb8) =
         *(float *)(*(int *)((int)this + 0x68) + 0xb8) - fVar1;
    iVar8 = FUN_0047f1a0(*(int *)((int)this + 0x60));
    fVar1 = local_54._0_4_ * (float)_DAT_004de6f8;
    local_54 = (double)fVar1;
    *(float *)(iVar8 + 0x2c) = *(float *)(iVar8 + 0x2c) + fVar1;
    iVar8 = FUN_0047f1a0(*(int *)((int)this + 100));
    *(float *)(iVar8 + 0x2c) = *(float *)(iVar8 + 0x2c) + (float)local_54;
    iVar8 = FUN_0047f1a0(*(int *)((int)this + 0x68));
    *(float *)(iVar8 + 0x2c) = *(float *)(iVar8 + 0x2c) + (float)local_54;
  }
  return;
}


