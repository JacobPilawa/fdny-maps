/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x004968b0 FUN_004968b0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_004968b0(void *this,float param_1)

{
  float *pfVar1;
  float fVar2;
  float fVar3;
  float10 fVar4;
  float fVar5;
  float *pfVar6;
  void *pvVar7;
  float *pfVar8;
  float10 fVar9;
  float10 fVar10;
  float local_18;
  float local_14;
  float local_10;
  float local_c [3];
  
  pfVar1 = (float *)((int)this + 4);
  fVar9 = (float10)fcos((float10)param_1 * (float10)_DAT_004e22bc);
  pfVar8 = (float *)((int)this + 0x10);
  fVar10 = (float10)fsin((float10)param_1 * (float10)_DAT_004e22bc);
  local_c[0] = (float)-fVar10;
  fVar5 = (*(float *)((int)this + 0xc) + *pfVar1) * _DAT_004dc50c;
  fVar2 = *pfVar1;
  fVar3 = *(float *)((int)this + 8);
  fVar4 = (float10)_DAT_004dc50c;
  local_18 = (float)((float10)*(float *)((int)this + 0x14) * (float10)_DAT_004dc50c +
                    fVar9 * (float10)*pfVar8 + fVar10 * (float10)*(float *)((int)this + 0x18));
  local_14 = (*(float *)((int)this + 0x18) + *pfVar8) * _DAT_004dc50c + *(float *)((int)this + 0x14)
  ;
  *pfVar1 = (float)((float10)*(float *)((int)this + 8) * (float10)_DAT_004dc50c +
                   fVar9 * (float10)*pfVar1 + fVar10 * (float10)*(float *)((int)this + 0xc));
  fVar10 = (float10)_DAT_004dc50c;
  *(float *)((int)this + 8) = fVar5 + *(float *)((int)this + 8);
  *(float *)((int)this + 0xc) =
       (float)((float10)fVar3 * fVar4 +
              (float10)local_c[0] * (float10)fVar2 + fVar9 * (float10)*(float *)((int)this + 0xc));
  local_10 = (float)((float10)*(float *)((int)this + 0x14) * fVar10 +
                    (float10)local_c[0] * (float10)*pfVar8 +
                    fVar9 * (float10)*(float *)((int)this + 0x18));
  pfVar6 = (float *)FUN_0045a230(&local_18,local_c);
  *pfVar8 = *pfVar6;
  *(float *)((int)this + 0x14) = pfVar6[1];
  *(float *)((int)this + 0x18) = pfVar6[2];
  pfVar6 = local_c;
  pvVar7 = (void *)FUN_004476b0(&local_18,pfVar1,pfVar8);
  pfVar6 = (float *)FUN_0045a230(pvVar7,pfVar6);
  *(float *)((int)this + 0x1c) = *pfVar6;
  *(float *)((int)this + 0x20) = pfVar6[1];
  *(float *)((int)this + 0x24) = pfVar6[2];
  pfVar6 = local_c;
  pvVar7 = (void *)FUN_004476b0(&local_18,pfVar8,(float *)((int)this + 0x1c));
  pfVar8 = (float *)FUN_0045a230(pvVar7,pfVar6);
  *pfVar1 = *pfVar8;
  *(float *)((int)this + 8) = pfVar8[1];
  *(float *)((int)this + 0xc) = pfVar8[2];
  return;
}



/* 0x00496360 FUN_00496360 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00496360(int param_1)

{
  float *pfVar1;
  void *pvVar2;
  float *pfVar3;
  float *pfVar4;
  float *pfVar5;
  float local_c [3];
  
  pfVar1 = (float *)FUN_0045a230(&stack0x00000004,local_c);
  pfVar4 = (float *)(param_1 + 0x1c);
  *pfVar4 = *pfVar1;
  *(float *)(param_1 + 0x20) = pfVar1[1];
  *(float *)(param_1 + 0x24) = pfVar1[2];
  pfVar1 = (float *)(param_1 + 0x10);
  if (ABS(*pfVar4 * *pfVar1 +
          *(float *)(param_1 + 0x14) * *(float *)(param_1 + 0x20) +
          *(float *)(param_1 + 0x18) * *(float *)(param_1 + 0x24)) <= (float)_DAT_004e17e0) {
    pfVar3 = (float *)&stack0x00000004;
    pvVar2 = (void *)FUN_004476b0(local_c,pfVar1,pfVar4);
    pfVar3 = (float *)FUN_0045a230(pvVar2,pfVar3);
    *(float *)(param_1 + 4) = *pfVar3;
    *(float *)(param_1 + 8) = pfVar3[1];
    *(float *)(param_1 + 0xc) = pfVar3[2];
    pfVar3 = (float *)&stack0x00000004;
    pvVar2 = (void *)FUN_004476b0(local_c,pfVar4,(float *)(param_1 + 4));
    pfVar4 = (float *)FUN_0045a230(pvVar2,pfVar3);
    *pfVar1 = *pfVar4;
    *(float *)(param_1 + 0x14) = pfVar4[1];
    *(float *)(param_1 + 0x18) = pfVar4[2];
    return;
  }
  pfVar3 = (float *)(param_1 + 4);
  if ((float)_DAT_004e17e0 <
      ABS(*pfVar4 * *pfVar3 +
          *(float *)(param_1 + 0x20) * *(float *)(param_1 + 8) +
          *(float *)(param_1 + 0x24) * *(float *)(param_1 + 0xc))) {
    FUN_00495c70(param_1);
  }
  pfVar5 = (float *)&stack0x00000004;
  pvVar2 = (void *)FUN_004476b0(local_c,pfVar4,pfVar3);
  pfVar5 = (float *)FUN_0045a230(pvVar2,pfVar5);
  *pfVar1 = *pfVar5;
  *(float *)(param_1 + 0x14) = pfVar5[1];
  *(float *)(param_1 + 0x18) = pfVar5[2];
  pfVar5 = (float *)&stack0x00000004;
  pvVar2 = (void *)FUN_004476b0(local_c,pfVar1,pfVar4);
  pfVar4 = (float *)FUN_0045a230(pvVar2,pfVar5);
  *pfVar3 = *pfVar4;
  *(float *)(param_1 + 8) = pfVar4[1];
  *(float *)(param_1 + 0xc) = pfVar4[2];
  return;
}



/* 0x00495eb0 FUN_00495eb0 */

void __cdecl FUN_00495eb0(float *param_1,int param_2,float *param_3)

{
  float fVar1;
  float fVar2;
  float fVar3;
  float fVar4;
  float fVar5;
  float fVar6;
  float fVar7;
  float fVar8;
  float fVar9;
  
  fVar7 = *(float *)(param_2 + 0x34) * *param_3;
  fVar5 = *(float *)(param_2 + 0xc);
  fVar9 = *(float *)(param_2 + 0x38) * param_3[1];
  fVar8 = *(float *)(param_2 + 0x3c) * param_3[2];
  fVar6 = *(float *)(param_2 + 8);
  fVar1 = *(float *)(param_2 + 0x14);
  fVar2 = *(float *)(param_2 + 0x20);
  fVar3 = *(float *)(param_2 + 0x18);
  fVar4 = *(float *)(param_2 + 0x24);
  *param_1 = *(float *)(param_2 + 0x10) * fVar9 +
             *(float *)(param_2 + 4) * fVar7 + *(float *)(param_2 + 0x1c) * fVar8;
  param_1[1] = fVar6 * fVar7 + fVar1 * fVar9 + fVar2 * fVar8;
  param_1[2] = fVar5 * fVar7 + fVar3 * fVar9 + fVar4 * fVar8;
  return;
}



