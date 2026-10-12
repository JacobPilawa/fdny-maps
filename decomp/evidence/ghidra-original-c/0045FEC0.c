/* Read-only Ghidra C decompilation.
 * Program: FDNYFirefighter.exe
 * Requested address: 0045fec0
 * Match: function entry
 * Entry: 0045fec0 FUN_0045fec0
 * Generated from the preserved Ghidra database; see tools/ghidra/README.md.
 */


/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_0045fec0(void *this,float param_1)

{
  undefined4 *puVar1;
  int iVar2;
  float *pfVar3;
  float10 fVar4;
  float10 fVar5;
  float local_30 [7];
  char *local_14;
  char *local_10;
  undefined4 local_c;
  
  param_1 = DAT_00502b04 * param_1 + *(float *)((int)this + 0x33c);
  if ((float)_DAT_004dd808 < param_1) {
    param_1 = 0.0;
    *(undefined4 *)((int)this + 0x368) = 0;
  }
  FUN_0045fb90(this,param_1);
  if (*(int *)((int)this + 0x2f0) < 1) {
    pfVar3 = local_30 + 6;
    for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
      *pfVar3 = 0.0;
      pfVar3 = pfVar3 + 1;
    }
    local_30[6] = 1.4013e-45;
    local_14 = "Array index out of bounds";
    local_10 = "..\\BluelAPI/Container/SArray.h";
    local_c = 0x1cd;
    FUN_00422060((int *)(local_30 + 6));
  }
  iVar2 = FUN_0047f1a0(**(int **)((int)this + 0x2ec));
  *(undefined4 *)(iVar2 + 0x28) = *(undefined4 *)((int)this + 0x344);
  *(undefined4 *)(iVar2 + 0x2c) = *(undefined4 *)((int)this + 0x348);
  *(undefined4 *)(iVar2 + 0x30) = *(undefined4 *)((int)this + 0x34c);
  if (*(int *)((int)this + 0x2f0) < 2) {
    pfVar3 = local_30 + 6;
    for (iVar2 = 6; iVar2 != 0; iVar2 = iVar2 + -1) {
      *pfVar3 = 0.0;
      pfVar3 = pfVar3 + 1;
    }
    local_30[6] = 1.4013e-45;
    local_14 = "Array index out of bounds";
    local_10 = "..\\BluelAPI/Container/SArray.h";
    local_c = 0x1cd;
    FUN_00422060((int *)(local_30 + 6));
  }
  iVar2 = FUN_0047f1a0(*(int *)(*(int *)((int)this + 0x2ec) + 4));
  *(undefined4 *)(iVar2 + 0x28) = *(undefined4 *)((int)this + 0x350);
  *(undefined4 *)(iVar2 + 0x2c) = *(undefined4 *)((int)this + 0x354);
  *(undefined4 *)(iVar2 + 0x30) = *(undefined4 *)((int)this + 0x358);
  local_30[0] = 25.0;
  local_30[1] = -100.0;
  local_30[2] = 0.0;
  fVar4 = (float10)*(float *)((int)this + 0x33c) * (float10)_DAT_004e16c4 * (float10)_DAT_004e27e8;
  local_30[3] = 12.5;
  local_30[4] = -7.5;
  local_30[5] = 10.0;
  fVar5 = (float10)fsin(fVar4);
  param_1 = (float)fVar5;
  if (fVar4 <= (float10)_DAT_004e27e0) {
    if (((float10)_DAT_004e1b30 < fVar4) || (fVar4 <= (float10)_DAT_004e1840)) {
      param_1 = param_1 * (float)_DAT_004e27d8;
    }
  }
  else {
    param_1 = 0.0;
  }
  puVar1 = (undefined4 *)FUN_00447710(local_30 + 6,param_1,local_30);
  *(undefined4 *)((int)this + 0x1f0) = *puVar1;
  *(undefined4 *)((int)this + 500) = puVar1[1];
  *(undefined4 *)((int)this + 0x1f8) = puVar1[2];
  puVar1 = (undefined4 *)FUN_00447710(local_30 + 6,param_1,local_30 + 3);
  *(undefined4 *)((int)this + 0x1e4) = *puVar1;
  *(undefined4 *)((int)this + 0x1e8) = puVar1[1];
  *(undefined4 *)((int)this + 0x1ec) = puVar1[2];
  return;
}


