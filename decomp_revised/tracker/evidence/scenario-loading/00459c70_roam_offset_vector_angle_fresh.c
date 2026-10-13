/* Fresh Ghidra decompilation: FUN_00459c70 at 0x00459c70;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   The Ghidra signature currently reports void, but UpdateVictimRoamTarget consumes
   the x87 return value as an angle. This helper forms param_1 - (this + 0x28) and
   compares that offset vector with the reference at this + 0x1c through
   ComputeVectorAngleDegrees. The semantic name of that reference vector remains
   under investigation.
   */

float __thiscall MeasurePointOffsetAlignmentAngle(void *this,float *param_1)

{
  void *this_00;
  float fVar1;
  float *other;
  float local_c [3];

  other = (float *)((int)this + 0x1c);
  this_00 = (void *)FUN_0043b2d0(local_c,param_1,(float *)((int)this + 0x28));
  fVar1 = ComputeVectorAngleDegrees(this_00,other);
  return fVar1;
}
