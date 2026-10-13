/* Fresh Ghidra decompilation: RotateTransformBasisAboutYDegrees at 0x00496c30;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   The helper takes degrees and rotates the first and third transform basis vectors
   about Y. _DAT_004e22bc is the float degrees-to-radians factor 0.0174532924. Cross-
   product and normalization helpers rebuild an orthonormal basis afterward.
   */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __thiscall RotateTransformBasisAboutYDegrees(void *this,float param_1)

{
  float *pfVar1;
  void *pvVar2;
  float *pfVar3;
  float *pfVar4;
  float10 fVar5;
  float10 fVar6;
  float *pfVar7;
  float local_18;
  float local_14;
  float local_10;
  float local_c [3];

  pfVar4 = (float *)((int)this + 0x1c);
  pfVar1 = (float *)((int)this + 4);
  fVar5 = (float10)fcos((float10)param_1 * (float10)_DAT_004e22bc);
  fVar6 = (float10)fsin((float10)param_1 * (float10)_DAT_004e22bc);
  local_18 = (float)(fVar5 * (float10)*pfVar1 + fVar6 * (float10)*pfVar4);
  local_14 = (float)(fVar5 * (float10)*(float *)((int)this + 8) +
                    fVar6 * (float10)*(float *)((int)this + 0x20));
  *pfVar1 = local_18;
  *(float *)((int)this + 8) = local_14;
  local_10 = (float)(fVar5 * (float10)*(float *)((int)this + 0xc) +
                    fVar6 * (float10)*(float *)((int)this + 0x24));
  *(float *)((int)this + 0xc) = local_10;
  pfVar7 = &local_18;
  pvVar2 = (void *)FUN_004476b0(local_c,pfVar1,(float *)((int)this + 0x10));
  pfVar3 = (float *)FUN_0045a230(pvVar2,pfVar7);
  *pfVar4 = *pfVar3;
  *(float *)((int)this + 0x20) = pfVar3[1];
  pfVar7 = local_c;
  *(float *)((int)this + 0x24) = pfVar3[2];
  pvVar2 = (void *)FUN_004476b0(&local_18,(float *)((int)this + 0x10),pfVar4);
  pfVar4 = (float *)FUN_0045a230(pvVar2,pfVar7);
  *pfVar1 = *pfVar4;
  *(float *)((int)this + 8) = pfVar4[1];
  *(float *)((int)this + 0xc) = pfVar4[2];
  return;
}
