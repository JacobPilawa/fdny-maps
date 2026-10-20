/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00496dd0 FUN_00496dd0 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall FUN_00496dd0(void *this,float param_1,float param_2,float param_3,float param_4)

{
  float fVar1;
  float *pfVar2;
  float *pfVar3;
  float *pfVar4;
  float *pfVar5;
  float *pfVar6;
  float10 fVar7;
  float10 fVar8;
  float local_78;
  float local_74;
  float local_70;
  float local_6c [3];
  float local_60 [3];
  float local_54 [3];
  float local_48 [3];
  float local_3c [3];
  float local_30 [3];
  float local_24 [3];
  float local_18 [3];
  float local_c [3];
  
  fVar7 = (float10)param_4 * (float10)_DAT_004e22bc * (float10)_DAT_004e1fd4;
  fVar8 = (float10)fcos(fVar7);
  fVar1 = (float)fVar8;
  pfVar2 = (float *)FUN_0045a230(&param_1,local_6c);
  fVar7 = (float10)fsin((float10)(float)fVar7);
  FUN_00447710(&local_78,(float)fVar7,pfVar2);
  pfVar2 = (float *)((int)this + 4);
  if (ABS(param_1 * *pfVar2 +
          param_3 * *(float *)((int)this + 0xc) + param_2 * *(float *)((int)this + 8)) <=
      (float)_DAT_004e17e0) {
    pfVar3 = (float *)FUN_004476b0(local_6c,&local_78,pfVar2);
    pfVar3 = (float *)FUN_004476b0(local_60,&local_78,pfVar3);
    pfVar4 = (float *)FUN_004476b0(local_54,&local_78,pfVar2);
    pfVar4 = (float *)FUN_00447710(local_48,fVar1 + fVar1,pfVar4);
    pfVar5 = (float *)FUN_00446390(local_3c,&local_78,
                                   local_78 * *pfVar2 +
                                   local_70 * *(float *)((int)this + 0xc) +
                                   local_74 * *(float *)((int)this + 8));
    pfVar6 = (float *)FUN_00447710(local_30,fVar1 * fVar1,pfVar2);
    pfVar5 = (float *)FUN_0042d050(local_24,pfVar6,pfVar5);
    pfVar4 = (float *)FUN_0042d050(local_18,pfVar5,pfVar4);
    pfVar3 = (float *)FUN_0042d050(local_c,pfVar4,pfVar3);
    *pfVar2 = *pfVar3;
    *(float *)((int)this + 8) = pfVar3[1];
    *(float *)((int)this + 0xc) = pfVar3[2];
  }
  pfVar2 = (float *)((int)this + 0x10);
  if (ABS(param_1 * *pfVar2 +
          param_3 * *(float *)((int)this + 0x18) + param_2 * *(float *)((int)this + 0x14)) <=
      (float)_DAT_004e17e0) {
    pfVar3 = (float *)FUN_004476b0(local_c,&local_78,pfVar2);
    pfVar3 = (float *)FUN_004476b0(local_18,&local_78,pfVar3);
    pfVar4 = (float *)FUN_004476b0(local_24,&local_78,pfVar2);
    pfVar4 = (float *)FUN_00447710(local_30,fVar1 + fVar1,pfVar4);
    pfVar5 = (float *)FUN_00446390(local_3c,&local_78,
                                   local_78 * *pfVar2 +
                                   local_70 * *(float *)((int)this + 0x18) +
                                   local_74 * *(float *)((int)this + 0x14));
    pfVar6 = (float *)FUN_00447710(local_48,fVar1 * fVar1,pfVar2);
    pfVar5 = (float *)FUN_0042d050(local_54,pfVar6,pfVar5);
    pfVar4 = (float *)FUN_0042d050(local_60,pfVar5,pfVar4);
    pfVar3 = (float *)FUN_0042d050(local_6c,pfVar4,pfVar3);
    *pfVar2 = *pfVar3;
    *(float *)((int)this + 0x14) = pfVar3[1];
    *(float *)((int)this + 0x18) = pfVar3[2];
  }
  pfVar2 = (float *)((int)this + 0x1c);
  if (ABS(param_1 * *pfVar2 +
          param_3 * *(float *)((int)this + 0x24) + param_2 * *(float *)((int)this + 0x20)) <=
      (float)_DAT_004e17e0) {
    pfVar3 = (float *)FUN_004476b0(local_c,&local_78,pfVar2);
    pfVar3 = (float *)FUN_004476b0(local_18,&local_78,pfVar3);
    pfVar4 = (float *)FUN_004476b0(local_24,&local_78,pfVar2);
    pfVar4 = (float *)FUN_00447710(local_30,fVar1 + fVar1,pfVar4);
    pfVar5 = (float *)FUN_00446390(local_3c,&local_78,
                                   local_78 * *pfVar2 +
                                   local_70 * *(float *)((int)this + 0x24) +
                                   local_74 * *(float *)((int)this + 0x20));
    pfVar6 = (float *)FUN_00447710(local_48,fVar1 * fVar1,pfVar2);
    pfVar5 = (float *)FUN_0042d050(local_54,pfVar6,pfVar5);
    pfVar4 = (float *)FUN_0042d050(local_60,pfVar5,pfVar4);
    pfVar3 = (float *)FUN_0042d050(local_6c,pfVar4,pfVar3);
    *pfVar2 = *pfVar3;
    *(float *)((int)this + 0x20) = pfVar3[1];
    *(float *)((int)this + 0x24) = pfVar3[2];
  }
  return;
}



