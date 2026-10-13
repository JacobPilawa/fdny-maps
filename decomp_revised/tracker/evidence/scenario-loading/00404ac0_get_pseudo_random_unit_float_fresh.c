
/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* Summary: Returns a pseudo-random unit float.

   Algorithm: Consumes a buffered random word (refilling through 00404960 when needed), applies
   xorshift steps, constructs a [1,2) float from 23 random bits, then subtracts 1.0.

   Parameters: None.

   Returns: A float in [0,1). */

float10 GetPseudoRandomUnitFloat(void)

{
  uint uVar1;

  DAT_004ff370 = DAT_004ff370 + -1;
  if (DAT_004ff370 < 0) {
    uVar1 = FUN_00404960();
  }
  else {
    uVar1 = *DAT_005023ec;
    DAT_005023ec = DAT_005023ec + 1;
    uVar1 = uVar1 ^ uVar1 >> 0xb;
    uVar1 = uVar1 ^ (uVar1 & 0xff3a58ad) << 7;
    uVar1 = uVar1 ^ (uVar1 & 0xffffdf8c) << 0xf;
    uVar1 = uVar1 >> 0x12 ^ uVar1;
  }
  return (float10)(float)(uVar1 & 0x7fffff | 0x3f800000) - (float10)_DAT_004dd808;
}
