/* Current Ghidra output. Return prototype normalized to float because manual float10 __thiscall typing introduced a false hidden-return parameter; see README and assembly trace. Native code returns through x87 ST0. FDNYFirefighter.exe SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Algorithm: Base radius is 0.5*(volume+0x44 + volume+0x4C), rejected when below 0.01. Add
   candidate padding, subtract volume origin at +0x28, and evaluate dx^2 + (2*dy)^2 + dz^2. Within
   the padded radius, return emitter(+0x60)+0xD0 divided by radius and multiplied by remaining
   radius after the square root.
   
   Parameters: this is the FireRescue volume; point is the candidate world point; padding is the
   candidate's extra radius.
   
   Returns: spatial influence/falloff, or zero outside the volume. The native instruction stream
   returns through x87 ST0. The MCP prototype is normalized to float because a manual float10
   __thiscall prototype inserted a false hidden return-storage parameter; use the assembly listing
   and preserved pre-normalization export for exact extended-precision handling. Shared by
   FireRescue exposure (0x004483A0) and caller 0x00463EF0. */

float __thiscall ComputeFireVolumeFalloff(void *this,float *point,float padding)

{
  float fVar1;
  float fVar2;
  float local_c;
  float local_8;
  float local_4;
  
  fVar2 = (*(float *)((int)this + 0x4c) + *(float *)((int)this + 0x44)) * (float)_DAT_004de6f8;
  if ((float)_DAT_004dfc78 <= fVar2) {
    fVar2 = fVar2 + padding;
    FUN_0043b2d0(&local_c,point,(float *)((int)this + 0x28));
    fVar1 = (local_8 + local_8) * (local_8 + local_8) + local_c * local_c + local_4 * local_4;
    if (fVar1 <= fVar2 * fVar2) {
      return (*(float *)(*(int *)((int)this + 0x60) + 0xd0) / fVar2) * (fVar2 - SQRT(fVar1));
    }
  }
  return _DAT_004dc50c;
}


