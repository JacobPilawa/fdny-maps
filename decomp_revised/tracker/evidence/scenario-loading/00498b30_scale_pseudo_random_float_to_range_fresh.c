
/* Summary: Scales a pseudo-random unit float into a caller-supplied range.

   Algorithm: Computes (upper - lower) * NextPseudoRandomUnitFloat() + lower.

   Parameters: lower and upper bounds.

   Returns: A floating-point value in the requested interval. */

float10 __cdecl ScalePseudoRandomFloatToRange(float param_1,float param_2)

{
  float10 fVar1;

  fVar1 = GetPseudoRandomUnitFloat();
  return ((float10)param_2 - (float10)param_1) * fVar1 + (float10)param_1;
}
