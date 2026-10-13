/* Fresh Ghidra decompilation: ComputeVectorAngleDegrees at 0x00497aa0;
   FDNYFirefighter.exe SHA-256 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e.

   The corrected Ghidra prototype models ECX (`this`) as the first vector and the
   stack argument as the second. The instructions compute dot/(lengthA*lengthB), apply
   acos, then multiply by 57.2957795.
   */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

float __thiscall ComputeVectorAngleDegrees(void *this,float *other)

{
  float10 fVar1;

  fVar1 = (float10)FUN_004b96e0();
  return fVar1 * (float10)_DAT_004e2330;
}
