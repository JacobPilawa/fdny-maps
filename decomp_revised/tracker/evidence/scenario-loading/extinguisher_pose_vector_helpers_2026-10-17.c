/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x0042d2b0 FUN_0042d2b0 */

void __thiscall FUN_0042d2b0(void *this,float *param_1)

{
  float fVar1;
  float fVar2;
  
  fVar1 = *(float *)((int)this + 8);
  fVar2 = *(float *)((int)this + 4);
  *param_1 = -*(float *)this;
  param_1[1] = -fVar2;
  param_1[2] = -fVar1;
  return;
}



/* 0x0042d280 FUN_0042d280 */

void __thiscall
FUN_0042d280(void *this,undefined4 *param_1,undefined4 param_2,undefined4 param_3,undefined4 param_4
            )

{
  *(undefined4 *)this = param_2;
  *(undefined4 *)((int)this + 8) = param_4;
  *(undefined4 *)((int)this + 4) = param_3;
  *param_1 = *(undefined4 *)this;
  param_1[1] = *(undefined4 *)((int)this + 4);
  param_1[2] = *(undefined4 *)((int)this + 8);
  return;
}



/* 0x00446390 FUN_00446390 */

void __cdecl FUN_00446390(float *param_1,float *param_2,float param_3)

{
  float fVar1;
  float fVar2;
  
  fVar1 = param_2[1];
  fVar2 = param_2[2];
  *param_1 = *param_2 * param_3;
  param_1[1] = fVar1 * param_3;
  param_1[2] = fVar2 * param_3;
  return;
}



/* 0x004476b0 FUN_004476b0 */

void __cdecl FUN_004476b0(float *param_1,float *param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  
  fVar1 = param_2[2];
  fVar2 = *param_3;
  fVar3 = param_3[2];
  fVar4 = *param_2;
  fVar5 = *param_2;
  fVar6 = param_3[1];
  fVar7 = *param_3;
  fVar8 = param_2[1];
  *param_1 = param_3[2] * param_2[1] - param_2[2] * param_3[1];
  param_1[1] = fVar1 * fVar2 - fVar3 * fVar4;
  param_1[2] = fVar5 * fVar6 - fVar7 * fVar8;
  return;
}



/* 0x004964b0 FUN_004964b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_004964b0(int param_1)

{
  void *pvVar1;
  float *pfVar2;
  float *pfVar3;
  float *pfVar4;
  float *pfVar5;
  float *pfVar6;
  float local_18 [3];
  float local_c [3];
  
  pfVar4 = (float *)(param_1 + 0x10);
  pfVar5 = (float *)(param_1 + 0x1c);
  if (ABS(*pfVar4 * *pfVar5 +
          *(float *)(param_1 + 0x14) * *(float *)(param_1 + 0x20) +
          *(float *)(param_1 + 0x18) * *(float *)(param_1 + 0x24)) <= (float)_DAT_004e17e0) {
    pfVar6 = local_18;
    pvVar1 = (void *)FUN_004476b0(local_c,pfVar4,pfVar5);
    pfVar2 = (float *)FUN_0045a230(pvVar1,pfVar6);
    pfVar6 = (float *)(param_1 + 4);
    *pfVar6 = *pfVar2;
    *(float *)(param_1 + 8) = pfVar2[1];
    pfVar3 = local_c;
    *(float *)(param_1 + 0xc) = pfVar2[2];
    pvVar1 = (void *)FUN_004476b0(local_18,pfVar5,pfVar6);
    pfVar3 = (float *)FUN_0045a230(pvVar1,pfVar3);
    *pfVar4 = *pfVar3;
    *(float *)(param_1 + 0x14) = pfVar3[1];
    *(float *)(param_1 + 0x18) = pfVar3[2];
    pfVar3 = local_c;
    pvVar1 = (void *)FUN_004476b0(local_18,pfVar6,pfVar4);
    pfVar4 = (float *)FUN_0045a230(pvVar1,pfVar3);
    *pfVar5 = *pfVar4;
    *(float *)(param_1 + 0x20) = pfVar4[1];
    *(float *)(param_1 + 0x24) = pfVar4[2];
    return;
  }
  pfVar6 = (float *)(param_1 + 4);
  if (ABS(*pfVar5 * *pfVar6 +
          *(float *)(param_1 + 0x20) * *(float *)(param_1 + 8) +
          *(float *)(param_1 + 0x24) * *(float *)(param_1 + 0xc)) <= (float)_DAT_004e17e0) {
    pfVar3 = local_c;
    pvVar1 = (void *)FUN_004476b0(local_18,pfVar5,pfVar6);
    pfVar3 = (float *)FUN_0045a230(pvVar1,pfVar3);
    *pfVar4 = *pfVar3;
    *(float *)(param_1 + 0x14) = pfVar3[1];
    *(float *)(param_1 + 0x18) = pfVar3[2];
    pfVar3 = local_c;
    pvVar1 = (void *)FUN_004476b0(local_18,pfVar6,pfVar4);
    pfVar3 = (float *)FUN_0045a230(pvVar1,pfVar3);
    *pfVar5 = *pfVar3;
    *(float *)(param_1 + 0x20) = pfVar3[1];
    *(float *)(param_1 + 0x24) = pfVar3[2];
    pfVar3 = local_c;
    pvVar1 = (void *)FUN_004476b0(local_18,pfVar4,pfVar5);
    pfVar4 = (float *)FUN_0045a230(pvVar1,pfVar3);
    *pfVar6 = *pfVar4;
    *(float *)(param_1 + 8) = pfVar4[1];
    *(float *)(param_1 + 0xc) = pfVar4[2];
    return;
  }
  if ((float)_DAT_004e17e0 <
      ABS(*pfVar6 * *pfVar4 +
          *(float *)(param_1 + 8) * *(float *)(param_1 + 0x14) +
          *(float *)(param_1 + 0xc) * *(float *)(param_1 + 0x18))) {
    FUN_00495c70(param_1);
  }
  pfVar3 = local_c;
  pvVar1 = (void *)FUN_004476b0(local_18,pfVar6,pfVar4);
  pfVar3 = (float *)FUN_0045a230(pvVar1,pfVar3);
  *pfVar5 = *pfVar3;
  *(float *)(param_1 + 0x20) = pfVar3[1];
  *(float *)(param_1 + 0x24) = pfVar3[2];
  pfVar3 = local_c;
  pvVar1 = (void *)FUN_004476b0(local_18,pfVar4,pfVar5);
  pfVar3 = (float *)FUN_0045a230(pvVar1,pfVar3);
  *pfVar6 = *pfVar3;
  *(float *)(param_1 + 8) = pfVar3[1];
  *(float *)(param_1 + 0xc) = pfVar3[2];
  pfVar3 = local_c;
  pvVar1 = (void *)FUN_004476b0(local_18,pfVar5,pfVar6);
  pfVar5 = (float *)FUN_0045a230(pvVar1,pfVar3);
  *pfVar4 = *pfVar5;
  *(float *)(param_1 + 0x14) = pfVar5[1];
  *(float *)(param_1 + 0x18) = pfVar5[2];
  return;
}



