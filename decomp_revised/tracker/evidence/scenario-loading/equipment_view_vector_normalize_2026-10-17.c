/* Ghidra pseudocode export; names and types are provisional. */
/* Program: FDNYFirefighter.exe */

/* Input SHA-256: 4b3b4dc1cc0b05bcb42c8e27ac6f6ca72fe5f7064ca8122fda7d41e7f6d2fc3e */

/* 0x00459b80 FUN_00459b80 */

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __fastcall FUN_00459b80(float *param_1)

{
  float fVar1;
  
  fVar1 = SQRT(param_1[2] * param_1[2] + param_1[1] * param_1[1] + *param_1 * *param_1);
  if ((fVar1 < (float)_DAT_004e0b68) && ((float)_DAT_004e0b60 < fVar1)) {
    return;
  }
  fVar1 = _DAT_004dfc24 / fVar1;
  *param_1 = fVar1 * *param_1;
  param_1[1] = fVar1 * param_1[1];
  param_1[2] = fVar1 * param_1[2];
  return;
}



